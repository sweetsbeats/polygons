
/*
  POLYGONS

*/

#include <iostream>
#include <cstdint>

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shaders.hpp"


#define arrSize(arr)(sizeof(arr)/sizeof(arr[0]))
#define foreach(iter, arr) for(iter=arr; *iter != arr[arrSize(arr)]; iter++)



uint16_t WINDOW_WIDTH = 600;
uint16_t WINDOW_HEIGHT = 400;

bool running = true;


int main()
{

  
  SDL_Window* window = nullptr;
  SDL_GLContext GLcontext;

  
  GLuint shaderProgram;
  GLuint vertexShader;
  GLuint fragmentShader;

  
  GLuint vbo;
  GLuint vao;

  GLuint indexBufferID;



  struct Cursor {
    int32_t x, y;
  } cursor;

  
  
  /*
  float square[] =
    {
     0.f,  0.0f,
     0.5f, 0.f,
     0.5f, -0.5f,
     0.f, -0.5f
    };

  */

  /*
  float triangle[] = {
		      (float)(WINDOW_WIDTH/2), (float)(WINDOW_HEIGHT/2) -100,
		      (float)(WINDOW_WIDTH/2)+100, (float)(WINDOW_HEIGHT/2) +100,
		      (float)(WINDOW_WIDTH/2)-100, (float)(WINDOW_HEIGHT/2) +100
  };

  */

  /*
  GLfloat polygon[] = {
		     50, 50,
		     100, 50,
		     100, 100,
		     50, 100,
  };
  */

  GLfloat polygon[8] = {0};
  for(int i = 0; i < 8; i++) {
    polygon[i] = -1.0f;
  }
  
  /*
    
    FUNCTION FOR THE COUNT OF INDICES PAIRS
    f(x) = x(x-1)/2  =  x^2-x/2


    To make sure there's no dud lines, check for each pair that !=


  */

  GLubyte indices[] = {
		       0, 1,
		       1, 2,
		       2, 3,
		       3, 0,
  };
  
  /*
  GLubyte indices[] = {
		       1, 0,
		       1, 2,
		       2, 3,
		       2, 0,
		       3, 0,
		       3, 1,
  };
  */

  
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

  

  //glOrtho(0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.f, 0.f, 1.f);


    
  //VBO
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(polygon), polygon, GL_DYNAMIC_DRAW);

  glGenBuffers(1, &indexBufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

  
  
  
  //VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);


  //SHADERS

  GLint status;


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
    return -1;
  }
  
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);
  
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);


  glm::mat4 projection = glm::ortho(0.f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.f, -1.f, 1.f);

  GLint projectionMatrix = glGetUniformLocation(shaderProgram, "projection");
  glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, glm::value_ptr(projection));
  

  
  SDL_GL_SetSwapInterval(1);
  
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(window);

 
   
  
  
  SDL_Event event;
  
  while (running) {


    SDL_GL_SetSwapInterval(1);
    
    while( SDL_PollEvent(&event) ) {

      switch(event.type) {
      case SDL_QUIT:
	running = false;
	break;
	
      case SDL_MOUSEMOTION:
	cursor.x = event.motion.x;
	cursor.y = event.motion.y;
	break;
	
      case SDL_MOUSEBUTTONDOWN: {
	
	GLfloat* mem = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	
	if (!mem) {
	  std::cout << "Fatal error mapping array buffer! Exiting:" << std::endl;
	  glUnmapBuffer(GL_ARRAY_BUFFER);
	  goto Exit;
	}
	else {
	  for(int i = 0; i <= 6; i+=2) {
	    std::cout << mem[i] << std::endl;
	    if(mem[i] == -1 && mem[i+1] == -1) {
	      mem[i] = cursor.x;
	      mem[i+1] = cursor.y;
	      break;
	    }
	  }
	}
	
	
	
      }
	
	glUnmapBuffer(GL_ARRAY_BUFFER);
	
	break;
	
	  
      case SDL_KEYDOWN:
	switch(event.key.keysym.sym) {
       
	case SDLK_RIGHT:
	  glClearColor(1.0, 0.0, 0.0, 1.0);
	 
	  break;
	  
	case SDLK_LEFT:
	  glClearColor(1.0, 0.0, 1.0, 0.0);
	  break;
	  
	}
      }
    }


    /*   MOUSE POSITION DEBUG PRINT
    std::cout << "Mouse Pos {" << cursor.x << ", " << cursor.y << "}" << std::endl;
    */

    

    
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    //glDrawArrays(GL_LINE_STRIP, 0, 6);
    glDrawElements(GL_LINES, arrSize(indices), GL_UNSIGNED_BYTE, indices);

    SDL_GL_SwapWindow(window);
    
    
    //    SDL_Delay(2000);
    //    running = false;
  }    


 Exit:
  SDL_GL_DeleteContext(GLcontext);
  SDL_DestroyWindow(window);

  SDL_Quit();
  
  return 0;
}
