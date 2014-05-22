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
	CC200_PRINT(
		"%zu bytes" CC200_FROM CC200_LINK,
		len, link
	);
	cc200_datalink_from_physical(frame, link);
}

void cc200_physical_from_datalink(cc200_frame_t frame, int link) {
	size_t len = sizeof(frame);
	CC200_PRINT(
		"%zu bytes" CC200_TO CC200_LINK,
		len, link
	);
	CC200_CHECK(CNET_write_physical(link, (void *) &frame, &len));
}
