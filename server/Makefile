# Makefile

# Set compiler
CC = gcc

# Set compile optiosn
CFLAGS = -Wall -I../common

# Set debug compile options
DEBUG_FLAGS = -DDEBUG

#Set execution file name
TARGET = server
DEBUG_TARGET = server_debug

# Source files
SRC = server.c util.c team_login_validator.c create_new_team.c team_list.c user_login.c manage_user_table.c make_team_table.c

# Build
all: $(TARGET)

# 
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Debug build
debug: $(SRC)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(DEBUG_TARGET) $(SRC)

# Clean
clean:
	rm -f $(TARGET)
