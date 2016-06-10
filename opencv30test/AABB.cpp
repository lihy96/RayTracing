#include "AABB.h"
#include "intersectResult.h"
int AABB::intersect(const Ray& ray, IntersectResult& result){

	id = ray.getID();
	
	double dist[6];
	MyVec3 ip[6], d = ray.getDir(), o = ray.getOri();
	int retval = MISS;
	for ( int i = 0; i < 6; i++ ) 
		dist[i] = -1;
	MyVec3 v1 = pos, v2 = pos_2;
	if (d.x != 0) {
		double rc = ((double)1.0) / d.x;
		dist[0] = (v1.x - o.x) * rc;
		dist[3] = (v2.x - o.x) * rc;
	}
	if (d.y != 0) {
		double rc = ((double)1.0) / d.y;
		dist[1] = (v1.y - o.y) * rc;
		dist[4] = (v2.y - o.y) * rc;
	}
	if (d.z != 0) 
	{
		double rc = ((double)1.0) / d.z;
		dist[2] = (v1.z - o.z) * rc;
		dist[5] = (v2.z - o.z) * rc;
	}

	const double EPSILON = 0.001;
	//cout << p->distance << endl;
	if(result.distance < INF - 1){
		cout << "bug haha" ;
		exit(1);
	}
		
	result.distance = INF;
		
	for (int i = 0; i < 6; i++ ){//遍历面上面的每一个交点，交点在内部就比较距离最小的那个
		if (dist[i] > 0){
			ip[i] = o + dist[i] * d;
			if ((ip[i].x > (v1.x - EPSILON)) && (ip[i].x < (v2.x + EPSILON)) && 
				(ip[i].y > (v1.y - EPSILON)) && (ip[i].y < (v2.y + EPSILON)) &&
				(ip[i].z > (v1.z - EPSILON)) && (ip[i].z < (v2.z + EPSILON))){
				if (dist[i] < result.distance) {
					result.distance = dist[i];
					result.intersectPoint = ip[i];
					retval = HIT;
				}
			}
		}
	}
	return retval;
}