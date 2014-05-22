#include "assignment.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

int cc200_routing_table[CC200_NODES][CC200_NODES] = {
	{ 0, 1, 2, 3, 4 },
	{ 1, 0, 2, 1, 1 },
	{ 1, 2, 0, 1, 1 },
	{ 1, 1, 1, 0, 2 },
	{ 1, 1, 1, 2, 0 },
};

cc200_byte cc200_next_seq_to_send[CC200_NODES];
cc200_byte cc200_next_data_seq_expected[CC200_NODES];
cc200_byte cc200_next_ack_seq_expected[CC200_NODES];

#include "physical.h"
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
	CC200_CHECK(CNET_enable_application(ALLNODES));
}
