# -*- coding: utf-8 -*-
"""
Created on Thu Aug 22 14:09:22 2019

@author: v-catsai
"""

import pandas as pd

bit_len= 3

Dict = pd.read_csv('exhaustive.csv')
Dict_0 = {}
for i in range(2**bit_len):
   #Dict[str(i)] = [-1]
   Dict_0[str(i)] = []
#print(Dict)
count = 0
df = pd.DataFrame(Dict)
for i in range(2**bit_len):
    temp = pd.DataFrame(Dict_0)
    print(str(i)+'----------------')
    for j in range(i):
        row= df.copy()
        row[str(j)] = i
            #count += 1
        print(temp.shape)
                #print(type(temp), type(row_d))
        temp = pd.concat([temp, row], ignore_index= True)
    df= pd.concat([df, temp], ignore_index=True)
    df = df.drop_duplicates()
df.to_csv('exhaustive_partial.csv', index= False)
print(df.shape[0])