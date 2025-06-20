import importlib
import subprocess
import sys
import importlib.metadata

# Path to the TA-Lib wheel file
TALIB_WHEEL_PATH = r"C:/Users/pc/Downloads/ta_lib-0.6.4-cp312-cp312-win_amd64.whl"

# List of packages with specific versions
packages = [
    "yfinance==0.2.44",
    "pandas==2.2.3",
    "numpy==2.1.2",  # Updated to 2.x for TA-Lib 0.6.4 compatibility
    "scikit-learn==1.5.2",
    "tensorflow==2.17.0",
    "matplotlib==3.9.2",
    "newsapi-python==0.2.7",
    "textblob==0.18.0.post0",
    "nltk==3.9.1",
    "alpha_vantage==3.0.0",
    "vaderSentiment==3.3.2",
    "xgboost==2.1.1",
    "plotly==5.24.1",
    "vectorbt==0.26.2"
]

# Dictionary to map package names to import names
import_names = {
    "yfinance": "yfinance",
    "pandas": "pandas",
    "numpy": "numpy",
    "scikit-learn": "sklearn",
    "tensorflow": "tensorflow",
    "matplotlib": "matplotlib",
    "newsapi-python": "newsapi",
    "textblob": "textblob",
    "nltk": "nltk",
    "alpha_vantage": "alpha_vantage",
    "vaderSentiment": "vaderSentiment",
    "xgboost": "xgboost",
    "plotly": "plotly",
    "vectorbt": "vectorbt",
    "TA-Lib": "talib"
}

def check_and_install_package(package):
    """Check if a package is installed and install it if missing."""
    package_name = package.split("==")[0]
    try:
        importlib.metadata.version(package_name)
        print(f"{package_name} is already installed.")
    except importlib.metadata.PackageNotFoundError:
        print(f"Installing {package}...")
        try:
            subprocess.check_call([sys.executable, "-m", "pip", "install", package])
            print(f"{package} installed successfully.")
        except subprocess.CalledProcessError as e:
            print(f"Failed to install {package}: {e}")
            if package_name == "tensorflow":
                print("Trying TensorFlow pre-release version...")
                try:
                    subprocess.check_call([sys.executable, "-m", "pip", "install", "tensorflow", "--pre"])
                    print("TensorFlow pre-release installed successfully.")
                except subprocess.CalledProcessError as e:
                    print(f"Failed to install TensorFlow pre-release: {e}")
                    print("Consider downgrading to Python 3.11.")

def install_talib():
    """Install TA-Lib using the specified wheel file."""
    try:
        import talib
        print("TA-Lib is already installed.")
    except ImportError:
        print(f"Installing TA-Lib from {TALIB_WHEEL_PATH}...")
        try:
            subprocess.check_call([sys.executable, "-m", "pip", "install", TALIB_WHEEL_PATH])
            print("TA-Lib 0.6.4 installed successfully.")
        except subprocess.CalledProcessError as e:
            print(f"Failed to install TA-Lib: {e}")
            print("Ensure the wheel file path is correct and compatible with Python 3.12 64-bit.")
            print("Download from: https://github.com/cgohlke/talib-build/releases")

def setup_nltk():
    """Download required NLTK data."""
    try:
        import nltk
        print("Downloading NLTK data (punkt, vader_lexicon)...")
        nltk.download("punkt", quiet=True)
        nltk.download("vader_lexicon", quiet=True)
        print("NLTK data downloaded successfully.")
    except ImportError:
        print("NLTK is not installed yet. It will be installed with other packages.")

def main():
    print("Checking and installing packages in virtual environment...")
    print(f"Virtual environment: {sys.executable}")
    
    # Ensure pip is up-to-date
    print("Updating pip...")
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "install", "--upgrade", "pip"])
        print("pip updated successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Failed to update pip: {e}")
    
    # Reinstall numpy first to ensure TA-Lib compatibility
    print("Reinstalling numpy==2.1.2 for TA-Lib 0.6.4 compatibility...")
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "install", "--force-reinstall", "numpy==2.1.2"])
        print("numpy==2.1.2 reinstalled successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Failed to reinstall numpy: {e}")
    
    # Reinstall TA-Lib to ensure compatibility with numpy 2.1.2
    install_talib()
    
    # Reinstall problematic packages
    for package in ["scikit-learn==1.5.2", "newsapi-python==0.2.7"]:
        print(f"Reinstalling {package} to resolve import issues...")
        try:
            subprocess.check_call([sys.executable, "-m", "pip", "install", "--force-reinstall", package])
            print(f"{package} reinstalled successfully.")
        except subprocess.CalledProcessError as e:
            print(f"Failed to reinstall {package}: {e}")
    
    # Install other packages
    for package in packages:
        check_and_install_package(package)
    
    # Set up NLTK data
    setup_nltk()
    
    # Verify installations
    print("\nVerifying installed packages...")
    for package in [p.split("==")[0] for p in packages] + ["TA-Lib"]:
        try:
            __import__(import_names[package])
            print(f"{package} is successfully imported.")
        except ImportError as e:
            print(f"Failed to import {package}: {e}. Check installation or dependencies.")
    
    print("\nInstallation process completed.")
    print("Note: For newsapi-python, import as 'newsapi' and get an API key from https://newsapi.org.")
    print("Note: For alpha_vantage, get an API key from https://www.alphavantage.co.")
    print("If issues persist with TA-Lib, ensure numpy==2.1.2 is used.")
    print("If TensorFlow issues occur, consider downgrading to Python 3.11 or using --pre.")

if __name__ == "__main__":
    main()