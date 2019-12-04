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

for idx, row in df.iterrows():
    print(idx)
    q155= QCSynthesizer(np.array(row).astype(int), bit_len)
    q155.BFS_Algorithm(permute=False, control_min=False, direction= 'bi', cost_typ='NCV-155')
    qc155= q155.output_circuit()
 
    q111= QCSynthesizer(np.array(row).astype(int), bit_len)
    q111.BFS_Algorithm(permute=False, control_min=False, direction= 'bi', cost_typ='NCV-111')
    qc111= q111.output_circuit()
   
    q012= QCSynthesizer(np.array(row).astype(int), bit_len)
    q012.BFS_Algorithm(permute=False, control_min=False, direction= 'bi', cost_typ='NCV-012')
    qc012= q012.output_circuit()

    if not qc111==qc012 or not qc111==qc155:
        print("not identical in BFS Algorithm for the case:")
        print(np.array(row))
        print("for NCV-111:")
        print(qc111)
        print("for NCV-012:")
        print(qc012)
        print("for NCV-111:")
        print(qc012)
        raise ValueError("difference exist!!")
        break
