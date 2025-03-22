typedef struct {
    int rows;
    int cols;
    char **rmap;
    char **gmap;
    char **bmap;
} Image;

Image* readPNGImage(char* filename);
void writePPMImage(Image *image, char *filename);

/*
 * Opens a PPM image made by "writePPMImage" into an Image struct
 * this allocates memory (duh) so free it later
 */
Image* openPPMImage(char* filename);

/*
 * Frees internal stuff for result from "openPPMimage"
 * Call this then free(image)
 */
void freeBitmaps(Image *image);
