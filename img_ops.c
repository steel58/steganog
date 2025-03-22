#include <png.h>
#include <stdint.h>
#include <string.h>
#include "img_ops.h"

void hide_message(Image *image, const char* msg) {
    int color_state = 0;
    int x = 0;
    int y = 0;

    int length = strlen(msg);
    for (int bit = 31; bit >= 0; bit--) {
        int b = (length >> bit) & 1;
        switch (color_state) {
            case 0: image->rmap[y][x] = (image->rmap[y][x] & (~1)) | b; break;
            case 1: image->gmap[y][x] = (image->gmap[y][x] & (~1)) | b; break;
            case 2: image->bmap[y][x] = (image->bmap[y][x] & (~1)) | b; break;
        }

        x++;
        if (x >= image->cols) {
            y++;
            x = 0;
        }
        color_state = (color_state + 1) % 3;
    }

    for (int idx = 0; idx < length; idx++) {
        char c = msg[idx];
        for (int bit = 7; bit >= 0; bit--) {
            int b = (c >> bit) & 1;
            switch (color_state) {
                case 0: image->rmap[y][x] = (image->rmap[y][x] & (~1)) | b; break;
                case 1: image->gmap[y][x] = (image->gmap[y][x] & (~1)) | b; break;
                case 2: image->bmap[y][x] = (image->bmap[y][x] & (~1)) | b; break;
            }

            x++;
            if (x >= image->cols) {
                y++;
                //printf("%d\n", y);
                x = 0;
            }
            color_state = (color_state + 1) % 3;
        }
    }
}

char* decode_message(Image *image) {
    int color_state = 0;

    int x = 0;
    int y = 0;
    int len = 0;
    for (int bit = 31; bit >= 0; bit--) {
        len = len << 1;
        switch (color_state) {
            case 0: len = len | (image->rmap[y][x] & 1); break;
            case 1: len = len | (image->gmap[y][x] & 1); break;
            case 2: len = len | (image->bmap[y][x] & 1); break;
        }

        x++;
        if (x >= image->cols) {
            y++;
            x = 0;
        }
        color_state = (color_state + 1) % 3;
    }


    printf("Found len: %d\n", len);
    char *new_msg = malloc((len + 1) * sizeof(char));

    for (int idx = 0; idx < len; idx++) {
        char c = 0;
        for (int bit = 0; bit < 8; bit++) {
            c = c << 1;
            switch (color_state) {
                case 0: c = c | (image->rmap[y][x] & 1); break;
                case 1: c = c | (image->gmap[y][x] & 1); break;
                case 2: c = c | (image->bmap[y][x] & 1); break;
            }

            x++;
            if (x >= image->cols) {
                y++;
                x = 0;
            }
            color_state = (color_state + 1) % 3;
        }

        new_msg[idx] = (char) c;
        new_msg[idx + 1] = 0;
    }

    return new_msg;
}
