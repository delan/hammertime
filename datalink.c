#include "datalink.h"

#include <string.h>

#include "util.h"
#include "assignment.h"
#include "network.h"

void cc200_datalink_from_network(cc200_packet_t packet) {
	int link = cc200_routing_table[packet.source][packet.destination];
	CC200_PRINT(
		"data " CC200_SEQ CC200_TO CC200_LINK,
		cc200_next_seq_to_send[link], link
	);
	cc200_frame_t frame;
	frame.checksum = 0;
	frame.kind = CC200_DATA;
	frame.sequence_number = cc200_next_seq_to_send[link];
	memcpy(&frame.payload, &packet, sizeof(packet));
	frame.checksum = CNET_crc32(
		(void *) &frame,
		sizeof(frame)
	);
	cc200_next_seq_to_send[link] ^= 1;
	CC200_CHECK(CNET_disable_application(ALLNODES));
	cc200_physical_from_datalink(frame, link);
}

void cc200_datalink_ack(cc200_byte sequence_number, int link) {
	CC200_PRINT(
		"ACK " CC200_SEQ CC200_TO CC200_LINK,
		sequence_number, link
	);
	cc200_frame_t frame;
	frame.checksum = 0;
	frame.kind = CC200_ACK;
	frame.sequence_number = sequence_number;
	frame.checksum = CNET_crc32(
		(void *) &frame,
		sizeof(frame)
	);
	cc200_physical_from_datalink(frame, link);
}

void cc200_datalink_from_physical(cc200_frame_t frame, int link) {
	uint32_t alleged_checksum = frame.checksum;
	frame.checksum = 0;
	uint32_t actual_checksum = CNET_crc32(
		(void *) &frame,
		sizeof(frame)
	);
	if (alleged_checksum == actual_checksum) {
		switch (frame.kind) {
		case CC200_DATA:
			CC200_PRINT(
				"data " CC200_SEQ CC200_FROM CC200_LINK,
				frame.sequence_number, link
			);
			cc200_datalink_ack(frame.sequence_number, link);
			if (
				frame.sequence_number ==
				cc200_next_data_seq_expected[link]
			) {
				cc200_next_data_seq_expected[link] ^= 1;
				cc200_network_from_datalink(frame.payload);
			}
			break;
		case CC200_ACK:
			CC200_PRINT(
				"ACK " CC200_SEQ CC200_FROM CC200_LINK,
				frame.sequence_number, link
			);
			if (
				frame.sequence_number ==
				cc200_next_ack_seq_expected[link]
			) {
				cc200_next_ack_seq_expected[link] ^= 1;
				CC200_CHECK(
					CNET_enable_application(ALLNODES)
				);
			}
			break;
		}
	} else {
		CC200_PRINT("bad frame" CC200_FROM CC200_LINK, link);
	}
}
