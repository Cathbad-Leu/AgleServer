#ifndef _GLOBALS_H
#define _GLOBALS_H
// Some usefull headers
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
//Some STL headers
#include <vector>
#include <string>

//Using the STL exception library increases the
//chances that someone else using our code will corretly
//catch any exceptions that we throw.
#include <stdexcept>

// Usa namespace standard
using namespace std;

typedef short int		sh_int;

#define NET_ADDRS		"127.0.0.1"
#define NET_TCP_PORT		4000
#define NET_IUDP_PORT		4001
#define NET_OUDP_PORT		4002

#define DATABASE_HOST		"localhost"
#define DATABASE_NAME		"agle"
#define DATABASE_USER_NAME	"agle"
#define DATABASE_USER_PASSW	"agle"

#define MAX_STRING_LENGTH	 1024

#define LOG_DIR			"../log/"
#define LOG_FILE		"log.txt"
#define ERROR_FILE		"error.txt"

#define WORLD_DIR		"../world/"
#define ZONE_LIST		"zones.lst"

#endif

