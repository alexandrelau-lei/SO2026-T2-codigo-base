#!/bin/bash

make clean && make

echo "=========================================="
echo " Executando Todos os Algoritmos VM        "
echo "=========================================="

ALGOS=("FIFO" "RANDOM" "LRU" "NRU" "CLOCK")

for algo in "${ALGOS[@]}"; do
    echo ""
    echo ">>> Algoritmo: $algo"
    ./ossim --vm "$algo" --frames 3 cenarios/lru_vs_fifo.csv
done
