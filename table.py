# -*- coding: utf-8 -*-
"""
Created on Tue Nov 19 15:35:08 2019

@author: v-catsai
"""

def pla_reader(file):
    f = open(file, 'r')
    bit_len, i, t= 0, 0, 0
    for line in f:
        if i ==0 : 
            bit_len= int(line.split(' ')[0])
            t = Table(2**bit_len)
        else: 
            res = line.split(' ')
            t[int(res[0],2)] = int(res[1],2)
        i += 1
    return t, bit_len

def Hamming_Dist(bit1, bit2, bit_len):
    if bit1==-1 or bit2==-1:
        return 0
    diff, count= bit1^bit2, 0
    #print(diff)
    for i in range(bit_len):
        if diff%2==1:
            count += 1
        diff = diff//2
    return count


class Table:
    def __init__(self, length, array = 0):
        self.dict = {}
        self.done = set([])
        self.length = length
        if not isinstance(array, int):
            for i in range(array.shape[0]):
                if array[i] != -1: self.dict[i]= int(array[i])
            
    def __str__(self): return str(self.dict)    
    def __getitem__(self, key):
        if key in self.dict: return self.dict[key]
        elif key in self.done: return key 
        elif key < self.length: return -1
        else: 
            raise ValueError('"'+str(key)+'" out of range (>'+str(self.length)+')')
            
    def __setitem__(self, key, value):
        self.dict[key]= value
        
    def __del__(self):
        del self.dict
        del self.done
        del self.length
    def __len__(self): return len(self.dict)
    def __iter__(self):
        self.iterator= iter(self.dict)
        return self
    def __next__(self): return next(self.iterator)
    def __contains__(self, key):return key in self.dict
    #def __str__(self): return 'Table'
    
    def copy(self): 
        t = Table(self.length)
        t.dict= self.dict.copy()
        t.done = self.done.copy()
        return t
    
    def traversed_pop(self, key):
        self.dict.pop(key, None)
        self.done.add(key)
    
    def summ(self):
        result = 0
        for i in self.dict:
            result += self.dict[i]
        return result
    
    