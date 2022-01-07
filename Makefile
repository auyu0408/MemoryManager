CC := gcc
override CFLAGS += -O3 -Wall

SOURCE := MemManager.c
BINARY := MemManager

all: $(BINARY)

debug: CFLAGS += -DDEBUG -g
debug: $(BINARY)

$(BINARY): $(SOURCE) $(patsubst %.c, %.h, $(SOURCE))
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(BINARY)
