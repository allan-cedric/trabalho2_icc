#!/usr/bin/bash

# Filtragem do LIKWID
PATTERN="RDTSC Runtime|TABLE|L3|L2|FLOP"

# Core do processador para executar os testes
CORE=7

mkdir -p Resultados
rm -f ./Resultados/*

for i in 5 6 7 10 # 32 50 64 100 128 200 256 300 400 512 1000
do
    echo "$i" > temp
    ./gera_entrada $i > entrada.in

    likwid-perfctr -C $CORE -g FLOPS_DP -O -m ./main < entrada.in | grep -E "$PATTERN" >> temp
    likwid-perfctr -C $CORE -g L2CACHE -O -m ./main < entrada.in | grep -E "$PATTERN" >> temp
    likwid-perfctr -C $CORE -g L3 -O -m ./main < entrada.in | grep -E "$PATTERN" >> temp
  
    likwid-perfctr -C $CORE -g FLOPS_DP -O -m ./main_optimized < entrada.in | grep -E "$PATTERN" >> temp
    likwid-perfctr -C $CORE -g L2CACHE -O -m ./main_optimized < entrada.in | grep -E "$PATTERN" >> temp
    likwid-perfctr -C $CORE -g L3 -O -m ./main_optimized < entrada.in | grep -E "$PATTERN" >> temp

    python3 analise.py < temp
done

rm -f temp entrada.in

python3 gera_plot.py

rm -f ./Resultados/*.tsv
