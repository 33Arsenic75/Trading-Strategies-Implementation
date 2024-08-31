# importing necessary libraries
from datetime import datetime, timedelta
import pandas as pd
from jugaad_data.nse import stock_df
import sys


def get_stock_data(symbol, start_date_str, end_date_str, n, strategy):
    start_date = datetime.strptime(start_date_str, "%d/%m/%Y").date()
    end_date = datetime.strptime(end_date_str, "%d/%m/%Y").date()
    adjusted_start_date = start_date - timedelta(days=n * 365)
    stock_info = stock_df(
        symbol=symbol, from_date=adjusted_start_date, to_date=end_date, series="EQ"
    )
    df = pd.DataFrame(stock_info, columns=["DATE", "CLOSE"])
    df.drop_duplicates(subset="DATE", keep="first", inplace=True)
    df["DATE"] = pd.to_datetime(df["DATE"], format="%d/%m/%Y")
    df.sort_values(by="DATE", inplace=True)
    df.reset_index(drop=True, inplace=True)
    start_date = pd.to_datetime(start_date)
    start_date_index = (df["DATE"] >= start_date).idxmax()
    print(start_date_index)
    df["DATE"] = df["DATE"].dt.strftime("%d/%m/%Y")
    df.set_index("DATE", inplace=True, drop=True)
    df.to_csv(f"{symbol}_{strategy}.csv")
    return start_date_index


if __name__ == "__main__":
    symbol = sys.argv[1]
    start_date = sys.argv[2]
    end_date = sys.argv[3]
    n = int(sys.argv[4])
    strategy = sys.argv[5]
    get_stock_data(
        symbol=symbol,
        start_date_str=start_date,
        end_date_str=end_date,
        n=n,
        strategy=strategy,
    )
