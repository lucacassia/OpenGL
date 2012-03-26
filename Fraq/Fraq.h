#include"../lib/complex.h"
#include <vector>

class Fraq{

    complex focus;
    double zoom;
    double shift;
    int width;
    int height;
    std::vector<double> pixels;

  public:

    Fraq();
    ~Fraq();
    int w() const;
    int h() const;
    int size() const;
    double & operator [] (int);
    const double & operator [] (int) const;
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

