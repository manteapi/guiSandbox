#ifndef PARTITIONER_INL
#define PARTITIONER_INL

#include "partitioner.hpp"

//--------------------------------------------------------------------------
//---------------------------------PIXEL------------------------------------
//--------------------------------------------------------------------------

template<class PartitionData>
Voxel<PartitionData>::~Voxel(){}

template<class PartitionData>
Voxel<PartitionData>::Voxel()
{
    length = 0.0;
    offset = Vec3r(0.0,0.0,0.0);
    m_type = VoxelType::None;
    triangles.clear();
    partitions.clear();
}

template<class PartitionData>
Voxel<PartitionData>::Voxel(const HReal length_, const Vec3r &offset_, VoxelType type_)
{
    length = length_;
    offset = offset_;
    m_type = type_;
    triangles.clear();
    partitions.clear();
}

template<class PartitionData>
Voxel<PartitionData>::Voxel(const Voxel& voxel)
{
    length = voxel.length;
    offset = voxel.offset;
    triangles = voxel.triangles;
    partitions = voxel.partitions;
}

template<class PartitionData>
Partition<PartitionData>& Voxel<PartitionData>::operator[](const int partitionId)
{
    return partitions[partitionId];
}

template<class PartitionData>
const Partition<PartitionData>& Voxel<PartitionData>::operator[](const int partitionId) const
{
    return partitions[partitionId];
}

template<class PartitionData>
void Voxel<PartitionData>::addTriangle(const FacetHandle& facetHandle)
{
    triangles.push_back(facetHandle);
}

template<class PartitionData>
void Voxel<PartitionData>::clearGeometry()
{
    triangles.clear();
}

template<class PartitionData>
size_t Voxel<PartitionData>::size() const
{
    return partitions.size();
}


//--------------------------------------------------------------------------
//---------------------------------PARTITION--------------------------------
//--------------------------------------------------------------------------

template<class PartitionData>
Partition<PartitionData>::~Partition(){}

template<class PartitionData>
Partition<PartitionData>::Partition(){}

template<class PartitionData>
Partition<PartitionData>::Partition(const int& voxelId, const FacetHandleSet& triangles, const std::set<Vec2i,Vec2iCompare>& neighbors, const PartitionData& data)
{
    m_voxelId = voxelId;
    m_triangles = triangles;
    for(const Vec2i & n : neighbors) m_neighbors.insert(n);
    m_data = data;
}

template<class PartitionData>
Partition<PartitionData>::Partition( const Partition& partition )
{
    m_triangles = partition.m_triangles;
    m_neighbors = partition.m_neighbors;
    m_data = partition.m_data;
}

template<class PartitionData>
std::set<Vec2i, Vec2iCompare> &Partition<PartitionData>::getNeighbors()
{
    return m_neighbors;
}

template<class PartitionData>
const std::set<Vec2i, Vec2iCompare>& Partition<PartitionData>::getNeighbors() const
{
    return m_neighbors;
}

template<class PartitionData>
void Partition<PartitionData>::clearGeometry()
{
    m_triangles.clear();
}

template<class PartitionData>
void Partition<PartitionData>::addTriangle(const FacetHandle& facetHandle)
{
    m_triangles.insert(facetHandle);
}

//--------------------------------------------------------------------------
//----------------------------------PARTITIONER----------------------------
//--------------------------------------------------------------------------

template<class PartitionData>
Partitioner<PartitionData>::Partitioner()
{}

template<class PartitionData>
Partitioner<PartitionData>::Partitioner(CustomPolyhedron &polyhedron, const HReal& voxelSpacing)
{
    //Step 1: Compute the grid embedding the mesh
    Vec3r minBB( std::numeric_limits<float>::max() ), maxBB( -std::numeric_limits<float>::max() ), scale;
    for(Polyhedron::Vertex_const_iterator it = polyhedron.vertices_begin(); it!=polyhedron.vertices_end(); ++it)
    {
        for(size_t i=0; i<3; ++i)
        {
            minBB[i] = std::min( minBB[i], (float)(it->point()[i]) );
            maxBB[i] = std::max( maxBB[i], (float)(it->point()[i]) );
        }
    }
    minBB = minBB - Vec3r(voxelSpacing);
    maxBB = maxBB + Vec3r(voxelSpacing);
    scale = maxBB-minBB;
    m_gridInfo = GridUtility(minBB, scale, voxelSpacing);

    //Step 3: Resize the image
    initImage();

    //Step 2: Put the triangles into the grids
    for(Polyhedron::Facet_iterator fit = polyhedron.facets_begin(); fit!=polyhedron.facets_end(); ++fit)
    {
        int i=0;
        std::array<Vec3r,3> triangle;
        Polyhedron::Halfedge_around_facet_const_circulator eBegin = fit->facet_begin(), eit=eBegin;
        do{triangle[i] = Vec3r(eit->vertex()->point()[0], eit->vertex()->point()[1], eit->vertex()->point()[2]); ++i;} while(++eit != eBegin);
        std::set<unsigned int> voxels = getCellsOverTriangle(m_gridInfo, triangle);
        for(const int& cellId : voxels) m_image[cellId].addTriangle(fit);
    }

    //Step 3: Mark the cells as IN/OUT/BORDER
    markVoxels();

    //Step 4: Build connectivity for IN cells with no BORDER cells as neighbors
    for(size_t i=0; i<m_image.size(); ++i)
    {
        Voxel<PartitionData> & voxel = m_image[i];
        if(voxel.m_type==VoxelType::In)
        {
            std::vector<int> neighbors;
            m_gridInfo.get27Neighbors(neighbors, i, m_gridInfo.spacing());
            bool hasBorderNeighbor = false;
            for(const int & n : neighbors) if(m_image[n].m_type==VoxelType::Border) hasBorderNeighbor = true;
            if(!hasBorderNeighbor)
            {
                std::set<Vec2i, Vec2iCompare> neighborhood;
                FacetHandleSet triangles;
                PartitionData data;
                for(const int & n : neighbors) neighborhood.insert( Vec2i(n,0) );
                Partition<PartitionData> partition(i, triangles, neighborhood, data);
                voxel.partitions.push_back(partition);
            }
        }
    }

    //Step 4: Basic flood fill on triangles in BORDER cells
    for(size_t i=0; i<m_image.size(); ++i)
    {
        Voxel<PartitionData> & voxel = m_image[i];
        if(voxel.m_type==VoxelType::Border)
        {
            std::vector< FacetHandleSet > triangles;
            floodfill(voxel.triangles, triangles);
            for(size_t j=0; j<triangles.size(); ++j)
            {
                std::set<Vec2i, Vec2iCompare> neighborhood;
                PartitionData data;
                Partition<PartitionData> partition(i, triangles[j], neighborhood, data);
                voxel.partitions.push_back(partition);
            }
        }
    }

    //Step 5: Compute intersections of triangles with cells
    for(size_t i=0; i<m_image.size(); ++i)
    {
        Voxel<PartitionData> & voxel = m_image[i];
        std::array<std::array<Vec3r,4>,6> voxelFaces = m_gridInfo.cellFaces(i);
        for(size_t j=0; j<voxelFaces.size(); ++j)
        {
            for(size_t k=0; k<voxel.size(); ++k)
            {
                Partition<PartitionData> & partition = voxel.partitions[k];
                FacetHandleSet & triangles = partition.m_triangles;
                for(const FacetHandle & f : triangles)
                {
                    std::set<IntersectionPtr> intersections;
                    triangleVoxelIntersection(j, voxelFaces[j], f, intersections);
                    for(const IntersectionPtr & inter : intersections)
                    {
                        partition.m_intersections.insert(inter);
                    }
                }
            }
        }
    }
}

template<class PartitionData>
void Partitioner<PartitionData>::markVoxels()
{
    //Mark border voxels
    for(Voxel<PartitionData>& voxel : m_image)
    {
        if(!voxel.triangles.empty()) voxel.m_type=VoxelType::Border;
    }

    //Mark out voxels
    std::vector<unsigned int> tovisit;
    std::set<unsigned int> visited;
    tovisit.push_back( m_gridInfo.cellId( m_gridInfo.offset ) );
    while(!tovisit.empty())
    {
        unsigned int cellId = tovisit.back();
        tovisit.pop_back();
        m_image[cellId].m_type = VoxelType::Out;
        visited.insert(cellId);
        std::vector<int> neighbors;
        m_gridInfo.get27Neighbors(neighbors, cellId, m_gridInfo.spacing());
        for(const int& n : neighbors)
        {
            bool isNotBorder = (m_image[n].m_type != VoxelType::Border);
            bool hasBeenVisited = (visited.find(n) != visited.end());
            bool willBeVisited = (std::find(tovisit.begin(), tovisit.end(), n)!=tovisit.end());
            if(!hasBeenVisited && !willBeVisited && isNotBorder)
                tovisit.push_back(n);
        }
    }

    //Mark in voxels
    for(Voxel<PartitionData>& voxel : m_image)
    {
        if(voxel.m_type==VoxelType::None && voxel.triangles.empty())
        {
            voxel.m_type=VoxelType::In;
        }
    }
}

template<class PartitionData>
Partitioner<PartitionData>::~Partitioner(){}

template<class PartitionData>
Partitioner<PartitionData>::Partitioner(const Partitioner& partitioner)
{
    m_gridInfo = partitioner.m_gridInfo;
    m_image = partitioner.m_image;
}

template<class PartitionData>
Partitioner<PartitionData>::Partitioner(const GridUtility &_gridInfo)
{
    m_gridInfo = _gridInfo;
}

template<class PartitionData>
void Partitioner<PartitionData>::initImage()
{
    m_image.resize(m_gridInfo.size());
    for(size_t i=0; i<m_image.size(); ++i)
    {
        Vec3r voxelOffset = m_gridInfo.gridToWorld(i);
        m_image[i] = Voxel<PartitionData>(m_gridInfo.spacing(), voxelOffset, VoxelType::None);
    }
}

template<class PartitionData>
template<class TData>
void Partitioner<PartitionData>::setConnectivity(const Partitioner<TData>& partitioner)
{
    m_gridInfo = partitioner.m_gridInfo;
    m_image.resize(partitioner.m_image.size());
 //#pragma omp parallel for
    for(size_t i=0; i<m_image.size(); ++i)
    {
        m_image[i].length = partitioner.m_image[i].length;
        m_image[i].offset = partitioner.m_image[i].offset;
        m_image[i].triangles = partitioner.m_image[i].triangles;
        m_image[i].partitions.resize(partitioner.m_image[i].partitions.size());
        for(size_t j=0; j<m_image[i].partitions.size(); ++j)
        {
            m_image[i].partitions[j].triangles = partitioner.m_image[i].partitions[j].triangles;
            m_image[i].partitions[j].neighbors = partitioner.m_image[i].partitions[j].neighbors;
        }
    }
}

template<class PartitionData>
Voxel<PartitionData>& Partitioner<PartitionData>::operator[](const int voxelId)
{
    return m_image[voxelId];
}

template<class PartitionData>
const Voxel<PartitionData>& Partitioner<PartitionData>::operator[](const int voxelId) const
{
    return m_image[voxelId];
}

template<class PartitionData>
Voxel<PartitionData>& Partitioner<PartitionData>::operator()(const int i, const int j, const int k)
{
    return m_image[m_gridInfo.cellId(i,j,k)];
}

template<class PartitionData>
void Partitioner<PartitionData>::fill( const PartitionData& value )
{
    for(auto& voxel : m_image)
    {
        for(auto& partition : voxel.partitions)
        {
            partition.data = value;
        }
    }
}

template<class PartitionData>
void Partitioner<PartitionData>::init(const GridUtility& _gridInfo, const VoxelGeometry& _voxelGeometry, const PartitionGeometry& _partitionGeometry, const PartitionerConnectivity& _connectivity)
{
    assert(_gridInfo.size() == _voxelGeometry.size());
    assert(_gridInfo.size() == _partitionGeometry.size());

    m_gridInfo = _gridInfo;
    m_image.resize(m_gridInfo.size());
    for(size_t i=0; i<m_image.size(); ++i)
    {
        m_image[i].triangles = _voxelGeometry[i];
        m_image[i].partitions.resize(_partitionGeometry[i].size());
        for(size_t j=0; j<m_image[i].partitions.size(); ++j)
        {
            m_image[i].partitions[j].triangles = _partitionGeometry[i][j];
            m_image[i].partitions[j].neighbors = _connectivity[i][j];
        }
    }
}

#endif
