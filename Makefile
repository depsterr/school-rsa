.POSIX:
.PHONY: clean test
OBJECTS=rsacrypt.o util.o prim.o vec.o main.o
CFLAGS=-Ofast -pipe -march=native -I/usr/local/lib/ghc/include -I/usr/lib64/ghc-8.8.4/include
rsa: $(OBJECTS)
	ghc -no-hs-main -o $@ $(OBJECTS)
rsacrypt.o: rsacrypt.hs
	ghc -c -XForeignFunctionInterface -O rsacrypt.hs
main.o: rsacrypt.o
test: rsa
	./rsa
clean:
	rm -f $(OBJECTS) rsacrypt.hi rsacrypt_stub.h
.SUFFIXES: .o .c
.c.o:
	$(CC) -c $(EXTRA) $(CFLAGS) -o $@ $<
