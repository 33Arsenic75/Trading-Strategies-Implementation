# importing necessary libraries
from datetime import datetime, timedelta
import pandas as pd
from jugaad_data.nse import stock_df
import sys 
def get_stock_data(symbol, start_date_str, end_date_str,strategy):
    start_date = datetime.strptime(start_date_str, '%d/%m/%Y').date()
    end_date = datetime.strptime(end_date_str, '%d/%m/%Y').date()
    adjusted_start_date = start_date - timedelta(days=30)
    stock_info = stock_df(symbol=symbol, from_date=adjusted_start_date, to_date=end_date, series="EQ")
    df = pd.DataFrame(stock_info, columns=['DATE', 'CLOSE','OPEN','VWAP','LOW','HIGH',"NO OF TRADES"])
    df.drop_duplicates(subset='DATE', keep='first', inplace=True)
    df['CLOSE_t'] = df['CLOSE'].shift(1)
    df['OPEN_t'] = df['OPEN'].shift(1)
    df['DATE'] = df['DATE'].shift(1)
    df['DATE'] = pd.to_datetime(df['DATE'], format='%d/%m/%Y')
    df = df [['DATE','CLOSE_t','CLOSE','OPEN','VWAP','LOW','HIGH',"NO OF TRADES",'OPEN_t']]
    df.sort_values(by='DATE', inplace=True)
    df.reset_index(drop=True, inplace=True)
    start_date = pd.to_datetime(start_date)
    df = df[df['DATE'] >= start_date]
    df['DATE'] = df['DATE'].dt.strftime('%d/%m/%Y')
    df.set_index('DATE', inplace=True, drop=True)
    df.to_csv(f"{symbol}_{strategy}.csv")

if __name__=='__main__':
    symbol = sys.argv[1]
    start_date = sys.argv[2]
    end_date = sys.argv[3]
    strategy = sys.argv[4]
    get_stock_data(symbol=symbol,start_date_str=start_date,end_date_str=end_date,strategy=strategy)