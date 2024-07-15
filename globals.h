#define NUM_TOKENS 50
#define LINE_CHARS 1000
#define MAX_ARRAY_SIZE 100000

enum token_types {DELIMITER, IDENTIFIER, FUNCTION, NUMBER, KEYWORD, FINISHED};

// A token
struct node {
	int node_type;  // From enum token_types
	char node_token[100];  // Token's name
	int array_found;  // If the token is an array or not
	int array_type;  // Array: a[n], array_type: if n is a variable with a value (IDENTIFIER) or a number (NUMBER)
	char array_token[100]; // Array: a[n], array_token={n}
	int node_loc;  // The token's order in the row
	int node_indent;  // tabs
};

// A row (all of its elements)
struct nodes {
	int num0ftokens;
	struct node elements[NUM_TOKENS];  // The tokens of the row
};

// All of the commands of the entire programm in ipl
extern struct nodes *nodes_table;  // Each row of the matrix is a row of commands from the program

// A variable
struct variable {
	char var_name[128]; // Variable's name
	int var_value;  // Variable's value
	int array_found;  // If it is an array
	int array_index;  // Value of a[i], i=0,1,...
	int array_length;  // Array's size
};

extern struct variable *variables_table;  // All of the variables of the program
extern int **array;  // Each row of this matrix contains an array (its elements) of the program

// An argument
struct argument {
	int arg_index;  // Indicates if it's the first, second... argument that the user inserts from command line
	int arg_value;  // The value of the argument
};

extern struct argument *arguments_table;  // Array of arguments from command line

extern int printout;  // If the argument -v exists
extern int node_row_index;
extern int variables_size;  // The number of variables of the program
extern int arguments_size;  // The number of arguments given from command line