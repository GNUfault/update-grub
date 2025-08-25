SU=sudo
CC=gcc
OPT=s
GDB=0
SRC=main.c
BIN=update-grub
INS=/usr/local/sbin/$(BIN)

all: check-for-root clean compile install

check-for-root:
	@if [ "$(shell id -u)" -ne 0 ]; then \
        	echo "You must run this as root (try $(SU))"; \
        	exit 1; \
	fi

compile:
	$(CC) -O$(OPT) -g$(GDB) $(SRC) -o $(BIN)

clean:
	rm -f $(BIN)

install: check-for-root
	install -m 755 $(BIN) $(INS)

remove:
	$(SU) rm -f $(INS)

.PHONY: all check-for-root compile clean install remove
