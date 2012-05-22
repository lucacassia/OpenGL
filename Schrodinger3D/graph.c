#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_utils.h"
#include "distribution.h"

#define N 256

distribution *complexSurface;

GLuint program;
GLint attribute_coord2d;
GLint uniform_vertex_transform;
GLint uniform_texture_transform;
GLint uniform_color;
GLuint texture_id;
GLint uniform_mytexture;

float offset_x = 0.0;
float offset_y = 0.0;
float scale = 1.0;

bool interpolate = true;
bool clamp = false;
bool rotate = false;
bool polygonoffset = true;
bool active = true;

int modeView = 0;

GLuint vbo[3];

struct point {
  GLfloat x;
  GLfloat y;
};

GLbyte graph[N][N];

void setupTexture()
{
  // Create our datapoints, store it as bytes
  double max = 0;
  for(int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      double z = 0;
      switch(modeView){
        case 0:
          z = _complex_mod(complexSurface->psi[i*N+j]);
          break;
        case 1:
          z = (complexSurface->psi[i*N+j]).re;
          break;
        case 2:
          z = (complexSurface->psi[i*N+j]).im;
          break;
      }
      if(fabs(z)>max)
        max = fabs(z);
    }
  }
  for(int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      double z = 0;
      switch(modeView){
        case 0:
          z = _complex_mod(complexSurface->psi[i*N+j]);
          break;
        case 1:
          z = (complexSurface->psi[i*N+j]).re;
          break;
        case 2:
          z = (complexSurface->psi[i*N+j]).im;
          break;
      }
      graph[i][j] = roundf(z/max * 127 + 128);
    }
  }

  /* Upload the texture with our datapoints */
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(
     GL_TEXTURE_2D,      // target
     0,                  // level, 0 = base, no minimap,
     GL_LUMINANCE,       // internalformat
     N,                  // width
     N,                  // height
     0,                  // border, always 0 in OpenGL ES
     GL_LUMINANCE,       // format
     GL_UNSIGNED_BYTE,   // type
     graph
  );

  // Create an array for 101 * 101 vertices
  point vertices[101][101];

  for(int i = 0; i < 101; i++) {
    for(int j = 0; j < 101; j++) {
      vertices[i][j].x = (j - 50) / 50.0;
      vertices[i][j].y = (i - 50) / 50.0;
    }
  }

  // Tell OpenGL to copy our array to the buffer objects
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

  // Create an array of indices into the vertex array that traces both horizontal and vertical lines
  GLushort indices[100 * 101 * 6];
  int i = 0;

  for(int y = 0; y < 100; y++) {
    for(int x = 0; x < 100; x++) {
      indices[i++] = y * 101 + x;
      indices[i++] = y * 101 + x + 1;
    }
  }

  for(int x = 0; x < 101; x++) {
    for(int y = 0; y < 100; y++) {
      indices[i++] = y * 101 + x;
      indices[i++] = (y + 1) * 101 + x;
    }
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 100 * 101 * 4 * sizeof *indices, indices, GL_STATIC_DRAW);

  // Create another array of indices that describes all the triangles needed to create a completely filled surface
  i = 0;

  for(int y = 0; y < 101; y++) {
    for(int x = 0; x < 100; x++) {
      indices[i++] = y * 101 + x;
      indices[i++] = y * 101 + x + 1;
      indices[i++] = (y + 1) * 101 + x + 1;

      indices[i++] = y * 101 + x;
      indices[i++] = (y + 1) * 101 + x + 1;
      indices[i++] = (y + 1) * 101 + x;
    }
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

}

int init_resources()
{

  int vertex_texture_units;
  glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &vertex_texture_units);
  if(!vertex_texture_units) {
    fprintf(stderr, "Your graphics cards does not support texture lookups in the vertex shader!\n");
    return 0;
  }

  GLint link_ok = GL_FALSE;

  GLuint vs, fs;
  if ((vs = create_shader("graph.v.glsl", GL_VERTEX_SHADER))   == 0) return 0;
  if ((fs = create_shader("graph.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    return 0;
  }

  const char* attribute_name;
  attribute_name = "coord2d";
  attribute_coord2d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord2d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }

  const char* uniform_name;
  uniform_name = "vertex_transform";
  uniform_vertex_transform = glGetUniformLocation(program, uniform_name);
  if (uniform_vertex_transform == -1) {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }

  uniform_name = "texture_transform";
  uniform_texture_transform = glGetUniformLocation(program, uniform_name);
  if (uniform_texture_transform == -1) {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }

  uniform_name = "color";
  uniform_color = glGetUniformLocation(program, uniform_name);
  if (uniform_color == -1) {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }

  // Generate texture name
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &texture_id);
  // Create two vertex buffer objects
  glGenBuffers(3, vbo);

  setupTexture();

  return 1;
}

void display()
{
  if(active){
    distribution_compute(complexSurface);
    setupTexture();
  }

  glUseProgram(program);
  glUniform1i(uniform_mytexture, 0);

  glm::mat4 model;

  if(rotate)
    model = glm::rotate(glm::mat4(1.0f), float(glutGet(GLUT_ELAPSED_TIME) / 100.0), glm::vec3(0.0f, 0.0f, 1.0f));
  else
    model = glm::mat4(1.0f);  

  glm::mat4 view = glm::lookAt(glm::vec3(0.0, -2.0, 2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
  glm::mat4 projection = glm::perspective(45.0f, 1.0f*640/480, 0.1f, 10.0f);

  glm::mat4 vertex_transform = projection * view * model;
  glm::mat4 texture_transform = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1)), glm::vec3(offset_x, offset_y, 0));

  glUniformMatrix4fv(uniform_vertex_transform, 1, GL_FALSE, glm::value_ptr(vertex_transform));
  glUniformMatrix4fv(uniform_texture_transform, 1, GL_FALSE, glm::value_ptr(texture_transform));

  glClearColor(1.0, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Set texture wrapping mode */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);

  /* Set texture interpolation mode */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolate ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolate ? GL_LINEAR : GL_NEAREST);

  /* Draw the triangles, a little dark, with a slight offset in depth. */
  GLfloat grey[4] = {0.5, 0.5, 0.5, 1};
  glUniform4fv(uniform_color, 1, grey);

  glEnable(GL_DEPTH_TEST);

  if(polygonoffset) {
    glPolygonOffset(1, 0);
    glEnable(GL_POLYGON_OFFSET_FILL);
  }

  glEnableVertexAttribArray(attribute_coord2d);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
  glDrawElements(GL_TRIANGLES, 100 * 100 * 6, GL_UNSIGNED_SHORT, 0);

  glPolygonOffset(0, 0);
  glDisable(GL_POLYGON_OFFSET_FILL);

  /* Draw the grid, very bright */
  GLfloat bright[4] = {2, 2, 2, 1};
  glUniform4fv(uniform_color, 1, bright);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
  glDrawElements(GL_LINES, 100 * 101 * 4, GL_UNSIGNED_SHORT, 0);

  /* Stop using the vertex buffer object */
  glDisableVertexAttribArray(attribute_coord2d);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glutSwapBuffers();
}

void special(int key, int x, int y)
{
  switch(key) {
    case GLUT_KEY_F1:
      interpolate = !interpolate;
      break;
    case GLUT_KEY_F2:
      clamp = !clamp;
      break;
    case GLUT_KEY_F3:
      rotate = !rotate;
      break;
    case GLUT_KEY_F4:
      polygonoffset = !polygonoffset;
      break;
    case GLUT_KEY_F5:
      modeView = 0;
      break;
    case GLUT_KEY_F6:
      modeView = 1;
      break;
    case GLUT_KEY_F7:
      modeView = 2;
      break;
    case GLUT_KEY_F11:
      glutFullScreenToggle();
      break;
    case GLUT_KEY_LEFT:
      offset_x -= 0.03;
      break;
    case GLUT_KEY_RIGHT:
      offset_x += 0.03;
      break;
    case GLUT_KEY_UP:
      offset_y += 0.03;
      break;
    case GLUT_KEY_DOWN:
      offset_y -= 0.03;
      break;
    case GLUT_KEY_PAGE_UP:
      scale *= 1.5;
      break;
    case GLUT_KEY_PAGE_DOWN:
      scale /= 1.5;
      break;
    case GLUT_KEY_HOME:
      offset_x = 0.0;
      offset_y = 0.0;
      scale = 1.0;
      break;
  }
  glutPostRedisplay();
}

void free_resources()
{
  distribution_free(complexSurface);
  glDeleteProgram(program);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'q': case 'Q': case 27:
            free_resources();
            exit(EXIT_SUCCESS);
        case ' ':
            active =! active;
            break;
    }
}

int main(int argc, char* argv[]) {

  complexSurface = distribution_alloc(N,N);

  for(int i = 0; i < N; i++)
    for(int j = 0; j < N; j++) {
      float x = i*2.0/(N-1)-1;
      float y = j*2.0/(N-1)-1;
      float d = hypotf(x, y) * 4.0;
      float z = (1 - d * d) * expf(d * d / -2.0);
      float s = 0.1;
      float kx = -1e2;
      float ky = 0;
      complexSurface->psi[i*N+j].re = z;
      complexSurface->psi[i*N+j].re = cos(kx*x+ky*y)*expf(-((x-0.5)*(x-0.5)+y*y)/(2*s*s))/sqrt(2*3.1415926)/s;
      complexSurface->psi[i*N+j].im = sin(kx*x+ky*y)*expf(-((x-0.5)*(x-0.5)+y*y)/(2*s*s))/sqrt(2*3.1415926)/s;
    }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
  glutInitWindowSize(640, 480);
  glutCreateWindow("graph");

  GLenum glew_status = glewInit();
  if (GLEW_OK != glew_status) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
    return 1;
  }

  if (!GLEW_VERSION_2_0) {
    fprintf(stderr, "No support for OpenGL 2.0 found\n");
    return 1;
  }

  GLint max_units;
  glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &max_units);
  if(max_units < 1) {
	  fprintf(stderr, "Your GPU does not have any vertex texture image units\n");
	  return 1;
  }

  if (init_resources()) {
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
  }

  free_resources();
  return 0;
}
