#include "physical.h"

#include <stdlib.h>

#include "util.h"
#include "assignment.h"

EVENT_HANDLER(cc200_physical_ready) {
	int link;
	cc200_frame_t frame;
	size_t len = sizeof(frame);
	CC200_CHECK(CNET_read_physical(&link, &frame, &len));
	uint32_t alleged_checksum = frame.checksum;
	frame.checksum = 0;
	uint32_t actual_checksum = CNET_crc32(
		(void *) &frame,
		sizeof(frame)
	);
	if (alleged_checksum == actual_checksum)
		CC200_PRINT("valid %zu bytes from link %d", len, link);
	else
		CC200_PRINT("invalid %zu bytes from link %d", len, link);
}

void cc200_physical_from_datalink(cc200_frame_t frame) {
	int link = cc200_routing_table
		[frame.payload.source]
		[frame.payload.destination];
	size_t len = sizeof(frame);
	CC200_PRINT("%zu bytes over link %d", len, link);
	CNET_write_physical(link, (void *) &frame, &len);
}
