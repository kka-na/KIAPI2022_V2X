/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "../asn/J2735_201603DA.ASN"
 * 	`asn1c -fcompound-names -pdu=all`
 */

#ifndef	_ExitService_H_
#define	_ExitService_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include "ITIScodes.h"
#include "ITIStextPhrase.h"
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ExitService__Member__item_PR {
	ExitService__Member__item_PR_NOTHING,	/* No components present */
	ExitService__Member__item_PR_itis,
	ExitService__Member__item_PR_text
} ExitService__Member__item_PR;

/* Forward definitions */
typedef struct ExitService__Member {
	struct ExitService__Member__item {
		ExitService__Member__item_PR present;
		union ExitService__Member__item_u {
			ITIScodes_t	 itis;
			ITIStextPhrase_t	 text;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} item;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ExitService__Member;

/* ExitService */
typedef struct ExitService {
	A_SEQUENCE_OF(ExitService__Member) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ExitService_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ExitService;
extern asn_SET_OF_specifics_t asn_SPC_ExitService_specs_1;
extern asn_TYPE_member_t asn_MBR_ExitService_1[1];
extern asn_per_constraints_t asn_PER_type_ExitService_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _ExitService_H_ */
#include <asn_internal.h>