CC=gcc
CFLAGS=-Wall -Wextra --pedantic -std=gnu99

all: wythoff_advisor

debug: CFLAGS += -g
debug: wythoff_advisor

wythoff_advisor : wythoff.o

.PHONY: clean

clean:
	rm -rf *.o wythoff wythoff_advisor

