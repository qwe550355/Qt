#include "TypeTransform.h"
#include"QDebug"
typedef unsigned char uchar;
//---------------------------------------------------------------------------
TypeTransform::TypeTransform()
{

}
//---------------------------------------------------------------------------
float TypeTransform::ByteToNum(byte* buffer, int start, int length)
{
    if (2 == length)
    {
        unsigned char b1 = buffer[start];
        unsigned char b2 = buffer[start + 1];
        unsigned short num1 = 0;
        num1 = (b1 << 8) + b2;
        return (double)num1;
    }
    else if(4 == length)
    {
        union   conv
          {
               float f;
               byte  b[4];
          }   c;
        c.b[3]=buffer[start];
        c.b[2]=buffer[start+1];
        c.b[1]=buffer[start+2];
        c.b[0]=buffer[start+3];
        double x=(double)c.f;
        return x;
    }
}
//---------------------------------------------------------------------------
void TypeTransform::NumToByte(byte* buffer, int start,double set)
{
    union  conv
      {
        float f;
        byte  b[4];
      }c;
    c.f=(float)set;
    for(uchar i=0;i<4;i++)
    {
        // qDebug()<<c.b[i];
        buffer[start + 3 - i]=c.b[i];
    }
}
//---------------------------------------------------------------------------
void TypeTransform::NumToByte(byte* buffer, int start,int set)
{


    union MyUnion_1
    {
        unsigned short n1;
        struct MyStruct_1
        {
            unsigned char num_l;
            unsigned char num_h;
        }half1;
    }num1;

    num1.n1 = set;

    buffer[ start + 1 ] = num1.half1.num_l;
    buffer[ start ] = num1.half1.num_h;
}
