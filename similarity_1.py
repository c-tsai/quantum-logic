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
control_min, cost_typ= False, 'NCV-155'
BFS, DFS= 0, 0

for idx, row in df.iterrows():
    print(idx)
    q= QCSynthesizer(np.array(row).astype(int), bit_len)
    q.Dym_Algorithm(permute=False, control_min=control_min, direction='bi', cost_typ=cost_typ)
    if (q.order[0]|q.order[1])==q.order[2]: DFS += 1
    else: BFS += 1
    

print('amount of Dym became DFS: ', DFS)
print('amount of Dym became BFS: ', BFS)
print('for NCV-155 cost control_min=False')
