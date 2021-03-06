# -*- coding: utf-8 -*-
"""
Created on Thu Aug 22 15:15:31 2019

@author: v-catsai
"""
import numpy as np
import pandas as pd
from synthesizer import QCSynthesizer
from tools import Table
bit_len=3

df = pd.read_csv('exhaustive_partial.csv')
resultG= np.zeros((20,9)).astype(int)
result111= np.zeros((100,9)).astype(int)
result012= np.zeros((100,9)).astype(int)
result155= np.zeros((300,9)).astype(int)

for idx, row in df.iterrows():
    x=8
    for i in row:
        if i == -1: x -= 1 
    print(idx, row)
    table = Table(2**bit_len, row)
    q= QCSynthesizer(table, bit_len)
    q.DFS_Algorithm(permute=False, control_min=True, direction= 'bi', cost_typ='length')
    qc= q.output_circuit()
    resultG[len(qc), x] = result111[len(qc), x] + 1
    #print(qc.cost(0, 'NCV-155'))
    result111[qc.cost(0, 'NCV-111'),x] = result111[qc.cost(0, 'NCV-111'),x] + 1
    result155[qc.cost(0, 'NCV-155'),x] = result155[qc.cost(0, 'NCV-155'),x] + 1
    result012[qc.cost(0, 'NCV-012'),x] = result012[qc.cost(0, 'NCV-012'),x] + 1
    for i in range(len(row)):
        if not row[i]== qc.inf(i) and not row[i]== -1:
            raise ValueError('wrong result '+ str(row[i])+ ' to '+ str(qc.inf(i)))




f= open('result_NCV111_BFS_controled.txt', 'w+')
f.write('----gate cost ----'+ '\n')
print('----gate cost ----')
res, count, s= np.sum(resultG, axis=1), np.zeros((9)), np.zeros((9))
for i in range(resultG.shape[0]):
    f.write(str(res[i])+ '\n')
    print(str(res[i]))
    for j in range(9):
        count[j] += i*resultG[i, j]
        s[j] += resultG[i, j]
f.write('\n')
print('\n')
r = np.divide(count, s)
for i in r:
    f.write(str(i)+ '\n')
    print(str(i))

f.write('\n')
print('\n')
f.write(str(sum(count)/sum(s))+ '\n')
print(str(sum(count)/sum(s)))


f.write('----NCV-111 cost ----'+ '\n')
print('----NCV-111 cost ----')
res, count, s= np.sum(result111, axis=1), np.zeros((9)), np.zeros((9))
for i in range(result111.shape[0]):
    f.write(str(res[i])+ '\n')
    print(str(res[i]))
    for j in range(9):
        count[j] += i*result111[i, j]
        s[j] += result111[i, j]
f.write('\n')
print('\n')
r = np.divide(count, s)
for i in r:
    f.write(str(i)+ '\n')
    print(str(i))

f.write('\n')
print('\n')
f.write(str(sum(count)/sum(s)) + '\n')
print(str(sum(count)/sum(s)))


f.write('----NCV-012 cost ----'+ '\n')
print('----NCV-012 cost ----')
res, count, s= np.sum(result012, axis=1), np.zeros((9)), np.zeros((9))
for i in range(result012.shape[0]):
    f.write(str(res[i])+ '\n')
    print(str(res[i]))
    for j in range(9):
        count[j] += i*result012[i, j]
        s[j] += result012[i, j]
f.write('\n')
print('\n')
r = np.divide(count, s)
for i in r:
    f.write(str(i)+ '\n')
    print(str(i))

f.write('\n')
print('\n')
f.write(str(sum(count)/sum(s)) + '\n')
print(str(sum(count)/sum(s)))


f.write('----NCV-155 ----'+ '\n')
print('----NCV-155 ----')
res, count, s= np.sum(result155, axis=1), np.zeros((9)), np.zeros((9))
for i in range(result155.shape[0]):
    f.write(str(res[i])+ '\n')
    print(str(res[i]))
    for j in range(9):
        count[j] += i*result155[i, j]
        s[j] += result155[i, j]
f.write('\n')
print('\n')
r = np.divide(count, s)
for i in r:
    f.write(str(i)+ '\n')
    print(str(i))

f.write('\n')
print('\n')
f.write(str(sum(count)/sum(s))+ '\n')
print(str(sum(count)/sum(s)))
f.close()
    
