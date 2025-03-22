#include <stdio.h>
#include <string.h>
#include "img_ops.h"
#include "encryption/permisions.h"

int test(int, int);

int main() {
    printf("Reading PNG\n");
    Image* img = readPNGImage("./images/test.png");
    printf("Writing into PPM\n");
    writePPMImage(img, "./testOut/test1.ppm");
    printf("Check /testOut/test1.ppm\n");


    char hidden_message[100] = "test_message32";
    hide_message(img, hidden_message);
    char* found_message = decode_message(img);

    printf("%s\n", hidden_message);
    printf("%s\n", found_message);
    test(strcmp(hidden_message, found_message), 0);

    printf("Writting secret image to PPM\n");
    writePPMImage(img, "./testOut/test_secret.ppm");


    printf("Testing pad_password\n");
    char old_password[100] = "old";
    char new_password[100] = "old\03\03\03";
    char* test_password = pad_password(old_password, 6);
    printf("Expected: %s\n", new_password);
    printf("Actual: %s\n", test_password);
    test(strcmp(test_password, new_password), 0);
    test(strcmp(test_password, new_password), 0);

}

int test(int actual, int expected) {
    if (actual == expected) { 
        return 1;
    }

    printf("Test Failed: found %d, expected %d\n", actual, expected);

    return 0;
}
