CC 			= gcc
CFLAGS 		= -Wall -Werror
OBJFILES	= queue.o main.o
TARGET 		= PokemonGame

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

clean: 
	rm -f $(OBJFILES) $(TARGET) *~ core
