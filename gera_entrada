#! /usr/bin/env python3
# coding=latin-1

import sys
import numpy as np

from string import *
from math import *
from numpy.random import *
from numpy.polynomial.polynomial import *
from datetime import *

def sample (low, high, k=1) :
    """ Return a k-length list of unique random floats
        in the range of low <= x <= high
    """
    result = []
    for i in range(k):
        x = uniform(low, high)
        while x in result:
            x = uniform(low, high)
        result.append(x)
    return result

    
# =====================================================
# Inicio Programa principal
# =====================================================
#

# seed (202102)
seed (int(datetime.timestamp(datetime.now())))

N = 64
M = randint(2,6)

if len(sys.argv) > 1 :
    N = int(sys.argv[1])
    if len(sys.argv) > 2 :
        M = int(sys.argv[2])

# Gera vetor x, ordenado e sem valores duplicados
x = sample(-1.0,1.0,N-1)
x.sort()

# Evita que pontos gerem underflow na SL de interpolação e ajuste de curvas [GD]
x = np.append(x,1)

# Gera pontos a partir de polinômio aleatório
fm = []
for j in range (0,M) :
    f = []
    # Gera polinomio aleatório de grau N-1
    pol = uniform(-20.0,20.0,N)
    for k in range(0,N) :
        f.append(polyval(x[k], pol))
    fm.append(f)

# Gera saída
print(N,M)
for i in x :
    print(i, end=" ")

print("")
for i in fm :
    for j in i :
        print(j, end=" ")
    print("")

print("")

#FIM
