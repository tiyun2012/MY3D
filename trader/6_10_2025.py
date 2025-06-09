import yfinance as yf
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from datetime import datetime, timedelta
import pytz

# Configuration
TICKER = "GC=F"  # Gold futures (most reliable)
PPI_RELEASE_TIME = "2025-06-10 12:30:00"  # Eastern Time
LOOKBACK_HOURS = 4
LOOKFORWARD_HOURS = 4

def get_market_data():
    eastern = pytz.timezone('America/New_York')
    ppi_time = eastern.localize(datetime.strptime(PPI_RELEASE_TIME, "%Y-%m-%d %H:%M:%S"))
    
    # Get 3 days of data to ensure coverage
    data = yf.download(
        TICKER,
        start=(ppi_time - timedelta(days=1)).strftime("%Y-%m-%d"),
        end=(ppi_time + timedelta(days=1)).strftime("%Y-%m-%d"),
        interval="5m",
        progress=False,
        auto_adjust=True
    )
    
    if data.empty:
        raise ValueError("No data returned - market may be closed")
    
    # Convert index to Eastern Time
    data.index = data.index.tz_convert('America/New_York')
    return data, ppi_time

def analyze_ppi_impact():
    try:
        data, ppi_time = get_market_data()
        
        # Check if PPI time is during trading hours
        trading_start = ppi_time.replace(hour=18, minute=0) - timedelta(days=1)
        trading_end = ppi_time.replace(hour=17, minute=0)
        
        if not (trading_start <= ppi_time <= trading_end):
            raise ValueError(f"PPI time {ppi_time.strftime('%H:%M')} ET is outside gold futures trading hours (6PM-5PM ET)")
        
        # Filter to analysis window
        analysis_data = data[
            (data.index >= ppi_time - timedelta(hours=LOOKBACK_HOURS)) & 
            (data.index <= ppi_time + timedelta(hours=LOOKFORWARD_HOURS))
        ]
        
        if len(analysis_data) < 5:
            raise ValueError(f"Only {len(analysis_data)} data points - market may have been closed")
        
        # Analysis
        pre_ppi = analysis_data[analysis_data.index < ppi_time]
        post_ppi = analysis_data[analysis_data.index >= ppi_time]
        
        print(f"\n{' GOLD PPI ANALYSIS ':=^80}")
        print(f"Ticker: {TICKER} | PPI Time: {ppi_time.strftime('%Y-%m-%d %H:%M ET')}")
        print(f"Data Points: {len(analysis_data)} (Pre: {len(pre_ppi)}, Post: {len(post_ppi)})")
        
        if not pre_ppi.empty:
            print(f"\nPre-PPI Close: ${pre_ppi.iloc[-1]['Close']:.2f}")
        if not post_ppi.empty:
            print(f"Post-PPI Open: ${post_ppi.iloc[0]['Open']:.2f}")
            if not pre_ppi.empty:
                change_pct = (post_ppi.iloc[0]['Open']/pre_ppi.iloc[-1]['Close']-1)*100
                print(f"Immediate Change: {change_pct:.2f}%")
        
        # Plotting
        plt.figure(figsize=(14, 7))
        plt.plot(analysis_data.index, analysis_data['Close'], color='gold', linewidth=2)
        plt.axvline(ppi_time, color='red', linestyle='--', label='PPI Release')
        
        # Formatting
        plt.title(f"Gold Futures (GC=F) Around PPI Release\n{ppi_time.strftime('%Y-%m-%d')}")
        plt.ylabel("Price (USD)")
        plt.grid(alpha=0.3)
        plt.legend()
        
        ax = plt.gca()
        ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M', tz=eastern))
        ax.xaxis.set_major_locator(mdates.HourLocator(interval=1, tz=eastern))
        
        plt.tight_layout()
        plt.savefig("gold_ppi_analysis.png", dpi=300)
        plt.show()
        
    except Exception as e:
        print(f"\n{' ERROR ':=^80}")
        print(f"Failed to analyze: {str(e)}")
        print("\nTroubleshooting Tips:")
        print("1. Gold futures trade Sun-Fri 6PM-5PM ET")
        print("2. Try different dates (avoid weekends/holidays)")
        print("3. For spot gold, use FX markets (not available via Yahoo Finance)")
        print("="*80)

if __name__ == "__main__":
    analyze_ppi_impact()