/*
 * Copyright 2016 Ali Reza Barkhordari
 * 
 * Code base credits to Guillaume Cottenceau, edited by Ali Reza Barkhordari
 * 
 * Note: To compile using gcc you should use flag -lpng
 */

/*
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <string.h>
#include <png.h>
#include "read_write_ops.h"

Image* readPNGImage(char* filename){
    Image *image = malloc(sizeof(Image));
    png_byte color_type;
    png_bytep * row_pointers;
    png_structp png_ptr;
    png_infop info_ptr;
    int i, j;

    // 8 is the maximum size that can be checked
    char header[8];

    /* open file and test for it being a png */
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "[read_png_file] File %s could not be opened for reading", filename);
        return NULL;
    }

    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)){
       fprintf(stderr, "[read_png_file] File %s is not recognized as a PNG file", filename);
       return NULL;
    }


    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr){
        fprintf(stderr, "[read_png_file] png_create_read_struct failed");
        return NULL;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr){
        fprintf(stderr, "[read_png_file] png_create_info_struct failed");
        return NULL;
    }

    if (setjmp(png_jmpbuf(png_ptr))){
        fprintf(stderr, "[read_png_file] Error during init_io");
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    image->cols = png_get_image_width(png_ptr, info_ptr);
    image->rows = png_get_image_height(png_ptr, info_ptr);

    printf("%d\n", image->rows);
    printf("%d\n", image->cols);
    color_type = png_get_color_type(png_ptr, info_ptr);
    if(color_type != PNG_COLOR_TYPE_RGB){
        fprintf(stderr, "[read_png_file] Only RGB PNGs are supported");
        return NULL;
    }
    png_read_update_info(png_ptr, info_ptr);

    /* read file */
    if (setjmp(png_jmpbuf(png_ptr))){
        fprintf(stderr, "[read_png_file] Error during read_image");
        return NULL;
    }

    /* memory allocation */
    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * image->rows);
    for (i = 0; i < image->rows; i += 1){
        row_pointers[i] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    image->rmap = malloc(image->rows * sizeof(int *));
    image->gmap = malloc(image->rows * sizeof(int *));
    image->bmap = malloc(image->rows * sizeof(int *));
    for (i = 0; i < image->rows; i += 1) {
        (image->rmap)[i] =  malloc(image->cols * sizeof(int));
        (image->gmap)[i] = malloc(image->cols * sizeof(int));
        (image->bmap)[i] = malloc(image->cols * sizeof(int));
    }

    png_read_image(png_ptr, row_pointers);
    fclose(fp);

    for (i = 0; i < image->rows; i += 1) {
        png_byte* row = row_pointers[i];
        for (j = 0; j < image->cols; j += 1) {
            png_byte* ptr = &(row[j * 3]);

            image->rmap[i][j] = ptr[0];
            image->gmap[i][j] = ptr[1];
            image->bmap[i][j] = ptr[2];
        }
    }

    /* clean up */
    for (i = 0; i < image->rows; i += 1){
        free(row_pointers[i]);
    }
    free(row_pointers);

    return image;
}

void writePPMImage(Image *image, char *filename){
    FILE* fp;
    int i, j;

    /* Opening */
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("error in opening file %s\n", filename);
        exit(1);
    }

    /*  Magic number writing */
    fprintf(fp, "P6\n");

    /* Dimensions */
    fprintf(fp, "%d %d \n", image->cols, image->rows);

    /* Max val */
    fprintf(fp, "%d\n", 255);

    /* Pixel values */
    for(i = 0; i < image->rows; i += 1){
        for(j = 0; j < image->cols ; j += 1){
            fwrite(&(image->rmap[i][j]), sizeof(char), 1, fp);
            fwrite(&(image->gmap[i][j]), sizeof(char), 1, fp);
            fwrite(&(image->bmap[i][j]), sizeof(char), 1, fp);
        }
    }

    /* Close file */
    fclose(fp);
}


//End Copywriten code


Image* openPPMImage(char* filename) {
    FILE* fp;
    int i, j;

    /* Opening */
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("error in opening file %s\n", filename);
        exit(1);
    }

    char magicbuff[2];
    fscanf(fp, "%s\n", magicbuff);
    if (strcmp(magicbuff, "P6") != 0) {
        printf("Wrong ppm file type, found %s, expected P6\n", magicbuff);
        exit(1);
    }

    int width, height;
    fscanf(fp, "%d %d \n", &width, &height);

    int max_val;
    fscanf(fp, "%d\n", &max_val);
    if (max_val != 255) {
        printf("Wrong maximum color value, found %d, expected 255\n", max_val);
        exit(1);
    }

    Image* new_image = calloc(1, sizeof(Image));
    new_image->cols = width;
    new_image->rows = height;
    //Initialize bitmaps
    new_image->rmap = calloc(height, sizeof(char*));
    new_image->gmap = calloc(height, sizeof(char*));
    new_image->bmap = calloc(height, sizeof(char*));
    for (int i = 0; i < height; i++) {
        new_image->rmap[i] = calloc(width, sizeof(char));
        new_image->gmap[i] = calloc(width, sizeof(char));
        new_image->bmap[i] = calloc(width, sizeof(char));
    }

    //Load old data into "Image"
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            char pix[3];
            fread(pix, 1, 3, fp);
            new_image->rmap[y][x] = pix[0];
            new_image->gmap[y][x] = pix[1];
            new_image->bmap[y][x] = pix[2];
        }
    }

    fclose(fp);

    return new_image;
}

void freeBitmaps(Image *image) {
    for (int i = 0; i < image->rows; i++) {
        free(image->rmap[i]);
        free(image->gmap[i]);
        free(image->bmap[i]);
    }

    free(image->rmap);
    free(image->gmap);
    free(image->bmap);
}
