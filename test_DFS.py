# -*- coding: utf-8 -*-
"""
Created on Tue Dec  3 14:07:08 2019

@author: v-catsai
"""
bit_len = 4


def num_1(targ, bit_l):
    point, result= 1, 0
    for i in range(bit_l):
        if targ &point != 0: result += 1
        point *= 2
    return result

def num_1_summ(l, bit_l):
    result = 0
    for i in l: result += num_1(i, bit_l)
    return result

def num_1_min(l, bit_l):return min([num_1(i, bit_l) for i in l])

def remove_DFS(l, bit_l):
    lines.remove(min(lines))
    
def remove_BFS(l, bit_l):
    num_1s = [num_1(i, bit_l) for i in l]
    l.pop(num_1s.index(min(num_1s)))


lines = [i for i in range(2**bit_len)]
result = 0
while lines != []:
    result += num_1_summ(lines, bit_len)
    remove_DFS(lines, bit_len)
print(result)