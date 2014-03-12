#include "windows.h"

#include <GL/glut.h>

// disable warning C4786: symbol greater than 255 character,
// okay to ignore (See MSDN.)
//#pragma warning(disable: 4786) 
// disable warning C4018: '<' : signed/unsigned mismatch
// gets annoying in for loops
#pragma warning(disable: 4018) 

#include <iostream>
#include <stdlib.h>
#include <math.h>

#include "mtxlib/mtxlib.h"

using namespace std;
