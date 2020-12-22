#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

struct Item
{
	struct Node* data;
	struct Item* next;
	struct Item* last;
};
struct List
{
	int size;
	struct Item* begin;
	struct Item* end;
};
struct Node
{
	struct Node* parent;
	struct List* children;
	int value;
};
struct List* create_list()
{
	struct List* list=(struct List*)malloc(sizeof(struct List));
	list->size=0;
	list->begin=NULL;
	list->end=NULL;
	return list;
}
struct Item* push_item(struct List* list, struct Node* node)
{
	struct Item* new=(struct Item*)malloc(sizeof(struct Item));
	new->data=node;
	if(list->end==NULL)
	{
		list->begin=new;
		list->end=new;
		new->last=NULL;
		new->next=NULL;
		list->size++;
		return new;
	}

	list->end->next=new;
	new->last=list->end;
	new->next=NULL;
	new->data=node;
	list->end=new;
	list->size++;
	return new;
}
void delete_item(struct List* list, struct Item* item)
{
	if(item==NULL)
	{
		printf("delete_item: got NULL-item to delete!\n");
		return;
	}
	if(list->size==0)
	{
		return;
	}
	struct Item* left=item->last;
	struct Item* right=item->next;
	if(left==NULL && right==NULL)
	{
		list->begin=NULL;
		list->end=NULL;
		list->size=0;
		item->next=NULL;
		item->last=NULL;
		item->data=NULL;
		free(item);
		return;
	}
	if(left==NULL)
	{
		right->last=NULL;
		list->begin=right;
		list->size--;
		item->next=NULL;
		item->last=NULL;
		item->data=NULL;
		free(item);
		return;
	}
	if(right==NULL)
	{
		left->next=NULL;
		list->end=left;
		list->size--;
		item->next=NULL;
		item->last=NULL;
		item->data=NULL;
		free(item);
		return;
	}
	left->next=right;
	right->last=left;
	list->size--;
	item->next=NULL;
	item->last=NULL;
	item->data=NULL;
	free(item);
	return;
}
void delete_items(struct Item* item)
{
	struct Item* x=NULL;
	while(item!=NULL)
	{
		x=item->next;
		free(item);
		item=x;

	}
}
void delete_list(struct List* list)
{
	if(list==NULL)
		return;
	delete_items(list->begin);
	list->begin=NULL;
	list->end=NULL;
	list->size=0;
	free(list);
	return;
}
struct Item* find_item(struct List* list, struct Node* node)
{
	struct Item* a=list->begin;
	while(a->data!=node)
	{
		if(a==NULL)
			return NULL;
		a=a->next;
	}
	return a;
}
struct Tree
{
	struct Node* root;
	int size;
};
struct Tree* create_tree(void)
{
	struct Tree* tree=(struct Tree*)malloc(sizeof(struct Tree));
	tree->size=0;
	tree->root=NULL;
	return tree;
}
struct Node* create_node(struct Tree* tree, struct Node* parent, int value)
{
	struct Node* node=(struct Node*)malloc(sizeof(struct Node));

	node->value=value;
	node->children=create_list();
	if(parent==NULL)
	{
		if(tree->root==NULL)
		{
			tree->root=node;
			node->parent=NULL;

		}
		else
		{
		push_item(tree->root->children, node);
		node->parent=tree->root;
		}
		tree->size++;
		return node;
	}
	node->parent=parent;
	push_item(parent->children, node);
	tree->size++;
	return node;
}
void delete_node(struct Tree* tree, struct Node* node)
{
	if(node==NULL || tree==NULL)
	{
		return;
	}
	struct Item* a=node->children->begin;
	struct Item* tmp=a;
	while(a!=NULL)
	{
		tmp=a->next;
		delete_node(tree, a->data);
		a=tmp;
	}
	delete_list(node->children);
	node->children=NULL;
	node->value=0;
	if(node->parent!=NULL)
	{
		delete_item(node->parent->children, find_item(node->parent->children, node));
	}
	else
	{
		tree->root=NULL;
	}
	free(node);
	tree->size--;
	return;

}
void delete_tree(struct Tree* tree)
{
	if(tree->root==NULL)
		return;
	delete_node(tree,tree->root);
	free(tree);

}
int ok(struct Node* node)
{
	if(node==NULL)
		return 1;
	if(node->children->size==0)
		return 1;
	if(node->children->size==1)
		return ok(node->children->begin->data);
	return 0;
}
int is_linear(struct Tree* tree)
{
	return ok(tree->root);
}
void show_nodes(struct Node* node, int level)
{
	if(node==NULL)
		return;
	for(int i=0;i<level;i++)
		printf("  ");
	printf("%d\n", node->value);

	if(node->children->size==0)
		return;
	struct Item* a=node->children->begin;

	while(a!=NULL)
	{
		show_nodes(a->data,level+1);
		a=a->next;
	}
}
void draw(struct Tree* tree)
{
	show_nodes(tree->root,0);

}
int main()
{
	struct Tree* tree=create_tree();
	int running=1;
	char action[30];
	char* add="add";
	char* del="del";
	char* is="is";
	char* stop="stop";
	char* show="show";
	int value;
	struct Node* address;
	struct Node* newaddr=NULL;
	while(running)
	{
		scanf(" %s", action);
		if(strcmp(action,add)==0)
		{
			scanf(" %d %p", &value, &address);
			struct Node* newaddr=create_node(tree, address, value);
			printf("address of new node is %p\n", newaddr);
		}
		if(strcmp(action,is)==0)
		{
			if(is_linear(tree))
			{
				printf("Yes\n");
			}
			else
			{
				printf("No\n");
			}
			
		}
		if(strcmp(action,del)==0)
		{
			scanf(" %p", &address);
			delete_node(tree, address);
		}
		if(strcmp(action,show)==0)
		{
			printf("size is %d\n", tree->size);
			draw(tree);
			printf("\n");
		}
		if(strcmp(action,stop)==0)
		{
			running=0;
		}
	}
	delete_tree(tree);
}
