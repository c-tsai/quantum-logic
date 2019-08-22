class TofoliGate:
    def __init__(self, control, inv, length):
        self.control= control
        self.inv = inv
        self.length= length
    def inf(self, bit):
        if bit == -1:
           return -1
        if bit&self.control == self.control:
            return bit^self.inv
        return bit
    def __str__(self):
        dot, xor, string= self.control, self.inv, '--'
        for i in range(self.length):
            if dot%2==1:
               string = string + '·--'
            elif xor%2==1:
               string = string + '⊕--'
            else:
               string = string + '---'
            dot, xor = dot//2, xor//2
        string = string +'\n'+'--'
        for i in range(self.length):
           string = string + '|--'
        return string


class SwapGate:
    def __init__(self, bit1, bit2, length):
        self.bit1= bit1
        self.bit2 = bit2
        self.length= length
    def inf(self, bit):
        if bit == -1:
            return -1
        b = bit|self.bit1|self.bit2
        if bit&self.bit1==0:
            b = b -self.bit2
        if bit&self.bit2==0:
            b = b - self.bit1
        return b
    def __str__(self):
        swap, string= self.bit1+self.bit2, '--'
        for i in range(self.length):
            if swap%2==1:
               string = string + 's--'
            else:
               string = string + '---'
            swap= swap//2
        string = string + '\n--'
        for i in range(self.length):
           string = string + '|--'
        return string

class QCircuit:
    def __init__(self, q_list):
        self.list= q_list
    def inf(self, bit):
        b = bit
        for q in self.list:
            b = q.inf(b)
        return b
    def __str__(self):
        string = ''
        for q in self.list:
           string = string + str(q)+'\n'
        return string
    def __len__(self):
        return len(self.list)
    def add(self, circuit, typ):
        if typ == 'f':
            self.list= self.list + circuit.list
        else:
            self.list = circuit.list+ self.list
    def reverse(self):
        self.list.reverse()
        result = QCircuit(self.list.copy())
        self.list.reverse()
        return result
    def cost(self, h_cost):
        return len(self)
            
'''
t= SwapGate(1,4,3)
l= QCircuit([t])
print(l)
print(l.inf(3))
'''
