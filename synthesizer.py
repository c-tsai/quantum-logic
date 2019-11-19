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

def pla_reader(file):
    f = open(file, 'r')
    bit_len, i, array= 0, 0, 0
    for line in f:
        if i ==0 : 
            bit_len= int(line.split(' ')[0])
            array = -1*np.ones((2^bit_len), dtype=int)
        else: 
            res = line.split(' ')
            array[int(res[0],2)] = int(res[1],2)
        i += 1
        return array, bit_len

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


######################
# Private Functions ##
######################


   def gate_syns(self, i_bit, f_bit, typ, control_min, cost_typ):
       
       if not control_min: return self.gate_syns_2(i_bit, f_bit, typ)
       
       b, result, param= i_bit, QCircuit([]), self
       if i_bit==-1 or f_bit==-1:   return result, param
       while not b == f_bit:
          diff, point, candi= b^f_bit, 1, set([])
          for i in range(self.length):
              if not diff&point == 0:
                  for c in self.all_c_line:
                      if c&point == 0 and c&b==c:
                          candi.add(TofoliGate(c,point,self.bit_len))
                          #if not control_min: break
                  #if not len(candi)==0 and not control_min: break
              point *=2

          result_gate, cost_q, cost_h, control_num= 0, 100000000, 10000000, 10000000
          for q in candi:
              temp = QCSynthesizer(self.table_f, self.bit_len, self.table_b)
              temp_c = copy.deepcopy(result)
              temp_c.add(QCircuit([q]), 'f')
              temp.add(temp_c,typ)
              t_h, t_control = temp.hamming_cost(), q.control_num()
              t_q = temp_c.cost(t_h, cost_typ)
              #print(t_q)
              #print('---------')
              
              if t_q < cost_q:
                  result_gate, cost_q, cost_h, control_num = temp_c, t_q, t_h, t_control
                  del param
                  param= temp
              elif t_q==cost_q and t_h < cost_h:
                  result_gate, cost_q, cost_h, control_num = temp_c, t_q, t_h, t_control
                  del param
                  param= temp
              elif t_q==cost_q and t_h == cost_h and t_control < control_num:
                  result_gate, cost_q, cost_h, control_num = temp_c, t_q, t_h, t_control
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
   
   def gate_syns_2(self, i_bit, f_bit, typ):
       result= QCircuit([])
       if i_bit==-1 or f_bit==-1:   return result, self
       diff_1, diff_0, point= (i_bit^f_bit)&i_bit, (i_bit^f_bit)&f_bit, 1
       param = QCSynthesizer(self.table_f, self.bit_len, self.table_b)
       for i in range(self.length):
           if not diff_0&point == 0:
               result.add(QCircuit([TofoliGate(i_bit, point, self.bit_len)]), 'f')
           point *=2
       point= 1 
       for i in range(self.length):
           if not diff_1&point == 0:
               result.add(QCircuit([TofoliGate(f_bit, point, self.bit_len)]), 'f')
           point *=2  
       param.add(result, typ)
       return result, param


   def select_b_or_f(self, targ, control_min, cost_typ):
       if self.table_b[targ]== -1:
           c, p =self.gate_syns(self.table_f[targ], targ, 'f', control_min, cost_typ)
           return c, p , 'f'
       if self.table_f[targ]== -1:
           c, p =  self.gate_syns(targ, self.table_b[targ], 'b', control_min, cost_typ)
           return c, p, 'b'
       circuit_f, param_f= self.gate_syns(self.table_f[targ], 
                                          targ, 'f', control_min, cost_typ)
       circuit_b, param_b= self.gate_syns(targ, self.table_b[targ], 
                                          'b', control_min, cost_typ)
       #print(circuit_b)
       #print('    ')
       #print(circuit_f)
       if circuit_b.cost(param_b.hamming_cost(), cost_typ) < circuit_f.cost(param_f.hamming_cost(), cost_typ) :
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


######################
# Public Functionss ##
######################


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
   

#####################################
# The Algorithm's Helper Function  ##
#####################################


   def given_order_alg(self, order, control_min, direction, cost_typ):
       for targ in order:
           #print(targ)
           circuit, param, typ= 0, 0, 'f'
           if direction=='bi':
               circuit, param, typ = self.select_b_or_f(targ, control_min, cost_typ)
           else:
               circuit, param= self.gate_syns(self.table_f[targ], targ, 'f', control_min, cost_typ)
           #print(circuit)
           #print(param.table_f, param.table_b, typ)
           #print('           ')
           self.add(circuit, typ, param.table_b, param.table_f, param.total_hamming)
           self.all_c_line.remove(targ)
       #for targ in order:
       #    if not (self.table_f[targ]==targ or self.table_f[targ]==-1):
       #        raise ValueError(targ, self.table_f[targ])
               
   def dynamic_proto(self, pick_func, control_min, direction, cost_typ):
       circuit, param, typ = 0, 0, 'f'
       if direction=='bi':
           circuit, param, typ = self.select_b_or_f(0, control_min, cost_typ)
       else:
           circuit, param= self.gate_syns(self.table_f[0], 0, 'f', control_min, cost_typ)
       self.add(circuit, typ, param.table_b, param.table_f, param.total_hamming)
       self.all_c_line.remove(0)
       candi, done = set([]), set([0])
       for i in range(self.bit_len):
           candi.add(2**i)
       while candi:
           circuit, param, targ, typ = pick_func(candi, control_min, direction, cost_typ)
           #print(candi)
           self.add(circuit, typ, param.table_b, param.table_f, param.total_hamming)
           self.all_c_line.remove(targ)
           new = set([])
           for t in done: new.add(t|targ)
           done.add(targ)
           candi = candi.union(new)-done
           
           
   def BFS(self, candi, control_min, direction, cost_typ):
       weight, targ= 100000, -1
       for t in candi:
            w = Hamming_Dist(t, 0, self.bit_len)
            if w < weight: targ, weight = t, w
       circuit, param, typ = 0, 0, 'f'
       if direction=='bi':
           circuit, param, typ = self.select_b_or_f(targ, control_min, cost_typ)
       else:
           circuit, param= self.gate_syns(self.table_f[targ], targ, 'f', control_min, cost_typ)
       return circuit, param, targ, typ
       
   
   def Dym(self, candi, control_min, direction, cost_typ):
       cost, circuit, param, targ, typ= 10000000, 0, 0, -1, 'f'
       for t in candi:
           circuit_t, param_t, typ_t = 0, 0, 0
           if direction == 'bi':
               circuit_t, param_t, typ_t = self.select_b_or_f(t, control_min, cost_typ)
           else:
               circuit_t, param_t= self.gate_syns(self.table_f[t], t, 'f', control_min, cost_typ)
               typ_t = 'f'
           c = circuit_t.cost(param_t.hamming_cost(), cost_typ)
           if c < cost: 
               del circuit, param
               cost, circuit, param, targ, typ = c, circuit_t, param_t, t, typ_t
           else:
               del circuit_t, param_t
       return circuit, param, targ, typ
       
               
   def permuting(self, alg, para, control_min, direction, cost_typ):
       q= QCSynthesizer(self.table_f, self.bit_len, self.table_b)
       q.algorithm_selector(alg, para, control_min, direction, cost_typ)
       h_cost, qc= q.hamming_cost(), q.output_circuit()
       self.output_b, self.output_f, cost = q.output_b, q.output_f, qc.cost(h_cost, cost_typ)
       for i in range( self.bit_len-1):
           for j in range(i+1, self.bit_len):
               q= QCSynthesizer(self.table_f, self.bit_len, self.table_b)
               q.add(QCircuit([SwapGate(2**i, 2**j, self.bit_len)]), 'f')
               q.algorithm_selector(alg, para, control_min, direction, cost_typ)
               h_cost = q.hamming_cost()
               qc = q.output_circuit()
               if qc.cost(h_cost, cost_typ) < cost:
                   self.output_b, self.output_f, cost= q.output_b, q.output_f, qc.cost(h_cost)
      
   def algorithm_selector(self, string, para, control_min, direction, cost_typ):
       if string == 'given_order_alg':
           self.given_order_alg(para, control_min, direction, cost_typ)
       elif string == 'BFS':
           self.dynamic_proto(self.BFS, control_min, direction, cost_typ)
       elif string == 'Dym':
           self.dynamic_proto(self.Dym, control_min, direction, cost_typ)
           

###################
# The Algorithms ##
###################
          
 
   def Order_Algorithm(self, order, permute=True, control_min=True, direction='bi', cost_typ= 'length'):
       if permute: 
           self.permuting('given_order_alg', order, control_min, direction, cost_typ)
       else:
           self.given_order_alg(order, control_min, direction, cost_typ)
           

   def DFS_Algorithm(self, permute=True, control_min=True, direction='bi', cost_typ='length'):
       if permute: 
           self.permuting('given_order_alg', range(self.length), control_min, direction, cost_typ)
       else:
           self.given_order_alg(range(self.length), control_min, direction, cost_typ)


   def BFS_Algorithm(self, permute=True, control_min=True, direction='bi', cost_typ='length'):
       if permute: 
           self.permuting('BFS', [], control_min, direction, cost_typ)
       else:
           self.dynamic_proto(self.BFS, control_min, direction, cost_typ)


   def Dym_Algorithm(self, permute=True, control_min=True, direction='bi', cost_typ='length'):
       if permute: 
           self.permuting('Dym', [], control_min, direction, cost_typ)
       else:
           self.dynamic_proto(self.Dym, control_min, direction, cost_typ)
       
           
  
'''
table = np.array([-1,6,5,-1,-1,0,-1,2])
q= QCSynthesizer(table, 3)
q.BFS_Algorithm(permute=True, control_min=True, direction='bi')
qc = q.output_circuit()
#print(q.output_b)
#print(q.output_f)
print(qc)
print(qc.cost(0,"NCV-012"))
for i in range(8):
    print(qc.inf(i))
'''