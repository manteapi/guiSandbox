#ifndef CGALTOOLBOX_INL
#define CGALTOOLBOX_INL

#include "cgalToolBox.hpp"

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//                                       CGAL MeshToPolyhedronBuilder
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

template<class HDS, class PointList, class TriangleList>
MeshToPolyhedronBuilder<HDS, PointList, TriangleList>::MeshToPolyhedronBuilder(PointList* pList_, TriangleList* tList_)
{
    tList = tList_;
    pList = pList_;
}

template<class HDS, class PointList, class TriangleList>
void MeshToPolyhedronBuilder<HDS, PointList, TriangleList>::operator()( HDS& hds)
{
    // Postcondition: hds is a valid polyhedral surface.
    CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);

    B.begin_surface(pList->size(), tList->size()/3, 0, CGAL::Polyhedron_incremental_builder_3<HDS>::RELATIVE_INDEXING);

    typedef typename HDS::Vertex   Vertex;
    typedef typename Vertex::Point Point;

    //Fill vertex data structure
    for(size_t i=0; i<pList->size(); ++i)
    {
        double x = (*pList)[i][0];
        double y = (*pList)[i][1];
        double z = (*pList)[i][2];
        Polyhedron::Vertex_iterator vh = B.add_vertex( Point( x, y, z));
        vh->id() = i;
    }

    //Fill faces data structure
    for(size_t i=0; i<(tList->size())/3; ++i)
    {
        unsigned int v1,v2,v3;
        v1 = (*tList)[3*i+0];
        v2 = (*tList)[3*i+1];
        v3 = (*tList)[3*i+2];
        Polyhedron::Facet_handle fh = B.begin_facet();
        B.add_vertex_to_facet(v1);
        B.add_vertex_to_facet(v2);
        B.add_vertex_to_facet(v3);
        B.end_facet();
        fh->id() = i;
    }

    B.end_surface();
}

#endif
