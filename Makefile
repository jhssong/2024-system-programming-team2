# Makefile

# Set compiler
CC = gcc

# Set compile optiosn
CFLAGS = -Wall
LDFLAGS = -lncurses

#Set execution file name
TARGET = app

# Source files
SRC = main.c

# Build
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Clean
clean:
	rm -f $(TARGET)
