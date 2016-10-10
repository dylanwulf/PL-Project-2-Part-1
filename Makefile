CC = g++
OBJECTS = driver.o scan.o parser_generator.o
INCLUDES = scan.h parser_generator.h

all : driver

driver : $(OBJECTS)
	$(CC) -g -o driver $(OBJECTS)

driver.o : driver.cc scan.h
	$(CC) -c -g driver.cc

scan.o : scan.cc scan.h
	$(CC) -c -g scan.cc

parser_generator.o: parser_generator.cpp parser_generator.h
	$(CC) -c -g parser_generator.cpp

clean :
	rm -f driver $(OBJECTS)
