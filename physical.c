#include "physical.h"

#include <stdlib.h>

#include "util.h"
#include "assignment.h"
#include "datalink.h"

EVENT_HANDLER(cc200_physical_ready) {
	int link;
	cc200_frame_t frame;
	size_t len = sizeof(frame);
	CC200_CHECK(CNET_read_physical(&link, &frame, &len));
	CC200_PRINT("%zu bytes from link %d", len, link);
	cc200_datalink_from_physical(frame);
}

void cc200_physical_from_datalink(cc200_frame_t frame) {
	size_t len = sizeof(frame);
	CC200_PRINT("%zu bytes over link %d", len, frame.link);
	CNET_write_physical(frame.link, (void *) &frame, &len);
}
