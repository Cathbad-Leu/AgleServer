
#include "client.h"
#include "helper.h"
#include "world.h"
#include "system.h"
#include "database.h"

extern CWorld world;
extern CSystem sys;
extern CDataBase db;

void CClient::client_init(int fd) {

	fAuthorized = false;
	fPlaing = false;
	
	tcp_sock.s = fd;
	tcp_sock.sock_noblock();
	// Associa alla client l'ip dell'utente
	ip = tcp_sock.sock_ip();
	
	sys.log("Client inizitializated with ip: "+string( ip ));
}

int CClient::process_output() {

	int n = tcp_sock.sock_puts(tcp_out_buffer);
	memset(tcp_out_buffer, 0, sizeof(tcp_out_buffer));
	return n;
}

int CClient::process_input() {

	string msg;
	string sep = "::";
	vector<string> words;
	
	// Libera il buffer di input
	memset(tcp_in_buffer, 0, sizeof(tcp_in_buffer));
	
	// Leggi i dati nella socket e mettili nell'input buffer    
	n_read = tcp_sock.sock_gets(tcp_in_buffer, 1024);
	
	if (n_read < 0){
	   // La socket si e' chiusa 
	   return 0;  
	}
	
	// Assicurati che la stringa termini con null
	tcp_in_buffer[n_read] = '\0';
	// Converti la stringa c in una di c++
	msg = tcp_in_buffer;
	
	// Se la client non e' stata autorizzata
	if (fAuthorized == false) {	
	   // Se la stringa inviata dalla client e' tel tipo: Auth::Nome::Password
	   if (msg.find("Auth",0) != string::npos) {
	      // si separa la stringa in parole e resituisci un vettore contenente le varie parti
	      str_split(msg,sep,words);
	      sys.log( "Auth string split: separator =" + sep);
	      if ( client_auth(msg, words) == 0 ) return 1;
	      else {
	        sys.log( "Auth not found 1");
	        return 0;
	      }
	      sys.log( "Auth not found 2");      
	      return 0;
	   } 
	   sys.log( "Auth not valid string");
	   return 0;	   
	}
	
	// Se la client e' gia' stata autorizzata
	else if (fAuthorized == true) {	
	  // Se la client non e' ancora nel gioco effettivo
	  if (fPlaing == false) {
	     str_split(msg,sep,words);
	    
	     if (words[1] == "Make")
	     {
	        // Se la stringa inviata dalla client e' del
	        // tipo: Make::Nome (Crea un personaggio)
	        if ( client_make(msg, words) == 0 ) return 1;
	        else return 0;  
	     }
	     if (words[1] == "Delt")
	     {
	        // Se la stringa inviata dalla client e' del
	        // tipo: Delt::Nome (Cancella un personaggio)
	        return 1;
	     }
	     if (words[1] == "Play")
	     {    
	        // Se la stringa inviata dalla client e' del
	        // tipo: Play::Nome (Gioca con un personaggio)
	        if ( client_play(msg, words) == 0 ) return 1;
	        else return 0;
	     }
	     return 0;	    
	  } 
	  
	  // Se la client e' in gioco
	 else if (fPlaing == true) {  
	     str_split(msg,sep,words);
	    
	     if (words[1] == "Exit")
	     {
	        // Se la stringa inviata dalla client e' del
	       // tipo: Exit  (Esci dal gioco)
	        fPlaing = false;
	        return 1;
	     } 
	     
	     if (words[1] == "Msgt")
	     {
	        // Se la stringa inviata dalla client e' del
	        // Msgt::Nome (Manda un messaggio ad una client)
	        return 1;
	     }
	     
	     if (words[1] == "Msgz")
	     {    
	        // Se la stringa inviata dalla client e' del
	        // tipo: Msgz::Zona::Msg (Manda un messaggio alle client di zona)	      
	        if ( words.size() > 2 )
		{
	           // Spedisci il messaggio a tutti i giocatore nella zona
	           msg_to_zone(words[2],words[1]);
		   return 1;
	        } 
		return 0;
	     }	     
	     return 0;
	        
	  }	   
	  return 0; 	  
	} 
	return 0;
}

int CClient::client_auth(string msg, vector<string> words ){

	// Assicurati che ci sia un nome ed una passowrd	    
	if ( words.size() > 2 )
	{
		sys.log( "Autorizing user: " + words[1]);
		// Cerca l'utente nel database
		vector<string> db_response;
		if( db.db_query("SELECT * FROM Users WHERE user_name='"+words[1]+"' AND user_passw='"+words[2]+"'",
		    db_response) != 0 ) return 0;
		    
		sys.error("auth u 1");
		// Se e' stato trovato il vettore di risposta conterra' i suoi dati
		// altrimenti segnala che non esiste (ritorna 0)
		if( db_response.size() < 1 )
		{
			sprintf(tcp_out_buffer,"Auth no_player");
			return 0; 
		}
		sys.error("auth u 2");
		// Se si arriva a questo punto allora il giocatore esiste nel database
		// quindi associa alla client il nome dell'utente
		user_name = words[1];
		// Imposta il flag di autorizzazione a true
		fAuthorized = true;
		// Termina la funzione segnalando che tutto e' ok
		return 1;
	}
	// Se si e' arrivati qui' la stringa di autorizzazione non e' valida
	return 0;

}

int CClient::client_play(string msg, vector<string> words ){

	// Assicurati che ci sia anche un nome
	if ( words.size() > 1 ){
	   // Cerca il personaggio nel database
	   vector<string> db_response;
	   db.db_query("SELECT * FROM Chars WHERE char_name='"+words[1]+"' AND user_name='"+user_name+"'", db_response);
	   // Se e' stato trovato il vettore di risposta conterra' i suoi dati
	   if( db_response.size() < 1) {
	      sprintf(tcp_out_buffer,"Play no_char");
	      return 1; 
	   }
	   // Inizializza e carica il personaggio
	   character.char_init(words[1]);
	   // Spedisci il messaggio alle client del giocatore in gioco
	   broadcast("Enter "+ ip + " " + character.char_name + "\n");     	      
	   // Logga l'entrata del personaggio
	   sys.log( "Character: " + words[1] + " entering the game");
	   fPlaing = true;
	   return 1;
	} 
	else return 0;
}

int CClient::client_make(string msg, vector<string> words ){

	// Assicurati che ci sia anche un nome
	if ( words.size() > 1 ){   
	   // Inserisci il personaggio nel databaseg
	   vector<string> db_response;
	   db.db_query("INSERT INTO Chars (user_name ,char_name) VALUES ('"+user_name+"','"+words[1]+"')", db_response); 
	   return 1;
	} else return 0;

}

void CClient::process_udp_input(string buffer) {

}

void CClient::msg_to_char(string msg, string ip){

	char* tmpmsg = (char*)msg.c_str();

	if (world.client_list.size() != 0){
	   vector<CClient>::iterator i;
	   for ( i = ((world).client_list).begin(); i < ((world).client_list).end(); i++ ) {
	      // Questo tipo di messaggio e' utile solo se la client e' in gioco
	      if ( ((*i).fPlaing == true ) && ((*i).ip == ip) ){
	         sprintf((*i).tcp_out_buffer,"%s",tmpmsg);
	      }
	   }
	}
}

void CClient::msg_to_zone(string msg, string zone){

	char* tmpmsg = (char*)msg.c_str();

	if (world.client_list.size() != 0){
	   vector<CClient>::iterator i;
	   for ( i = ((world).client_list).begin(); i < ((world).client_list).end(); i++ ) {
	      // Questo tipo di messaggio e' utile solo se la client e' in gioco
	      if ( ((*i).fPlaing == true ) && ((*i).character.zone_name == zone) ){
	         sprintf((*i).tcp_out_buffer,"%s",tmpmsg);
	     }
	   }
	}
}

void CClient::msg_to_room(string msg, string room){

	char* tmpmsg = (char*)msg.c_str();

	if (world.client_list.size() != 0){
	   vector<CClient>::iterator i;
	   for ( i = ((world).client_list).begin(); i < ((world).client_list).end(); i++ ) {
	      // Questo tipo di messaggio e' utile solo se la client e' in gioco
	      if ( ((*i).fPlaing == true ) && ((*i).character.room_name == room) ){
	         sprintf((*i).tcp_out_buffer,"%s",tmpmsg);
	     }
	   }
	}
}

void CClient::broadcast(string msg){

	char* tmpmsg = (char*)msg.c_str();

	if (world.client_list.size() != 0){
	   vector<CClient>::iterator i;
	   for ( i = ((world).client_list).begin(); i < ((world).client_list).end(); i++ ) {
	      // Questo tipo di messaggio e' utile solo se la client e' in gioco
	      if ((*i).fPlaing == true ) {
	         sprintf((*i).tcp_out_buffer,"%s",tmpmsg);
	      }
	   }
	}
}

void CClient::client_close(void){

	// Avvisa che ce ne andiamo
	broadcast("Exit "+ ip + " " + character.char_name + "\n");
	tcp_sock.sock_close();
}



