CC 			= gcc
CFLAGS 		= -Wall -Werror
OBJFILES	= tlib.o queue.o map.o main.o
TARGET 		= PokemonGame

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

clean: 
	rm -f $(OBJFILES) $(TARGET) *~ core
