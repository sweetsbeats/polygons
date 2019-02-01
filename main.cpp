
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

#include "Engine.hpp"



#define arrSize(arr)(sizeof(arr)/sizeof(arr[0]))
#define foreach(iter, arr) for(iter=arr; *iter != arr[arrSize(arr)]; iter++)

#define PI 3.141592


int getIndicesFromVBO(int size);

inline double getArcBetweenPoints(double radius, int points) {
  return (2*PI*radius)/points;
}


bool running = true;


int main()
{

  struct Cursor {
    int32_t x, y;
  } cursor;

  

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
  
  
  GLuint vbo;
  GLuint vao;

  GLuint indexBufferID;

  // Calls SDL/GL(EW) init functions
  Poly::Core.init();

    
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


  // Initialize shader program
  Poly::Shader.init();



  SDL_GL_SetSwapInterval(1);
  
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(Poly::Core.window);


  
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

	  // I am a heretic, sue me
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

    
    glClear(GL_COLOR_BUFFER_BIT);
    
    //glDrawArrays(GL_LINE_STRIP, 0, 6);
    glDrawElements(GL_LINES, arrSize(indices), GL_UNSIGNED_BYTE, indices);

    SDL_GL_SwapWindow(Poly::Core.window);
    
  }    


 Exit:
  Poly::Core.exit();
  
  return 0;
}
