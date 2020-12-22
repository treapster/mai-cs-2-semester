
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
