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
control_min, cost_typ= True, 'NCV-012'
BFS_si, DFS_si= 0, 0

for idx, row in df.iterrows():
    print(idx)
    q= QCSynthesizer(np.array(row).astype(int), bit_len)
    q.Dym_Algorithm(permute=False, control_min=control_min, direction='bi', cost_typ=cost_typ)
 
    qDFS= QCSynthesizer(np.array(row).astype(int), bit_len)
    qDFS.DFS_Algorithm(permute=False, control_min=control_min, direction= 'bi', cost_typ=cost_typ)
    DFS_si += qDFS.order_difference(q)
   
    qBFS= QCSynthesizer(np.array(row).astype(int), bit_len)
    qBFS.BFS_Algorithm(permute=False, control_min=control_min, direction= 'bi', cost_typ=cost_typ)
    BFS_si += qBFS.order_difference(q)

print('TOL DFS difference to Dym: ', DFS_si)
print('TOL BFS difference to Dym: ', BFS_si)
print('for NCV-012 cost control_min=True')
