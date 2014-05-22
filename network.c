#include "network.h"

#include <string.h>

#include "util.h"
#include "datalink.h"
#include "application.h"

void cc200_network_from_application(
	CnetAddr source,
	CnetAddr destination,
	cc200_byte *payload,
	cc200_byte len
) {
	cc200_packet_t packet;
	packet.source = source;
	packet.destination = destination;
	packet.payload_length = len;
	memcpy(&packet.payload, payload, len);
	CC200_PRINT(
		"%d bytes from node %d to %d",
		len, source, destination
	);
	cc200_datalink_from_network(packet);
}

void cc200_network_from_datalink(cc200_packet_t packet) {
	if (packet.destination == nodeinfo.nodenumber) {
		CC200_PRINT(
			"packet from node %d up to application",
			packet.source
		);
		cc200_application_from_network(
			packet.payload,
			packet.payload_length
		);
	} else {
		CC200_PRINT(
			"forwarding packet from node %d to node %d",
			packet.source,
			packet.destination
		);
		packet.source = nodeinfo.nodenumber;
		cc200_datalink_from_network(packet);
	}
}
