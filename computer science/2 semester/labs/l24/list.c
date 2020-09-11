
enum stored_type
{
	variable, number, operator, bracket, unknown
};
struct Token
{
int level;
enum stored_type type;
	union
	{
		struct Char_vector* var_name;
		int value;
		char operator;
		char bracket;

	};
};
struct Item
{
	struct Token data;
	struct Item* next;
	struct Item* prev;
};
struct Token_list
{
	int size;
	struct Item* begin;
	struct Item* end;
};
struct Token_list* create_token_list()
{
	struct Token_list* list=(struct Token_list*)malloc(sizeof(struct Token_list));
	list->size=0;
	list->begin=NULL;
	return list;
}
struct Item* push_item(struct Token_list* list, struct Item* after, struct Token token)
{
	struct Item* new=(struct Item*)malloc(sizeof(struct Item));
	new->data=token;
	if(list->begin==NULL)
	{
		list->begin=new;
		list->end=new;
		new->prev=NULL;
		new->next=NULL;
		list->size++;
		return new;
	}
	if(after==NULL || after==list->end)
	{
		after=list->end;
		list->end=new;
	}
	new->prev=after;
	new->next=after->next;
	new->data=token;
	if(after->next!=NULL)
	{
		after->next->prev=new;
	}
	after->next=new;

	list->size++;
	return new;
}
struct Item* delete_item(struct Token_list* list, struct Item* item)
{
	if(item==NULL)
	{
		printf("delete_item: got NULL-item to delete!\n");
		return NULL;
	}
	if(list->size==0)
	{
		return NULL;
	}
	struct Item* left=item->prev;
	struct Item* right=item->next;
	if(list->size==1)
	{
		list->begin=NULL;
		list->size=0;
		item->next=NULL;
		item->prev=NULL;
		if(item->data.type=variable)
		{
			delete_char_vector(item->data.var_name);
		}
		item->data.type=unknown;
		item->data.value=0;
		free(item);
		return NULL;
	}
	if(item==list->begin)
	{
		list->begin=list->begin->next;
	}
	if(item==list->end)
	{
		list->end=list->end->prev;
	}
	if(left!=NULL)
	{
		left->next=right;
	}
	if(right!=NULL)
	{
		right->prev=left;
	}
	
	item->next=NULL;
	item->prev=NULL;
	
	free(item);
	list->size--;
	return right;
}
void clear_token_list(struct Token_list* list)
{
	struct Item* item=list->begin;
	struct Item* x=NULL;
	for(int i=0;i<list->size;i++)
	{
		x=item->next;
		item->next=NULL;
		item->prev=NULL;
		if(item->data.type==variable)
		{
			delete_char_vector(item->data.var_name);
			item->data.type=unknown;
		}
		free(item);
		item=x;

	}
	list->begin=NULL;
	list->size=0;
}
void delete_token_list(struct Token_list* list)
{
	if(list==NULL)
		return;
	clear_token_list(list);
	free(list);
	return;
}
/*
int main()
{
	struct Token_list* list=create_list();
	struct Item* curr_item=NULL;
	int running=1;
	char action[30];
	char value;
	while(running)
	{
		if(list->size==0)
			printf("list is empty\n");
		else
		{
			struct Item* x=list->begin;
			for(int i=0;i<list->size;i++)
			{
				if(curr_item!=x)
					printf("%c ", x->data);
				else
					printf("(%c) ", x->data);

				x=x->next;

			}
			printf("\n");
		}
		scanf(" %s", action);
		if(strcmp(action,"add")==0)
		{
			scanf(" %c", &value);
			curr_item=push_item(list, curr_item, value);
		}
		if(strcmp(action,"del")==0)
		{
			curr_item=delete_item(list, curr_item);
		}
		if((strcmp(action,"next")==0 || strcmp(action,"n")==0) && curr_item!=NULL)
		{
			if(curr_item->next!=NULL)
				curr_item=curr_item->next;
		}
		if((strcmp(action,"back")==0 || strcmp(action,"prev")==0 || strcmp(action,"p")==0) && curr_item!=NULL)
		{
			if(curr_item->prev!=NULL)
			curr_item=curr_item->prev;
		}
		if(strcmp(action,"clear")==0)
		{
			clear_list(list);
		}
		if(strcmp(action,"size")==0)
		{
			printf("size of list is %d\n", list->size);
		}
		if(strcmp(action,"help")==0 || strcmp(action,"h")==0)
		{
			printf("\nhelp or h - displays this guide\n"
				"add X - adds value X to the current position\n"
				"del - deletes current item\n"
				"n or next - switches to the next item\n"
				"p or prev or back - switches to the prevous item\n"
				"clear - clears list\n"
				"swap [k] - swaps k-1-st and k+1st items\n"
				"head - makes current item head of list\n"
				"size - print size of the list\n"
				"stop or q - quit the program\n\n");
		}
		if(strcmp(action,"stop")==0 || strcmp(action,"q")==0)
		{
			running=0;
		}
	}
	delete_list(list);
}
*/