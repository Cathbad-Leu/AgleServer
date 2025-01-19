
#include "globals.h"
#include <mysql.h>

class CDataBase {

	public:
		MYSQL *connection, mysql;
		MYSQL_RES *result;

		//Distruttore
		~CDataBase();
		// Definiamo una funzione di errore che dia piu' informazioni
		void db_error();
		// Connettiamoci al databse
		void db_init(char *domain, char *user, char *password, char *database);
		// Crea il database da zero
		void db_create(char *database);
		// Fai una query e restituisci il risultato come un vettore di stringhe
		int db_query(string str_query, vector<string> &response);
		// Rialascia le risorse dopo aver fatto la query
		void query_done();
		// Chiudi la connessione
		void db_close();
};

