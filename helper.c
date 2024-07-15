#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helper.h"
#include <ctype.h>

// Random number
int call_random (void) {
	int value;
	value=rand();
	return value;
}

// Add two numbers
int call_add (int value1, int value2) {
	return (value1 + value2);
}

// Substruct two numbers
int call_sub (int value1, int value2) {
	return (value1 - value2);
}

// Multiply two numbers
int call_mul (int value1, int value2) {
	return (value1 * value2);
}

// Divide two numbers
int call_div (int value1, int value2) {
	if (value2==0)
		printf("Error! Devide by zero!\n");
	return (value1 / value2);
}

// num1 mod num2
int call_rem (int value1, int value2) {
	return (value1 % value2);
}

// Read number from input
int call_read (void) {
	char s[80];
	fgets(s, 80, stdin);
	return atoi(s);
}

// Function write: print a number
void call_write (int value) {
	printf("%d ", value);
}

// Function writeln: print a number and go to the next line
void call_writeln (int value) {
	printf("%d\n", value);
}

// Function nl: leave a blanck line
void call_nl (void) {
	printf("\n");
}

// Deal with the operators
int call_eval_expression (int value1, int value2, char* oper) {
	int value=0;

	// For the operators (<, <=, >, >=, ==, !=): if value=1, then the condition is real
	if (!strcmp(oper, "<"))
		value = (value1 < value2);
	else if (!strcmp(oper, "<="))
		value = (value1 <= value2);
	else if (!strcmp(oper, ">"))
		value = (value1 > value2);
	else if (!strcmp(oper, ">="))
		value = (value1 >= value2);
	else if (!strcmp(oper, "=="))
		value = (value1 == value2);
	else if (!strcmp(oper, "!="))
		value = (value1 != value2);

	// Arithmetic operations
	else if (!strcmp(oper, "+"))
		value = call_add(value1, value2);
	else if (!strcmp(oper, "-"))
		value = call_sub(value1, value2);
	else if (!strcmp(oper, "*"))
		value = call_mul(value1, value2);
	else if (!strcmp(oper, "/"))
		value = call_div(value1, value2);
	else if (!strcmp(oper, "%"))
		value = call_rem(value1, value2);
	else
		printf("Error! Invalid delimiter!\n");  // Non-existent operator

	return value;
}

// Print error
void syntax_error (int row, char* error) {
	printf("Row: %d, Syntax Error: %s\n", row, error);
	exit(1);
}

// Print error
void runtime_error (int row, char* error) {
	printf("Row: %d, Runtime Error: %s\n", row, error);
	exit(1);
}

// Search for a delimiter
int isdelim (char c) {
	if (strchr(" !+-<>/*%=[]", c) || c==9 || c=='\r' || c=='\n' || c==0)
		return 1;
	else
		return 0;
}

// Skip over white space
int iswhite (char c) {
	if (c==' ' || c=='\t')
		return 1;
	else
		return 0;
}

// Check if it's a number or a valuable
int isnumber (char* s) {
	int i;
	for (i=0; i<strlen(s); i++)
		if (!isdigit(s[i]))
			return 0;
	return 1;
}

// Check if the variable has a valid name based on the rules
int is_valid_variable (char* s) {
	int i;
	for (i=0; i<strlen(s); i++) {
		if (i==0)
			if (!isalpha(s[i]))
				return 0;
		if (!isdigit(s[i]) && !isalpha(s[i]) && s[i]!='_')
			return 0;
	}
	return 1;
}