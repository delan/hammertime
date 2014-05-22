#include "application.h"

#include <stdlib.h>

#include "type.h"
#include "util.h"
#include "network.h"

EVENT_HANDLER(cc200_application_ready) {
	CnetAddr dest;
	cc200_byte payload[CC200_MAX_APPLICATION];
	size_t len = CC200_MAX_APPLICATION;
	CC200_CHECK(CNET_read_application(&dest, payload, &len));
	CC200_PRINT(
		"%zu bytes" CC200_TO CC200_NODE,
		len, dest
	);
	cc200_network_from_application(
		nodeinfo.nodenumber,
		dest,
		payload,
		len
	);
}

void cc200_application_from_network(cc200_byte *payload, cc200_byte len) {
	size_t payload_length = len;
	CC200_PRINT(
		"%zu bytes received",
		payload_length
	);
	CC200_CHECK(CNET_write_application(payload, &payload_length));
}
