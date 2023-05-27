
#include "Vector.h"

#include "Collider.h"

int Collider::BoxesSAT(VECTOR2<double>* vertex1, VECTOR2<double>* vertex2, VECTOR2<double>* axis, VECTOR2<double> dist)
{
	int nColl = 0;
	
	for (int i = 0; i < 4; i++)// All axis
	{
		double min = INFINITY, max = -INFINITY;
		double rA, rB;

		for (int j = 0; j < 4; j++)// Collider1 projection
		{
			double p = vertex1[j].Dot(axis[i]);

			if (min > p)
			{
				min = p;
			}
			if (max < p)
			{
				max = p;
			}
		}
		rA = max - min;

		min = INFINITY, max = -INFINITY;
		for (int j = 0; j < 4; j++)//Collider2 projection
		{
			double p = vertex2[j].Dot(axis[i]);

			if (min > p)
				min = p;
			if (max < p)
				max = p;
		}
		rB = max - min;

		if (abs(dist.Dot(axis[i])) >= (rA + rB) * 0.5) // is not collision?
			nColl++;

		/*test[i].value[0] = l.Dot(axis[i]);
		test[i].value[1] = (rA + rB) * 0.5;*/
	}

	return nColl;
}

VECTOR2<double>* Collider::BoxVertexes()
{
	VECTOR2<double> halfWH[2];
	VECTOR2<double>* vertex;
	vertex = new VECTOR2<double>[4];

	halfWH[0] = VECTOR2_X_D * wh.x * 0.5;
	halfWH[1] = VECTOR2_Y_D * wh.y * 0.5;

	vertex[0] = halfWH[0] + halfWH[1];
	vertex[1] = halfWH[0] - halfWH[1];
	vertex[2] = halfWH[0] * -1 - halfWH[1];
	vertex[3] = halfWH[0] * -1 + halfWH[1];


	return vertex;
}
