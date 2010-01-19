#pragma once

enum  eEntityType
{
	ent_Player  = 1 ,
	ent_Monster
};

inline std::string GetNameOfEntity( eEntityType e )
{
	switch ( e )
	{
	case ent_Player :
		return  "player";
	case ent_Monster:
		return  "Monster"

	default :
		return  "UnKnown"
	}
}