#include "Engine.hpp"
#include "Shaders.hpp"

#include <iostream>



namespace Poly {

  _Core Core;
  _Shader Shader;

};


bool Poly::_Core::init() {
  if ( SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Error initializing SDL" << std::endl;
    return -1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);


  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  
  window = SDL_CreateWindow("polygons", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			    WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  if (!window) {
    std::cout << "Error initializing WINDOW" << std::endl;
    return -1;
  }

  GLcontext = SDL_GL_CreateContext(window);


  glewExperimental = true;
  GLenum err =  glewInit();

  if (err!=GLEW_OK) {
    std::cout << "Error intializing GLEW\n" << glewGetErrorString(err) << std::endl;
    return -1;
  }
  
  return true;
}


void Poly::_Core::exit() {
  SDL_GL_DeleteContext(GLcontext);
  SDL_DestroyWindow(window);

  SDL_Quit();
  
}





bool Poly::_Shader::init() {
  

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    std::cout << "Vertex shader failed to compile" << std::endl;
    char buffer[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    for (int i = 0; i < 512; i++) {
      std::cout << buffer[i] << std::endl;
    }
    return -1;
  }
  
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    std::cout << "Vertex shader failed to compile" << std::endl;
    return false;
  }
  
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);
  
  posAttrib = glGetAttribLocation(shaderProgram, "position");
  
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);


  projection = glm::ortho(0.f, (float)Poly::Core.WINDOW_WIDTH,
			  (float)Poly::Core.WINDOW_HEIGHT,
			  0.f, -1.f, 1.f);

  projectionMatrix = glGetUniformLocation(shaderProgram, "projection");

  glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE,
		     glm::value_ptr(projection));
  

  return true;
  
}
