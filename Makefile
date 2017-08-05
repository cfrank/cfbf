CC = gcc
FLAGS = -g --std=c99 -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual \
		-Wstrict-prototypes -Wmissing-prototypes -Wcast-align -Winline \
		-Wfloat-equal -Wconversion -Wundef
LIBS = -lm
TARGET = cfbf
SRC = src/
OUTPUT = bin/

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard $(SRC)*.c))
HEADERS = $(wildcard $(SRC)*.h)

%.o: %.c $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(FLAGS) $(LIBS) -o $(OUTPUT)$@

clean:
	@echo "Cleaning"
	-rm -f $(SRC)*.o
	-rm -f $(OUTPUT)$(TARGET)