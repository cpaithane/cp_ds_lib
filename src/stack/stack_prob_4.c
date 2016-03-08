/*
 * Description : 
 * 	Given an expression which contains pairs multiple parantheses. Check whether
 * 	the parathenses are matching or not.
 */

#include "stack.h"

#define OPEN_PAR 0
#define CLOSE_PAR 1
#define STACK_MAX_SUPPORTED_PAR 3

char open_par[] = {'(', '{', '['};
char close_par[] = {')', '}', ']'};

/*
 * This function checks whether ch1 is matching pair with ch2. For e.g. : ( matches 
 * with )
 */
int stack_prob_4_is_pair(char ch1, char ch2)
{

	int i;
	int is_pair = 0;

	for (i = 0; i < STACK_MAX_SUPPORTED_PAR; i++)
	{

		if ((open_par[i] == ch1) && (close_par[i] == ch2))
		{

			is_pair = 1;
			break;

		}

	}

	return is_pair;

}

/*
 * This function returns : 
 * 0 if ch is one of from open_par
 * 1 if ch is one of from close_par
 */
int stack_prob_4_check_type(char ch)
{

	int rc, i;
	
	for (i = 0; i < STACK_MAX_SUPPORTED_PAR; i++)
	{

		if (ch == open_par[i])
		{

			rc = OPEN_PAR;
			break;

		}
		else if (ch == close_par[i])
		{

			rc = CLOSE_PAR;
			break;

		}

	}

	return rc;

}

/*
 * This is workhorse function for problem specified above.
 */
int stack_prob_4(char *input)
{

	int rc, i, is_pair;
	char ch, *data;
	stack_st *st;

	CHECK_RC_ASSERT((input == NULL), 0);

	i = 0;
	st = stack_alloc_stack(sizeof(char));

	while ((ch = input[i++]) != '\0')
	{

		rc = stack_prob_4_check_type(ch);
		if (rc == OPEN_PAR)
		{

			rc = stack_push(st, &ch, sizeof(char));
			CHECK_RC_ASSERT(rc, EOK);

		}
		else
		{

			data = (char*)stack_pop(st, &rc);
			if (rc == ENOENT)
			{

				printf("ERROR: Parantheses mismatch at location %d\n",
					i);
				break;

			}
			else 
			{

				is_pair = stack_prob_4_is_pair(*data, ch);

				if (is_pair == 0)
				{

					printf("ERROR: Parantheses mismatch at "
						"location %d\n", i);
					CHECK_RC_ASSERT(1, 0);

				}
				
			}

		}

	}

	if (ch == '\0')
	{
		printf("SUCCESS: Parantheses matches is the input %s\n", input);
	}

        rc = stack_dealloc_stack(st);
	return rc;

}

