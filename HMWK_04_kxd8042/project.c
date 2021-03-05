// Duwal, Kritan
// kxd8042
// 2020-11-13
//----------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include "matrix.h"
#include "pipeline.h"
#include "transforms.h"

//----------------------------------------------------------------
void project( Parameters *paramsP )
{
  Matrix projection;

  //----------------------------------------
  // Announce ourselves and construct the proper
  //  projection matrix.
  if ( paramsP->viewpoint == 0.0 ) {
    printf(
      "//------------------------- Parallel -----\n"
      "// Projecting\n"
      "//    world range : (%g, %g) .. (%g, %g)\n"
      "//    canvas size : (W %d, H %d)\n",
      paramsP->wXmin, paramsP->wYmin,
      paramsP->wXmax, paramsP->wYmax,
      paramsP->width, paramsP->height );

    projectParallelMatrix(paramsP->wXmin, paramsP->wXmax,
      paramsP->wYmin, paramsP->wYmax,
      paramsP->width, paramsP->height, projection);
  } else {
    printf(
      "//------------------------- Perspective --\n"
      "// Projecting\n"
      "//    world range : (%g, %g) .. (%g, %g)\n"
      "//    canvas size : (W %d, H %d)\n"
      "//    viewpoint   : %g\n",
      paramsP->wXmin, paramsP->wYmin,
      paramsP->wXmax, paramsP->wYmax,
      paramsP->width, paramsP->height,
      paramsP->viewpoint );

    projectPerspectiveMatrix(paramsP->viewpoint,
      paramsP->wXmin, paramsP->wXmax,
      paramsP->wYmin, paramsP->wYmax,
      paramsP->width, paramsP->height, projection);
  }

  //----------------------------------------
  // Transform all of the points!
  Point temp[paramsP->numPoints];
  int i;
  for(i = 0; i < paramsP->numPoints; i++)
  {
    multMatrixPoint(projection, paramsP->points[i], temp[i]);
  }
  for(i = 0; i < paramsP->numPoints; i++)
  {
    normalizePoint(temp[i], temp[i]);
  }
  for(i = 0; i < paramsP->numPoints; i++)
  {
    paramsP->points[i][0] = temp[i][0];
    paramsP->points[i][1] = temp[i][1];
    paramsP->points[i][2] = temp[i][2];
    paramsP->points[i][3] = temp[i][3];
  }
}

//----------------------------------------------------------------
