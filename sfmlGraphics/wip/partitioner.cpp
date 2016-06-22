#ifndef PARTITIONER_CPP
#define PARTITIONER_CPP

#include "partitioner.inl"
#include <array>

Intersection::Intersection(){}
Intersection::Intersection(const FacetHandle& meshFace, const int gridFace) :
    m_meshFace(meshFace), m_gridFace(gridFace)
{
}
Intersection::~Intersection(){}

SegmentIntersection::SegmentIntersection(){}
SegmentIntersection::SegmentIntersection(const FacetHandle& meshFace, const int gridFace, const CGALSegment& segment) :
    Intersection(meshFace, gridFace), m_segment(segment)
{

}
SegmentIntersection::~SegmentIntersection(){}

PointIntersection::PointIntersection(){}
PointIntersection::PointIntersection(const FacetHandle& meshFace, const int gridFace, const CGALPoint& point) :
        Intersection(meshFace, gridFace), m_point(point)
{

}

PointIntersection::~PointIntersection(){}

TriangleIntersection::TriangleIntersection(){}
TriangleIntersection::TriangleIntersection(const FacetHandle& meshFace, const int gridFace, const CGALTriangle& triangle) :
    Intersection(meshFace, gridFace), m_triangle(triangle)
{

}
TriangleIntersection::~TriangleIntersection(){}


std::set<unsigned int> getCellsOverTriangle(const GridUtility& grid, const std::array<Vec3r,3>& t)
{
    std::set<unsigned int> result;

    //Step 1: Get bounding box
    Vec3r minBB(std::numeric_limits<float>::max()), maxBB(-std::numeric_limits<float>::max());
    for(size_t i=0; i<3; ++i) for(size_t j=0; j<3; ++j) {minBB[j] = std::min(minBB[j], t[i][j]); maxBB[j] = std::max(maxBB[j], t[i][j]);}
    Vec3i gMinBB = grid.worldToGrid(minBB), gMaxBB = grid.worldToGrid(maxBB);

    //Step 2: For each cell in the bounding box
    for(size_t i=gMinBB[0]; i<=gMaxBB[0]; ++i)
    {
        for(size_t j=gMinBB[1]; j<=gMaxBB[1]; ++j)
        {
            for(size_t k=gMinBB[2]; k<=gMaxBB[2]; ++k)
            {
                int cellId = grid.cellId(i,j,k);
                std::array<Vec3r,8> cellVertices = grid.cellVertices(cellId);
                //Check if one of the triangle's vertex is inside the cell
                bool isInside = true;
                for(size_t l=0; l<3; ++l)
                {
                    std::array<float,8> triWeights = trilinearWeights(cellVertices, t[l]);
                    for(const float& weight : triWeights) isInside = isInside && (weight>=0 && weight<=1.0);
                    if(isInside) { result.insert(cellId); break; }
                }

                //Check if the triangle intersects the cell
                std::array<std::array<Vec3r,4>,6> cubeFaces = grid.cellFaces(cellId);
                if(!isInside)
                {
                    for(size_t l=0; l<6; ++l)
                    {
                        bool hasIntersected = trianglePlaneIntersection(cubeFaces[l], t);
                        if(hasIntersected) { result.insert(cellId); break;}
                    }
                }

            }
        }
    }
    return result;
}

std::array<float,8> trilinearWeights(const std::array<Vec3r,8>& cellVertices, const Vec3r& x)
{
    std::array<float,8> weights;
    const Vec3r& minBB = cellVertices[0];
    const Vec3r& maxBB = cellVertices[6];
    Vec3r n; //Normalized
    n[0] = (x[0]-minBB[0])/(maxBB[0]-minBB[0]);
    n[1] = (x[1]-minBB[1])/(maxBB[1]-minBB[1]);
    n[2] = (x[2]-minBB[2])/(maxBB[2]-minBB[2]);

    weights[0] = (1-n[0])*(1-n[1])*(1-n[2]);
    weights[1] = n[0]*(1-n[1])*(1-n[2]);
    weights[2] = n[0]*n[1]*(1-n[2]);
    weights[3] = (1-n[0])*n[1]*(1-n[2]);

    weights[4] = (1-n[0])*(1-n[1])*n[2];
    weights[5] = n[0]*(1-n[1])*n[2];
    weights[6] = n[0]*n[1]*n[2];
    weights[7] = (1-n[0])*n[1]*n[2];

    return weights;
}

void floodfill(const FacetHandleList& triangles, std::vector<FacetHandleSet>& trianglesSets)
{
    std::set<FacetHandle> visited;
    for(const FacetHandle& f : triangles)
    {
        if(visited.find(f)==visited.end())
        {
            std::set<FacetHandle> component;
            std::vector<FacetHandle> tovisit;
            tovisit.push_back(f);
            while(!tovisit.empty())
            {
                const FacetHandle & candidate = tovisit.back();
                tovisit.pop_back();
                component.insert(candidate);
                visited.insert(candidate);
                Polyhedron::Halfedge_around_facet_circulator eBegin = candidate->facet_begin(), eit=eBegin;
                do
                {
                    if(!eit->opposite()->is_border())
                    {
                        FacetHandle n = eit->opposite()->facet();
                        bool isInsideCell = std::find(triangles.begin(), triangles.end(), n)!=triangles.end();
                        bool isVisited = visited.find(n) != visited.end();
                        if(isInsideCell && !isVisited)
                        {
                            tovisit.push_back(n);
                        }
                    }
                }
                while(++eit != eBegin);
            }
            trianglesSets.push_back(component);
        }
    }
}

std::array<Vec3r,3> getTriangleFromFace(const FacetHandle& f)
{
    int i = 0;
    std::array<Vec3r,3> triangle;
    Polyhedron::Halfedge_around_facet_const_circulator eBegin = f->facet_begin(), eit=eBegin;
    do{triangle[i] = Vec3r(eit->vertex()->point()[0], eit->vertex()->point()[1], eit->vertex()->point()[2]); ++i;} while(++eit != eBegin);
    return triangle;
}

bool triangleVoxelIntersection(const int voxelFaceId, const std::array<Vec3r,4>& voxelFaces,
                               const FacetHandle & meshFace, std::set<IntersectionPtr>& intersections)
{
    bool intersect = false;
    std::array<Vec3r,3> t = getTriangleFromFace(meshFace);
    CGALTriangle cgT( CGALPoint(t[0][0], t[0][1], t[0][2]), CGALPoint(t[1][0], t[1][1], t[1][2]), CGALPoint(t[2][0], t[2][1], t[2][2]) );
    CGALTriangle cgP1( CGALPoint(voxelFaces[0][0], voxelFaces[0][1], voxelFaces[0][2]), CGALPoint(voxelFaces[1][0], voxelFaces[1][1], voxelFaces[1][2]), CGALPoint(voxelFaces[2][0], voxelFaces[2][1], voxelFaces[2][2]) );
    CGALTriangle cgP2( CGALPoint(voxelFaces[0][0], voxelFaces[0][1], voxelFaces[0][2]), CGALPoint(voxelFaces[2][0], voxelFaces[2][1], voxelFaces[2][2]), CGALPoint(voxelFaces[3][0], voxelFaces[3][1], voxelFaces[3][2]) );

    auto result1 = CGAL::intersection(cgT, cgP1);
    CGALTriangle iTriangle1;
    CGALPoint iPoint1;
    CGALSegment iSegment1;
    if(CGAL::assign(iTriangle1, result1))
    {
        intersect = true;
        TriangleIntersectionPtr inter = std::make_shared<TriangleIntersection>();
        intersections.insert(inter);
    }
    else if(CGAL::assign(iPoint1, result1))
    {
        intersect = true;
        PointIntersectionPtr inter = std::make_shared<PointIntersection>();
        intersections.insert(inter);
    }
    else if(CGAL::assign(iSegment1, result1))
    {
        intersect = true;
        SegmentIntersectionPtr inter = std::make_shared<SegmentIntersection>();
        intersections.insert(inter);
    }

    auto result2 = CGAL::intersection(cgT, cgP2);
    CGALTriangle iTriangle2;
    CGALPoint iPoint2;
    CGALSegment iSegment2;
    if(CGAL::assign(iTriangle2, result2))
    {
        intersect = true;
        TriangleIntersectionPtr inter = std::make_shared<TriangleIntersection>();
        intersections.insert(inter);
    }
    else if(CGAL::assign(iPoint2, result2))
    {
        intersect = true;
        PointIntersectionPtr inter = std::make_shared<PointIntersection>();
        intersections.insert(inter);
    }
    else if(CGAL::assign(iSegment2, result2))
    {
        intersect = true;
        SegmentIntersectionPtr inter = std::make_shared<SegmentIntersection>();
        intersections.insert(inter);
    }
    return intersect;
}

bool trianglePlaneIntersection(std::array<Vec3r, 4>& plane, const std::array<Vec3r,3>& t)
{
    bool intersect = false;
    CGALTriangle cgT( CGALPoint(t[0][0], t[0][1], t[0][2]), CGALPoint(t[1][0], t[1][1], t[1][2]), CGALPoint(t[2][0], t[2][1], t[2][2]) );

    CGALTriangle cgP1( CGALPoint(plane[0][0], plane[0][1], plane[0][2]), CGALPoint(plane[1][0], plane[1][1], plane[1][2]), CGALPoint(plane[2][0], plane[2][1], plane[2][2]) );
    CGALTriangle cgP2( CGALPoint(plane[0][0], plane[0][1], plane[0][2]), CGALPoint(plane[2][0], plane[2][1], plane[2][2]), CGALPoint(plane[3][0], plane[3][1], plane[3][2]) );


    auto result1 = CGAL::intersection(cgT, cgP1);
    CGALTriangle iTriangle1;
    CGALPoint iPoint1;
    CGALSegment iSegment1;
    if(CGAL::assign(iTriangle1, result1) || CGAL::assign(iPoint1, result1) || CGAL::assign(iSegment1, result1))
    {
        intersect = true;
    }

    auto result2 = CGAL::intersection(cgT, cgP2);
    CGALTriangle iTriangle2;
    CGALPoint iPoint2;
    CGALSegment iSegment2;
    if(CGAL::assign(iTriangle2, result2) || CGAL::assign(iPoint2, result2) || CGAL::assign(iSegment2, result2))
    {
        intersect = true;
    }

    return intersect;
}

#endif //PARTITIONER_CPP
