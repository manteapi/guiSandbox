#ifndef PARTITIONER_RENDERABLE_INL
#define PARTITIONER_RENDERABLE_INL

#include "partitionerRenderable.hpp"
#include <gl_helper.hpp>
#include <log.hpp>
#include <Io.hpp>
#include <Utils.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

template<typename PartitionData>
PartitionerRenderable<PartitionData>::~PartitionerRenderable()
{
    glcheck(glDeleteBuffers(1, &m_bbBuffer[0]));
    glcheck(glDeleteBuffers(1, &m_bbBuffer[1]));

    glcheck(glDeleteBuffers(1, &m_gridBuffer[0]));
    glcheck(glDeleteBuffers(1, &m_gridBuffer[1]));

    glcheck(glDeleteBuffers(1, &m_voxelTriBuffer[0]));
    glcheck(glDeleteBuffers(1, &m_voxelTriBuffer[1]));

    glcheck(glDeleteBuffers(1, &m_voxelPointsBuffer[0]));
    glcheck(glDeleteBuffers(1, &m_voxelPointsBuffer[1]));
}

template<typename PartitionData>
void PartitionerRenderable<PartitionData>::buildVoxelTriangles()
{
    for(const Voxel<PartitionData>& v : m_partitioner->m_image)
    {
        for(const FacetHandle & f : v.triangles)
        {
            Polyhedron::Halfedge_around_facet_const_circulator eBegin = f->facet_begin(), eit=eBegin;
            std::array<glm::vec3, 3> triangle;
            std::array<glm::vec3, 3> normal;
            int i = 0;
            do
            {
                triangle[i] = glm::vec3(eit->vertex()->point()[0],eit->vertex()->point()[1],eit->vertex()->point()[2]) ;
                normal[i] = glm::vec3(f->normal()[0],f->normal()[1],f->normal()[2]);
                ++i;
            }
            while(++eit != eBegin);
            for(size_t i=0; i<3; ++i)
            {
                m_voxelTriColors.push_back( glm::vec4(1.0,1.0,1.0,1.0) );
                m_voxelTriPositions.push_back( triangle[i] );
                m_voxelTriNormals.push_back( normal[i] );
            }
        }
    }

    //Create buffers
    glGenBuffers(1, &m_voxelTriBuffer[0]); //vertices
    glGenBuffers(1, &m_voxelTriBuffer[1]); //colors

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_voxelTriBuffer[0]));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_voxelTriPositions.size()*sizeof(glm::vec3), m_voxelTriPositions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_voxelTriBuffer[1]));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_voxelTriColors.size()*sizeof(glm::vec4), m_voxelTriColors.data(), GL_STATIC_DRAW));
}

template<typename PartitionData>
void PartitionerRenderable<PartitionData>::buildGrid()
{
    m_gridPositions.clear();
    m_gridColors.clear();
    glm::vec3 minBB = m_partitioner->m_gridInfo.offset;
    glm::vec3 maxBB = minBB+m_partitioner->m_gridInfo.scale;
    glm::ivec3 dimension = m_partitioner->m_gridInfo.dimension;

    int xSlice = dimension[0];
    int ySlice = dimension[1];
    int zSlice = dimension[2];
    for(size_t i=0; i<ySlice; ++i)
    {
        for(size_t j=0; j<zSlice; ++j)
        {
            glm::vec3 offset = m_partitioner->m_gridInfo.gridToWorld( m_partitioner->m_gridInfo.cellId(0,i,j) );
            m_gridPositions.push_back( glm::vec3(minBB[0], offset[1], offset[2]) );
            m_gridPositions.push_back( glm::vec3(maxBB[0], offset[1], offset[2]) );
            m_gridColors.push_back( glm::vec4(0.0,0.0,0.0,1.0) );
            m_gridColors.push_back( glm::vec4(0.0,0.0,0.0,1.0) );
        }
    }
    for(size_t i=0; i<xSlice; ++i)
    {
        for(size_t j=0; j<ySlice; ++j)
        {
            glm::vec3 offset = m_partitioner->m_gridInfo.gridToWorld( m_partitioner->m_gridInfo.cellId(i,j,0) );
            m_gridPositions.push_back( glm::vec3(offset[0], offset[1], minBB[2]) );
            m_gridPositions.push_back( glm::vec3(offset[0], offset[1], maxBB[2]) );
            m_gridColors.push_back( glm::vec4(0.0,0.0,0.0,1.0) );
            m_gridColors.push_back( glm::vec4(0.0,0.0,0.0,1.0) );
        }
    }
    for(size_t i=0; i<xSlice; ++i)
    {
        for(size_t j=0; j<zSlice; ++j)
        {
            glm::vec3 offset = m_partitioner->m_gridInfo.gridToWorld( m_partitioner->m_gridInfo.cellId(i,0,j) );
            m_gridPositions.push_back( glm::vec3(offset[0], minBB[1], offset[2]) );
            m_gridPositions.push_back( glm::vec3(offset[0], maxBB[1], offset[2]) );
            m_gridColors.push_back( glm::vec4(0.0,0.0,0.0,1.0) );
            m_gridColors.push_back( glm::vec4(0.0,0.0,0.0,1.0) );
        }
    }

    //Create buffers
    glGenBuffers(1, &m_gridBuffer[0]); //vertices
    glGenBuffers(1, &m_gridBuffer[1]); //colors

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_gridBuffer[0]));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_gridPositions.size()*sizeof(glm::vec3), m_gridPositions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_gridBuffer[1]));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_gridColors.size()*sizeof(glm::vec4), m_gridColors.data(), GL_STATIC_DRAW));
}

template<typename PartitionData>
void PartitionerRenderable<PartitionData>::buildBoundingBox()
{
    m_bbPositions.clear();
    m_bbColors.clear();
    glm::vec3 minBB = m_partitioner->m_gridInfo.offset;
    glm::vec3 scale = m_partitioner->m_gridInfo.scale;
    glm::vec3 maxBB = minBB+m_partitioner->m_gridInfo.scale;

    int xSlice = 2;
    int ySlice = 2;
    int zSlice = 2;
    for(size_t i=0; i<ySlice; ++i)
    {
        for(size_t j=0; j<zSlice; ++j)
        {
            float yOffset = (i/float(ySlice-1))*scale[1];
            float zOffset = (j/float(zSlice-1))*scale[2];
            m_bbPositions.push_back( glm::vec3(minBB[0], minBB[1]+yOffset, minBB[2]+zOffset) );
            m_bbPositions.push_back( glm::vec3(maxBB[0], minBB[1]+yOffset, minBB[2]+zOffset) );
            m_bbColors.push_back( glm::vec4(0.0,0.0,0.0,1.0) );
            m_bbColors.push_back( glm::vec4(0.0,0.0,0.0,1.0) );
        }
    }
    for(size_t i=0; i<xSlice; ++i)
    {
        for(size_t j=0; j<ySlice; ++j)
        {
            float xOffset = (i/float(ySlice-1))*scale[0];
            float yOffset = (j/float(ySlice-1))*scale[1];
            m_bbPositions.push_back( glm::vec3(minBB[0]+xOffset, minBB[1]+yOffset, minBB[2]) );
            m_bbPositions.push_back( glm::vec3(minBB[0]+xOffset, minBB[1]+yOffset, maxBB[2]) );
            m_bbColors.push_back( glm::vec4(0.0,0.0,0.0,1.0) );
            m_bbColors.push_back( glm::vec4(0.0,0.0,0.0,1.0) );
        }
    }
    for(size_t i=0; i<xSlice; ++i)
    {
        for(size_t j=0; j<zSlice; ++j)
        {
            float xOffset = (i/float(ySlice-1))*scale[0];
            float zOffset = (j/float(zSlice-1))*scale[2];
            m_bbPositions.push_back( glm::vec3(minBB[0]+xOffset, minBB[1], minBB[2]+zOffset) );
            m_bbPositions.push_back( glm::vec3(minBB[0]+xOffset, maxBB[1], minBB[2]+zOffset) );
            m_bbColors.push_back( glm::vec4(0.0,0.0,0.0,1.0) );
            m_bbColors.push_back( glm::vec4(0.0,0.0,0.0,1.0) );
        }
    }

    //Create buffers
    glGenBuffers(1, &m_bbBuffer[0]); //vertices
    glGenBuffers(1, &m_bbBuffer[1]); //colors

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_bbBuffer[0]));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_bbPositions.size()*sizeof(glm::vec3), m_bbPositions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_bbBuffer[1]));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_bbColors.size()*sizeof(glm::vec4), m_bbColors.data(), GL_STATIC_DRAW));
}

template<typename PartitionData>
void PartitionerRenderable<PartitionData>::buildVoxelPoints()
{
    m_voxelPointsPositions.clear();
    m_voxelPointsColors.clear();

    glm::vec3 offset(0.5f*m_partitioner->m_gridInfo.spacing());
    for(const Voxel<PartitionData>& voxel : m_partitioner->m_image)
    {
        if(voxel.m_type==VoxelType::Out)
        {
            m_voxelPointsPositions.push_back( voxel.offset + offset );
            m_voxelPointsColors.push_back( glm::vec4(0.0,0.0,1.0,1.0) );
        }
        else if(voxel.m_type==VoxelType::In)
        {
            m_voxelPointsPositions.push_back( voxel.offset + offset );
            m_voxelPointsColors.push_back( glm::vec4(1.0,0.0,0.0,1.0) );
        }
        else if(voxel.m_type==VoxelType::Border)
        {
            m_voxelPointsPositions.push_back( voxel.offset + offset );
            m_voxelPointsColors.push_back( glm::vec4(0.0,1.0,0.0,1.0) );
        }
        else
        {
            m_voxelPointsPositions.push_back( voxel.offset + offset );
            m_voxelPointsColors.push_back( glm::vec4(0.0,0.0,0.0,1.0) );
        }
    }
    std::cout << "Size: " << m_voxelPointsPositions.size() << " / " << m_voxelPointsColors.size() << std::endl;

    //Create buffers
    glGenBuffers(1, &m_voxelPointsBuffer[0]); //vertices
    glGenBuffers(1, &m_voxelPointsBuffer[1]); //colors

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_voxelPointsBuffer[0]));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_voxelPointsPositions.size()*sizeof(glm::vec3), m_voxelPointsPositions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_voxelPointsBuffer[1]));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_voxelPointsColors.size()*sizeof(glm::vec4), m_voxelPointsColors.data(), GL_STATIC_DRAW));
}

template<typename PartitionData>
PartitionerRenderable<PartitionData>::PartitionerRenderable(ShaderProgramPtr shaderProgram, std::shared_ptr<Partitioner<PartitionData> > &partitioner) :
    HierarchicalRenderable(shaderProgram), m_partitioner(partitioner)
{

    m_bbBuffer = {{0,0}};
    m_gridBuffer = {{0,0}};
    m_voxelTriBuffer = {{0,0}};
    m_voxelPointsBuffer = {{0,0}};

    buildBoundingBox();
    buildGrid();
    buildVoxelTriangles();
    buildVoxelPoints();
}

template<typename PartitionData>
void PartitionerRenderable<PartitionData>::voxelTriangles(std::vector<glm::vec3>& positions, std::vector<unsigned int>& indices, std::vector<glm::vec3> &normals)
{
    positions = m_voxelTriPositions;
    normals = m_voxelTriNormals;
    indices.resize(positions.size());
    for(size_t i=0; i<indices.size(); ++i) indices[i] = i;
}

template<typename PartitionData>
void PartitionerRenderable<PartitionData>::drawVoxelTriangles()
{
    //Location
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");

    //Send data to GPU
    if(modelLocation != ShaderProgram::null_location)
    {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if(positionLocation != ShaderProgram::null_location)
    {
        //Activate location
        glcheck(glEnableVertexAttribArray(positionLocation));
        //Bind buffer
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_voxelTriBuffer[0]));
        //Specify internal format
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_voxelTriBuffer[1]));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if( nitLocation != ShaderProgram::null_location )
      {
        glcheck(glUniformMatrix3fv( nitLocation, 1, GL_FALSE,
          glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
      }

    //Draw lines elements
    glcheck(glLineWidth(2.0));
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_voxelTriPositions.size()));
    glcheck(glLineWidth(1.0));

    if(positionLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
}

template<typename PartitionData>
void PartitionerRenderable<PartitionData>::drawBoundingBox()
{
    //Location
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");

    //Send data to GPU
    if(modelLocation != ShaderProgram::null_location)
    {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if(positionLocation != ShaderProgram::null_location)
    {
        //Activate location
        glcheck(glEnableVertexAttribArray(positionLocation));
        //Bind buffer
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_bbBuffer[0]));
        //Specify internal format
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_bbBuffer[1]));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if( nitLocation != ShaderProgram::null_location )
      {
        glcheck(glUniformMatrix3fv( nitLocation, 1, GL_FALSE,
          glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
      }

    //Draw lines elements
    glcheck(glLineWidth(2.0));
    glcheck(glDrawArrays(GL_LINES,0, m_bbPositions.size()));
    glcheck(glLineWidth(1.0));

    if(positionLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
}

template<typename PartitionData>
void PartitionerRenderable<PartitionData>::drawGrid()
{
    //Location
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");

    //Send data to GPU
    if(modelLocation != ShaderProgram::null_location)
    {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if(positionLocation != ShaderProgram::null_location)
    {
        //Activate location
        glcheck(glEnableVertexAttribArray(positionLocation));
        //Bind buffer
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_gridBuffer[0]));
        //Specify internal format
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_gridBuffer[1]));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if( nitLocation != ShaderProgram::null_location )
      {
        glcheck(glUniformMatrix3fv( nitLocation, 1, GL_FALSE,
          glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
      }

    //Draw lines elements
    glcheck(glDrawArrays(GL_LINES,0, m_gridPositions.size()));

    if(positionLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
}

template<typename PartitionData>
void PartitionerRenderable<PartitionData>::drawVoxelPoints()
{
    //Location
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");

    //Send data to GPU
    if(modelLocation != ShaderProgram::null_location)
    {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if(positionLocation != ShaderProgram::null_location)
    {
        //Activate location
        glcheck(glEnableVertexAttribArray(positionLocation));
        //Bind buffer
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_voxelPointsBuffer[0]));
        //Specify internal format
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_voxelPointsBuffer[1]));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if( nitLocation != ShaderProgram::null_location )
      {
        glcheck(glUniformMatrix3fv( nitLocation, 1, GL_FALSE,
          glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
      }

    //Draw points elements
    glcheck(glEnable(GL_POINT_SPRITE));
    glcheck(glEnable(GL_VERTEX_PROGRAM_POINT_SIZE));
    glcheck(glPointSize(5.0));
    glcheck(glDrawArrays(GL_POINTS,0, m_voxelPointsPositions.size()));
    glcheck(glPointSize(1.0));
    glcheck(glDisable(GL_POINT_SPRITE));
    glcheck(glDisable(GL_VERTEX_PROGRAM_POINT_SIZE));

    if(positionLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
}

template<typename PartitionData>
void PartitionerRenderable<PartitionData>::do_draw()
{
    drawBoundingBox();
    drawGrid();
    drawVoxelPoints();
    //drawVoxelTriangles();
}

template<typename PartitionData>
void PartitionerRenderable<PartitionData>::do_animate(float time) {}

#endif //PARTITIONER_RENDERABLE_INL
