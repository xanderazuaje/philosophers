# Compiler and flags
CC := cc
CFLAGS := -Wall -Wextra -Werror #-g3
LDFLAGS := #-fsanitize=thread

# Directories
SRC_DIR := src
OBJ_DIR := obj

# Source files
SRC_FILES := $(SRC_DIR)/main.c \
		$(SRC_DIR)/forks_stuff.c \
		$(SRC_DIR)/philosopher.c \
		$(SRC_DIR)/state_checker.c \
		$(SRC_DIR)/states.c \
		$(SRC_DIR)/utils/fake_sleep.c \
		$(SRC_DIR)/utils/ft_atoi.c \
		$(SRC_DIR)/utils/get_time.c \
		$(SRC_DIR)/assert_input/parse_input.c \

# Object files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Executable
TARGET := philo


all: $(TARGET)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

bonus:: CFLAGS += -D BONUS=1
bonus: re

rebonus: fclean bonus

valgrind: all
	valgrind -

# Build rule
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
# Object file rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

# Phony targets
.PHONY: all clean fclean re
