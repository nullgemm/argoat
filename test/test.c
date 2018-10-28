#include "../src/argoat.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// user-defined handling functions
void handle_arg_w(void* data, char** pars, const int pars_count)
{
	if (pars_count > 0)
	{
		*((bool*) data) = (strcmp(pars[0], "what") == 0);
	}
}

void handle_arg_t(void* data, char** pars, const int pars_count)
{
	if (pars_count > 1)
	{
		// false-positive warning
		*((int*) data) = atoi(pars[0]) + atoi(pars[1]);
	}
}

void handle_arg_f(void* data, char** pars, const int pars_count)
{
	if (pars_count > 0)
	{
		*((char**) data) = pars[0];
	}
}

void handle_main(void* data, char** pars, const int pars_count)
{
	if (pars_count > 2)
	{
		((char**) data)[0] = pars[0]; // strange warning
		((char**) data)[1] = pars[1]; // strange warning
		((char**) data)[2] = pars[2]; // strange warning
	}
}

// example usage
int main(int argc, char** argv)
{
	bool data1 = false;
	int data2 = 0;
	char* data3 = NULL;
	char* data4[3];

	char* unflagged[10];

	const struct argoat_sprig sprigs[4] =
	{
		{NULL, 0, (void*) data4, handle_main},
		{"waaa", 1, (void*) &data1, handle_arg_w},
		{"t", 2, (void*) &data2, handle_arg_t},
		{"f", 1, (void*) &data3, handle_arg_f},
	};

	struct argoat args = {sprigs, 4, unflagged, 0, 10};

	argoat_graze(&args, argc, argv);
	
	// do some stuff
	printf("w-flag boolean: %d\n", data1);
	printf("t-flag integer: %d\n", data2);
	printf("f-flag string:  \"%s\"\n", data3 ? data3 : "");

	printf("main string: %s\n", data4[0]);
	printf("main string: %s\n", data4[1]);
	printf("main string: %s\n", data4[2]);

	return 0;
}
