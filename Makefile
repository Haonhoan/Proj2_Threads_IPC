all: p2p4.c
	gcc -g -o p2p4 p2p4.c -lpthread
	./p2p4

clean:
	$(RM) p2p4
