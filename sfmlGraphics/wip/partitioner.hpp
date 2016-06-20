#ifndef PARTITIONER_HPP
#define PARTITIONER_HPP

#include <iostream>
#include <set>
#include <vector>
#include <memory>

#include "gridUtility.hpp"
#include "cgalToolBox.inl"
#include <omp.h>


enum VoxelType { None, Out, In, Border};

typedef glm::ivec2 Vec2i;
typedef std::vector< std::vector<int> > VoxelGeometry;
typedef std::vector< std::vector< std::set<int> > > PartitionGeometry;
typedef std::vector< std::vector< std::set< Vec2i > > > PartitionerConnectivity;
typedef std::vector< FacetHandle > FacetHandleList;
typedef std::set< FacetHandle > FacetHandleSet;
typedef std::vector< Vec2i > VertexToPartition;

struct Vec2iCompare {bool operator() (const Vec2i& lhs, const Vec2i& rhs) const{return lhs[0] < rhs[0];}};

std::set<unsigned int> getCellsOverTriangle(const GridUtility& grid, const std::array<Vec3r,3>& t);
std::array<float,8> trilinearWeights(const std::array<Vec3r,8>& cellVertices, const Vec3r& x);
bool trianglePlaneIntersection(std::array<Vec3r, 4>& plane, const std::array<Vec3r, 3>& t);

template<class PartitionData>
class Partition
{
public:
    ~Partition();
    Partition();
    /*!
     * \brief Clone constructor.
     * \param partition a partition to clone.
     */
    Partition(const Partition& partition);
    Partition(const int& voxelId, const FacetHandleSet& triangles,
              const std::set<Vec2i, Vec2iCompare> &neighbors, const PartitionData& data);

    int m_voxelId; //!< A reference to the voxel which contains this partition.
    FacetHandleSet m_triangles; //!< A set of triangles which constitutes the partition.
    std::set<Vec2i, Vec2iCompare> m_neighbors; //<! Store the coordinates (voxelId,partitionId) of the neighbors.
    PartitionData m_data; //!< Partition data.

    /*!
     * \brief Return a list of neighbors of the current partition.
     * \return A list of coordinates (voxelId,partitionId) of the partition's neighbors.
     */
    std::set<Vec2i, Vec2iCompare>& getNeighbors();
    const std::set<Vec2i, Vec2iCompare> &getNeighbors() const;
    void clearGeometry();
    void addTriangle(const FacetHandle& facetHandle);
};

template<class PartitionData>
class Voxel
{
public:   
    HReal length; //!< Voxel length. The voxel is assumed to be a square.
    Vec3r offset; //!< Bottom left voxel corner world coordinates.
    FacetHandleList triangles; //!< List of reference to triangles which intersects the voxel.
    std::vector< Partition<PartitionData> > partitions; //!< List of partitions which constitutes the voxel.
    VoxelType m_type;

    ~Voxel();
    Voxel();

    /*!
     * \brief Clone constructor.
     * \param voxel a voxel to copy.
     */
    Voxel(const Voxel& voxel);

    /*!
     * \brief Specific constructor.
     * \param length_ the voxel length. The voxel is assumed to be a square.
     * \param offset_ the bottom left voxel corner world coordinates.
     */
    Voxel(HReal length_,const Vec3r& offset_, VoxelType type_);

    /*!
     * \brief Access a partition from its id.
     * \param partitionId the id of the partition.
     * \return a reference to the corresponding partition.
     */
    Partition<PartitionData>& operator[](const int partitionId);

    /*!
     * \brief Access a partition from its id.
     * \param partitionId the id of the partition.
     * \return a const reference to the corresponding partition.
     */
    const Partition<PartitionData>& operator[](const int partitionId) const;

    void addTriangle(const FacetHandle& facetHandle);
    void clearGeometry();
    size_t size() const;

    friend std::istream& operator >> ( std::istream& in, Voxel& /*p*/ )
    {
        return in;
    }

    friend std::ostream& operator << ( std::ostream& out, const Voxel& p )
    {
        out << "Partition size : " << p.partitions.size();
        out << std::endl;
        out << "Geometry size : " << p.triangles.size();
        out << std::endl;
        out << "Geometry id :";
        out << std::endl;
        for(size_t i=0; i<p.triangles.size(); ++i)
        {
            out << p.triangles[i] << " ";
            out << std::endl;
        }
        return out;
    }

};

/*!
 * \brief An image containing data organized with respect to a geometry.
 *
 */
template<class PartitionData>
class Partitioner
{

public :
    GridUtility m_gridInfo; //!< Grid informations : offset, dimension, utility functions.
    std::vector< Voxel<PartitionData> > m_image; //!< Grid data. A list  of voxel containing partitions and data.

    /*!
     * \brief Destructor.
     */
    ~Partitioner();

    /*!
     * \brief Default constructor.
     */
    Partitioner();

    Partitioner(CustomPolyhedron& polyhedron, const HReal &voxelSpacing);

    void markVoxels();

    /*!
     * \brief Clone constructor.
     * \param partitioner a partitioner to copy.
     */
    Partitioner(const Partitioner& partitioner);

    /*!
     * \brief Specific constructor.
     *
     * From gridInformations initialize the grid.
     *
     * \param _gridIndo the information about the grid.
     */
    Partitioner(const GridUtility & _gridInfo);

    /*! \brief Resize the image.
     *
     * The vector of voxel is resized and initialized with respect to the grid informations.
     */
    void initImage();

    /*!
     * \brief Set the connectivity of the image from another one.
     * \param partitioner a reference to the image whose connectivity will be copied.
     */
    template<class TData>
    void setConnectivity(const Partitioner<TData>& partitioner);

    /*!
     * \brief Fill all the partitions of the image with a given value.
     * \param value the partition value.
     */
    void fill(const PartitionData& value);

    /*!
     * \brief Access a voxel from its linear id.
     * \param voxelId the voxel id.
     * \return A reference to the corresponding voxel.
     */
    Voxel<PartitionData>& operator[](const int voxelId);

    /*!
     * \brief Access a voxel from its linear id.
     * \param voxelId the voxel id.
     * \return A const reference to the corresponding voxel.
     */
    const Voxel<PartitionData>& operator[](const int voxelId) const;

    /*!
     * \brief Access a voxel from its coordinates in the image.
     * \param i the line id of the voxel.
     * \param j the column id of the voxel.
     * \return A reference to the corresponding voxel.
     */
    Voxel<PartitionData>& operator()(const int i, const int j, const int k);

    /*!
     * \brief Access a voxel from its coordinates in the image.
     * \param i the line id of the voxel.
     * \param j the column id of the voxel.
     * \return A const reference to the corresponding voxel.
     */
    Voxel<PartitionData>& operator()(const int i, const int j, const int k) const;

    friend std::istream& operator >> ( std::istream& in, Partitioner& /*p*/ )
    {
        return in;
    }

    friend std::ostream& operator << ( std::ostream& out, const Partitioner& p )
    {
        out << "offset : " << p.m_gridInfo.offset[0] << ", "<< p.m_gridInfo.offset[1] << ", "<< p.m_gridInfo.offset[2] << std::endl;
        out << "spacing : " << p.m_gridInfo.spacing() << std::endl;
        out << "dimension : " << p.m_gridInfo.dimension[0] << ", "<< p.m_gridInfo.dimension[1] << ", "<< p.m_gridInfo.dimension[2] << std::endl;
        return out;
    }

    void init(const GridUtility& _gridInfo, const VoxelGeometry& _voxelGeometry, const PartitionGeometry& _partitionGeometry, const PartitionerConnectivity& _connectivity);
};


#endif // PARTITIONER_HPP
