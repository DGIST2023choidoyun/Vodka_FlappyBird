#include <cmath>
#include <iostream>
#include <vector>


#include "RigidBody.h"

#define RAD2DEG			180/PI
#define PI				3.1415926535


std::vector<RigidBody*> RigidBody::list;
//DOUBLE_PAIR<double> RigidBody::test[4];

void RigidBody::Frame(const double& delta)
{
	std::vector<RigidBody*>::iterator iter;
	for (iter = list.begin(); iter != list.end(); iter++)
	{
		if((*iter)->enabled)
			(*iter)->DoFrame(delta);
	}
}

void RigidBody::DoFrame(const double& delta)
{
	acc = (netForce + instantForce) * inv_Mass;
	vel += acc * delta;
	if (drag)
	{
		vel *= dragPower;
	}
	pos += vel * delta;

	switch (rotation)
	{
	case true:
	{
		VECTOR2<double> nF = netForce + instantForce;
		double F = nF.Magnitude();
		VECTOR2<double> rVec = VECTOR2<double>{ r * cos(rad), r * sin(rad) };
		double ccw = rVec.Normal().Dot(nF);

		aacc = inv_inertia * sqrt(pow(r * F, 2) - pow(nF * rVec, 2)) * ((ccw >= 0) ? -1 : 1);// *((0 < vel.y) - (0 > vel.y));

		avel += aacc * delta;
		if (drag)
		{
			avel *= dragPower;

		}
		rad += avel * delta;


		//com.x = cos(avel * delta) * com.x - sin(avel * delta) * com.y;
		//com.y = sin(avel * delta) * com.x + cos(avel * delta) * com.y;
	}
		break;
	default:
		break;
	}

	instantForce = VECTOR2_ZERO_D;
}

void RigidBody::ApplyForce(VECTOR2<double> F)
{
	netForce += F;
}
void RigidBody::ApplyForce(const double x, const double y)
{
	netForce.x += x;
	netForce.y += y;

}

void RigidBody::SetCOM(const VECTOR2<double> main)
{
	com = main;
	r = main.Magnitude();
	inertia = mass * r * r;
	inv_inertia = 1 / inertia;
}
void RigidBody::SetCOM(const double x, const double y)
{
	com.x = x;
	com.y = y;
	r = com.Magnitude();
	inertia = mass * r * r;
	inv_inertia = 1 / inertia;
}

void RigidBody::SetMass(const double m)
{
	mass = m;
	inv_Mass = 1 / m;
}

void RigidBody::ApplyTorque(const double tau)
{
	aacc += tau * inv_inertia;
}

RigidBody::RigidBody()
{
	list.push_back(this);
}

void RigidBody::SetConstForce(const VECTOR2<double> F)
{
	netForce -= constForce;
	constForce = F;
	netForce += F;
}
void RigidBody::SetConstForce(const double x, const double y)
{
	netForce -= constForce;
	constForce.x = x;
	constForce.y = y;
	netForce.x += x;
	netForce.y += y;
}

RigidBody::~RigidBody()
{
}

void RigidBody::Impulse(VECTOR2<double> I)
{
	instantForce = I;
}
void RigidBody::Impulse(const double x, const double y)
{
	instantForce.x = x;
	instantForce.y = y;

}

Collider* RigidBody::Collision()
{

	std::vector<RigidBody*>::iterator iter;
	for (iter = list.begin(); iter != list.end(); iter++)
	{
		if (*iter == this)
			continue;

		switch (type + (*iter)->type)
		{
		case 0: // Circle&Circle

			break;
		case 1: // Circle&Box
			break;
		case 2: // Box&Box, SAT

			VECTOR2<double> centre1 = ColliderCenter();
			VECTOR2<double> centre2 = (*iter)->ColliderCenter();
			//VECTOR2<double> halfWH[4];
			VECTOR2<double> axis[4];
			VECTOR2<double>* vertex1;
			VECTOR2<double>* vertex2;
			/*VECTOR2<double> vertex1[4];
			VECTOR2<double> vertex2[4];*/
			VECTOR2<double> l;
			double oRad = (*iter)->rad;

			l = centre1 - centre2;

			axis[0] = VECTOR2_X_D.Rotation(rad);
			axis[1] = VECTOR2_Y_D.Rotation(rad);

			/*halfWH[0] = axis[0] * wh.x * 0.5;
			halfWH[1] = axis[1] * wh.y * 0.5;*/

			vertex1 = BoxVertexes();
			vertex1[0].Rotate(rad);
			vertex1[1].Rotate(rad);
			vertex1[2].Rotate(rad);
			vertex1[3].Rotate(rad);
			/*vertex1[0] = halfWH[0] + halfWH[1];
			vertex1[1] = halfWH[0] - halfWH[1];
			vertex1[2] = halfWH[0] * -1 - halfWH[1];
			vertex1[3] = halfWH[0] * -1 + halfWH[1];*/



			axis[2] = VECTOR2_X_D.Rotation(oRad);
			axis[3] = VECTOR2_Y_D.Rotation(oRad);

			/*halfWH[2] = axis[2] * (*iter)->wh.x * 0.5;
			halfWH[3] = axis[3] * (*iter)->wh.y * 0.5;*/

			vertex2 = (*iter)->BoxVertexes();
			vertex2[0].Rotate(oRad);
			vertex2[1].Rotate(oRad);
			vertex2[2].Rotate(oRad);
			vertex2[3].Rotate(oRad);
			/*vertex2[0] = halfWH[2] + halfWH[3];
			vertex2[1] = halfWH[2] - halfWH[3];
			vertex2[2] = halfWH[2] * -1 - halfWH[3];
			vertex2[3] = halfWH[2] * -1 + halfWH[3];*/



			int cnt = BoxesSAT(vertex1, vertex2, axis, l);

			if (cnt == 0)
			{
				//collidedOthers = true;
				return *iter;
			}

			delete[] vertex1;
			delete[] vertex2;

			break;
		}
	}
	return nullptr;
}

//Collider* RigidBody::CollisionEnter()
//{
//	if (collidedOthers)
//		return nullptr;
//
//	std::vector<RigidBody*>::iterator iter;
//	for (iter = list.begin(); iter != list.end(); iter++)
//	{
//		if (*iter == this)
//			continue;
//
//		switch (type + (*iter)->type)
//		{
//		case 0: // Circle&Circle
//
//			break;
//		case 1: // Circle&Box
//			break;
//		case 2: // Box&Box, SAT
//			VECTOR2<double> centre1 = ColliderCenter();
//			VECTOR2<double> centre2;
//			VECTOR2<double> axis[4];
//			VECTOR2<double>* vertex1;
//			VECTOR2<double>* vertex2;
//			VECTOR2<double> l;
//			double oRad = (*iter)->rad;
//
//			centre2 = (*iter)->ColliderCenter();
//			l = centre1 - centre2;
//
//			axis[0] = VECTOR2_X_D.Rotation(rad);
//			axis[1] = VECTOR2_Y_D.Rotation(rad);
//
//			vertex1 = BoxVertexes();
//
//			axis[2] = VECTOR2_X_D.Rotation(oRad);
//			axis[3] = VECTOR2_Y_D.Rotation(oRad);
//
//			vertex2 = (*iter)->BoxVertexes();
//
//			int cnt = BoxesSAT(vertex1, vertex2, axis, l);
//
//			if (cnt == 0)
//			{
//				collidedOthers = true;
//				return *iter;
//			}
//
//			break;
//		}
//	}
//	return nullptr;
//}
//
//Collider* RigidBody::CollisionExit()
//{
//	if (!collidedOthers)
//		return nullptr;
//
//	std::vector<RigidBody*>::iterator iter;
//	for (iter = list.begin(); iter != list.end(); iter++)
//	{
//		if (*iter == this)
//			continue;
//
//		switch (type + (*iter)->type)
//		{
//		case 0: // Circle&Circle
//
//			break;
//		case 1: // Circle&Box
//			break;
//		case 2: // Box&Box, SAT
//			VECTOR2<double> centre1 = ColliderCenter();
//			VECTOR2<double> centre2;
//			VECTOR2<double> axis[4];
//			VECTOR2<double>* vertex1;
//			VECTOR2<double>* vertex2;
//			VECTOR2<double> l;
//			double oRad = (*iter)->rad;
//
//			centre2 = (*iter)->ColliderCenter();
//			l = centre1 - centre2;
//
//			axis[0] = VECTOR2_X_D.Rotation(rad);
//			axis[1] = VECTOR2_Y_D.Rotation(rad);
//
//			vertex1 = BoxVertexes();
//
//			axis[2] = VECTOR2_X_D.Rotation(oRad);
//			axis[3] = VECTOR2_Y_D.Rotation(oRad);
//
//			vertex2 = (*iter)->BoxVertexes();
//
//			int cnt = BoxesSAT(vertex1, vertex2, axis, l);
//
//			if (cnt == 0)
//			{
//				collidedOthers = true;
//			}
//
//			break;
//		}
//	}
//
//
//	return nullptr;
//}

VECTOR2<double> RigidBody::ColliderCenter() const
{
	return (origin - com).Rotation(rad) + pos;
}
