// Duwal, Kritan
// kxd8042
// 2020-11-13
//----------------------------------------------------------------
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "matrix.h"
#include "pipeline.h"
#include "transforms.h"

//----------------------------------------------------------------
void eulerRotate( Parameters *paramsP )
{
  FLOAT64 phiDeg   = RAD2DEG( paramsP->phi );
  FLOAT64 thetaDeg = RAD2DEG( paramsP->theta );
  FLOAT64 psiDeg   = RAD2DEG( paramsP->psi );

  //----------------------------------------
  // Announce ourselves
  printf(
    "//----------------------------------------\n"
    "// Euler rotation by ϕ %g°, θ %g°, ψ %g° ...\n",
    phiDeg, thetaDeg, psiDeg );

  //----------------------------------------
  // Construct the three rotation matrices.
  Matrix R_phi;
  copyMatrix( I4, R_phi);

  R_phi[1][1] = cos(paramsP->phi);
  R_phi[1][2] = sin(paramsP->phi) * (-1);
  R_phi[2][1] = sin(paramsP->phi);
  R_phi[2][2] = cos(paramsP->phi);

  Matrix R_theta;
  copyMatrix( I4, R_theta);

  R_theta[0][0] = cos(paramsP->theta);
  R_theta[0][2] = sin(paramsP->theta);
  R_theta[2][0] = sin(paramsP->theta) * (-1);
  R_theta[2][2] = cos(paramsP->theta);

  Matrix R_psi;
  copyMatrix( I4, R_psi);

  R_psi[0][0] = cos(paramsP->psi);
  R_psi[0][1] = sin(paramsP->psi) * (-1);
  R_psi[1][0] = sin(paramsP->psi);
  R_psi[1][1] = cos(paramsP->psi);

  //----------------------------------------
  // Compute the object's center.
  FLOAT64 xmin, ymin, zmin = DBL_MAX;
  FLOAT64 xmax, ymax, zmax = -DBL_MAX;
  int i;
  for(i = 0; i < paramsP->numPoints; i++)
  {
    if(paramsP->points[i][0] < xmin)  xmin = paramsP->points[i][0];
    if(paramsP->points[i][0] > xmax)  xmax = paramsP->points[i][0];

    if(paramsP->points[i][1] < ymin)  ymin = paramsP->points[i][1];
    if(paramsP->points[i][1] > ymax)  ymax = paramsP->points[i][1];

    if(paramsP->points[i][2] < zmin)  zmin = paramsP->points[i][2];
    if(paramsP->points[i][2] > zmax)  zmax = paramsP->points[i][2];
  }

  FLOAT64 tx, ty, tz;
  tx = (xmax + xmin) / 2;
  ty = (ymax + ymin) / 2;
  tz = (zmax + zmin) / 2;

  //----------------------------------------
  // Construct the two translation matrices.
  Matrix T_inv;
  copyMatrix( I4 , T_inv);

  T_inv[0][3] = tx;
  T_inv[1][3] = ty;
  T_inv[2][3] = tz;

  Matrix T;
  copyMatrix( I4 , T);

  T[0][3] = tx * (-1);
  T[1][3] = ty * (-1);
  T[2][3] = tz * (-1);

  //----------------------------------------
  // Construct the combined transformation.
  Matrix temp1, temp2, temp3, combined;
  multMatrix(T_inv, R_phi, temp1);
  multMatrix(temp1, R_theta, temp2);
  multMatrix(temp2, R_psi, temp3);
  multMatrix(temp3, T, combined);

  //----------------------------------------
  // Display the r and e values.
  printf( "//   r00 = %11.6f, r01 = %11.6f, r02 = %11.6f\n",
    combined[0][0], combined[0][1], combined[0][2] );
  printf( "//   r10 = %11.6f, r11 = %11.6f, r12 = %11.6f\n",
    combined[1][0], combined[1][1], combined[1][2] );
  printf( "//   r20 = %11.6f, r21 = %11.6f, r22 = %11.6f\n",
    combined[2][0], combined[2][1], combined[2][2] );

  printf( "//   ex  = %11.6f, ey  = %11.6f, ez  = %11.6f\n",
    combined[0][3], combined[1][3], combined[2][3] );

  //----------------------------------------
  // Transform all of the points!
  Point temp[paramsP->numPoints];

  for(i = 0; i < paramsP->numPoints; i++)
  {
    multMatrixPoint(combined, paramsP->points[i], temp[i]);
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
