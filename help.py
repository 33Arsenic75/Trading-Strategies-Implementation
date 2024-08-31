import csv
import subprocess
from tabulate import tabulate
import pandas as pd

df = pd.read_csv('data.csv')
result = {}

# Iterate over rows
for index, row in df.iterrows():
    n_value = row['n']
    output_value = row['Output']

    # Check if 'n' is already in the result dictionary
    if n_value in result:
        result[n_value] += output_value
    else:
        result[n_value] = output_value

result_df = pd.DataFrame(list(result.items()), columns=['n', 'Output'])

# Write the DataFrame to a CSV file
result_df.to_csv('cumulative.csv', index=False)
