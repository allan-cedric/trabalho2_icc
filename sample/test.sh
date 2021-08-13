#!/bin/bash

echo "uso: $0 sample_X"

rm -f *.csv *.gpl;

gcc verifica.c -o verifica;

IN=$1.in
OUT=$1.out
./verifica $IN $OUT > graficos.gpl;

gnuplot graficos.gpl;

rm -f *.csv *.gpl verifica;
