#include "Collidable.h"
#include "../unit/UnitNode.h"

bool Collidable::willCollide(class UnitNode* unit)
{
	return willCollide( unit->getPosition(), unit->getUnitData()->collide );
}