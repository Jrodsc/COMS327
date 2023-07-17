CC 			= g++
CFLAGS 		= -Wall -Werror -ggdb -O2 -std=c++17
LDFLAGS 	= -lncurses
OBJFILES 	= hanoi.o
TARGET		= Hanoi 

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS) 2> error.log 

clean: 
	rm -f $(OBJFILES) $(TARGET1) $(TARGET2) *~ core
