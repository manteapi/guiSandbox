if( ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR}/build )
else()
	message( FATAL_ERROR "Dear student, you just tried to launch"
	" cmake in another directory than build/. We encourage you to"
	" be more careful in the future and NEVER DO THAT AGAIN. Now "
	"the build system will stop, and you will remove " 
	"../CMakeCache.txt and ../CMakefiles/.\n--Your 3D Computer Graphics teachers.")
endif()

#==========================================
#Project options
#==========================================
cmake_minimum_required(VERSION 2.8)

#==========================================
#Project name
#==========================================
project(sfmlGraphicsPipeline)

#==========================================
#Building options
#==========================================
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

#==========================================
#Libraries path : glm, sfml, glew, opengl, freetype
#==========================================

#ITSELF
include_directories(include)

#GLM Libraries
set(GLM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/glm-0.9.7.1" CACHE PATH "glm")
include_directories(${GLM_INCLUDE_DIRS})

#GLEW Libraries
set(GLEW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/glew-1.13.0/include" CACHE PATH "glew")
include_directories(${GLEW_INCLUDE_DIRS})

#SFML Libraries
set(SFML_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/sfml-2.3.2/include/" CACHE PATH "sfml")
include_directories(${SFML_INCLUDE_DIRS})

#FREETYPE Libraries
set(FREETYPE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/freetype-2.5.5/include" CACHE PATH "freetype")
include_directories(${FREETYPE_INCLUDE_DIRS})

#TINYOBJLOADER Libraries
set(TINYOBJLOADER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/tinyobjloader/" CACHE PATH "tinyobjloader")
include_directories(${TINYOBJLOADER_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

#==============================================
#Project sources : src, include, shader, exe
#==============================================
file(
    GLOB_RECURSE
    HEADER_FILES
    include/*.hpp
    )

file(
    GLOB_RECURSE
    SOURCE_FILES
    src/*.cpp
    )
    
file(
    GLOB_RECURSE
    SHADER_FILES
    shaders/*.glsl
    )

#==============================================
#Project library 
#==============================================
add_library(SFML_GRAPHICS_PIPELINE ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES})

#==============================================
#Doxygen command
#==============================================
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        file(MAKE_DIRECTORY doc) 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        COMMENT "Propagating API documentation in web directory" VERBATIM
        COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/doc/html/ ${CMAKE_CURRENT_SOURCE_DIR}/../../chamilo/api_doc/
        )
endif(DOXYGEN_FOUND)

set(EXTRA_CLEANING
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/html
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/latex
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/doxygen_sqlite3.db
    )
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${EXTRA_CLEANING}")

message( "The build type is set to " ${CMAKE_BUILD_TYPE})
