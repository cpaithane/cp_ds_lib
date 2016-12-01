#include "misc.h"

int main(int argc, char *argv[])
{

	int rc = EOK;
        two_d_arr();

	if (argc > 1)
	{

		rc = pattern_detect(argv[1]);
		if (rc != 0)
		{
			printf("Pattern found\n");
		}
		else
		{
			printf("Pattern not found\n");
		}

	}

	resize_arr_tc();
	drop_duplicates_tc();

        return 0;

}
