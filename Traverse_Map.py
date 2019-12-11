# -*- coding: utf-8 -*-
"""
Created on Thu Dec  5 13:12:05 2019

@author: v-catsai
"""

class Node:
    def __init__(self, i, bit_len):
        self.id = i
        self.pre = []
        self.de = []
        self.traversed = False
        self.bit_len= bit_len
    def add_pre(self, node):
        self.pre = self.pre + [node]
    def add_all_pre(self, lib):
        point = 1
        for i in range(self.bit_len):
            if point|self.id == self.id:
                aim = (point|self.id) - point
                if not aim in lib: 
                    lib[aim]= Node(aim, self.bit_len) 
                    lib[aim].add_all_pre(lib)
                lib[aim].add_de(self)
                #print(aim, point, self.id, lib[aim].de)
            point *= 2
    def add_de(self, node):
        self.de = self.de + [node]
        node.add_pre(self)
    def traverse_add(self):
        self.traversed = True
        add = set([])
        for n in self.de:
            if n.traversed: continue
            block = True
            for m in n.pre: 
                if not m.traversed:
                    block =False
                    break
            if block: add.add(n.id)
        return add
    def __str__(self):
        string = '[' +str(self.id) + ' precending: '
        for n in self.pre: 
           string = string + str(n.id) + ','
        string = string + ';decending: '
        for n in self.de: 
           string = string + str(n.id) + ','
        string = string + ']'
        return string
        
            
        
        
        
class Traverse_Map:
    def __init__(self, bit_len):
        self.bit_len = bit_len
        self.nodes = {}
        n = Node(0, self.bit_len)
        self.nodes[0] = n
        self.available = set([0])
        
    def traverse(self, idx):
        self.available.remove(idx)
        point = 1
        for i in range(self.bit_len):
            if point&idx != point and not point|idx in self.nodes: 
                #print(point)
                self.nodes[point|idx]= Node(point|idx, self.bit_len)
                self.nodes[point|idx].add_all_pre(self.nodes)
            point *= 2
        #print(self.nodes[idx])        
        new = self.nodes[idx].traverse_add()
        self.available = self.available.union(new)