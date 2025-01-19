
#ifndef CTCPSOCKET_H_
#define CTCPSOCKET_H_

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

class CTcpSocket {

	public:
		CTcpSocket();
		~CTcpSocket();
		void sock_init( char *netaddr, int port );
		void sock_connect(char *netaddr, int port);
		void sock_listen(void);
		int sock_accept(void);
		void sock_close(void);
		int sock_read( char *buf, size_t count );
		int sock_write(char *buf, size_t count );
		int sock_gets( char *buf, size_t count );
		int sock_puts( char *buf );
		char* sock_ip(void);
		void sock_noblock(void);
		void ignore_pipe(void);
	
		int s;
};

#endif
