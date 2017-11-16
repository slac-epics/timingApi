#ifndef TIMINGFIFOAPI_H
#define TIMINGFIFOAPI_H 
/*
 * The minimal declarations needed to make the timing FIFO API work!
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_TS_QUEUE			512		/**< # timestamps queued per event	*/

#include<epicsTime.h>

/**
 * The timingFifoInfo structure is used to retrieve data from the eventCode arrival time FIFO.
 */
struct timingFifoInfo
{
	epicsTimeStamp		fifo_time;		/**< EPICS timestamp for this arrival time	*/
	long long			fifo_fid;		/**< 64 bit fiducial pulseID				*/
	long long			fifo_tsc;		/**< 64 bit cpu timestamp counter			*/
	int					fifo_status;	/**< 0 on success, else epicsTimeERROR		*/
};

/**
 * The timingGetFifoInfo() call allows a timingFifo client to access a
 * FIFO queue of the last MAX_TS_QUEUE eventCode arrival timestamps.
 * Each client has their own index position in the queue which can be controlled w/ the incr argument.
 * Clients should not write directly to the index value.
 *   - incr==MAX_TS_QUEUE: index set to the position of the most recent eventCode arrival.
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
 * Return value:
 *   - 0 on success
 *   - epicsTimeError for NULL return ptrs, invalid index, or FIFO overflow/underflow.
 *
 * These values are returned via ptr:
 *   - index ptr: 64 bit FIFO position. Adjusted according to the incr
 *     argument before reading the info from the FIFO
 *   - pFifoInfoRet ptr: epicsTimestamp, 64 bit fiducial, 64 bit tsc, and status
 */
extern  int timingGetFifoInfo(	unsigned int        	eventCode,
                            	int                 	incr,
                            	unsigned long long	*	index,
								timingFifoInfo		*	pFifoInfoRet	);

/** timingGetLastFiducial returns lastfid, the last fiducial set by ISR.  */
extern long long timingGetLastFiducial( );

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TIMINGFIFOAPI_H */
