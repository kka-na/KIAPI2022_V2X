/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "../asn/J2735_201603DA.ASN"
 * 	`asn1c -fcompound-names -pdu=all`
 */

#include "IntersectionState.h"

static int
memb_regional_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	/* Determine the number of elements */
	size = _A_CSEQUENCE_FROM_VOID(sptr)->count;
	
	if((size >= 1 && size <= 4)) {
		/* Perform validation of the inner elements */
		return SEQUENCE_OF_constraint(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_regional_constr_11 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..4)) */};
static asn_per_constraints_t asn_PER_type_regional_constr_11 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 2,  2,  1,  4 }	/* (SIZE(1..4)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_regional_constr_11 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..4)) */};
static asn_per_constraints_t asn_PER_memb_regional_constr_11 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 2,  2,  1,  4 }	/* (SIZE(1..4)) */,
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_regional_11[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_RegionalExtension_136P0,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_regional_tags_11[] = {
	(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_regional_specs_11 = {
	sizeof(struct IntersectionState__regional),
	offsetof(struct IntersectionState__regional, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_regional_11 = {
	"regional",
	"regional",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_regional_tags_11,
	sizeof(asn_DEF_regional_tags_11)
		/sizeof(asn_DEF_regional_tags_11[0]) - 1, /* 1 */
	asn_DEF_regional_tags_11,	/* Same as above */
	sizeof(asn_DEF_regional_tags_11)
		/sizeof(asn_DEF_regional_tags_11[0]), /* 2 */
	{ &asn_OER_type_regional_constr_11, &asn_PER_type_regional_constr_11, SEQUENCE_OF_constraint },
	asn_MBR_regional_11,
	1,	/* Single element */
	&asn_SPC_regional_specs_11	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_IntersectionState_1[] = {
	{ ATF_POINTER, 1, offsetof(struct IntersectionState, name),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DescriptiveName,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"name"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct IntersectionState, id),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IntersectionReferenceID,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"id"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct IntersectionState, revision),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_MsgCount,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"revision"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct IntersectionState, status),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IntersectionStatusObject,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"status"
		},
	{ ATF_POINTER, 3, offsetof(struct IntersectionState, moy),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MinuteOfTheYear,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"moy"
		},
	{ ATF_POINTER, 2, offsetof(struct IntersectionState, timeStamp),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSecond,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"timeStamp"
		},
	{ ATF_POINTER, 1, offsetof(struct IntersectionState, enabledLanes),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_EnabledLaneList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"enabledLanes"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct IntersectionState, states),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MovementList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"states"
		},
	{ ATF_POINTER, 2, offsetof(struct IntersectionState, maneuverAssistList),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ManeuverAssistList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maneuverAssistList"
		},
	{ ATF_POINTER, 1, offsetof(struct IntersectionState, regional),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		0,
		&asn_DEF_regional_11,
		0,
		{ &asn_OER_memb_regional_constr_11, &asn_PER_memb_regional_constr_11,  memb_regional_constraint_1 },
		0, 0, /* No default value */
		"regional"
		},
};
static const int asn_MAP_IntersectionState_oms_1[] = { 0, 4, 5, 6, 8, 9 };
static const ber_tlv_tag_t asn_DEF_IntersectionState_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_IntersectionState_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* name */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* id */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* revision */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* status */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* moy */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* timeStamp */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* enabledLanes */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* states */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* maneuverAssistList */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 } /* regional */
};
asn_SEQUENCE_specifics_t asn_SPC_IntersectionState_specs_1 = {
	sizeof(struct IntersectionState),
	offsetof(struct IntersectionState, _asn_ctx),
	asn_MAP_IntersectionState_tag2el_1,
	10,	/* Count of tags in the map */
	asn_MAP_IntersectionState_oms_1,	/* Optional members */
	6, 0,	/* Root/Additions */
	10,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_IntersectionState = {
	"IntersectionState",
	"IntersectionState",
	&asn_OP_SEQUENCE,
	asn_DEF_IntersectionState_tags_1,
	sizeof(asn_DEF_IntersectionState_tags_1)
		/sizeof(asn_DEF_IntersectionState_tags_1[0]), /* 1 */
	asn_DEF_IntersectionState_tags_1,	/* Same as above */
	sizeof(asn_DEF_IntersectionState_tags_1)
		/sizeof(asn_DEF_IntersectionState_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_IntersectionState_1,
	10,	/* Elements count */
	&asn_SPC_IntersectionState_specs_1	/* Additional specs */
};

