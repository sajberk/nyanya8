# OBJS specifies which object files to compile as part of the project
OBJS = main.o SDLHandler.o Chip8.o

# CC specifies which compiler we're using
CC = g++

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2

# OBJ_NAME specifies the name of our executable
OBJ_NAME = nyanya8

# This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

# Rule to compile main.cpp into main.o
main.o : main.cpp
	$(CC) -c main.cpp $(COMPILER_FLAGS)

# Rule to compile SDLHandler.cpp into SDLHandler.o
SDLHandler.o : SDLHandler.cpp
	$(CC) -c SDLHandler.cpp $(COMPILER_FLAGS)

Chip8.o : Chip8.cpp
	$(CC) -c Chip8.cpp $(COMPILER_FLAGS)
# Clean up object files and the executable
clean:
	rm -f *.o $(OBJ_NAME)
