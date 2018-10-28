#include "argoat.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void argoat_unflagged_sacrifice(const struct argoat* args)
{
	int len = args->sprigs_count;

	args->sprigs[len].func(args->sprigs[len].data,
		args->unflagged,
		args->unflagged_count);
}

int argoat_increment_pars(struct argoat* args, char* flag, char* pars)
{
	// unflagged pars
	if (flag == NULL)
	{
		int count = args->unflagged_count;

		if (count < args->unflagged_max)
		{
			args->unflagged[count] = pars;
			++args->unflagged_count;
		}

		return 0;
	}
	else
	{
		return 1;
	}
}

// function execution
void argoat_sacrifice(struct argoat* args,
	char* flag,
	char** pars,
	int pars_count)
{
	// first flag found or tag compound passed
	if (flag == NULL)
	{
		return;
	}

	// searches for an equal sign
	char* eq = strchr(flag, '=');
	int flag_len;

	if (eq != NULL)
	{
		flag_len = eq - flag;
	}
	else
	{
		// false-positive warning
		flag_len = strlen(flag);
	}

	// searches the tag in the argoat structure
	int i = 0;
	int len = args->sprigs_count;

	while(i < len)
	{
		if (strncmp(args->sprigs[i].flag, flag, flag_len) == 0)
		{
			break;
		}

		++i;
	}

	// the flag was not registered
	if (i == len)
	{
		return;
	}

	// maximum number of pars, changed to 1 if an '=' is detected
	int max = args->sprigs[i].pars_max;

	// handles equal chars
	if (eq != NULL)
	{
		// moves past the equal char
		++eq; 
		// moves the pars pointer to the flag
		--pars;
		++pars_count;
		// copies the par following '=' at the beginning of the flag
		// false-positive warning
		memcpy(pars[0], eq, strlen(eq) + 1);
		max = 1;
	}

	// if there are more parameters here than the maximum
	// we save them to the unflagged buffer
	if (pars_count > max)
	{
		for(int k = max; k < pars_count; ++k)
		{
			argoat_increment_pars(args, NULL, pars[k]);
		}

		pars_count = max;
	}

	// calls the approriate function
	args->sprigs[i].func(args->sprigs[i].data, pars, pars_count);
}

// executes functions without pars for compound tags
void argoat_compound(struct argoat* args, char** pars)
{
	int scroll = 1;
	// strange warning
	char flag[2];
	flag[1] = '\0';

	do
	{
		flag[0] = pars[0][scroll];
		argoat_sacrifice(args, flag, pars, 0);
		++scroll;
	}
	while(pars[0][scroll] != '\0');
}

// executes functions with pars for each flag
void argoat_graze(struct argoat* args, int argc, char** argv)
{
	char** pars = NULL;
	char* flag = NULL;
	int pars_count = 0;
	char dash;

	// sets the unflagged zone past the end
	args->unflagged = argv + argc;

	for (int i = 0; i < argc; ++i)
	{
		dash = argv[i][1];

		// pars
		if (argv[i][0] != '-')
		{
			pars_count += argoat_increment_pars(args,
				flag,
				argv[i]);
		}
		// lone dash pars
		else if (dash == '\0')
		{
			pars_count += argoat_increment_pars(args,
				flag,
				argv[i]);
		}
		// very probably long flags
		else if (dash == '-')
		{
			// lone double-dash pars
			if (argv[i][2] == '\0')
			{
				pars_count += argoat_increment_pars(args,
					flag,
					argv[i]);
			}
			// long flags
			else
			{
				// executes for previous flag
				argoat_sacrifice(args, flag, pars, pars_count);
				// starts a new flag scope
				flag = argv[i] + 2;
				pars = argv + i + 1;
				pars_count = 0;
			}
		}
		// flags
		else
		{
			// executes for previous flag
			argoat_sacrifice(args, flag, pars, pars_count);

			// compound flags (eg "-xvzf") directly executes
			if ((argv[i][2] != '=') && (argv[i][2] != '\0'))
			{
				// to get rid of the dash
				argoat_compound(args, argv + i);
				flag = NULL;
				pars = NULL;
			}
			// simple flags
			else
			{
				flag = argv[i] + 1;
				pars = argv + i + 1;
			}

			pars_count = 0;
		}
	}

	// we call the function corresponding to the last flag
	argoat_sacrifice(args, flag, pars, pars_count);
	argoat_unflagged_sacrifice(args);
}
