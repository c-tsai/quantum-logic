# -*- coding: utf-8 -*-
"""
Created on Thu Aug 22 15:15:31 2019

@author: v-catsai
"""
import os 
from synthesizer import QCSynthesizer
from table import pla_reader
import time

folder ='revlib_spec'
#f= open('BFS_NCV111_cont_revlib_result.txt', 'w+')
#f.write('q_cost,gate_len,NCV-111,NCV-012,NCV-155'+'\n')
print('Dym_NCV-111')

for spec in os.listdir(folder):
    #f.write(spec)
    print(spec)
    table, bit_len = pla_reader( os.path.join(folder, spec))
    #print(array.shape, bit_len)
    Q= QCSynthesizer(table, bit_len)
    start = time.time()
    Q.Dym_Algorithm(permute=False, control_min=False, direction= 'bi', cost_typ='NCV-111')
    QC= Q.output_circuit()
    tol = time.time()-start
    #print string
    for i in table:
        if not table[i]== QC.inf(i):
            raise ValueError('wrong result '+ str(table[i])+ ' to '+ str(QC.inf(i)))
    #print string[-4]
    #print(QC)
    #f.write(','+str(QC.cost(0,'length')))
    #f.write(','+str(QC.cost(0,'NCV-111')))
    #f.write(','+str(QC.cost(0,'NCV-012')))
    #f.write(','+str(QC.cost(0,'NCV-155')))
    print(QC.cost(0,'length'), QC.cost(0,'NCV-111'), QC.cost(0,'NCV-012'), QC.cost(0,'NCV-155'), tol)
    print(QC.dict)
    #f.write('\n')
    
#f.close()

    

