/* vim: set tabstop=8 softtabstop=4 shiftwidth=4 noexpandtab : */
/*
 * jauthchk - IOCCC JSON .author.json checker and validator
 *
 * "Because sometimes even the IOCCC Judges need some help." :-)
 */

#define JAUTHCHK_C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * Our header file - #includes the header files we need
 */
#include "jauthchk.h"


int
main(int argc, char **argv)
{
    extern char *optarg;	/* option argument */
    extern int optind;		/* argv index of the next arg */
    char *file;		/* file argument to check */
    int ret;			/* libc return code */
    int i;


    /*
     * parse args
     */
    program = argv[0];
    while ((i = getopt(argc, argv, "hv:Vqs")) != -1) {
	switch (i) {
	case 'h':		/* -h - print help to stderr and exit 0 */
	    usage(1, "-h help mode", program); /*ooo*/
	    not_reached();
	    break;
	case 'v':		/* -v verbosity */
	    /*
	     * parse verbosity
	     */
	    errno = 0;		/* pre-clear errno for errp() */
	    verbosity_level = (int)strtol(optarg, NULL, 0);
	    if (errno != 0) {
		errp(1, __func__, "cannot parse -v arg: %s error: %s", optarg, strerror(errno)); /*ooo*/
		not_reached();
	    }
	    break;
	case 'V':		/* -V - print version and exit */
	    errno = 0;		/* pre-clear errno for warnp() */
	    ret = printf("%s\n", JAUTHCHK_VERSION);
	    if (ret <= 0) {
		warnp(__func__, "printf error printing version string: %s", JAUTHCHK_VERSION);
	    }
	    exit(0); /*ooo*/
	    not_reached();
	    break;
	case 'q':
	    quiet = true;
	    break;
	case 's':
	    strict = true;
	    break;
	default:
	    usage(1, "invalid -flag", program); /*ooo*/
	    not_reached();
	 }
    }
    /* must have the exact required number of args */
    if (argc - optind != REQUIRED_ARGS) {
	usage(1, "wrong number of arguments", program); /*ooo*/
	not_reached();
    }
    file = argv[optind];
    dbg(DBG_LOW, "JSON file: %s", file);

    /*
     * Welcome
     */
    if (!quiet) {
	errno = 0;			/* pre-clear errno for errp() */
	ret = printf("Welcome to jauthchk version: %s\n", JAUTHCHK_VERSION);
	if (ret <= 0) {
	    errp(4, __func__, "printf error printing the welcome string");
	    not_reached();
	}
    /*
     * environment sanity checks
     */
	para("", "Performing sanity checks on your environment ...", NULL);
    }

    sanity_chk(file);
    if (!quiet) {
	para("... environment looks OK", "", NULL);
    }

    check_author_json(file);

    /*
     * All Done!!! - Jessica Noll, age 2
     */
    exit(author.issues != 0);
}


/*
 * sanity_chk - perform basic sanity checks
 *
 * We perform basic sanity checks on paths.
 *
 * given:
 *
 *      file        - path to JSON file to parse
 *
 * NOTE: This function does not return on error or if things are not sane.
 */
static void
sanity_chk(char const *file)
{
    /*
     * firewall
     */
    if (file == NULL) {
	err(5, __func__, "called with NULL arg");
	not_reached();
    }

    /*
     * file must be readable
     */
    if (!exists(file)) {
	fpara(stderr,
	      "",
	      "The JSON path specified does not exist. Perhaps you made a typo?",
	      "Please check the path and try again."
	      "",
	      "    jauthchk [options] <file>"
	      "",
	      NULL);
	err(6, __func__, "file does not exist: %s", file);
	not_reached();
    }
    if (!is_file(file)) {
	fpara(stderr,
	      "",
	      "The file specified, whilst it exists, is not a regular file.",
	      "",
	      "Perhaps you need to use another path:",
	      "",
	      "    jauthchk [...] <file>",
	      "",
	      NULL);
	err(7, __func__, "file is not a file: %s", file);
	not_reached();
    }
    if (!is_read(file)) {
	fpara(stderr,
	      "",
	      "The JSON path, whilst it is a file, is not readable.",
	      "",
	      "We suggest you check the permissions on the path or use another path:",
	      "",
	      "    jauthchk [...] <file>"
	      "",
	      NULL);
	err(8, __func__, "file is not readable: %s", file);
	not_reached();
    }

    return;
}


/*
 * check_author_json  - check file as .author.json
 *
 * given:
 *
 *	file	-   path to the file to check
 *
 * Attempts to validate the file as .author.json, reporting any problems found.
 *
 * Function does not return on error.
 */
static void
check_author_json(char const *file)
{
    FILE *stream;
    int ret;
    char *data;		/* .author.json contents */
    char *data_dup;	/* contents of file strdup()d */
    size_t length;	/* length of input buffer */
    char *p;


    /*
     * firewall
     */
    if (file == NULL) {
	err(9, __func__, "passed NULL arg");
	not_reached();
    }
    stream = fopen(file, "r");
    if (stream == NULL) {
	err(10, __func__, "couldn't open %s", file);
	not_reached();
    }

    /* read in the file */
    data = read_all(stream, &length);
    if (data == NULL) {
	err(11, __func__, "error while reading data in %s", file);
	not_reached();
    } else if (length == 0) {
	err(12, __func__, "zero length data in file %s", file);
	not_reached();
    }
    dbg(DBG_MED, "%s read length: %lu", file, (unsigned long)length);

    /* close the stream as we no longer need it, having read in all the file */
    errno = 0;
    ret = fclose(stream);
    if (ret != 0) {
	warnp(__func__, "error in fclose to .author.json file %s", file);
    }

    /*
     * scan for embedded NUL bytes (before EOF)
     *
     */
    errno = 0;			/* pre-clear errno for errp() */
    p = (char *)memchr(data, 0, (size_t)length);
    if (p != NULL) {
	err(13, __func__, "found NUL before EOF: %s", file);
	not_reached();
    }
   
    errno = 0;
    data_dup = strdup(data);
    if (data_dup == NULL) {
	errp(14, __func__, "unable to strdup file contents");
	not_reached();
    }

    /* verify that the very first character is a '{' */
    if (check_first_json_char(file, data_dup, strict, &p)) {
	err(15, __func__, "first character in file %s not a '{': '%c'", file, *p);
	not_reached();
    }
    dbg(DBG_MED, "first character: '%c'", *p);

    /* verify that the very last character is a '}' */
    if (check_last_json_char(file, data_dup, strict, &p)) {
	err(16, __func__, "last character in file %s not a '}': '%c'", file, *p);
	not_reached();
    }
    dbg(DBG_MED, "last character: '%c'", *p);

    /* free data */
    free(data);
    data = NULL;

    /* free strdup()d data */
    free(data_dup);
    data_dup = NULL;
}


/*
 * usage - print usage to stderr
 *
 * Example:
 *      usage(3, "missing required argument(s), program: %s", program);
 *
 * given:
 *	exitcode        value to exit with
 *	str		top level usage message
 *	program		our program name
 *
 * NOTE: We warn with extra newlines to help internal fault messages stand out.
 *       Normally one should NOT include newlines in warn messages.
 *
 * This function does not return.
 */
static void
usage(int exitcode, char const *str, char const *prog)
{
    /*
     * firewall
     */
    if (str == NULL) {
	str = "((NULL str))";
	warn(__func__, "\nin usage(): program was NULL, forcing it to be: %s\n", str);
    }
    if (prog == NULL) {
	prog = "((NULL prog))";
	warn(__func__, "\nin usage(): program was NULL, forcing it to be: %s\n", prog);
    }

    /*
     * print the formatted usage stream
     */
    vfprintf_usage(DO_NOT_EXIT, stderr, "%s\n", str);
    vfprintf_usage(exitcode, stderr, usage_msg, prog, DBG_DEFAULT, JAUTHCHK_VERSION);
    exit(exitcode); /*ooo*/
    not_reached();
}
