# -*- coding: utf-8 -*-
"""
Created on Thu Aug 22 15:15:31 2019

@author: v-catsai
"""
import csv
import os 
#import numpy as np

bit_len=3
name ='exhaustive_partial'
mode = 0

df = open(name+'.csv')
reader = csv.reader(df, delimiter=',')
idx, failed= 0, 0
for row in reader:
    x=8
    for i in row:
        if int(float(i)) == -1: x -= 1 
    path = 'temp.pla'
    print(idx)
    idx += 1
    if idx == 1: continue
    f = open(path, 'w+')
    f.write(str(bit_len)+' 2\n')
    for i in range(len(row)):
        if int(float(row[i])) != -1:
            f.write('{0:03b}'.format(i)+ ' '+ '{0:03b}'.format(int(float(row[i])))+'\n')
    f.close()
    string = os.popen('./a.exe temp.pla '+str(mode)+ ' t bi q').read()
    #print(string)
    if string.split()[-1] == 'instead)': 
        failed += 1
        print(string)
        break
    #print string[-4]
    