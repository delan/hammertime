#include "datalink.h"

#include <string.h>

#include "util.h"
#include "assignment.h"

void cc200_datalink_from_network(cc200_packet_t packet) {
	int link = cc200_routing_table[packet.source][packet.destination];
	CC200_PRINT(
		"building frame with seq %u",
		cc200_next_seq_to_send[link]
	);
	cc200_frame_t frame;
	frame.checksum = 0;
	frame.sequence_number = cc200_next_seq_to_send[link];
	memcpy(&frame.payload, &packet, sizeof(packet));
	frame.checksum = CNET_crc32(
		(void *) &frame,
		sizeof(frame)
	);
	cc200_next_seq_to_send[link] ^= 1;
	cc200_physical_from_datalink(frame);
}
