#ifndef GRID_UTILITY_HPP
#define GRID_UTILITY_HPP

#include <vector>
#include <array>
#include <glm/glm.hpp>
typedef glm::vec3 Vec3r;
typedef glm::ivec3 Vec3i;
typedef float HReal;

class GridUtility
{
    public :
        GridUtility();
        GridUtility(const Vec3r& _offset, const Vec3r& _scale, const HReal& _spacing);
        GridUtility(const Vec3r& _offset, const Vec3i& _dimension, const HReal& _spacing);
        ~GridUtility();

        Vec3r offset;
        Vec3r scale;
        HReal h;
        Vec3i dimension;

        void get27Neighbors(std::vector<int>& neighbors, const int i, const HReal radius);
        void get27Neighbors(std::vector<Vec3i>& neighbors,const int i, const HReal radius);

        void get27Neighbors(std::vector<int>& neighbors, const Vec3r& p, const HReal radius);
        void get27Neighbors(std::vector<Vec3i>& neighbors,const Vec3r& p, const HReal radius);

        void get27Neighbors(std::vector<int>& neighbors, const Vec3i& p, const int radius);
        void get27Neighbors(std::vector<Vec3i>& neighbors, const Vec3i& p, const int radius);


        void get7Neighbors(std::vector<int>& neighbors, const Vec3r& p) const;
        void get7Neighbors(std::vector<Vec3i>& neighbors,const Vec3r& p) const;

        void get7Neighbors(std::vector<int>& neighbors, const Vec3i& p) const;
        void get7Neighbors(std::vector<Vec3i>& neighbors, const Vec3i& p) const;

        bool isInside(int id) const;
        bool isInside(int i, int j, int k) const;
        bool isInside(const Vec3i& v) const;
        bool isInside(const Vec3r& v) const;

        Vec3r gridToWorld(const Vec3i& v) const;
        Vec3r gridToWorld(const int i) const;

        Vec3i worldToGrid(const Vec3r& v) const;

        int cellId(const Vec3r& v) const;
        int cellId(const Vec3i& v) const;
        int cellId(int i, int j, int k) const;

        std::array<Vec3r,8> cellVertices(const int cellId) const;
        std::array< std::array<Vec3r,4>, 6> cellFaces(const int cellId) const;
        int width() const;
        int height() const;
        int depth() const;
        int size() const;
        HReal spacing() const;

        void update(const Vec3r& _offset, const Vec3i& _scale, const HReal& _spacing);
        void update(const Vec3r& _offset, const Vec3r& _scale, const HReal& _spacing);

        void init(const Vec3r& _offset, const Vec3r& _scale, const HReal& _spacing);
        void init(const Vec3r& _offset, const Vec3i& _scale, const HReal& _spacing);
        Vec3i gridCoord(int i) const;
        void info();
};

#endif
