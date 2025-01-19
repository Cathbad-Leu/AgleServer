
#include "system.h"

void CSystem::sys_init(void) {

	string log_fname;

	log_fname = string(LOG_DIR)+string(LOG_FILE);
	log_file.open( (log_fname.c_str()) , ios::out | ios::trunc);
	
	string error_fname;
	
	error_fname = string(LOG_DIR)+string(ERROR_FILE);
	error_file.open( (error_fname.c_str()) , ios::out | ios::trunc );

}

void CSystem::sys_close(void) {

	log_file.close();
	error_file.close();

}

void CSystem::log(string buf){

	log_file << buf << endl;

}

void CSystem::error(string buf){

	error_file << buf << endl;

}
