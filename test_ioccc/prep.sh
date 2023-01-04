#!/usr/bin/env bash
#
# prep.sh - prep for a release - actions for make prep and make release
#
# Copyright (c) 2022-2023 by Landon Curt Noll.  All Rights Reserved.
#
# Permission to use, copy, modify, and distribute this software and
# its documentation for any purpose and without fee is hereby granted,
# provided that the above copyright, this permission notice and text
# this comment, and the disclaimer below appear in all of the following:
#
#       supporting documentation
#       source copies
#       source works derived from this source
#       binaries derived from this source or from derived source
#
# LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
# INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
# EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
# CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
# USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
#
# chongo (Landon Curt Noll, http://www.isthe.com/chongo/index.html) /\oo/\
#
# Share and enjoy! :-)

# setup
#
export LOG_FILE=
export PREP_VERSION="0.1 2022-04-19"
export USAGE="usage: $0 [-h] [-v level] [-V] [-e] [-o] [-m make] [-M Makefile] [-l logfile]

    -h              print help and exit
    -v level        flag ignored
    -V              print version and exit

    -e		    exit in first make action error (def: exit only at end)
    -o		    do NOT use backup files, fail if bison or flex cannot be used (def: use)
    -m make	    make command (def: make)
    -M Makefile	    path to Makefile (def: ./Makefile)
    -l logfile      write details of actions to logfile (def: send to stdout)

Exit codes:
     0   all is OK for both bison and flex
     1   -h and help string printed or -V and version string printed
     2	 command line error
     3	 Makefile not a readable file that exists
     4	 Could not make writable log file
 >= 10   some make action exited non-zero

$0 version: $PREP_VERSION"
export MAKE="make"
export MAKEFILE="./Makefile"
export V_FLAG="0"
export E_FLAG=
export EXIT_CODE="0"
export O_FLAG=

# parse args
#
while getopts :hv:Veom:M:l: flag; do
    case "$flag" in
    h)	echo "$USAGE" 1>&2
	exit 1
	;;
    v)	V_FLAG="$OPTARG";
	;;
    V)	echo "$PREP_VERSION"
	exit 1
	;;
    m)	MAKE="$OPTARG";
	;;
    M)	MAKEFILE="$OPTARG";
	;;
    e)	E_FLAG="-e"
	;;
    o)	O_FLAG="-o"
	;;
    l)	LOG_FILE="$OPTARG"
	;;
    \?) echo "$0: ERROR: invalid option: -$OPTARG" 1>&2
	exit 2
	;;
    :)	echo "$0: ERROR: option -$OPTARG requires an argument" 1>&2
	exit 2
	;;
    *)
	;;
    esac
done

# check args
#
shift $(( OPTIND - 1 ));
if [[ $# -ne 0 ]]; then
    echo "$0: ERROR: expected 0 arguments, found $#" 1>&2
    exit 2
fi

# firewall
#
if [[ ! -e $MAKEFILE ]]; then
    echo "$0: ERROR: Makefile not found: $MAKEFILE" 1>&2
    exit 3
fi
if [[ ! -f $MAKEFILE ]]; then
    echo "$0: ERROR: Makefile not a regular file: $MAKEFILE" 1>&2
    exit 3
fi
if [[ ! -r $MAKEFILE ]]; then
    echo "$0: ERROR: Makefile not a readable file: $MAKEFILE" 1>&2
    exit 3
fi

# if -l logfile was specified, remove it and recreate it to start out empty
#
if [[ -n "$LOG_FILE" ]]; then
    rm -f "$LOG_FILE"
    touch "$LOG_FILE"
    if [[ ! -f "${LOG_FILE}" ]]; then
	echo "$0: ERROR: couldn't create log file" 1>&2
	exit 4
    fi
    if [[ ! -w "${LOG_FILE}" ]]; then
	echo "$0: ERROR: log file not writable" 1>&2
	exit 4
    fi
fi

# write_echo - write a message to either the log file or both the log file and
# stdout
#
write_echo()
{
    local MSG="$*"

    if [[ -n "$LOG_FILE" ]]; then
	echo "$MSG" | tee -a -- "$LOG_FILE"
    else
	echo "$MSG" 1>&2
    fi
}

# exec_command - invoke command redirecting output only to the log file or to
# both stdout and the log file
exec_command()
{
    local COMMAND=$*
    if [[ -n "$LOG_FILE" ]]; then
	command ${COMMAND} >> "$LOG_FILE"
	return $?
    else
	command ${COMMAND} 2>&1
	return $?
    fi

}

# make action
#
# usage:
#	make_action code rule
#
#	code - exit code of rule fails
#	rule - Makefile rule to call
#
make_action() {

    # parse args
    #
    if [[ $# -ne 2 ]]; then
	echo "$0: ERROR: function expects 2 args, found $#" 1>&2
	exit 3
    fi
    local CODE="$1"
    local RULE="$2"

    # announce pre-action
    #

    write_echo "=-=-= Start: $MAKE $RULE =-=-="
    write_echo

    # perform action
    #
    write_echo "$MAKE" -f "$MAKEFILE" "$RULE"
    exec_command "$MAKE" -f "$MAKEFILE" "$RULE"
    status="$?"
    if [[ $status -ne 0 ]]; then

	# process a make action failure
	#
	EXIT_CODE="$CODE"
	write_echo
	write_echo "$0: Warning: EXIT_CODE is now: $EXIT_CODE" 1>&2
	if [[ -n $E_FLAG ]]; then
	    write_echo
	    write_echo "$0: ERROR: $MAKE -f $MAKEFILE $RULE exit status: $status" 1>&2
	    write_echo
	    write_echo "=-=-= FAIL: $MAKE $RULE =-=-="
	    write_echo
	    exit "$EXIT_CODE"
	else
	    write_echo
	    write_echo "$0: Warning: $MAKE -f $MAKEFILE $RULE exit status: $status" 1>&2
	    write_echo
	    write_echo "=-=-= FAIL: $MAKE $RULE =-=-="
	    write_echo
	fi

    # announce post-action
    #
    else
	write_echo
	write_echo "=-=-= PASS: $MAKE $RULE =-=-="
	write_echo
    fi
    return 0;
}

# perform make actions
#
write_echo "=-=-=-=-= Start: $0 =-=-=-=-="
write_echo
make_action 10 clobber
make_action 11 all
make_action 12 depend
make_action 13 clean_mkchk_sem
make_action 14 all_sem_ref
make_action 15 mkchk_sem
make_action 16 all
if [[ -n $O_FLAG ]]; then
    make_action 17 parser-o
else
    make_action 18 parser
fi
make_action 19 all
make_action 20 load_json_ref
make_action 21 use_json_ref
make_action 22 clean_generated_obj
make_action 23 all
make_action 24 bug_report-txl
make_action 25 shellcheck
make_action 26 seqcexit
make_action 27 picky
make_action 28 tags
make_action 29 test
if [[ $EXIT_CODE -eq 0 ]]; then
    write_echo "=-=-=-=-= PASS: $0 =-=-=-=-="
    write_echo
else
    write_echo "=-=-=-=-= FAIL: $0 =-=-=-=-="
    write_echo
    write_echo "=-=-=-=-= Will exit: $EXIT_CODE =-=-=-=-="
    write_echo
fi

# All Done!!! -- Jessica Noll, Age 2
#
exit "$EXIT_CODE"
