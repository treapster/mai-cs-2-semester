#include "udt.h"
int main()
{
	Dequeue* deq=create_dequeue();
	int value;
	char action;
	char place;
	int running=1;
	while(running)
	{
		scanf(" %c", &action);
		switch(action)
		{
			case('a'):
				scanf("%c", &place);
				scanf("%d", &value);
				if(place=='f')
					push_front(deq,value);
				else
					push_back(deq, value);
			break;

			case('p'):
				scanf("%c", &place);
				if(place=='f')
					pop_front(deq);
				else
					pop_back(deq);
			break;

			case('s'):;
				int size=deq->size;
				Dequeue** arr=create_deq_arr(deq);
				delete_dequeue(deq);
				Dequeue* new=merge_sort(arr, 0, size-1);
				deq=new;
				free(arr);
			break;

			case('q'):
				running=0;
				continue;
			break;
		}
		for(int i=0;i<deq->size;++i)
		{
			printf("%d ", deq->data[i]);
		}
		printf("\n");
	}
	delete_dequeue(deq);
}