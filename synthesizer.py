import numpy as np
from gates import TofoliGate, SwapGate, QCircuit



def Hamming_Dist(bit1, bit2, bit_len):
    if bit1=='-' or bit2=='-':
        return 0
    diff, count= bit1^bit2, 0
    for i in range(bit_len):
        if diff%2==1:
            count += 1
        diff = diff//2
    return count


class QCSynthesizer:

   def __init__(self, table, bit_len, table_b=0):
       self.table_f=table
       self.length = table.shape[0]
       self.bit_len= bit_len
       self.output_f = QCircuit([])
       self.output_b = QCircuit([])
       self.table_b = np.array(['-' for i in range(self.length)])
       self.total_hamming= 0
       if table_b == 0: self.update_table_b()
       else: self.table_b
       self.generate_all_c_line()


   def gate_syns(self, i_bit, f_bit, typ, control_min):
       b , result, param= i_bit, QCircuit([]), self
       if i_bit=='-' or f_bit=='-':   return result, param
       while not b == f_bit:
          diff, point, candi= b^f_bit, 1, set([])
          for i in range(length):
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
              temp_c = QCircuit([q])
              temp.add(temp_c,typ,1)
              if temp.hamming_cost < cost:
                  result_gate, cost= temp_c, temp.hamming_cost()
                  del param
                  param= temp
              else: del temp_c
              del temp
          result.add(result_gate,typ)
          del candi
       return result, param


   def select_b_or_f(self, targ, control_min):
       circuit_f, param_f= self.gate_syns(self.table_f[targ], 
                                          targ, 'f', control_min)
       circuit_b, param_b= self.gate_syns(targ, self.table_b[targ], 
                                          'b', control_min)
       if len(circuit_b)==0 or circuit_f.cost(param_f.hamming_cost()) < circuit_b.cost(param_b.haming_cost) :
           self.add(circuit_f, 'f', param_f.table_b, 
                    param_f.table_f, param_f.hamming_total)
       else: self.add(circuit_b, 'b', param_b.table_b, 
                    param_b.table_f, param_b.hamming_total)
       

        
   def generate_allowed_control_line(self):
       self.all_c_line= set([])
       for i in range(self.length):
           self.all_c_line.add(i)
   def update_total_hamming(self):
       self.total_hamming= np.zeros(table.shape)
       for i in range(self.length):
           self.total_hamming= Hamming_Dist(i, self.table_f, self.bit_len)
   def update_table_b(self):
       for i in range(self.length):
           if not self.table_f[i]=='-':
               self.table_b[self.table]= i

   def add(self, circuit, typ, table_b=0, table_f=0, table_h=0):
       if table_f != 0: self.table_f= table_f
       elif typ == 'f':
           for i in range(self.length):
               self.table_f[i]=circuit.inf(self.table_f[i])
           self.output_f.add(circuit, typ)
       else:
           temp = np.array(['-'for i in range(self.length)])
           for i in range(self.length):
               n= circuit.inf(i)
               if not n=='-': temp[i]=self.table_f[n] 
           self.table_f=temp
       if table_h != 0: self.total_hamming= table_h
       else: update_total_hamming()
       if table_b != 0: self.table_b= table_b
       else:  update_table_b()

   def hamming_cost(self) 
       if self.total_hamming = 0:
          self.update_total_hamming()
       return np.sum(self.total_hamming)
  

