#include "practical_02_scene.hpp"
#include <ShaderProgram.hpp>
#include <CylinderRenderable.hpp>
#include <FrameRenderable.hpp>
#include <MeshRenderable.hpp>

void initialize_practical_02_scene( Viewer& viewer )
{
  ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
      "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
      "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");

  viewer.addShaderProgram( flatShader );

  viewer.addRenderable( std::make_shared<CylinderRenderable>(flatShader) );
  viewer.addRenderable( std::make_shared<FrameRenderable>(flatShader ) );
  MeshRenderablePtr mesh = std::make_shared<MeshRenderable>(flatShader, "./../../sfmlGraphicsPipeline/meshes/suzanne.obj");
  mesh->setModelMatrix( glm::translate(glm::mat4(1.0), glm::vec3(-6,2,0)) );
  viewer.addRenderable(mesh);
}
