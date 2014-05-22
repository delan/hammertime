#include "assignment.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "physical.h"
#include "application.h"

int cc200_routing_table[CC200_NODES][CC200_NODES] = {
	{ 0, 1, 2, 3, 4 },
	{ 1, 0, 2, 1, 1 },
	{ 1, 2, 0, 1, 1 },
	{ 1, 1, 1, 0, 2 },
	{ 1, 1, 1, 2, 0 },
};

/**
 * The indices to the following arrays are link numbers, not node addresses.
 * The capacity of the arrays is CC200_NODES because in the worst case,
 * each node is connected to itself (link 0) and all other nodes (links 1
 * through (CC200_NODES - 1)).
 */

cc200_byte cc200_next_seq_to_send[CC200_NODES];
cc200_byte cc200_next_data_seq_expected[CC200_NODES];
cc200_byte cc200_next_ack_seq_expected[CC200_NODES];
cc200_list cc200_frame_queue[CC200_NODES];

EVENT_HANDLER(cc200_reboot_node) {
	for (int i = 0; i < CC200_NODES; i++)
		cc200_frame_queue[i] = cc200_list_new();
	if (nodeinfo.nodenumber == 0) /* print once */
		CC200_PRINT(
			"cnet started with pid %d",
			getpid()
		);
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
