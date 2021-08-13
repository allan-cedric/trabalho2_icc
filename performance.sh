#!/usr/bin/bash

# Filtragem do LIKWID
PATTERN="RDTSC Runtime|TABLE|L3|L2|FLOP"

# Core do processador para executar os testes
CORE=7

mkdir -p Resultados
rm -f ./Resultados/*

for i in 5 6 7 10 32 50 64 100 128 200 256 300 400 512 1000
do
    echo " --- n: $i ---"
    echo -n "Generating entry... "
    ./gera_entrada $i > entrada.in
    echo "Done!"

    echo "$i" > temp

    # Avaliação de desempenho programa 'main'
    echo -n "(main) FLOPS_DP evaluation... "
    likwid-perfctr -C $CORE -g FLOPS_DP -O -m ./main < entrada.in | grep -E "$PATTERN" >> temp
    echo "Done!"

    echo -n "(main) L2CACHE evaluation... "
    likwid-perfctr -C $CORE -g L2CACHE -O -m ./main < entrada.in | grep -E "$PATTERN" >> temp
    echo "Done!"

    echo -n "(main) L3 evaluation... "
    likwid-perfctr -C $CORE -g L3 -O -m ./main < entrada.in | grep -E "$PATTERN" >> temp
    echo "Done!"

    # Avaliação de desempenho programa 'main_optimized'
    echo -n "(main_optimized) FLOPS_DP evaluation... "
    likwid-perfctr -C $CORE -g FLOPS_DP -O -m ./main_optimized < entrada.in | grep -E "$PATTERN" >> temp
    echo "Done!"

    echo -n "(main_optimized) L2CACHE evaluation... "
    likwid-perfctr -C $CORE -g L2CACHE -O -m ./main_optimized < entrada.in | grep -E "$PATTERN" >> temp
    echo "Done!"

    echo -n "(main_optimized) L3 evaluation... "
    likwid-perfctr -C $CORE -g L3 -O -m ./main_optimized < entrada.in | grep -E "$PATTERN" >> temp
    echo "Done!"

    python3 analise.py < temp
done

python3 gera_plot.py

rm -f ./temp ./entrada.in