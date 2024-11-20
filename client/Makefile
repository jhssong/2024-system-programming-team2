# Makefile

# Set compiler
CC = gcc

# Set compile optiosn
CFLAGS = -Wall -I../common
LDFLAGS = -lncurses

#Set execution file name
TARGET = app

# Source files
SRC = main.c util.c menu_selector.c greeting_menu.c create_new_team.c client.c select_existing_team.c

# Build
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Clean
clean:
	rm -f $(TARGET)
