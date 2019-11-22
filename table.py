# -*- coding: utf-8 -*-
"""
Created on Tue Nov 19 15:35:08 2019

@author: v-catsai
"""
class Table:
    def __init__(self, length):
        self.dict = {}
        self.length = length
        
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
    #def __str__(self): return 'Table'
    
    def copy(self): 
        t = Table(self.length)
        t.dict= self.dict.copy()
        return t
    
    def summ(self):
        result = 0
        for i in self:
            result += self.__getitem__(i)
        return summ
            