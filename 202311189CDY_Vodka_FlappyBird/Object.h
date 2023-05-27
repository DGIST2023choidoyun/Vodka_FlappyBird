
#ifndef _OBJECT_H_
#define _OBJECT_H_


class RigidBody;
class Sprite;
class Collider;

class Object
{
public:
	static std::vector<Object*> list;
	static void Frame();

	RigidBody* rigidbody;
	Sprite* sprite;
	Collider* collider;

	bool enabled = true;

	//bool scrOutDisable = false;
	//VECTOR2<double> scrBorder = VECTOR2_ZERO_D;

	Object();
	~Object();

	void Release();
	bool CheckSOD();
};

#endif