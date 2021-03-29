#ifndef TIMING_FIFO_API_H
#define TIMING_FIFO_API_H

#include <stdint.h>
#include "timingApiTypes.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * TS_INDEX_INIT is a magic value that when used as the incr argument 
 * to timingFifoRead() tells it to set the index to the most recently
 * arrived timing info.
 */
#define TS_INDEX_INIT           1000000

/**
 * The BeamData structure is used to retrieve beam related data from the timing stream.
 * The timing service must guarantee that all values returned via this structure are for the same pulse.
 */
typedef struct _BeamData
{
    epicsTimeStamp      timeStamp;      /**< EPICS timestamp for this beam information					*/
    unsigned int		timeSlot;		/**< 360Hz timeslot 1-6											*/
    unsigned int		beamRequested;	/**< Non-zero when beam	was requested from the injection gun	*/
}   BeamData;

/**
 * The EventTimingData structure is used to retrieve synchronous data from the eventCode arrival time FIFO.
 * The timing service must guarantee that all values returned via this structure are for the same pulse.
 */
typedef struct _EventTimingData
{
    epicsTimeStamp      fifo_time;      /**< EPICS timestamp for this arrival time  */
    TimingPulseId       fifo_fid;       /**< 64 bit fiducial pulseID                */
    int64_t             fifo_tsc;       /**< 64 bit cpu timestamp counter           */
}   EventTimingData;

/**
 * The timingFifoRead() call allows a timingFifo client to access a
 * FIFO queue of the last TS_INDEX_INIT eventCode arrival timestamps.
 * Each client has their own index position in the queue which can be 
 * controlled w/ the incr argument.
 *
 * Clients should not write directly to the index value.
 *   - incr==TS_INDEX_INIT: index set to the position of the most recent eventCode arrival.
 *   - incr!=TS_INDEX_INIT: index set to index+incr.  Use +1/-1 to advance up and down the queue
 *   - incr==1: Set to 1 once synced to fetch one synced timestamp per sample indefinitely as
 *     long as you don't overrun or underrun.
 *
 * The index is adjusted according to the incr argument before reading the
 * eventCode arrival time info from the FIFO
 *
 * FIFO underruns occur by trying to fetch another timestamp when you've already fetched
 * the most recent one.
 *
 * FIFO overruns occur by trying to fetch the next timestamp after it's
 * position in the FIFO is reused by a new eventCode arrival time.
 *
 * The timing service must guarantee that all values returned via this call are for the same pulse.
 *
 * Return value:
 *   0 on success
 *   -1 for NULL return ptrs
 *   -2 for invalid index
 *   -3 for FIFO overflow
 *   -4 for FIFO underflow.
 *
 * These values are returned via ptr:
 *   - index ptr: 64 bit FIFO position. Adjusted according to the incr
 *     argument before reading the info from the FIFO
 *   - pTimingDataDest ptr: epicsTimeStamp, 64 bit fiducial, and 64 bit tsc
 */
extern  int timingFifoRead( unsigned int        eventCode,
                            int                 incr,
                            uint64_t        *   index,
                            EventTimingData *   pTimingDataDest   );

/** timingGetLastFiducial returns lastfid, the last fiducial set by ISR.  */
extern TimingPulseId timingGetLastFiducial( );

/** timingGetFiducialForTimeStamp returns the fiducial that corresponds to the specified timestamp.
 * If the timing module cannot determine the correct fiducial, it returns TIMING_PULSEID_INVALID.  */
extern TimingPulseId timingGetFiducialForTimeStamp( epicsTimeStamp  timeStamp );

/**
 * Retrieve the most recent timestamp available
 */
extern  int     timingGetCurTimeStamp(      epicsTimeStamp  *   pTimeStampDest );/**< Return EPICS timestamp via this ptr   */

/**
 * Retrieve the most recent timestamp and pulseId for the specified eventCode
 */
extern  int     timingGetEventTimeStamp(    epicsTimeStamp  *   pTimeStampDest, /**< Return EPICS timestamp via this ptr    */
                                            int                 eventCode   );  /**< Which eventCode */

/**
 * Retrieve the most recent beam data
 */
extern  int     timingGetBeamData(    BeamData  *   pBeamDataDest ); /**< Return Beam data via this ptr    */

/**
 * TimingEventCallbacks get called w/ 1 parameter
 *   - pUserPvt is any pointer the callback client needs to establish context
 *
 * Timing services will call this callback on every timing event received.
 * Note that the timing service will only receive timing data for events it has subscribed to.
 */
typedef void (*TimingEventCallback)( void * pUserPvt );

/**
 * RegisterTimingEventCallback is called by timing service clients to
 * register a callback function for new timing events.
 *
 * The pUserPvt pointer can be used to establish context or set to 0 if not needed.
 *
 * Timing services must support this RegisterTimingEventCallback() function and call the
 * callback function once for each new timing event to be compliant w/ this timing BSA API.
 *
 * Each callback client should get only one callback per beam pulse, even if there
 * are multiple timing events per pulse, such as w/ LCLS-1.  For LCLS-1 specifically, the timing
 * callback should be made for event code 1, the fiducial.
 *
 * The Timing service must support multiple timingEventCallbacks.
 */
extern int RegisterTimingEventCallback( TimingEventCallback callback, void * pUserPvt );

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TIMING_FIFO_API_H */
