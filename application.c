#include "application.h"

#include <stdlib.h>

#include "type.h"
#include "util.h"

EVENT_HANDLER(cc200_application_ready) {
	CnetAddr dest;
	cc200_byte payload[CC200_MAX_APPLICATION];
	size_t len = CC200_MAX_APPLICATION;
	CC200_CHECK(CNET_read_application(&dest, payload, &len));
	CC200_PRINT("%zu bytes to node %d", len, dest);
	CNET_disable_application(ALLNODES);
	/*cc200_network_from_application(payload, len);*/
}
