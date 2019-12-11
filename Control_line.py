# -*- coding: utf-8 -*-
"""
Created on Tue Dec 10 12:04:32 2019

@author: v-catsai
"""
from table import Hamming_Dist
import copy as c

class Control_lines:
    def __init__(self, bit_len):
        self.lib= [set([]) for i in range(bit_len+1)]
        self.bit_len= bit_len
        self.iterid = -1
    def add(self, line, b_num=-1):
        if b_num == -1:
            self.lib[Hamming_Dist(line,0,self.bit_len)].add(line)
        else: self.lib[b_num].add(line)
    def pop(self, line, b_num=-1):
        if b_num == -1:
            self.lib[Hamming_Dist(line,0,self.bit_len)].discard(line)
        else: self.lib[b_num].discard(line)
    def __str__(self):
        string = '['
        for s in self.lib:
            string = string + '('
            for i in s:
                string = string + str(i) + ','
            string = string + ')'+ ';'
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
        
    def __contains__(self, key): 
        return key in self.lib[Hamming_Dist(key,0,self.bit_len)]
    
    def contain(self, key, b_num): return key in self.lib[b_num] 
    
    def __len__(self): return len(self.lib)
    def __iter__(self):
        self.iterator, self.iterid= iter(self.lib[0]), 0
        #print(self.iterid)
        return self
    def __next__(self):
        try:
            #print(self.iterid)
            res = next(self.iterator)
        except StopIteration:
            self.iterid += 1
            if self.iterid == self.bit_len:
                self.iterid = -1
                raise StopIteration
            self.iterator = iter(self.lib[self.iterid])
            res = next(self)
        return res
    def union(self, new):
        for i in range(self.bit_len):
            self.lib[i]= self.lib[i].union(new[i])
    def copy(self):
        new = Control_lines(self.bit_len)
        new.lib= c.deepcopy(self.lib)
        return new
            


class Control_lib(Control_lines):
    def __init__(self, bit_len):
        self.lib= [{} for i in range(bit_len+1)]
        self.bit_len= bit_len
        self.iterid = -1
    def add(self, key, c_lines, b_num=-1):
        if b_num == -1:
            self.lib[Hamming_Dist(key,0,self.bit_len)][key]= c_lines
        else: self.lib[b_num][key]= c_lines
    def pop(self, line, b_num=-1):
        if b_num == -1:
            self.lib[Hamming_Dist(line,0,self.bit_len)].pop(line, None)
        else: self.lib[b_num].discard(line, None)
    def get(self, targ, b_num=-1):
        if b_num == -1:
            if self.iterid != -1:
                try: return self.lib[self.iterid][targ]
                except KeyError:  
                    return self.lib[Hamming_Dist(targ,0,self.bit_len)][targ]
            return self.lib[Hamming_Dist(targ,0,self.bit_len)][targ]
        return self.lib[b_num][targ]
    def __str__(self):
        string = '['
        for s in self.lib:
            string = string + '{'
            for i in s:
                string = string+ str(i)+ ':'+ str(s[i]) + ','
            string = string + '}'+ '\n'
        string = string + ']'
        return string
        
        

class Control_lines_generator:
    def __init__(self, bit_len):
        self.bit_len = bit_len
        self.unable = Control_lines(bit_len)
        self.lib_all = Control_lib(bit_len)
        self.lib_fin = Control_lib(bit_len)
        
    def remove(self, targ):
        self.unable.add(targ)
        self.lib_all.pop(targ)
        self.lib_fin.pop(targ)
        bit_num = Hamming_Dist(targ,0,self.bit_len)
        #print(self.lib_fin)
        for key in self.lib_fin:
            #print(0)
            self.lib_fin.get(key).pop(targ,bit_num) 
            #print(1, self.lib_fin.get(key))
        
    def able_clines(self, bit1, controled):
        aim, point = (bit1|controled)- controled, 1
        #print(aim, self.lib_fin)
        if aim in self.lib_fin: return self.lib_fin.get(aim)
        
        pre_fin, pre_all = Control_lines(self.bit_len), Control_lines(self.bit_len)
        pre_all.add(0,0)
        if not self.unable.contain(0,0): pre_fin.add(0,0)
        dist, diff = 100000000000, aim
        for key in self.lib_fin:
            d= key^aim
            if aim|key == aim and Hamming_Dist(d,0,self.bit_len)< dist:
                del pre_fin, pre_all
                pre_fin, diff= self.lib_fin.get(key).copy(), d 
                pre_all = self.lib_all.get(key, self.lib_fin.iterid).copy()
                dist = Hamming_Dist(d,0, self.bit_len)   
        for i in range(self.bit_len):
            if (point&diff)!=0:
                for j in range(self.bit_len):
                    new = Control_lines(self.bit_len)
                    for targ in pre_all[j]:
                        if targ|point ==targ: continue
                        new.add(targ|point, j+1)
                        if not self.unable.contain(targ|point, j+1):
                            pre_fin.add(targ|point, j+1)
                    pre_all.union(new)
                #print('new', new)
            point *= 2
            #print(result) 
        self.lib_all.add(aim, pre_all)
        self.lib_fin.add(aim, pre_fin)
        #print(self.lib_fin)
        return pre_fin
        
            