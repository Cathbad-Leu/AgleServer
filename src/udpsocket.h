
#ifndef CUDPSOCKET_H_
#define CUDPSOCKET_H_

#include "globals.h"


/*	Qui seguono i modelli delle strutture usate per contenere i dati
	relativi alle socket.

struct sockaddr{
	unsigned short	sa_family;			// address family
	char			sa_data[14];		// 14 bytes of protocol address
};

struct in_addr {
	unsigned long	s_addr;
};

struct sockaddr_in {
	short int  			sin_family;		// Address family
	unsigned short int	sin_port;		// Port
	struct in_addr		sin_addr;		// Internet Address
	unsigned char		sin_zero[8];	// Same size as struct sockaddr
};

struct hostent
{
	char	*h_name;				// Official name of host.
	char	**h_aliases;			// Alias list.
	int		h_addrtype;				// Host address type.
	int		h_length;				// Length of address.
	char	**h_addr_list;			// List of addresses from name server.
#define h_addr  h_addr_list[0]		// Address, for backward compatibility.
};*/

class CUdpSocket {

	public:
		~CUdpSocket();
		void sock_init( char *netaddr, int port );
		void sock_close();
		int sock_read(string &buf, string &ip);
		int sock_write(string buf, string ip, int port);
	
		int s;
};

#endif
