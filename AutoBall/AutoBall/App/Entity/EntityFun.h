/**
 *   @brief  封装一些实体的函数模板
 *	 @author expter
 */
#pragma  once

#include "BaseEntity.h"
#include "../../Render/Geometry.h"

/**
 *  判断2个实体是否重叠
 */
template <class T, class conT>
bool Overlapped(const T* ob, const conT& conOb, double MinDistBetweenObstacles = 40.0)
{
	typename conT::const_iterator it;

	for (it=conOb.begin(); it != conOb.end(); ++it)
	{
		if (TwoCirclesOverlapped(ob->Pos(),
			ob->BRadius()+MinDistBetweenObstacles,                             
			(*it)->Pos(),
			(*it)->BRadius()))
		{
			return true;
		}
	}

	return false;
}


template <class T, class conT>
void TagNeighbors(T* entity, conT& others, const double radius)
{
	typename conT::iterator it;

	//iterate through all entities checking for range
	for (it=others.begin(); it != others.end(); ++it)
	{
		//first clear any current tag
		(*it)->UnTag();

		//work in distance squared to avoid sqrts
		Vector2D to = (*it)->Pos() - entity->Pos();

		//the bounding radius of the other is taken into account by adding it 
		//to the range
		double range = radius + (*it)->BRadius();

		//if entity within range, tag for further consideration
		if ( ((*it) != entity) && (to.LengthSq() < range*range))
		{
			(*it)->Tag();
		}

	}//next entity
}


template <class T, class conT>
void EnforceNonPenetrationContraint(T entity, const conT& others)
{
	typename conT::const_iterator it;

	//iterate through all entities checking for any overlap of bounding
	//radii
	for (it=others.begin(); it != others.end(); ++it)
	{
		//make sure we don't check against this entity
		if (*it == entity) continue;

		//calculate the distance between the positions of the entities
		Vector2D ToEntity = entity->Pos() - (*it)->Pos();

		double DistFromEachOther = ToEntity.Length();

		//if this distance is smaller than the sum of their radii then this
		//entity must be moved away in the direction parallel to the
		//ToEntity vector   
		double AmountOfOverLap = (*it)->BRadius() + entity->BRadius() -
			DistFromEachOther;

		if (AmountOfOverLap >= 0)
		{
			//move the entity a distance away equivalent to the amount of overlap.
			entity->SetPos(entity->Pos() + (ToEntity/DistFromEachOther) *
				AmountOfOverLap);
		}
	}//next entity
}
