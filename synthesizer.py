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
       if table_b == 0:
           self.update_table_b()
       else: 
           self.table_b
       self.generate_all_c_line()

   def gate_syns(self, i_bit, f_bit, typ, control_min):
       b , result, fin_table= i_bit, QCircuit([]), self.table_f 
       h_cost= self.hamming_cost()
       if i_bit=='-' or f_bit=='-':
           return result, fin_table, h_cost
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
              temp.add(temp_c,typ,False)
              if temp.hamming_cost < cost:
                  result_gate, cost= temp_c, temp.hamming_cost()
                  fin_table= temp.table_f
              else: del temp_c
              del temp
          result.add(result_gate,typ)
          h_cost += cost
          del candi
       return result, fin_table, h_cost
        
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

   def add(self, circuit, typ, calc_table_b=True):
       if typ == 'f':
           for i in range(self.length):
               self.table_f[i]=circuit.inf(self.table_f[i])
           self.output_f.add(circuit, typ)
       else:
           temp = np.array(['-'for i in range(self.length)])
           for i in range(self.length):
               n= circuit.inf(i)
               if not n=='-': temp[i]=self.table_f[n] 
           self.table_f=temp
       update_total_hamming()
       if calc_table_b:
           update_table_b()

   def hamming_cost(self) 
       if self.total_hamming = 0:
          self.update_total_hamming()
       return np.sum(self.total_hamming)
  

