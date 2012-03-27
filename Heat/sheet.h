#include <vector>

class sheet
{
    int width,height;
    std::vector<double> pixels,old;

  public:

    sheet(int,int);
    ~sheet();
    int w()const;
    int h()const;
    int size()const;
    double& operator[](int);
    const double& operator[](int)const;
    double compute(int);
    void operator++(int);
};

