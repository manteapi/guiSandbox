#ifndef PARTITIONER_CPP
#define PARTITIONER_CPP

#include "partitioner.inl"
#include <array>

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
