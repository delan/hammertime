#ifndef CC200_APPLICATION_H
#define CC200_APPLICATION_H

#include <cnet.h>

#include "type.h"

EVENT_HANDLER(cc200_application_ready);

void cc200_application_from_network(cc200_byte *payload, cc200_byte len);

#endif
