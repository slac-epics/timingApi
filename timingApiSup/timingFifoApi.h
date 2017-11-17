#ifndef TIMING_FIFO_API_H
#define TIMING_FIFO_API_H 

#include "timingApiTypes.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * TS_INDEX_INIT is a magic value that when used as the incr argument 
 * to timingGetFifoInfo() tells it to set the index to the most recently
 * arrived timing info.
 */
#define TS_INDEX_INIT           1000000

/**
 * The timingFifoInfo structure is used to retrieve synchronous data from the eventCode arrival time FIFO.
 * The timing service must guarantee that all values returned via this structure are for the same pulse.
 */
typedef	struct _timingFifoInfo
{
	epicsTimeStamp		fifo_time;		/**< EPICS timestamp for this arrival time	*/
	timingPulseId		fifo_fid;		/**< 64 bit fiducial pulseID				*/
	long long		fifo_tsc;		/**< 64 bit cpu timestamp counter			*/
}	timingFifoInfo;

/**
 * The timingGetFifoInfo() call allows a timingFifo client to access a
 * FIFO queue of the last MAX_TS_QUEUE eventCode arrival timestamps.
 * Each client has their own index position in the queue which can be 
 * controlled w/ the incr argument.
 *
 * Clients should not write directly to the index value.
 *   - incr==TS_INDEX_INIT: index set to the position of the most recent eventCode arrival.
 *   - incr!=MAX_TS_QUEUE: index set to index+incr.  Use +1/-1 to advance up and down the queue
 *   - incr==1: Set to 1 once synced to fetch one synced timestamp per sample indefinitely as
 *     long as you don't overrun or underrun.
 *
 * The index is adjusted according to the incr argument before reading the
 * eventCode arrival time info from the FIFO
 *
 * FIFO overruns occur by trying to fetch another timestamp when you've already fetched
 * the most recent one.
 *
 * FIFO underruns occur by trying to fetch the next timestamp after it's
 * position in the FIFO is reused by a new eventCode arrival time.
 *
 * The timing service must guarantee that all values returned via this call are for the same pulse.
 *
 * Return value:
 *   - 0 on success
 *   - epicsTimeError for NULL return ptrs, invalid index, or FIFO overflow/underflow.
 *
 * These values are returned via ptr:
 *   - index ptr: 64 bit FIFO position. Adjusted according to the incr
 *     argument before reading the info from the FIFO
 *   - pFifoInfoDest ptr: epicsTimestamp, 64 bit fiducial, 64 bit tsc, and status
 */
extern  int timingGetFifoInfo(	unsigned int        	eventCode,
                            	int                 	incr,
                            	unsigned long long	*	index,
				timingFifoInfo		*	pFifoInfoDest	);

/** timingGetLastFiducial returns lastfid, the last fiducial set by ISR.  */
extern timingPulseId	 timingGetLastFiducial( );

/**
 * Retrieve the most recent timestamp available
 */
extern  int		timingGetCurTimestamp(		epicsTimeStamp	*	pTimeStampDest );/**< Return EPICS timestamp via this ptr	*/

/**
 * Retrieve the most recent timestamp and pulseId for the specified eventCode
 */
extern  int		timingGetEventTimestamp(	epicsTimeStamp	*	pTimeStampDest,	/**< Return EPICS timestamp via this ptr	*/
											unsigned int		eventCode	);	/**< Which eventCode */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TIMING_FIFO_API_H */
