
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
#include <sys/fcntl.h>
#include <fcntl.h>

#include "tcpsocket.h"
#include "system.h"

extern CSystem sys;

/*
 *  Crea la socket, e associala all'indirizzo "netadress" e
 *  alla porta "port". 
 */
void CTcpSocket::sock_init( char *netaddr, int port ) {

	// ignora il segnale sig_ign
	ignore_pipe();

	// crea la socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		sys.error("socket error");
		sock_close();
		exit(1);
	}
	
	// imposta i flag della socket
	int reuse_addr = 1;
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
	
	// rendi la socket non bloccante
	sock_noblock();

	// struttura che conterra' la nostra socket principale
	struct sockaddr_in sa;
	// riempi la struttura di indirizzo
	memset(&sa,0,sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = inet_addr(netaddr);

	// chiama bind
	if(bind(s, (struct  sockaddr *) &sa, sizeof(sockaddr)) < 0 ){
		sys.error("tcp bind() error");
		sock_close();
		exit(1);
	}
}

void CTcpSocket::sock_connect( char *netaddr, int port ) {

	// ignora il segnale sig_ign
	ignore_pipe();

	// crea la socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		sys.error("socket error");
		sock_close();
		exit(1);
	}

	// struttura che conterra' la nostra socket principale
	struct sockaddr_in sa;
	// riempi la struttura di indirizzo
	memset(&sa,0,sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = inet_addr(netaddr);

	// connetti al server
	if(connect(s, (struct  sockaddr *) &sa, sizeof(sa)) < 0 ){
		sys.error("connect error to adress: " + string(netaddr));
		sock_close();
		exit(1);
	}
}

/* 
 *  Tiene la socket in ascolto con una coda di max 5
 */
void CTcpSocket::sock_listen() {

  if (listen(s, 5) < 0)
  {
    sys.error("could not listen");
    sock_close();
    exit(1);
  }
}

/*  
 *  Accetta le socket pendenti, ricava l'indirizzo ip e crea
 *  un thread a cui passi la funzione per gestire la client e
 *  una struttura contenente l'ip ed il socket descriptor.
 */
int CTcpSocket::sock_accept() {

	int nsd;
	struct sockaddr_in ca;
	socklen_t calen = sizeof(ca);
	memset(&ca, 0, calen);

	// Accetta le connessioni pendenti
	nsd = accept(s, (struct sockaddr*)&ca, &calen);
	// Immagazzina l'ip della client corrente nella strutture
	
	if ( nsd < 0 ) {
	  if (errno != EINTR) {
	    sys.error("accept error");
	    sock_close();
	    exit(1);
	  }
	}
	return nsd;
}

/*
 *  Questa e' come la chiamata di sistema write(), solo che
 *  questa si accerta che tutti i dati siano trasmessi. 
 */
int CTcpSocket::sock_write(char *buf, size_t count)
{
	size_t bytes_sent = 0;
	int this_write;

	while (bytes_sent < count) {
	  do
	    this_write = write(s, buf, count - bytes_sent);
	  while ( (this_write < 0) && (errno == EINTR) );
	  if (this_write <= 0)
            return this_write;
	  bytes_sent += this_write;
	  buf += this_write;
	}
	return count;
}

/*
 *  Questa funzione legge da una socket finche' riceve un carattere
 *  di fine linea. Riempie il buffer "str" fino al massimo dato da "count".  
 *  Questa funzione ritornera' -1 se il socket e' stato  chiuso durante la 
 *  lettura. Ricorda che tutti i dati in eccesso rispetto la lunghezza
 *  count verranno persi. 
 */
int CTcpSocket::sock_gets(char *buf, size_t count)
{
	int bytes_read;
	int total_count = 0;
	char *current_position;
	char last_read = 0;

	current_position = buf;
	while (last_read != 10) {
	  bytes_read = read(s, &last_read, 1);
	  if (bytes_read <= 0) {
	    /* L'altro lato potrebbe essersi chiuso inaspettatamente */
	    return -1;
	  }
	  if ( ((unsigned int)total_count < count) && (last_read != 10) && (last_read !=13) ) {
	    current_position[0] = last_read;
	    current_position++;
	    total_count++;
	  }
	}
	if (count > 0)
	  current_position[0] = 0;
	return total_count;
}

/* Questa funzione scrive in uscita sulla socket una stringa di caratteri.
   Ritorna -1 se la connessione e' stata chiusa mentre si prova a scrivere. */
int CTcpSocket::sock_puts(char *buf)
{
	return sock_write(buf, strlen(buf));
}

/* Imposta la socket a non bloccante */
void CTcpSocket::sock_noblock(void) {

	int opts;

	opts = fcntl(s,F_GETFL);
	if (opts < 0) {
	  sys.error("fcntl(F_GETFL)");
	  sock_close();
	  exit(1);
	}
	
	opts = (opts | O_NONBLOCK);
	if (fcntl(s,F_SETFL,opts) < 0) {
	  sys.error("fcntl(F_SETFL)");
	  sock_close();
	  exit(1);
	}
}

char* CTcpSocket::sock_ip(void){

	struct sockaddr_in sock;
	struct hostent *from;
	char buf[MAX_STRING_LENGTH];
	
	socklen_t size = sizeof(sock);
	if ( getpeername( s, (struct sockaddr *) &sock, &size ) < 0 )
	{
	  sys.error("New_descriptor: getpeername");
	  return (char*)"(unknown)";
	}
	else
	{
	  int addr;
	  addr = ntohl( sock.sin_addr.s_addr );
	  sprintf( buf, "%d.%d.%d.%d",
	    ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
	    ( addr >>  8 ) & 0xFF, ( addr       ) & 0xFF
	    );
	  from = gethostbyaddr( (char *) &sock.sin_addr,sizeof(sock.sin_addr), AF_INET );
	  return  (from ? from->h_name : buf) ;
    }

}

/* Questa permette di ignorare il segnale SIGPIPE.  Che e' generalmente
   una buona idea in quanto normalmente termina l'applicazione.
   SIGPIPE e' spedito quando si prova a scrivere ad una socket sconnessa.
   Per esser sicuri di cogliere l'errore e' meglio controllare i codici
   di ritorno di write!. */
void CTcpSocket::ignore_pipe(void)
{
	struct sigaction sig;

	sig.sa_handler = SIG_IGN;
	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	sigaction(SIGPIPE,&sig,NULL);
}

/* Chiudi la socket e termina il thread*/
void CTcpSocket::sock_close() { 
	close(s);
}

CTcpSocket::CTcpSocket() { }

CTcpSocket::~CTcpSocket() { }



