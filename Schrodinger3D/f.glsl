varying vec4 graph_coord;
uniform vec4 color;
uniform sampler2D mytexture;

void main(void) {
    float x = texture2D(mytexture, graph_coord.xy / 2.0 + 0.5).z - 0.5;
    vec4 fragment_color;
    if(x<0) x += -floor(x)+1;
    if(x>=1) x -= floor(x);
    if(0<= x && x<1.0/6.0){
        fragment_color.r = 0;
        fragment_color.g = 6*x;
        fragment_color.b = 1;
    }
    if(1.0/6.0<= x && x<2.0/6.0){
        fragment_color.r = 0;
        fragment_color.g = 1;
        fragment_color.b = 2-6*x;
    }
    if(2.0/6.0<= x && x<3.0/6.0){
        fragment_color.r = 6*x-2;
        fragment_color.g = 1;
        fragment_color.b = 0;
    }
    if(3.0/6.0<= x && x<4.0/6.0){
        fragment_color.r = 1;
        fragment_color.g = 4-6*x;
        fragment_color.b = 0;
    }
    if(4.0/6.0<= x && x<5.0/6.0){
        fragment_color.r = 1;
        fragment_color.g = 0;
        fragment_color.b = 6*x-4;
    }
    if(5.0/6.0<= x && x<6.0/6.0){
        fragment_color.r = 6-6*x;
        fragment_color.g = 0;
        fragment_color.b = 1;
    }
    fragment_color.a = 1;

  float factor;

  if(gl_FrontFacing)
	  factor = 1.0;
  else
	  factor = 0.5;

  gl_FragColor = fragment_color.rgba * color * factor;
}
