#ifndef _WORLD_H
#define _WORLD_H

#include "globals.h"
#include "client.h"
#include "object.h"
#include "mobile.h"

class CRoom {

	public:
	  string room_name;
	  fstream room_file;
	  
	  vector<CMob>	mob_list;
	  vector<CObj>	obj_list;
	  
	  void room_init(string room_name);
	  void room_update(void);
};

class CZone {

	public:
	  string zone_name;
	  fstream zone_file;
	  
	  vector<CRoom>	room_list;
	  
	  void zone_init(string zone_name);
	  void zone_update(void);
};

class CWorld {

	public:
	  fstream world_file;
	
	  vector<CClient>	client_list;
	  vector<CZone>		zone_list;
	
	  void world_init(void);
	  void world_update(void);
};

#endif
