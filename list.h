#ifndef CC200_LIST_H
#define CC200_LIST_H

#include <stddef.h>

#include "type.h"

typedef struct cc200_list_node {
	cc200_frame_t data;
	struct cc200_list_node *prev;
	struct cc200_list_node *next;
} *cc200_list_node;

typedef struct cc200_list {
	size_t count;
	cc200_list_node head;
	cc200_list_node tail;
} *cc200_list;

cc200_list_node cc200_list_node_new(cc200_frame_t data);

void cc200_list_node_free(cc200_list_node node);

cc200_list cc200_list_new(void);

void cc200_list_free(cc200_list list);

cc200_list_node cc200_list_insert_after(cc200_list list,
	cc200_frame_t data, cc200_list_node which);

cc200_list_node cc200_list_remove(cc200_list list,
	cc200_list_node node);

#endif
