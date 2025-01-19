
#include "database.h"
#include "system.h"

extern CSystem sys;

// Distruttore della classe
CDataBase::~CDataBase() {
	mysql_close(&mysql);
}

void CDataBase::db_error()  {

	sys.error("database error: " + string(mysql_error(&mysql)) );
}

void CDataBase::db_init(char *domain, char *user, char *password, char *database) {

	mysql_init(&mysql);
	connection = (mysql_real_connect(&mysql,domain,user,password,database,0,NULL,0));

	if (connection == NULL) {
		db_error();
		
		// Probabilmente il database richiesto nel server non esiste, prova il generico
		connection = (mysql_real_connect(&mysql,domain,user,password,NULL,0,NULL,0));
		
		// Se siamo ancora in errore o non esiste l'utente o non si ha accesso al server
		// ( sempre che mysql sia attivo ed installato )
		if (connection == NULL) {
			db_error();
			exit(1);
		}
		
		// Se siam qui il server c'e' ma non il database proviamo a crearlo
		db_create(database);
		
	} else {
		sys.log("Connected to database: " + string(database) + "  user: " + string(user));
	}
}

// Funzione che crea un primo database
void CDataBase::db_create(char *database) {

		vector<string> db_response;
		
		db_query("CREATE DATABASE "+string(database), db_response);
		sys.log("Database Creation");
		
		mysql_select_db(&mysql,database);
		
		db_query("CREATE TABLE Users (user_name VARCHAR(20) PRIMARY KEY, user_passw VARCHAR(20))", db_response);
		sys.log("Users Table Creation");
		db_query("INSERT INTO Users (user_name,user_passw) VALUES ('agle','agle')",db_response);
		
		db_query("CREATE TABLE Chars (user_name VARCHAR(20) PRIMARY KEY, char_name VARCHAR(20))", db_response);
		sys.log("CharactersTable Creation");
		db_query("INSERT INTO Chars (user_name,char_name) VALUES ('agle','agle')",db_response);
}

int CDataBase::db_query(string str_query, vector<string> &response) {

	char *query = (char*)((str_query).c_str());

	if (mysql_real_query( connection , query, strlen(query))) {
		sys.error("real query error");
		db_error();
		return 1;
	}

	// Se il risultato non arriva segnala un errore
	if (!(result = mysql_store_result(connection))) {
		sys.error("result error");
		db_error();
		return 1;
	}

	MYSQL_ROW row;
	// altrimenti preleva i risultati
	while((row = mysql_fetch_row(result))) {
		for (unsigned int i=0; i < mysql_num_fields(result); i++) {
			sys.log("MYSLQ RESULT " + string(row[i]));
			// Immagazzina il risultato all'interno di un vettore di stringhe
			response.push_back(row[i]);
		}
	}
	
	// Se siamo qui ma mysql pensa che nulla sia stato fatto segnala errore
	if (!mysql_eof(result)) {
		db_error();
		return 1;
	}
	
	// Se siamo giunti la query e' andata a buon fine
	query_done();
	return 0;
}


void CDataBase::query_done() {
	// Rilascia le risorse del server
	mysql_free_result(result);
}

// Chiudi la connessione al database
void CDataBase::db_close() {
 	mysql_close(&mysql);
}

