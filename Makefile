CC=gcc
CFLAGS=-I.
DEPS=decimal.h
OBJS=decimal.o

%.o: %.cpp $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

add::$(OBJS) testadd.o
	$(CC) -o testadd.exe testadd.o decimal.o -lstdc++

convert:$(OBJS) testconvert.o
	$(CC) -o testconvert.exe testconvert.o decimal.o -lstdc++

clean:
	rm *.o *.exe 