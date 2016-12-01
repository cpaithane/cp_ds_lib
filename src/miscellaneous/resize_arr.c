#include "misc.h"

int *resize_arr(int *old_arr, size_t old_size, size_t new_size) {

	int i;

	int *new_arr = (int *)malloc(sizeof (int) * new_size);
	if (new_arr == NULL) {
		return NULL;
	}

	for (i = 0; i < old_size; i++) {
		new_arr[i] = old_arr[i];
	}

	old_arr = new_arr;
	return old_arr;

}

void resize_arr_tc() {

	size_t old_size = 5;
	size_t new_size = 10;
	int *old_arr = NULL;
	int *new_arr = NULL;

	old_arr = (int *)malloc(sizeof (int) * old_size);

	if (old_arr == NULL) {
		return;
	}

	memset(old_arr, 0, sizeof (int) * old_size);
	new_arr = resize_arr(old_arr, old_size, new_size);

}
