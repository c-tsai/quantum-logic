# -*- coding: utf-8 -*-
"""
Created on Tue Dec 10 16:02:06 2019

@author: v-catsai
"""
from Control_line import Control_lines_generator
c = Control_lines_generator(3)
a = c.able_clines(3,2)
print(a)
c.remove(1)
a = c.able_clines(7,2)
print(a)
c.remove(4)
a = c.able_clines(7,2)
print(a)