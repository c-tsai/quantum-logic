# -*- coding: utf-8 -*-
"""
Created on Thu Aug 22 14:09:22 2019

@author: v-catsai
"""

import pandas as pd

bit_len= 3

Dict = {}
Dict_0 = {}
for i in range(2**bit_len):
   Dict[str(i)] = [-1]
   Dict_0[str(i)] = []
#print(Dict)
df = pd.DataFrame(Dict)
for i in range(2**bit_len):
    print(i)
    temp = pd.DataFrame(Dict_0)
    for j in range(2**bit_len):
        for idx, row in df.iterrows():
            row_d = {}
            for k in range(2**bit_len):
                row_d[str(k)]= [row[str(k)]]
            row_d[str(j)]= i
                #print(type(temp), type(row_d))
            temp = pd.concat([temp, pd.DataFrame(row_d)], ignore_index= True)
    df= pd.concat([df, temp], ignore_index=True)
    df.drop_duplicates()
df.to_csv('exhaustive_partial.csv', index= False)
print(df.shape[0])