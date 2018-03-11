#ifndef TYPETRANSFORM_H
#define TYPETRANSFORM_H
#include"S7/snap7.h"
#include"iostream"
using namespace std;
typedef unsigned char uchar;
class TypeTransform
{
public:
    TypeTransform();
    float ByteToNum(byte* buffer, int start, int length);
    void NumToByte(byte* buffer, int start,double set);
    void NumToByte(byte* buffer,int start,int set);
};

#endif // TYPETRANSFORM_H
