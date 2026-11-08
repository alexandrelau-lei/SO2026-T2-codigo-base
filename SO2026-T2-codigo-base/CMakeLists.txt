cmake_minimum_required(VERSION 3.10)
project(SO2026_T2_Simulador)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(ossim 
    ossim.c 
    virtmem.c 
    scheduler.c 
    burst_queue.c 
    queue.c
)
