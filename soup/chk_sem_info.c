/*
 * chk_sem_info - check .info.json semantics
 *
 * "Because grammar and syntax alone do not make a complete language." :-)
 *
 * The concept of this file was developed by:
 *
 *	chongo (Landon Curt Noll, http://www.isthe.com/chongo/index.html) /\oo/\
 *
 * and it was was auto-generated by:
 *
 *	make mkchk_sem
 *
 * The JSON parser was co-developed in 2022 by:
 *
 *	@xexyl
 *	https://xexyl.net		Cody Boone Ferguson
 *	https://ioccc.xexyl.net
 * and:
 *	chongo (Landon Curt Noll, http://www.isthe.com/chongo/index.html) /\oo/\
 *
 * "Because sometimes even the IOCCC Judges need some help." :-)
 *
 * "Share and Enjoy!"
 *     --  Sirius Cybernetics Corporation Complaints Division, JSON spec department. :-)
 */


/* special comments for the seqcexit tool */
/* exit code out of numerical order - ignore in sequencing - ooo */
/* exit code change of order - use new value in sequencing - coo */


/*
 * chk_sem_info - check .info.json semantics
 */
#include "chk_sem_info.h"


struct json_sem sem_info[SEM_INFO_LEN+1] = {
/* depth    type        min     max   count   index  name_len validate  name    data */
  { 5,	JTYPE_STRING,	10,	72,	72,	0,	0,	NULL,	NULL,	NULL },
  { 4,	JTYPE_MEMBER,	1,	1,	1,	1,	8,	chk_Makefile,	"Makefile",	NULL },
  { 4,	JTYPE_MEMBER,	1,	1,	1,	2,	9,	chk_auth_JSON,	"auth_JSON",	NULL },
  { 4,	JTYPE_MEMBER,	1,	1,	1,	3,	5,	chk_c_src,	"c_src",	NULL },
  { 4,	JTYPE_MEMBER,	0,	31,	31,	4,	10,	chk_extra_file,	"extra_file",	NULL },
  { 4,	JTYPE_MEMBER,	1,	1,	1,	5,	9,	chk_info_JSON,	"info_JSON",	NULL },
  { 4,	JTYPE_MEMBER,	1,	1,	1,	6,	7,	chk_remarks,	"remarks",	NULL },
  { 3,	JTYPE_OBJECT,	5,	36,	36,	7,	0,	NULL,	NULL,	NULL },
  { 2,	JTYPE_NUMBER,	7,	7,	7,	8,	0,	NULL,	NULL,	NULL },
  { 2,	JTYPE_STRING,	50,	50,	50,	9,	0,	NULL,	NULL,	NULL },
  { 2,	JTYPE_BOOL,	16,	16,	16,	10,	0,	NULL,	NULL,	NULL },
  { 2,	JTYPE_ARRAY,	1,	1,	1,	11,	0,	NULL,	NULL,	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	12,	13,	chk_IOCCC_contest,	"IOCCC_contest",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	13,	16,	chk_IOCCC_contest_id,	"IOCCC_contest_id",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	14,	18,	chk_IOCCC_info_version,	"IOCCC_info_version",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	15,	10,	chk_IOCCC_year,	"IOCCC_year",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	16,	17,	chk_Makefile_override,	"Makefile_override",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	17,	8,	chk_abstract,	"abstract",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	18,	16,	chk_chkentry_version,	"chkentry_version",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	19,	14,	chk_empty_override,	"empty_override",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	20,	17,	chk_first_rule_is_all,	"first_rule_is_all",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	21,	15,	chk_fnamchk_version,	"fnamchk_version",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	22,	16,	chk_formed_timestamp,	"formed_timestamp",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	23,	21,	chk_formed_timestamp_usec,	"formed_timestamp_usec",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	24,	14,	chk_found_all_rule,	"found_all_rule",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	25,	16,	chk_found_clean_rule,	"found_clean_rule",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	26,	18,	chk_found_clobber_rule,	"found_clobber_rule",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	27,	14,	chk_found_try_rule,	"found_try_rule",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	28,	15,	chk_highbit_warning,	"highbit_warning",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	29,	17,	chk_iocccsize_version,	"iocccsize_version",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	30,	8,	chk_manifest,	"manifest",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	31,	13,	chk_min_timestamp,	"min_timestamp",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	32,	20,	chk_mkiocccentry_version,	"mkiocccentry_version",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	33,	10,	chk_no_comment,	"no_comment",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	34,	11,	chk_nul_warning,	"nul_warning",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	35,	16,	chk_rule_2a_mismatch,	"rule_2a_mismatch",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	36,	16,	chk_rule_2a_override,	"rule_2a_override",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	37,	12,	chk_rule_2a_size,	"rule_2a_size",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	38,	16,	chk_rule_2b_override,	"rule_2b_override",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	39,	12,	chk_rule_2b_size,	"rule_2b_size",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	40,	11,	chk_submit_slot,	"submit_slot",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	41,	7,	chk_tarball,	"tarball",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	42,	9,	chk_test_mode,	"test_mode",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	43,	15,	chk_timestamp_epoch,	"timestamp_epoch",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	44,	5,	chk_title,	"title",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	45,	16,	chk_trigraph_warning,	"trigraph_warning",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	46,	14,	chk_txzchk_version,	"txzchk_version",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	47,	14,	chk_ungetc_warning,	"ungetc_warning",	NULL },
  { 1,	JTYPE_MEMBER,	1,	1,	1,	48,	15,	chk_wordbuf_warning,	"wordbuf_warning",	NULL },
  { 0,	JTYPE_OBJECT,	1,	1,	1,	49,	0,	NULL,	NULL,	NULL },
  { 0,	JTYPE_UNSET,	0,	0,	0,	-1,	0,	NULL,	NULL,	NULL }
};
