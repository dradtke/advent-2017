CFLAGS = $(shell pkg-config --cflags glib-2.0 gio-2.0)
LDFLAGS = $(shell pkg-config --libs glib-2.0 gio-2.0)

part1:
	clang $(CFLAGS) -o part1 part1.c $(LDFLAGS)

part2:
	clang $(CFLAGS) -o part2 part2.c $(LDFLAGS)

clean:
	rm -f part1 part2

.PHONY: part1 part2 clean
