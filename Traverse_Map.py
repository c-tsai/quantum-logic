# -*- coding: utf-8 -*-
"""
Created on Thu Dec  5 13:12:05 2019

@author: v-catsai
"""

class Node:
    def __init__(self, i):
        self.id = i
        self.pre = []
        self.de = []
        self.traversed = False
    def add_pre(self, node):
        self.pre = self.pre + [node]
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
        
        
class Traverse_Map:
    def __init__(self, bit_len):
        self.nodes = []
        for i in range(2**bit_len):
            self.nodes = self.nodes + [Node(i)]
        for n in self.nodes:
            point = 1
            for i in range(bit_len):
               if point|n.id != n.id: n.add_de(self.nodes[point|n.id])
               point *= 2
        self.available = set([0])
        
    def traverse(self, i):
        self.available.remove(i)
        new = self.nodes[i].traverse_add()
        self.available = self.available.union(new)