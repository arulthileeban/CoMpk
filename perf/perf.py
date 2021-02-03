#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Oct 21 14:24:42 2020
@author: jubayer, Virginia Tech
@description:
"""




import os, sys
import numpy as np
import math
import matplotlib.pyplot as plt
#from scipy.interpolate import interp1d
import seaborn as sns
import cv2
import random

import matplotlib
font = {'family' : 'sans-serif',
        'weight' : 'bold',
        'size'   : 14}

matplotlib.rc('font', **font)



f = open('data.txt', 'r')

def cal(f):
    lines = f.readlines()
    m= []
    for i in range(len(lines)):
        line=lines[i].strip().split()
        m.append(sum([int(line[j]) for j in range(len(line))])/len(line))
    return m
f = open('data.txt', 'r')
m1 = cal(f)
f = open('data_libmpk_0.5.txt', 'r')
m2 = cal(f)
f = open('data_libmpk_1.txt', 'r')
m3 = cal(f)




fig,ax = plt.subplots()
ax.plot(np.array(m1)/10**6, label = 'Standard MPK', linewidth = 3)
ax.plot(np.array(m2)/10**6, label = 'CoMPK', linewidth = 3)
#ax.plot(np.array(m2)/10**6)

ax.set_xlabel('Measurements')
ax.set_ylabel('Mean cycle (millions)')
plt.legend()
plt.tight_layout()
plt.savefig('mpk.pdf', transparent=True)