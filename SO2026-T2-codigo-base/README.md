# Trabalho Prático 2 - Simulador de Algoritmos de Substituição de Páginas

Sistemas Operativos 2026/2027

## Estrutura do Projeto
- `ossim.c`: Ponto de entrada do simulador.
- `virtmem.c` / `virtmem.h` / `virtmem_types.h`: Algoritmos de substituição de páginas (FIFO, RANDOM, LRU, NRU, CLOCK).
- `scheduler.c` / `scheduler.h`: Escalonador de processos.
- `cenarios/`: Ficheiros de input CSV de teste.

## Como Compilar
```bash
make
```

## Como Executar
```bash
./ossim --vm [FIFO|RANDOM|LRU|NRU|CLOCK] [--sched FIFO|RR] [--frames N] cenarios/lru_vs_fifo.csv
```
