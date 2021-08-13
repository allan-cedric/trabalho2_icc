#!/usr/bin/bash

# Allan Cedric G. B. Alves da Silva - GRR20190351
# Gabriel N. H. do Nascimento - GRR20190361

# Filtragem do LIKWID
PATTERN="RDTSC Runtime|TABLE|L3|L2|FLOP"

# Core do processador para executar os testes
CORE=7

mkdir -p Resultados
rm -f ./Resultados/*

for i in 10 32 50 64 100 128 200 256 300 400 512 1000
do
    echo " --- n: $i ---"
    echo -n "Generating entry... "
    ./gera_entrada $i > entrada.in
    echo "Done!"

    echo "$i" > temp

    # Avaliação de desempenho programa 'main_unoptimized'
    echo -n "(main_unoptimized) FLOPS_DP evaluation... "
    likwid-perfctr -C $CORE -g FLOPS_DP -O -m ./main_unoptimized < entrada.in | grep -E "$PATTERN" >> temp
    echo "Done!"

    echo -n "(main_unoptimized) L2CACHE evaluation... "
    likwid-perfctr -C $CORE -g L2CACHE -O -m ./main_unoptimized < entrada.in | grep -E "$PATTERN" >> temp
    echo "Done!"

    echo -n "(main_unoptimized) L3 evaluation... "
    likwid-perfctr -C $CORE -g L3 -O -m ./main_unoptimized < entrada.in | grep -E "$PATTERN" >> temp
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

# Gera os gráficos no diretório Resultados/
python3 gera_plot.py

rm -f ./temp ./entrada.in