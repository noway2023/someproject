#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<Vec3f> vts_;
	std::vector<Vec3f> vns_;
	std::vector<std::vector<int> > faces_;
	std::vector<std::vector<int> > uv_;
	std::vector<std::vector<int> > normal_;
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	Vec3f vt(int i);
	Vec3f vn(int i, int j);
	std::vector<int> face(int idx);
	std::vector<int> normals(int idx);
	std::vector<int> uvs(int idx);
};

#endif //__MODEL_H__
