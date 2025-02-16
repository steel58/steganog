#include <stdio.h>
#include <stdlib.h>
#include "PNG_ops.h"



int isPNG(char file_path[]) {
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) { return -1; }

    // 8 bytes that start every PNG file according to:
    // http://www.libpng.org/pub/png/spec/1.2/PNG-Structure.html
    char id_bytes[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    char read_bytes[8];
    fread(read_bytes, sizeof(char), 8, file);
    

    for (int i = 0; i < 8; i++) {
        if (id_bytes[i] != read_bytes[i]) { return 0; }
    }
    return 1;
}
