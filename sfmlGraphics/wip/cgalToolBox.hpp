#ifndef CGALTOOLBOX_HPP
#define CGALTOOLBOX_HPP

#include <glm/glm.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_3<K> CGALPoint;
typedef CGAL::Segment_3<K> CGALSegment;
typedef CGAL::Plane_3<K> CGALPlane;
typedef CGAL::Vector_3<K> CGALVector;
typedef CGAL::Triangle_3<K> CGALTriangle;

//A face type with id and normal member variable.
template <class Refs, class Traits>
struct CustomVertex: public CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true, typename Traits::Point_3>
{
public:
    CustomVertex() : CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true, typename Traits::Point_3>() {}
    CustomVertex(const typename Traits::Point_3& pp) : CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true, typename Traits::Point_3>(pp) {}
    unsigned int m_id;
    unsigned int & id(){return m_id;}
    const unsigned int & id() const {return m_id;}
};

template <class Refs, class Traits>
struct CustomFace : public CGAL::HalfedgeDS_face_base<Refs, CGAL::Tag_true, typename Traits::Plane_3>
{
public:
    unsigned int m_id;
    CGALVector m_normal;
    unsigned int & id(){return m_id;}
    const unsigned int & id() const {return m_id;}
    CGALVector & normal(){return m_normal;}
    const CGALVector & normal() const {return m_normal;}
};

template <class Refs>
struct CustomHalfedge : public CGAL::HalfedgeDS_halfedge_base<Refs>
{
public:
};

//An items type using CustomFace and CustomVertex.
struct CustomItems : public CGAL::Polyhedron_items_3
{
public:
    template < class Refs, class Traits>
    struct Vertex_wrapper {
        typedef typename Traits::Point_3 Point;
        typedef CustomVertex<Refs,Traits> Vertex;
    };
    template < class Refs, class Traits>
    struct Halfedge_wrapper {
        typedef CustomHalfedge<Refs>    Halfedge;
    };
    template < class Refs, class Traits>
    struct Face_wrapper {
        typedef typename Traits::Plane_3 Plane;
        typedef CustomFace<Refs,Traits> Face;
    };
};

typedef CGAL::Polyhedron_3<K,CustomItems>                                                   Polyhedron;
typedef Polyhedron::HalfedgeDS                                                              HalfedgeDS;
typedef Polyhedron::Facet_handle    FacetHandle;
typedef Polyhedron::Vertex_handle   VertexHandle;
typedef Polyhedron::Halfedge_handle   HalfedgeHandle;

template<class HDS, class PointList, class TriangleList>
class MeshToPolyhedronBuilder : public CGAL::Modifier_base<HDS>
{
public:
    TriangleList* tList;
    PointList* pList;
    MeshToPolyhedronBuilder(PointList *pList_, TriangleList *tList_);
    void operator()( HDS& hds);
};

class CustomPolyhedron : public Polyhedron
{
public:
    CustomPolyhedron();
    ~CustomPolyhedron();
    std::vector<unsigned int> indices();
    std::vector<glm::vec3> positions();
};

void computeFacetNormal( FacetHandle& f );

#endif
