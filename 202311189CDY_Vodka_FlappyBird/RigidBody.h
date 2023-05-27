
#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include "Vector.h"
#include "Collider.h"

class RigidBody : public Collider
{
public:
	static std::vector<RigidBody*> list;
	static void Frame(const double& delta);
	//static DOUBLE_PAIR<double> test[4];

	VECTOR2<double> acc = VECTOR2_ZERO_D;
	VECTOR2<double> vel = VECTOR2_ZERO_D;
	VECTOR2<double> pos = VECTOR2_ZERO_D; // Center of mass's global position

	double mass = 1, inv_Mass = 1;
	double inertia = 0, inv_inertia = 1;
	bool rotation = true;
	bool drag = true;
	double dragPower = 0.9;

	VECTOR2<double> com = VECTOR2_ZERO_D; // Gap b/w com and image origin(UV). If rotates, the com vector will change.

	double r;

	VECTOR2<double> constForce = VECTOR2_ZERO_D;
	VECTOR2<double> instantForce = VECTOR2_ZERO_D;
	VECTOR2<double> netForce = VECTOR2_ZERO_D;

	//bool collidedOthers = false;

	double rad = 0;
	double avel = 0;
	double aacc = 0;

	bool enabled = true;

	RigidBody();
	~RigidBody();

	void SetConstForce(VECTOR2<double> F);
	void SetConstForce(const double x, const double y);
	void SetMass(const double m);
	void SetCOM(const VECTOR2<double> main);
	void SetCOM(const double x, const double y);
	void DoFrame(const double& delta);
	void Impulse(const VECTOR2<double> I);
	void Impulse(const double x, const double y);
	void ApplyForce(VECTOR2<double> F);
	void ApplyForce(const double x, const double y);
	void ApplyTorque(const double tau);

	VECTOR2<double> ColliderCenter() const;
	//VECTOR2<double> VertexNormal() const;
	virtual Collider* Collision();
	//virtual Collider* CollisionEnter();
	//virtual Collider* CollisionExit();
};

#endif