#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helper.h"
#include "functions.h"
#include "globals.h"

// The interpreter uses the arrays with the tokens created by the parser in order to execute the commands of the program

int nestedwhile = 0;
int breakcond = 0;

int interpreter(int row) {
	int temprow = row;
	int i, value1, value2, value3, argindex, numofbreaks = 0;
	char func_name[100];

	if (breakcond != 0) {
		temprow = row + 1;
		return temprow;
	}
	printout_line(row); // Line that is currently in execution

	// Cases: while number1 <= number2, while number <= 7
	if ((nodes_table[row].num0ftokens == 4) && (nodes_table[row].elements[0].node_type == KEYWORD) && (!strcmp(nodes_table[row].elements[0].node_token, "while"))
	&& (nodes_table[row].elements[2].node_type == DELIMITER))
	{
		temprow = execute_while_block(row);
		return temprow;
	}
	// Cases: if number1 == number2, if number == 7
	else if ((nodes_table[row].num0ftokens == 4) && (nodes_table[row].elements[0].node_type == KEYWORD) && (!strcmp(nodes_table[row].elements[0].node_token, "if"))
		&& (nodes_table[row].elements[2].node_type == DELIMITER))
	{
		temprow = execute_if_block(row);
		return temprow;
	}
	// break
	else if ((nodes_table[row].num0ftokens == 1 || nodes_table[row].num0ftokens == 2) && (nodes_table[row].elements[0].node_type == KEYWORD)
		&& (!strcmp(nodes_table[row].elements[0].node_token, "break")))
	{
		numofbreaks = 1;
		if (nodes_table[row].num0ftokens == 2) { // Command: break n
			if (nodes_table[row].elements[1].node_type == NUMBER)
				numofbreaks = atoi(nodes_table[row].elements[1].node_token);  // Find the value of n
			else
				runtime_error(row + 1, "Invalid break statement!");
		}
		if (nestedwhile > 0)
			breakcond = (numofbreaks-1) * 1000 + 1;  // break: breakcond = 1, break 2: breakcond = 1001, break 3: breakcond = 2001,...
		else  // We can't have 'break' without a previous 'while'
			runtime_error(row + 1, "Invalid break statement!");
	}
	// continue
	else if ((nodes_table[row].num0ftokens == 1 || nodes_table[row].num0ftokens == 2) && (nodes_table[row].elements[0].node_type == KEYWORD)
		&& (!strcmp(nodes_table[row].elements[0].node_token, "continue")))
	{
		numofbreaks = 1;
		if (nodes_table[row].num0ftokens == 2) {  // Command: continue n
			if (nodes_table[row].elements[1].node_type==NUMBER)
				numofbreaks = atoi(nodes_table[row].elements[1].node_token);  // Find the value of n
			else
				runtime_error(row + 1, "Invalid continue statement!");
		}
		if (nestedwhile > 0)
			breakcond = (numofbreaks-1) * 1000 + 2;  // continue: breakcond = 2, continue 2: breakcond = 1002, continue 3: breakcond = 2002,...
		else  // We can't have 'continue' without a previous 'while'
			runtime_error(row + 1, "Invalid continue statement!");
	}
	// Cases: number = 1, number2 = number1, number = 2 + 1, number2 = number1 + 1, number2 = 1 + number1, number3 = number1 + number2
	else if ((nodes_table[row].num0ftokens >= 3) && (nodes_table[row].elements[0].node_type == IDENTIFIER) && (nodes_table[row].elements[1].node_type == DELIMITER)
		&& (!strcmp(nodes_table[row].elements[1].node_token, "=")))
	{
		if ((nodes_table[row].num0ftokens == 3) || ((nodes_table[row].num0ftokens == 5) && (nodes_table[row].elements[3].node_type == DELIMITER))) {
			value3 = evaluate_expression(2, row);
		}
		assign_variable_value(nodes_table[row].elements[0].node_token, row, 0, value3);
	}
	// e.g. function 1, function number
	else if ((nodes_table[row].num0ftokens == 3) && (nodes_table[row].elements[0].node_type == FUNCTION) && (nodes_table[row].elements[2].node_type == IDENTIFIER))
	{
		strcpy(func_name, nodes_table[row].elements[0].node_token);
		argindex = atoi(nodes_table[row].elements[1].node_token);
		// e.g. argument 1 minnumb
		if (!strcmp(func_name, "argument")) {
			if (nodes_table[row].elements[1].node_type == NUMBER) {
				value1 = get_argument_value(row, argindex);
				assign_variable_value(nodes_table[row].elements[2].node_token, row, 2, value1);
			}
			else if (nodes_table[row].elements[1].node_type == KEYWORD && (!strcmp(nodes_table[row].elements[1].node_token, "size"))) {
				assign_variable_value(nodes_table[row].elements[2].node_token, row, 2, arguments_size);
			}
		}
	}
	// Function's syntax: function number
	else if ((nodes_table[row].num0ftokens == 2) && (nodes_table[row].elements[0].node_type == FUNCTION) && ((nodes_table[row].elements[1].node_type == IDENTIFIER)
		|| (nodes_table[row].elements[1].node_type == NUMBER)))
	{
		strcpy(func_name, nodes_table[row].elements[0].node_token);
		if (!strcmp(func_name, "write") || !strcmp(func_name, "writeln"))
		{
			if ((nodes_table[row].elements[1].node_type == IDENTIFIER))
				value1 = get_variable_value(nodes_table[row].elements[1].node_token, row, 1);
			else
				value1 = atoi(nodes_table[row].elements[1].node_token);
		}
		// read number
		if (!strcmp(func_name, "read")) {
			value1 = call_read();
			assign_variable_value(nodes_table[row].elements[1].node_token, row, 1, value1);
		}
		// write number
		else if (!strcmp(func_name, "write")) {
			call_write(value1);
		}
		// writeln number
		else if (!strcmp(func_name, "writeln")) {
			call_writeln(value1);
		}
		// random number
		else if (!strcmp(func_name, "random")) {
			value1 = call_random();
			assign_variable_value(nodes_table[row].elements[1].node_token, row, 1, value1);
		}
		// new array
		else if (!strcmp(func_name, "new")) {
			array_memory_allocation(nodes_table[row].elements[1].node_token, row, 1);
		}
		// free array
		else if (!strcmp(func_name, "free")) {
			free_array(row, nodes_table[row].elements[1].node_token);
		}
	}
	// Function nl
	else if ((nodes_table[row].num0ftokens == 1) && (nodes_table[row].elements[0].node_type == FUNCTION)) {
		strcpy(func_name, nodes_table[row].elements[0].node_token);
		if (!strcmp(func_name, "nl"))
			call_nl();
	}
	else if (nodes_table[row].num0ftokens > 0) {  // Invalid statement
		runtime_error(row + 1, "Invalid statement!");
	}
	temprow = row + 1; // Current row

	return temprow;
}

// Find the value of a[i]
int get_array_index(int varindex, int row, int ntoken) {
	int colindex = -1;

	if (nodes_table[row].elements[ntoken].array_type == IDENTIFIER) {  // Case: a[n], where n is a variable
		colindex = get_variable_value(nodes_table[row].elements[ntoken].array_token, row, ntoken);  // Find the value of n
	}
	else if ((nodes_table[row].elements[ntoken].array_type == NUMBER)) {  // Case: a[j], where j is a number, e.g. a[7]
		colindex = atoi(nodes_table[row].elements[ntoken].array_token);  // Turn 7 from char to int
	}
	if (colindex < 0 || (colindex >= variables_table[varindex].array_length)) {
		runtime_error(row+1, "Array index out of bounds!");  // Invalid element of array
		exit(1);
	}
	return colindex;
}

// Command: n1 = n2 (assign)
void assign_variable_value(char* varname, int row, int ntoken, int value) {
	int i, colindex = -1;

	for (i = 0; i < variables_size; i++) {
		if (!strcmp(variables_table[i].var_name, varname)) {  // The variable should exist in order to assign its value into another variable
			if (variables_table[i].array_found > 0) {  // It is an array
				colindex = get_array_index(i, row, ntoken);  // Get the value of a[j]
				array[variables_table[i].array_index][colindex] = value;
			}
			else
				variables_table[i].var_value = value;  // Assign new value to the variable "variables_table[i]"
			return;
		}
	}
	runtime_error(row + 1, "Variable does not exist!");
}

// Find the value that a variable currently has
int get_variable_value(char* varname, int row, int ntoken) {
	int i, colindex = -1;

	for (i = 0; i < variables_size; i++) {
		if (!strcmp(variables_table[i].var_name, varname)) {  // The variable must be in the array of variables
			if (variables_table[i].array_found > 0) {  // It is an array
				colindex = get_array_index(i, row, ntoken);
				return array[variables_table[i].array_index][colindex];
			}
			else
				return variables_table[i].var_value;  // It is a simple variable
		}
	}
	runtime_error(row + 1, "Unknown variable!");
}

// Find the value of an argument
int get_argument_value (int row, int argindex) {
	int i;

	for (i = 0; i < arguments_size; i++) {
		if (arguments_table[i].arg_index == argindex) {  // The first, second,... argument from command line
			return arguments_table[i].arg_value;
		}
	}
	runtime_error(row + 1, "Unknown ARGUMENT!");
}

// Memory allocation for the arrays of the program
void array_memory_allocation(char* varname, int row, int ntoken) {
	int i, index = -1, nelements = 0;

	for (i = 0; i < variables_size; i++) {
		if (!strcmp(variables_table[i].var_name, varname) && variables_table[i].array_found > 0)
			index = i; // Its place in the row
	}
	if (index < 0) { // The array doesn't exists
		runtime_error(row + 1, "Array not found!");
		return;
	}
	if (nodes_table[row].elements[ntoken].array_type == IDENTIFIER) { // Case: a[n], where n is a variable
		for (i = 0; i < variables_size; i++)
			if (!strcmp(nodes_table[row].elements[ntoken].array_token, variables_table[i].var_name) && variables_table[i].array_found == 0)
				nelements = variables_table[i].var_value;
	}
	else if ((nodes_table[row].elements[ntoken].array_type == NUMBER)) {  // Case: a[j], where j is a number, e.g. a[7]
		nelements = atoi(nodes_table[row].elements[ntoken].array_token);  // Turn 7 from char to int
	}
	if (nelements <= 0) {  // Case: a[n], where n<=0
		runtime_error(row + 1, "Array too small!");
		return;
	}
	if (nelements > MAX_ARRAY_SIZE) {
		runtime_error(row + 1, "Array too large!");
		return;
	}
	// The variable "variables_table[index] is an array
	variables_table[index].array_length = nelements; // The number of the elements of the array
	array[variables_table[index].array_index] = calloc(nelements, sizeof(int));  // Memory allocation for the array

	if (array[variables_table[index].array_index] == NULL) {
		runtime_error(row + 1, "Memory allocation error!");
		return;
	}
	for (i = 0; i < nelements; i++)
		array[variables_table[index].array_index][i] = 0; // Initialization of the elements of the array
}

// Free the arrays that were created by the program
void free_array(int row, char* arrayname) {
	int i;

	for (i = 0; i < variables_size; i++) { // An array with elements
		if (!strcmp(variables_table[i].var_name, arrayname) && variables_table[i].array_found>0 && variables_table[i].array_length>0) {
			free(array[variables_table[i].array_index]);
			variables_table[i].array_length = 0;
			strcpy(variables_table[i].var_name, "");
			return;
		}
	}
	runtime_error(row + 1, "Array not found!");
}

// Execute the command of an expression
int evaluate_expression(int initial_token, int row) {
	int i, count = 0, value = 0;
	int values[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	char oper[100] = "";

	for (i = initial_token; i < nodes_table[row].num0ftokens; i++) {  // Check all of the tokens of current row
		// Find what each token is
		if (nodes_table[row].elements[i].node_type == IDENTIFIER) {
			values[count] = get_variable_value(nodes_table[row].elements[i].node_token, row, i);
			count++;
		}
		else if ((nodes_table[row].elements[i].node_type == NUMBER)) {
			values[count] = atoi(nodes_table[row].elements[i].node_token);
			count++;
		}
		else if ((nodes_table[row].elements[i].node_type == DELIMITER)) {
			sprintf(oper, "%s", nodes_table[row].elements[i].node_token);
		}
	}
	if (count == 0)
		runtime_error(row + 1, "Invalid expression!");
	if (count == 1)
		value = values[0];
	else  // count >= 2
		value = call_eval_expression(values[0], values[1], oper);

	return value;
}

// If
int execute_if_block(int row) {
	int elseexists = 0, valuecond = 0;
	int temprow = row + 1;  // The condition of 'if' is in this row
	int indent = nodes_table[row].elements[0].node_indent;
	valuecond = evaluate_expression(1, row); // Find if the condition is true or false

	while (temprow < node_row_index) {
		if (nodes_table[temprow].num0ftokens == 0)
			temprow++;  // Go to the next line
		if ((nodes_table[temprow].elements[0].node_indent == indent) && (nodes_table[temprow].elements[0].node_type==KEYWORD)
			&& (!strcmp(nodes_table[temprow].elements[0].node_token, "else")) && (elseexists == 0))
		{
			printout_line(temprow);
			elseexists=1;
			temprow++;
		}
		// valuecond=1: the condition of 'if' is true | valuecond=0: the condition of 'if' is false
		else if (nodes_table[temprow].elements[0].node_indent>indent && elseexists == 0 && valuecond == 1)
			temprow = interpreter(temprow); // Execute the commands of if
		else if (nodes_table[temprow].elements[0].node_indent > indent && elseexists == 1 && valuecond == 1)
			temprow++;  // Ignore the commands of else
		else if (nodes_table[temprow].elements[0].node_indent > indent && elseexists == 0 && valuecond == 0)
			temprow++;  // Ignore the commands of if
		else if (nodes_table[temprow].elements[0].node_indent > indent && elseexists == 1 && valuecond == 0)
			temprow = interpreter(temprow);  // Execute the commands of else
		else
			return temprow;
	}
	return temprow;
}

// While
int execute_while_block(int row) {
	int temprow = row + 1;  // The condition of 'while' is in this row
	int valuecond=0;
	int indent = nodes_table[row].elements[0].node_indent;
	nestedwhile++;
	valuecond = evaluate_expression(1, row);  // Find if the condition is true or false

	while (temprow < node_row_index) {
		if (nodes_table[temprow].num0ftokens > 0 && nodes_table[temprow].elements[0].node_indent>indent && valuecond) // The condition is true
			temprow = interpreter(temprow);  // Execute the commands of while
		if (nodes_table[temprow].num0ftokens > 0 && nodes_table[temprow].elements[0].node_indent>indent && !valuecond) // The condition is false
			temprow++;  // Ignore the commands of while
		if (nodes_table[temprow].num0ftokens == 0)
			temprow++;  // Go to the next line
		if (((nodes_table[temprow].num0ftokens>0) && (nodes_table[temprow].elements[0].node_indent <= indent)) || (temprow >= node_row_index)) {
			if (breakcond == 1 || breakcond > 1000) // If there is one break or more
				break;
			breakcond = 0;
			valuecond = evaluate_expression(1, row);  // Check if the condition of while is true
			// Note: continue n = break n - 1 + continue
			if (valuecond) {  // breakcond = 2
				printout_line(row);
				temprow = row + 1;
				continue;
			}
			else
				break;
		}
	}
	nestedwhile--;  // Execute one break
	if (breakcond > 1000)
		breakcond = breakcond - 1000;
	else
		breakcond = 0;

	return temprow;
}

// Command -v
// row: current row that is being executed
void printout_line(int row) {
	int j, k;
	int tokensprinted = 0;
	
	if (printout) { // If -v exists
		for (j = 0; j < NUM_TOKENS; j++) {
			if (nodes_table[row].elements[j].node_loc > 0) {
				if (tokensprinted == 0) {
					printf("%d:\t", row + 1); // Current row
					for (k = 0; k < nodes_table[row].elements[j].node_indent; k++) // Print tabs where necessary
						printf("\t");
				}
					printf("%s ", nodes_table[row].elements[j].node_token); // Print each element of the row
				tokensprinted++;
			}
		}
		if (tokensprinted > 0) // Leave a blank line for the tokens of the next row that will be printed
			printf("\n");
	}
}