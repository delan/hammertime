#ifndef CC200_NETWORK_H
#define CC200_NETWORK_H

#include <cnet.h>

#include "type.h"

void cc200_network_from_application(
	CnetAddr source,
	CnetAddr destination,
	cc200_byte *payload,
	cc200_byte len
);

#endif
