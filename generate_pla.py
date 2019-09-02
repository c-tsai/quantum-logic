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
mode =6

file = open(name+'.csv', 'r')
my_reader = csv.reader(file, delimiter= ',')
result= np.zeros((20,9)).astype(int)
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
    #string = os.popen('./a.out temp.pla '+str(mode)).read().split()
    #result[string[-2], x] = result[string[-2], x] + 1
    
res, count, s= np.sum(result, axis=1), np.zeros((9)), np.zeros((9))
for i in range(result.shape[0]):
    print(res[i])
    for j in range(9):
        count[j] += i*result[i, j]
        s[j] += result[i, j]
print('\n')
r = np.divide(count, s)
for i in r:
    print(i)

print('\n')
print(sum(count)/sum(s))

    
