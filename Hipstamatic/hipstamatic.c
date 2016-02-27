#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <GL/freeglut.h>

typedef struct{
    unsigned char * raw_image;
    int image_width;
    int image_height;
    int num_components;
    int jpeg_color_space;
}image_t;

typedef struct{
    float * raw_image;
    int image_width;
    int image_height;
}image_HSB_t;

image_t read_JPEG_file( char *filename )
{

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPROW row_pointer[1];
    
    FILE *infile = fopen( filename, "rb" );
    unsigned long location = 0;
    int i;

    image_t tmp = {NULL,0,0,0,0};

    if( !infile ){
        printf("Error opening jpeg file %s\n!", filename );
        return tmp;
    }

    cinfo.err = jpeg_std_error( &jerr );

    jpeg_create_decompress( &cinfo );

    jpeg_stdio_src( &cinfo, infile );

    jpeg_read_header( &cinfo, TRUE );

    tmp.image_width = cinfo.image_width;
    tmp.image_height = cinfo.image_height;
    tmp.num_components = cinfo.num_components;
    tmp.jpeg_color_space = cinfo.jpeg_color_space;

    jpeg_start_decompress( &cinfo );

    tmp.raw_image = (unsigned char *) malloc( cinfo.output_width * cinfo.output_height * cinfo.num_components * sizeof(unsigned char) );

    row_pointer[0] = (unsigned char *) malloc( cinfo.output_width * cinfo.num_components );

    while( cinfo.output_scanline < cinfo.image_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i = 0; i < cinfo.image_width * cinfo.num_components; i++)
            tmp.raw_image[location++] = row_pointer[0][i];
    }

    jpeg_finish_decompress( &cinfo );
    jpeg_destroy_decompress( &cinfo );
    free( row_pointer[0] );
    fclose( infile );

    return tmp;
}

int write_JPEG_file( image_t obj, char *filename )
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    
    JSAMPROW row_pointer[1];

    FILE * outfile = fopen( filename, "wb" );
    
    if( !outfile ){
        printf("Error opening output jpeg file %s\n!", filename );
        return 1;
    }

    cinfo.err = jpeg_std_error( &jerr );
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = obj.image_width;    
    cinfo.image_height = obj.image_height;
    cinfo.input_components = obj.num_components;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults( &cinfo );

    jpeg_start_compress( &cinfo, TRUE );

    while( cinfo.next_scanline < cinfo.image_height )
    {
        row_pointer[0] = &obj.raw_image[ cinfo.next_scanline * cinfo.image_width * cinfo.input_components];
        jpeg_write_scanlines( &cinfo, row_pointer, 1 );
    }

    jpeg_finish_compress( &cinfo );
    jpeg_destroy_compress( &cinfo );
    fclose( outfile );

    return 0;
}

unsigned char truncate_color(float c)
{
    int tmp = (int) c;
    if(tmp >= 255) return 255;
    if(tmp <= 0) return 0;
    return (unsigned char) tmp;
}

void imageGreyscale(image_t image)
{
    int i; int sum;
    int image_size = image.image_width * image.image_height;
    for(i = 0; i < image_size; i++){
        sum = image.raw_image[3*i+0] + image.raw_image[3*i+1] + image.raw_image[3*i+2];
        image.raw_image[3*i+0] = sum / 3;
        image.raw_image[3*i+1] = sum / 3;
        image.raw_image[3*i+2] = sum / 3;
    }
}

void imageAdjustContrast(image_t image, int c)
{
    int i;
    int buffer_size = image.image_width * image.image_height * image.num_components;
    float f = ( 259.0 * ( c + 255 ) ) / ( ( 259 - c ) * 255.0 );
    for(i = 0; i < buffer_size; i++)
        image.raw_image[i] = truncate_color( f * ( image.raw_image[i] - 128 ) + 128 );
}

void imageAdjustBrightness(image_t image, int b)
{
    int i;
    int buffer_size = image.image_width * image.image_height * image.num_components;
    for(i = 0; i < buffer_size; i++)
        image.raw_image[i] = truncate_color( image.raw_image[i] + b );
}

void imageAdd(image_t image, float r, float g, float b)
{
    int i;
    int buffer_size = image.image_width * image.image_height * image.num_components;
    for(i = 0; i < buffer_size; i++){
        if(i%3 == 0) image.raw_image[i] = truncate_color ( image.raw_image[i] * r );
        if(i%3 == 1) image.raw_image[i] = truncate_color ( image.raw_image[i] * g );
        if(i%3 == 2) image.raw_image[i] = truncate_color ( image.raw_image[i] * b );
    }
}

image_HSB_t imageRGBtoHSB(image_t image)
{
    image_HSB_t converted_image;
    converted_image.image_width = image.image_width;
    converted_image.image_height = image.image_height;
    converted_image.raw_image = NULL;

    if(image.jpeg_color_space != JCS_RGB) return converted_image;

    int image_size = image.image_width * image.image_height;
    converted_image.raw_image = (float *) malloc( 3 * image_size * sizeof(float) );

    int component;
    for(component = 0; component < image_size; component++){

        float hue, saturation, brightness;
        int r = image.raw_image[3*component+0];
        int g = image.raw_image[3*component+1];
        int b = image.raw_image[3*component+2];

        int cmax = (r > g) ? r : g;
        if (b > cmax) cmax = b;
        int cmin = (r < g) ? r : g;
        if (b < cmin) cmin = b;

        brightness = ((float) cmax) / 255.0f;
        if (cmax != 0)
            saturation = ((float) (cmax - cmin)) / ((float) cmax);
        else
            saturation = 0;
        if (saturation == 0)
            hue = 0;
        else{
            float redc = ((float) (cmax - r)) / ((float) (cmax - cmin));
            float greenc = ((float) (cmax - g)) / ((float) (cmax - cmin));
            float bluec = ((float) (cmax - b)) / ((float) (cmax - cmin));
            if (r == cmax)
                hue = bluec - greenc;
            else if (g == cmax)
                hue = 2.0f + redc - bluec;
            else
                hue = 4.0f + greenc - redc;
            hue = hue / 6.0f;
            if (hue < 0)
                hue = hue + 1.0f;
        }

        converted_image.raw_image[3*component+0] = hue;
        converted_image.raw_image[3*component+1] = saturation;
        converted_image.raw_image[3*component+2] = brightness;
    }

    return converted_image;
}

image_t imageHSBtoRGB(image_HSB_t image_HSB)
{
    int image_size = image_HSB.image_width * image_HSB.image_height;

    image_t converted_image;
    converted_image.raw_image = (unsigned char *) malloc( 3 * image_size * sizeof(unsigned char) );
    converted_image.image_width = image_HSB.image_width;
    converted_image.image_height = image_HSB.image_height;
    converted_image.num_components = 3;
    converted_image.jpeg_color_space = JCS_RGB;

    int component;
    for(component = 0; component < image_size; component++){

        int r = 0, g = 0, b = 0;
        float hue = image_HSB.raw_image[3*component+0];
        float saturation = image_HSB.raw_image[3*component+1];
        float brightness = image_HSB.raw_image[3*component+2];

        if(saturation == 0) {
            r = g = b = (int) (brightness * 255.0f + 0.5f);
        }else{
            float h = (hue - (float)floor(hue)) * 6.0f;
            float f = h - (float)floor(h);
            float p = brightness * (1.0f - saturation);
            float q = brightness * (1.0f - saturation * f);
            float t = brightness * (1.0f - (saturation * (1.0f - f)));
            switch ((int) h) {
                case 0:
                    r = (int) (brightness * 255.0f + 0.5f);
                    g = (int) (t * 255.0f + 0.5f);
                    b = (int) (p * 255.0f + 0.5f);
                    break;
                case 1:
                     r = (int) (q * 255.0f + 0.5f);
                     g = (int) (brightness * 255.0f + 0.5f);
                     b = (int) (p * 255.0f + 0.5f);
                     break;
                case 2:
                     r = (int) (p * 255.0f + 0.5f);
                     g = (int) (brightness * 255.0f + 0.5f);
                     b = (int) (t * 255.0f + 0.5f);
                     break;
                case 3:
                     r = (int) (p * 255.0f + 0.5f);
                     g = (int) (q * 255.0f + 0.5f);
                     b = (int) (brightness * 255.0f + 0.5f);
                     break;
                case 4:
                     r = (int) (t * 255.0f + 0.5f);
                     g = (int) (p * 255.0f + 0.5f);
                     b = (int) (brightness * 255.0f + 0.5f);
                     break;
                case 5:
                     r = (int) (brightness * 255.0f + 0.5f);
                     g = (int) (p * 255.0f + 0.5f);
                     b = (int) (q * 255.0f + 0.5f);
                     break;
            }
        }

        converted_image.raw_image[3*component+0] = r;
        converted_image.raw_image[3*component+1] = g;
        converted_image.raw_image[3*component+2] = b;
    }

    return converted_image;
}

image_t img;
image_t original;

void GLInit()
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f ,0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    gluOrtho2D(0.0, 2 * img.image_width, 0.0, img.image_height);
}

void displayF()
{
    int row_index;
    for(row_index = 0; row_index < img.image_height; row_index++){
        glRasterPos2i(0, img.image_height - row_index - 1);
        glDrawPixels(img.image_width, 1, GL_RGB, GL_UNSIGNED_BYTE, &original.raw_image[3 * row_index * img.image_width]);
        glRasterPos2i(img.image_width, img.image_height - row_index - 1);
        glDrawPixels(img.image_width, 1, GL_RGB, GL_UNSIGNED_BYTE, &img.raw_image[3 * row_index * img.image_width]);
    }
    glutSwapBuffers();
}

void idleF(){}

void keyboardF(unsigned char key, int mouseX, int mouseY)
{
    image_HSB_t converted;

    switch(key)
    {
        case 'p': case 'P':
            write_JPEG_file(img, "out.jpg" );
            break;
        case 'q': case 'Q': case 27:
            free(img.raw_image);
            exit(EXIT_SUCCESS);
        case '+':
            imageAdjustContrast(img, 1);
            glutPostRedisplay();
            break;
        case '-':
            imageAdjustContrast(img, -1);
            glutPostRedisplay();
            break;
        case '*':
            imageAdjustBrightness(img, 1);
            glutPostRedisplay();
            break;
        case '_':
            imageAdjustBrightness(img, -1);
            glutPostRedisplay();
            break;
        case 'a':
            imageAdd(img, 1.25, 1.25, 0.9);
            glutPostRedisplay();
            break;
        case 'g':
            imageGreyscale(img);
            glutPostRedisplay();
            break;
        case 'h':
            imageAdjustContrast(img, 24);
            imageAdd(img, 1.25, 1.25, 0.9);
            converted = imageRGBtoHSB(img);
            free(img.raw_image);
            img = imageHSBtoRGB(converted);
            free(converted.raw_image);
            glutPostRedisplay();
            break;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    img = read_JPEG_file(argv[1]);
    img.jpeg_color_space = JCS_RGB;
    original = read_JPEG_file(argv[1]);

    printf( "JPEG File Information: \n" );
    printf( "Image width: %d pixels\n", img.image_width );
    printf( "Image height: %d pixels\n", img.image_height );
    printf( "Color components per pixel: %d.\n", img.num_components );
    printf( "Color space: %d.\n", img.jpeg_color_space );

    glutInitWindowSize(2 * img.image_width, img.image_height); 
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow("Hipstamatic"); 
    GLInit();
    glutDisplayFunc(displayF); 
    glutIdleFunc(idleF);
    glutKeyboardFunc(keyboardF);
    glutMainLoop();
    return 0;
}
