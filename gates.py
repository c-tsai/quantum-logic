class TofoliGate:
    def __init__(self, control, inv, length):
        self.control= control
        self.inv = inv
        self.length= length
        self.num = '-'
    def inf(self, bit):
        if bit == -1:
           return -1
        if bit&self.control == self.control:
            return bit^self.inv
        return bit
#    def cost():
#        count, c =0, self.control
    def control_num(self):
        if self.num != '-':
            return self.num
        num, point = 0, 1
        for i in range(self.length):
            if self.control&point == point:
                num += 1
            point *= 2
        self.num = num
        return num
    def __eq__(self, other):
       return self.control==other.control and self.inv==other.inv \
              and self.length==other.length
    def __hash__(self):
        return hash((2^self.length*self.control)+self.inv)
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
    def control_num(self):
        return 0
    def __eq__(self, other):
        return self.bit1==other.bit1 and self.bit2==other.bit2 \
               and self.length==other.length
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
        self.dict = {}
        for q in q_list:
            if not str(q.control_num()) in self.dict:
                self.dict[str(q.control_num())] = 1
            else: self.dict[str(q.control_num())] += 1
    def inf(self, bit):
        b = bit
        for q in self.list:
            b = q.inf(b)
        return b
    def __iter__(self):
        self.iterator = iter(self.list)
        return self
    def __next__(self):return next(self.iterator)
    def __str__(self):
        string = ''
        for q in self.list:
           string = string + str(q)+'\n'
        return string
    def __len__(self):
        return len(self.list)
    def __eq__(self, other):
        if len(self) != len(other): return False
        for i in range(len(self)):
            if not self.list[i]==other.list[i]:return False
        return True
    def add(self, circuit, typ):
        if typ == 'f':
            self.list= self.list + circuit.list
        else:
            self.list = circuit.list+ self.list
        for key in circuit.dict:
            if not key in self.dict:
                self.dict[key] = circuit.dict[key]
            else: self.dict[key] += circuit.dict[key]
    def reverse(self):
        self.list.reverse()
        result = QCircuit(self.list.copy())
        self.list.reverse()
        return result
    def cost(self, h_cost, typ='length'):
        if typ == 'length':
            #print(len(self))
            return len(self)
        if typ == 'Hamming':
            return h_cost
        if typ[:3]=='NCV':
            result = 0
            table = [0,1,1,5,13,29,61,125,253,509,1021]
            n_cost, c_cost, v_cost = int(typ[-3]), int(typ[-2]), int(typ[-1])
            for key in self.dict:
                if key == '0':
                   result += n_cost*self.dict['0']
                elif key == '1':
                   result += c_cost*self.dict['1']
                elif key == '2':
                   result += (2*c_cost + 3*v_cost)*self.dict['2']
                elif int(key) <= 10:
                    result += table[int(key)]*self.dict[key]
                else: result += (1<<int(key))-3
                #print(result)
            #print("complete")
            return result 
            
            
'''
t= SwapGate(1,4,3)
l= QCircuit([t])
print(l)
print(l.inf(3))
'''
