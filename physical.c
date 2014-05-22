#include "physical.h"

#include <stdlib.h>

#include "type.h"
#include "util.h"

EVENT_HANDLER(cc200_physical_ready) {
	int link;
	cc200_frame_t frame;
	cc200_byte *raw = (cc200_byte *) &frame;
	size_t len = sizeof(frame);
	CC200_CHECK(CNET_read_physical(&link, raw, &len));
	uint32_t alleged_checksum = frame.checksum;
	frame.checksum = 0;
	uint32_t actual_checksum = CNET_crc32(raw, sizeof(frame));
	if (alleged_checksum == actual_checksum)
		CC200_PRINT("valid %zu bytes from link %d", len, link);
	else
		CC200_PRINT("invalid %zu bytes from link %d", len, link);
}
