#ifndef BSA_CALLBACK_API_H
#define BSA_CALLBACK_API_H

#include "timingApiTypes.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * BSA Timing Pattern data
 */
typedef struct BsaTimingPatternStruct
{
    timingPulseId       pulseId;            /**< 64bit pulseId  */
    uint64_t            edefInitMask;       /**< EDEF initialized mask  */
    uint64_t            edefActiveMask;     /**< EDEF active mask */
    uint64_t            edefAvgDoneMask;    /**< EDEF average-done mask */
    uint64_t            edefUpdateMask;     /**< EDEF update mask   */
    uint64_t            edefMinorMask;      /**< EDEF minor severity mask   */
    uint64_t            edefMajorMask;      /**< EDEF major severity mask   */
    epicsTimeStamp      timeStamp;          /**< TimeStamp for this BSA timing data */
}   BsaTimingData;

/**
 * BsaTimingCallbacks get called w/ 2 parameters
 *   - pUserPvt is any pointer the callback client needs to establish context
 *   - pNewPattern is a pointer to the new BSA timing data
 *
 * Timing services must guarantee the BSA timing pattern data in this structure is all
 * from the same beam pulse and does not change before the callback returns.
 */
typedef void (*BsaTimingCallback)( void * pUserPvt, const BsaTimingData * pNewPattern );


/**
 * RegisterBsaTimingCallback is called by the BSA client to register a callback function
 * for new BsaTimingData.
 *
 * The pUserPvt pointer can be used to establish context or set to 0 if not needed.
 *
 * Timing services must support this RegisterBsaTimingCallback() function and call the
 * callback function once for each new BsaTimingData to be compliant w/ this timing BSA API.
 *
 * The Timing service may support more than one BSA client, but is allowed to refuse
 * attempts to register multiple BSA callbacks.
 *
 * Each timing service should provide it's timing BSA code using a unique library name
 * so we can have EPICS IOC's that build applications for multiple timing service types.
 */
extern int RegisterBsaTimingCallback( BsaTimingCallback callback, void * pUserPvt );

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* BSA_CALLBACK_API_H */
