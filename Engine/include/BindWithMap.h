#pragma once

#include "Map.h"
#include "Log.h"

class BindWithMap
{
private:
public:
	BindWithMap(Map* map);
	BindWithMap();
	virtual ~BindWithMap();
	void SetMap(Map* map);
protected:
	 bool CheckCollisionDestWithMap(Object* object);
	 bool CheckCollisionScreenWithMap(Object* object);
	 bool CheckCollisionDirectionWithMap(Object* object, bool destOrScreen, bool XorY);
	Map* m_map;

};

