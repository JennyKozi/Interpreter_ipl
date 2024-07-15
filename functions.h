// Prototypes of functions

void initialize_buffers(void);
void cleanup_buffers(void);
int analyze_source(char*);
void array_memory_allocation(char *, int, int);
void free_array(int, char *);
void create_variable(char *, int, int);
void assign_variable_value(char *, int, int, int);
int get_variable_value(char *, int, int);
int get_argument_value(int, int);
int check_commands(char *);
void get_next_token(int);
int interpreter(int);
int get_array_index(int, int, int);
int evaluate_expression(int, int);
int execute_if_block(int);
int execute_while_block(int);
int check_block_condition(int);
void printout_line(int);