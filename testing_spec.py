# -*- coding: utf-8 -*-
"""
Created on Thu Aug 22 15:15:31 2019

@author: v-catsai
"""
import os 
from synthesizer import QCSynthesizer, pla_reader

folder ='revlib_spec'
f= open('BFS_NCV111_cont_revlib_result.txt', 'w+')
f.write('q_cost,gate_len,NCV-111,NCV-012,NCV-155'+'\n')

for spec in os.listdir(folder):
    f.write(spec)
    print(spec)
    array, bit_len = pla_reader( os.path.join(folder, spec))
    #print(array.shape, bit_len)
    Q= QCSynthesizer(array, bit_len)
    Q.BFS_Algorithm(permute=False, control_min=False, direction= 'bi', cost_typ='NCV-111')
    QC= Q.output_circuit()
    #print string
    for i in range(array.shape[0]):
        if not array[i]== QC.inf(i) and not array[i]== -1:
            raise ValueError('wrong result '+ str(array[i])+ ' to '+ str(array.inf(i)))
    #print string[-4]
    print(QC)
    f.write(','+str(QC.cost(0,'length')))
    f.write(','+str(QC.cost(0,'NCV-111')))
    f.write(','+str(QC.cost(0,'NCV-012')))
    f.write(','+str(QC.cost(0,'NCV-155')))
    print(QC.cost(0,'length'), QC.cost(0,'NCV-111'), QC.cost(0,'NCV-012'), QC.cost(0,'NCV-155'))
    f.write('\n')
    
f.close()

    

