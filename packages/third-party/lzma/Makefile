all: lzma

%.o: %.c
	$(CC) -c -o $@ $^

lzma: src/LzmaEnc.o src/LzmaDec.o src/LzFind.o src/7zFile.o
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -rf src/*.o lzma
