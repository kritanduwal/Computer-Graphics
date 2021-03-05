// Duwal, Kritan
// kxd8042
// 2020-11-16
//----------------------------------------------------------------
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "pipeline.h"

static void drawFace( FILE *fp, int v1, int v2, int v3, Parameters *paramsP );
static void drawLine( FILE *fp, int p1, int p2, Parameters *paramsP );

//----------------------------------------------------------------
int display( char *fName, Parameters *paramsP )
{
  //----------------------------------------
  // Announce ourselves
  printf(
    "//----------------------------------------\n"
    "// Displaying\n"
    "//    out file    : \"%s\"\n"
    "//    canvas size : (W %d, H %d)\n"
    "//    clip region : (%d, %d) .. (%d, %d)\n",
    fName,
    paramsP->width, paramsP->height,
    paramsP->sXmin, paramsP->sYmin,
    paramsP->sXmax, paramsP->sYmax );

  //----------------------------------------
  // Get output file open
  FILE *fp = fopen( fName, "w" );
  if ( fp == NULL ) {
    printf( "// Unable to open \"%s\" for write, (%d) %s.\n",
      fName, errno, strerror( errno ) );

    return ERROR;
  }

  //--------------------------------------
  // Canvas size
  fprintf( fp, "# Canvas size\n" );

  fprintf( fp, "c %d %d\n", paramsP->width, paramsP->height);
  //--------------------------------------
  // Clip region lines
  fprintf( fp, "\n# Clip region lines ...\n" );

  fprintf( fp, "l %d %d %d %d\n", paramsP->sXmin, paramsP-> sYmin,
            paramsP->sXmax, paramsP->sYmin);
  fprintf( fp, "l %d %d %d %d\n", paramsP->sXmax, paramsP-> sYmin,
            paramsP->sXmax, paramsP->sYmax);
  fprintf( fp, "l %d %d %d %d\n", paramsP->sXmax, paramsP-> sYmax,
            paramsP->sXmin, paramsP->sYmax);
  fprintf( fp, "l %d %d %d %d\n", paramsP->sXmin, paramsP-> sYmax,
            paramsP->sXmin, paramsP->sYmin);

  //--------------------------------------
  // Draw each face
  fprintf( fp, "\n# Object lines ...\n" );

  int i;
  for(i = 0; i < paramsP->numFaces; i++)
  {
    drawFace(fp, paramsP->faces[i].v1, paramsP->faces[i].v2,
      paramsP->faces[i].v3, paramsP);
  }

  //--------------------------------------
  // Free/close local items.
  fclose( fp );

  // All done, w/no error!
  return OK;
}

//----------------------------------------------------------------
static void drawFace( FILE *fp, int v1, int v2, int v3, Parameters *paramsP )
{
  // Draw each of the sides of the face, IN THE PROPER ORDER.
  //  (The proper order is v1 -> v2, v2 -> v3, v3 -> v1.)

  drawLine(fp, v1, v2, paramsP);
  drawLine(fp, v2, v3, paramsP);
  drawLine(fp, v3, v1, paramsP);
}

static void drawLine( FILE *fp, int p1, int p2, Parameters *paramsP )
{
  // Draw a line from the object's p1 to the object's p2.

  // ROUND the x,y of each point to be an integer.
  int p1x, p1y;
  int p2x, p2y;

  p1x = ROUND(paramsP->points[p1][0]);
  p1y = ROUND(paramsP->points[p1][1]);
  p2x = ROUND(paramsP->points[p2][0]);
  p2y = ROUND(paramsP->points[p2][1]);

  // Use clipLine() to see if the line should be drawn.
  //  If so, draw it, using the updated coordinates of p1 and p2.
  int draw = clipLine(&p1x, &p1y, &p2x, &p2y, paramsP);
  if (draw != 0)
    fprintf( fp, "l %d %d %d %d\n", p1x, p1y, p2x, p2y);
}

//----------------------------------------------------------------
