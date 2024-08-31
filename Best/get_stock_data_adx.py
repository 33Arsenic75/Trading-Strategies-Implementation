# importing necessary libraries
from datetime import datetime, timedelta
import pandas as pd
from jugaad_data.nse import stock_df
import sys 
def get_stock_data(symbol, start_date_str, end_date_str,n,strategy):
    start_date = datetime.strptime(start_date_str, '%d/%m/%Y').date()
    end_date = datetime.strptime(end_date_str, '%d/%m/%Y').date()
    adjusted_start_date = start_date - timedelta(days=365*n)
    stock_info = stock_df(symbol=symbol, from_date=adjusted_start_date, to_date=end_date, series="EQ")
    #print(stock_info)
    df = pd.DataFrame(stock_info, columns=['DATE','LOW','HIGH', "PREV. CLOSE","CLOSE"])
    df.drop_duplicates(subset='DATE', keep='first', inplace=True)
    df['HIGH_t-1'] = df['HIGH'].shift(-1)
    df['LOW_t-1'] = df['LOW'].shift(-1)
    df['DATE'] = pd.to_datetime(df['DATE'], format='%d/%m/%Y')
    df = df [['DATE','LOW','HIGH',"LOW_t-1" ,"HIGH_t-1", "PREV. CLOSE","CLOSE"]]
    df.sort_values(by='DATE', inplace=True)
    df.reset_index(drop=True, inplace=True)
    df.dropna(inplace=True)
    start_date = pd.to_datetime(start_date)
    #df = df[df['DATE'] >= start_date]
    start_date_index = (df['DATE'] >= start_date).idxmax()
    df['DATE'] = df['DATE'].dt.strftime('%d/%m/%Y')
    df.set_index('DATE', inplace=True, drop=True)
    df.to_csv(f"{symbol}_{strategy}.csv")
    print(start_date_index)
    return start_date_index

if __name__=='__main__':
    symbol = sys.argv[1]
    start_date = sys.argv[2]
    end_date = sys.argv[3]
    n = int(sys.argv[4])
    strategy = sys.argv[5]
    get_stock_data(symbol=symbol,start_date_str=start_date,end_date_str=end_date,n=n,strategy=strategy)