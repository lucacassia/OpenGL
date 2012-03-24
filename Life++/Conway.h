#ifndef CONWAY
#define CONWAY

#include<cstdlib>

class Conway
{
  private:
    int w,h;
  public:
    bool *bitmap;

    Conway(int,int);
    Conway(const Conway& arg);
    ~Conway();
    int width()const;
    int height()const;
    Conway operator++(int);
};

#endif
