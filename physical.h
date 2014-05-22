#ifndef CC200_PHYSICAL_H
#define CC200_PHYSICAL_H

#include <cnet.h>

#include "type.h"

EVENT_HANDLER(cc200_physical_ready);

void cc200_physical_from_datalink(cc200_frame_t frame);

#endif
