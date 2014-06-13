#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include "init.h"

/********** Sunjin's code ***********/
void insertData(int data, int line, int col){
	node* new_node;
	node* prev_node;

	new_node = (node*)malloc(sizeof(node));
	new_node->next = NULL;
	new_node->data = data;
	new_node->line = line;
	new_node->col = col;

	if(head == NULL){
		head = new_node;
	}else{
		prev_node = head;
		while(prev_node->next != NULL){
			prev_node = prev_node->next;
		}
		prev_node->next = new_node;
	}
	
	return;
}

void deleteData(int line, int col){
	if(head != NULL){
		node* cur_node;
		node* prev_node;

		cur_node = head;
		while(cur_node->next != NULL){
			if(col != 0){
				if(col == 1 && line == 1){
					head = cur_node->next;
					// free(cur_node);
					return;
				}else{
					prev_node = cur_node;
					cur_node = prev_node->next;

					if((cur_node->line == line) && (cur_node->col == col)){
						prev_node->next = cur_node->next;
						// free(cur_node);
						return;
					}
				}
			}else{
				if(line == 1){
					prev_node = cur_node;
					cur_node = prev_node->next;

					if(cur_node->line == line){
						prev_node->next = cur_node->next;
						if(prev_node->next == NULL){
							head = NULL;
							return;
						}
					}
				}else{
					prev_node = cur_node;
					cur_node = prev_node->next;

					if(cur_node->line == line){
						prev_node->next = cur_node->next;
						if(cur_node->col == 1) return;
						cur_node = cur_node->next;
					}
				}
			}
		}
	}
	return;
}

void printList(node* head, FILE* f){
	node* cur;

	cur = head;
	for(cur = head; cur->next != NULL; cur = cur->next){
		fputc(cur->data, f);
	}
	fputc(cur->data, f);

	return;
}
