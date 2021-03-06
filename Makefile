CC = gcc
FLAGS = --std=c99 -O3 -pedantic -Wall -Wextra -Werror -Wshadow \
	-Wstrict-prototypes -Wmissing-prototypes -Wcast-align -Winline \
	-Wfloat-equal -Wconversion -Wundef -Wno-newline-eof
LIBS =
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

$(TARGET): $(OBJECTS) | $(OUTPUT)
	$(CC) $(OBJECTS) $(FLAGS) $(LIBS) -o $(OUTPUT)$@

$(OUTPUT):
	@echo "Creating $(OUTPUT)"
	@mkdir -p $@

clean:
	@echo "Cleaning"
	-rm -f $(SRC)*.o
	-rm -f $(OUTPUT)$(TARGET)
