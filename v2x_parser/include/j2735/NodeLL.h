/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "../asn/J2735_201603DA.ASN"
 * 	`asn1c -fcompound-names -pdu=all`
 */

#ifndef	_NodeLL_H_
#define	_NodeLL_H_


#include <asn_application.h>

/* Including external dependencies */
#include "NodeOffsetPointLL.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct NodeAttributeSetLL;

/* NodeLL */
typedef struct NodeLL {
	NodeOffsetPointLL_t	 delta;
	struct NodeAttributeSetLL	*attributes	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} NodeLL_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_NodeLL;
extern asn_SEQUENCE_specifics_t asn_SPC_NodeLL_specs_1;
extern asn_TYPE_member_t asn_MBR_NodeLL_1[2];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "NodeAttributeSetLL.h"

#endif	/* _NodeLL_H_ */
#include <asn_internal.h>
