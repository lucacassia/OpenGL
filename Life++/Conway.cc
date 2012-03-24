#include"Conway.h"

Conway::Conway(int width,int height)
{
    w = width;
    h = height;
    bitmap = (bool*)malloc(sizeof(bool)*w*h);
    for(int k = 0; k < w*h; k++)
        bitmap[k] = false;
}

Conway::Conway(const Conway& arg)
{
    w = arg.width();
    h = arg.height();
    bitmap = (bool*)malloc(sizeof(bool)*w*h);
    for(int k = 0; k < w*h; k++)
        bitmap[k] = arg.bitmap[k];
}

Conway::~Conway()
{
    free(bitmap);
}

int Conway::width()const
{
    return w;
}

int Conway::height()const
{
    return h;
}

Conway Conway::operator++(int)
{
    Conway clone(*this);
    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++)
        {
            int tmp1 = 0, tmp2 = 0;
            if(j+1 < w) tmp1 += clone.bitmap[(i)*w+(j+1)];
            if(j-1 >= 0) tmp1 += clone.bitmap[(i)*w+(j-1)];
            if(i+1 < h) tmp1 += clone.bitmap[(i+1)*w+(j)];
            if(i-1 >= 0) tmp1 += clone.bitmap[(i-1)*w+(j)];
            if(i+1 < h && j+1 < w) tmp1 += clone.bitmap[(i+1)*w+(j+1)];
            if(i-1 >= 0 && j-1 >= 0) tmp1 += clone.bitmap[(i-1)*w+(j-1)];
            if(i+1 < h && j-1 >= 0) tmp1 += clone.bitmap[(i+1)*w+(j-1)];
            if(i-1 >= 0 && j+1 < w) tmp1 += clone.bitmap[(i-1)*w+(j+1)];

            if(j+2 < w) tmp2 += clone.bitmap[(i)*w+(j+2)];
            if(j-2 >= 0) tmp2 += clone.bitmap[(i)*w+(j-2)];
            if(i+2 < h) tmp2 += clone.bitmap[(i+2)*w+(j)];
            if(i-2 >= 0) tmp2 += clone.bitmap[(i-2)*w+(j)];

            if(i+2 < h && j+2 < w) tmp2 += clone.bitmap[(i+1)*w+(j+1)];
            if(i+1 < h && j+2 < w) tmp2 += clone.bitmap[(i+1)*w+(j+1)];
            if(i+2 < h && j+1 < w) tmp2 += clone.bitmap[(i+1)*w+(j+1)];

            if(i-2 >= 0 && j-2 >= 0) tmp2 += clone.bitmap[(i-2)*w+(j-2)];
            if(i-1 >= 0 && j-2 >= 0) tmp2 += clone.bitmap[(i-1)*w+(j-2)];
            if(i-2 >= 0 && j-1 >= 0) tmp2 += clone.bitmap[(i-2)*w+(j-1)];

            if(i+2 < h && j-2 >= 0) tmp2 += clone.bitmap[(i+2)*w+(j-2)];
            if(i+1 < h && j-2 >= 0) tmp2 += clone.bitmap[(i+1)*w+(j-2)];
            if(i+2 < h && j-1 >= 0) tmp2 += clone.bitmap[(i+2)*w+(j-1)];

            if(i-2 >= 0 && j+2 < w) tmp2 += clone.bitmap[(i-2)*w+(j+2)];
            if(i-1 >= 0 && j+2 < w) tmp2 += clone.bitmap[(i-1)*w+(j+2)];
            if(i-2 >= 0 && j+1 < w) tmp2 += clone.bitmap[(i-2)*w+(j+1)];

            if(tmp1 == 4 || tmp1 == 5)
                bitmap[i*w+j] = true;
            if(tmp1 < 2 || tmp1 > 5)
                bitmap[i*w+j] = false;
        }
    return *this;
}

