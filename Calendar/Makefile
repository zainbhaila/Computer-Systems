CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings \
	-Wshadow -pedantic-errors \
	-fstack-protector-all -Wextra
PROGS: public01 public02 public03 public04 public05 \
	student_tests public01_2 public02_2

.PHONY: all clean

all: $(PROGS)

clean:
	@rm -f *.o *~ $(PROGS)

student_tests: student_tests.o calendar.o my_memory_checker_216.o
	$(CC) $(CFLAGS) -o student_tests student_tests.o \
	calendar.o my_memory_checker_216.o

public01: public01.c calendar.o my_memory_checker_216.o
	$(CC) $(CFLAGS) -o public01 public01.c calendar.o \
	my_memory_checker_216.o

public02: public02.c calendar.o my_memory_checker_216.o
	$(CC) $(CFLAGS) -o public02 public02.c calendar.o \
	my_memory_checker_216.o

public03: public03.c calendar.o my_memory_checker_216.o
	$(CC) $(CFLAGS) -o public03 public03.c calendar.o \
	my_memory_checker_216.o

public04: public04.c calendar.o my_memory_checker_216.o
	$(CC) $(CFLAGS) -o public04 public04.c calendar.o \
	my_memory_checker_216.o

public05: public05.c calendar.o my_memory_checker_216.o
	$(CC) $(CFLAGS) -o public05 public05.c calendar.o \
	my_memory_checker_216.o

public01_2: public01_2.c calendar.o my_memory_checker_216.o
	$(CC) $(CFLAGS) -o public01_2 public01_2.c calendar.o \
	my_memory_checker_216.o

public02_2: public02_2.c calendar.o my_memory_checker_216.o
	$(CC) $(CFLAGS) -o public02_2 public02_2.c calendar.o \
	my_memory_checker_216.o

my_memory_checker_216.o: my_memory_checker_216.c my_memory_checker_216.h
calendar.o: calendar.c calendar.h event.h
student_tests.o: student_tests.c

