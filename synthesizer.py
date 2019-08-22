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
       self.length = self.table_f.shape[0]
       self.bit_len= bit_len
       self.output_f = QCircuit([])
       self.output_b = QCircuit([])
       self.table_b = np.array([-1 for i in range(self.length)])
       self.total_hamming= 0
       if isinstance(table_b, int): self.update_table_b()
       else: self.table_b= table_b.copy()
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
       #print(circuit_b)
       #print('    ')
       #print(circuit_f)
       if len(circuit_f)==0 or len(circuit_b)!=0 and circuit_b.cost(param_b.hamming_cost()) < circuit_f.cost(param_f.hamming_cost()) :
           return circuit_b, param_b, 'b'
       else: return circuit_f, param_f, 'f'
       

        
   def generate_all_c_line(self):
       self.all_c_line= set([])
       for i in range(self.length):
           self.all_c_line.add(i)
   def update_total_hamming(self):
       self.total_hamming= np.zeros(self.table_f.shape)
       for i in range(self.length):
           self.total_hamming= Hamming_Dist(i, self.table_f[i], self.bit_len)
   def update_table_b(self):
       del self.table_b
       self.table_b = np.array([-1 for i in range(self.length)])
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

   def given_order_alg(self, order, control_min, direction):
       for targ in order:
           #print(targ)
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
               
   def dynamic_proto(self, pick_func, control_min, direction):
       circuit, param, typ = 0, 0, 'f'
       if direction=='bi':
           circuit, param, typ = self.select_b_or_f(0, control_min)
       else:
           circuit, param= self.gate_syns(self.table_f[0], 0, 'f', control_min)
       self.add(circuit, typ, param.table_b, param.table_f, param.total_hamming)
       self.all_c_line.remove(0)
       candi = set([])
       for i in range(self.bit_len):
           candi.add(2**i)
       while candi.size != 0:
           circuit, param, targ, typ = pick_func(candi)
           self.add(circuit, typ, param.table_b, param.table_f, param.total_hamming)
           self.all_c_line.remove(targ)
           candi.remove(targ)
           new = set([])
           for t in candi: new.add(t|targ)
           candi = candi.union(new)
           
           
   def BFS(self, candi, control_min, direction):
       weight, targ= 100000, -1
       for t in candi:
            w = Hamming_Dist(t, 0, self.bit_len)
            if w < weight: targ, weight = t, w
       circuit, param, typ = 0, 0, 'f'
       if direction=='bi':
           circuit, param, typ = self.select_b_or_f(targ, control_min)
       else:
           circuit, param= self.gate_syns(self.table_f[targ], targ, 'f', control_min)
       return circuit, param, targ, typ
       
   
   def Dym(self, candi, control_min, direction):
       cost, circuit, param, targ= 0, 0, 0, -1
       for t in candi:
           if direction == 'bi':
               circuit
       
               
   def permuting(self, alg, para, control_min, direction):
       q= QCSynthesizer(self.table_f, self.bit_len, self.table_b)
       q.algorithm_selector(alg, para, control_min, direction)
       h_cost, qc= q.hamming_cost(), q.output_circuit()
       self.output_b, self.output_f, cost = q.output_b, q.output_f, qc.cost(h_cost)
       for i in range( self.bit_len-1):
           for j in range(i+1, self.bit_len):
               q= QCSynthesizer(self.table_f, self.bit_len, self.table_b)
               q.add(QCircuit([SwapGate(2**i, 2**j, self.bit_len)]), 'f')
               q.algorithm_selector(alg, para, control_min, direction)
               h_cost = q.hamming_cost()
               qc = q.output_circuit()
               if qc.cost(h_cost) < cost:
                   self.output_b, self.output_f, cost= q.output_b, q.output_f, qc.cost(h_cost)
      
   def algorithm_selector(self, string, para, control_min, direction):
       if string == 'given_order_alg':
           self.given_order_alg(para, control_min, direction)
           
           
   def Order_Algorithm(self, order, permute=True, control_min=True, direction='bi'):
       if permute: 
           self.permuting('given_order_alg', order, control_min, direction)
       else:
           self.given_order_alg(order, control_min, direction)
           

       
           
  

table = np.array([-1,6,5,-1,-1,0,-1,2])
q= QCSynthesizer(table, 3)
q.Order_Algorithm(range(8), True)
qc = q.output_circuit()
#print(q.output_b)
#print(q.output_f)
print(qc)
for i in range(8):
    print(qc.inf(i))

