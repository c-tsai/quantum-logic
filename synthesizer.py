import copy
from gates import TofoliGate, SwapGate, QCircuit
from table import Table, Hamming_Dist
from Control_line  import Control_lines_generator
from Traverse_Map import Traverse_Map




class QCSynthesizer:

   def __init__(self, table, bit_len, table_b=0):
       self.table_f= table.copy()
       self.length = self.table_f.length
       self.bit_len= bit_len
       self.output_f = QCircuit([])
       self.output_b = QCircuit([])
       self.table_b = Table(bit_len)
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
          #print(b, f_bit, typ, self.table_f, self.table_b)
          diff, point, candi= b^f_bit, 1, set([])
          for i in range(self.bit_len):
              if not diff&point == 0:
                  for c in self.all_c_line:
                      if c&point == 0 and c&b==c:
                          candi.add(TofoliGate(c,point,self.bit_len))
                          #if not control_min: break
                  #if not len(candi)==0 and not control_min: break
              point *=2

          result_gate, cost_q, cost_h, control_num= 0, 100000000, 10000000, 10000000
          for q in candi:
              #print(q, '\n')
              temp = QCSynthesizer(self.table_f, self.bit_len, self.table_b)
              temp_c = copy.deepcopy(result)
              temp_c.add(QCircuit([q]), 'f')
              temp.add(temp_c,typ)
              t_h, t_control = temp.hamming_cost().summ(), q.control_num()
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
          #print(param.table_f, b, f_bit, typ)
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
       #print(i_bit, f_bit, typ, self.table_f)
       for i in range(self.bit_len):
           if not diff_0&point == 0:
               result.add(QCircuit([TofoliGate(i_bit, point, self.bit_len)]), 'f')
           point *=2
       point= 1 
       for i in range(self.bit_len):
           if not diff_1&point == 0:
               result.add(QCircuit([TofoliGate(f_bit, point, self.bit_len)]), 'f')
           point *=2
       param.add(result, typ)
       return result, param


   def select_b_or_f(self, targ, control_min, cost_typ):
       #print(self.table_f)
       if self.table_b[targ]== -1:
           c, p =self.gate_syns(self.table_f[targ], targ, 'f', control_min, cost_typ)
           return c, p , 'f'
       if self.table_f[targ]== -1:
           c, p =  self.gate_syns(targ, self.table_b[targ], 'b', control_min, cost_typ)
           return c, p, 'b'
       circuit_f, param_f= self.gate_syns(self.table_f[targ], 
                                          targ, 'f', control_min, cost_typ)
       #print('f', self.table_f)
       circuit_b, param_b= self.gate_syns(targ, self.table_b[targ], 
                                          'b', control_min, cost_typ)
       #print('b', self.table_f)
       #print(circuit_b)
       #print('    ')
       #print(circuit_f)
       if circuit_b.cost(param_b.hamming_cost(), cost_typ) < circuit_f.cost(param_f.hamming_cost(), cost_typ) :
           #print('b')
           return circuit_b, param_b, 'b'
       else:
           #print('f')
           return circuit_f, param_f, 'f'
       

        
   def generate_all_c_line(self):
       self.all_c_line= Control_lines_generator(self.bit_len)
   def update_total_hamming(self):
       self.total_hamming= Table(self.length)
       for i in self.table_f:
           self.total_hamming[i]= Hamming_Dist(i, self.table_f[i], self.bit_len)
   def update_table_b(self):
       del self.table_b
       self.table_b = Table(self.length)
       for i in self.table_f:
           self.table_b[self.table_f[i]]= i
   def update_table_f(self):
       del self.table_f
       self.table_f = Table(self.length)
       for i in self.table_b:
           self.table_f[self.table_b[i]]= i


######################
# Public Functionss ##
######################


   def add(self, circuit, typ, table_b=0, table_f=0, table_h=0):
       #print(self.table_f, '\n', circuit, typ)
       if not isinstance(table_f, int): self.table_f= table_f.copy()
       if not isinstance(table_b, int): self.table_b= table_b.copy()
       elif typ == 'f':
           temp = Table(self.length)
           for i in self.table_f:
               temp[i]=circuit.inf(self.table_f[i])
               #print(i, self.table_f[i], temp)
           self.table_f = temp
       else:
           for q in circuit.reverse():
               #print(q)
               temp = Table(self.length)
               for i in self.table_b:
                   temp[i]=q.inf(self.table_b[i])
               #print(i, self.table_b[i], temp)
               self.table_b = temp
           self.update_table_f()
       if typ == 'f':
           self.output_f.add(circuit, typ)
       else:
           self.output_b.add(circuit, typ)
       #print(self.table_f)
       if not isinstance(table_h, int): self.total_hamming= table_h.copy()
       else: self.update_total_hamming()
       if not isinstance(table_b, int): self.table_b= table_b.copy()
       else:  self.update_table_b()
       #print(self.table_f, '------------------')

   def hamming_cost(self):
       if self.total_hamming == 0:
          self.update_total_hamming()
       return self.total_hamming
   
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
       #print(0, self.table_b)
       self.all_c_line.remove(0)
       t_map = Traverse_Map(self.bit_len)
       t_map.traverse(0)
       self.order= [0]
       while t_map.available:
           circuit, param, targ, typ = pick_func(t_map.available, control_min, direction, cost_typ)
           #print(candi)
           self.add(circuit, typ, param.table_b, param.table_f, param.total_hamming)
           #print(targ, self.table_b)
           self.all_c_line.remove(targ)
           t_map.traverse(targ)
           self.order = self.order + [targ]
           
           
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
  
     
   def DFS(self, candi, control_min, direction, cost_typ):
       targ = min(candi)
       circuit, param, typ = 0, 0, 'f'
       if direction=='bi':
           circuit, param, typ = self.select_b_or_f(targ, control_min, cost_typ)
       else:
           circuit, param= self.gate_syns(self.table_f[targ], targ, 'f', control_min, cost_typ)
       return circuit, param, targ, typ
   
    
   def Dym(self, candi, control_min, direction, cost_typ):
       cost, hamm, circuit, param, targ, typ= 10000000, 10000000, 0, 0, -1, 'f'
       for t in candi:
           circuit_t, param_t, typ_t = 0, 0, 0
           if direction == 'bi':
               circuit_t, param_t, typ_t = self.select_b_or_f(t, control_min, cost_typ)
           else:
               circuit_t, param_t= self.gate_syns(self.table_f[t], t, 'f', control_min, cost_typ)
               typ_t = 'f'
           c = circuit_t.cost(param_t.hamming_cost(), cost_typ)
           h = param_t.hamming_cost().summ()
           if c < cost: 
               del circuit, param
               cost, hamm, circuit, param, targ, typ = c, h, circuit_t, param_t, t, typ_t
           elif c == cost and h < hamm: 
               del circuit, param
               cost, hamm, circuit, param, targ, typ = c, h, circuit_t, param_t, t, typ_t
           else:
               del circuit_t, param_t
       return circuit, param, targ, typ
   
    
   def Dym_DFS(self, candi, control_min, direction, cost_typ):
       cost_q, cost_h, circuit, param, targ, typ= 10000000, 10000000, 0, 0, -1, 'f'
       control_num = 0
       for t in candi:
           circuit_t, param_t, typ_t = 0, 0, 0
           control_num_t = Hamming_Dist(t,0,self.bit_len)
           if control_num_t < control_num: continue
           if direction == 'bi':
               circuit_t, param_t, typ_t = self.select_b_or_f(t, control_min, cost_typ)
           else:
               circuit_t, param_t= self.gate_syns(self.table_f[t], t, 'f', control_min, cost_typ)
               typ_t = 'f'
           c = circuit_t.cost(param_t.hamming_cost(), cost_typ)
           h = param_t.hamming_cost()
           if control_num_t > control_num:
               del circuit, param
               cost_q, cost_h, circuit, param, targ, typ = c, h, circuit_t, param_t, t, typ_t
               control_num = control_num_t
           elif c < cost_q: 
               del circuit, param
               cost_q, cost_h, circuit, param, targ, typ = c, h, circuit_t, param_t, t, typ_t
           elif c==cost_q and h < cost_h: 
               del circuit, param
               cost_q, cost_h, circuit, param, targ, typ = c, h, circuit_t, param_t, t, typ_t
           else:
               del circuit_t, param_t
       return circuit, param, targ, typ
       
               
   def permuting(self, alg, para, control_min, direction, cost_typ):
       q= QCSynthesizer(self.table_f, self.bit_len, self.table_b)
       q.algorithm_selector(alg, para, control_min, direction, cost_typ)
       h_cost, qc= q.hamming_cost().summ(), q.output_circuit()
       self.output_b, self.output_f, cost = q.output_b, q.output_f, qc.cost(h_cost, cost_typ)
       for i in range( self.bit_len-1):
           for j in range(i+1, self.bit_len):
               q= QCSynthesizer(self.table_f, self.bit_len, self.table_b)
               q.add(QCircuit([SwapGate(2**i, 2**j, self.bit_len)]), 'f')
               q.algorithm_selector(alg, para, control_min, direction, cost_typ)
               h_cost = q.hamming_cost().summ()
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
       elif string == 'DFS':
           self.dynamic_proto(self.DFS, control_min, direction, cost_typ)
       elif string == 'Dym_DFS':
           self.dynamic_proto(self.Dym_DFS, control_min, direction, cost_typ)
           

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
           self.permuting('DFS', [], control_min, direction, cost_typ)
       else:
           self.dynamic_proto(self.DFS, control_min, direction, cost_typ)


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
   
    
   def Dym_DFS_Algorithm(self, permute=True, control_min=True, direction='bi', cost_typ='length'):
       if permute: 
           self.permuting('Dym_DFS', [], control_min, direction, cost_typ)
       else:
           self.dynamic_proto(self.Dym_DFS, control_min, direction, cost_typ)
           
  
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