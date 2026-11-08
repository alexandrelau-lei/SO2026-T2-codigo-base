CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = ossim

SRCS = ossim.c virtmem.c scheduler.c burst_queue.c queue.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

test: all
	./$(TARGET) --vm FIFO --frames 3 cenarios/basico.csv
	./$(TARGET) --vm LRU --frames 3 cenarios/lru_vs_fifo.csv
	./$(TARGET) --vm NRU --frames 3 cenarios/nru_dirty.csv
	./$(TARGET) --vm CLOCK --frames 3 cenarios/lru_vs_fifo.csv

.PHONY: all clean test
