/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "../asn/J2735_201603DA.ASN"
 * 	`asn1c -fcompound-names -pdu=all`
 */

#ifndef	_DirectionOfUse_H_
#define	_DirectionOfUse_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DirectionOfUse {
	DirectionOfUse_unavailable	= 0,
	DirectionOfUse_forward	= 1,
	DirectionOfUse_reverse	= 2,
	DirectionOfUse_both	= 3
} e_DirectionOfUse;

/* DirectionOfUse */
typedef long	 DirectionOfUse_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_DirectionOfUse_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_DirectionOfUse;
extern const asn_INTEGER_specifics_t asn_SPC_DirectionOfUse_specs_1;
asn_struct_free_f DirectionOfUse_free;
asn_struct_print_f DirectionOfUse_print;
asn_constr_check_f DirectionOfUse_constraint;
ber_type_decoder_f DirectionOfUse_decode_ber;
der_type_encoder_f DirectionOfUse_encode_der;
xer_type_decoder_f DirectionOfUse_decode_xer;
xer_type_encoder_f DirectionOfUse_encode_xer;
oer_type_decoder_f DirectionOfUse_decode_oer;
oer_type_encoder_f DirectionOfUse_encode_oer;
per_type_decoder_f DirectionOfUse_decode_uper;
per_type_encoder_f DirectionOfUse_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _DirectionOfUse_H_ */
#include <asn_internal.h>
