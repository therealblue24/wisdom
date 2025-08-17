/* Copyright (C) 2025 therealblue24 under the MIT license */

#if defined(__linux__) || defined(__APPLE__) || defined(__unix__)
#define UNIXLIKE (1)
#endif

#ifdef UNIXLIKE
#include <unistd.h>
#endif /* UNIXLIKE */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Count lines in a file by counting how much newlines ('\n') there are */
static size_t count_lines(FILE *f)
{
	rewind(f);
	size_t lines = 0;
	while(!feof(f)) {
		lines += (fgetc(f) == '\n');
	}
	return lines;
}

/* Prints line `line` in the file by counting newlines until we reach
 * the target line and then print characters we receive until we hit
 * another newline. */
static void print_line(FILE *f, size_t line)
{
	rewind(f);
	size_t lines_cur = 0;
	while(!feof(f) && lines_cur != line) {
		lines_cur += (fgetc(f) == '\n');
	}
	int c = fgetc(f);
	do {
		putchar(c);
	} while((c = fgetc(f)) != '\n');
	return;
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	/* execute for random amount of time */

	volatile uint64_t counter = 1 + clock();

	/* Try to screw around with the CPU */
	/* collatz conjecture used for optimal unpredictability */
	for(int i = 0; i < 100; i++) {
		counter *= counter + 1;
		/* intentionally screw around with the branch predictor
		 * to get random timing */
		if(counter & 1) {
			counter = (counter * 3) + 1;
		} else {
			counter >>= 1;
		}
	}

#ifdef __APPLE__
	sranddev();
#else
	srand(time(NULL) + clock() + counter);
#endif

	/* Open the wisdomfile, check lines, generate a number in that
	 * lines range and print it */
#ifdef UNIXLIKE
	char *fname = strdup("wisdomfile"); /* wisdomfile is default */
	/* check if there is an env variable where we should find the wisdomfile */
	char *possible_wisdomfile = getenv("WISDOMFILE");
	/* if so, use it */
	if(possible_wisdomfile) {
		free(fname);
		fname = strdup(possible_wisdomfile);
	}
	FILE *wisdom = fopen(fname, "r");
#else
	FILE *wisdom = fopen("wisdomfile", "r");
#endif /* UNIXLIKE */

	size_t lines = count_lines(wisdom);
	size_t line_to_print = rand() % lines;
	print_line(wisdom, line_to_print);
	putchar('\n');

#ifdef UNIXLIKE
	free(fname);
#endif

	fclose(wisdom);
	return 0;
}
