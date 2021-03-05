// Duwal, Kritan
// kxd8042
// 2020-10-08
//----------------------------------------------------------------
#include <stdio.h>

#include "matrix.h"
#include "transforms.h"

//----------------------------------------------------------------
#define XMIN       (-16.0)
#define XMAX       ( 16.0)
#define YMIN       (- 9.0)
#define YMAX       (  9.0)

#define SCALE      (60)
#define WIDTH      (SCALE*16)
#define HEIGHT     (SCALE* 9)

#define CAMERAPOS  (22.33)

//----------------------------------------------------------------
int main()
{
  //	    Separate your Student ID into ten digits and put those
  //        digits in the points p1, p2, and p3 BACKWARDS as their
  //        x, y, z coordintates.  KEEP ALL THREE OF THE w
  //        COORDINATES 1!  So, if your student ID happened to be
  //        1001668042, the points would be as follows,

  Point p1 = { 2, 4, 0, 1 };
  Point p2 = { 8, 6, 6, 1 };
  Point p3 = { 1, 0, 0, 1 };

  Point pTmp;
  Point pTmp2;
  Point pTmp3;

  Matrix rotX;
  Matrix rotY;
  Matrix rotZ;
  Matrix rotZYX;

  Matrix mTmp;

  rotateXMatrix( DEG2RAD(  60.0 ), rotX );
  rotateYMatrix( DEG2RAD( -30.0 ), rotY );
  rotateZMatrix( DEG2RAD(  45.0 ), rotZ );

  multMatrix( rotX, rotY, mTmp );
  multMatrix( mTmp, rotZ, rotZYX );

  printf( "//-- Projection parameters --------------------------------------\n" );
  printf( "XMIN, XMAX    : ( %g, %g )\n", XMIN, XMAX );
  printf( "YMIN, YMAX    : ( %g, %g )\n", YMIN, YMAX );
  printf( "WIDTH, HEIGHT : ( %d, %d )\n", WIDTH, HEIGHT );
  printf( "CAMERAPOS     : %g\n", CAMERAPOS );

  Matrix parMatrix;
  Matrix perMatrix;

  projectParallelMatrix( XMIN, XMAX, YMIN, YMAX, WIDTH, HEIGHT, parMatrix );
  projectPerspectiveMatrix( CAMERAPOS, XMIN, XMAX, YMIN, YMAX, WIDTH, HEIGHT, perMatrix );

  printf( "\n//-- p1 ---------------------\n" );
  printf( "Original    : " ); dumpPointT( p1 );
  multMatrixPoint( rotZYX, p1, pTmp );
  printf( "Rotated     : " ); dumpPointT( pTmp );
  multMatrixPoint( parMatrix, pTmp, pTmp2 );
  printf( "Parallel    : " ); dumpPointT( pTmp2 );
  multMatrixPoint( perMatrix, pTmp, pTmp2 );
  printf( "Perspective : " ); dumpPointT( pTmp2 );
  normalizePoint( pTmp2, pTmp3 );
  printf( "Normalized  : " ); dumpPointT( pTmp3 );

  printf( "\n//-- p2 ---------------------\n" );
  printf( "Original    : " ); dumpPointT( p2 );
  multMatrixPoint( rotZYX, p2, pTmp );
  printf( "Rotated     : " ); dumpPointT( pTmp );
  multMatrixPoint( parMatrix, pTmp, pTmp2 );
  printf( "Parallel    : " ); dumpPointT( pTmp2 );
  multMatrixPoint( perMatrix, pTmp, pTmp2 );
  printf( "Perspective : " ); dumpPointT( pTmp2 );
  normalizePoint( pTmp2, pTmp3 );
  printf( "Normalized  : " ); dumpPointT( pTmp3 );

  printf( "\n//-- p3 ---------------------\n" );
  printf( "Original    : " ); dumpPointT( p3 );
  multMatrixPoint( rotZYX, p3, pTmp );
  printf( "Rotated     : " ); dumpPointT( pTmp );
  multMatrixPoint( parMatrix, pTmp, pTmp2 );
  printf( "Parallel    : " ); dumpPointT( pTmp2 );
  multMatrixPoint( perMatrix, pTmp, pTmp2 );
  printf( "Perspective : " ); dumpPointT( pTmp2 );
  normalizePoint( pTmp2, pTmp3 );
  printf( "Normalized  : " ); dumpPointT( pTmp3 );

  printf( "\n//---------------------------------------------------------------\n" );
}

//----------------------------------------------------------------
