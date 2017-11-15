#ifndef	TIMING_API_H
#define	TIMING_API_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define EDEF_MAX 64
typedef uint64_t BsaPulseId;

struct BsaTimingPatternStruct
{
	BsaPulseId     pulseId;
	uint64_t       edefInitMask, edefActiveMask, edefAvgDoneMask, edefUpdateMask;
	uint64_t       edefMinorMask, edefMajorMask;
	epicsTimeStamp timeStamp;
};

typedef struct BsaTimingPatternStruct *BsaTimingPattern;

typedef void (*BsaTimingCallback)( BsaTimingPattern newPattern );

extern int RegisterBsaTimingCallback( BsaTimingCallback callback, void * pUserPvt );

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TIMING_API_H */
