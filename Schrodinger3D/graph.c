#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "shader_utils.h"
#include "distribution.h"

//#define PIPE
#define PI 3.141592653589793238462643383279502884197169399375105820974944592308
#define N 256

distribution *complexSurface;

GLuint program;
GLint attribute_coord2d;
GLint uniform_vertex_transform;
GLint uniform_texture_transform;
GLint uniform_color;
GLuint texture_id;
GLint uniform_mytexture;

float camera_angle = PI/4;
float camera_distance = 3.0f;
float texture_angle = 0.0f;
float texture_offset_x = 0.0f;
float texture_offset_y = 0.0f;
float texture_scale = 1.0f;

float xrot = 0.0f;
float yrot = 0.0f;
float mouse_x = 0.0f;
float mouse_y = 0.0f;

float amplitude = 1.0f;

int mouse_down = 0;
int interpolate = 1;
int clamp = 0;
int rotate = 0;
int polygonoffset = 1;
int active = 1;

int modeView = 0;
int width = 640;
int height = 480;
int grid = 150;

GLuint vbo[3];

struct point {
    GLfloat x;
    GLfloat y;
};

GLbyte graph[N][N];
unsigned char *frame = NULL;

void savePPM(unsigned char *frame)
{
    FILE *f = fopen("image.ppm", "wb");
    fprintf(f, "P6\n%d %d\n255\n", width, height);
    int i,j;
    for(i = height-1; i >= 0; i--)
        for(j = 0; j < width; j++)
            fwrite(&frame[(i*width+j)*3], sizeof(unsigned char), 3, f);
    fclose(f);
}

void setupTexture()
{
    int i,j,x,y;
    // Create our datapoints, store it as bytes
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
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
            graph[i][j] = roundf(z * amplitude * 127 + 128);
        }
    }

    /* Upload the texture with our datapoints */
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(
        GL_TEXTURE_2D,        // target
        0,                    // level, 0 = base, no minimap,
        GL_LUMINANCE,         // internalformat
        N,                    // width
        N,                    // height
        0,                    // border, always 0 in OpenGL ES
        GL_LUMINANCE,         // format
        GL_UNSIGNED_BYTE,     // type
        graph
    );

    // Create an array for (grid+1) * (grid+1) vertices
    struct point vertices[(grid+1)][(grid+1)];

    for(i = 0; i < (grid+1); i++) {
        for(j = 0; j < (grid+1); j++) {
            vertices[i][j].x = (j - (grid/2)) / (grid/2.0);
            vertices[i][j].y = (i - (grid/2)) / (grid/2.0);
        }
    }

    // Tell OpenGL to copy our array to the buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    // Create an array of indices into the vertex array that traces both horizontal and vertical lines
    GLushort indices[grid * (grid+1) * 6];

    for(i = y = 0; y < grid; y++) {
        for(x = 0; x < grid; x++) {
            indices[i++] = y * (grid+1) + x;
            indices[i++] = y * (grid+1) + x + 1;
        }
    }

    for(x = 0; x < (grid+1); x++) {
        for(y = 0; y < grid; y++) {
            indices[i++] = y * (grid+1) + x;
            indices[i++] = (y + 1) * (grid+1) + x;
        }
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, grid * (grid+1) * 4 * sizeof *indices, indices, GL_STATIC_DRAW);

    // Create another array of indices that describes all the triangles needed to create a completely filled surface
    for(i = y = 0; y < (grid+1); y++) {
        for(x = 0; x < grid; x++) {
            indices[i++] = y * (grid+1) + x;
            indices[i++] = y * (grid+1) + x + 1;
            indices[i++] = (y + 1) * (grid+1) + x + 1;

            indices[i++] = y * (grid+1) + x;
            indices[i++] = (y + 1) * (grid+1) + x + 1;
            indices[i++] = (y + 1) * (grid+1) + x;
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
    if ((vs = create_shader("v.glsl", GL_VERTEX_SHADER))   == 0) return 0;
    if ((fs = create_shader("f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

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
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    width = viewport[2];
    height = viewport[3];

#ifdef PIPE
        frame = (unsigned char*)malloc(3*width*height*sizeof(unsigned char));
        glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,frame);
        for(int i = height-1; i >= 0; i--)
            for(int j = 0; j < width; j++)
                fwrite(&frame[(i*width+j)*3], sizeof(unsigned char), 3, stdout);
        free(frame);
#endif

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);
    glUniform1i(uniform_mytexture, 0);

    /*Transformations*/
    GLfloat texture_transform[16], vertex_transform[16];

    /*set vertex transformations*/
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f * 640 / 480, 0.1f, 500.0f);
    if(camera_angle + yrot > PI/2.1)
        yrot = PI/2.1 - camera_angle;
    if(camera_angle + yrot < -PI/2.1)
        yrot = -PI/2.1-camera_angle;
    gluLookAt( 0, -camera_distance * cos(camera_angle + yrot), camera_distance * sin(camera_angle + yrot), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    if(rotate && !mouse_down)
        texture_angle += 0.5;
    glRotatef(texture_angle + xrot, 0.0f, 0.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, vertex_transform);
    glUniformMatrix4fv(uniform_vertex_transform, 1, GL_FALSE, vertex_transform);
    glPopMatrix();

    /*set texture transformations*/
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(texture_offset_x, texture_offset_y, 0.0f);
    glScalef(texture_scale, texture_scale, texture_scale);
    glTranslatef(-texture_offset_x, -texture_offset_y, 0.0f);
    glGetFloatv(GL_TEXTURE_MATRIX, texture_transform);
    glUniformMatrix4fv(uniform_texture_transform, 1, GL_FALSE, texture_transform);
    glPopMatrix();

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
    glDrawElements(GL_TRIANGLES, grid * grid * 6, GL_UNSIGNED_SHORT, 0);

    glPolygonOffset(0, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);

    /* Draw the grid, very bright */
    GLfloat bright[4] = {2, 2, 2, 1};
    glUniform4fv(uniform_color, 1, bright);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glDrawElements(GL_LINES, grid * (grid+1) * 4, GL_UNSIGNED_SHORT, 0);

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
            setupTexture();
            break;
        case GLUT_KEY_F6:
            modeView = 1;
            setupTexture();
            break;
        case GLUT_KEY_F7:
            modeView = 2;
            setupTexture();
            break;
        case GLUT_KEY_F8:
            frame = (unsigned char*)malloc(3*width*height*sizeof(unsigned char));
            glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,frame);
            savePPM(frame);
            free(frame);
            break;
        case GLUT_KEY_LEFT:
            texture_offset_x -= 0.03;
            break;
        case GLUT_KEY_RIGHT:
            texture_offset_x += 0.03;
            break;
        case GLUT_KEY_UP:
            texture_offset_y += 0.03;
            break;
        case GLUT_KEY_DOWN:
            texture_offset_y -= 0.03;
            break;
        case GLUT_KEY_PAGE_UP:
            texture_scale *= 1.5;
            break;
        case GLUT_KEY_PAGE_DOWN:
            texture_scale /= 1.5;
            break;
        case GLUT_KEY_HOME:
            texture_offset_x = 0.0;
            texture_offset_y = 0.0;
            texture_scale = 1.0;
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (!mouse_down && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouse_down = 1;
        mouse_x = x;
        mouse_y = y;

    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        mouse_down = 0;
        texture_angle += xrot;
        camera_angle += yrot;
        xrot = 0.0;
        yrot = 0.0;
    }
}

void motion(int x, int y)
{
    if (mouse_down) {
        xrot = (x-mouse_x)*45.0/640;
        yrot = (y-mouse_y)*PI/480/2.0;
        glutPostRedisplay();
    }
}

void free_resources()
{
    distribution_free(complexSurface);
    glDeleteProgram(program);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 'q': case 'Q': case 27:
            free_resources();
            exit(EXIT_SUCCESS);
        case ' ':
            active =! active;
            break;
        case '+':
            amplitude *= 1.5;
            setupTexture();
            break;
        case '-':
            amplitude /= 1.5;
            setupTexture();
            break;
        case 'z':
            camera_distance /= 1.1;
            break;
        case 'Z':
            camera_distance *= 1.1;
            break;
    }
}

int main(int argc, char* argv[]) {

    complexSurface = distribution_alloc(N,N);
    distribution_init(complexSurface,0.1,0,0);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowSize(width, height);
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
        glutMouseFunc(mouse);
        glutMotionFunc(motion);
        glutMainLoop();
    }

    free_resources();
    return 0;
}
