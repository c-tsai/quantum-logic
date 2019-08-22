import numpy as np
import copy
from gates import TofoliGate, SwapGate, QCircuit



def Hamming_Dist(bit1, bit2, bit_len):
    if bit1==-1 or bit2==-1:
        return 0
    diff, count= bit1^bit2, 0
    #print(diff)
    for i in range(bit_len):
        if diff%2==1:
            count += 1
        diff = diff//2
    return count


class QCSynthesizer:

   def __init__(self, table, bit_len, table_b=0):
       self.table_f=table.copy()
       self.length = table.shape[0]
       self.bit_len= bit_len
       self.output_f = QCircuit([])
       self.output_b = QCircuit([])
       self.table_b = np.array([-1 for i in range(self.length)])
       self.total_hamming= 0
       if isinstance(table_b, int): self.update_table_b()
       else: self.table_b.copy()
       self.generate_all_c_line()


   def gate_syns(self, i_bit, f_bit, typ, control_min):
       b, result, param= i_bit, QCircuit([]), self
       if i_bit==-1 or f_bit==-1:   return result, param
       while not b == f_bit:
          diff, point, candi= b^f_bit, 1, set([])
          for i in range(self.length):
              if not diff&point == 0:
                  for c in self.all_c_line:
                      if c&point == 0 and c&b==c:
                          candi.add(TofoliGate(c,point,self.bit_len))
                          if not control_min: break
                  if not len(candi)==0 and not control_min: break
              point *=2

          result_gate, cost= 0, 100000000
          for q in candi:
              temp = QCSynthesizer(self.table_f, self.bit_len, self.table_b)
              temp_c = copy.deepcopy(result)
              temp_c.add(QCircuit([q]), 'f')
              temp.add(temp_c,typ)
              if temp.hamming_cost() < cost:
                  result_gate, cost= temp_c, temp.hamming_cost()
                  del param
                  param= temp
              else: del temp_c
              del temp
          #print(param.table_f, b, f_bit)
          #print('           ')
          del candi, result
          result = result_gate
          b = result.inf(i_bit)
       return result, param


   def select_b_or_f(self, targ, control_min):
       circuit_f, param_f= self.gate_syns(self.table_f[targ], 
                                          targ, 'f', control_min)
       circuit_b, param_b= self.gate_syns(targ, self.table_b[targ], 
                                          'b', control_min)
       if len(circuit_b)==0 or circuit_f.cost(param_f.hamming_cost()) < circuit_b.cost(param_b.hamming_cost) :
           return circuit_f, param_f, 'f'
       else: return circuit_b, param_b, 'b'
       

        
   def generate_all_c_line(self):
       self.all_c_line= set([])
       for i in range(self.length):
           self.all_c_line.add(i)
   def update_total_hamming(self):
       self.total_hamming= np.zeros(self.table_f.shape)
       for i in range(self.length):
           self.total_hamming= Hamming_Dist(i, self.table_f[i], self.bit_len)
   def update_table_b(self):
       for i in range(self.length):
           if not self.table_f[i]==-1:
               self.table_b[self.table_f[i]]= i

   def add(self, circuit, typ, table_b=0, table_f=0, table_h=0):
       if not isinstance(table_f, int): self.table_f= table_f.copy()
       elif typ == 'f':
           for i in range(self.length):
               self.table_f[i]=circuit.inf(self.table_f[i])
       else:
           temp = np.array([-1 for i in range(self.length)])
           for i in range(self.length):
               n= circuit.inf(i)
               if not n=='-': temp[i]=self.table_f[n] 
           self.table_f=temp
       if typ == 'f':
           self.output_f.add(circuit, typ)
       else:
           self.output_b.add(circuit, typ)
       #print(self.table_f)
       if not isinstance(table_h, int): self.total_hamming= table_h.copy()
       else: self.update_total_hamming()
       if not isinstance(table_b, int): self.table_b= table_b.copy()
       else:  self.update_table_b()

   def hamming_cost(self):
       if self.total_hamming == 0:
          self.update_total_hamming()
       return np.sum(self.total_hamming)
   
   def output_circuit(self):
       result = self.output_b.reverse()
       result.add(self.output_f.reverse(),'f')
       return result

   def given_order_alg(self, order, control_min=True, direction='bi'):
       for targ in order:
           circuit, param, typ= 0, 0, 'f'
           if direction=='bi':
               circuit, param, typ = self.select_b_or_f(targ, control_min)
           else:
               circuit, param= self.gate_syns(self.table_f[targ], targ, 'f', control_min)
           #print(circuit)
           #print(param.table_f, param.table_b, typ)
           #print('           ')
           self.add(circuit, typ, param.table_b, param.table_f, param.total_hamming)
           self.all_c_line.remove(targ)
       for targ in order:
           if not (self.table_f[targ]==targ or self.table_f[targ]==-1):
               raise ValueError(targ, self.table_f[targ])
               
           
  

table = np.array([1,6,5,3,4,0,7,2])
q= QCSynthesizer(table, 3)
q.given_order_alg(range(8), True)
qc = q.output_circuit()
#print(q.output_b)
#print(q.output_f)
print(qc)
for i in range(8):
    print(qc.inf(i))

