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
        self.length = length
        if not isinstance(array, int):
            for i in range(array.shape[0]):
                if array[i] != -1: self.dict[i]= int(array[i])
            
    def __str__(self): return str(self.dict)    
    def __getitem__(self, key):
        if key in self.dict: return self.dict[key]
        elif key < self.length: return -1
        else: 
            raise ValueError('"'+str(key)+'" out of range (>'+str(self.length)+')')
            
    def __setitem__(self, key, value):
        self.dict[key]= value
        
    def __del__(self):
        del self.dict
        del self.length
    def __len__(self): return self.length
    def __iter__(self):
        self.iterator= iter(self.dict)
        return self
    def __next__(self): return next(self.iterator)
    def __contains__(self, key):return key in self.dict
    #def __str__(self): return 'Table'
    
    def copy(self): 
        t = Table(self.length)
        t.dict= self.dict.copy()
        return t
    
    def summ(self):
        result = 0
        for i in self:
            result += self.__getitem__(i)
        return result
    
    
class Control_lines:
    def __init__(self, bit_len):
        self.lib= [set([]) for i in range(bit_len)]
        self.bit_len= bit_len
    def add(self, line, b_num=0):
        if b_num == 0:
            self.lib[Hamming_Dist(line,0,self.bit_len)-1].add(line)
        else: self.lib[b_num-1].add(line)
    def __getitem__(self, key):
        if key < 1 or key > self.bit_len:
            raise IndexError(str(key)+" is not in: 1~"+str(self.bit_len))
        return self.lib[key-1] 
    def __setitem__(self, key, value):
        raise ValueError("The function shouldn't be used")
    def __del__(self):
        del self.lib
        del self.bit_len
        
    def __contains__(self, key): 
        return key in self.lib[Hamming_Dist(key,0,self.bit_len)-1]
    
    def __len__(self): return len(self.lib)
    def __iter__(self):
        self.iterator, self.iterid= iter(self.lib[0]), 0
        return self
    def __next__(self):
        try:
            res = next(self.iterator)
        except StopIteration:
            self.iterid += 1
            if self.iterid == self.bit_len: raise StopIteration
            self.iterator = iter(self.lib[self.iterid])
            res = next(self.iterator)
        return res
    def union(self, new):
        for i in range(self.bit_len):
            self.lib[i]= self.lib[i].union(new[i+1])
        

class Control_lines_generator:
    def __init__(self, bit_len):
        self.bit_len = bit_len
        self.unable = Control_lines(bit_len)
        
    def remove(self, targ):
        self.unable.add(targ)
        
    def able_clines(self, bit1, controled):
        aim = (bit1|controled)- controled
        result, point= Control_lines(self.bit_len), 1
        for i in range(self.bit_len):
            if not point in self.unable[1] and (point&aim)!=0: 
                result.add(point, 1)
                new = Control_lines(self.bit_len)
                for i in result:new.add(i|point)
                result.union(new)
            point *= 2
        return result
        
            