#ifndef _CLIENT_H
#define _CLIENT_H

#include "globals.h"
#include "character.h"
#include "tcpsocket.h"
#include "udpsocket.h"

class CClient {

	public:
	  CTcpSocket	tcp_sock;
	  char		tcp_in_buffer[MAX_STRING_LENGTH];
	  char		tcp_out_buffer[MAX_STRING_LENGTH];
	
	  string	ip;
	  string	user_name;
	  
	  CChar		character;
	  
	  void	client_init(int fd);
	  void	client_close(void);
	  
	  int	process_input(void);  
	  
	  int	process_output(void);
	  void	process_udp_input(string buffer);
	
	private:
	  // Flags della client
	  bool	fAuthorized;
	  bool	fPlaing;
	
	  int	n_read;
	  
	  int	client_auth(string msg, vector<string> words );
	  int	client_make(string msg, vector<string> words );
	  int	client_play(string msg, vector<string> words );
	
	  void	broadcast(string msg);
	  void	msg_to_char(string msg, string ip);
	  void	msg_to_zone(string msg, string zone);
	  void	msg_to_room(string msg, string room);
};

#endif
