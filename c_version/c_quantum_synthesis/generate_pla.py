# -*- coding: utf-8 -*-
"""
Created on Thu Aug 22 15:15:31 2019

@author: v-catsai
"""
import csv
import os 
import numpy as np

bit_len=3
name ='exhaustive'
mode =3

file = open(name+'.csv', 'r')
my_reader = csv.reader(file, delimiter= ',')
result= np.zeros((20,9)).astype(int)
resultQ= np.zeros((20,9)).astype(int)
idx=0

for row in my_reader:
    x=8
    for i in row:
        if i == -1: x -= 1 
    path = 'temp.pla'
    print(idx)
    idx += 1
    f = open(path, 'w+')
    f.write(str(bit_len)+' 2\n')
    for i in range(len(row)):
        if row[i] != -1:
            f.write('{0:03b}'.format(i)+ ' '+ '{0:03b}'.format(int(float(row[i])))+'\n')
    f.close()
    string = os.popen('./simp_cont_write temp.pla '+str(mode)+' t bi q temp.real').read().split()
    result[string[2], x] = result[string[2], x] + 1
    resultQ[string[4], x] = resultQ[string[4], x] + 1
    
res, count, s= np.sum(resultQ, axis=1), np.zeros((9)), np.zeros((9))
f= open('resultQ.txt', 'w+')
for i in range(resultQ.shape[0]):
    f.write(res[i])
    for j in range(9):
        count[j] += i*resultQ[i, j]
        s[j] += resultQ[i, j]
f.write('\n')
r = np.divide(count, s)
for i in r:
    f.write(i)

f.write('\n')
f.write(sum(count)/sum(s))
f.close()

res, count, s= np.sum(result, axis=1), np.zeros((9)), np.zeros((9))
f= open('result.txt', 'w+')
for i in range(result.shape[0]):
    f.write(res[i])
    for j in range(9):
        count[j] += i*result[i, j]
        s[j] += result[i, j]
f.write('\n')
r = np.divide(count, s)
for i in r:
    f.write(i)

f.write('\n')
f.write(sum(count)/sum(s))
f.close()

    
