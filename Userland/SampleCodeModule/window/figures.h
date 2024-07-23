// @deprecated
#ifndef FGR_H
#define FGR_H
#include "window.h"

HexColor RetOSBackground(int x, int y, double xScaleFactor,
                         double yScaleFactor);
HexColor windowsBackground(int x, int y, double xScaleFactor,
                           double yScaleFactor);
HexColor colorMapper(HexColor *colorMap, int code);

#endif
