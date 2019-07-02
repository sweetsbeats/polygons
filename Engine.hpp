#pragma once

#include <cstdint>
#include <cmath>

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.141592


namespace Poly {

  class _Core {
  public:
    SDL_Window* window = nullptr;
    SDL_GLContext GLcontext;

    uint16_t WINDOW_WIDTH = 600;
    uint16_t WINDOW_HEIGHT = 400;

    uint16_t MAX_VERT_COUNT = 128;
    uint16_t vertCount = 4;


    
    
    
    bool init();
    void exit();

    inline double getAngle(double radius, int points) {
      return (2*PI*radius)/points;
    }

    void getPoint(float& x, float& y, double angle, int point);
    void makePolyFromCount(GLuint& vbo, GLuint& vao, int vertices);
  };
  
  class _Shader {
  public:
    GLuint shaderProgram;
    GLuint vertexShader;
    GLuint fragmentShader;
    
    GLint status;
    
    GLint posAttrib;
    
    glm::mat4 projection;

    GLint projectionMatrix;
    
    bool init();
  };



  extern _Core Core;
  extern _Shader Shader;

};
