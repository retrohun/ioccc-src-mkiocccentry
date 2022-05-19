/* vim: set tabstop=8 softtabstop=4 shiftwidth=4 noexpandtab : */
/*
 * json_ckk - support jinfochk and jauthchk services
 *
 * "Because sometimes even the IOCCC Judges need some help." :-)
 *
 * This is currently being worked on by:
 *
 *	@xexyl
 *	https://xexyl.net		Cody Boone Ferguson
 *	https://ioccc.xexyl.net
 *
 * and
 *
 *	chongo (Landon Curt Noll, http://www.isthe.com/chongo/index.html) /\oo/\
 *
 * This is very much a work in progress!
 */


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>

/*
 * json_ckk - support jinfochk and jauthchk services
 */
#include "json_chk.h"


/*
 * XXX The code that acts on struct json_field and struct json_value and the
 * structs themselves might be removed once the JSON parser is written.  This
 * will be determined after the parser is finished. Thus much of this file might
 * be removed or dramatically changed.
 */


/*
 * Common JSON fields table used to determine if a name is a common field,
 * whether it's been added to the found_common_json_fields list, how many times
 * it has been seen and how many are allowed.
 */
struct json_field common_json_fields[] =
{
    { JSON_PARSING_DIRECTIVE_NAME,  NULL, 0, 1, false, JTYPE_STRING,	false, NULL },
    { "ioccc_contest",		    NULL, 0, 1, false, JTYPE_STRING,	false, NULL },
    { "ioccc_year",		    NULL, 0, 1, false, JTYPE_NUMBER,	false, NULL },
    { "mkiocccentry_version",	    NULL, 0, 1, false, JTYPE_STRING,	false, NULL },
    { "fnamchk_version",	    NULL, 0, 1, false, JTYPE_STRING,	false, NULL },
    { "IOCCC_contest_id",	    NULL, 0, 1, false, JTYPE_STRING,	false, NULL },
    { "entry_num",		    NULL, 0, 1, false, JTYPE_NUMBER,	false, NULL },
    { "tarball",		    NULL, 0, 1, false, JTYPE_STRING,	false, NULL },
    { "formed_timestamp",	    NULL, 0, 1, false, JTYPE_NUMBER,	false, NULL },
    { "formed_timestamp_usec",	    NULL, 0, 1, false, JTYPE_NUMBER,	false, NULL },
    { "timestamp_epoch",	    NULL, 0, 1, false, JTYPE_STRING,	false, NULL },
    { "min_timestamp",		    NULL, 0, 1, false, JTYPE_NUMBER,	false, NULL },
    { "formed_UTC",		    NULL, 0, 1, false, JTYPE_STRING,	false, NULL },
    { "test_mode",		    NULL, 0, 1, false, JTYPE_BOOL,	false, NULL },
    { NULL,			    NULL, 0, 0, false, JTYPE_UNSET,	false, NULL } /* this **MUST** be last! */
};
size_t SIZEOF_COMMON_JSON_FIELDS_TABLE = TBLLEN(common_json_fields);


/*
 * .info.json fields table used to determine if a name belongs in the file,
 * whether it's been added to the found_info_json_fields list, how many times
 * it's been seen and how many are allowed.
 */
struct json_field info_json_fields[] =
{
    { "IOCCC_info_version",	NULL, 0, 1, false, JTYPE_STRING,    false,  NULL },
    { "jinfochk_version",	NULL, 0, 1, false, JTYPE_STRING,    false,  NULL },
    { "iocccsize_version",	NULL, 0, 1, false, JTYPE_STRING,    false,  NULL },
    { "txzchk_version",		NULL, 0, 1, false, JTYPE_STRING,    false,  NULL },
    { "title",			NULL, 0, 1, false, JTYPE_STRING,    false,  NULL },
    { "abstract",		NULL, 0, 1, false, JTYPE_STRING,    false,  NULL },
    { "rule_2a_size",		NULL, 0, 1, false, JTYPE_NUMBER,    false,  NULL },
    { "rule_2b_size",		NULL, 0, 1, false, JTYPE_NUMBER,    false,  NULL },
    { "empty_override",		NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "rule_2a_override",	NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "rule_2a_mismatch",	NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "rule_2b_override",	NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "highbit_warning",	NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "nul_warning",		NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "trigraph_warning",	NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "wordbuf_warning",	NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "ungetc_warning",		NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "Makefile_override",	NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "first_rule_is_all",	NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "found_all_rule",		NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "found_clean_rule",	NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "found_clobber_rule",	NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "found_try_rule",		NULL, 0, 1, false, JTYPE_BOOL,	    false,  NULL },
    { "manifest",		NULL, 0, 1, false, JTYPE_ARRAY,	    false,  NULL },
    { "info_JSON",		NULL, 0, 1, false, JTYPE_STRING,    false,  NULL },
    { "author_JSON",		NULL, 0, 1, false, JTYPE_STRING,    false,  NULL },
    { "c_src",			NULL, 0, 1, false, JTYPE_STRING,    false,  NULL },
    { "Makefile",		NULL, 0, 1, false, JTYPE_STRING,    false,  NULL },
    { "remarks",		NULL, 0, 1, false, JTYPE_STRING,    false,  NULL },
    { "extra_file",		NULL, 0, 0, false, JTYPE_STRING,    false,  NULL },
    { NULL,			NULL, 0, 0, false, JTYPE_UNSET,	    false,  NULL } /* this **MUST** be last */
};
size_t SIZEOF_INFO_JSON_FIELDS_TABLE = TBLLEN(info_json_fields);


/*
 * .author.json fields table used to determine if a name belongs in the file,
 * whether it's been added to the found_author_json_fields list, how many times
 * it's been seen and how many are allowed.
 *
 * XXX - As of 4 March 2022 all fields are in the table but because arrays
 * are not yet parsed not all of these values will be dealt with: that is they
 * won't be checked. Additionally as of some days back (it's now 16 March 2022)
 * it was decided that a more general JSON parser should be implemented via
 * flex(1) and bison(1) so the parsers in jinfochk.c and jauthchk.c will be
 * removed and a single parser will be made (probably in this file).
 */
struct json_field author_json_fields[] =
{
    { "IOCCC_author_version",	NULL, 0, 1, false, JTYPE_STRING,	false,	NULL },
    { "jauthchk_version",	NULL, 0, 1, false, JTYPE_STRING,	false,	NULL },
    { "author_count",		NULL, 0, 1, false, JTYPE_NUMBER,	false,  NULL },
    { "authors",		NULL, 0, 1, false, JTYPE_ARRAY,		false,	NULL },
    { "name",			NULL, 0, 5, false, JTYPE_STRING,	false,  NULL },
    { "location_code",		NULL, 0, 5, false, JTYPE_STRING,	false,	NULL },
    { "email",			NULL, 0, 5, false, JTYPE_STRING,	true,	NULL },
    { "url",			NULL, 0, 5, false, JTYPE_STRING,	true,	NULL },
    { "twitter",		NULL, 0, 5, false, JTYPE_STRING,	true,	NULL },
    { "github",			NULL, 0, 5, false, JTYPE_STRING,	true,	NULL },
    { "affiliation",		NULL, 0, 5, false, JTYPE_STRING,	true,	NULL },
    { "past_winner",		NULL, 0, 1, false, JTYPE_BOOL,		true,	NULL },
    { "default_handle",		NULL, 0, 1, false, JTYPE_BOOL,		true,	NULL },
    { "author_handle",		NULL, 0, 5, false, JTYPE_STRING,	true,	NULL },
    { "author_number",		NULL, 0, 5, false, JTYPE_NUMBER,	false,	NULL },
    { NULL,			NULL, 0, 0, false, JTYPE_UNSET,		false,	NULL } /* this **MUST** be last */
};
size_t SIZEOF_AUTHOR_JSON_FIELDS_TABLE = TBLLEN(author_json_fields);

struct json_field *found_common_json_fields;

/*
 * global for jwarn(): -w in jinfochk/jauthchk says to show full warning
 *
 * XXX This currently is not used because there are some problems that have to
 * be resolved first that will take more time and thought. Before this can even
 * be done the JSON parser has to be finished and that's not close to being done
 * either.
 */
bool show_full_json_warnings = false;


/*
 * find_json_field_in_table	    - find field 'name' in json table
 *
 * given:
 *
 *	table			    - the table to check
 *	name			    - the name to check
 *	loc			    - if != NULL set to index
 *
 * NOTE: The table MUST end with NULL!
 *
 * NOTE: If loc != NULL and the name is not found in the table then in the
 * calling function loc will be the number of entries in the table. To get the
 * number of entries in the table pass in a NULL pointer or zero length name.
 *
 * This function will return NULL if the field is not in the table. It will not
 * return if table is NULL.
 */
struct json_field *
find_json_field_in_table(struct json_field *table, char const *name, size_t *loc)
{
    struct json_field *field = NULL;	/* the found field or NULL if not found */
    size_t i = 0;			/* current index */
    /*
     * firewall
     */
    if (table == NULL) {
	jerr(JSON_CODE_RESERVED(5), NULL, __func__, __FILE__, NULL, __LINE__, "passed NULL table");
	not_reached();
    }

    /* zero length name is equivalent to NULL */
    if (name != NULL && strlen(name) <= 0) {
	name = NULL;
    }

    /*
     * While the current index's name is not what we're looking for, continue to
     * the next field in the table.
     */
    for (i = 0; table[i].name != NULL; ++i) {
	if (name && !strcmp(table[i].name, name)) {
	    field = &table[i];
	    break;
	}
    }

    if (loc != NULL) {
	*loc = i;
    }

    return field;
}


/*
 * check_json_fields_tables	- check the three JSON fields tables
 *
 * This function calls the functions check_common_json_fields_table(),
 * check_info_json_fields_table() and check_author_json_fields_table() to make
 * sure that they're all sane.
 *
 * This means that the only element with the JTYPE_UNSET type is the last element,
 * that the field types are valid (see json_parse.h), that there are no embedded NULL
 * elements (name == NULL) and that the final element _IS_ NULL.
 *
 * This function does not return if any of these conditions are not met in any
 * of the tables.
 */
void
check_json_fields_tables(void)
{
    dbg(DBG_VVHIGH, "Running sanity checks on common_json_fields table ...");
    check_common_json_fields_table();
    dbg(DBG_VVHIGH, "... all OK.");

    dbg(DBG_VVHIGH, "Running sanity checks on info_json_fields table ...");
    check_info_json_fields_table();
    dbg(DBG_VVHIGH, "... all OK.");

    dbg(DBG_VVHIGH, "Running sanity checks on author_json_fields table ...");
    check_author_json_fields_table();
    dbg(DBG_VVHIGH, "... all OK.");
}


/*
 * check_common_json_fields_table - perform some sanity checks on the common_json_fields table
 *
 * This function checks if JTYPE_UNSET is used on any field other than the NULL
 * field. It also makes sure that each field_type is valid. Additionally it
 * makes sure that there are no NULL elements before the final element and that
 * the final element _IS_ NULL. It also makes sure the table is not empty.
 *
 * These sanity checks are performed on the common_json_fields table.
 *
 * This function does not return on error.
 */
void
check_common_json_fields_table(void)
{
    size_t i;
    size_t max = SIZEOF_COMMON_JSON_FIELDS_TABLE;

    if (max <= 0) {
	err(200, __func__, "bogus common_json_fields table length: %ju <= 0", (uintmax_t)max);
	not_reached();
    }

    for (i = 0; i < max-1 && common_json_fields[i].name != NULL; ++i) {
	switch (common_json_fields[i].field_type) {
	    case JTYPE_UNSET:
		if (common_json_fields[i].name != NULL) {
		    jerr(JSON_CODE_RESERVED(1), NULL, __func__, __FILE__, NULL, __LINE__,
						"found JTYPE_UNSET element with non NULL name '%s' location %ju "
						"in common_json_fields table; fix table and recompile",
                            common_json_fields[i].name, (uintmax_t)i);
		    not_reached();
		}
		break;
	    case JTYPE_NUMBER:
	    case JTYPE_BOOL:
	    case JTYPE_STRING:
	    case JTYPE_ARRAY:
		/* these are all the valid types */
		break;
	    default:
		jerr(JSON_CODE_RESERVED(2), NULL, __func__, __FILE__, NULL, __LINE__,
					    "found invalid data_type in common_json_fields table at location %ju; "
					    "fix table and recompile", (uintmax_t)i);
		not_reached();
		break;
	}
    }
    if (max - 1 != i) {
	jerr(JSON_CODE_RESERVED(3), NULL, __func__, __FILE__, NULL, __LINE__,
				    "found embedded NULL element in common_json_fields table at location %ju; "
				    "fix table and recompile", (uintmax_t)i);
	not_reached();
    }
    if (common_json_fields[i].name != NULL) {
	jerr(JSON_CODE_RESERVED(4), NULL, __func__, __FILE__, NULL, __LINE__,
				    "no final NULL element found in common_json_fields table; "
				    "fix table and recompile");
	not_reached();
    }
}


/*
 * check_info_json_fields_table	 - sanity check info_json_fields table
 *
 * This function checks if JTYPE_UNSET is used on any field other than the NULL
 * field. It also makes sure that each field_type is valid. Additionally it
 * makes sure that there are no NULL elements before the final element and that
 * the final element _IS_ NULL. It also makes sure the table is not empty.
 *
 * These sanity checks are performed on the info_json_fields table.
 *
 * This function does not return on error.
 */
void
check_info_json_fields_table(void)
{
    size_t i;
    size_t max = SIZEOF_INFO_JSON_FIELDS_TABLE;
    bool found_manifest = false;

    if (max <= 0) {
	err(201, __func__, "bogus info_json_fields table length: %ju <= 0", (uintmax_t)max);
	not_reached();
    }

    for (i = 0; i < max - 1 && info_json_fields[i].name != NULL; ++i) {
	if (info_json_fields[i].name != NULL && !strcmp(info_json_fields[i].name, "manifest")) {
	    found_manifest = true;
	}

	switch (info_json_fields[i].field_type) {
	    case JTYPE_UNSET:
		if (info_json_fields[i].name != NULL) {
		    jerr(JSON_CODE_RESERVED(1), NULL, __func__, __FILE__, NULL, __LINE__,
						"found JTYPE_UNSET element with non NULL name '%s' location %ju "
						"in info_json_fields table; fix table and recompile",
			    info_json_fields[i].name, (uintmax_t)i);
		    not_reached();
		}
		break;
	    case JTYPE_NUMBER:
	    case JTYPE_BOOL:
	    case JTYPE_STRING:
	    case JTYPE_ARRAY:
		/* these are all the valid types */
		break;
	    default:
		jerr(JSON_CODE_RESERVED(2), NULL, __func__, __FILE__, NULL, __LINE__,
					    "found invalid data_type in info_json_fields table location %ju; "
					    "fix table and recompile", (uintmax_t)i);
		not_reached();
		break;
	}
    }

    if (max - 1 != i) {
	jerr(JSON_CODE_RESERVED(3), NULL, __func__, __FILE__, NULL, __LINE__,
				    "found embedded NULL element in info_json_fields table at location %ju; "
				    "fix table and recompile", (uintmax_t)i);
	not_reached();
    }

    if (info_json_fields[i].name != NULL) {
	jerr(JSON_CODE_RESERVED(4), NULL, __func__, __FILE__, NULL, __LINE__,
				    "no final NULL element found in info_json_fields table; "
				    "fix table and recompile");
	not_reached();
    }
    if (!found_manifest) {
	err(202, __func__, "'manifest' field not found in info_json_fields table; "
			   "fix table in %s and recompile", __FILE__);
	not_reached();
    }
}


/*
 * check_author_json_fields_table - perform author_json_fields table sanity checks
 *
 * This function checks if JTYPE_UNSET is used on any field other than the NULL
 * field. It also makes sure that each field_type is valid.  Additionally it
 * makes sure that there are no NULL elements before the final element and that
 * the final element _IS_ NULL. It also makes sure the table is not empty.
 *
 * The sanity checks are performed on the author_json_fields table.
 *
 * This function does not return on error.
 */
void
check_author_json_fields_table(void)
{
    size_t i;
    size_t max = SIZEOF_AUTHOR_JSON_FIELDS_TABLE;
    bool found_authors = false;

    if (max <= 0) {
	err(203, __func__, "bogus author_json_fields table length: %ju <= 0", (uintmax_t)max);
	not_reached();
    }

    for (i = 0; i < max - 1 && author_json_fields[i].name != NULL; ++i) {
	if (author_json_fields[i].name != NULL && !strcmp(author_json_fields[i].name, "authors")) {
	    found_authors = true;
	}

	switch (author_json_fields[i].field_type) {
	    case JTYPE_UNSET:
		if (author_json_fields[i].name != NULL) {
		    jerr(JSON_CODE_RESERVED(1), NULL, __func__, __FILE__, NULL, __LINE__,
						"found JTYPE_UNSET element with non NULL name '%s' location %ju "
						"in author_json_fields table; fix table and recompile",
                            author_json_fields[i].name, (uintmax_t)i);
		    not_reached();
		}
		break;
	    case JTYPE_NUMBER:
	    case JTYPE_BOOL:
	    case JTYPE_STRING:
	    case JTYPE_ARRAY:
		/* these are all the valid types */
		break;
	    default:
		jerr(JSON_CODE_RESERVED(2), NULL, __func__, __FILE__, NULL, __LINE__,
					    "found invalid data_type in author_json_fields table location %ju; "
					    "fix table and recompile", (uintmax_t)i);
		not_reached();
		break;
	}
    }

    if (max - 1 != i) {
	jerr(JSON_CODE_RESERVED(3), NULL, __func__, __FILE__, NULL, __LINE__,
				    "found embedded NULL element in author_json_fields table at location %ju; "
				    "fix table and recompile", (uintmax_t)i);
	not_reached();
    }
    if (author_json_fields[i].name != NULL) {
	jerr(JSON_CODE_RESERVED(4), NULL, __func__, __FILE__, NULL, __LINE__,
				    "no final NULL element found in author_json_fields table; "
				    "fix table and recompile");
	not_reached();
    }
    if (!found_authors) {
	err(204, __func__, "'authors' field not found in authors_json_fields table; fix table in %s and recompile", __FILE__);
	not_reached();
    }

}


/*
 * json_filename - determine basename of JSON type
 *
 * This function will return ".info.json", ".author.json" or "null" depending on
 * the value of type.
 *
 * given:
 *
 *	type	    - INFO_JSON or AUTHOR_JSON
 *
 * Returns a char * depending on type or "null" (NOT a NULL pointer!) if invalid
 * type.
 *
 * This function never returns NULL.
 *
 */
char const *
json_filename(int type)
{
    char const *name = INVALID_JSON_FILENAME; /* "null" */

    switch (type) {
	case INFO_JSON:
	    return INFO_JSON_FILENAME; /* ".info.json" */
	    break; /* in case the return is ever removed */
	case AUTHOR_JSON:
	    return AUTHOR_JSON_FILENAME; /* ".author.json" */
	    break; /* in case the return is ever removed */
	default:
	    break; /* in case the return is ever removed ... which it ironically was. */
    }
    return name;
}


/*
 * check_first_json_char - check if first char is ch
 *
 * given:
 *
 *	file		- path to file
 *	data		- the data read in from the file
 *	first		- if != NULL set *first to the first character
 *
 * Returns 0 if first character is ch and 1 if it is not.
 *
 * Sets *first to the first character (for debugging purposes).
 *
 * Does not return on NULL.
 *
 * XXX This function will almost certainly be removed once the JSON parser is
 * complete but we need this in right now so that the current jinfochk and
 * jauthchk code does not fail. These tools could be modified but it seems to be
 * not worth the time when more important things (like working on the parser and
 * other problems) should be worked on and resolved. There are some test JSON
 * files that are in error as a misunderstanding of the JSON spec (earlier on)
 * but these will be removed in time.
 */
int
check_first_json_char(char const *file, char *data, char **first, char ch)
{
    /*
     * firewall
     */
    if (data == NULL || strlen(data) == 0) {
	err(205, __func__, "passed NULL or zero length data");
	not_reached();
    } else if (file == NULL || first == NULL) {
	err(206, __func__, "passed NULL arg(s)");
	not_reached();
    }

    /* ignore leading whitespace */
    while (*data && isspace(*data)) {
	++data;
    }

    if (first != NULL) {
	*first = data;
    }

    if (*data != ch)
	return 1;
    return 0;
}


/*
 * check_last_json_char - check if last char is ch
 *
 * given:
 *
 *	file		- path to file
 *	data		- the data read in from the file
 *	last		- if != NULL set *last to last char
 *	ch		- the char to check that the last char is
 *
 * Returns 0 if last character is ch and 1 if it is not.
 *
 * Does not return on NULL pointers.
 *
 * XXX This function will almost certainly be removed once the JSON parser is
 * complete but we need this in right now so that the current jinfochk and
 * jauthchk code does not fail. These tools could be modified but it seems to be
 * not worth the time when more important things (like working on the parser and
 * other problems) should be worked on and resolved. There are some test JSON
 * files that are in error as a misunderstanding of the JSON spec (earlier on)
 * but these will be removed in time.
 *
 * XXX On the other hand this function might be useful for something else though
 * what that might be at this time does not come to mind (and probably won't be
 * useful in the end). Still for now we need this in.
 */
int
check_last_json_char(char const *file, char *data, char **last, char ch)
{
    char *p;

    /*
     * firewall
     */
    if (data == NULL || strlen(data) == 0) {
	err(207, __func__, "passed NULL or zero length data");
	not_reached();
    } else if (file == NULL || last == NULL) {
	err(208, __func__, "passed NULL arg(s)");
	not_reached();
    }

    p = data + strlen(data) - 1;

    /* ignore trailing whitespace */
    while (*p && isspace(*p)) {
	--p;
    }

    if (last != NULL) {
	*last = p;
    }
    if (*p != ch)
	return 1;

    return 0;
}


/*
 * add_found_common_json_field - add to found_common_json_fields list
 *
 * This function will add a common JSON field name and value to the found_common_json_fields list.
 *
 * given:
 *
 *	json_filename	- json file being parsed (the path to)
 *	name		- name of the field
 *	val		- value of the field
 *	line_num	- 'line number' of the value
 *
 * Allocates a struct json_field * and add it to the found_json_common_fields
 * list. This will be used for reporting errors after parsing the file as well
 * as (later on) verifying that these fields are present in both .info.json and
 * .author.json (this detail is not yet completely thought out).
 *
 * This function returns the newly allocated struct json_field OR if the field
 * was already in the list, the previous one with the count incremented and the
 * value added to the values list.
 */
struct json_field *
add_found_common_json_field(char const *json_filename, char const *name, char const *val, int line_num)
{
    struct json_field *field; /* newly allocated field */
    struct json_field *field_in_table = NULL;
    size_t loc = 0; /* location in table */

    /*
     * firewall
     */
    if (json_filename == NULL || name == NULL || val == NULL) {
	jerr(JSON_CODE_RESERVED(6), NULL, __func__, __FILE__, NULL, __LINE__, "passed NULL arg(s)");
	not_reached();
    }

    field_in_table = find_json_field_in_table(common_json_fields, name, &loc);
    if (field_in_table == NULL) {
	jerr(JSON_CODE_RESERVED(12), NULL, __func__, json_filename, NULL, __LINE__,
				    "couldn't add value '%s' to field '%s' file %s",
				    val, name, json_filename);
	err(209, __func__, "called add_found_common_json_field() on uncommon field '%s'", name);
	not_reached();
    }
    /*
     * Set in table that it's found and increment the number of times it's been
     * seen.
     */
    common_json_fields[loc].found = true;
    common_json_fields[loc].count++;
    for (field = found_common_json_fields; field != NULL; field = field->next) {
	if (field->name && !strcmp(field->name, name)) {
	    field->count++;
	    if (add_json_value(json_filename, field, val, line_num) == NULL) {
		jerr(JSON_CODE_RESERVED(7), NULL, __func__, __FILE__, NULL, __LINE__,
					    "couldn't add value '%s' to field '%s' file %s",
					    val, field->name, json_filename);
		not_reached();
	    }
	    return field;
	}
    }

    field = new_json_field(json_filename, name, val, line_num);
    if (field == NULL) {
	/* this should NEVER be reached but we check just to be sure */
	jerr(JSON_CODE_RESERVED(8), NULL, __func__, __FILE__, NULL, __LINE__,
				    "error creating new struct json_field * for field '%s' value '%s' file %s",
				    name, val, json_filename);
	not_reached();
    }

    /* add field to found_common_json_fields list */
    field->next = found_common_json_fields;
    found_common_json_fields = field;

    dbg(DBG_HIGH, "added field '%s' value '%s' to found_common_json_fields", field->name, val);
    return field;
}


/*
 * add_common_json_field - add name, if it is common, to the common list
 *
 * This function will determine of a name is common to both the .info.json
 * as well as to .author.json.  If it is, then it will be added to the
 * common list.
 *
 * given:
 *
 *	program		- which util called this (jinfochk or jauthchk)
 *	json_filename	- the file being parsed (path to)
 *	name		- the field name
 *	val		- the value of the field
 *	line_num	- the 'line number' of the value
 *
 * returns:
 *	true ==> if the name is common to both files and the field was added to
 *		 the found list
 *	false ==> if it's not one of the common field names
 *
 * NOTE: Does not return on error (NULL pointers).
 */
bool
add_common_json_field(char const *program, char const *json_filename, char *name, char *val, int line_num)
{
    bool ret = true;	/* return value: true ==> known field and added to list, false ==> not a common field */
    struct json_field *field = NULL; /* the field in the common_json_fields table if found */
    size_t loc = 0; /* location in the common_json_fields table */

    /*
     * firewall
     */
    if (program == NULL || json_filename == NULL || name == NULL || val == NULL) {
	err(210, __func__, "passed NULL arg(s)");
	not_reached();
    }

    /*
     * check if the name is an expected common field
     */
    field = find_json_field_in_table(common_json_fields, name, &loc);
    if (field != NULL) {
	dbg(DBG_HIGH, "found common field '%s' value '%s'", field->name, val);
	add_found_common_json_field(json_filename, field->name, val, line_num);
    } else {
	ret = false;
    }
    return ret;
}


/*
 * check_found_common_json_fields - found_common_json_fields table checks
 *
 * The found_common_json_fields table will be checked to determine if
 * all fields have a valid value. It also checks that the expected fields are in
 * the file.
 *
 * given:
 *
 *	program		- which util called this (jinfochk or jauthchk)
 *	json_filename	- the file being parsed (path to)
 *	fnamchk		- path to fnamchk util
 *
 * returns:
 *	>0 ==> the number of issues found
 *	0 ==> if no issues were found
 *
 * NOTE: Does not return on error (NULL pointers).
 */
int
check_found_common_json_fields(char const *program, char const *json_filename, char const *fnamchk, bool test)
{
    int year = 0;	/* ioccc_year: IOCCC year as an integer */
    int entry_num = -1;	/* entry_num: entry number as an integer */
    long ts = 0;	/* formed_timestamp_usec: microseconds as an integer */
    struct tm tm;	/* formed_timestamp: formatted as a time structure */
    int exit_code = 0;	/* tarball: exit code from fnamchk command */
    int issues = 0;	/* number of issues found */
    char *p;
    struct json_field *field; /* current field in found_common_json_fields list */
    struct json_value *value; /* current value in current field's values list */
    struct json_field *common_field = NULL; /* element in the common_json_fields table */
    struct json_field *tarball_field = NULL; /* for checks after initial loop */
    char *tarball_val = NULL; /* for checks after initial loop */
    char *entry_num_val = NULL; /* for checks after initial loop */
    char *contest_id_val = NULL; /* for checks after initial loop */
    char *formed_timestamp_val = NULL; /* for checks after initial loop */
    size_t loc = 0;	/* location in the common_json_fields table */
    size_t val_length = 0; /* current value length */
    bool test_mode = false; /* will be set to true if test_mode value is true */

    /*
     * firewall
     */
    if (program == NULL || json_filename == NULL || fnamchk == NULL) {
	err(211, __func__, "passed NULL arg(s)");
	not_reached();
    }

    /*
     * pre-clear time before strptime() use
     */
    memset(&tm, 0, sizeof tm);

    for (field = found_common_json_fields; field != NULL; field = field->next) {
	/*
	 * process a given common field
	 */

	/*
	 * first make sure the name != NULL and strlen() > 0
	 */
	if (field->name == NULL || !strlen(field->name)) {
	    jerr(JSON_CODE_RESERVED(9), NULL, __func__, __FILE__, NULL, __LINE__,
					"found NULL or empty field in found_common_json_fields list in file %s",
					json_filename);
	    not_reached();
	}

	/* now make sure it's allowed to be in this table. */
	loc = 0;
	common_field = find_json_field_in_table(common_json_fields, field->name, &loc);

	/*
	 * If the field is not allowed in the list then it suggests there is a
	 * problem in the code because only common fields should be added to the
	 * list in the first place. Thus it's an error if a field that's in the
	 * common list is not a common field name.
	 */
	if (common_field == NULL) {
	    jerr(JSON_CODE_RESERVED(10), NULL, __func__, __FILE__, NULL, __LINE__,
					 "illegal field name '%s' in found_common_json_fields list in file %s",
					 field->name, json_filename);
	    not_reached();
	}

	/*
	 * Next we test if the field has been seen more times than allowed. In
	 * the case of the common fields each is only allowed once but in
	 * uncommon fields some are allowed more than once.
	 */
	for (value = field->values; value != NULL; value = value->next) {
	    char *val = value->value;

	    if (val == NULL) {
		jerr(JSON_CODE_RESERVED(11), NULL, __func__, __FILE__, NULL, __LINE__,
					     "NULL pointer val for field '%s' in file %s",
					     field->name, json_filename);
		not_reached();
	    }

	    if (common_field->max_count > 0 && common_field->count > common_field->max_count) {
		jwarn(JSON_CODE(1), program, __func__, __FILE__, NULL, value->line_num,
				    "field '%s' found %ju times but is only allowed %ju time%s in file %s",
				    common_field->name, (uintmax_t)common_field->count,
				    (uintmax_t)common_field->max_count, common_field->max_count==1?"":"s",
				    json_filename);
		++issues;
	    }


	    val_length = strlen(val);

	    if (val_length <= 0) {
		warn(__func__, "empty value found for field '%s' in file %s: '%s'", field->name, json_filename, val);
		/* don't increase issues because the below checks will do that
		 * too: this warning only notes the reason the test will fail.
		 */
	    }
	    /* first we do checks on the field type */
	    switch (common_field->field_type) {
		case JTYPE_BOOL:
		    if (strcmp(val, "false") && strcmp(val, "true")) {
			warn(__func__, "bool field '%s' has non boolean value in file %s: '%s'",
				       common_field->name, json_filename, val);
			++issues;
			continue;
		    }
		    break;
		case JTYPE_NUMBER:
		    if (!is_decimal(val, val_length)) {
			warn(__func__, "number field '%s' has non-number value in file %s: '%s'",
				       common_field->name, json_filename, val);
			++issues;
			continue;
		    }
		    break;
		default:
		    break;
	    }

	    if (!strcmp(field->name, "ioccc_contest")) {
		if (strcmp(val, IOCCC_CONTEST)) {
		    warn(__func__, "ioccc_contest != \"%s\" in file %s: \"%s\"", IOCCC_CONTEST, json_filename, val);
		    ++issues;
		}
	    } else if (!strcmp(field->name, "ioccc_year")) {
		year = string_to_int(val);
		if (year != IOCCC_YEAR) {
		    warn(__func__, "ioccc_year != IOCCC_YEAR %d in file %s: '%s' (%d)", IOCCC_YEAR, json_filename, val, year);
		    ++issues;
		}
	    } else if (!strcmp(field->name, "mkiocccentry_version")) {
		if (!test && strcmp(val, MKIOCCCENTRY_VERSION)) {
		    warn(__func__, "mkiocccentry_version != MKIOCCCENTRY_VERSION \"%s\" in file %s: \"%s\"",
				   MKIOCCCENTRY_VERSION, json_filename, val);
		    ++issues;
		}
	    } else if (!strcmp(field->name, "fnamchk_version")) {
		if (!test && strcmp(val, FNAMCHK_VERSION)) {
		    warn(__func__, "fnamchk_version != FNAMCHK_VERSION \"%s\" in file %s: \"%s\"",
				   FNAMCHK_VERSION, json_filename, val);
		    ++issues;
		}
	    } else if (!strcmp(field->name, "IOCCC_contest_id")) {
		/*
		 * We need this for verifying that the tarball matches the contest id, entry num and formed timestamp
		 */
		contest_id_val = contest_id_val != NULL ? contest_id_val : val;
		if (!valid_contest_id(val)) {
		    warn(__func__, "IOCCC_contest_id is invalid in file %s: \"%s\"", json_filename, val);
		    ++issues;
		}
	    } else if (!strcmp(field->name, "min_timestamp")) {
		ts = string_to_long(val);
		if (ts != MIN_TIMESTAMP) {
		    warn(__func__, "min_timestamp != MIN_TIMESTAMP '%ld' in file %s: '%s' (%ld)",
				   MIN_TIMESTAMP, json_filename, val, ts);
		    ++issues;
		}
	    } else if (!strcmp(field->name, "timestamp_epoch")) {
		if (strcmp(val, TIMESTAMP_EPOCH)) {
		    warn(__func__, "timestamp_epoch != TIMESTAMP_EPOCH \"%s\" in file %s: \"%s\"",
				   TIMESTAMP_EPOCH, json_filename, val);
		    ++issues;
		}
	    } else if (!strcmp(field->name, "formed_timestamp_usec")) {
		errno = 0;
		ts = string_to_long(val);
		if (ts < 0 || ts > 999999) {
		    warnp(__func__, "formed_timestamp_usec out of range of >= 0 && <= 999999 in file %s: '%ld'", json_filename, ts);
		    ++issues;
		}
	    } else if (!strcmp(field->name, "entry_num")) {
		/*
		 * We need this for verifying that the tarball matches the contest id, entry num and formed timestamp
		 */
		entry_num_val = entry_num_val != NULL ? entry_num_val : val;
		entry_num = string_to_int(val);
		if (!(entry_num >= 0 && entry_num <= MAX_ENTRY_NUM)) {
		    warn(__func__, "entry number out of range in file %s: %d", json_filename, entry_num);
		    ++issues;
		}
	    } else if (!strcmp(field->name, "formed_UTC")) {
		p = strptime(val, FORMED_UTC_FMT, &tm);
		if (p == NULL) {
		    warn(__func__, "formed_UTC does not match FORMED_UTC_FMT \"%s\": \"%s\"", FORMED_UTC_FMT, val);
		    ++issues;
		}
	    } else if (!strcmp(field->name, "formed_timestamp")) {
		/*
		 * We need this for verifying that the tarball matches the contest id, entry num and formed timestamp
		 */
		formed_timestamp_val = formed_timestamp_val != NULL ? formed_timestamp_val : val;
		ts = string_to_long(val);
		if (ts < MIN_TIMESTAMP) {
		    warn(__func__, "formed_timestamp < MIN_TIMESTAMP '%ld' in file %s: '%s' (%ld)",
				   MIN_TIMESTAMP, json_filename, val, ts);
		    ++issues;
		}
	    } else if (!strcmp(field->name, "tarball")) {
		/*
		 * We have to do special tests on this after we find if
		 * test_mode == true
		 */
		tarball_field = field;
		/*
		 * We need this for verifying that the tarball matches the contest id, entry num and formed timestamp
		 */
		tarball_val = tarball_val != NULL ? tarball_val: val;
	    } else if (!strcmp(field->name, "test_mode")) {
		/*
		 * Only set to the value if !test_mode: this is so that if
		 * there's more than one of "test_mode" in the file it will not
		 * set back to false if already seen as true (we start out as
		 * false so this isn't a problem).
		 *
		 * Note that if this field is seen more than once in the file
		 * that still will be reported after the loop.
		 */
		if (!test_mode) {
		    test_mode = strtobool(val);
		    dbg(DBG_LOW, "set test_mode to %s", test_mode?"true":"false");
		}

	    } else {
		/*
		 * This should never actually be reached but if it is it
		 * suggests that a field was not added to be checked: if it's
		 * not a valid field we would have already detected and aborted
		 * earlier in this loop so we don't have to check for that.
		 */
		warn(__func__, "found unhandled common field in file %s: '%s'", json_filename, field->name);
		/*
		 * NOTE: Don't increment issues because this doesn't mean
		 * there's anything wrong with the json file but rather that the
		 * field isn't verified.
		 */
	    }

	}
    }

    /*
     * Now that we've checked each field by name, we still have to make sure
     * that each field expected is actually there. Note that in the above loop
     * we already tested if each field has been seen more times than allowed so
     * we don't do that here. This is because the fields that are in the list
     * are those that will potentially have more than allowed whereas here we're
     * making sure every field that is required is actually in the list.
     */
    for (loc = 0; common_json_fields[loc].name != NULL; ++loc) {
	if (!common_json_fields[loc].found && common_json_fields[loc].max_count > 0) {
	    warn(__func__, "required field not found in found_common_json_fields list in file %s: '%s'",
			   json_filename, common_json_fields[loc].name);
	    ++issues;
	}
    }

    /* test consistency of test_mode and tarball field */
    if (tarball_field == NULL) {
	warn(__func__, "required field not found in found_common_json_fields list in file %s: 'tarball'", json_filename);
	++issues;
    } else if (test_mode) {
	/*
	 * test_mode was set: execute the fnamchk command with -t.
	 *
	 * Note: Although having the tarball path more than once is considered
	 * an issue we still check each value to be complete.
	 */
	for (value = tarball_field->values; value != NULL; value = value->next) {
	    char const *val = value->value;

	    exit_code = shell_cmd(__func__, true, "% -t % >/dev/null", fnamchk, val);
	    if (exit_code != 0) {
		warn(__func__, "%s: %s %s > /dev/null: failed with exit code: %d",
				    program, fnamchk, val, WEXITSTATUS(exit_code));
		++issues;
	    }
	}
    } else {
	/*
	 * test_mode false: execute the fnamchk command with -u.
	 *
	 * Note: Although having the tarball path more than once is considered
	 * an issue we still check each value to be complete.
	 */
	for (value = tarball_field->values; value != NULL; value = value->next) {
	    char const *val = value->value;

	    exit_code = shell_cmd(__func__, true, "% -u % >/dev/null", fnamchk, val);
	    if (exit_code != 0) {
		warn(__func__, "%s: %s %s > /dev/null: failed with exit code: %d",
				    program, fnamchk, val, WEXITSTATUS(exit_code));
		++issues;
	    }
	}

    }

    /*
     * We have to verify that tarball matches the contest id, entry num and
     * formed timestamp.
     *
     * NOTE: This resolves test_JSON/info.json/bad/info.tarball-0.json as well
     * as some others.
     */
    if (tarball_val != NULL && contest_id_val != NULL && entry_num_val != NULL && formed_timestamp_val != NULL) {
	int ret;
	char *str = NULL;

	errno = 0;
	str = calloc(1, strlen(tarball_val) + strlen(contest_id_val) + strlen(entry_num_val) + strlen(formed_timestamp_val) + 1);
	if (str == NULL) {
	    err(212, __func__, "couldn't allocate memory to verify that contest_id and entry_num matches the tarball");
	    not_reached();
	}

	errno = 0;
	ret = sprintf(str, "entry.%s-%s.%s.txz", contest_id_val, entry_num_val, formed_timestamp_val);
	if (ret <= 0) {
	    warn(__func__, "couldn't write entry.%s-%s.%s.txz to string for tarball match test",
			   contest_id_val, entry_num_val, formed_timestamp_val);
	    ++issues;
	} else {
	    if (strcmp(str, tarball_val)) {
		warn(__func__, "tarball '%s' does not match entry.%s-%s.%s.txz",
			       tarball_val, contest_id_val, entry_num_val, formed_timestamp_val);
		++issues;
	    }
	}
	free(str);
	str = NULL;
    } else {
	warn(__func__, "couldn't verify tarball matches IOCCC_contest_id, entry_num and "
		       "formed_timestamp due to one or more missing fields");
	++issues;
    }

    return issues;
}


/*
 * new_json_field - allocate a new json_field with the value passed in
 *
 * given:
 *
 *	name	    - the field name
 *	val	    - the field's value
 *	line_num    - the 'line number' of the value
 *
 * Returns the newly allocated struct json_field *.
 *
 * NOTE: This function does not return on NULL and it does not check any list:
 * as long as no NULL pointers are encountered it will return a newly allocated
 * struct json_field *. This means that it is the caller's responsibility to
 * check if the field is already in the list.
 */
struct json_field *
new_json_field(char const *json_filename, char const *name, char const *val, int line_num)
{
    struct json_field *field; /* the new field */

    /*
     * firewall
     */
    if (json_filename == NULL || name == NULL || val == NULL) {
	err(213, __func__, "passed NULL arg(s)");
	not_reached();
    }

    errno = 0;
    field = calloc(1, sizeof *field);
    if (field == NULL) {
	errp(214, __func__, "error allocating new struct json_field * for field '%s' and value '%s': %s",
		  name, val, strerror(errno));
	not_reached();
    }

    errno = 0;
    field->name = strdup(name);
    if (field->name == NULL) {
	errp(215, __func__, "unable to strdup() field name '%s': %s", name, strerror(errno));
	not_reached();
    }

    if (add_json_value(json_filename, field, val, line_num) == NULL) {
	err(216, __func__, "error adding value '%s' to field '%s'", val, name);
	not_reached();
    }

    field->count = 1;

    return field;
}


/*
 * add_json_value - add a value to a struct json_field
 *
 * given:
 *
 *	json_filename	- path to the json file being parsed
 *	field		- the field to add to
 *	val		- the value to add
 *	line_num	- the 'line number' of the value
 *
 * This function returns the newly allocated struct json_value * with the value
 * strdup()d and added to the struct json_field * values list.
 *
 * NOTE: If the value of the field is already in the field we still add the
 * value.
 *
 * This function does not return on error.
 *
 */
struct json_value *
add_json_value(char const *json_filename, struct json_field *field, char const *val, int line_num)
{
    struct json_value *new_value = NULL;    /* the newly allocated value */
    struct json_value *value = NULL;	    /* the current list of values in field */

    /*
     * firewall
     */
    if (json_filename == NULL || field == NULL || val == NULL) {
	err(217, __func__, "passed NULL arg(s)");
	not_reached();
    }

    /* allocate new json_value */
    errno = 0;
    new_value = calloc(1, sizeof *new_value);
    if (new_value == NULL) {
	errp(218, __func__, "error allocating new value '%s' for field '%s' in file %s: %s",
		  val, field->name, json_filename, strerror(errno));
	not_reached();
    }
    errno = 0;
    new_value->value = strdup(val);
    if (new_value->value == NULL) {
	errp(219, __func__, "error strdup()ing value '%s' for field '%s': %s", val, field->name, strerror(errno));
	not_reached();
    }

    new_value->line_num = line_num;

    /* find end of list */
    for (value = field->values; value != NULL && value->next != NULL; value = value->next)
	; /* satisfy warnings */

    /* append new value to values list (field->values) */
    if (!value) {
	field->values = new_value;
    } else {
	value->next = new_value;
    }

    return new_value;
}


/*
 * free_json_field_values - free a field's values list
 *
 * given:
 *
 *	field			- field to free the values list
 *
 * Returns void.
 *
 * NOTE: This function does not return on error (NULL field passed in).
 */
void
free_json_field_values(struct json_field *field)
{
    struct json_value *value, *next_value; /* to free the values of the field */

    /*
     * firewall
     */
    if (field == NULL) {
	err(220, __func__, "passed NULL field");
	not_reached();
    }

    /* free each value */
    for (value = field->values; value != NULL; value = next_value) {
	next_value = value->next;

	if (value->value != NULL) {
	    free(value->value);
	    value->value = NULL;
	}
	value = NULL;
    }
}


/*
 * free_found_common_json_fields  - free the common json fields list
 *
 * This function returns void.
 */
void
free_found_common_json_fields(void)
{
    struct json_field *field, *next_field;


    for (field = found_common_json_fields; field != NULL; field = next_field) {
	next_field = field->next;

	if (field->name) {
	    free(field->name);
	    field->name = NULL;
	}
	if (field->values) {
	    free_json_field_values(field);
	    field->values = NULL; /* redundant but for safety anyway */
	}
	free(field);
	field = NULL;
    }

    found_common_json_fields = NULL;
}


/*
 * free_json_field	- release memory in a struct json_field *
 *
 * given:
 *
 *	field	    - the field to free
 *
 * NOTE: This function does not return on NULL.
 *
 * NOTE: It is the caller's responsibility to remove it from the list(s) it is in!
 */
void
free_json_field(struct json_field *field)
{
    /*
     * firewall
     */
    if (field == NULL) {
	err(221, __func__, "passed NULL field");
	not_reached();
    }

    /* free the field's values list */
    free_json_field_values(field);
    field->values = NULL; /* redundant but for safety */

    if (field->name) {
	free(field->name);
	field->name = NULL;
    }

    free(field);
    field = NULL;
}
