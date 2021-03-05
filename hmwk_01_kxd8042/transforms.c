// Duwal, Kritan
// kxd8042
// 2020-10-02
//----------------------------------------------------------------
#include <math.h>
#include <stdio.h>

#include "transforms.h"

//----------------------------------------------------------------
void rotateXMatrix( FLOAT64 theta, Matrix xform )
{
  copyMatrix( I4, xform );

  xform[1][1] = cos(theta);
  xform[1][2] = -sin(theta);
  xform[2][1] = sin(theta);
  xform[2][2] = cos(theta);
}

void rotateYMatrix( FLOAT64 theta, Matrix xform )
{
  copyMatrix( I4, xform );

  xform[0][0] = cos(theta);
  xform[0][2] = sin(theta);
  xform[2][0] = -sin(theta);
  xform[2][2] = cos(theta);
}

void rotateZMatrix( FLOAT64 theta, Matrix xform )
{
  copyMatrix( I4, xform );

  xform[0][0] = cos(theta);
  xform[0][1] = -sin(theta);
  xform[1][0] = sin(theta);
  xform[1][1] = cos(theta);
}

//----------------------------------------------------------------
void scaleMatrix( FLOAT64 sx, FLOAT64 sy, FLOAT64 sz, Matrix xform )
{
  copyMatrix( I4, xform );

  xform[0][0] = sx;
  xform[1][1] = sy;
  xform[2][2] = sz;
}

//----------------------------------------------------------------
void shearXMatrix( FLOAT64 hxy, FLOAT64 hxz, Matrix xform )
{
  copyMatrix( I4, xform );

  xform[0][1] = hxy;
  xform[0][2] = hxz;
}

void shearYMatrix( FLOAT64 hyx, FLOAT64 hyz, Matrix xform )
{
  copyMatrix( I4, xform );

  xform[1][0] = hyx;
  xform[1][2] = hyz;

}

void shearZMatrix( FLOAT64 hzx, FLOAT64 hzy, Matrix xform )
{
  copyMatrix( I4, xform );

  xform[2][0] = hzx;
  xform[2][1] = hzy;

}

//----------------------------------------------------------------
void translateMatrix( FLOAT64 tx, FLOAT64 ty, FLOAT64 tz, Matrix xform )
{
  copyMatrix( I4, xform );

  xform[0][3] = tx;
  xform[1][3] = ty;
  xform[2][3] = tz;
}

//----------------------------------------------------------------
