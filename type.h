#ifndef CC200_TYPE_H
#define CC200_TYPE_H

#define CC200_MAX_APPLICATION 256

typedef uint8_t cc200_byte;

typedef struct cc200_packet {
	CnetAddr source;
	CnetAddr destination;
	cc200_byte payload_length;
	cc200_byte payload[CC200_MAX_APPLICATION];
} __attribute__ ((packed)) cc200_packet_t;

typedef struct cc200_frame {
	uint32_t checksum;
	cc200_byte sequence_number;
	cc200_packet_t payload;
} __attribute__ ((packed)) cc200_frame_t;

#endif
