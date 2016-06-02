#include <ShaderProgram.hpp>
#include <Viewer.hpp>

#include <HierarchicalCylinderRenderable.hpp>
#include <FrameRenderable.hpp>
#include <HierarchicalMeshRenderable.hpp>

void initialize_practical_03_scene( Viewer& viewer )
{
  ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
      "../shaders/flatVertex.glsl",
      "../shaders/flatFragment.glsl");

  viewer.addShaderProgram( flatShader );

  //Temporary variable to use to define transformation
   glm::mat4 rotationM(1.0), rot1(1.0), rot2(1.0);
   glm::mat4 scaleM(1.0);
   glm::mat4 translationM(1.0);

   //Frame
   FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
   viewer.addRenderable(frame);

   //Torso
   HierarchicalCylinderRenderablePtr torso= std::make_shared<HierarchicalCylinderRenderable>(flatShader);
   scaleM = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,2.0));
   torso->setLocalTransform(scaleM*torso->getModelMatrix());

   //Head
   HierarchicalMeshRenderablePtr head= std::make_shared<HierarchicalMeshRenderable>(flatShader, "./../meshes/suzanne.obj");
   translationM = glm::translate(glm::mat4(1.0), glm::vec3(0,0,3));
   head->setParentTransform(translationM);
   rotationM = glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(1,0,0));
   head->setLocalTransform(rotationM);

   //Left arm
   HierarchicalCylinderRenderablePtr leftArm = std::make_shared<HierarchicalCylinderRenderable>(flatShader);
   rotationM = glm::rotate(glm::mat4(1.0), -(float)(M_PI/2.0), glm::vec3(0,1,0));
   translationM = glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,2.0));
   leftArm->setParentTransform(translationM);
   scaleM = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,2.0));
   leftArm->setLocalTransform(rotationM*scaleM);

   //Left hand
   HierarchicalMeshRenderablePtr leftHand= std::make_shared<HierarchicalMeshRenderable>(flatShader, "./../meshes/suzanne.obj");
   translationM = glm::translate(glm::mat4(1.0), glm::vec3(-2.5,0,0));
   leftHand->setParentTransform(translationM);
   scaleM = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
   rot1 = glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(1,0,0));
   rot2 = glm::rotate(glm::mat4(1.0), -(float)(M_PI/2.0), glm::vec3(0,0,1));
   leftHand->setLocalTransform(rot2*rot1*scaleM);

   //Right arm
   HierarchicalCylinderRenderablePtr rightArm = std::make_shared<HierarchicalCylinderRenderable>(flatShader);
   rotationM = glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0,1,0));
   translationM = glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,2.0));
   rightArm->setParentTransform(translationM);
   scaleM = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,2.0));
   rightArm->setLocalTransform(rotationM*scaleM);

   //Right hand
   HierarchicalMeshRenderablePtr rightHand= std::make_shared<HierarchicalMeshRenderable>(flatShader, "./../meshes/suzanne.obj");
   translationM = glm::translate(glm::mat4(1.0), glm::vec3(2.5,0,0));
   rightHand->setParentTransform(translationM);
   scaleM = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
   rot1 = glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(1,0,0));
   rot2 = glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0,0,1));
   rightHand->setLocalTransform(rot2*rot1*scaleM);

   HierarchicalRenderable::addChild(torso, head);
   HierarchicalRenderable::addChild(torso, leftArm);
   HierarchicalRenderable::addChild(torso, rightArm);
   HierarchicalRenderable::addChild(leftArm, leftHand);
   HierarchicalRenderable::addChild(rightArm, rightHand);
   viewer.addRenderable(torso);
}
