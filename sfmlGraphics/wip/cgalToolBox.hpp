#ifndef CGALTOOLBOX_HPP
#define CGALTOOLBOX_HPP

#include <glm/glm.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel                                 K;
typedef CGAL::Polyhedron_3<K,CGAL::Polyhedron_items_with_id_3>                              Polyhedron;
typedef Polyhedron::HalfedgeDS                                                              HalfedgeDS;
typedef Polyhedron::Facet_handle    FacetHandle;
typedef Polyhedron::Vertex_handle   VertexHandle;
typedef CGAL::Point_3<K> CGALPoint;
typedef CGAL::Segment_3<K> CGALSegment;
typedef CGAL::Plane_3<K> CGALPlane;
typedef CGAL::Triangle_3<K> CGALTriangle;

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

#endif
