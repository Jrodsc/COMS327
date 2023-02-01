CC 			= gcc
CFLAGS 		= -Wall -Werror -ggdb
OBJFILES	= queue.o main.o
TARGET 		= PokemonGame

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

#main.o: main.c queue.h
#	$(CC) $(CFLAGS) -ggdb main.c -c

#queue.o: queue.c queue.h
#	$(CC) $(CFLAGS) -ggdb queue.c -c

clean: 
	rm -f $(OBJFILES) $(TARGET) *~ core
