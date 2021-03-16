.POSIX:
.PHONY: clean test
OBJECTS=math.o util.o prim.o vec.o main.o
CFLAGS=-Ofast -pipe -march=native -I/usr/local/lib/ghc/include -I/usr/lib64/ghc-8.8.4/include -Wall -Wextra
rsa: $(OBJECTS)
	ghc -no-hs-main -o $@ $(OBJECTS)
math.o: math.hs
	ghc -c -XForeignFunctionInterface -O math.hs
main.o: math.o
test: rsa
	./rsa
clean:
	rm -f $(OBJECTS) math.hi math_stub.h
.SUFFIXES: .o .c
.c.o:
	$(CC) -c $(EXTRA) $(CFLAGS) -o $@ $<
