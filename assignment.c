#include <cnet.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CC200_PRINT_ARGS(FORMAT, ...)                                  \
	"%d:%s:%d: " FORMAT "\n",                                      \
	nodeinfo.nodenumber,                                           \
	__func__,                                                      \
	__LINE__,                                                      \
	__VA_ARGS__

#define CC200_PRINT(FORMAT, ...) do {                                  \
	printf(CC200_PRINT_ARGS(FORMAT, __VA_ARGS__));                 \
	fprintf(stderr, CC200_PRINT_ARGS(FORMAT, __VA_ARGS__));        \
} while (0)

#define CC200_CHECK_BODY do {                                          \
	CC200_PRINT(                                                   \
		"%s: %s",                                              \
		cnet_errname[cnet_errno],                              \
		cnet_errstr[cnet_errno]                                \
	);                                                             \
	exit(0);                                                       \
} while (0)

#define CC200_CHECK(CALL) do {                                         \
	if ((CALL) != 0)                                               \
		CC200_CHECK_BODY;                                      \
} while (0)

#define CC200_CHECK0(CALL) do {                                        \
	if ((CALL) == 0)                                               \
		CC200_CHECK_BODY;                                      \
} while (0)

#define CC200_MAX_APPLICATION 256

typedef uint8_t byte;

typedef struct cc200_frame {
	byte sequence_number;
	byte payload_length;
	byte payload[CC200_MAX_APPLICATION];
} cc200_frame_t;

EVENT_HANDLER(application_ready) {
	CnetAddr dest;
	byte payload[CC200_MAX_APPLICATION];
	size_t len = CC200_MAX_APPLICATION;
	CC200_CHECK(CNET_read_application(&dest, payload, &len));
	CC200_PRINT("need to send %zu bytes to node %d", len, dest);
}

EVENT_HANDLER(physical_ready) {
	int link;
	byte raw[80];
	size_t len = sizeof(raw);
	CC200_CHECK(CNET_read_physical(&link, raw, &len));
	CC200_PRINT("received %zu bytes from link %d", len, link);
}

EVENT_HANDLER(reboot_node) {
	CC200_PRINT(
		"%s has %d links",
		nodeinfo.nodename,
		nodeinfo.nlinks
	);
	CC200_CHECK(CNET_set_handler(
		EV_APPLICATIONREADY, application_ready, 0
	));
	CC200_CHECK(CNET_set_handler(
		EV_PHYSICALREADY, physical_ready, 0
	));
	CC200_CHECK(CNET_enable_application(ALLNODES));
}
