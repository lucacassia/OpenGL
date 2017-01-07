#include "julia.h"
#include <GL/freeglut.h>
#include <stdio.h>

#define PI 3.141592654

void savePPM()
{
    unsigned char *frame = (unsigned char*) malloc( 3 * width * height * sizeof(unsigned char) );
    glReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, frame );

    FILE *f = fopen( "image.ppm", "wb" );
    fprintf( f, "P6\n%d %d\n255\n", width, height );
    fwrite( frame, sizeof(unsigned char), 3 * width * height, f );
    fclose( f );

    free( frame );
}

void GLInit()
{
    glDisable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT );
    glOrtho( 0, width, 0, height, 0, 1 );
}

void displayF()
{
    float* data = (float*) malloc( 3 * width * height * sizeof(float) );

    unsigned int k;
    for( k = 0; k < width * height; k++ )
    {
        data[3*k+0] = sin( factor * pixels[k] * PI / 100 + colorPhase[0] );
        data[3*k+1] = sin( factor * pixels[k] * PI / 100 + colorPhase[1] + PI / 3 );
        data[3*k+2] = sin( factor * pixels[k] * PI / 100 + colorPhase[2] + 2 * PI / 3 );
    }
    glRasterPos2i( 0, 0 );
    glDrawPixels( width, height, GL_RGB, GL_FLOAT, data );
    glutSwapBuffers();

    free( data );
}

void keyboardF( unsigned char key, int mouseX, int mouseY )
{
    int flagCompute = 0;

    switch( key )
    {
        case 'p': case 'P':
            savePPM();
            break;
        case 'q': case 'Q': case 27:
            clear();
            exit( EXIT_SUCCESS );

        case 'r': colorPhase[0] += 0.1; break;
        case 'g': colorPhase[1] += 0.1; break;
        case 'b': colorPhase[2] += 0.1; break;
        case 'c': colorPhase[0] += 0.1;
                  colorPhase[1] += 0.1;
                  colorPhase[2] += 0.1; break;
        case 'R': colorPhase[0] -= 0.1; break;
        case 'G': colorPhase[1] -= 0.1; break;
        case 'B': colorPhase[2] -= 0.1; break;
        case 'C': colorPhase[0] -= 0.1;
                  colorPhase[1] -= 0.1;
                  colorPhase[2] -= 0.1; break;
        case 'f': factor += 0.01; break;
        case 'F': factor -= 0.01; break;

        case '+': flagCompute = 1; zoom *= 2; shift /= 2; break;
        case '-': flagCompute = 1; zoom /= 2; shift *= 2; break;
        case 'w': flagCompute = 1; c.im *= 1.01; break;
        case 'a': flagCompute = 1; c.re /= 1.01; break;
        case 's': flagCompute = 1; c.im /= 1.01; break;
        case 'd': flagCompute = 1; c.re *= 1.01; break;

        case ',': flagCompute = 1; divergence += 1; break;
        case '.': flagCompute = 1; halt += 1; break;
        case 'j': flagCompute = 1; jexp *= 1.1; break;

        case ' ': flagCompute = 1; resetView(); break;
    }

    if ( flagCompute ) compute();
}

void specialKeyF( int key, int x, int y )
{
    switch( key ){
        case GLUT_KEY_F11:
            glutFullScreenToggle();
            glClear( GL_COLOR_BUFFER_BIT );
            break;
        case GLUT_KEY_UP:
            focus.im += shift;
            compute();
            break;
        case GLUT_KEY_LEFT:
            focus.re -= shift;
            compute();
            break;
        case GLUT_KEY_DOWN:
            focus.im -= shift;
            compute();
            break;
        case GLUT_KEY_RIGHT:
            focus.re += shift;
            compute();
            break;
    }
}

int main( int argc, char *argv[] )
{
    init();

    glutInit( &argc, argv );
    glutInitWindowSize( width, height ); 
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow( "Julia" ); 
    GLInit(); 
    glutDisplayFunc( displayF ); 
    glutReshapeFunc( updateSize );
    glutIdleFunc( glutPostRedisplay ); 
    glutKeyboardFunc( keyboardF );
    glutSpecialFunc( specialKeyF );
    glutMainLoop();

    return 0;
}
