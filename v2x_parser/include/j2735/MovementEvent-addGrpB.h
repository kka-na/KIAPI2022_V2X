/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "AddGrpB"
 * 	found in "../asn/J2735_201603DA.ASN"
 * 	`asn1c -fcompound-names -pdu=all`
 */

#ifndef	_MovementEvent_addGrpB_H_
#define	_MovementEvent_addGrpB_H_


#include <asn_application.h>

/* Including external dependencies */
#include "TimeRemaining.h"
#include "MinTimetoChange.h"
#include "MaxTimetoChange.h"
#include "TimeIntervalConfidence.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* MovementEvent-addGrpB */
typedef struct MovementEvent_addGrpB {
	TimeRemaining_t	*startTime	/* OPTIONAL */;
	MinTimetoChange_t	 minEndTime;
	MaxTimetoChange_t	*maxEndTime	/* OPTIONAL */;
	TimeRemaining_t	*likelyTime	/* OPTIONAL */;
	TimeIntervalConfidence_t	*confidence	/* OPTIONAL */;
	TimeRemaining_t	*nextTime	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MovementEvent_addGrpB_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MovementEvent_addGrpB;

#ifdef __cplusplus
}
#endif

#endif	/* _MovementEvent_addGrpB_H_ */
#include <asn_internal.h>
