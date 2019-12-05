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
resultG= np.zeros((20,9)).astype(int)
result111= np.zeros((100,9)).astype(int)
result012= np.zeros((100,9)).astype(int)
result155= np.zeros((300,9)).astype(int)
DFS, BFS = 0, 0

for idx, row in df.iterrows():
    x=8
    for i in row:
        if i == -1: x -= 1 
    print(idx)
    q= QCSynthesizer(np.array(row).astype(int), bit_len)
    q.Dym_Algorithm(permute=False, control_min=False, direction= 'bi', cost_typ='NCV-155')
    qc= q.output_circuit()
    #print(q.order)
    resultG[len(qc), x] = resultG[len(qc), x] + 1
    if (q.order[1]|q.order[2])==q.order[3]: DFS += 1
    else: BFS += 1
    #print(qc.cost(0, 'NCV-155'))
    result111[qc.cost(0, 'NCV-111'),x] = result111[qc.cost(0, 'NCV-111'),x] + 1
    result155[qc.cost(0, 'NCV-155'),x] = result155[qc.cost(0, 'NCV-155'),x] + 1
    result012[qc.cost(0, 'NCV-012'),x] = result012[qc.cost(0, 'NCV-012'),x] + 1
    for i in range(len(row)):
        if not row[i]== qc.inf(i) and not row[i]== -1:
            raise ValueError('wrong result '+ str(row[i])+ ' to '+ str(qc.inf(i)))


print('result_NCV-155_Dym.txt')
print('amount of Dym became DFS: ', DFS)
print('amount of Dym became BFS: ', BFS)


f= open('result_NCV-155_Dym.txt', 'w+')
f.write('----gate cost ----'+ '\n')
res, count, s= np.sum(resultG, axis=1), np.zeros((9)), np.zeros((9))
for i in range(resultG.shape[0]):
    f.write(str(res[i])+ '\n')
    for j in range(9):
        count[j] += i*resultG[i, j]
        s[j] += resultG[i, j]
f.write('\n')
r = np.divide(count, s)
for i in r:
    f.write(str(i)+ '\n')

f.write('\n')
f.write(str(sum(count)/sum(s))+ '\n')


f.write('----NCV-111 cost ----'+ '\n')
res, count, s= np.sum(result111, axis=1), np.zeros((9)), np.zeros((9))
for i in range(result111.shape[0]):
    f.write(str(res[i])+ '\n')
    for j in range(9):
        count[j] += i*result111[i, j]
        s[j] += result111[i, j]
f.write('\n')
r = np.divide(count, s)
for i in r:
    f.write(str(i)+ '\n')
f.write('\n')
f.write(str(sum(count)/sum(s)) + '\n')


f.write('----NCV-012 cost ----'+ '\n')
res, count, s= np.sum(result012, axis=1), np.zeros((9)), np.zeros((9))
for i in range(result012.shape[0]):
    f.write(str(res[i])+ '\n')
    for j in range(9):
        count[j] += i*result012[i, j]
        s[j] += result012[i, j]
f.write('\n')
r = np.divide(count, s)
for i in r:
    f.write(str(i)+ '\n')

f.write('\n')
f.write(str(sum(count)/sum(s)) + '\n')

f.write('----NCV-155 ----'+ '\n')
res, count, s= np.sum(result155, axis=1), np.zeros((9)), np.zeros((9))
for i in range(result155.shape[0]):
    f.write(str(res[i])+ '\n')
    for j in range(9):
        count[j] += i*result155[i, j]
        s[j] += result155[i, j]
f.write('\n')
r = np.divide(count, s)
for i in r:
    f.write(str(i)+ '\n')

f.write('\n')
f.write(str(sum(count)/sum(s))+ '\n')
f.close()
    
