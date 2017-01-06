#include"../lib/complex.h"
#include <vector>

/* modes Norm = 0
         Conj = 1
          Abs = 2
         Iabs = 3
*/

class Julia{

    complex c;
    complex focus;
    double zoom;
    double shift;
    double div;
    int jexp;
    int halt;
    int mode;
    int width;
    int height;

  public:

    std::vector<unsigned int> pixels;

    Julia();
    ~Julia();
    int W()const;
    int H()const;
    Julia operator ++(int);
    Julia operator --(int);
    void left();
    void right();
    void up();
    void down();
    void w();
    void a();
    void s();
    void d();
    void resetView();
    void updateSize(int,int);
    unsigned innerLoop(int,int);
    void compute();
};

