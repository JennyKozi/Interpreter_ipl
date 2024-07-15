#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "helper.h"
#include "functions.h"
#include "globals.h"

// The parser reads every line of the program and saves the important elements of each row into arrays

char token_return;
int arrayfound = 0;
int indent = 0;
int arrays_size = 0;
int node_col_index = 0;
char* line_pointer;
char* line_buffer;

int analyze_source(char* fname) {
	FILE* fp;
	int i = 0;
	fp = fopen(fname, "r");  // Open file.ipl

	if (fp == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	line_buffer = malloc(LINE_CHARS);  // Use the line_buffer for fgets
	if (line_buffer == NULL) {
		printf("Memory allocation error!\n");
		exit(1);
	}

	while (fgets(line_buffer, LINE_CHARS-1, fp) != NULL) {  // Read next line
		token_return = 0;
		indent = 0;
		arrayfound = 0;
		line_pointer = line_buffer;
		nodes_table[node_row_index].num0ftokens=0;
		for (i = 0; i < NUM_TOKENS; i++) {  // Initialization of the array
			nodes_table[node_row_index].elements[i].node_loc = 0;
			nodes_table[node_row_index].elements[i].node_indent = 0;
		}
		while (token_return != FINISHED) {
			get_next_token(node_row_index); // Get next token of current row
		}
		node_row_index++;  // Go to the next line
		node_col_index=0;  // nodes_table[node_row_index].elements[0] is the first token of the row
		nodes_table = realloc(nodes_table, (node_row_index + 2) * sizeof(struct nodes));  // Expand the martix by adding space for one more row
		if (nodes_table == NULL) {
			printf("Memory allocation error!\n");
			exit(1);
		}
	}
	free(line_buffer);  // Free memory
	fclose(fp); // Close file.ipl

	return 1;
}

// If a token has been recognised as an IDENTIFIER
void create_variable (char* varname, int row, int array_found) {
	int i;

	if (!is_valid_variable(varname)) // Check if the variable has a valid name
		syntax_error(row + 1, "Invalid variable name!");

	for (i = 0; i < variables_size; i++) {
		if (!strcmp(variables_table[i].var_name, varname)) { // Check if there is already a variable with the same name
			if (array_found != variables_table[i].array_found)  // Error: variable_name=array_name
				syntax_error(row+1, "Variable already exists!");
			return;
		}
	}
	strcpy(variables_table[variables_size].var_name, varname);
	variables_table[variables_size].var_value = 0;
	variables_table[variables_size].array_found = array_found;
	variables_table[variables_size].array_length = 0;

	if (array_found > 0) {
		variables_table[variables_size].array_index = arrays_size;
		arrays_size++;  // arrays_size: For each array it's the order of the first appearance in the program
		array = realloc(array, (arrays_size + 2) * sizeof(int*));  // One more pointer for an array
		if (array == NULL) {
			printf("Memory allocation error!\n");
			exit(1);
		}
	}
	variables_size++; // Add one more variable
	variables_table = realloc(variables_table, (variables_size+2) * sizeof(struct variable)); // Memory for one more variable in the array of variables
	if (variables_table == NULL) {
		printf("Memory allocation error!\n");
		exit(1);
	}
}

// Search if a token is a FUNCTION or a KEYWORD
int check_commands (char* s) {
	int i, k, f;
	char* keywords[] = {"if","else","while","break","continue","size"};  // Keywords
	char* functions[] = {"read","write","writeln","nl","argument","random","new","free"};  // Functions

	f = (sizeof(functions)/sizeof(char*));
	for (i = 0; i < f; i++) {  // Search through the array of functions
		if (!strcmp(functions[i], s))
			return 1;
	}

	k = (sizeof(keywords)/sizeof(char*));
	for (i = 0; i < k; i++) {  // Search through the array of keywords
		if (!strcmp(keywords[i], s))
			return 2;
	}

	return 3;
}

void get_next_token (int row) {
	int i;
	char token[128];
	char* temp;
	char token_type = 0;
	token_return = 0;
	temp = token;
	*temp = '\0';

	// Skip over white space
	while (iswhite(*line_pointer) && *line_pointer) {
		if (*line_pointer == '\t')
			indent++;
		line_pointer++;
	}
	if (*line_pointer == '\r' || *line_pointer == '\n' || *line_pointer == '\0' || *line_pointer == '#') {
		*token = '\0';
		token_return = FINISHED;
		if (arrayfound > 0)
			syntax_error(row + 1, "Unknown array!");
		arrayfound = 0;

		// Decide to create all variables
		for (i = 0; i < nodes_table[row].num0ftokens; i++) {
			if (nodes_table[row].elements[i].node_type == IDENTIFIER) { // If in this row a token is an IDENTIFIER
				if ((nodes_table[row].elements[i].array_found == 0 && strcmp(nodes_table[row].elements[0].node_token, "free"))
					|| ((nodes_table[row].elements[0].node_type == FUNCTION) && (!strcmp(nodes_table[row].elements[0].node_token, "new")) // new <array>
					&& nodes_table[row].elements[1].node_type == IDENTIFIER))  // function <var>
				{
					create_variable(nodes_table[row].elements[i].node_token, row, nodes_table[row].elements[i].array_found);
				}
			}
		}
		return;
	}
	if (strchr("!<>=", *line_pointer)) {  // Is or might be a relation operator
		switch (*line_pointer) {
			case '=':
				if (*(line_pointer + 1) == '=') {
					line_pointer+=2;
					strcpy(token, "==");
				}
				break;
			case '!':
				if (*(line_pointer + 1) == '=') {
			   		line_pointer+=2;
					strcpy(token, "!=");
				}
				break;
			case '<':
				if (*(line_pointer + 1) == '=') {
					line_pointer+=2;
					strcpy(token, "<=");
				}
				else {
					line_pointer++;
					strcpy(token, "<");
				}
				break;
			case '>':
				if (*(line_pointer + 1) == '=') {
					line_pointer+=2;
					strcpy(token, ">=");
				}
				else {
					line_pointer++;
					strcpy(token, ">");
				}
				break;
		}
		if (*token) {
			nodes_table[row].num0ftokens = node_col_index + 1;
			nodes_table[row].elements[node_col_index].node_loc = node_col_index + 1; // First token: node_loc=1
			nodes_table[row].elements[node_col_index].node_type = DELIMITER;
			nodes_table[row].elements[node_col_index].node_indent = indent; // tabs
			nodes_table[row].elements[node_col_index].array_found = 0; // It is not an array
			strcpy(nodes_table[row].elements[node_col_index].node_token, token); // token[i][j]
			node_col_index++; // Go to the next token
			token_return = DELIMITER;
			return;
		}
	}
	if (strchr("+-*/%=[]", *line_pointer)) {  // Search for a delimiter
		*temp = *line_pointer;
		line_pointer++;
		temp++;
		*temp = '\0';
		token_return = DELIMITER;
		if ((!strcmp(token, "[")) || (!strcmp(token, "]"))) {
			if (!strcmp(token, "["))
				arrayfound++;
			else
				arrayfound--;
			if (node_col_index < 1)
				syntax_error(row+1, "Unknown array!");  // Error: e.g. [i] instead of a[i]
			return;
		}
		nodes_table[row].num0ftokens = node_col_index + 1; // One more token in this row
		nodes_table[row].elements[node_col_index].node_loc = node_col_index + 1; // Token's place in the row
		nodes_table[row].elements[node_col_index].node_type = DELIMITER;
		nodes_table[row].elements[node_col_index].node_indent = indent; // tabs
		nodes_table[row].elements[node_col_index].array_found = 0; // Not an array
		strcpy(nodes_table[row].elements[node_col_index].node_token, token); // Copy token's name into nodes_table with the other tokens
		node_col_index++;
		return;
	}
	if (isdigit(*line_pointer)) {  // Search for a number
		while (!isdelim(*line_pointer))
		*temp++ = *line_pointer++;
		*temp='\0';
		if (!isnumber(token))
			syntax_error(row+1, "Invalid number!");

		token_return = NUMBER;
		if (arrayfound >= 1) {  // If it is an array
			if (arrayfound == 1 && node_col_index >= 1 && nodes_table[row].elements[node_col_index-1].node_type == IDENTIFIER) {
				nodes_table[row].elements[node_col_index-1].array_found = arrayfound; // It is an array
				nodes_table[row].elements[node_col_index-1].array_type = NUMBER; // e.g a[5]
				strcpy(nodes_table[row].elements[node_col_index-1].array_token, token); // Copy array's name into nodes_table with the other tokens
			}
			else
				syntax_error(row + 1, "Unknown array!");
			return;
		}
		// If it is a variable, but not an array
		nodes_table[row].num0ftokens = node_col_index+1;
		nodes_table[row].elements[node_col_index].node_loc = node_col_index+1;
		nodes_table[row].elements[node_col_index].node_type = NUMBER;
		nodes_table[row].elements[node_col_index].node_indent = indent;
		nodes_table[row].elements[node_col_index].array_found = 0;
		strcpy(nodes_table[row].elements[node_col_index].node_token, token);
		node_col_index++;
		return;
	}
	// Search for variable or command
	if (isalpha(*line_pointer)) {
		while (!isdelim(*line_pointer)) {
			*temp++ = *line_pointer++;
		}
		token_type = IDENTIFIER;
		*temp = '\0';
		token_return = check_commands(token); // Check if we have a FUNCTION or KEYWORD

		if (token_return==1)
			token_type = FUNCTION;
		else if (token_return == 2)
			token_type = KEYWORD;
		else
			token_type = IDENTIFIER;

		token_return = token_type;
		if (arrayfound >= 1) {  // It is an array
			if (arrayfound == 1 && node_col_index>=1 && token_type == IDENTIFIER && nodes_table[row].elements[node_col_index-1].node_type == IDENTIFIER) {
				nodes_table[row].elements[node_col_index-1].array_found = arrayfound;
				nodes_table[row].elements[node_col_index-1].array_type = token_type; // array_type==IDENTIFIER: e.g. a[n], n:variable
				strcpy(nodes_table[row].elements[node_col_index-1].array_token, token);
			}
			else
				syntax_error(row+1, "Unknown array!");
			return;
		}
		// It is not an array
		nodes_table[row].num0ftokens = node_col_index + 1;
		nodes_table[row].elements[node_col_index].node_loc = node_col_index + 1;
		nodes_table[row].elements[node_col_index].node_type = token_type;
		nodes_table[row].elements[node_col_index].node_indent = indent;
		nodes_table[row].elements[node_col_index].array_found = 0;
		strcpy(nodes_table[row].elements[node_col_index].node_token, token);
		node_col_index++;
	}
	else
		syntax_error(row + 1, "Invalid character detected!");
}