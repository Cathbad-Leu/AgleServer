#ifndef _MAIN_H
#define _MAIN_H

#include "globals.h"

#define UMAX(a, b) ((a) > (b) ? (a) : (b))

void *handle_upd(void *args);
void *handle_network(void *args);

#endif

