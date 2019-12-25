# -*- coding: utf-8 -*-
"""
Created on Tue Dec 10 12:04:32 2019

@author: v-catsai
"""
from table import Hamming_Dist, bit_list
import copy as c

    
    
class Control_lines:
    def __init__(self, bit_len):
        self.lib= [set([]) for i in range(bit_len+1)]
        self.bit_len= bit_len
        self.iterid = -1
        self.mn = 0
        self.mx = 0
    def add(self, line, b_num=-1):
        b=0
        if b_num == -1:
            b=Hamming_Dist(line,0,self.bit_len)
            self.lib[b].add(line)
        else: 
            self.lib[b_num].add(line)
            b = b_num
        if b >self.mx: self.mx=b
        if b < self.mn: self.mn=b
    def pop(self, line, b_num=-1):
        b=0
        if b_num == -1:
            b =Hamming_Dist(line,0,self.bit_len)
            self.lib[b].discard(line)
        else: 
            self.lib[b_num].discard(line)
            b =b_num
        if b==self.mn and not self.lib[b]: 
            for i in range(self.mn+1,self.mx+1):
                if self.lib[i]:
                    self.mn = i
                    break
        elif b==self.mx and not self.lib[b]:
            for i in range(self.mx-1, self.mn-1, -1):
                if self.lib[i]:
                    self.mx=i
                    break        
                
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
    def max_group(self): return self.lib[self.mx]
    def min_group(self): return self.lib[self.mn]
    def __del__(self):
        for s in self.lib: del s
        del self.lib
        
    def __contains__(self, key): 
        return key in self.lib[Hamming_Dist(key,0,self.bit_len)]
    
    def contain(self, key, b_num): return key in self.lib[b_num] 
    
    def __len__(self): return len(self.lib)
    def __iter__(self):
        self.iterator, self.iterid= iter(self.lib[self.mn]), self.mn
        #print(self.iterid)
        return self
    def __next__(self):
        try:
            #print(self.iterid)
            res = next(self.iterator)
        except StopIteration:
            self.iterid += 1
            if self.iterid == (self.mx+1):
                self.iterid = -1
                raise StopIteration
            self.iterator = iter(self.lib[self.iterid])
            res = next(self)
        return res
    def union(self, new, b_num=-1):
        if b_num != -1:
            self.lib[b_num]= self.lib[b_num].union(new)
            if b_num >self.mx: self.mx = b_num
            if b_num < self.mn: self.mn = b_num
            return
        for i in range(new.mn,new.mx+1):
            self.lib[i]= self.lib[i].union(new[i])
        if new.mn < self.mn: self.mn = new.mn
        if new.mx > self.union: self.mx = new.mx
        
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
        else: self.lib[b_num].pop(line, None)
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
        self.lib = Control_lib(bit_len)
        self.smllst_b = 0
        self.allowed_num = 1
        self.each_num= [0 for i in range(self.bit_len+1)]
        
    def remove(self, targ):
        bit_num = Hamming_Dist(targ,0,self.bit_len)
        self.unable.add(targ, bit_num)
        self.each_num[bit_num] += 1
        #print(bit_num, self.smllst_b)
        if bit_num == self.smllst_b:
            #print('=============')
            if self.each_num[bit_num] == self.allowed_num:
                self.smllst_b += 1
                self.allowed_num *= (self.bit_len-self.smllst_b+1)
                self.allowed_num = self.allowed_num//(self.smllst_b)
                
    def combination(self, b_list, targ, num):
        if num==0: self.lib.add(targ,[0],0)
        if self.lib.contain(targ,num): return
        b = b_list[-1]
        if len(b_list)==num:
            self.lib.add(targ, [targ], num)
            return
        self.combination(b_list[:-1], targ-b, num-1)
        self.combination(b_list[:-1], targ-b, num)
        self.lib.add(targ, self.lib.get(targ-b,num)+[i+b for i in self.lib.get(targ-b,num-1)],num)
        
            
        
        
    def best_clines(self, bit1, controled):
        aim = (bit1|controled) - controled
        #print(bit1, controled, aim)
        if aim == 0: return [0]
        l = bit_list(aim, self.bit_len)
        b_num = len(l)
        result = []
        for i in range(self.smllst_b, b_num+1):
            self.combination(l,aim,i)
            #print(i, '--------')
            for b in self.lib.get(aim,i):
                #print(b)
                if not self.unable.contain(b,i):
                    result = result + [b]
            if result: return result
        #print(self.unable)
        #raise ValueError('no match for: bit_1= '+str(bit1)+', controled= '+ str(controled))
        return result
            
            
        
    
    def __del__(self):
        del self.unable
        del self.lib
            