#ifndef CGALTOOLBOX_CPP
#define CGALTOOLBOX_CPP

#include "cgalToolBox.inl"

CustomPolyhedron::CustomPolyhedron() : Polyhedron()
{

}

CustomPolyhedron::~CustomPolyhedron()
{
}

std::vector<unsigned int> CustomPolyhedron::indices()
{
    std::vector<unsigned int> indices;
    for(Polyhedron::Face_const_iterator fit = this->facets_begin(); fit!=this->facets_end(); ++fit)
    {
        Polyhedron::Halfedge_around_facet_const_circulator eBegin = fit->facet_begin(), eit=eBegin;
        do
        {
            indices.push_back(eit->vertex()->id());
        }
        while(++eit != eBegin);
    }
    return indices;
}

std::vector<glm::vec3> CustomPolyhedron::positions()
{
    std::vector<glm::vec3> positions;
    for(Polyhedron::Vertex_const_iterator it = this->vertices_begin(); it!=this->vertices_end(); ++it)
    {
        positions.push_back( glm::vec3(it->point()[0], it->point()[1], it->point()[2]) );
    }
    return positions;
}

#endif
