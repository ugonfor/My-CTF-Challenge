all: PoorMineSweeper

PoorMineSweeper: pb.o crc32.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f PoorMineSweeper *.o