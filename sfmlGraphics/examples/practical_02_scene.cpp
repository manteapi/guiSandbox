#include <ShaderProgram.hpp>
#include <Viewer.hpp>

#include <CylinderRenderable.hpp>
#include <FrameRenderable.hpp>
#include <MeshRenderable.hpp>

void initialize_practical_02_scene( Viewer& viewer )
{
  ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
      "../shaders/flatVertex.glsl",
      "../shaders/flatFragment.glsl");

  ShaderProgramPtr pointLightShader = std::make_shared<ShaderProgram>(
      "../shaders/pointLightVertex.glsl",
      "../shaders/pointLightFragment.glsl");

  viewer.addShaderProgram( flatShader );
  viewer.addShaderProgram( pointLightShader );


  viewer.addRenderable( std::make_shared<CylinderRenderable>(flatShader) );
  viewer.addRenderable( std::make_shared<FrameRenderable>(flatShader ) );

  MeshRenderablePtr mesh = std::make_shared<MeshRenderable>(pointLightShader, "./../meshes/suzanne.obj");
  mesh->setModelMatrix( glm::translate(glm::mat4(1.0), glm::vec3(-6,2,0)) );
  viewer.addRenderable(mesh);
}
