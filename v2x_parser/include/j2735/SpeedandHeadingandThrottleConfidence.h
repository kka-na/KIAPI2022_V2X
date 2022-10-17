/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "../asn/J2735_201603DA.ASN"
 * 	`asn1c -fcompound-names -pdu=all`
 */

#ifndef	_SpeedandHeadingandThrottleConfidence_H_
#define	_SpeedandHeadingandThrottleConfidence_H_


#include <asn_application.h>

/* Including external dependencies */
#include "HeadingConfidence.h"
#include "SpeedConfidence.h"
#include "ThrottleConfidence.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SpeedandHeadingandThrottleConfidence */
typedef struct SpeedandHeadingandThrottleConfidence {
	HeadingConfidence_t	 heading;
	SpeedConfidence_t	 speed;
	ThrottleConfidence_t	 throttle;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SpeedandHeadingandThrottleConfidence_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SpeedandHeadingandThrottleConfidence;
extern asn_SEQUENCE_specifics_t asn_SPC_SpeedandHeadingandThrottleConfidence_specs_1;
extern asn_TYPE_member_t asn_MBR_SpeedandHeadingandThrottleConfidence_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _SpeedandHeadingandThrottleConfidence_H_ */
#include <asn_internal.h>
