# -*- coding: utf-8 -*-
"""
Created on Thu Aug 22 15:15:31 2019

@author: v-catsai
"""
import numpy as np
import pandas as pd
from synthesizer import QCSynthesizer
bit_len=3

df = pd.read_csv('exhaustive.csv')
result= np.zeros((20)).astype(int)
for idx, row in df.iterrows():
    print(idx)
    q= QCSynthesizer(np.array(row).astype(int), bit_len)
    q.BFS_Algorithm(permute=True, control_min=True, direction= 'bi')
    qc= q.output_circuit()
    result[len(qc)] = result[len(qc)] + 1
    for i in range(len(row)):
        if not row[i]== qc.inf(i):
            raise ValueError
count, s= 0, 0
for i in range(result.shape[0]):
    print(result[i])
    count += i*result[i]
    s += result[i]
print('\n'+str(count/s))
    