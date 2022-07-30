/* vim: set tabstop=8 softtabstop=4 shiftwidth=4 noexpandtab : */
/*
 * chk_validate - validate functions for checking JSON semantics
 *
 * "Because grammar and syntax alone do not make a complete language." :-)
 *
 * The concept of this file was developed by:
 *
 *	chongo (Landon Curt Noll, http://www.isthe.com/chongo/index.html) /\oo/\
 *
 * "Because sometimes even the IOCCC Judges need some help." :-)
 */


/* special comments for the seqcexit tool */
/*ooo*/ /* exit code out of numerical order - ignore in sequencing */
/*coo*/ /* exit code change of order - use new value in sequencing */


#include <stdio.h>
#include <unistd.h>

/*
 * chk_validate - validate functions for checking JSON semantics
 */
#include "chk_validate.h"


/*
 * static function declarations
 */
static bool chk_null_args(struct json *node, unsigned int depth, struct json_sem *sem,
			  char const *name, struct json_sem_val_err **val_err);
static char *member_value_decoded_str(struct json *node, unsigned int depth, struct json_sem *sem,
				      char const *name, struct json_sem_val_err **val_err);
static bool *member_value_bool(struct json *node, unsigned int depth, struct json_sem *sem,
			       char const *name, struct json_sem_val_err **val_err);
static struct str_or_null member_value_str_or_null(struct json *node, unsigned int depth, struct json_sem *sem,
						   char const *name, struct json_sem_val_err **val_err);
static int *member_value_int(struct json *node, unsigned int depth, struct json_sem *sem,
			     char const *name, struct json_sem_val_err **val_err);

/*
 * chk_null_args - check arguments for NULL
 *
 * Check if critical if args (node, sem, name) are NULL.  Report if a critical arg
 * is NULL via a val_err pointer to address where to place a JSON semantic validation error.
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	name	name of caller function (NULL ==> "((NULL))")
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	true ==> one or more args are NULL, *val_err if non-NULL is JSON semantic error
 *	false ==> all args are non-NULL
 */
static bool
chk_null_args(struct json *node, unsigned int depth, struct json_sem *sem,
	      char const *name, struct json_sem_val_err **val_err)
{
    /*
     * firewall - args
     */
    if (name == NULL) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(10, node, depth, sem, "((NULL))", "name is NULL");
	}
	return true;
    }
    if (node == NULL) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(11, node, depth, sem, name, "node is NULL");
	}
	return true;
    }
    if (sem == NULL) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(12, node, depth, sem, name, "sem is NULL");
	}
	return true;
    }
    return false;
}


/*
 * member_value_decoded_str - return the JSON decoded value string from a JTYPE_MEMBER
 *
 * Given a JSON node of type JTYPE_MEMBER, look at the value of the JSON member
 * and if it is a JTYPE_STRING (JSON string), return the value's JSON decoded string
 * or return NULL on error or invalid input.
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	name	name of caller function (NULL ==> "((NULL))")
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	!= NULL ==> decoded JTYPE_STRING from the value part of JTYPE_MEMBER
 *	    The val_err arg is ignored
 *	NULL ==> invalid arguments or JSON conversion error
 *	    If val_err != NULLm then *val_err is JSON semantic validation error (struct json_cnt_err)
 */
static char *
member_value_decoded_str(struct json *node, unsigned int depth, struct json_sem *sem,
			 char const *name, struct json_sem_val_err **val_err)
{
    struct json_member *item = NULL;		/* JSON member */
    struct json *value = NULL;			/* value of JTYPE_MEMBER */
    struct json_string *istr = NULL;		/* JTYPE_MEMBER value as JTYPE_STRING */
    char *str = NULL;				/* JTYPE_STRING as decoded JSON string */

    /*
     * firewall - args
     */
    if (chk_null_args(node, depth, sem, name, val_err) == true) {
	return NULL;
    }

    /*
     * validate JSON parse node type
     */
    if (node->type != JTYPE_MEMBER) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(13, node, depth, sem, name, "node type %s != JTYPE_MEMBER",
				    json_type_name(node->type));
	}
	return NULL;
    }
    item = &(node->item.member);
    if (item->converted == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(14, node, depth, sem, name, "JTYPE_MEMBER node converted is false");
	}
	return NULL;
    }

    /*
     * firewall - value
     */
    value = item->value;
    if (value == NULL) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(15, node, depth, sem, name, "node value is NULL");
	}
	return NULL;
    }

    /*
     * validate JSON parse node value type
     */
    if (value->type != JTYPE_STRING) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(16, node, depth, sem, name, "node value type %s != JTYPE_STRING",
				    json_type_name(value->type));
	}
	return NULL;
    }
    istr = &(value->item.string);
    if (istr->converted == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(17, node, depth, sem, name, "node value JTYPE_STRING converted is false");
	}
	return NULL;
    }

    /*
     * firewall - decoded JSON string
     */
    str = istr->str;
    if (str == NULL) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(18, node, depth, sem, name, "node value decoded JSON string is NULL");
	}
	return NULL;
    }

    /*
     * case success: return decoded JSON string
     */
    return str;
}


/*
 * member_value_bool - return the JSON boolean from a JTYPE_BOOL from a JTYPE_MEMBER
 *
 * Given a JSON node of type JTYPE_MEMBER, look at the value of the JSON member
 * and if it is a JTYPE_BOOL (JSON boolean), return a pointer to the JSON boolean
 * or return NULL on error or invalid input.
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	name	name of caller function (NULL ==> "((NULL))")
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	!= NULL ==> decoded JTYPE_BOOL from the value part of JTYPE_MEMBER
 *	    The val_err arg is ignored
 *	NULL ==> invalid arguments or JSON conversion error
 *	    If val_err != NULLm then *val_err is JSON semantic validation error (struct json_cnt_err)
 */
static bool *
member_value_bool(struct json *node, unsigned int depth, struct json_sem *sem,
		  char const *name, struct json_sem_val_err **val_err)
{
    struct json_member *item = NULL;		/* JSON member */
    struct json *value = NULL;			/* value of JTYPE_MEMBER */
    struct json_boolean *ibool = NULL;		/* JTYPE_MEMBER value as JTYPE_BOOL */

    /*
     * firewall - args
     */
    if (chk_null_args(node, depth, sem, name, val_err) == true) {
	return NULL;
    }

    /*
     * validate JSON parse node type
     */
    if (node->type != JTYPE_MEMBER) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(19, node, depth, sem, name, "node type %s != JTYPE_MEMBER",
				    json_type_name(node->type));
	}
	return NULL;
    }
    item = &(node->item.member);
    if (item->converted == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(20, node, depth, sem, name, "JTYPE_MEMBER node converted is false");
	}
	return NULL;
    }

    /*
     * firewall - value
     */
    value = item->value;
    if (value == NULL) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(21, node, depth, sem, name, "node value is NULL");
	}
	return NULL;
    }

    /*
     * validate JSON parse node value type
     */
    if (value->type != JTYPE_BOOL) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(22, node, depth, sem, name, "node value type %s != JTYPE_BOOL",
				    json_type_name(value->type));
	}
	return NULL;
    }
    ibool = &(value->item.boolean);
    if (ibool->converted == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(23, node, depth, sem, name, "node value JTYPE_BOOL converted is false");
	}
	return NULL;
    }

    /*
     * case success: return JSON boolean
     */
    return &(ibool->value);
}


/*
 * member_value_str_or_null - return JSON decoded value string or JSON null from a JTYPE_MEMBER
 *
 * Given a JSON node of type JTYPE_MEMBER, look at the value of the JSON member
 * and if it is a JTYPE_STRING (JSON string), return the value's JSON decoded string,
 * and if it is a JTYPE_NULL (JSON null), indicate that value is JSON null.
 *
 * The return structure gives you a  way to distinguish between JTYPE_STRING and JTYPE_NULL:
 *
 *   When value is a JTYPE_STRING (JSON string):
 *
 *	ret.valid == true
 *	ret.is_null == false
 *	ret.str is the JSON decoded value string
 *
 *   When value is a JTYPE_NULL (JSON null):
 *
 *	ret.valid == true
 *	ret.is_null == true
 *	ret.str is NULL
 *
 *   When there is an error or invalid input:
 *
 *	ret.valid == false
 *	ret.is_null == false
 *	ret.str is NULL
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	name	name of caller function (NULL ==> "((NULL))")
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	struct str_or_null (see above)
 */
static struct str_or_null
member_value_str_or_null(struct json *node, unsigned int depth, struct json_sem *sem,
			 char const *name, struct json_sem_val_err **val_err)
{
    struct json_member *item = NULL;		/* JSON member */
    struct json *value = NULL;			/* value of JTYPE_MEMBER */
    struct json_string *istr = NULL;		/* JTYPE_MEMBER value as JTYPE_STRING */
    struct json_null *inull = NULL;		/* JTYPE_MEMBER value as JTYPE_NULL */
    struct str_or_null ret = {			/* return value - initialized to invalid */
	false, false, NULL
    };

    /*
     * firewall - args
     */
    if (chk_null_args(node, depth, sem, name, val_err) == true) {
	return ret;
    }

    /*
     * validate JSON parse node type
     */
    if (node->type != JTYPE_MEMBER) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(24, node, depth, sem, name, "node type %s != JTYPE_MEMBER",
				    json_type_name(node->type));
	}
	return ret;
    }
    item = &(node->item.member);
    if (item->converted == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(25, node, depth, sem, name, "JTYPE_MEMBER node converted is false");
	}
	return ret;
    }

    /*
     * firewall - value
     */
    value = item->value;
    if (value == NULL) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(26, node, depth, sem, name, "node value is NULL");
	}
	return ret;
    }

    /*
     * process based on value type
     */
    switch (value->type) {

    /*
     * case: value is JTYPE_STRING
     */
    case JTYPE_STRING:

	/*
	 * firewall - value
	 */
	istr = &(value->item.string);
	if (istr->converted == false) {
	    if (val_err != NULL) {
		*val_err = werr_sem_val(27, node, depth, sem, name, "node value JTYPE_STRING converted is false");
	    }
	    return ret;
	}

	/*
	 * firewall - decoded JSON string
	 */
	ret.str = istr->str;
	if (ret.str == NULL) {
	    if (val_err != NULL) {
		*val_err = werr_sem_val(28, node, depth, sem, name, "node value decoded JSON string is NULL");
	    }
	    return ret;
	}
	ret.valid = true;
	break;

    /*
     * case: value is JTYPE_NULL
     */
    case JTYPE_NULL:

	/*
	 * firewall - value
	 */
	inull = &(value->item.null);
	if (inull->converted == false) {
	    if (val_err != NULL) {
		*val_err = werr_sem_val(29, node, depth, sem, name, "node value JTYPE_NULL converted is false");
	    }
	    return ret;
	}
	ret.is_null = true;
	ret.valid = true;
	break;

    /*
     * case: everything else
     */
    default:
	if (val_err != NULL) {
	    *val_err = werr_sem_val(30, node, depth, sem, name, "node value type %s != JTYPE_STRING and != JTYPE_NULL",
				    json_type_name(value->type));
	}
	break;
    }

    /*
     * return result
     */
    return ret;
}


/*
 * member_value_int - return pointer to int from a JSON number value of JTYPE_MEMBER
 *
 * Given a JSON node of type JTYPE_MEMBER, look at the value of the JSON member
 * and if it is a JTYPE_NUMBER (JSON number), return a pointer to a converted int
 * or return NULL on error or invalid input.
 *
 * If the JTYPE_NUMBER (JSON number) cannot be returned as an int, NULL is returned.
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	name	name of caller function (NULL ==> "((NULL))")
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	!= NULL ==> decoded JTYPE_NUMBER as an int from the value part of JTYPE_MEMBER
 *	    The val_err arg is ignored
 *	NULL ==> invalid arguments or JSON conversion error
 *	    If val_err != NULLm then *val_err is JSON semantic validation error (struct json_cnt_err)
 */
static int *
member_value_int(struct json *node, unsigned int depth, struct json_sem *sem,
		 char const *name, struct json_sem_val_err **val_err)
{
    struct json_member *item = NULL;		/* JSON member */
    struct json *value = NULL;			/* value of JTYPE_MEMBER */
    struct json_number *inum = NULL;		/* JTYPE_MEMBER value as JTYPE_NUMBER */

    /*
     * firewall - args
     */
    if (chk_null_args(node, depth, sem, name, val_err) == true) {
	return NULL;
    }

    /*
     * validate JSON parse node type
     */
    if (node->type != JTYPE_MEMBER) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(31, node, depth, sem, name, "node type %s != JTYPE_MEMBER",
				    json_type_name(node->type));
	}
	return NULL;
    }
    item = &(node->item.member);
    if (item->converted == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(32, node, depth, sem, name, "JTYPE_MEMBER node converted is false");
	}
	return NULL;
    }

    /*
     * firewall - value
     */
    value = item->value;
    if (value == NULL) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(33, node, depth, sem, name, "node value is NULL");
	}
	return NULL;
    }

    /*
     * validate JSON parse node value type
     */
    if (value->type != JTYPE_NUMBER) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(34, node, depth, sem, name, "node value type %s != JTYPE_BOOL",
				    json_type_name(value->type));
	}
	return NULL;
    }
    inum = &(value->item.number);
    if (inum->converted == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(35, node, depth, sem, name, "node value JTYPE_NUMBER converted is false");
	}
	return NULL;
    }

    /*
     * validate JTYPE_NUMBER was able to be converted into an int
     */
    if (inum->int_sized == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(36, node, depth, sem, name, "node value JTYPE_NUMBER was unable to convert to an int");
	}
	return NULL;
    }

    /*
     * case success: return JSON boolean
     */
    return &(inum->as_int);
}


/*
 * chk_IOCCC_author_version - JSON semantic check for IOCCC_author_version
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	true ==> JSON element is valid
 *	false ==> JSON element is NOT valid, or NULL pointer, or some internal error
 */
bool
chk_IOCCC_author_version(struct json *node,
			 unsigned int depth, struct json_sem *sem, struct json_sem_val_err **val_err)
{
    char *str = NULL;				/* JTYPE_STRING as decoded JSON string */
    bool test = false;				/* validation test result */

    /*
     * firewall - args
     */
    str = member_value_decoded_str(node, depth, sem, __func__, val_err);
    if (str == NULL) {
	return false;
    }

    /*
     * validate decoded JSON string
     */
    test = test_IOCCC_author_version(str);
    if (test == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(37, node, depth, sem, __func__, "invalid IOCCC_author_version");
	}
	return false;
    }

    /*
     * return validation success
     */
    if (val_err != NULL) {
	*val_err = NULL;
    }
    return true;
}


/*
 * chk_IOCCC_contest_id - JSON semantic check for IOCCC_contest_id
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	true ==> JSON element is valid
 *	false ==> JSON element is NOT valid, or NULL pointer, or some internal error
 */
bool
chk_IOCCC_contest_id(struct json *node,
		     unsigned int depth, struct json_sem *sem, struct json_sem_val_err **val_err)
{
    char *str = NULL;				/* JTYPE_STRING as decoded JSON string */
    bool test = false;				/* validation test result */

    /*
     * firewall - args
     */
    str = member_value_decoded_str(node, depth, sem, __func__, val_err);
    if (str == NULL) {
	return false;
    }

    /*
     * validate decoded JSON string
     */
    test = test_IOCCC_contest_id(str);
    if (test == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(38, node, depth, sem, __func__, "invalid IOCCC_contest_id");
	}
	return false;
    }

    /*
     * return validation success
     */
    if (val_err != NULL) {
	*val_err = NULL;
    }
    return true;
}


/*
 * chk_IOCCC_info_version - JSON semantic check for IOCCC_info_version
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	true ==> JSON element is valid
 *	false ==> JSON element is NOT valid, or NULL pointer, or some internal error
 */
bool
chk_IOCCC_info_version(struct json *node,
		       unsigned int depth, struct json_sem *sem, struct json_sem_val_err **val_err)
{
    char *str = NULL;				/* JTYPE_STRING as decoded JSON string */
    bool test = false;				/* validation test result */

    /*
     * firewall - args
     */
    str = member_value_decoded_str(node, depth, sem, __func__, val_err);
    if (str == NULL) {
	return false;
    }

    /*
     * validate decoded JSON string
     */
    test = test_IOCCC_info_version(str);
    if (test == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(39, node, depth, sem, __func__, "invalid IOCCC_info_version");
	}
	return false;
    }

    /*
     * return validation success
     */
    if (val_err != NULL) {
	*val_err = NULL;
    }
    return true;
}


/*
 * chk_Makefile - JSON semantic check for Makefile filename
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	true ==> JSON element is valid
 *	false ==> JSON element is NOT valid, or NULL pointer, or some internal error
 */
bool
chk_Makefile(struct json *node,
	     unsigned int depth, struct json_sem *sem, struct json_sem_val_err **val_err)
{
    char *str = NULL;				/* JTYPE_STRING as decoded JSON string */
    bool test = false;				/* validation test result */

    /*
     * firewall - args
     */
    str = member_value_decoded_str(node, depth, sem, __func__, val_err);
    if (str == NULL) {
	return false;
    }

    /*
     * validate decoded JSON string
     */
    test = test_Makefile(str);
    if (test == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(40, node, depth, sem, __func__, "invalid Makefile filename");
	}
	return false;
    }

    /*
     * return validation success
     */
    if (val_err != NULL) {
	*val_err = NULL;
    }
    return true;
}


/*
 * chk_Makefile_override - JSON semantic check for Makefile_override
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	true ==> JSON element is valid
 *	false ==> JSON element is NOT valid, or NULL pointer, or some internal error
 */
bool
chk_Makefile_override(struct json *node,
		      unsigned int depth, struct json_sem *sem, struct json_sem_val_err **val_err)
{
    bool *boolean = NULL;			/* pointer to JTYPE_BOOL as decoded JSON boolean */
    bool test = false;				/* validation test result */

    /*
     * firewall - args
     */
    boolean = member_value_bool(node, depth, sem, __func__, val_err);
    if (boolean == NULL) {
	return false;
    }

    /*
     * validate decoded JSON string
     */
    test = test_Makefile_override(*boolean);
    if (test == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(41, node, depth, sem, __func__, "invalid Makefile filename");
	}
	return false;
    }

    /*
     * return validation success
     */
    if (val_err != NULL) {
	*val_err = NULL;
    }
    return true;
}


/*
 * chk_abstract - JSON semantic check for abstract
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	true ==> JSON element is valid
 *	false ==> JSON element is NOT valid, or NULL pointer, or some internal error
 */
bool
chk_abstract(struct json *node,
	     unsigned int depth, struct json_sem *sem, struct json_sem_val_err **val_err)
{
    char *str = NULL;				/* JTYPE_STRING as decoded JSON string */
    bool test = false;				/* validation test result */

    /*
     * firewall - args
     */
    str = member_value_decoded_str(node, depth, sem, __func__, val_err);
    if (str == NULL) {
	return false;
    }

    /*
     * validate decoded JSON string
     */
    test = test_abstract(str);
    if (test == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(42, node, depth, sem, __func__, "invalid abstract");
	}
	return false;
    }

    /*
     * return validation success
     */
    if (val_err != NULL) {
	*val_err = NULL;
    }
    return true;
}


/*
 * chk_affiliation - JSON semantic check for affiliation
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	true ==> JSON element is valid
 *	false ==> JSON element is NOT valid, or NULL pointer, or some internal error
 */
bool
chk_affiliation(struct json *node,
	     unsigned int depth, struct json_sem *sem, struct json_sem_val_err **val_err)
{
    struct str_or_null val;			/* report JSON JTYPE_MEMBER value */
    bool test = false;				/* validation test result */

    /*
     * firewall - args
     */
    val = member_value_str_or_null(node, depth, sem, __func__, val_err);
    if (val.valid == false) {
	return false;
    }

    /*
     * case: affiliation is null
     */
    if (val.is_null == true) {
	return true;
    }

    /*
     * validate decoded JSON string
     */
     /* paranoia */
    if (val.str == NULL) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(43, node, depth, sem, __func__,
				    "val.valid true, val.is_null false, but val.str is NULL");
	}
	return false;
    }
    test = test_affiliation(val.str);
    if (test == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(44, node, depth, sem, __func__, "invalid affiliation");
	}
	return false;
    }

    /*
     * return validation success
     */
    if (val_err != NULL) {
	*val_err = NULL;
    }
    return true;
}


/*
 * chk_author_JSON - JSON semantic check for author_JSON
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	true ==> JSON element is valid
 *	false ==> JSON element is NOT valid, or NULL pointer, or some internal error
 */
bool
chk_author_JSON(struct json *node,
	     unsigned int depth, struct json_sem *sem, struct json_sem_val_err **val_err)
{
    char *str = NULL;				/* JTYPE_STRING as decoded JSON string */
    bool test = false;				/* validation test result */

    /*
     * firewall - args
     */
    str = member_value_decoded_str(node, depth, sem, __func__, val_err);
    if (str == NULL) {
	return false;
    }

    /*
     * validate decoded JSON string
     */
    test = test_author_JSON(str);
    if (test == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(45, node, depth, sem, __func__, "invalid author_JSON");
	}
	return false;
    }

    /*
     * return validation success
     */
    if (val_err != NULL) {
	*val_err = NULL;
    }
    return true;
}


/*
 * chk_author_count - JSON semantic check for author_count
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	true ==> JSON element is valid
 *	false ==> JSON element is NOT valid, or NULL pointer, or some internal error
 */
bool
chk_author_count(struct json *node,
	     unsigned int depth, struct json_sem *sem, struct json_sem_val_err **val_err)
{
    int *value = NULL;				/* JSON_NUMBER as decoded int */
    bool test = false;				/* validation test result */

    /*
     * firewall - args
     */
    value = member_value_int(node, depth, sem, __func__, val_err);
    if (value == NULL) {
	return false;
    }

    /*
     * validate decoded JSON string
     */
    test = test_author_count(*value);
    if (test == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(46, node, depth, sem, __func__, "invalid author_count");
	}
	return false;
    }

    /*
     * return validation success
     */
    if (val_err != NULL) {
	*val_err = NULL;
    }
    return true;
}


/*
 * chk_author_handle - JSON semantic check for author_handle
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	true ==> JSON element is valid
 *	false ==> JSON element is NOT valid, or NULL pointer, or some internal error
 */
bool
chk_author_handle(struct json *node,
	     unsigned int depth, struct json_sem *sem, struct json_sem_val_err **val_err)
{
    char *str = NULL;				/* JTYPE_STRING as decoded JSON string */
    bool test = false;				/* validation test result */

    /*
     * firewall - args
     */
    str = member_value_decoded_str(node, depth, sem, __func__, val_err);
    if (str == NULL) {
	return false;
    }

    /*
     * validate decoded JSON string
     */
    test = test_author_handle(str);
    if (test == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(47, node, depth, sem, __func__, "invalid author_handle");
	}
	return false;
    }

    /*
     * return validation success
     */
    if (val_err != NULL) {
	*val_err = NULL;
    }
    return true;
}


/*
 * chk_author_number - JSON semantic check for author_number
 *
 * given:
 *	node	JSON parse node being checked
 *	depth	depth of node in the JSON parse tree (0 ==> tree root)
 *	sem	JSON semantic node triggering the check
 *	val_err	pointer to address where to place a JSON semantic validation error,
 *		NULL ==> do not report a JSON semantic validation error
 *
 * returns:
 *	true ==> JSON element is valid
 *	false ==> JSON element is NOT valid, or NULL pointer, or some internal error
 */
bool
chk_author_number(struct json *node,
		  unsigned int depth, struct json_sem *sem, struct json_sem_val_err **val_err)
{
    int *value = NULL;				/* JSON_NUMBER as decoded int */
    bool test = false;				/* validation test result */

    /*
     * firewall - args
     */
    value = member_value_int(node, depth, sem, __func__, val_err);
    if (value == NULL) {
	return false;
    }

    /*
     * validate decoded JSON string
     */
    test = test_author_number(*value);
    if (test == false) {
	if (val_err != NULL) {
	    *val_err = werr_sem_val(48, node, depth, sem, __func__, "invalid author_number");
	}
	return false;
    }

    /*
     * return validation success
     */
    if (val_err != NULL) {
	*val_err = NULL;
    }
    return true;
}