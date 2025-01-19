#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "globals.h"
#include "mass.h"
#include "math3d.h"

class CChar {

	public:
	  string	char_name;
	  string	zone_name;
	  string	room_name;
	
	  CMass		mass;
	  float		angle;
	  CVector3	position;
	  CVector3	velocity;
	  
	  sh_int	sex;
	  sh_int	char_class;
	  sh_int	race;
	  sh_int	level;  
	  sh_int	hit;
	  sh_int	max_hit;
	  sh_int	mana;
	  sh_int	max_mana;
	  int		gold;
	  int		exp;
	  sh_int	saving_throw;
	  sh_int	alignment;
	  sh_int	hitroll;
	  sh_int	damroll;
	  sh_int	armor;

	  void	char_init(string char_name);

};

#endif
