// Duwal, Kritan
// kxd8042
// 2020-10-08
//----------------------------------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "transforms.h"

//----------------------------------------------------------------
void normalizePoint( Point p1, Point p2 )
{
  if(p1[3] == 0) //if p1's w = 0 then p1 is not a point but a vector
  {
    fprintf(stderr, "w of p1 is 0, p1 is not a point!\n");
    exit(1);
  }
  // compute norm, n
  FLOAT64 n = sqrt( (p1[0]*p1[0]) + (p1[1]*p1[1]) + (p1[2]*p1[2]) + (p1[3]*p1[3]) );
  //normalize point and store into p2
  p2[0] = p1[0] / n;
  p2[1] = p1[1] / n;
  p2[2] = p1[2] / n;
  p2[3] = p1[3] / n;
  //check if w = 1
  if(p2[3] != 1) 
  {
    p2[0] = p2[0] / p2[3];
    p2[1] = p2[1] / p2[3];
    p2[2] = p2[2] / p2[3];
    p2[3] = p2[3] / p2[3];
  }
}

//----------------------------------------------------------------
void projectParallelMatrix(
  FLOAT64 xmin, FLOAT64 xmax,
  FLOAT64 ymin, FLOAT64 ymax,
  int width, int height,
  Matrix xform )
{
  if(xmin == xmax) //if xmin-xmax = 0
  {
    fprintf(stderr, "xmin = xmax, cannot divide by zero.!\n");
    exit(1);
  }
  if(ymin == ymax) //if ymin-ymax = 0
  {
    fprintf(stderr, "ymin = ymax, cannot divide by zero.!\n");
    exit(1);
  }
  FLOAT64 sx = (width - 1) / (xmax - xmin);
  FLOAT64 sy = (height -1) / (ymax - ymin);
  FLOAT64 tx = -xmin;
  FLOAT64 ty = -ymin;

  copyMatrix( I4, xform );

  xform[0][0] = sx; 
  xform[1][1] = sy;
  xform[0][3] = sx * tx;
  xform[1][3] = sy * ty;
  xform[2][2] = 0;
}

void projectPerspectiveMatrix(
  FLOAT64 d,
  FLOAT64 xmin, FLOAT64 xmax,
  FLOAT64 ymin, FLOAT64 ymax,
  int width, int height,
  Matrix xform )
{
  if(xmin == xmax) //if xmin-xmax = 0
  {
    fprintf(stderr, "xmin = xmax, cannot divide by zero.!\n");
    exit(1);
  }
  if(ymin == ymax) //if ymin-ymax = 0
  {
    fprintf(stderr, "ymin = ymax, cannot divide by zero.!\n");
    exit(1);
  }
  if(d == 0) //if d = 0
  {
    fprintf(stderr, "d = 0, cannot divide by zero.!\n");
    exit(1);
  }
  FLOAT64 sx = (width - 1) / (xmax - xmin);
  FLOAT64 sy = (height -1) / (ymax - ymin);
  FLOAT64 tx = -xmin;
  FLOAT64 ty = -ymin;

  copyMatrix( I4, xform );

  xform[0][0] = sx; 
  xform[1][1] = sy;
  xform[0][3] = sx * tx;
  xform[1][3] = sy * ty;
  xform[2][2] = 0;
  xform[0][2] = sx * tx / d * -1;
  xform[1][2] = sy * ty / d * -1;
  xform[3][2] = -1 / d;
}

//----------------------------------------------------------------
void rotateXMatrix( FLOAT64 theta, Matrix xform )
{
  copyMatrix( I4, xform );

  double cth = cos( theta );
  double sth = sin( theta );

  xform[1][1] =  cth;
  xform[1][2] = -sth;
  xform[2][1] =  sth;
  xform[2][2] =  cth;
}

void rotateYMatrix( FLOAT64 theta, Matrix xform )
{
  copyMatrix( I4, xform );

  double cth = cos( theta );
  double sth = sin( theta );

  xform[0][0] =  cth;
  xform[0][2] =  sth;
  xform[2][0] = -sth;
  xform[2][2] =  cth;
}

void rotateZMatrix( FLOAT64 theta, Matrix xform )
{
  copyMatrix( I4, xform );

  double cth = cos( theta );
  double sth = sin( theta );

  xform[0][0] =  cth;
  xform[0][1] = -sth;
  xform[1][0] =  sth;
  xform[1][1] =  cth;
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

void shearXYZMatrix(
  FLOAT64 hxy, FLOAT64 hxz,
  FLOAT64 hyx, FLOAT64 hyz,
  FLOAT64 hzx, FLOAT64 hzy,
  Matrix xform )
{
  copyMatrix( I4, xform );

  xform[0][1] = hxy;
  xform[0][2] = hxz;
  xform[1][0] = hyx;
  xform[1][2] = hyz;
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
