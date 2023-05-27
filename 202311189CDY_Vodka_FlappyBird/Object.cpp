

#include <iostream>
#include <vector>

#include "RigidBody.h"
#include "Sprite.h"
#include "Collider.h"
#include "Object.h"

std::vector<Object*> Object::list;

void Object::Frame()
{
	std::vector<Object*>::iterator iter;
	std::vector<Object*>::iterator prev;
	for (iter = list.begin(); iter != list.end(); iter++)
	{
		/*if (iter != list.begin())
			prev = iter;
		if ((*iter)->CheckSOD())
		{
			list.erase(iter);
			iter = prev;
		}*/
	}

}

Object::Object()
{
	list.push_back(this);
}
Object::~Object()
{
	Release();
}


void Object::Release()
{
	if (rigidbody)
	{
		delete rigidbody;
		rigidbody = nullptr;
	}
	if (sprite)
	{
		delete sprite;
		sprite = nullptr;
	}
}

bool Object::CheckSOD()
{
	/*if (scrOutDisable)
	{
		if (rigidbody->pos > scrBorder)
		{
			return true;
		}
	}*/

	return false;
}
