#include <vector>

class sheet{

    int width,height;

  public:

    std::vector<double> pixels,old;

    sheet(int,int);
    ~sheet();
    int w()const;
    int h()const;
    void compute();
};

