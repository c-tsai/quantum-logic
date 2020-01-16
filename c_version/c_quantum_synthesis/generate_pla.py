# -*- coding: utf-8 -*-
"""
Created on Thu Aug 22 15:15:31 2019

@author: v-catsai
"""
import csv
import os 
import numpy as np

bit_len=3
name ='exhaustive_partial'
mode = 5

df = open(name+'.csv')
reader = csv.reader(df, delimiter=',')
result111= np.zeros((100,9)).astype(int)
result012= np.zeros((100,9)).astype(int)
result155= np.zeros((300,9)).astype(int)
idx, failed= 0, 0
for row in reader:
    x=8
    for i in row:
        if int(float(i)) == -1: x -= 1 
    path = 'temp.pla'
    print idx
    idx += 1
    if idx == 1: continue
    f = open(path, 'w+')
    f.write(str(bit_len)+' 2\n')
    for i in range(len(row)):
        if int(float(row[i])) != -1:
            f.write('{0:03b}'.format(i)+ ' '+ '{0:03b}'.format(int(float(row[i])))+'\n')
    f.close()
    string = os.popen('./a.out temp.pla '+str(mode)).read().split()
    print string
    if len(string) < 8 or string[7] != 'correctly': 
        failed += 1
        continue
    #print string[-4]
    result111[string[-5], x] = result111[string[-5], x] + 1
    result012[string[-3], x] = result012[string[-3], x] + 1
    result155[string[-1], x] = result155[string[-1], x] + 1
    
f= open('qcost_result'+str(mode)+'.txt', 'w+')
f.write('\n'+"NCV-111 COST ---"+'\n')
print "NCV-111 COST ---"
res, count, s= np.sum(result111, axis=1), np.zeros((9)), np.zeros((9))
for i in range(result111.shape[0]):
    f.write(str(res[i])+ '\n')
    print res[i]
    for j in range(9):
        count[j] += i*result111[i, j]
        s[j] += result111[i, j]
f.write('\n')
print('\n')
r = np.divide(count, s)
for i in r:
    f.write(str(i)+'\n')
    print(i)

f.write('\n')
print('\n')
f.write(str(sum(count)/sum(s))+'\n')
print(sum(count)/sum(s))
f.write(str(failed))
print(failed)


f.write('\n'+"NCV-012 COST ---"+'\n')
print "NCV-012 COST ---"
res, count, s= np.sum(result012, axis=1), np.zeros((9)), np.zeros((9))
for i in range(result012.shape[0]):
    f.write(str(res[i])+ '\n')
    print res[i]
    for j in range(9):
        count[j] += i*result012[i, j]
        s[j] += result012[i, j]
f.write('\n')
print('\n')
r = np.divide(count, s)
for i in r:
    f.write(str(i)+'\n')
    print(i)

f.write('\n')
print('\n')
f.write(str(sum(count)/sum(s))+'\n')
print(sum(count)/sum(s))
f.write(str(failed))
print(failed)


f.write('\n'+"NCV-155 COST ---"+'\n')
print "NCV-155 COST ---"
res, count, s= np.sum(result155, axis=1), np.zeros((9)), np.zeros((9))
for i in range(result155.shape[0]):
    f.write(str(res[i])+ '\n')
    print res[i]
    for j in range(9):
        count[j] += i*result155[i, j]
        s[j] += result155[i, j]
f.write('\n')
print('\n')
r = np.divide(count, s)
for i in r:
    f.write(str(i)+'\n')
    print(i)

f.write('\n')
print('\n')
f.write(str(sum(count)/sum(s))+'\n')
print(sum(count)/sum(s))
f.write(str(failed))
print(failed)
f.close()

    
