#include "list.h"

#include <stdlib.h>

cc200_list_node cc200_list_node_new(cc200_frame_t data) {
	cc200_list_node node = malloc(sizeof(struct cc200_list_node));
	node->data = data;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

void cc200_list_node_free(cc200_list_node node) {
	free(node);
}

cc200_list cc200_list_new(void) {
	cc200_list list = malloc(sizeof(struct cc200_list));
	list->count = 0;
	list->head = NULL;
	list->tail = NULL;
	return list;
}

void cc200_list_free(cc200_list list) {
	cc200_list_node node = list->head;
	while (node) {
		cc200_list_node next = node->next;
		cc200_list_node_free(node);
		node = next;
	}
	free(list);
}

cc200_list_node cc200_list_insert_after(cc200_list list,
	cc200_frame_t data, cc200_list_node which) {
	cc200_list_node before = which;
	cc200_list_node node = cc200_list_node_new(data);
	cc200_list_node after = which ? which->next : list->head;
	if (before) {
		before->next = node;
		node->prev = before;
	} else {
		list->head = node;
	}
	if (after) {
		node->next = after;
		after->prev = node;
	} else {
		list->tail = node;
	}
	list->count++;
	return node;
}

cc200_list_node cc200_list_remove(cc200_list list,
	cc200_list_node node) {
	cc200_list_node before = node->prev;
	cc200_list_node after = node->next;
	if (list->head == node)
		list->head = node->next;
	if (list->tail == node)
		list->tail = node->prev;
	if (before)
		before->next = node->next;
	if (after)
		after->prev = node->prev;
	node->prev = NULL;
	node->next = NULL;
	list->count--;
	return node;
}
