#ifndef includes_dot_haitch_h
#define includes_dot_haitch_h

////////////////////////////////////////////////
// useful includes & typedefs for a c project //
////////////////////////////////////////////////

/*
 * defaults
 * */
#include <math.h>
#include <time.h>
#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
/*
 * linux
 * */
#include <signal.h>
#include <sys/stat.h>
#include <sys/termios.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
/*
 * external
 * */
#include "../external/sds/sds.h"
#include "../external/sds/sdsalloc.h"

// #include "../include/glad/glad.h"
// #include "../external/glfw/include/GLFW/glfw3.h"
/*
 * math
 * */
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define CGLM_OMIT_NS_FROM_STRUCT_API
#include "../external/cglm/include/cglm/struct.h"
/*
 * defines
 * */

#endif
