#ifndef TIMING_API_TYPES_H
#define TIMING_API_TYPES_H
/*
 * The minimal declarations needed to make the timing FIFO API work!
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <epicsTime.h>
#include <stdint.h>

/** LCLS-II uses 64bit pulseID's */
typedef uint64_t TimingPulseId;

/** Reserved value for Invalid pulseIds */
#define TIMING_PULSEID_INVALID  0xFFFFFFFFFFFFFFFFLL


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TIMING_API_TYPES_H */
