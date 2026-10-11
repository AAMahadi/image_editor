CC = gcc
RAYLIB_DIR = libs/raylib/src

# Include Raylib headers from the submodule
CFLAGS = -Wall -Wextra -std=c11 -Isrc -I$(RAYLIB_DIR)

# Link against the local static library (libraylib.a) and required Linux system libraries
LDFLAGS = -L$(RAYLIB_DIR) -lraylib -lm -lpthread -ldl -lrt -lX11

SRC = src/main.c src/processing.c
OBJ = $(SRC:.c=.o)
TARGET = editor

all: raylib $(TARGET)

# Builds Raylib automatically if libraylib.a does not exist yet
raylib:
	@if [ ! -f $(RAYLIB_DIR)/libraylib.a ]; then \
		echo "Building Raylib from submodule..."; \
		$(MAKE) -C $(RAYLIB_DIR) PLATFORM=PLATFORM_DESKTOP; \
	fi

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)