#include "kdTree.h"
#include "primitive.h"
#include "intersectResult.h"

void KdTreeNode::build(){
	//遍历所有物体，找到边界
	for (auto& it : objects) {
		AABB curr = it->getAABB();
		bounds.pos = MyVec3(min(bounds.pos.x, curr.pos.x),
							min(bounds.pos.y, curr.pos.y),
							min(bounds.pos.z, curr.pos.z));
		bounds.pos_2 = MyVec3(max(bounds.pos_2.x, curr.pos_2.x),
							max(bounds.pos_2.y, curr.pos_2.y),
							max(bounds.pos_2.z, curr.pos_2.z));
	}
	bounds.size = bounds.pos_2 - bounds.pos;

	//如果当前空间中已经小于的20个物体，就不操作了
	if (objects.size() <= MIN_OBJECT_COUNT) {
		return;
	}

	//确定在哪里分割，注意要根据当前的轴向x,y,z 以及最大最小
	double splitValue = getSplitValue();

	vector<Primitive*> leftObjects;
	vector<Primitive*> rightObjects;

	//遍历物体，获得物体的边界，然后将他们放进各自的vector
	for (auto& it : objects) {
		AABB curr = it->getAABB();
		double minn, maxx;
		switch(axis) {
			case 0:
				minn = curr.pos.x;
				maxx = curr.pos_2.x;
			break;
			case 1:
				minn = curr.pos.y;
				maxx = curr.pos_2.y;
			break;
			case 2:
				minn = curr.pos.z;
				maxx = curr.pos_2.z;
			break;
		}

		if (minn < splitValue) {
			leftObjects.push_back(it);
		}

		if (maxx > splitValue) {
			rightObjects.push_back(it);
		}
	}

	//判断这样的分割有没有真正将物体分开，如果没有的话就再次尝试构建，但是要换方向了
	if (leftObjects.size() != objects.size() &&
		rightObjects.size() != objects.size()) {
		left = new KdTreeNode(depth + 1, (axis + 1) % 3, leftObjects);
		right = new KdTreeNode(depth + 1, (axis + 1) % 3, rightObjects);
	} else if (axisRetries < 2) {
		axis = toggleAxis();
		axisRetries++;
		build();
	} else {
		// Do nothing since we're out of axis retries.
	}
	
}
double KdTreeNode::getSplitValue(){
	return 0;
}

int KdTreeNode::findNearest(const Ray& ray, IntersectResult& result) {

	if (!bounds.intersect(ray, result)) {
		return MISS;
	}

	// 对于父节点直接递归
	if (left != NULL && right != NULL) {
		IntersectResult leftIntersection;
		int re_l = left->findNearest(ray, leftIntersection);
		IntersectResult rightIntersection;
		int re_r = right->findNearest(ray, rightIntersection);

		if(re_l == 0 && re_r == 0)
			return MISS;
	

		if(leftIntersection.distance < rightIntersection.distance) {
			result = leftIntersection;
			//cout << "zuo " << re_l << endl;
			return re_l;
		}else if(leftIntersection.distance > rightIntersection.distance){
			result = rightIntersection;
			//cout << "you " << re_r << endl;
			return re_r;
		}else{	
			//cout << re_l << " " <<re_r << "INF INF" << endl;
			return 0;
		}
		
	} else { //对于叶节点直接找到最近的物体即可
		int in_out = 0;
		//cout << "tmp " << objects.size() << endl;
		for (auto& it : objects) {
			IntersectResult tmp;
			int re = it->intersect(ray, tmp);
			//cout << it->getType() << endl;
			if(re){
				if (tmp.distance < result.distance){
					result = tmp;
					in_out = re;
				}
			}
		}
		//cout << "in_out " << in_out << endl;
		return in_out;
	}
}




//--------------------------------------------------------------------------------------------------
/*
void KdTreeNode::build(){
	double minn[4],maxx[4];
	for(int i = 0;i < 3;i++){
		minn[i] = 100000.0;
		maxx[i] = -1000000.0;
	}

	for(auto& it : vec){
		AABB cur = it->getAABB();
		for(int i = 0;i < 3;i++){
			minn[i] = std::min(minn[i], cur.pos[i]);
			maxx[i] = std::max(maxx[i], cur.pos_2[i]);
		}
	}

	bound = new AABB(MyVec3(minn[0],minn[1],minn[2]), MyVec3(maxx[0] - minn[0], maxx[1] - minn[1], maxx[2] - minn[2]));

	if(vec.size() < KDTREE_MIN_NODE)
		return;

	//get split value
	splitPool.clear();
	for(auto& it : vec){

	}


}*/
