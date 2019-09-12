
/*
  POLYGONS

*/

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine.hpp"

#include <iostream>
#include <cstdint>
#include <vector>
#include <cassert>



#define arrSize(arr)(sizeof(arr)/sizeof(arr[0]))
#define foreach(iter, arr) for(iter=arr; *iter != arr[arrSize(arr)]; iter++)

#define PI 3.141592


int getIndicesFromVBO(int size);


int indicesArrSizeFromVertices(int size);


bool running = true;


int main()
{

  struct Cursor {
    int32_t x, y;
  } cursor;

  struct Point {
    int32_t x, y;
  };
  
  struct Circle {
    Point center{ Poly::Core.WINDOW_WIDTH/2,
		  Poly::Core.WINDOW_HEIGHT/2};
    
    double radius = 200.f;
    
  } circle;

  
  /*
    
    THESIS: Any convex polygon can be drawn inside 
            of a circle. 
			   
    PLAN: We can display this by taking
	  N vertices, calculating an arc len. between
	  N points on the unit circle using the formula:
	                 arc = 2*PI/N
	  Then for N vertices, emplace a vertex at:
	            (cos(N*arc), sin(N*arc)) 
	  We then want to connect every vertex in the shape 
	  with a line; This means not only an outline of
	  the shape, but also lines through the polygon.
			   
   */

  /*
  
  // 128 Vert count; Initialize to -1 
  GLfloat polygon[Poly::Core.MAX_VERT_COUNT*2] = {0};
  for(int i = 0; i < Poly::Core.MAX_VERT_COUNT*2; i++) {
    polygon[i] = -1.0f;
  }
  */
  
  
  /*
    FUNCTION FOR THE COUNT OF INDICES PAIRS
    f(x) = x(x-1)/2  =  x^2-x/2

    To make sure there's no dud lines, check for each pair that !=
  */

  
  /*
  GLfloat polygon[] = {
		       50.f, 50.f,
		       75.f, 30.f,
		       100.f, 50.f,
		       100.f, 75.f,
		       75.f, 95.f,
		       50.f, 75.f,
		       50.f, 50.f
  };  
  */

  
  std::vector<GLfloat> testPoly =
    {
     50.f, 50.f,
     75.f, 30.f,
     100.f, 50.f,
     100.f, 75.f,
     75.f, 95.f,
     50.f, 75.f,
     50.f, 50.f
    };

  std::vector<GLfloat> vecArr;
  vecArr.resize(testPoly.size());

  // TODO(sweets): Is this used for anything?
  int size = 6;

  /*
  for (auto& v : testPoly) {
    v+=100.f;
    // std::cout << "v: " << v << std::endl;
  }
*/

  double angle = Poly::Core.getAngle(1, 6);

  float tx, ty;
  // For every X,Y coord pair in polygon
  for (int i = 0; i < vecArr.size(); i+=2) {
    Poly::Core.getPoint(tx, ty, angle,
		        (i/2));
    vecArr[i]   = tx;
    vecArr[i+1] = ty;
    
  }

  assert(vecArr.size() == testPoly.size());
  
  for (auto& v: vecArr) {
    v *= 10.f;
   }

    for (int i = 0; i < vecArr.size(); i+=2) {
      std::cout << "x: " << vecArr[i] << " y: " << vecArr[i+1]
	      << " \t: " << (i/2) << std::endl;
  }

  
  /*
    0, 1
    0, 2
    0, 3
    1, 2
    1, 3
    2, 3
  */

  std::cout << "indicesArrSizeFromVertices: "
	    << indicesArrSizeFromVertices(size) << std::endl;

  
  std::vector<GLubyte> indices = {
				  0, 1,
				  1, 2,
				  2, 3,
				  3, 4,
				  4, 0,
  };
  /*
  indices.resize(vecArr.size());
  std::cout << "Big test" << std::endl;
  for (int i = 0; i < size; i++) {
    for (int x = i+1; x < size; x++) {
      std::cout << i << ", " << x << std::endl;
      indices.push_back(i);
      indices.push_back(x);
    }
  }
  */

  

  
  GLuint vbo;
  GLuint vao;

  GLuint indexBufferID;

  // Calls SDL/GL(EW) init functions
  Poly::Core.init();
    
  //VBO
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, testPoly.size()*sizeof(GLfloat),
		 testPoly.data(), GL_DYNAMIC_DRAW);
  

  
  glGenBuffers(1, &indexBufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLubyte),
	       indices.data(), GL_DYNAMIC_DRAW);
  
  
  //VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);


  // Initialize shader program
  Poly::Shader.init();


  float x, y;
  // X and Y are passed by reference and set in getPoint
  Poly::Core.getPoint(x, y, Poly::Core.getAngle(1, 6), 0);
  std::cout << "x: " << x << "y: " << y << std::endl;

  int testPoints = 4;
  
  
  
  SDL_GL_SetSwapInterval(1);
  
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
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
	
      case SDL_MOUSEBUTTONDOWN: 
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


    glDrawArrays(GL_TRIANGLE_STRIP, 0, testPoly.size());
    glDrawElements(GL_TRIANGLE_STRIP, indices.size(),
    		   GL_UNSIGNED_BYTE, indices.data());

    std::cout << glGetError() << std::endl;
    
    SDL_GL_SwapWindow(Poly::Core.window);
    
  }    


 Exit:
  Poly::Core.exit();
  
  return 0;
}

int indicesArrSizeFromVertices(int size) {
  return (size*(size-1))/2;
}
