CC     = clang
CFLAGS = -O2 -Wall -pedantic

LIBS     = libavcodec libavformat libavutil
CFLAGS  += $(shell pkg-config --cflags $(LIBS))
LDFLAGS += $(shell pkg-config --libs $(LIBS))

build: probe
probe: probe.c
	@$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	@rm -f probe

.PHONY: clean
