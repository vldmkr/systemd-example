CC       = gcc

OPTFLAGS = -O3 -g

CFLAGS   += $(OPTFLAGS) \
            -std=gnu99 \
            -W \
            -Wall \
            -Wextra \
            -Wimplicit-function-declaration \
            -Wredundant-decls \
            -Wstrict-prototypes \
            -Wundef \
            -Wshadow \
            -Wpointer-arith \
            -Wformat \
            -Wreturn-type \
            -Wsign-compare \
            -Wmultichar \
            -Wformat-nonliteral \
            -Winit-self \
            -Wuninitialized \
            -Wformat-security \
            -Werror

LDLIBS    = -lsystemd

all: dir binary

dir:
	mkdir -p build/

binary: server client

server:
	$(CC) $(CFLAGS) -o build/server server.c $(LDLIBS)

client:
	$(CC) $(CFLAGS) -o build/client client.c

install:
	sudo cp matrix.socket matrix.service /etc/systemd/system
	cp build/server /usr/bin/matrix

remove:
	sudo rm -f /etc/systemd/system/matrix.*
	rm -f /usr/bin/matrix

start:
	sudo systemctl enable matrix.socket
	sudo systemctl start matrix.socket

restart:
	sudo systemctl restart matrix.socket

stop:
	sudo systemctl stop matrix.socket
	sudo systemctl disable matrix.socket
	sudo rm -f /tmp/matrix.sk

watch:
	journalctl -xe -f

own-matrix:
	./build/server

rabbit-run:
	./build/client

clean:
	rm -rf build
