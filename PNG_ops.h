#include <stdio.h>
#include <stdlib.h>


/*
 * Determines if the file at the given filepath contains a PNG
 *
 * If the file cannot be opened will return -1;
 * If the file is not a PNG will return 0;
 * If the file is a PNG will return 1;
 */
int isPNG(char file_path[]);
