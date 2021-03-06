#ifndef CC200_TYPE_H
#define CC200_TYPE_H

#include <cnet.h>
#include <stdint.h>

#define CC200_MAX_APPLICATION 255

typedef enum cc200_frame_type {
	CC200_DATA,
	CC200_ACK
} cc200_frame_type_t;

typedef uint8_t cc200_byte;

typedef struct cc200_packet {
	CnetAddr source;
	CnetAddr destination;
	cc200_byte payload_length;
	cc200_byte payload[CC200_MAX_APPLICATION];
} __attribute__ ((packed)) cc200_packet_t;

typedef struct cc200_frame {
	uint32_t checksum;
	cc200_frame_type_t kind;
	cc200_byte sequence_number;
	cc200_packet_t payload;
} __attribute__ ((packed)) cc200_frame_t;

#endif
