/*
 * fnamchk - IOCCC compressed tarball filename sanity check tool
 *
 * "Because even fprintf has a return value worth paying attention to." :-)
 *
 * Copyright (c) 2022 by Landon Curt Noll.  All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright, this permission notice and text
 * this comment, and the disclaimer below appear in all of the following:
 *
 *       supporting documentation
 *       source copies
 *       source works derived from this source
 *       binaries derived from this source or from derived source
 *
 * LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * chongo (Landon Curt Noll, http://www.isthe.com/chongo/index.html) /\oo/\
 *
 * "Because everyone hates when someone gets their name wrong." :-)
 *
 * Share and enjoy! :-)
 */


#if !defined(INCLUDE_FNAMCHK_H)
#    define  INCLUDE_FNAMCHK_H


/*
 * dbg - info, debug, warning, error, and usage message facility
 */
#include "../dbg/dbg.h"

/*
 * jparse - the parser
 */
#include "../jparse/jparse.h"

/*
 * limit_ioccc - IOCCC size and rule related limitations
 */
#include "../soup/limit_ioccc.h"

/*
 * utf8_posix_map - translate UTF-8 into POSIX portable filename and + chars
 */
#include "../soup/utf8_posix_map.h"


/*
 * usage message
 *
 * Use the usage() function to print the usage_msg([0-9]?)+ strings.
 */
static const char * const usage_msg =
    "usage: %s [-h] [-v level] [-q] [-V] [-E ext] [-t|-u] filepath\n"
    "\n"
    "\t-h\t\t\tprint help message and exit\n"
    "\t-v level\t\tset verbosity level: (def level: %d)\n"
    "\t-q\t\t\tquiet mode: silence msg(), warn(), warnp() if -v 0 (def: not quiet)\n"
    "\t-V\t\t\tprint version string and exit\n"
    "\t-E ext\t\t\tchange extension to test (def: txz)\n"
    "\t-t\t\t\tfilename must match test entry filename\n"
    "\t-u\t\t\tfilename must match real entry filename\n"
    "\n"
    "\t\tNOTE: -t and -u cannot be used together.\n\n"
    "\tfilepath\t\tpath to an IOCCC compressed tarball\n"
    "\n"
    "Exit codes:\n"
    "     0   all OK\n"
    "     1   \n"
    "     2   -h and help string printed or -V and version string printed\n"
    "     3   command line error\n"
    "	  4   first '-' separated token length != %ju\n"
    "	 11   second '-' separated token length != %ju\n"
    "  >=10   internal error\n"
    "fnamchk version: %s\n";


/*
 * globals
 */


/*
 * forward declarations
 */
static void usage(int exitcode, char const *name, char const *str) __attribute__((noreturn));


#endif /* INCLUDE_FNAMCHK_H */
