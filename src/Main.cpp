
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

#include <glib-2.0/glib.h>

#include "main.h"
#include "client.h"
#include "udpsocket.h"
#include "tcpsocket.h"
#include "world.h"
#include "system.h"
#include "database.h"

CWorld world;
CSystem sys;
CDataBase db;

/*
 *  Questa funzione si occupa di gestire il traffico udp.
 *  
 */
void *handle_upd( void *args) {

	CUdpSocket iudp_sock;
	iudp_sock.sock_init( NET_ADDRS, NET_IUDP_PORT );
	sys.log("udp in socket initializated");
	
	CUdpSocket oudp_sock;
	oudp_sock.sock_init( NET_ADDRS, NET_OUDP_PORT );
	sys.log("udp out socket initializated");
	
	// Iteratore necessario per le nostre operazioni sul vettore
	vector<CClient>::iterator i;
	
	while(1){
	  
	  string ip;
	  string buffer;
	  
	  // Leggi dalla socket se ci sono dati in arrivo
	  if (iudp_sock.sock_read(buffer, ip) == 0 ) {

	    for ( i = ((world).client_list).begin(); i < ((world).client_list).end(); i++ ) {
	      // Se abbiamo una client in lista che ha lo stesso ip prcessa i dati in arrivo 
	      if ((*i).ip == ip ) (*i).process_udp_input(buffer);
	    }
	  }
	}
	return 0;
}

/*
 *  Questa funzine si occupa di gestire il traffico
 *  tcp-ip, nuove connessioni, disconnessioni e msg
 *  importanti. Per ogni nuova connessionie crea una
 *  istanza della classe client e la aggiunge al vettore
 *  client_list della variabile globale world. Usa un
 *  thread a parte
 */
void *handle_network( void *args) {

	int 		maxdesc;
	// Set di descrittore per la select
	fd_set		in_set;
	fd_set		out_set;
	fd_set		exc_set;
	// Istanza della nostra socket principale
	CTcpSocket	tcp_sock;
	// Struttura per il timeout della select
	struct timeval	timeout;
	// Iteratore necessario per le nostre operazioni sul vettore client_list
	vector<CClient>::iterator i;
	
	sys.log("Network thread started");
	
	tcp_sock.sock_init( NET_ADDRS, NET_TCP_PORT );
	tcp_sock.sock_listen();
	
	while(1){
  
	  timeout.tv_sec = 1;
          timeout.tv_usec = 0;
	  
	  /*
	   * Poll all active descriptors.
	   */
	  FD_ZERO( &in_set  );
	  FD_ZERO( &out_set );
	  FD_ZERO( &exc_set );
	  FD_SET( tcp_sock.s, &in_set );
	  
	  maxdesc = tcp_sock.s;
	  for ( i = ((world).client_list).begin(); i < ((world).client_list).end(); i++ ) {
	  
	    maxdesc = UMAX( maxdesc, ((*i).tcp_sock).s );
	    FD_SET( ((*i).tcp_sock).s, &in_set  );
	    FD_SET( ((*i).tcp_sock).s, &out_set );
	    FD_SET( ((*i).tcp_sock).s, &exc_set );
	  }

	  if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &timeout ) < 0 ) {
	    sys.error("Select: poll loop error");
	    exit( 1 );
	  }
	
	  /*
	   * Nuove connessioni?
	   */
	  if ( FD_ISSET( tcp_sock.s, &in_set ) ) {
	  // Crea una client
	  CClient *client = new CClient;
	  // Inizializza la client assegnadoli il socket descriptor
	  client->client_init(tcp_sock.sock_accept());
	  // Aggiungi la client alla lista del mondo
	  world.client_list.push_back(*client);
	  }

	  
	  /*
	   * Elimina le connessioni in errore.
	   */
	  for ( i = ((world).client_list).begin(); i < ((world).client_list).end(); i++ ) {
 
	    if ( FD_ISSET( ((*i).tcp_sock).s, &exc_set ) )
	    {
	      FD_CLR( ((*i).tcp_sock).s, &in_set  );
	      FD_CLR( ((*i).tcp_sock).s, &out_set );
	       
	      (*i).client_close();
	      world.client_list.erase(i);
	    }
	  }

	  /*
	   * Process input.
	   */
	  for ( i = ((world).client_list).begin(); i < ((world).client_list).end(); i++ ) {

	    if ( FD_ISSET( ((*i).tcp_sock).s, &in_set ) )
	    {
	      if ( (*i).process_input() < 1 )
	      {
	        FD_CLR( ((*i).tcp_sock).s, &out_set );
		
	        (*i).client_close();
	        world.client_list.erase(i);
	        continue;
	      }
	    }
	    /*
	     * Process output
	     */	    
	    (*i).process_output();
	  }
	}
	return 0;
}

/*
 *  Funizone principale del server, avvia i thread della
 *  gestione del network ed aggiorna, gestisce il mondo
 */
int main( int argc, char **argv) {

	GError *error = NULL;
	void *th_retval;
	
	// Inizializza le variabili di sistema, apri  i file di log e di errore
	sys.sys_init();
	// Inizializza e apri il database
	db.db_init( DATABASE_HOST, DATABASE_USER_NAME, DATABASE_USER_PASSW, DATABASE_NAME );
	// Inizializza il mondo
	world.world_init();
	
	if (!g_thread_supported()) {
	  g_thread_init(NULL);
	} else {
	  sys.error("Glibc threads are not supported");
	  exit(1);	  
	}
	
	if (!g_thread_create( handle_network, NULL, FALSE, &error)) 
	{
	  sys.error("Failed to create network thread:"+string(error->message));
	  exit(1);
	}
	
	if (!g_thread_create( handle_upd, NULL, FALSE, &error)) 
	{
	  sys.error("Failed to create network thread:"+string(error->message));
	  exit(1);
	}
	
	while(1){
	
	  world.world_update();
	
	}
	g_thread_exit(th_retval);
	g_thread_exit(th_retval);
	
	return 0;
}
