/*
 * Description of problem :
 * Find out postfix expression.
 */

#include "stack.h"

/*
 * This function returns whether character is not an operator
 */
int stack_is_operand(char ch)
{

	int is_operand = 0;

	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
	{
		is_operand = 1;
	}

	return is_operand;

}

/*
 * This function returns precedence of operator. Precedence is used to make decision
 * whether to push the operator on stack or not.
 */
int stack_find_prec(char operator)
{

	int prec = 0;

	switch(operator)
	{

		case '+':
		case '-':
			prec = 1;
			break;

		case '*':
		case '/':
			prec = 2;
			break;

		case '^':
			prec = 3;
			break;

		default:
			break;

	}

	return prec;

}

/*
 * This function handles case 2 : Closing ).
 */
void stack_prob_3_handle_case_2(stack_st *st)
{

	int rc;
	char val1;

	while (!stack_is_stack_empty(st))
	{

		val1 = *(char *)stack_pop(st, &rc);
		if (val1 == '(')
		{
			break;
		}

		printf("%c", val1);

	}

}

/*
 * This function handles case 3 : operator
 */
void stack_prob_3_handle_case_3(stack_st *st, char ch)
{

	char val;
	int prec_in, rc;
	int prec_stack = 0;

	prec_in = stack_find_prec(ch);

	/*
	 * If precedence of input operator is greater than 
	 * operator from top of stack, push it.
	 */
	while ((!stack_is_stack_empty(st)) && 
	      (stack_find_prec(*(char*)(stack_get_peek(st))) >= prec_in))
	{

		val = *(char *)stack_pop(st, &rc);
		CHECK_RC_ASSERT(rc, EOK);
		printf("%c", val);

	}

	rc = stack_push(st, &ch, sizeof(char));
	CHECK_RC_ASSERT(rc, EOK);

}

/*
 * This is workhorse function for problem 3 stated above.
 */
int stack_prob_3(char *input)
{

	int rc, i;
	stack_st *st;
	char ch;

	CHECK_RC_ASSERT((input == NULL), 0);

	printf("Postfix expression = ");

	i = 0;
	st = stack_alloc_stack(sizeof(char));
	while ((ch = input[i++]) != '\0')
	{

		/*
		 * If character is a digit, print it.
		 */
		if (isdigit(ch) || stack_is_operand(ch))
		{
			printf("%c", ch);
		}
		else
		{

			/*
			 * Case 1: If character is (, push it on stack.
			 */
			if (ch == '(')
			{

				rc = stack_push(st, &ch, sizeof(char));
				CHECK_RC_ASSERT(rc, EOK);

			}
			/*
			 * Case 2: If character is ), pop till ( appears from stack.
			 */
			else if (ch == ')')
			{
				stack_prob_3_handle_case_2(st);
			}
			/*
			 * Case 3: Else, we got operator. Find precedence of it.
			 */
			else
			{
				stack_prob_3_handle_case_3(st, ch);
			}

		}

	}

	while (!stack_is_stack_empty(st))
	{
		printf("%c", *(char *)(stack_pop(st, &rc)));
	}

	printf("\n");
	rc = stack_dealloc_stack(st);
	return rc;

}

