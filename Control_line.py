# -*- coding: utf-8 -*-
"""
Created on Tue Dec 10 12:04:32 2019

@author: v-catsai
"""
from table import Hamming_Dist

class Control_lines:
    def __init__(self, bit_len):
        self.lib= [set([]) for i in range(bit_len+1)]
        self.bit_len= bit_len
    def add(self, line, b_num=0):
        if b_num == 0:
            self.lib[Hamming_Dist(line,0,self.bit_len)].add(line)
        else: self.lib[b_num].add(line)
    def pop(self, line, b_num=0):
        if b_num == 0:
            self.lib[Hamming_Dist(line,0,self.bit_len)].discard(line)
        else: self.lib[b_num].discard(line)
    def __str__(self):
        string = '['
        for s in self.lib:
            string = string + '('
            for i in s:
                string = string + str(i) + ','
            string = string + ')'+ '\n'
        string = string + ']'
        return string
    def __getitem__(self, key):
        if key < 0 or key > self.bit_len:
            raise IndexError(str(key)+" is not in: 1~"+str(self.bit_len))
        return self.lib[key] 
    def __setitem__(self, key, value):
        raise ValueError("The function shouldn't be used")
    def __del__(self):
        del self.lib
        del self.bit_len
        
    def __contains__(self, key): 
        return key in self.lib[Hamming_Dist(key,0,self.bit_len)]
    
    def contain(self, key, b_num): return key in self.lib[b_num] 
    
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
            self.lib[i]= self.lib[i].union(new[i])
        

class Control_lines_generator:
    def __init__(self, bit_len):
        self.bit_len = bit_len
        self.unable = Control_lines(bit_len)
        self.lib_all = {}
        self.lib_fin = {}
        
    def remove(self, targ):
        self.unable.add(targ)
        self.lib_all.pop(targ, None)
        self.lib_fin.pop(targ, None)
        bit_num = Hamming_Dist(targ,0,self.bit_len)
        for key in self.lib_fin:self.lib_fin[key].pop(targ,bit_num) 
        
    def able_clines(self, bit1, controled):
        aim, point = (bit1|controled)- controled, 1
        if aim in self.lib_fin: return self.lib_fin[aim]
        
        pre_fin, pre_all = Control_lines(self.bit_len), Control_lines(self.bit_len)
        pre_fin.add(0,0)
        pre_all.add(0,0)
        dist, diff = 100000000000, aim
        for key in self.lib_fin:
            d= key^aim
            if aim|key == aim and Hamming_Dist(d,0)< dist:
                del pre_fin, pre_all
                pre_fin, pre_all, diff= self.lib_fin[key], self.lib_all[key], d
                dist = Hamming_Dist(d,0)
        self.lib_all[aim], self.lib_fin[aim] = pre_all, pre_fin        
        for i in range(self.bit_len):
            if (point&diff)!=0:
                for i in range(self.bit_len):
                    new = Control_lines(self.bit_len)
                    for targ in self.lib_all[aim][i]:
                        if targ|point ==targ: continue
                        new.add(targ|point, i+1)
                        if not self.unable.contain(targ|point, i+1):
                            self.lib_fin[aim].add(targ|point, i+1)
                    self.lib_all[aim].union(new)
                #print('new', new)
            point *= 2
            #print(result)
        return self.lib_fin[aim]
        
            