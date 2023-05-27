#pragma once

class Collider
{
public:
	enum TYPE { CIRCLE = 0, BOX = 1 };// deferred
	TYPE type;
	
	VECTOR2<double> wh;
	VECTOR2<double> origin;

	bool isTrigger = false;


	virtual Collider* Collision() = 0;
	//virtual Collider* CollisionEnter() = 0;
	//virtual Collider* CollisionExit() = 0;

	int BoxesSAT(VECTOR2<double>* vertex1, VECTOR2<double>* vertex2, VECTOR2<double>* axis, VECTOR2<double> dist);
	VECTOR2<double>* BoxVertexes();


};

