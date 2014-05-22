#ifndef CC200_DATALINK_H
#define CC200_DATALINK_H

#include "type.h"
#include "physical.h"

void cc200_datalink_from_network(cc200_packet_t packet);

void cc200_datalink_ack(int link, cc200_byte sequence_number);

void cc200_datalink_from_physical(cc200_frame_t frame);

#endif
