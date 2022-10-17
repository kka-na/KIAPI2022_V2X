/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "../asn/J2735_201603DA.ASN"
 * 	`asn1c -fcompound-names -pdu=all`
 */

#ifndef	_SignalStatusPackage_H_
#define	_SignalStatusPackage_H_


#include <asn_application.h>

/* Including external dependencies */
#include "IntersectionAccessPoint.h"
#include "MinuteOfTheYear.h"
#include "DSecond.h"
#include "PrioritizationResponseStatus.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct SignalRequesterInfo;
struct IntersectionAccessPoint;
struct RegionalExtension;

/* SignalStatusPackage */
typedef struct SignalStatusPackage {
	struct SignalRequesterInfo	*requester	/* OPTIONAL */;
	IntersectionAccessPoint_t	 inboundOn;
	struct IntersectionAccessPoint	*outboundOn	/* OPTIONAL */;
	MinuteOfTheYear_t	*minute	/* OPTIONAL */;
	DSecond_t	*second	/* OPTIONAL */;
	DSecond_t	*duration	/* OPTIONAL */;
	PrioritizationResponseStatus_t	 status;
	struct SignalStatusPackage__regional {
		A_SEQUENCE_OF(struct RegionalExtension) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *regional;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SignalStatusPackage_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SignalStatusPackage;
extern asn_SEQUENCE_specifics_t asn_SPC_SignalStatusPackage_specs_1;
extern asn_TYPE_member_t asn_MBR_SignalStatusPackage_1[8];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "SignalRequesterInfo.h"
#include "IntersectionAccessPoint.h"
#include "RegionalExtension.h"

#endif	/* _SignalStatusPackage_H_ */
#include <asn_internal.h>
