#ifndef _UDT_H_
#define _UDT_H_
#include <stdio.h>
#include <stdlib.h>
typedef struct{
	int size;
	int* data;

}Dequeue;

Dequeue* create_dequeue();
void push_front(Dequeue* deq, int value);
void push_back(Dequeue* deq, int value);
int pop_front(Dequeue* deq);
int pop_back(Dequeue* deq);
Dequeue* merge(Dequeue* a, Dequeue* b);
Dequeue* merge_sort(Dequeue** arr, int left, int right);
Dequeue** create_deq_arr(Dequeue* deq);
void delete_dequeue(Dequeue* deq);


#endif