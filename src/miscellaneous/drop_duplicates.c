#include "misc.h"

void drop_duplicates(char *str) {

	if (str == NULL) {
		return;
	}

	char *slow = str;
	char *fast = str;
	int i;
	int *hash = (int *)calloc(256, sizeof (int));
	if (hash == NULL) {
		return;
	}

	while (*fast != '\0') {
		i = *fast;
		hash[i]++;
		fast++;
	}

	fast = str;
	while (*fast != '\0') {

		if (hash[*fast] == 1) {
			*slow = *fast;
			slow++;
		}
		fast++;

	}

	*slow = '\0';
	printf("Dropped string = %s\n", str);
	free(hash);

}

void drop_duplicates_tc() {

	char str[6] = "abbbc";
	drop_duplicates(str);

}
