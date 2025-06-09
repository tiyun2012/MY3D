# -*- coding: utf-8 -*-
import yfinance as yf
import pandas as pd
import numpy as np
import talib
from sklearn.preprocessing import MinMaxScaler
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM, Dense
import matplotlib.pyplot as plt
import os

# ====================
# 1. Fetch Gold Data
# ====================
def fetch_gold_data():
    print("Fetching gold data from Yahoo Finance...")
    try:
        # GC=F is Gold Futures, for spot gold try "XAU=USD" (may not always work)
        gold = yf.download("GC=F", start="2020-01-01", end="2025-06-01", progress=False)
        gold.to_csv("gold_data.csv")
        print(f"Data saved to {os.getcwd()}\\gold_data.csv")
        return gold
    except Exception as e:
        print(f"Error fetching data: {e}")
        return None

# ====================
# 2. Preprocess Data
# ====================
def preprocess_data(gold):
    print("Adding technical indicators...")
    gold['SMA_50'] = talib.SMA(gold['Close'], timeperiod=50)
    gold['RSI'] = talib.RSI(gold['Close'], timeperiod=14)
    gold['MACD'], _, _ = talib.MACD(gold['Close'], fastperiod=12, slowperiod=26, signalperiod=9)
    gold.dropna(inplace=True)
    gold.to_csv("gold_with_indicators.csv")
    return gold

# ====================
# 3. Build LSTM Model
# ====================
def build_model(gold):
    print("Building LSTM model...")
    scaler = MinMaxScaler()
    features = gold[['Close', 'SMA_50', 'RSI', 'MACD']]
    scaled_data = scaler.fit_transform(features)

    # Create sequences
    def create_sequences(data, window=60):
        X, y = [], []
        for i in range(window, len(data)):
            X.append(data[i-window:i])
            y.append(data[i, 0])  # Predict 'Close' price
        return np.array(X), np.array(y)

    X, y = create_sequences(scaled_data)
    split = int(0.8 * len(X))
    X_train, X_test = X[:split], X[split:]
    y_train, y_test = y[:split], y[split:]

    model = Sequential([
        LSTM(50, return_sequences=True, input_shape=(X_train.shape[1], X_train.shape[2])),
        LSTM(50),
        Dense(1)
    ])
    model.compile(optimizer='adam', loss='mse')
    history = model.fit(X_train, y_train, epochs=10, batch_size=32, 
                       validation_data=(X_test, y_test), verbose=1)
    return model, scaler, X_test

# ====================
# 4. Visualize Results
# ====================
def plot_results(gold, model, scaler, X_test):
    print("Generating predictions...")
    predictions = model.predict(X_test)
    # Inverse transform predictions
    predictions = scaler.inverse_transform(
        np.concatenate((predictions, np.zeros((len(predictions), 3))), axis=1))[:, 0]
    
    # Get corresponding dates
    test_dates = gold.index[-len(predictions):]
    
    plt.figure(figsize=(12, 6))
    plt.plot(test_dates, gold['Close'].values[-len(predictions):], label='Actual Price', color='blue')
    plt.plot(test_dates, predictions, label='Predicted Price', color='red', linestyle='--')
    plt.title("Gold Price Prediction (LSTM Model)")
    plt.xlabel("Date")
    plt.ylabel("Price (USD)")
    plt.legend()
    plt.grid()
    plt.savefig("gold_prediction.png")
    print(f"Plot saved to {os.getcwd()}\\gold_prediction.png")
    plt.show()

# ====================
# Main Execution
# ====================
if __name__ == "__main__":
    # Step 1: Fetch data
    gold = fetch_gold_data()
    if gold is not None:
        # Step 2: Preprocess
        gold = preprocess_data(gold)
        # Step 3: Build model
        model, scaler, X_test = build_model(gold)
        # Step 4: Visualize
        plot_results(gold, model, scaler, X_test)