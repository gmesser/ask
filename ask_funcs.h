/* ask_funcs.h - ask functions */

#ifndef ASK_FUNCS_H
#define ASK_FUNCS_H

#ifdef __cplusplus
extern "C" {
#endif

// Use this during testing if you want debug messages.
// #define DEBUG	

typedef struct response_struct {
   int error_code;
   int response_key;
   int response_index;
   char *response_list;
} response_struct;

void set_default_options();
void set_case_sensitive_mode(int value);
int ask(char *prompt, char *response_list);

#ifdef __cplusplus
}
#endif

#endif /* ASK_FUNCS_H */

