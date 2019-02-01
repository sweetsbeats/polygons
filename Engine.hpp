#pragma once

#include <cstdint>
#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>





namespace Poly {

  class _Core {
  public:
    SDL_Window* window = nullptr;
    SDL_GLContext GLcontext;

    uint16_t WINDOW_WIDTH = 600;
    uint16_t WINDOW_HEIGHT = 400;
  
    bool init();
    void exit();
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
