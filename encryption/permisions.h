#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* this will hash a username password pair and store them in steganog_login.txt
 * this will be the reference for logingin in in the future
 */
void new_login(char* username, char* password);

/* this will check if the given credentials match the hashed login data
 * stored in steganog_login.txt and returns true if both match
 */
bool check_credentials(char* username, char* password);

/* this will pad a password based on the PKCS#7 standard until it is length
 * "length" and will return a pointer to the resulting string
 */
char *pad_password(char* password, int length);

/* this will encrypt a password using aes and return a pointer to the resulting
 * string, probably
 */
char *encrypt_password(char* password);
