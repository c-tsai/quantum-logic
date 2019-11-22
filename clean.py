# -*- coding: utf-8 -*-
"""
Created on Fri Nov 22 13:47:32 2019

@author: v-catsai
"""
import os
import sys

name = sys.argv[1]

F = open(name, 'w+')
F.close()
os.remove(name)