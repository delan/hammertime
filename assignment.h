#ifndef CC200_ASSIGNMENT_H
#define CC200_ASSIGNMENT_H

#include <cnet.h>

#include "type.h"
#include "list.h"

#define CC200_NODES 5

extern int cc200_routing_table[CC200_NODES][CC200_NODES];

extern cc200_byte cc200_next_seq_to_send[CC200_NODES];
extern cc200_byte cc200_next_data_seq_expected[CC200_NODES];
extern cc200_byte cc200_next_ack_seq_expected[CC200_NODES];

extern cc200_list cc200_frame_queue[CC200_NODES];

EVENT_HANDLER(cc200_test);
EVENT_HANDLER(cc200_reboot_node);

#endif
