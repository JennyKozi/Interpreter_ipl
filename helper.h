// Prototypes of functions for helper.c

int call_random(void);
int call_add(int, int);
int call_sub(int, int);
int call_mul(int, int);
int call_div(int, int);
int call_rem(int, int);
int call_read(void);
int call_eval_expression(int, int, char *);
int isdelim(char);
int iswhite(char);
int isnumber(char *);
int is_valid_variable(char *);
void call_write(int);
void call_writeln(int);
void call_nl(void);
void syntax_error(int, char *);
void runtime_error(int, char *);