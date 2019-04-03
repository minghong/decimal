CC=gcc
CFLAGS=-I.
DEPS=decimal.h
OBJS=decimal.o

%.o: %.cpp $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

div:$(OBJS) testdiv.o
	$(CC) -o div.exe testdiv.o decimal.o -lstdc++

add:$(OBJS) testadd.o
	$(CC) -o add.exe testadd.o decimal.o -lstdc++

convert:$(OBJS) testconvert.o
	$(CC) -o convert.exe testconvert.o decimal.o -lstdc++

clean:
	rm *.o *.exe 