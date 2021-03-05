// Duwal, Kritan
// kxd8042
// 2020-11-27
//----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include "pipeline.h"

//----------------------------------------------------------------
void backfaceCull( Parameters *paramsP, int noisy )
{
  printf( "//-------------------- Backface Culling --\n" );

  int numInvisible = 0;

  // Get pointers to the Point and Face arrays to make them
  //  easier to access.
  Point *p = paramsP->points;
  Face  *f = paramsP->faces;

  // Construct the x, y, z of the camera position using the
  //  viewpoint in paramsP.  Remember that when the viewpoint is
  //  0.0 the projection is Parallel and the camera position must
  //  approach infinity!  (Use DBL_MAX.)

  FLOAT64 cameraPosX = 0;
  FLOAT64 cameraPosY = 0;
  FLOAT64 cameraPosZ;
  if(paramsP->viewpoint == 0) cameraPosZ = DBL_MAX;
  else cameraPosZ = paramsP->viewpoint;

  if ( noisy ) {
    printf(
      "//   cameraPos  : ( %14.6e, %14.6e, %14.6e )\n",
      cameraPosX, cameraPosY, cameraPosZ );
  }

  // Loop over all of the faces, properly setting the face's
  //  invisible bit.

  for ( int i=0; i<paramsP->numFaces; i++ ) {
    if ( noisy ) {
      printf(
        "//----------------------------------------\n"
        "//   %d.  f %3d %3d %3d\n",
        i, f[i].v1, f[i].v2, f[i].v3 );
    }

    // Compute the a vector: the face's v2 minus the face's v1.

    FLOAT64 ax = p[f[i].v2][0] -  p[f[i].v1][0];
    FLOAT64 ay = p[f[i].v2][1] -  p[f[i].v1][1];
    FLOAT64 az = p[f[i].v2][2] -  p[f[i].v1][2];

    if ( noisy ) {
      printf( "//     a vector          : ( %14.6e, %14.6e, %14.6e )\n",
        ax, ay, az );
    }

    // Compute the b vector: the face's v3 minus the face's v1.

    FLOAT64 bx = p[f[i].v3][0] -  p[f[i].v1][0];
    FLOAT64 by = p[f[i].v3][1] -  p[f[i].v1][1];
    FLOAT64 bz = p[f[i].v3][2] -  p[f[i].v1][2];

    if ( noisy ) {
      printf( "//     b vector          : ( %14.6e, %14.6e, %14.6e )\n",
        bx, by, bz );
    }

    // Compute the surface normal sn = a × b.

    FLOAT64 snx = (ay *bz) - (az * by);
    FLOAT64 sny = (-1)*((ax *bz) - (az * bx));
    FLOAT64 snz = (ax *by) - (ay * bx);

    if ( noisy ) {
      printf( "//     surface normal    : ( %14.6e, %14.6e, %14.6e )\n",
        snx, sny, snz );
    }

    // Compute the toTriangle vector, toT: the face's v1 - the camera
    //  position.

    FLOAT64 toTx = p[f[i].v1][0] - cameraPosX;
    FLOAT64 toTy = p[f[i].v1][1] - cameraPosY;
    FLOAT64 toTz = p[f[i].v1][2] - cameraPosZ;

    if ( noisy ) {
      printf( "//     toTriangle vector : ( %14.6e, %14.6e, %14.6e )\n",
        toTx, toTy, toTz );
    }

    // Compute the cos of the angle: sn · toT.

    FLOAT64 dotProd = (snx * toTx) + (sny * toTy) + (snz * toTz);

    if ( noisy ) {
      printf( "//     dot product       : %14.6e\n",
        dotProd );
    }

    // Set the face's invisible bit.  (1 when the face is
    //  invisible, 0 when the face is visible.)

    if(dotProd < 0) f[i].invisible = 0;
    else f[i].invisible = 1;

    // We count how many faces were made invisible.

    if(f[i].invisible == 1) numInvisible++;

    if ( noisy ) {
      printf( "//   %d.  f %3d %3d %3d  ->   %d (%svisible)\n",
        i, f[i].v1, f[i].v2, f[i].v3, f[i].invisible,
        f[i].invisible ? "in" : "" );
    }
  }

  if ( noisy ) {
    printf( "//----------------------------------------\n" );
  }

  printf( "//   %d face%s in model, %d %s invisible.\n",
    paramsP->numFaces, paramsP->numFaces == 1 ? "" : "s",
    numInvisible, numInvisible == 1 ? "is" : "are" );
}

//----------------------------------------------------------------
