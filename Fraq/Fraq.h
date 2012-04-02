#include"../lib/complex.h"
#include <vector>

class Fraq{

    complex focus;
    long double zoom;
    long double shift;
    int width;
    int height;
    std::vector<long double> pixels;

  public:

    Fraq();
    ~Fraq();
    int w() const;
    int h() const;
    int size() const;
    long double & operator [] (int);
    const long double & operator [] (int) const;
    Fraq operator ++ (int);
    Fraq operator -- (int);
    void left();
    void right();
    void up();
    void down();
    void resetView();
    void updateSize(int,int);
    void innerLoop(int,int);
    void compute();
};

