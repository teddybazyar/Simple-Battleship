CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = battleship
OBJS = main.o battleship.o

# Default target
all: $(TARGET) battleship.log

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile main.c
main.o: main.c battleship.h
	$(CC) $(CFLAGS) -c main.c

# Compile battleship.c
battleship.o: battleship.c battleship.h
	$(CC) $(CFLAGS) -c battleship.c

# Touch the log file if it doesn't exist
battleship.log:
	touch battleship.log

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS) battleship.log
