
#include "world.h"
/*
void CRoom::room_init(string  zone_name){

	
}

void CRoom::room_update( void ){

}

void CZone::zone_init(string  zone_name){

	char buffer[256];
	string zone_fname;

	zone_fname = string(WORLD_DIR)+zone_name;
	zone_file.open( (zone_fname.c_str()) , ios::out | ios::trunc);
	
	string line;
	while (getline(zone_file,line,'\n'))
	{
	  zone_file.getline(buffer,100);
	  CRoom *newroom = new CRoom;
	  newroom->room_init(line);
	  room_list.push_back(*newroom);
	}
	
}

void CZone::zone_update( void ){

}
*/
void CWorld::world_init( void ){

	char buffer[256];
	string world_fname;
/*
	world_fname = string(WORLD_DIR)+string(ZONE_LIST);
	world_file.open( (world_fname.c_str()) , ios::out | ios::trunc);
	
	string line;
	while (getline(world_file,line,'\n'))
	{
	  world_file.getline(buffer,100);
	  CZone *newzone = new CZone;
	  newzone->zone_init(line);
	  zone_list.push_back(*newzone);
	}
*/
}


void CWorld::world_update( void ){

}

