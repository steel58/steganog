#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "read_write_ops.h"


/* Embeds the given "msg" in the last bits of the "r" "g" and "b" values respecively
 * "msg" does not have any length requirement but a length of 14 is assumed when retrieving
 * a "msg".
 * REQUIREMENT: Image must have minimum len(msg) * 8 pixels
 *
 */
void hide_message(Image *image, const char *msg);


/* Retrieves a message that has been embeded by "hide_message".
 * Expects message to be of length 14
 * Requires that image meets same size standards of hide_message
 * msg is the buffer that the message will be written into and data there will not be saved
 */
char* decode_message(Image *image);
