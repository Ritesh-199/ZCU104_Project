# Cross-compiler
CC = aarch64-linux-gnu-gcc
# Compiler flags
CFLAGS = -Wall -Wextra
# Linker flags
LDFLAGS =

# Source files
SRCS = server_main.c functions.c
# Object files
OBJS = $(SRCS:.c=.o)
# Target executable
TARGET = server

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(TARGET)


