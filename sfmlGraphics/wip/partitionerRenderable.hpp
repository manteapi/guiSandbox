#ifndef PARTITIONER_RENDERABLE_HPP
#define PARTITIONER_RENDERABLE_HPP

#include <HierarchicalRenderable.hpp>

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "partitioner.hpp"

template<typename PartitionData>
class PartitionerRenderable : public HierarchicalRenderable
{
    public:
        ~PartitionerRenderable();
        PartitionerRenderable(ShaderProgramPtr program, std::shared_ptr< Partitioner<PartitionData> >& partitioner);
        void voxelTriangles(std::vector<glm::vec3>& positions, std::vector<unsigned int>& indices, std::vector<glm::vec3>& normals);

private:
        void buildBoundingBox();
        void drawBoundingBox();

        void buildVoxelTriangles();
        void drawVoxelTriangles();

        void buildGrid();
        void drawGrid();

        void buildVoxelPoints();
        void drawVoxelPoints();

        void do_draw();
        void do_animate( float time );


        std::shared_ptr< Partitioner<PartitionData> > m_partitioner;

        //Data for the bounding box of the partitioner
        std::vector< glm::vec3 > m_bbPositions;
        std::vector< glm::vec4 > m_bbColors;
        std::array<unsigned int,2> m_bbBuffer;

        //Data for the raw grid of the partitioner
        std::vector< glm::vec3 > m_gridPositions;
        std::vector< glm::vec4 > m_gridColors;
        std::array<unsigned int,2> m_gridBuffer;

        //Data for the raw grid of the partitioner
        std::vector< glm::vec3 > m_voxelTriPositions;
        std::vector< glm::vec3 > m_voxelTriNormals;
        std::vector< glm::vec4 > m_voxelTriColors;
        std::array<unsigned int,2> m_voxelTriBuffer;

        //Data for the voxel type of the partitioner
        std::vector< glm::vec3 > m_voxelPointsPositions;
        std::vector< glm::vec4 > m_voxelPointsColors;
        std::array<unsigned int,2> m_voxelPointsBuffer;
};

#endif
