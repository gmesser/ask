/* ask_funcs.c - ask functions */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "ask_funcs.h"

/* private functions */

void write_prompt(char *str);
int read_response_key();
int set_error_code(int response_key);
int get_response_index(char *response_list, int response_key);

// my case-ignoring version of the standard C strchr() function
char *strichr(char *s, int c);  

/* Global variable */

int case_sensitive_mode;

/* Set the default option values. */
void set_default_options() {
    set_case_sensitive_mode(0);
}

/* Set the case-sensitive mode value */
void set_case_sensitive_mode(int value) {
    case_sensitive_mode = value;
}

/*
 Prompt the user, get the response, and return either the index of the response or the error code.
 
 Parameters:
	The prompt string
	The list of response keys

 Return:
	The one-based index of the entered key in the response list, or the error code
	0 if the user entered a key that was not in the response list
	127 error code if the user pressed the Esc key
	126 error code if the user did not enter a key
 */
int ask(char *prompt, char *responses) {
    if(prompt == NULL || responses == NULL) {
            return 0;
    }

	response_struct response = {.response_list = responses, .response_index = 0, .error_code = 0};

	write_prompt(prompt);
	response.response_key = read_response_key();
	response.error_code = set_error_code(response.response_key);

	if(response.error_code == 0) {
		response.response_index = get_response_index(response.response_list, response.response_key);

#ifdef DEBUG
	printf("DEBUG: ask(): No error - return value = response index = %d\n", response.response_index);
#endif

		return response.response_index;
	} else {

#ifdef DEBUG
	printf("DEBUG: ask(): Error detected - return value = error code = %d\n", response.error_code);
#endif

		return response.error_code;
	}
}


/*
 Write the prompt string, processing a limited subset of escape sequences.

 Parameter:
	The prompt string

 Return:
	nothing
 */
void write_prompt(char *prompt) {
	char *ch = prompt;

	while (*ch != 0) {
		if (*ch == '\\') {
			/* This is an escape sequence. */
			ch++;
			if (*ch != 0) {
				/* 
				 Encountered '\' in the prompt followed by another character.
				 Write the appropriate character for this escape sequence.
				 */
				switch (*ch) {
					case '\\':	/* Another '\' - just write "\\". */
						putchar('\\');
						putchar('\\');
						break;
					case 'a':	/* Alarm */
						putchar('\a');
						break;
					case 'n':	/* LF */
						putchar('\n');
						break;
					case 'r':	/* CR */
						putchar('\r');
						break;
					case 't':	/* Tab */
						putchar('\t');
						break;
					default:	/* I don't handle {character} - just write "\{character}". */
						putchar('\\');
						putchar(*ch);
				}

				ch++;
			}
			else {
				/* 
				 Encountered '\' as the last character in the prompt string.
				 Write '\'.
				 */
				putchar('\\');
			}
		}
		else {
			/* Not an escape sequence.  Write the character. */
			putchar(*ch);

			ch++;
		}
	}
}


/*
 Read the user's response.  
 The user must press Enter to complete the process.
 If the user enters more than one character, returns the first character.

 Parameters:
	none

 Return:
	The key the user entered
 */
int read_response_key() {
	int response_key;

	/* Get the key code. */
	response_key = getchar();
	if (response_key == 0) {
		/* Special key. */
		response_key = getchar() + 255;
	}

#ifdef DEBUG
	printf("DEBUG:  read_response_key(): response_key = %d\n", response_key);
#endif

	return response_key;
}


/*
 Set the error code appropriately for the key the user entered.

 Parameter:
	The key the user entered

 Return:
	127 if the user pressed Esc
 	126 if the user did not press a key at all
 */
int set_error_code(int response_key) {
	int error_code;

	if (response_key == 0x1B) {
		/* ESC key pressed. */
		error_code = 128 - 1; // Return 127 instead of -1.  (128 + -1 if you want to look at it that way.)
	} else if(response_key == '\r' || response_key == '\n') {
		/* EOL alone - no choice entered. */
		error_code = 128 - 2; // Return 126 instead of -2.  (128 + -2)
	} else {
		error_code = 0;
	}

	return error_code;
}


/*
 Get the index of the response character in the response list.

 Parameters:
	response_list	The list of acceptable response characters
	response_key	The response character to find

 Return:
	-1	The response character was not in response_list
	n	The one-based index of the response_key in response_list
 */
int get_response_index(char *response_list, int response_key) {
	int response_index;
	char *c;

    if(case_sensitive_mode) {
        c = strchr(response_list, response_key);
    } else {
	    c = strichr(response_list, response_key);
    }

	if (c == 0) {
		/* Not found. */
		response_index = 0;
	} else {
		response_index = (int) ((c - response_list) + 1);
	}

#ifdef DEBUG
	printf("DEBUG: get_response_index(): response_list = %s, response_key = %c, response_index = %d\n",
			response_list, response_key, response_index);
#endif

	return response_index;
}

/*
 Case-insensitive version of strchr().

 Parameters:
    s   The string to search
    c   The character to search for

 Return:
    A pointer to the first occurrence of c (converted to an uppercase char) in string s, or 
    a null pointer if c does not occur in the string. 
 */
char *strichr(char *s, int c) {
    if(s != 0) {
        register char *ps = s;
        int uc = toupper(c);
        for(ps = s; *ps; ps++) {
            if(uc == toupper(*ps)) {
                return(ps);
            }
        }
    }
    
    return 0;
}

/* end of ask_funcs.c */
