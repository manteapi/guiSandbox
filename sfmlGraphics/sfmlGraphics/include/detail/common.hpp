#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>

static const float g_cubeVertices[] = {
 -1, -1, -1,       1,  1, -1,       1, -1, -1,      -1, -1, -1,      -1,  1, -1,       1,  1, -1,
  1,  1,  1,      -1, -1,  1,       1, -1,  1,      -1, -1,  1,       1,  1,  1,      -1,  1,  1,
  1,  1,  1,       1, -1, -1,       1,  1, -1,       1,  1,  1,       1, -1,  1,       1, -1, -1,
 -1,  1,  1,      -1,  1, -1,      -1, -1, -1,      -1,  1,  1,      -1, -1, -1,      -1, -1,  1,
 -1, -1, -1,       1, -1, -1,       1, -1,  1,      -1, -1, -1,       1, -1,  1,      -1, -1,  1,
 -1,  1, -1,       1,  1,  1,       1,  1, -1,      -1,  1, -1,      -1,  1,  1,       1,  1,  1
};

static const float g_cubeColors[] = {
  0,  0,  0,       0,  1,  0,       0,  0,  1,       0,  0,  0,       0,  1,  1,       0,  1,  0,
  1,  1,  0,       1,  0,  0,       1,  0,  1,       1,  0,  0,       1,  1,  0,       1,  1,  1,
  1,  1,  0,       0,  0,  1,       0,  1,  0,       1,  1,  0,       1,  0,  1,       0,  0,  1,
  1,  1,  1,       0,  1,  1,       0,  0,  0,       1,  1,  1,       0,  0,  0,       1,  0,  0,
  0,  0,  0,       0,  0,  1,       1,  0,  1,       0,  0,  0,       1,  0,  1,       1,  0,  0,
  0,  1,  1,       1,  1,  0,       0,  1,  0,       0,  1,  1,       1,  1,  1,       1,  1,  0,
};

static const float g_cubeNormals[] = {
  0,  0, -1,       0,  0, -1,       0,  0, -1,       0,  0, -1,       0,  0, -1,       0,  0, -1,
  0,  0,  1,       0,  0,  1,       0,  0,  1,       0,  0,  1,       0,  0,  1,       0,  0,  1,

  1,  0,  0,       1,  0,  0,       1,  0,  0,       1,  0,  0,       1,  0,  0,       1,  0,  0,
 -1,  0,  0,      -1,  0,  0,      -1,  0,  0,      -1,  0,  0,      -1,  0,  0,      -1,  0,  0,

  0, -1,  0,       0, -1,  0,       0, -1,  0,       0, -1,  0,       0, -1,  0,       0, -1,  0,
  0,  1,  0,       0,  1,  0,       0,  1,  0,       0,  1,  0,       0,  1,  0,       0,  1,  0,
};

static const unsigned int g_numberOfCubeVertices = 36;

/**
 * Data for a cone
 * - center     0, 0, 0
 * - direction  1, 0, 0
 * - length     1
 * - max radius 1
 */
static const float g_coneVertices[] = {
  0.000000,  0.000000,  0.000000,
  1.000000,  1.000000,  0.000000, 1.000000,  0.987688,  0.156434, 1.000000,  0.951057,  0.309017, 1.000000,  0.891007,  0.453990,
  1.000000,  0.809017,  0.587785, 1.000000,  0.707107,  0.707107, 1.000000,  0.587785,  0.809017, 1.000000,  0.453990,  0.891007,
  1.000000,  0.309017,  0.951057, 1.000000,  0.156434,  0.987688, 1.000000,  0.000000,  1.000000, 1.000000, -0.156434,  0.987688,
  1.000000, -0.309017,  0.951057, 1.000000, -0.453990,  0.891007, 1.000000, -0.587785,  0.809017, 1.000000, -0.707107,  0.707107,
  1.000000, -0.809017,  0.587785, 1.000000, -0.891007,  0.453990, 1.000000, -0.951057,  0.309017, 1.000000, -0.987688,  0.156434,
  1.000000, -1.000000,  0.000000, 1.000000, -0.987688, -0.156434, 1.000000, -0.951057, -0.309017, 1.000000, -0.891007, -0.453990,
  1.000000, -0.809017, -0.587785, 1.000000, -0.707107, -0.707107, 1.000000, -0.587785, -0.809017, 1.000000, -0.453990, -0.891007,
  1.000000, -0.309017, -0.951057, 1.000000, -0.156434, -0.987688, 1.000000, -0.000000, -1.000000, 1.000000,  0.156434, -0.987688,
  1.000000,  0.309017, -0.951057, 1.000000,  0.453990, -0.891007, 1.000000,  0.587785, -0.809017, 1.000000,  0.707107, -0.707107,
  1.000000,  0.809017, -0.587785, 1.000000,  0.891007, -0.453990, 1.000000,  0.951057, -0.309017, 1.000000,  0.987688, -0.156434,
  1.000000,  1.000000,  0.000000,
};

static const float g_coneNormals[] = {
 -1.000000,  0.000000,  0.000000,
  0.000000,  1.000000,  0.000000, 0.000000,  0.987688,  0.156434, 0.000000,  0.951057,  0.309017, 0.000000,  0.891007,  0.453990,
  0.000000,  0.809017,  0.587785, 0.000000,  0.707107,  0.707107, 0.000000,  0.587785,  0.809017, 0.000000,  0.453990,  0.891007,
  0.000000,  0.309017,  0.951057, 0.000000,  0.156434,  0.987688, 0.000000,  0.000000,  1.000000, 0.000000, -0.156434,  0.987688,
  0.000000, -0.309017,  0.951057, 0.000000, -0.453990,  0.891007, 0.000000, -0.587785,  0.809017, 0.000000, -0.707107,  0.707107,
  0.000000, -0.809017,  0.587785, 0.000000, -0.891007,  0.453990, 0.000000, -0.951057,  0.309017, 0.000000, -0.987688,  0.156434,
  0.000000, -1.000000,  0.000000, 0.000000, -0.987688, -0.156434, 0.000000, -0.951057, -0.309017, 0.000000, -0.891007, -0.453990,
  0.000000, -0.809017, -0.587785, 0.000000, -0.707107, -0.707107, 0.000000, -0.587785, -0.809017, 0.000000, -0.453990, -0.891007,
  0.000000, -0.309017, -0.951057, 0.000000, -0.156434, -0.987688, 0.000000, -0.000000, -1.000000, 0.000000,  0.156434, -0.987688,
  0.000000,  0.309017, -0.951057, 0.000000,  0.453990, -0.891007, 0.000000,  0.587785, -0.809017, 0.000000,  0.707107, -0.707107,
  0.000000,  0.809017, -0.587785, 0.000000,  0.891007, -0.453990, 0.000000,  0.951057, -0.309017, 0.000000,  0.987688, -0.156434,
  0.000000,  1.000000,  0.000000,
};

static const unsigned int g_numberOfConeVertices = 42;

static const float g_billboardVertices[] = {
    -0.5, -0.5,
     0.5, -0.5,
     0.5,  0.5,
    -0.5,  0.5
};

static const float g_billboardTexcoord[] = {
    0, 0,
    1, 0,
    1, 1,
    0, 1
};

enum { CUBE_VERTICES, CUBE_COLORS, CUBE_NORMALS, CONE_VERTICES, CONE_NORMALS, BILLBOARD_VERTICES, BILLBOARD_TEXCOORD, NUMBER_OF_BUFFERS };
static unsigned int g_buffersNames[ NUMBER_OF_BUFFERS ] = { 0 };

enum { BILLBOARD_FLOWER_TEXTURE, BILLBOARD_GRASS_TEXTURE, NUMBER_OF_TEXTURES };
static unsigned int g_texturesNames[ NUMBER_OF_TEXTURES ] = { 0 };

static const void initializeStaticGLObjects()
{
  if( !g_buffersNames[0] )
    {
      glcheck(glGenBuffers( NUMBER_OF_BUFFERS, g_buffersNames ));

      glcheck(glBindBuffer(GL_ARRAY_BUFFER, g_buffersNames[CUBE_VERTICES]));
      glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof g_cubeVertices, g_cubeVertices, GL_STATIC_DRAW ));

      glcheck(glBindBuffer(GL_ARRAY_BUFFER, g_buffersNames[CUBE_COLORS]));
      glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof g_cubeColors, g_cubeColors, GL_STATIC_DRAW ));

      glcheck(glBindBuffer(GL_ARRAY_BUFFER, g_buffersNames[CUBE_NORMALS]));
      glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof g_cubeNormals, g_cubeNormals, GL_STATIC_DRAW ));

      glcheck(glBindBuffer(GL_ARRAY_BUFFER, g_buffersNames[CONE_VERTICES]));
      glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof g_coneVertices, g_coneVertices, GL_STATIC_DRAW ));

      glcheck(glBindBuffer(GL_ARRAY_BUFFER, g_buffersNames[CONE_NORMALS]));
      glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof g_coneNormals, g_coneNormals, GL_STATIC_DRAW ));

      glcheck(glBindBuffer(GL_ARRAY_BUFFER, g_buffersNames[BILLBOARD_VERTICES]));
      glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof g_billboardVertices, g_billboardVertices, GL_STATIC_DRAW ));

      glcheck(glBindBuffer(GL_ARRAY_BUFFER, g_buffersNames[BILLBOARD_TEXCOORD]));
      glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof g_billboardTexcoord, g_billboardTexcoord, GL_STATIC_DRAW ));

      glcheck(glBindBuffer(GL_ARRAY_BUFFER, 0 ));

      glcheck(glGenTextures( NUMBER_OF_TEXTURES, g_texturesNames ));
      sf::Image image;

      assert(image.loadFromFile( "./../textures/billboardredflowers.png"));
      glcheck(glBindTexture(GL_TEXTURE_2D, g_texturesNames[ BILLBOARD_FLOWER_TEXTURE ]))
      glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
      glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
      glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
      glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
      //http://docs.gl/gl4/glTexImage2D
      glcheck(glTexImage2D(
          GL_TEXTURE_2D, 0, GL_RGBA,
          image.getSize().x, image.getSize().y,
          0,
          GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr()));


      assert(image.loadFromFile( "./../textures/grass.png"));
      glcheck(glBindTexture(GL_TEXTURE_2D, g_texturesNames[ BILLBOARD_GRASS_TEXTURE ]))
      glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
      glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
      glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
      glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
      glcheck(glTexImage2D(
          GL_TEXTURE_2D, 0, GL_RGBA,
          image.getSize().x, image.getSize().y,
          0,
          GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr()));

      glcheck(glBindTexture(GL_TEXTURE_2D, 0 ));
    }
}
