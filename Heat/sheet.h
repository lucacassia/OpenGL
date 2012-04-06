#include <vector>
#include <pthread.h>

class sheet
{
    int width,height;
    std::vector<double> pixels,old,source;
    pthread_t ids[4];

  public:

    sheet(int,int);
    sheet(const sheet&);
    ~sheet();
    int w()const;
    int h()const;
    int size()const;
    pthread_t id(int)const;
    double& operator[](int);
    const double& operator[](int)const;
    double compute(int);
    void operator++(int);
    sheet& operator =(const sheet&);

};

