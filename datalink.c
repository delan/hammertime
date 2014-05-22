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
	cc200_list_insert_after(
		cc200_frame_queue[link],
		frame,
		cc200_frame_queue[link]->tail
	);
	if (cc200_frame_queue[link]->count == 1)
		cc200_datalink_data_next(link);
}

void cc200_datalink_data_next(int link) {
	CnetTime timeout =
		sizeof(cc200_frame_t) * 8000000 /
		linkinfo[link].bandwidth +
		linkinfo[link].propagationdelay;
	timeout *= 3;
	CC200_CHECK(CNET_disable_application(ALLNODES));
	CC200_PRINT(
		"starting timer for " CC200_LINK " at %" PRId64 " us",
		link, timeout
	);
	CC200_CHECK0(
		cc200_link_timer_vector[link] =
		CNET_start_timer(
			cc200_link_event_vector[link],
			timeout,
			0
		)
	);
	cc200_physical_from_datalink(
		cc200_frame_queue[link]->head->data,
		link
	);
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
				CC200_CHECK(
					CNET_stop_timer(
						cc200_link_timer_vector[link]
					)
				);
				cc200_list_node_free(
					cc200_list_remove(
						cc200_frame_queue[link],
						cc200_frame_queue[link]->head
					)
				);
				cc200_next_ack_seq_expected[link] ^= 1;
				if (cc200_frame_queue[link]->count > 0)
					cc200_datalink_data_next(link);
				else
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
