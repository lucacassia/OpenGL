#include"../lib/complex.h"
#include <vector>

class Fraq{

    complex focus;
    double zoom;
    double shift;
    int width;
    int height;

  public:

    std::vector<short> pixels;

    Fraq();
    ~Fraq();
    int W()const;
    int H()const;
    Fraq operator ++(int);
    Fraq operator --(int);
    void left();
    void right();
    void up();
    void down();
    void resetView();
    void updateSize(int,int);
    short innerLoop(int,int);
    void compute();
};

