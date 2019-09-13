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


  int vecSize = 24;
  
  std::vector<GLfloat> vecArr;
  vecArr.resize(vecSize*2);

  
  for (int i = 0; i < testPoly.size(); i++) {
    vecArr[i] = testPoly[i];
  }

  auto angle = Poly::Core.getAngle(1, vecArr.size()/2);

  float tx, ty;
  // For every X,Y coord pair in polygon
  for (int i = 0; i < vecArr.size(); i+=2) {
    Poly::Core.getPoint(tx, ty, angle,
			(i/2));
    vecArr[i]   = tx;
    vecArr[i+1] = ty;

  }

  for (float& n : vecArr) {
    n *= 300.f;
  }
    for (int i = 0; i < vecArr.size(); i+=2) {
      vecArr[i] += circle.center.x;
      vecArr[i+1] += circle.center.y;

    }

    
    std::cout << "vecArr 3" << std::endl;
  for (int i = 0; i < (testPoly.size()/2)-1; i++) {
    std::cout << vecArr[i] << ", " << vecArr[i+1] << std::endl;
  }
  
  /*
    0, 1
    0, 2
    0, 3
    1, 2
    1, 3
    2, 3
  */

  std::vector<int> temp;

  int size = vecArr.size()/2;;

  temp.resize(indicesArrSizeFromVertices(size));
  
  std::vector<GLubyte> indices;
  indices.resize(vecArr.size());
  std::cout << "Big test" << std::endl;
  for (int i = 0; i < size; i++) {
    for (int x = i+1; x < size; x++) {
      std::cout << i << ", " << x << std::endl;
      indices.push_back(i);
      indices.push_back(x);
    }
  }


  
    /*= {
		       0, 1,
		       1, 2,
		       2, 3,
		       0, 2,
		       0, 3,
		       1, 3,

  };
    */


  
  
  

  
    
  /*
  GLubyte indices[] = {
		       0, 1,
		       1, 2,
		       2, 3,
		       3, 0,
  };
  */


  
  GLuint vbo;
  GLuint vao;

  GLuint indexBufferID;

  // Calls SDL/GL(EW) init functions
  Poly::Core.init();
    
  //VBO
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  //  glBufferData(GL_ARRAY_BUFFER, sizeof(polygon), polygon, GL_DYNAMIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, vecArr.size()*sizeof(GLfloat),
		 vecArr.data(), GL_DYNAMIC_DRAW);
  

  
  glGenBuffers(1, &indexBufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLubyte),
	       indices.data(), GL_DYNAMIC_DRAW);
  
  
  //VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);


  // Initialize shader program
  Poly::Shader.init();


  std::cout << "getArc: " << Poly::Core.getAngle(1, 4)
	    << std::endl;
  
  
  
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

    /*
    glBufferData(GL_ARRAY_BUFFER, sizeof(polygon),
		 polygon, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, arrSize(polygon)/2);
    */    

    glDrawElements(GL_LINES, indices.size()*sizeof(GLubyte),
		   GL_UNSIGNED_BYTE, indices.data());

    // std::cout << "testPoly.size: " << testPoly.size() << std::endl;

    
    //glDrawArrays(GL_LINE_STRIP, 0, testPoly.size()/2);

    SDL_GL_SwapWindow(Poly::Core.window);
    
  }    


 Exit:
  Poly::Core.exit();
  
  return 0;
}

int indicesArrSizeFromVertices(int size) {
  return (size*(size-1))/2;
}
