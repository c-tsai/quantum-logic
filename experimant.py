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
    q.given_order_alg(range(2**bit_len), control_min=False, direction= 'uni')
    qc= q.output_circuit()
    result[len(qc)] = result[len(qc)] + 1
for i in result:
    print(result)
    