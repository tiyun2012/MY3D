"""Dependency and environment checker for trading/data science stack."""

import sys
import importlib.metadata

print(f"Python version: {sys.version}")
print(f"Virtual environment: {sys.executable}")

try:
    import yfinance
    print(f"yfinance version: {yfinance.__version__}")
except ImportError as e:
    print(f"Failed to import yfinance: {e}")

try:
    import pandas
    print(f"pandas version: {pandas.__version__}")
except ImportError as e:
    print(f"Failed to import pandas: {e}")

try:
    import numpy
    print(f"numpy version: {numpy.__version__}")
    NUMPY_AVAILABLE = True
except ImportError as e:
    print(f"Failed to import numpy: {e}")
    NUMPY_AVAILABLE = False

try:
    import sklearn
    print(f"scikit-learn version: {sklearn.__version__}")
except ImportError as e:
    print(f"Failed to import scikit-learn: {e}")

try:
    import tensorflow
    print(f"tensorflow version: {tensorflow.__version__}")
except ImportError as e:
    print(f"Failed to import tensorflow: {e}")

try:
    import matplotlib
    print(f"matplotlib version: {matplotlib.__version__}")
except ImportError as e:
    print(f"Failed to import matplotlib: {e}")

# Only import version, not the module, to avoid unused import warning
try:
    newsapi_version = importlib.metadata.version("newsapi-python")
    print(f"newsapi-python version: {newsapi_version}")
except importlib.metadata.PackageNotFoundError:
    print("newsapi-python is installed but version could not be determined.")
except ImportError as e:
    print(f"Failed to import newsapi-python: {e}")

try:
    textblob_version = importlib.metadata.version("textblob")
    print(f"textblob version: {textblob_version}")
except importlib.metadata.PackageNotFoundError:
    print("textblob is installed but version could not be determined.")
except ImportError as e:
    print(f"Failed to import textblob: {e}")

try:
    import nltk
    print(f"nltk version: {nltk.__version__}")
except ImportError as e:
    print(f"Failed to import nltk: {e}")

try:
    alpha_vantage_version = importlib.metadata.version("alpha_vantage")
    print(f"alpha_vantage version: {alpha_vantage_version}")
except importlib.metadata.PackageNotFoundError:
    print("alpha_vantage is installed but version could not be determined.")
except ImportError as e:
    print(f"Failed to import alpha_vantage: {e}")

try:
    vader_version = importlib.metadata.version("vaderSentiment")
    print(f"vaderSentiment version: {vader_version}")
except importlib.metadata.PackageNotFoundError:
    print("vaderSentiment is installed but version could not be determined.")
except ImportError as e:
    print(f"Failed to import vaderSentiment: {e}")

try:
    import xgboost
    print(f"xgboost version: {xgboost.__version__}")
except ImportError as e:
    print(f"Failed to import xgboost: {e}")

try:
    import plotly
    print(f"plotly version: {plotly.__version__}")
except ImportError as e:
    print(f"Failed to import plotly: {e}")

try:
    import vectorbt
    print(f"vectorbt version: {vectorbt.__version__}")
except ImportError as e:
    print(f"Failed to import vectorbt: {e}")

try:
    import talib
    print(f"TA-Lib version: {talib.__version__}")
    # Test a TA-Lib function to verify numpy compatibility
    if NUMPY_AVAILABLE:
        test_data = numpy.array([1, 2, 3, 4, 5.0])
        sma = talib.SMA(test_data, timeperiod=2)  # type: ignore[attr-defined]
        print("TA-Lib SMA test passed successfully.")
    else:
        print("Skipping TA-Lib SMA test: numpy not available.")
except ImportError as e:
    print(f"Failed to import TA-Lib: {e}")
except (TypeError, AttributeError) as e:
    print(f"TA-Lib test failed: {e}. Check numpy compatibility (requires numpy 2.x).")

print("\nAll packages checked. If any failed to import, reinstall them individually.")