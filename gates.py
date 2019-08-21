import numpy as np

class TofoliuGate:
    def __init__(self, control, inv, length):
        self.control= control
        self.inv = inv
        self.length= length
    def inf(self, bit):
        if bit&self.control == self.control:
            return bit^self.inv
        return bit
    def print(self):
        dot, xor, string= self.control, self.inv, '--'
        for i in self.length:
            if dot%2==1:
               string = string + '·--'
            elif xor%2==1:
               string = string + '⊕--'
            else:
               string = string + '---'
            dot, xor = dot//2, xor//2
        print(string)
        string = '--'
        for i in self.length
           string = string + '|--'
        print(string)


class SwapGate:
    def __init__(self, bit1, bit2, length):
        self.bit1= bit1
        self.bit2 = bit2
        self.length= length
    def inf(self, bit):
        b = bit|self.bit1|self.bit2
        if bit&self.bit1==0:
            b = b -self.bit2
        if bit&self.bit2==0:
            b = b - self.bit1
        return b
    def print(self):
        swap, string= self.bit1+self.bit2, '--'
        for i in self.length:
            if swap%2==1:
               string = string + 's--'
            else:
               string = string + '---'
            swap= swap//2
        print(string)
        string = '--'
        for i in self.length
           string = string + '|--'
        print(string)
