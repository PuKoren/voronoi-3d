#include "Voronoi.h"

static btVector3 curVoronoiPoint;

struct pointCmp
{
    bool operator()(const btVector3& p1, const btVector3& p2) const
    {
        float v1 = (p1-curVoronoiPoint).length2();
        float v2 = (p2-curVoronoiPoint).length2();
        bool result0 = v1 < v2;
        //bool result1 = ((btScalar)(p1-curVoronoiPoint).length2()) < ((btScalar)(p2-curVoronoiPoint).length2());
        //apparently result0 is not always result1, because extended precision used in registered is different from precision when values are stored in memory
        return result0;
    }
}pointCompare;

Voronoi::Voronoi(){

}

Voronoi::~Voronoi(){

}

void Voronoi::getVerticesInsidePlanes(const vector<btVector3>& planes, vector<btVector3>& verticesOut, std::set<int>& planeIndicesOut)
{
    // Based on btGeometryUtil.cpp (Gino van den Bergen / Erwin Coumans)
    verticesOut.resize(0);
    planeIndicesOut.clear();
    const int numPlanes = planes.size();
    int i, j, k, l;
    for (i=0;i<numPlanes;i++)
    {
        const btVector3& N1 = planes[i];
        for (j=i+1;j<numPlanes;j++)
        {
            const btVector3& N2 = planes[j];
            btVector3 n1n2 = N1.cross(N2);
            if (n1n2.length2() > btScalar(0.0001))
            {
                for (k=j+1;k<numPlanes;k++)
                {
                    const btVector3& N3 = planes[k];
                    btVector3 n2n3 = N2.cross(N3);
                    btVector3 n3n1 = N3.cross(N1);
                    if ((n2n3.length2() > btScalar(0.0001)) && (n3n1.length2() > btScalar(0.0001) ))
                    {
                        btScalar quotient = (N1.dot(n2n3));
                        if (btFabs(quotient) > btScalar(0.0001))
                        {
                            btVector3 potentialVertex = (n2n3 * N1[3] + n3n1 * N2[3] + n1n2 * N3[3]) * (btScalar(-1.) / quotient);
                            for (l=0; l<numPlanes; l++)
                            {
                                const btVector3& NP = planes[l];
                                if (btScalar(NP.dot(potentialVertex))+btScalar(NP[3]) > btScalar(0.000001))
                                    break;
                            }
                            if (l == numPlanes)
                            {
                                // vertex (three plane intersection) inside all planes
                                verticesOut.push_back(potentialVertex);
                                planeIndicesOut.insert(i);
                                planeIndicesOut.insert(j);
                                planeIndicesOut.insert(k);
                            }
                        }
                    }
                }
            }
        }
    }
}

void Voronoi::voronoiBBShatter(const vector<btVector3>& points, const btVector3& bbmin, const btVector3& bbmax, const btQuaternion& bbq, const btVector3& bbt) {
    // points define voronoi cells in world space (avoid duplicates)
    // bbmin & bbmax = bounding box min and max in local space
    // bbq & bbt = bounding box quaternion rotation and translation
    // matDensity = Material density for voronoi shard mass calculation
    btVector3 bbvx = quatRotate(bbq, btVector3(1.0, 0.0, 0.0));
    btVector3 bbvy = quatRotate(bbq, btVector3(0.0, 1.0, 0.0));
    btVector3 bbvz = quatRotate(bbq, btVector3(0.0, 0.0, 1.0));
    btQuaternion bbiq = bbq.inverse();
    btConvexHullComputer* convexHC = new btConvexHullComputer();
    vector<btVector3> vertices;
    btVector3 rbb, nrbb;
    btScalar nlength, maxDistance, distance;
    vector<btVector3> sortedVoronoiPoints;
    for(unsigned int i = 0; i < points.size(); i++)
        sortedVoronoiPoints.push_back(points[i]);

    btVector3 normal, plane;
    vector<btVector3> planes;
    std::set<int> planeIndices;
    std::set<int>::iterator planeIndicesIter;
    int numplaneIndices;
    int cellnum = 0;
    int i, j, k;

    int numpoints = points.size();
    for (i=0; i < numpoints ;i++) {
        curVoronoiPoint = points[i];
        btVector3 icp = quatRotate(bbiq, curVoronoiPoint - bbt);
        rbb = icp - bbmax;
        nrbb = bbmin - icp;
        planes.resize(6);
        planes[0] = bbvx; planes[0][3] = rbb.x();
        planes[1] = bbvy; planes[1][3] = rbb.y();
        planes[2] = bbvz; planes[2][3] = rbb.z();
        planes[3] = -bbvx; planes[3][3] = nrbb.x();
        planes[4] = -bbvy; planes[4][3] = nrbb.y();
        planes[5] = -bbvz; planes[5][3] = nrbb.z();
        maxDistance = SIMD_INFINITY;
        sort(sortedVoronoiPoints.begin(), sortedVoronoiPoints.end(), pointCompare);
        //sortedVoronoiPoints(pointCmp());
        for (j=1; j < numpoints; j++) {
            normal = sortedVoronoiPoints[j] - curVoronoiPoint;
            nlength = normal.length();
            if (nlength > maxDistance)
                break;
            plane = normal.normalized();
            plane[3] = -nlength / btScalar(2.);
            planes.push_back(plane);
            getVerticesInsidePlanes(planes, vertices, planeIndices);
            if (vertices.size() == 0)
                break;
            numplaneIndices = planeIndices.size();
            if (numplaneIndices != planes.size()) {
                planeIndicesIter = planeIndices.begin();
                for (k=0; k < numplaneIndices; k++) {
                    if (k != *planeIndicesIter)
                        planes[k] = planes[*planeIndicesIter];
                    planeIndicesIter++;
                }
                planes.resize(numplaneIndices);
            }
            maxDistance = vertices[0].length();
            for (k=1; k < vertices.size(); k++) {
                distance = vertices[k].length();
                if (maxDistance < distance)
                    maxDistance = distance;
            }
            maxDistance *= btScalar(2.);
        }
        if (vertices.size() == 0)
            continue;

        // Clean-up voronoi convex shard vertices and generate edges & faces
        convexHC->compute(&vertices[0].getX(), sizeof(btVector3), vertices.size(), CONVEX_MARGIN,0.0);

        // At this point we have a complete 3D voronoi shard mesh contained in convexHC

        // Calculate volume and center of mass (Stan Melax volume integration)
        int numFaces = convexHC->faces.size();
        int v0, v1, v2; // Triangle vertices
        btScalar volume = btScalar(0.);
        btVector3 com(0., 0., 0.);
        for (j=0; j < numFaces; j++) {
            const btConvexHullComputer::Edge* edge = &convexHC->edges[convexHC->faces[j]];
            v0 = edge->getSourceVertex();
            v1 = edge->getTargetVertex();
            edge = edge->getNextEdgeOfFace();
            v2 = edge->getTargetVertex();
            while (v2 != v0) {
                // Counter-clockwise triangulated voronoi shard mesh faces (v0-v1-v2) and edges here...
                btScalar vol = convexHC->vertices[v0].triple(convexHC->vertices[v1], convexHC->vertices[v2]);
                volume += vol;
                com += vol * (convexHC->vertices[v0] + convexHC->vertices[v1] + convexHC->vertices[v2]);
                edge = edge->getNextEdgeOfFace();
                v1 = v2;
                v2 = edge->getTargetVertex();
            }
        }
        com /= volume * btScalar(4.);
        volume /= btScalar(6.);

        // Shift all vertices relative to center of mass
        int numVerts = convexHC->vertices.size();
        for (j=0; j < numVerts; j++)
        {
            convexHC->vertices[j] -= com;
        }

        cellnum ++;

    }
}

void Voronoi::voronoiConvexHullShatter(const vector<btVector3>& points, const vector<btVector3>& verts, const btQuaternion& bbq, const btVector3& bbt) {
    // points define voronoi cells in world space (avoid duplicates)
    // verts = source (convex hull) mesh vertices in local space
    // bbq & bbt = source (convex hull) mesh quaternion rotation and translation
    // matDensity = Material density for voronoi shard mass calculation
    btConvexHullComputer* convexHC = new btConvexHullComputer();
    vector<btVector3> vertices, chverts;
    btVector3 rbb, nrbb;
    btScalar nlength, maxDistance, distance;
    vector<btVector3> sortedVoronoiPoints;
    for(unsigned int i = 0; i < points.size(); i++)
        sortedVoronoiPoints.push_back(points[i]);
    btVector3 normal, plane;
    vector<btVector3> planes, convexPlanes;
    std::set<int> planeIndices;
    std::set<int>::iterator planeIndicesIter;
    int numplaneIndices;
    int cellnum = 0;
    int i, j, k;

    // Convert verts to world space and get convexPlanes
    int numverts = verts.size();
    chverts.resize(verts.size());
    for (i=0; i < numverts ;i++) {
        chverts[i] = quatRotate(bbq, verts[i]) + bbt;
    }
    //btGeometryUtil::getPlaneEquationsFromVertices(chverts, convexPlanes);
    // Using convexHullComputer faster than getPlaneEquationsFromVertices for large meshes...
    convexHC->compute(&chverts[0].getX(), sizeof(btVector3), numverts, 0.0, 0.0);
    int numFaces = convexHC->faces.size();
    int v0, v1, v2; // vertices
    for (i=0; i < numFaces; i++) {
        const btConvexHullComputer::Edge* edge = &convexHC->edges[convexHC->faces[i]];
        v0 = edge->getSourceVertex();
        v1 = edge->getTargetVertex();
        edge = edge->getNextEdgeOfFace();
        v2 = edge->getTargetVertex();
        plane = (convexHC->vertices[v1]-convexHC->vertices[v0]).cross(convexHC->vertices[v2]-convexHC->vertices[v0]).normalize();
        plane[3] = -plane.dot(convexHC->vertices[v0]);
        convexPlanes.push_back(plane);
    }
    const int numconvexPlanes = convexPlanes.size();

    int numpoints = points.size();
    for (i=0; i < numpoints ;i++) {
        curVoronoiPoint = points[i];
        planes.clear();
        for(unsigned int i = 0; i < convexPlanes.size(); i++)
            planes.push_back(convexPlanes[i]);

        for (j=0; j < numconvexPlanes ;j++) {
            planes[j][3] += planes[j].dot(curVoronoiPoint);
        }
        maxDistance = SIMD_INFINITY;
        sort(sortedVoronoiPoints.begin(), sortedVoronoiPoints.end(), pointCompare);
        //sortedVoronoiPoints.heapSort(pointCmp());
        for (j=1; j < numpoints; j++) {
            normal = sortedVoronoiPoints[j] - curVoronoiPoint;
            nlength = normal.length();
            if (nlength > maxDistance)
                break;
            plane = normal.normalized();
            plane[3] = -nlength / btScalar(2.);
            planes.push_back(plane);
            this->getVerticesInsidePlanes(planes, vertices, planeIndices);
            if (vertices.size() == 0)
                break;
            numplaneIndices = planeIndices.size();
            if (numplaneIndices != planes.size()) {
                planeIndicesIter = planeIndices.begin();
                for (k=0; k < numplaneIndices; k++) {
                    if (k != *planeIndicesIter)
                        planes[k] = planes[*planeIndicesIter];
                    planeIndicesIter++;
                }
                planes.resize(numplaneIndices);
            }
            maxDistance = vertices[0].length();
            for (k=1; k < vertices.size(); k++) {
                distance = vertices[k].length();
                if (maxDistance < distance)
                    maxDistance = distance;
            }
            maxDistance *= btScalar(2.);
        }
        if (vertices.size() == 0)
            continue;

        // Clean-up voronoi convex shard vertices and generate edges & faces
        convexHC->compute(&vertices[0].getX(), sizeof(btVector3), vertices.size(),0.0,0.0);

        // At this point we have a complete 3D voronoi shard mesh contained in convexHC

        // Calculate volume and center of mass (Stan Melax volume integration)
        numFaces = convexHC->faces.size();
        btScalar volume = btScalar(0.);
        btVector3 com(0., 0., 0.);
        for (j=0; j < numFaces; j++) {
            const btConvexHullComputer::Edge* edge = &convexHC->edges[convexHC->faces[j]];
            v0 = edge->getSourceVertex();
            v1 = edge->getTargetVertex();
            edge = edge->getNextEdgeOfFace();
            v2 = edge->getTargetVertex();
            while (v2 != v0) {
                // Counter-clockwise triangulated voronoi shard mesh faces (v0-v1-v2) and edges here...
                btScalar vol = convexHC->vertices[v0].triple(convexHC->vertices[v1], convexHC->vertices[v2]);
                volume += vol;
                com += vol * (convexHC->vertices[v0] + convexHC->vertices[v1] + convexHC->vertices[v2]);
                edge = edge->getNextEdgeOfFace();
                v1 = v2;
                v2 = edge->getTargetVertex();
            }
        }
        com /= volume * btScalar(4.);
        volume /= btScalar(6.);

        // Shift all vertices relative to center of mass
        int numVerts = convexHC->vertices.size();
        for (j=0; j < numVerts; j++)
        {
            convexHC->vertices[j] -= com;
        }

        cellnum ++;

    }
}
