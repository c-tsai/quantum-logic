# -*- coding: utf-8 -*-
"""
Created on Thu Aug 22 15:15:31 2019

@author: v-catsai
"""
import numpy as np
import pandas as pd
from synthesizer import QCSynthesizer
bit_len=3

df = pd.read_csv('exhaustive_partial.csv')
result= np.zeros((20,9)).astype(int)
for idx, row in df.iterrows():
    x=8
    for i in row:
        if i == -1: x -= 1 
    print(idx)
    q= QCSynthesizer(np.array(row).astype(int), bit_len)
    q.Dym_Algorithm(permute=False, control_min=True, direction= 'bi', cost_typ='Hamming')
    qc= q.output_circuit()
    result[len(qc), x] = result[len(qc), x] + 1
    for i in range(len(row)):
        if not row[i]== qc.inf(i) and not row[i]== -1:
            raise ValueError('wrong result '+ str(row[i])+ ' to '+ str(qc.inf(i)))
res, count, s= np.sum(result, axis=1), np.zeros((9)), np.zeros((9))
f= open('result_hammingcost_Dym.txt', 'w+')
for i in range(result.shape[0]):
    f.write(str(res[i])+ '\n')
    for j in range(9):
        count[j] += i*result[i, j]
        s[j] += result[i, j]
f.write('\n')
r = np.divide(count, s)
for i in r:
    f.write(str(i)+ '\n')

f.write('\n')
f.write(str(sum(count)/sum(s)))
f.close()
    
