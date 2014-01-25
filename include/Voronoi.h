#ifndef _VORONOI_H
#define _VORONOI_H
#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/LinearMath/btConvexHullComputer.h>
#include <vector>
#include <set>
#include <algorithm>

#define CONVEX_MARGIN 0.04

using namespace std;

class Voronoi{
public:
    Voronoi();
    ~Voronoi();
    void getVerticesInsidePlanes(const vector<btVector3>& planes, vector<btVector3>& verticesOut, std::set<int>& planeIndicesOut);
    void voronoiBBShatter(const vector<btVector3>& points, const btVector3& bbmin, const btVector3& bbmax, const btQuaternion& bbq, const btVector3& bbt);
    void voronoiConvexHullShatter(const vector<btVector3>& points, const vector<btVector3>& verts, const btQuaternion& bbq, const btVector3& bbt);
};

#endif
