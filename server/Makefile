# Makefile

# Set compiler
CC = gcc

# Set compile optiosn
CFLAGS = -Wall

#Set execution file name
TARGET = serfver

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