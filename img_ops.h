#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "read_write_ops.h"


/* Embeds the given "msg" in the last bits of the "r" "g" and "b" values respecively
 * "msg" does not have any length requirement 
 * REQUIREMENT: Image must have minimum len(msg) * 8 pixels
 *
 */
void hide_message(Image *image, const char *msg);


/* Retrieves a message that has been embeded by "hide_message".
 * Has now length requirement
 * Requires that image meets same size standards of hide_message
 */
char* decode_message(Image *image);
