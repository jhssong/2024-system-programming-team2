# Makefile

# Set compiler
CC = gcc

# Set compile optiosn
CFLAGS = -Wall
LDFLAGS = -lncurses

# Set debug compile options
DEBUG_FLAGS = -DDEBUG

#Set execution file name
TARGET = app
DEBUG_TARGET = app_debug

# Source files
SRC = main.c common.c util.c menu_selector.c greeting_menu.c create_new_team.c client.c select_existing_team.c login.c manage_tables.c team_table.c

# Build
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Debug build
debug: $(SRC)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(DEBUG_TARGET) $(SRC) $(LDFLAGS)

# Clean
clean:
	rm -f $(TARGET)
