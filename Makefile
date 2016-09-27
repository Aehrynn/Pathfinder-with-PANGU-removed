
LDFLAGS=-lm
CXXFLAGS=-O3

all: protocol_test

protocol_test: pan_protocol_lib.o pan_socket_io.o winsock_support.o

clean:
	/bin/rm -f protocol_test *.o
