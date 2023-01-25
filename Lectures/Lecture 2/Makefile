# target: list of dependencies
# [tab] command to build target fromo dependencies
# [tab] another command
# [tab] another command

# To build all the targets not only the first

all: times_tables

# Targets

times_tables: times_tables.c
	gcc -Wall -Werror -ggdb times_tables.c -o times_tables

# To remove all the (-f = force)

clean:
	rm -f times_tables *~ core