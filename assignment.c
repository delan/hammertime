#include "assignment.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "type.h"

#include "physical.h"
#if 0
#include "datalink.h"
#include "network.h"
#endif
#include "application.h"

EVENT_HANDLER(cc200_reboot_node) {
	CC200_PRINT(
		"%s has %d links",
		nodeinfo.nodename,
		nodeinfo.nlinks
	);
	CC200_CHECK(CNET_set_handler(
		EV_APPLICATIONREADY,
		cc200_application_ready,
		0
	));
	CC200_CHECK(CNET_set_handler(
		EV_PHYSICALREADY,
		cc200_physical_ready,
		0
	));
	/* CC200_CHECK(CNET_set_handler(
		EV_TIMER0,
		cc200_test,
		0
	)); */
	CC200_CHECK(CNET_enable_application(ALLNODES));
	/* CC200_CHECK0(CNET_start_timer(EV_TIMER0, 1, 0)); */
}

EVENT_HANDLER(cc200_test) {
	cc200_frame_t frame;
	cc200_byte *raw = (void *) &frame;
	size_t len = sizeof(frame);
	frame.checksum = 0;
	frame.checksum = CNET_crc32(raw, sizeof(frame));
	CC200_CHECK(CNET_write_physical(1, raw, &len));
}
