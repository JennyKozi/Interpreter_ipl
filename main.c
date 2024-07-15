#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "helper.h"
#include "functions.h"
#include "globals.h"

struct nodes* nodes_table; // All of the commands of the entire programm in ipl
struct variable* variables_table; // All of the variables of the program
int** array; // Each row of this matrix contains an array (its elements) of the program
struct argument* arguments_table; // Array of arguments from command line

int printout = 0;
int node_row_index = 0;
int variables_size = 0;
int arguments_size = 0;

// Main
int main(int argc, char* argv[]) {
	int i, current_row;
	long curtime;

	if (argc < 2) {  // Not enough arguments
		printf("usage: ./ipli <filename>\n");
		exit(1);
	}
	if (!strcmp(argv[1], "-v")) {  // If argv[1]=-v
		printout = 1;  // plus one not interesting argument
		if (argc < 3) {
			printf("usage: ./ipli -v <filename>\n");
			exit(1);
		}
	}
	arguments_size=argc; // In order to initialize the buffer for the arguments
	initialize_buffers();
	arguments_size = 0;

	for (i = (printout + 2); i < argc; i++) {  // Store the arguments on arguments_table
		arguments_table[arguments_size].arg_index = i - (printout + 1);
		arguments_table[arguments_size].arg_value = atoi(argv[i]);
		arguments_size++;
	}
	curtime = time(NULL);
	srand((unsigned int) curtime);  // Seed for rand()

	// Call parser
	if (!analyze_source(argv[printout + 1])) {  // argv[printout+1]="file.ipl"
		exit(1);
	}
	current_row = 0;

	// Call interpreter
	while (current_row < node_row_index) { // Start from the first row
		current_row = interpreter(current_row); // Use the interpreter for each row
	}
	cleanup_buffers(); // Free memory

	return 0;
}

// Initialize buffers
void initialize_buffers(void) {
	
	nodes_table = calloc(1, sizeof(struct nodes));  // Memory for the tokens of a row
	if (nodes_table == NULL) {
		printf("Memory allocation error!\n");
		exit(1);
	}

	variables_table = calloc(1, sizeof(struct variable));  // Memory for a variable
	if (variables_table == NULL) {
		printf("Memory allocation error!\n");
		exit(1);
	}

	arguments_table = calloc(arguments_size + 2, sizeof(struct argument));  // Memory for an argument
	if (arguments_table == NULL) {
		printf("Memory allocation error!\n");
		exit(1);
	}

	array = calloc(1, sizeof(int *));  // An array of the arrays used in the program
	if (array == NULL) {  // Memory for one more array
		printf("Memory allocation error\n");
		exit(1);
	}
}

// Free memory
void cleanup_buffers(void) {
	int i;
	for (i = 0; i < variables_size; i++)
		if (variables_table[i].array_found > 0 && variables_table[i].array_length > 0)
			free(array[variables_table[i].array_index]);
	free(array);
	free(nodes_table);
	free(variables_table);
	free(arguments_table);
}