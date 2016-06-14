#include "practical_07_scene.hpp"

#include <ShaderProgram.hpp>
#include <FrameRenderable.hpp>
#include <texturing/TexturedPlaneRenderable.hpp>
#include <texturing/TexturedCubeRenderable.hpp>
#include <texturing/MultiTexturedCubeRenderable.hpp>
#include <texturing/MipMapCubeRenderable.hpp>
#include <texturing/BillBoardPlaneRenderable.hpp>
#include <lighting/DirectionalLightRenderable.hpp>
#include <texturing/TexturedTriangleRenderable.hpp>

void initialize_practical_07_triangle( Viewer& viewer )
{
    //Position the camera
//    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, -8, 8 ), glm::vec3(0, 0, 0), glm::vec3( 0, 0, 1 ) ) );
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, 0, 8 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );

    //Default shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");
    viewer.addShaderProgram( flatShader );

    //Add a 3D frame to the viewer
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    //Temporary variables
    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    std::string filename;

    //Textured shader
    //    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/simpleTextureVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/simpleTextureFragment.glsl");
    viewer.addShaderProgram( texShader );

    //Textured plane
    filename = "./../../sfmlGraphicsPipeline/textures/checkerboard.png";
    filename = "./../../sfmlGraphicsPipeline/textures/number_two.jpg";

//    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    TexturedTriangleRenderablePtr texPlane = std::make_shared<TexturedTriangleRenderable>(texShader, filename);
    parentTransformation = glm::scale(glm::mat4(1.0), glm::vec3(10.0,10.0,10.0));
    texPlane->setParentTransform(parentTransformation);
    viewer.addRenderable(texPlane);
}


//void initialize_practical_07_scene( Viewer& viewer )
//{
//    //Position the camera
//    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, -8, 8 ), glm::vec3(0, 0, 0), glm::vec3( 0, 0, 1 ) ) );

//    //Default shader
//    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl","../shaders/flatFragment.glsl");
//    viewer.addShaderProgram( flatShader );

//    //Add a 3D frame to the viewer
//    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
//    viewer.addRenderable(frame);

//    //Temporary variables
//    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
//    std::string filename;
//    MaterialPtr pearl = Material::Pearl();

//    //Define a directional light for the whole scene
//    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
//    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(0.0,1.0,0.0), d_specular(1.0,1.0,1.0);
//    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
//    //Add a renderable to display the light and control it via mouse/key event
//    glm::vec3 lightPosition(0.0,0.0,5.0);
//    DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
//    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
//    directionalLightRenderable->setLocalTransform(localTransformation);
//    viewer.setDirectionalLight(directionalLight);
//    viewer.addRenderable(directionalLightRenderable);

//    //Textured shader
//    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
//    viewer.addShaderProgram( texShader );

//    ShaderProgramPtr multiTexShader = std::make_shared<ShaderProgram>("../shaders/multiTextureVertex.glsl","../shaders/multiTextureFragment.glsl");
//    viewer.addShaderProgram( multiTexShader );

//    ShaderProgramPtr billboardShader = std::make_shared<ShaderProgram>("../shaders/billboardVertex.glsl","../shaders/billboardFragment.glsl");
//    viewer.addShaderProgram( billboardShader );

//    //Textured plane
//    filename = "./../textures/checkerboard.png";
//    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
//    parentTransformation = glm::scale(glm::mat4(1.0), glm::vec3(10.0,10.0,10.0));
//    texPlane->setParentTransform(parentTransformation);
//    texPlane->setMaterial(pearl);
//    viewer.addRenderable(texPlane);

//    //Textured cube
//    filename = "./../textures/number_one.jpg";
//    TexturedCubeRenderablePtr texCube = std::make_shared<TexturedCubeRenderable>(texShader, filename);
//    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(-2,0.0,0.5));
//    texCube->setParentTransform(parentTransformation);
//    texCube->setMaterial(pearl);
//    viewer.addRenderable(texCube);

//    //Mipmap cube
//    std::vector< std::string > filenames;
//    filenames.push_back("./../textures/number_one.jpg");
//    filenames.push_back("./../textures/number_two.jpg");
//    filenames.push_back("./../textures/number_three.jpg");
//    filenames.push_back("./../textures/number_four.jpg");
//    MipMapCubeRenderablePtr mipmapCube = std::make_shared<MipMapCubeRenderable>(texShader, filenames);
//    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,0.5));
//    mipmapCube->setParentTransform(parentTransformation);
//    mipmapCube->setMaterial(pearl);
//    viewer.addRenderable(mipmapCube);

//    //Multi-textured cube
//    std::string filename1="./../textures/crate.jpg", filename2="./../textures/halflife.png";
//    MultiTexturedCubeRenderablePtr multitexCube = std::make_shared<MultiTexturedCubeRenderable>(multiTexShader, filename1, filename2);
//    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
//    multitexCube->setParentTransform(parentTransformation);
//    multitexCube->setMaterial(pearl);
//    viewer.addRenderable(multitexCube);

//    //Billboard
//    std::string filename3="./../textures/grass.png";
//    glm::vec3 billboardPosition(0.0,2.0,0.5);
//    glm::vec2 billboardDimension(4.0,4.0);
//    BillBoardPlaneRenderablePtr billboard = std::make_shared<BillBoardPlaneRenderable>(billboardShader, filename3, billboardPosition, billboardDimension);
//    billboard->setMaterial(pearl);
//    viewer.addRenderable(billboard);

//    viewer.startAnimation();
//    viewer.setAnimationLoop(true, 4.0);
//}
