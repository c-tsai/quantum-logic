# -*- coding: utf-8 -*-
"""
Created on Thu Aug 22 15:15:31 2019

@author: v-catsai
"""
import pandas as pd
import os 
bit_len=3
name ='exhaustive'
mode =6

df = pd.read_csv(name+'.csv')
#os.makedirs(name)
for idx, row in df.iterrows():
    path = 'temp.pla'
    print(idx)
    f = open(path, 'w+')
    f.write(str(bit_len)+' 2\n')
    for i in range(len(row)):
        if row[i] != -1:
            f.write('{0:03b}'.format(i)+ ' '+ '{0:03b}'.format(int(row[i]))+'\n')
    f.close()
    os.system('./a.out temp.pla '+str(mode))

    
