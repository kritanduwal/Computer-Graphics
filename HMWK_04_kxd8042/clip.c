// Duwal, Kritan
// kxd8042
// 2020-11-16
//----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include "pipeline.h"

//----------------------------------------------------------------
#define INSIDE    (0)
#define LEFT      (1 << 0)
#define ABOVE     (1 << 1)
#define RIGHT     (1 << 2)
#define BELOW     (1 << 3)

static int _zone( int x, int y, Parameters *paramsP );

//----------------------------------------------------------------
int clipLine( int *p1x, int *p1y, int *p2x, int *p2y, Parameters *paramsP )
{
  while ( 1 ) {
    // Compute the zones of p1 and p2.
    int zone_p1 = _zone(*p1x, *p1y, paramsP);
    int zone_p2 = _zone(*p2x, *p2y, paramsP);

    // trivial accept
    // accept if both zones are INSIDE
    if(zone_p1 == INSIDE && zone_p2 == INSIDE)
      return 1;

    // trivial reject
    // reject if both zones are the same and not INSIDE
    if(zone_p1 == zone_p2)
        return 0;

    //reject if both zones are on same side of INSIDE
    if(zone_p1 & LEFT && zone_p2 & LEFT) // both zones left
      return 0;
    if(zone_p1 & RIGHT && zone_p2 & RIGHT) // both zones right
      return 0;
    if(zone_p1 & BELOW && zone_p2 & BELOW) // both zones below
      return 0;
    if(zone_p1 & ABOVE && zone_p2 & ABOVE) // both zones above
      return 0;

    // Not a trivial case -- have to slide an endpoint
    //  and try again.
    int code, x, y;
    if(zone_p1 == INSIDE)  code = zone_p2;
    else                   code = zone_p1;

    if(code & ABOVE)
    {
      x = *p1x + (*p2x - *p1x) * (paramsP->sYmax - *p1y) / (*p2y - *p1y);
      y = paramsP->sYmax;
      if(code == zone_p1)
      {
        *p1x = x;
        *p1y = y;
      }
      else if(code == zone_p2)
      {
        *p2x = x;
        *p2y = y;
      }
    }
    else if(code & BELOW)
    {
      x = *p1x + (*p2x - *p1x) * (paramsP->sYmin - *p1y) / (*p2y - *p1y);
      y = paramsP->sYmin;
      if(code == zone_p1)
      {
        *p1x = x;
        *p1y = y;
      }
      else if(code == zone_p2)
      {
        *p2x = x;
        *p2y = y;
      }
    }
    else if(code & RIGHT)
    {
      x = paramsP->sXmax;
      y = *p1y + (*p2y - *p1y) * (paramsP->sXmax - *p1x) / (*p2x - *p1x);
      if(code == zone_p1)
      {
        *p1x = x;
        *p1y = y;
      }
      else if(code == zone_p2)
      {
        *p2x = x;
        *p2y = y;
      }
    }
    else if(code & LEFT)
    {
      x = paramsP->sXmin;
      y = *p1y + (*p2y - *p1y) * (paramsP->sXmin - *p1x) / (*p2x - *p1x);
      if(code == zone_p1)
      {
        *p1x = x;
        *p1y = y;
      }
      else if(code == zone_p2)
      {
        *p2x = x;
        *p2y = y;
      }
    }
  }
}

//----------------------------------------------------------------
static int _zone( int x, int y, Parameters *paramsP )
{
  int code = INSIDE;

  if(x < paramsP->sXmin)
    code = code | LEFT;
  else if(x > paramsP->sXmax)
    code = code | RIGHT;

  if(y < paramsP->sYmin)
    code = code | BELOW;
  else if(y > paramsP->sYmax)
    code = code | ABOVE;

  return code;
}

//----------------------------------------------------------------
