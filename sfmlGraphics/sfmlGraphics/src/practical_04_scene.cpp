#include "../include/practical_04_scene.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/Viewer.hpp"

#include "../include/KeyframedCylinderRenderable.hpp"
#include "../include/FrameRenderable.hpp"
#include "../include/GeometricTransformation.hpp"
#include <iostream>
#include <iomanip>

void initialize_practical_04_scene( Viewer& viewer )
{
    practical04_movingCylinder(viewer);
    practical04_movingTree(viewer);
}

void practical04_movingTree( Viewer& viewer )
{
    //Add shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl", "../shaders/flatFragment.glsl");
    viewer.addShaderProgram( flatShader );

    //Frame
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    //Tree modeling:
    //The modeling is hierarchical (setLocalTransform, setParentTransform)
    //The animation is hierarchical too (addParentTransformKeyframe, addLocalTransformKeyframe)

    //Main branch 
    KeyframedCylinderRenderablePtr root = std::make_shared<KeyframedCylinderRenderable>(flatShader);
    root->setLocalTransform( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}).toMatrix() );

    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{2,0,0}), 0.0 );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{2,0,0}, glm::quat(glm::vec3(0, 0.1, 0))), 1.0 );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{2,0,0}, glm::quat(glm::vec3(0, 0.1, 0))), 2.0 );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{2,0,0}, glm::quat(glm::vec3(0, 0.3, 0))), 2.5 );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{2,0,0}, glm::quat(glm::vec3(0, 0.3, 0))), 3.0 );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{2,0,0}, glm::quat(glm::vec3(0, 0.0, 0))), 5.0 );

    //First primary branch
    KeyframedCylinderRenderablePtr r1 = std::make_shared<KeyframedCylinderRenderable>(flatShader);

    r1->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3(0, 0.78, 0)) ), 0.0 );
    r1->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3(0, 0.84, 0)) ), 2.0 );
    r1->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3(0, float(M_PI_4), 0)) ), 5.0 );

    r1->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}), 0.0 );
    r1->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.06,0.06,2.0}), 2.0 );
    r1->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}), 5.0 );
    HierarchicalRenderable::addChild( root, r1 );


    KeyframedCylinderRenderablePtr r2 = std::make_shared<KeyframedCylinderRenderable>(flatShader);

    r2->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3( 0, 0.35, 0) ) ), 0.0 );
    r2->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3( 0, 0.25, 0) ) ), 1.0 );
    r2->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3( 0, 0.32, 0) ) ), 2.0 );
    r2->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3( 0, 0.25, 0) ) ), 3.0 );
    r2->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3( 0, 0.35, 0) ) ), 5.0 );

    r2->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}), 0.0 );
    r2->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.04,0.04,2.0}), 2.0 );
    r2->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}), 5.0 );

    HierarchicalRenderable::addChild( r1, r2 );

    //First secondary branch 
    KeyframedCylinderRenderablePtr s1 = std::make_shared<KeyframedCylinderRenderable>(flatShader);

    s1->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,0.4}, glm::angleAxis( float(M_PI_4), glm::normalize(glm::vec3( 1,1,0)) ), glm::vec3{1,1,1} ), 0 );

    s1->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,1.5}), 0 );
    s1->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.06,0.06,1.5}), 3 );
    s1->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,1.5}), 5 );
    HierarchicalRenderable::addChild( r1, s1 );

    KeyframedCylinderRenderablePtr s11 = std::make_shared<KeyframedCylinderRenderable>(flatShader);
    s11->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,1.5}, glm::quat( glm::vec3( 0, -0.78, 0) ), glm::vec3{1,1,1} ), 0 );
    s11->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,1.5}, glm::quat( glm::vec3( 0, -0.4, 0) ), glm::vec3{1,1,1} ), 2 );
    s11->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,1.5}, glm::quat( glm::vec3( 0, -0.6, 0) ), glm::vec3{1,1,1} ), 3 );
    s11->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,1.5}, glm::quat( glm::vec3( 0, -0.65, 0) ), glm::vec3{1,1,1} ), 4.5 );
    s11->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,1.5}, glm::quat( glm::vec3( 0, -0.78, 0) ), glm::vec3{1,1,1} ), 5 );

    s11->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}), 0 );
    s11->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.04,0.04,1.5}), 3 );
    s11->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}), 5 );

    HierarchicalRenderable::addChild( s1, s11 );

    //Second secondary branch
    KeyframedCylinderRenderablePtr s2 = std::make_shared<KeyframedCylinderRenderable>(flatShader);
    s2->setParentTransform( GeometricTransformation( glm::vec3{0,0,1.4}, glm::angleAxis( float(M_PI_4), glm::normalize(glm::vec3(-0.3 ,-1,0)) ), glm::vec3{1,1,1} ).toMatrix() );
    s2->setLocalTransform( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}).toMatrix() );
    HierarchicalRenderable::addChild( r1, s2 );

    KeyframedCylinderRenderablePtr s21 = std::make_shared<KeyframedCylinderRenderable>(flatShader);
    s21->setParentTransform( GeometricTransformation( glm::vec3{0,0,2.0}, glm::angleAxis( float(-M_PI_4), glm::normalize(glm::vec3( 0, -1, 0)) ), glm::vec3{1,1,1} ).toMatrix() );
    s21->setLocalTransform( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}).toMatrix() );
    HierarchicalRenderable::addChild( s2, s21 );

    //Third secondary branch
    KeyframedCylinderRenderablePtr s3 = std::make_shared<KeyframedCylinderRenderable>(flatShader);
    s3->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0, 0, 2 }, glm::quat( glm::vec3{-0.78, -0.6, -0.5} ) ), 0 );
    s3->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0, 0, 2 }, glm::quat( glm::vec3{-0.78,  0.3, -0.5} ) ), 2 );
    s3->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0, 0, 2 }, glm::quat( glm::vec3{-0.78, -0.6, -0.5} ) ), 5 );

    s3->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}), 0);
    s3->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.05,0.05,3.0}), 1);
    s3->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.08,0.08,2.5}), 4);
    s3->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}), 5);
    HierarchicalRenderable::addChild( r1, s3 );

    viewer.addRenderable( root );

    viewer.startAnimation();
    viewer.setAnimationLoop(true, 6.0);
}

void practical04_movingCylinder( Viewer& viewer )
{
    //Add shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl", "../shaders/flatFragment.glsl");
    viewer.addShaderProgram( flatShader );

    //Frame
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, -8, 4 ), glm::vec3(0, 0, 4), glm::vec3( 0, 0, 1 ) ) );

    //Animated cylinder
    auto cylinder = std::make_shared<KeyframedCylinderRenderable>(flatShader);
    cylinder->setParentTransform(glm::mat4(1.0));

    //Keyframes on parent transformation
    cylinder->addParentTransformKeyframe(GeometricTransformation( glm::vec3(0.0,0.0,0.0) ), 0.0);
    cylinder->addParentTransformKeyframe(GeometricTransformation( glm::vec3(1.0,0.0,0.0) ), 1.0);
    cylinder->addParentTransformKeyframe(GeometricTransformation( glm::vec3(0.0,1.0,0.0) ), 2.0);
    cylinder->addParentTransformKeyframe(GeometricTransformation( glm::vec3(-1.0,0.0,0.0) ), 3.0);
    cylinder->addParentTransformKeyframe(GeometricTransformation( glm::vec3(0.0,0.0,0.0) ), 4.0);

    //Keyframes on local transformation
    cylinder->addLocalTransformKeyframe(GeometricTransformation( glm::vec3{}, glm::angleAxis((float)(0.0), glm::vec3(0.0, 0.0, 1.0)) ),0.0);
    cylinder->addLocalTransformKeyframe(GeometricTransformation( glm::vec3{}, glm::angleAxis((float)(2.0*M_PI/4.0), glm::vec3(0.0, 0.0, 1.0)) ), 1.0 );
    cylinder->addLocalTransformKeyframe(GeometricTransformation( glm::vec3{}, glm::angleAxis((float)(4.0*M_PI/4.0), glm::vec3(0.0, 0.0, 1.0)) ), 2.0 );
    cylinder->addLocalTransformKeyframe(GeometricTransformation( glm::vec3{}, glm::angleAxis((float)(6.0*M_PI/4.0), glm::vec3(0.0, 0.0, 1.0)) ), 3.0 );
    cylinder->addLocalTransformKeyframe(GeometricTransformation( glm::vec3{}, glm::angleAxis((float)(8.0*M_PI/4.0), glm::vec3(0.0, 0.0, 1.0)) ), 4.0 );

    viewer.addRenderable( cylinder );

    viewer.startAnimation();
    viewer.setAnimationLoop(true, 6.0);
}
