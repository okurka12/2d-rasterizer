/**
 * Debugging macros
*/

#ifndef _U_T_I_L_S_H_

#include <stdio.h>
#include <time.h>

#ifndef NDEBUG

/*-----------------------------LOGGING----------------------------------------*/

/* logs plain string (like puts) or does nothing if NDEBUG is defined */
#define log(msg) fprintf(stderr, __FILE__ ":%03d: %s\n", __LINE__, msg)

/* logs format (like printf) or does nothing if NDEBUG is defined */
#define logf(msg, ...) fprintf(stderr, __FILE__ ":%03d: " msg "\n", \
                               __LINE__, __VA_ARGS__)

/*-----------------------------TIMING-----------------------------------------*/

/* prefix to a timer log */
#define tprfx "[TIMER]: "

/* defines + starts the timer (identifier `id`) */
#define timer_start(id) clock_t id = clock()

/* resets the timer (identifier `id`) */
#define timer_restart(id) id = clock()

/* stops the timer and prints the time labeled with `msg` */
#define timer_end(id, msg) \
fprintf(stderr, tprfx "%s: %.3g s\n", msg, (double)(clock()-id)/CLOCKS_PER_SEC)

/*----------------------------------------------------------------------------*/

#else  // ifndef NDEBUG

#define log(msg) {}
#define logf(msg, ...) {}
#define timer_start(id) {}
#define timer_restart(id) {}
#define timer_end(id, msg) {}


#endif  // ifndef NDEBUG

/* absolute value */
#define abs(x) (((x) >= 0) ? (x) : (-(x)))

#endif  // ifndef _U_T_I_L_S_H_