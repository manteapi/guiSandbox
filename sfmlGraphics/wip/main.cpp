#include <Viewer.hpp>
#include <Io.hpp>
#include <lighting/LightedMeshRenderable.hpp>
#include <lighting/DirectionalLightRenderable.hpp>
#include <FrameRenderable.hpp>

#include "cgalToolBox.inl"
#include "partitioner.inl"
#include "partitionerRenderable.inl"

#include <log.hpp>
#include <sstream>

int main( int argc, char* argv[] )
{
    Viewer viewer(1280,720);

    //Flat shader
    std::string vShader = "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl";
    std::string fShader = "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl";
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>( vShader,fShader);

    //Add a 3D frame to the viewer
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    //Phong Shader
    vShader = "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl";
    fShader = "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl";
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>( vShader,fShader);
    viewer.addShaderProgram( phongShader );

    //Light
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,-1.0,-1.0));
    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(0.3,0.3,0.1), d_specular(0.3,0.3,0.1);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    viewer.setDirectionalLight(directionalLight);

    //Light renderable
    glm::vec3 lightPosition(0.0,5.0,8.0);
    DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
    glm::mat4 localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    directionalLightRenderable->setLocalTransform(localTransformation);
    viewer.addRenderable(directionalLightRenderable);

    //Build a CGAL mesh
    typedef std::vector<glm::vec3> PointList;
    typedef std::vector<unsigned int> TriangleList;
    PointList points;
    TriangleList triangles;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::string meshFileName = "./../../sfmlGraphicsPipeline/meshes/Bunny.obj";
    read_obj(meshFileName, points, triangles, normals, texCoords);
    MeshToPolyhedronBuilder<HalfedgeDS, PointList, TriangleList> polyBuilder( &points, &triangles );
    CustomPolyhedron m_polyhedron;
    m_polyhedron.delegate(polyBuilder);
    /*
    points = m_polyhedron.positions();
    triangles = m_polyhedron.indices();
    std::cout << m_polyhedron.size_of_facets() << std::endl;
    std::cout << m_polyhedron.size_of_vertices() << std::endl;
    */

    //Build a partitioner for the mesh
    HReal voxelSpacing = 0.1;
    std::shared_ptr< Partitioner<int> > partitioner = std::make_shared< Partitioner<int> >(m_polyhedron, voxelSpacing);
    std::cout << *(partitioner.get()) << std::endl;
    std::shared_ptr< PartitionerRenderable<int> > partitionerRenderable = std::make_shared< PartitionerRenderable<int> >(flatShader, partitioner);
    viewer.addRenderable(partitionerRenderable);
    partitionerRenderable->voxelTriangles(points, triangles, normals);

    //Load CGAL mesh data in a renderable
    MaterialPtr pearl = Material::Pearl();
    LightedMeshRenderablePtr suzanne1 = std::make_shared<LightedMeshRenderable>(phongShader, points, triangles, normals, texCoords);
    glm::mat4 parentTransformation = glm::mat4(1.0);
    localTransformation = glm::mat4(1.0);
    suzanne1->setParentTransform(parentTransformation);
    suzanne1->setLocalTransform(localTransformation);
    suzanne1->setMaterial(pearl);
    viewer.addRenderable( suzanne1 );

    while( viewer.isRunning() )
    {
        viewer.handleEvent();
        viewer.animate();
        viewer.draw();
        viewer.display();
    }

    return EXIT_SUCCESS;
}
