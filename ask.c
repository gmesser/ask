/* ask.c - Ask a question and get a one-letter response using console I/O. */

/*
 ******************************************************************************

 ask

 Description: Ask a question, get a single-key response.

 ------------------------------------------------------------------------------

 Revision History:

 The ask program has come full-circle, from C to C++ and back to C.
 Now builds on Linux (gcc), macOS (clang), and Windows (Visual C++).

 11/20/1988 - ask.c in Microsoft C V5.00 for MS-DOS 3.10.
 04/27/1993 - ask.cpp in Borland C++ v4.52.
 10/16/1995 - Bug fixes and usage enhancements.
 09/06/2002 - ask.cpp in Microsoft Visual C++ 6.0.
 06/07/2005 - ask.c in Microsoft Visual C++ .NET 2003.
 06/29/2017 - ask.c in gcc.
 08/14/2017 - Separated the functions, creating ask_funcs for general use.
 08/05/2021 - Changed the return value so scripts can better detect actions.
 08/10/2021 - Added -c and -C to use case-sensitive keys.
 06/19/2022 - Some refactoring.
 07/08/2022 - Cleaned up.

 ******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "ask_funcs.h"

/* local function(s) */

void help(void);

/* global variables */

extern int case_sensitive_mode;

const char *Program_Name = "ask";
const char *Release = "Build: " __DATE__ ", " __TIME__;

const char *helpmsg = {
	"The ask program prompts the user and accepts a single-key response.\n"
	"\n"
    "Usage:  ask [options] \"prompt string\" responses\n"
	"\n"
	"Options:\n"
    "The options can appear anywhere on the command line.\n"
	"-c, -C, --case-sensitive   Case-sensitive response matching.\n"
	"-h, -H, --help             Show this help screen.\n"
    "\n"
    "The \"prompt string\" and responses are positional parameters.\n"
    "They must appear in order.\n"
    "\"prompt string\"  The prompt string the user sees\n"
    "responses          The characters the user is allowed to press to\n"
    "                   answer the prompt (Not a comma-separated list.)\n"
	"\n"
	"Return value (system exit code):\n"
	"   0   The user entered a response that was not in the list.\n"
	" 1-125 The index of the choice in the responses.\n"
	"       (The first response is 1, the next is 2, and so on.)\n"
	"  126  The user pressed Enter without making a choice.\n"
	"  127  The user pressed Escape.\n"
	"\n"
    "Usage Notes:\n"
	"* The user must press Enter after pressing a key.\n"
	"* The response is not case-sensitive by default.  Use -c if necessary.\n"
	"* If the user presses more than one key, the first key will be used.\n"
	"* The escape sequences \\\\, \\a, \\n, \\r, and \\t are allowed in the\n"
    "  prompt string.\n"
	"\n"
	"Example:\n"
	"  ask \"** Answer [Y]es, [N]o, or [M]aybe: \" YNM\n"
	"\n"
    "The example displays the following prompt, and reads the user's response:\n"
	"\n"
    "** Answer [Y]es, [N]o, or [M]aybe: \n"
    "\n"
    "The example returns:\n"
    " Exit code 1 if the user pressed y or Y.\n"
    " Exit code 2 if the user pressed n or N. \n"
    " Exit code 3 if the user pressed m or M.\n"
    " Exit code 0 if the user pressed a key that was not y, Y, n, N, m, or M.\n"
    " Exit code 126 if the user pressed Enter without pressing a key.\n"
    " Exit code 127 if the user pressed Escape.\n"
    "\n"
};

/* main function */

int main(int argc, char *argv[]) {
	int return_code;
    int prompt_arg = 0;
    int response_list_arg = 0;

    set_default_options();

	if(argc < 3) {
		/* The user needs help. */
		help();
		return 0;
	}

    /* Simply parse the command line. */
	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-h") == 0 ||
		    strcmp(argv[i], "-H") == 0 ||
		    strcmp(argv[i], "--help") == 0) {
			/* The user wants help. */
			help();
			return 0;
		} else if(strcmp(argv[i], "-c") == 0 ||
		    strcmp(argv[i], "-C") == 0 ||
		    strcmp(argv[i], "--case-sensitive") == 0) {
			/* The user specified case-sensitive mode. */
            set_case_sensitive_mode(1);
        } else if(argv[i][0] != '-') {
            /* 
             The prompt string and responses should appear in order.
             We could use "-p" and "-r" for them, but they're in 
             positional order instead. 
             */
            if(prompt_arg == 0) {
                prompt_arg = i;
            } else if(response_list_arg == 0) {
                response_list_arg = i;
            }
        } else {
            /* 
             Ignore this argument since we don't know what it means. 
             Don't make the user's day harder by blowing up over it.
             */
            continue;
        }
	}

    if(prompt_arg == 0 || response_list_arg == 0) {
        // Execution should not be able to reach here, but just in case...
        if(prompt_arg == 0) {
            printf("\nNo prompt string specified on the command line.\n\n");
        } else {
            printf("\nNo responses specified on the command line.\n\n");
        }
        return 0;
    }

	return_code = ask(argv[prompt_arg], argv[response_list_arg]);

	return return_code;
}

/*
 Print the help message.
 */
void help() {
	printf("\n%s - %s\n", Program_Name, Release);
	printf("\n%s\n", helpmsg);
}

/* end of ask.c */
