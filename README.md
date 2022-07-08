# ask

**The ask program prompts the user and accepts a single-key response.**

<br />

## Usage:  

## **ask [*options*] "prompt string" responses**
<br />

The ***options*** can appear anywhere on the command line.

| *Options*| Description |
| :---: | --- |
| -c, -C, --case-sensitive | Case-sensitive response matching |
| -h, -H, --help | Show this help screen |

<br />

The **"prompt string"** and **responses** are positional parameters.  They must be in order.

| Positional Parameter | Description |
| --- | --- |
| "prompt string" | The prompt string the user sees |
| responses | The characters the user is allowed to press to answer the prompt *(Not a comma-separated list.)* |

<br />

## Return value:

| System Exit Code | Meaning |
| :---: |  ---  |
| 0 | The user entered a response that was not in the allowed responses. |
| 1-125 | The index of the user's choice in the responses. (The first response is 1, the next is 2, and so on.) |
| 126 | The user pressed Enter without making a choice |
| 127 | The user pressed Escape |

*System Exit Code: In an sh-compatible shell, you check the ?\* variable.<br />
In a batch file you check the ERRORLEVEL.<br />
Other shells may be different.*

<br />

## Usage Notes:

* The user must press Enter after pressing a key.
* The response is not case-sensitive by default.  Use -c if case-sensitive mode is necessary.
* If the user presses more than one key, the first key will be used.  The user can use the keyboard to edit their response.
* The escape sequences \\, \\a, \\n, \\r, and \\t are allowed in the prompt string.

<br />

## Example:
  ask "** Answer [Y]es, [N]o, or [M]aybe: " YNM

The example displays the following prompt, and reads the user's response:

** Answer [Y]es, [N]o, or [M]aybe: 

The example returns:
* Exit code **1** if the user pressed **y** or **Y**.
* Exit code **2** if the user pressed **n** or **N**.
* Exit code **3** if the user pressed **m** or **M**.
* Exit code **0** if the user pressed a key that was *not* y, Y, n, N, m, or M.
* Exit code **126** if the user pressed **Enter** without pressing a key.
* Exit code **127** if the user pressed **Escape**.

<br />

<hr />

## Programming Notes

You can include the `ask_funcs.h` header and link the `ask_funcs.o` module to your own C/C++ programs.  This provides your programs the same functionality used by the **ask** program.  

It uses standard C library functions for I/O.  It uses the standard `putchar()` function to print the prompt string to *stdout*, and the standard `getchar()` function to read the response character from *stdin*.  
<br />

## Functions

The ask_funcs module provides the following public functions:

```
void set_default_options();
void set_case_sensitive_mode(int value);
int ask(char *prompt, char *response_list);
```

`void set_default_options();`
* The `set_default_options` function simply sets the default case_sensitive mode option to OFF (0).

`void set_case_sensitive_mode(int value);`
* The `set_case_sensitive_mode` function lets you turn the case sensitive mode ON (non-zero) or OFF (0).

`int ask(char *prompt, char *responses);`
* The `ask` function lets you prompt the user, specifying the prompt string and the response characters, and then receive the response code.
* The return value from the `ask` function is the same as the system exit codes described for the **ask** program.

<hr />
