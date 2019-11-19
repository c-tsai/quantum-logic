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
    def copy(self): return Table(self.length, self.dict.copy())