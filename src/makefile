CFLAGS = -std=c11 -pedantic-errors -rdynamic -g -D_GNU_SOURCE -Wall -Wextra

all: cref.o tests.c json.o
	@gcc cref/cref.o json.o tests.c -o tests -g -pedantic > /dev/null

tests: all
	@./tests

json.o: hashmap.c
	@gcc -c hashmap.c ${CFLAGS} -o json.o

cref.o:
	@cd cref && make linkable

clean:
	@rm tests *.o
	@cd cref && make clean
