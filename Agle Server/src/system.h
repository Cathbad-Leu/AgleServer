#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "globals.h"

class CSystem {

	public:
	
	  fstream log_file;
	  fstream error_file;
	  
	  void sys_init(void);
	  void sys_close(void);
	  void log(string buf);
	  void error(string buf);
	  
};

#endif
 
