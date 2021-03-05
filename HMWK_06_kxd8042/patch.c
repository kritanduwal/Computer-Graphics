// Duwal, Kritan
// kxd8042
// 2020-12-07
//----------------------------------------------------------------
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "pipeline.h"

static FLOAT64 computeCoefficient( int i, int j, FLOAT64 u, FLOAT64 v );
static void    computePatch( Parameters *paramsP, int p );
static void    displayPatch( FILE *fp, Parameters *paramsP );
static void    generatePatchFaces( Parameters *paramsP );

//----------------------------------------------------------------
void generatePatches( FILE *fp, Parameters *paramsP )
{
  // Generate the faces for the surface (patch).  We use the same
  //  face list for all surfaces (read the comment below for more
  //  info.)
  generatePatchFaces( paramsP );

  // For each Bézier surface (patch) ...
  for ( int p=0; p<paramsP->numPatches; p++ ) {
    // ... compute the points that make up the surface ...
    computePatch( paramsP, p );

    // ... and then display the triangles of the surface.
    displayPatch( fp, paramsP );
  }
}

//----------------------------------------------------------------
// Creates a set of faces that can be used to describe *every*
//  Bézier surface generated during this run.
//
// Think about it for a moment.  Every surface is generated with
//  the same resolution so every surface will have the same
//  number of generated points.
//
// The faces (triangles) of a Bézier surface are generated in a
//  regular pattern based on how many points across and how many
//  points down the surface is.
//
// For resolution r, the first face *always* uses points 0, r,
//  and r+1.  The second face *always* uses points r+1, 1, and 0.
//  The third face *always* uses points 1, r+1, and r+2.  The
//  fourth face *always* uses points r+2, 2, 1.  And so forth ...
//
// While which points are used for each face (triangle) are always
//  the same, the actual 3D positions of those points will be
//  different.  So while we can use the same indices for the
//  faces, the points themselves will be different so the drawn
//  triangles will be different.
//
// By generating this face list once, we can reuse it for every
//  surface (patch) we generate that has the same resolution.
//  This doesn't mean much when there's only one or two surfaces,
//  but when there are thousands of them, the time savings add up.

static void generatePatchFaces( Parameters *paramsP )
{
  Face *f = paramsP->bFaces;
  int   r = paramsP->resolution;

  if ( paramsP->debug ) {
    printf( "//------------------------ Bézier faces --\n" );
  }

  // row = 0 .. resolution-2
  for ( int row=0; row < r-1; row++ ) {
    int rowStart = row*r;

    // col = 0 .. resolution-2
    for ( int col=0; col < r-1; col++ ) {
      int here = rowStart + col;
      int there = here + r;

      // First triangle for this quad.
      f->v1 = here;
      f->v2 = there;
      f->v3 = there+1;

      if ( paramsP->debug ) {
        printf( "//   bf  %3d  %3d  %3d\n",
          f->v1, f->v2, f->v3 );
      }

      f++;

      // Second triangle for this quad.
      f->v1 = there+1;
      f->v2 = here+1;
      f->v3 = here;

      if ( paramsP->debug ) {
        printf( "//   bf  %3d  %3d  %3d\n",
          f->v1, f->v2, f->v3 );
      }

      f++;
    }
  }
}

//----------------------------------------------------------------
// Compute the values of the points making up the Bézier surface
//  (patch) with index p.  Also track the bounding box of the
//  generated surface so it can be displayed.
static void computePatch( Parameters *paramsP, int p )
{
  int r = paramsP->resolution;

  FLOAT64 xmin = DBL_MAX, xmax = -DBL_MAX;
  FLOAT64 ymin = DBL_MAX, ymax = -DBL_MAX;
  FLOAT64 zmin = DBL_MAX, zmax = -DBL_MAX;

  if ( paramsP->debug ) {
    printf( "//--------------------- Bézier patch %2d --\n", p );
  }

  // Get a pointer to the beginning of the regular points area.
  //  The control points come from this area and having this
  //  pointer make it easier to refer to the control points later.
  Point *cpP = paramsP->points;

  // Get a pointer to the beginning of the generated Bézier points
  //  area.  This makes it easier to refer to the points later.
  Point *ptP = paramsP->bPoints;

  // Clear all of the Bézier points so that when the linear
  //  combinations are computed, we're always starting from a
  //  zero point.
  memset( ptP, 0, sizeof( Point ) * paramsP->numBPoints );

  // Four nested for loops:
  //  m outermost -- counts 0 .. resolution-1, to compute u.
  //  n           -- counts 0 .. resolution-1, to compute v.
  //  i           -- counts 0 .. 3, to step the rows of the patch.
  //  j innermost -- counts 0 .. 3, to step the columns of the patch.

  // m = 0 .. resolution-1
  for ( int m=0; m < r; m++ ) {
    // Compute u, which is [0..1], from m.

    // Declare u and initialize it to the correct value
    //          using m.
    FLOAT64 u = m / (FLOAT64)(r-1);

    // n = 0 .. resolution-1
    for ( int n=0; n < r; n++ ) {
      // Compute v, which is [0..1], from n.

      // Declare v and initialize it to the correct value
      //          using n.
      FLOAT64 v = n / (FLOAT64)(r-1);

      // i = 0 .. 3
      for ( int i=0; i<=3; i++ ) {

        // j = 0 .. 3
        for ( int j=0; j<=3; j++ ) {
          // Get the i,j coefficient for u, v.

          // Declare c and initialize it to the correct
          //          value using i, j, u, and v as parameters to
          //          a call to computeCoefficient().
          FLOAT64 c = computeCoefficient(i,j,u,v);

          // Get the index of the i, j control point for patch p.
          int cpNum = paramsP->patches[p][4*i+j];

          // Accumulate in the Bézier point's x, y, z elements
          //  the value of the coefficient times the control
          //  point's x, y, z elements.

          // Update ptP's x, y, z elements properly using
          //          c, cpP, and cpNum.
          (*ptP)[0] = (*ptP)[0] + (c * cpP[cpNum][0]);
          (*ptP)[1] = (*ptP)[1] + (c * cpP[cpNum][1]);
          (*ptP)[2] = (*ptP)[2] + (c * cpP[cpNum][2]);

          if ( paramsP->debug ) {
            printf( "//   m %2d u %.4f n %2d v %.4f c %.6f px %12.8f py %12.8f pz %12.8f\n",
              m, u, n, v, c, (*ptP)[0], (*ptP)[1], (*ptP)[2] );
          }
        }
      }

      // Set the Bézier point's w element to 1.

      // Set ptP's w element to 1.0.
      (*ptP)[3] = 1.0;

      // Keep running track of xmin, xmax, ymin, ymax, zmin, zmax.

      // Update xmin, xmax, ymin, ymax, zmin, zmax as
      //          necessary using the x, y, z elements of ptP.
      xmin = fmin(xmin, (*ptP)[0]);
      xmax = fmax(xmax, (*ptP)[0]);
      ymin = fmin(ymin, (*ptP)[1]);
      ymax = fmax(ymax, (*ptP)[1]);
      zmin = fmin(zmin, (*ptP)[2]);
      zmax = fmax(zmax, (*ptP)[2]);

      // Move ptP to the next Bézier point position.

      //  Advance ptP to the next position.
      ptP++;
    }
  }

  if ( paramsP->debug ) {
    printf( "//   ( %12.8f .. %12.8f ), ( %12.8f .. %12.8f ), ( %12.8f .. %12.8f )\n",
      xmin, xmax, ymin, ymax, zmin, zmax );
  }
}

//----------------------------------------------------------------
// Compute the Bernstein coefficient for row i, column j given
//  the values of u, v.
static FLOAT64 computeCoefficient( int i, int j, FLOAT64 u, FLOAT64 v )
{
  // Precompute mu and mv to make the later computations easier
  //  more compact.
  FLOAT64 mu = 1.0 - u;
  FLOAT64 mv = 1.0 - v;

  // Default result is Not-A-Number so we can distinguish when an
  //  error happens (i.e., illegal row, column number).
  FLOAT64 result = NAN;

  switch ( i ) {
    // Coefficient is from row 0 ...
    case 0 :
      // Switch on the column number ...
      switch ( j ) {
        case 0 : result =             mu*mu*mu * mv*mv*mv; break;
        case 1 : result = 3 * v     * mu*mu*mu * mv*mv;    break;
        case 2 : result = 3 * v*v   * mu*mu*mu * mv;       break;
        case 3 : result =     v*v*v * mu*mu*mu;            break;

        // Bad column number!
        default :
          printf( "computeCoefficient: i %d, bad j %d?\n", i, j );
          break;
      }
      break;

    // Coefficient is from row 1 ...
    case 1 :
      // Switch on the column number ...
      switch ( j ) {
        // Fill in cases 0, 1, 2, 3, and default using the
        //          above as an example but with the proper
        //          calculations for this row.
        case 0 : result = 3 * u       * mu*mu * mv*mv*mv; break;
        case 1 : result = 9 * u*v     * mu*mu * mv*mv;    break;
        case 2 : result = 9 * u*v*v   * mu*mu * mv;       break;
        case 3 : result = 3 * u*v*v*v * mu*mu;            break;

        // Bad column number!
        default :
          printf( "computeCoefficient: i %d, bad j %d?\n", i, j );
          break;
      }
      break;

    // Coefficient is from row 2 ...
    case 2 :
      // Switch on the column number ...
      switch ( j ) {
        // Fill in cases 0, 1, 2, 3, and default using the
        //          above as an example but with the proper
        //          calculations for this row.
        case 0 : result = 3 * u*u       * mu * mv*mv*mv; break;
        case 1 : result = 9 * u*u*v     * mu * mv*mv;    break;
        case 2 : result = 9 * u*u*v*v   * mu * mv;       break;
        case 3 : result = 3 * u*u*v*v*v * mu;            break;

        // Bad column number!
        default :
          printf( "computeCoefficient: i %d, bad j %d?\n", i, j );
          break;
      }
      break;

    // Coefficient is from row 3 ...
    case 3 :
      // Switch on the column number ...
      switch ( j ) {
        // Fill in cases 0, 1, 2, 3, and default using the
        //          above as an example but with the proper
        //          calculations for this row.
        case 0 : result =     u*u*u       * mv*mv*mv; break;
        case 1 : result = 3 * u*u*u*v     * mv*mv;    break;
        case 2 : result = 3 * u*u*u*v*v   * mv;       break;
        case 3 : result =     u*u*u*v*v*v;            break;

        // Bad column number!
        default :
          printf( "computeCoefficient: i %d, bad j %d?\n", i, j );
          break;
      }
      break;

    // Bad row number!
    default :
      printf( "computeCoefficient: Bad i %d, j %d?\n", i, j );
      break;
  }

  // Return whatever we computed (or NAN if there were an error).
  return result;
}

//----------------------------------------------------------------
// Display the generated Bézier surface.
static void displayPatch( FILE *fp, Parameters *paramsP )
{
  // Cull backfacing triangles if we're supposed to.
  if ( paramsP->cull ) {
    backfaceCull( PATCHES, paramsP, 0 );
  }

  // Project the patch ...
  project( PATCHES, paramsP );

  // ... and display it!
  display( PATCHES, fp, paramsP );
}

//----------------------------------------------------------------
