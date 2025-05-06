# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -Werror -g
LFLAGS := -lncurses

# Directories
SRC_DIR := sources
OBJ_DIR := build
BIN := roguepupu

# Find all .c files in sources directory
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Create corresponding .o filenames in build/
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(BIN)

# Link object files into binary
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(BIN)

re:	fclean all

.PHONY: all clean fclean re

