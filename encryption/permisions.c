#include "aes.h"
#include "permisions.h"
#include "sha256.c"

void new_login(char* username, char* password) {
    char* hashed_username = SHA256(username);
    char* hashed_password = SHA256(password);

    FILE* fp = fopen("steganog_login.txt", "wb");
    fprintf(fp, "%s\n", hashed_username);
    fprintf(fp, "%s\n", hashed_password);
}

bool check_credentials(char* username, char* password) {
    char* hashed_username = SHA256(username);
    char* hashed_password = SHA256(password);

    FILE* fp = fopen("steganog_login.txt", "rb");
    char reference_username[100];
    char reference_password[100];
    fscanf(fp, "%s\n", reference_username);
    fscanf(fp, "%s\n", reference_password);

    if (strcmp(hashed_username, reference_username) != 0) {
        return false;
    }
    if (strcmp(hashed_password, reference_password) != 0) {
        return false;
    }

    return true;
}

char * pad_password(char* password, int length) {
    int orig_len = strlen(password);
    char padd_dist = (char) (length - orig_len);
    char *padded_password = calloc(length + 1, sizeof(char));
    for (int i = 0; i < orig_len; i++) {
        padded_password[i] = password[i];
    }

    for (int i = orig_len; i < length; i++) {
        padded_password[i] = padd_dist;
    }
    padded_password[length] = 0;

    return padded_password;
}

char *encrypt_password(char* password) {
    int orig_len = strlen(password);
    int new_len = AES_BLOCKLEN;
    for (new_len = AES_BLOCKLEN; new_len < orig_len; new_len += AES_BLOCKLEN);

    char* padded_password = pad_password(password, new_len);

    return NULL;
}
