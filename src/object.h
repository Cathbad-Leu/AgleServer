#ifndef _OBJECT_H
#define _OBJECT_H

#include "globals.h"

class CObj {

	public:

		string	name;
		sh_int	type;
		sh_int	material;
		int		value;
	
		sh_int	hitroll;
		sh_int	damroll;
	
		sh_int	range;
		
		// affinita'
		int		air;
		int		fire;
		int		water;
		int		earth;
		int		light;
		int		dark;
	
		int		human;
		int		beast;
		int		dragon;
		int		undead;
		int		phantom;
		int		evil;

};

#endif
