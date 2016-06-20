#ifndef GRID_UTILITY_CPP
#define GRID_UTILITY_CPP

#include "gridUtility.hpp"
#include <iostream>

GridUtility::GridUtility()
{
    offset = Vec3r(0,0,0);
    scale = Vec3r(0,0,0);
    dimension = Vec3i(0,0,0);
    h = 0.0;
}

int GridUtility::width() const { return dimension[0]; }
int GridUtility::height() const { return dimension[1]; }
int GridUtility::depth() const { return dimension[2]; }
int GridUtility::size() const { return dimension[0]*dimension[1]*dimension[2]; }
HReal GridUtility::spacing() const { return h; }

GridUtility::GridUtility(const Vec3r& _offset, const Vec3r& _scale, const HReal& _spacing)
{
    offset = _offset;
    scale = _scale;
    h = _spacing;
    for(int i=0; i<3; ++i)
    {
        dimension[i] = std::floor(scale[i]/h);
    }
}

GridUtility::GridUtility(const Vec3r& _offset, const Vec3i& _dimension, const HReal& _spacing)
{
    offset = _offset;
    dimension = _dimension;
    h = _spacing;
    for(int i=0; i<3; ++i)
    {
        scale[i] = dimension[i]*h;
    }
}

void GridUtility::update(const Vec3r& _offset, const Vec3i& _dimension, const HReal& _spacing)
{
    h = _spacing;
    for(int i=0; i<3; ++i)
    {
        if(_offset[i]<offset[i])
            offset[i] = _offset[i];
        if(_dimension[i]>dimension[i])
        {
            dimension[i] = _dimension[i];
            scale[i] = dimension[i]*h;
        }
    }
}

void GridUtility::update(const Vec3r& _offset, const Vec3r& _scale, const HReal& _spacing)
{
    h = _spacing;
    for(int i=0; i<3; ++i)
    {
        if(_offset[i]<offset[i])
            offset[i] = _offset[i];
        if(_scale[i]>scale[i])
        {
            scale[i] = _scale[i];
            dimension[i] = std::floor(scale[i]/h);
        }
    }
}

void GridUtility::init(const Vec3r& _offset, const Vec3r &_scale, const HReal& _spacing)
{
    offset = _offset;
    scale = _scale;
    h = _spacing;
    for(int i=0; i<3; ++i)
    {
        dimension[i] = std::floor(scale[i]/h);
    }
}

void GridUtility::init(const Vec3r& _offset, const Vec3i& _dimension, const HReal& _spacing)
{
    offset = _offset;
    dimension = _dimension;
    h = _spacing;
    for(int i=0; i<3; ++i)
    {
        scale[i] = dimension[i]*h;
    }
}


GridUtility::~GridUtility(){}

bool GridUtility::isInside(int id) const
{
    if(id>=0 && id<size())
        return true;
    else
        return false;
}


bool GridUtility::isInside(const Vec3r& v) const
{
    Vec3i gridCoord = worldToGrid(v);
    return isInside(gridCoord);
}

bool GridUtility::isInside(int i, int j, int k) const
{
    if( (i>=0 && i<dimension[0]) && (j>=0 && j<dimension[1]) && (k>=0 && k<dimension[2]) )
        return true;
    else
        return false;
}

bool GridUtility::isInside(const Vec3i& v) const
{
    if( (v[0]>=0 && v[0]<dimension[0]) && (v[1]>=0 && v[1]<dimension[1]) && (v[2]>=0 && v[2]<dimension[2]) )
        return true;
    else
        return false;
}

Vec3r GridUtility::gridToWorld(const int i) const
{
    Vec3i gCoord = gridCoord(i);
    Vec3r wCoord = gridToWorld(gCoord);
    return wCoord;
}

Vec3r GridUtility::gridToWorld(const Vec3i& v) const
{
    Vec3r result;
    for(int i=0; i<3; ++i)
        result[i] = offset[i] + h*v[i];
    return result;
}
Vec3i GridUtility::worldToGrid(const Vec3r& v) const
{
    Vec3i result;
    for(int i=0; i<3; ++i)
        result[i] = std::floor((v[i]-offset[i])/h);
    return result;
}
int GridUtility::cellId(const Vec3r& v) const
{
    Vec3i gridCoord = worldToGrid(v);
    return cellId(gridCoord);
}

int GridUtility::cellId(const Vec3i& v) const
{
    return v[0] + v[1]*dimension[0] + v[2]*dimension[0]*dimension[1];
}

int GridUtility::cellId(int i, int j, int k) const
{
    return i + j*dimension[0] + k*dimension[0]*dimension[1];
}

std::array<Vec3r,8> GridUtility::cellVertices(const int cellId) const
{
    std::array<Vec3r,8> result;
    Vec3r offset = gridToWorld(cellId);
    result[0] = offset + Vec3r(0.0,0.0,0.0);
    result[1] = offset + Vec3r(h, 0.0, 0.0);
    result[2] = offset + Vec3r(h, h, 0.0);
    result[3] = offset + Vec3r(0.0, h, 0.0);
    result[4] = offset + Vec3r(0.0,0.0,h);
    result[5] = offset + Vec3r(h, 0.0, h);
    result[6] = offset + Vec3r(h, h, h);
    result[7] = offset + Vec3r(0.0, h, h);
    return result;
}

std::array< std::array<Vec3r,4>, 6> GridUtility::cellFaces(const int cellId) const
{
    std::array< std::array<Vec3r,4>, 6> result;
    Vec3r offset = gridToWorld(cellId);

    result[0][0] = offset + Vec3r(0.0,0.0,0.0);
    result[0][1] = offset + Vec3r(h, 0.0, 0.0);
    result[0][2] = offset + Vec3r(h,h,0.0);
    result[0][3] = offset + Vec3r(0.0,h,0.0);

    result[1][0] = offset + Vec3r(0.0,0.0,h);
    result[1][1] = offset + Vec3r(h, 0.0, h);
    result[1][2] = offset + Vec3r(h, h, h);
    result[1][3] = offset + Vec3r(0.0, h, h);

    result[2][0] = offset + Vec3r(0.0,0.0,0.0);
    result[2][1] = offset + Vec3r(h, 0.0, 0.0);
    result[2][2] = offset + Vec3r(h, 0.0, h);
    result[2][3] = offset + Vec3r(0.0, 0.0, h);

    result[3][0] = offset + Vec3r(0.0,h,0.0);
    result[3][1] = offset + Vec3r(h, h, 0.0);
    result[3][2] = offset + Vec3r(h, h, h);
    result[3][3] = offset + Vec3r(0.0, h, h);

    result[4][0] = offset + Vec3r(0.0,0.0,0.0);
    result[4][1] = offset + Vec3r(0.0,0.0,h);
    result[4][2] = offset + Vec3r(0.0, h, h);
    result[4][3] = offset + Vec3r(0.0, h, 0.0);

    result[5][0] = offset + Vec3r(h,0.0,0.0);
    result[5][1] = offset + Vec3r(h,0.0,h);
    result[5][2] = offset + Vec3r(h, h, h);
    result[5][3] = offset + Vec3r(h, h, 0.0);

    return result;
}

void GridUtility::get27Neighbors(std::vector<int>& neighbors, const int i, const HReal radius)
{
    Vec3i gCoord= gridCoord(i);
    get27Neighbors(neighbors, gCoord, std::floor(radius/h) );
}

void GridUtility::get27Neighbors(std::vector<Vec3i>& neighbors,const int i, const HReal radius)
{
    Vec3i gCoord = gridCoord(i);
    get27Neighbors(neighbors, gCoord, std::floor(radius/h) );
}

void GridUtility::get27Neighbors(std::vector<int>& neighbors, const Vec3r& p, const HReal radius)
{
    Vec3i gridCoord = worldToGrid(p);
    get27Neighbors(neighbors, gridCoord, std::floor(radius/h) );
}

void GridUtility::get27Neighbors(std::vector<Vec3i>& neighbors,const Vec3r& p, const HReal radius)
{
    Vec3i gridCoord = worldToGrid(p);
    get27Neighbors(neighbors, gridCoord, std::floor(radius/h) );
}

void GridUtility::get27Neighbors(std::vector<Vec3i>& neighbors, const Vec3i& p, const int radius)
{
    neighbors.clear();
    if(!isInside(p))
        return;

    for(int k = std::max(p[2]-radius,0); k<= std::min(p[2]+radius,dimension[2]-1); ++k )
    {
        for(int j = std::max(p[1]-radius,0); j<= std::min(p[1]+radius,dimension[1]-1); ++j )
        {
            for(int i = std::max(p[0]-radius,0); i<= std::min(p[0]+radius,dimension[0]-1); ++i )
            {
                neighbors.push_back(Vec3i(i,j,k));
            }
        }
    }
}

void GridUtility::get27Neighbors(std::vector<int>& neighbors, const Vec3i& p, const int radius)
{
    neighbors.clear();
    if(!isInside(p))
        return;

    for(int k = std::max(p[2]-radius,0); k<= std::min(p[2]+radius,dimension[2]-1); ++k )
    {
        for(int j = std::max(p[1]-radius,0); j<= std::min(p[1]+radius,dimension[1]-1); ++j )
        {
            for(int i = std::max(p[0]-radius,0); i<= std::min(p[0]+radius,dimension[0]-1); ++i )
            {
                neighbors.push_back(cellId(i,j,k));
            }
        }
    }
}


void GridUtility::get7Neighbors(std::vector<int>& neighbors, const Vec3r& p) const
{
    Vec3i gridCoord = worldToGrid(p);
    get7Neighbors(neighbors, gridCoord);
}

void GridUtility::get7Neighbors(std::vector<Vec3i>& neighbors,const Vec3r& p) const
{
    Vec3i gridCoord = worldToGrid(p);
    get7Neighbors(neighbors, gridCoord);
}

void GridUtility::get7Neighbors(std::vector<Vec3i>& neighbors, const Vec3i& p) const
{
    neighbors.clear();
    if(!isInside(p))
        return;

    Vec3i neighbor;

    neighbor = p;
    if(isInside(neighbor))
        neighbors.push_back(neighbor);

    neighbor = p + Vec3i(1,0,0);
    if(isInside(neighbor))
        neighbors.push_back(neighbor);

    neighbor = p + Vec3i(-1,0,0);
    if(isInside(neighbor))
        neighbors.push_back(neighbor);

    neighbor = p + Vec3i(0,1,0);
    if(isInside(neighbor))
        neighbors.push_back(neighbor);

    neighbor = p + Vec3i(0,-1,0);
    if(isInside(neighbor))
        neighbors.push_back(neighbor);

    neighbor = p + Vec3i(0,0,1);
    if(isInside(neighbor))
        neighbors.push_back(neighbor);

    neighbor = p + Vec3i(0,0,-1);
    if(isInside(neighbor))
        neighbors.push_back(neighbor);
}

void GridUtility::get7Neighbors(std::vector<int>& neighbors, const Vec3i& p) const
{
    neighbors.clear();
    if(!isInside(p))
        return;

    Vec3i neighbor;

    neighbor = p;
    if(isInside(neighbor))
        neighbors.push_back(cellId(neighbor));

    neighbor = p + Vec3i(1,0,0);
    if(isInside(neighbor))
        neighbors.push_back(cellId(neighbor));

    neighbor = p + Vec3i(-1,0,0);
    if(isInside(neighbor))
        neighbors.push_back(cellId(neighbor));

    neighbor = p + Vec3i(0,1,0);
    if(isInside(neighbor))
        neighbors.push_back(cellId(neighbor));

    neighbor = p + Vec3i(0,-1,0);
    if(isInside(neighbor))
        neighbors.push_back(cellId(neighbor));

    neighbor = p + Vec3i(0,0,1);
    if(isInside(neighbor))
        neighbors.push_back(cellId(neighbor));

    neighbor = p + Vec3i(0,0,-1);
    if(isInside(neighbor))
        neighbors.push_back(cellId(neighbor));
}

Vec3i GridUtility::gridCoord(int i) const
{
    Vec3i coord;
    coord[2] = i/(dimension[0]*dimension[1]);
    coord[1] = (i - coord[2]*dimension[0]*dimension[1])/dimension[0];
    coord[0] = (i - coord[1]*dimension[0] - coord[2]*dimension[0]*dimension[1]);
    return coord;
}

void GridUtility::info()
{
    std::cout << "Dimension: " << width() << "x" << height() << "x" << depth() << " = " << size() << std::endl;
    std::cout << "Spacing : " << spacing() << std::endl;
    std::cout << "Offset : " << offset[0] << ", " << offset[1] << ", " << offset[2] << std::endl;
    std::cout << "Scale : " << scale[0] << ", " << scale[1] << ", " << scale[2] << std::endl;
    std::cout << "Max : " << offset[0]+scale[0] << ", " << offset[1]+scale[1] << ", " << offset[2] + scale[2] << std::endl;
}

#endif
