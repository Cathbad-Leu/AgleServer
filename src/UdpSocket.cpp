
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <limits.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "udpsocket.h"
#include "system.h"

extern CSystem sys;

void CUdpSocket::sock_init( char *netaddr, int port ) {

	// crea la socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		sys.error("udp socket error");
		sock_close();
		exit(1);
	}
	
	struct sockaddr_in sa;
	// riempi la struttura di indirizzo
	memset(&sa,0,sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = inet_addr(netaddr);
	
	int reuse_addr = 1;
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));

	// chiama bind
	if(bind(s, (struct  sockaddr *) &sa, sizeof(sockaddr)) < 0 ){
		sys.error("udp bind() error");
		sock_close();
		exit(1);
	}
}


int CUdpSocket::sock_read(string &buf, string &ip) {

	int rlen;
	char c_buf[MAX_STRING_LENGTH];
	
	struct sockaddr_in ca;
	socklen_t calen = sizeof(ca);
	
	rlen = recvfrom(s, c_buf, sizeof(c_buf)-1, 0, (struct sockaddr*)&ca, &calen);
	if (rlen == -1) {
		sys.error("recvfrom error");
		return -1;
	}
	c_buf[rlen] = '\0';
	ip = inet_ntoa(ca.sin_addr);
	buf = c_buf;
	return 0;
}


int CUdpSocket::sock_write(string buf, string ip, int port) {

	char *c_ip  = (char*)((ip).c_str());
	char *c_buf = (char*)((buf).c_str());
	
	struct sockaddr_in ca;
	// riempi la struttura di indirizzo
	memset(&ca,0,sizeof(ca));
	ca.sin_family = AF_INET;
	ca.sin_port = htons(port);
	inet_aton(c_ip, &ca.sin_addr );
	
	int wlen = sendto(s, c_buf, strlen(c_buf), 0,(struct sockaddr*)&ca, sizeof(ca) );
	if (wlen == -1) {
		sys.error("sendto error");
		exit(1);
	}
	return 0;
}

CUdpSocket::~CUdpSocket() { close(s); }

void CUdpSocket::sock_close() { close(s); }

