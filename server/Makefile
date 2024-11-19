# Makefile

# Set compiler
CC = gcc

# Set compile optiosn
CFLAGS = -Wall -I../common

#Set execution file name
TARGET = server

# Source files
SRC = server.c

# Build
all: $(TARGET)

# 
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean
clean:
	rm -f $(TARGET)