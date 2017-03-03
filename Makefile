all: tcpserver tcpclient

udpserver: tcpserver.c
	gcc -o tcpserver tcpserver.c -Wall

udpclient: udpclient.c
	gcc -o tcpclient tcpclient.c -Wall
