#include "udt.h"

Dequeue* create_dequeue()
{
	Dequeue* deq=(Dequeue*)malloc(sizeof(Dequeue));
	deq->size=0;
	deq->data=NULL;
	return deq;
}
void push_front(Dequeue* deq, int value)
{
	if(deq->data==NULL)
	{
		deq->data=(int*)malloc(sizeof(int));
		deq->data[0]=value;
		++deq->size;
		return;
	}
	int* new=(int*)malloc(sizeof(int)*(deq->size+1));
	new[0]=value;
	for(int i=1;i<=deq->size;i++)
	{
		new[i]=deq->data[i-1];
	}
	free(deq->data);
	deq->data=new;
	++deq->size;
	return;	
}
void push_back(Dequeue* deq, int value)
{
	if(deq->data==NULL)
	{
		deq->data=(int*)malloc(sizeof(int));
		deq->data[0]=value;
		++deq->size;
		return;
	}
	deq->data=realloc(deq->data,sizeof(int)*(deq->size+1));
	deq->data[deq->size]=value;
	++deq->size;
	return;
}
int pop_front(Dequeue* deq)
{

	if(deq->data==NULL)
	{
		return 0;
	}
	int* new=(int*)malloc(sizeof(int)*(deq->size-1));
	int value=deq->data[0];
	for(int i=1;i<deq->size;++i)
	{
		new[i-1]=deq->data[i];
	}
	free(deq->data);
	deq->data=new;
	deq->size--;
	return value;	
}
int pop_back(Dequeue* deq)
{
	if(deq->data==NULL)
	{
		return 0;
	}
	int value=deq->data[deq->size-1];
	deq->data=realloc(deq->data,sizeof(int)*deq->size-1);
	--deq->size;
	return value;
}
Dequeue* merge(Dequeue* a, Dequeue* b)
{
	int a_index=0;
	int b_index=0;
	int overall_size=a->size+b->size;
	int* new_data=(int*)malloc(sizeof(int)*overall_size);
	for(int i=0;i<overall_size;++i)
	{
		if(a_index<a->size && b_index<b->size)
		{
			if(a->data[a_index]<b->data[b_index])
			{
				new_data[i]=a->data[a_index];
				++a_index;
			}
			else
			{
				new_data[i]=b->data[b_index];
				++b_index;
			}
			continue;

		}
		if(a_index==a->size)
		{
			new_data[i]=b->data[b_index];
			++b_index;
			continue;
		}
		if(b_index==b->size)
		{
			new_data[i]=a->data[a_index];
			++a_index;
		}
		
	}
	Dequeue* new_deq=create_dequeue();
	new_deq->data=new_data;
	new_deq->size=overall_size;
	return new_deq;
}
Dequeue* merge_sort(Dequeue** arr, int left, int right)
{
	if(left==right)
	{
		return arr[left];
	}
	int middle=(left+right)/2;
	Dequeue* left_deq=merge_sort(arr, left, middle);
	Dequeue* right_deq=merge_sort(arr, middle+1, right);
	Dequeue* new=merge(left_deq, right_deq);
	delete_dequeue(left_deq);
	delete_dequeue(right_deq);
	return new;
}	
Dequeue** create_deq_arr(Dequeue* deq)
{
	int size=deq->size;
	Dequeue** arr=(Dequeue**)malloc(sizeof(Dequeue*)*size);
	for(int i=size-1;i>=0;--i)
	{
		arr[i]=create_dequeue();
		push_front(arr[i], pop_back(deq));
	}
	return arr;
}
void delete_dequeue(Dequeue* deq)
{
	if(deq==NULL)
		return;
	if(deq->data==NULL)
	{
		free(deq);
		return;
	}
	free(deq->data);
	free(deq);
	return;
}
