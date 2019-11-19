# -*- coding: utf-8 -*-
"""
Created on Tue Nov 19 15:35:08 2019

@author: v-catsai
"""
class Table:
    def __init__(self):
        self.dict = {}
    def __getitem__(self, key):
        if key in self.dict: return self.dict[key]
        else: return -1
    def __setitem__(self, key, value):
        self.dict[key]= value
    def __del__(self):
        del self.dict