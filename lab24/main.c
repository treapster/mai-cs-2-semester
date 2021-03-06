#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "char_vector.c"
#include "list.c"

enum stored_type type_of_char(char symbol){
	if((symbol>='a' && symbol <='z') || (symbol>='A' && symbol<='Z')){
		return variable;
	}
	if(symbol>='0' && symbol<='9'){
		return number;
	}
	if(symbol=='*' || symbol=='/' || symbol =='+' || symbol=='-' || symbol=='^'){
		return operator;
	}
	if(symbol=='(' || symbol==')'){
		return bracket;
	}
	return unknown;
}


int parse(struct Char_vector* expression, struct Token_list* tokens){
	int positive=1;
	int token_count=0;
	int letter_count=0;
	char symbol;
	enum stored_type type;
	struct Token curr_token;
	curr_token.type=unknown;
	curr_token.var_name=NULL;
	for(int i=0;expression->data[i]!=0;i++){
		symbol=expression->data[i];
		type=type_of_char(symbol);
		if(type!=unknown){
			if(curr_token.type==unknown){
				curr_token.type=type;
			}
			if(curr_token.type!=type){
				if(curr_token.type==number){
					curr_token.value*=positive;
					positive=1;
				}
				push_item(tokens, NULL, curr_token);
				curr_token.type=type;
			}
			switch(type){
				case variable:
					if(curr_token.var_name==NULL){
						curr_token.var_name=create_char_vector(0);
					}
					push_char(curr_token.var_name,symbol);
					break;
				case number:
					curr_token.value*=10;
					curr_token.value+=symbol-'0';
					break;
				case operator:
					// unary plus and minus cases
					if(tokens->size==0 || (tokens->end->data.type==bracket && tokens->end->data.bracket=='(')){
						if(symbol=='+'){
							curr_token.type=unknown;
							curr_token.var_name=NULL;
							continue;
						}
						int next_type=type_of_char(expression->data[i+1]);
						if(symbol=='-' && (next_type==variable || next_type == bracket)){
							curr_token.type=number;
							curr_token.value=-1;
							push_item(tokens, NULL, curr_token);
							curr_token.type=operator;
							curr_token.operator='*';
							push_item(tokens, NULL, curr_token);
							curr_token.type=unknown;
							curr_token.var_name=NULL;
							continue;
						}
						if(symbol=='-' && next_type==number){
							positive=-1;
							curr_token.type=unknown;
							curr_token.var_name=NULL;
							continue;
						}
					}
					if(symbol=='-'){ //biary minus transformed to plus
						int next_type=type_of_char(expression->data[i+1]);
						if(next_type==variable || next_type==bracket){
							curr_token.type=operator;
							curr_token.operator='+';
							push_item(tokens, NULL, curr_token);
							curr_token.type=number;
							curr_token.value=-1;
							push_item(tokens,NULL, curr_token);
							curr_token.type=operator;
							curr_token.operator='*';
							push_item(tokens,NULL, curr_token);
							curr_token.type=unknown;
							curr_token.var_name=NULL;
							continue;
						}
						if(next_type==number){
							curr_token.type=operator;
							curr_token.operator='+';
							push_item(tokens, NULL, curr_token);
							positive=-1;
							curr_token.type=unknown;
							curr_token.var_name=NULL;
							continue;
						}
					}
					// +, -. ^ and / operator tokenized as is
					curr_token.type=operator;
					curr_token.operator=symbol;
					push_item(tokens, NULL, curr_token);
					curr_token.type=unknown;
					curr_token.var_name=NULL;
					break;
				case bracket:
					curr_token.bracket=symbol;
					push_item(tokens, NULL, curr_token);
					curr_token.type=unknown;
					curr_token.var_name=NULL;
					break;
			}
			
		}
		else{
			if(symbol==' '){
				continue;
			}
			else{
				printf("%c is unknown symbol\n", symbol);
				if(curr_token.type==variable){
					delete_char_vector(curr_token.var_name);
				}
				clear_char_vector(expression);
				clear_token_list(tokens);

				return -1;
			}
			
		}
	}
	if(curr_token.type!=unknown){
		if(curr_token.type==number){
				curr_token.value*=positive;
				positive=1;
			}
		push_item(tokens, NULL, curr_token);
	}
	return 0;
}

struct Node{
	struct Token* data;
	struct Node* left;
	struct Node* right;
};
struct Node* create_node(void){
	struct Node* node=(struct Node*)malloc(sizeof(struct Node));
	node->data=NULL;
	node->left=NULL;
	node->right=NULL;
	return node;
}
void delete_node(struct Node* node){
	if(node==NULL)
		return;
	delete_node(node->left);
	delete_node(node->right);
	free(node);
}
struct Node* calculate(struct Node* node){
	if(node==NULL){
		return NULL;
	}
	if(node->left==NULL || node->right==NULL){
		if(node->data->type==number){
			return node;
		}
		else{
			return NULL;
		}
	}
	if(node->data->type==number){
		return node;
	}
	if(node->data->type==operator && node->data->operator=='+' && node->left->data->type==number && node->right->data->type==number){
		node->data->type=number;
		node->data->value=node->left->data->value+node->right->data->value;
		delete_node(node->left);
		delete_node(node->right);
		node->left=NULL;
		node->right=NULL;
		return node;
	}
	if(node->data->type==operator && node->data->operator=='+'){
		struct Node* left=calculate(node->left);
		struct Node* right=calculate(node->right);
		if(left!=NULL && right!=NULL){
			right->data->value=left->data->value+right->data->value;
			left->data->value=0;
			return right;
		}
		if(left!=NULL)
			return left;
		else
			return right;
	}
	
	if(node->data->type==operator && (node->data->operator=='*' || node->data->operator=='/' || node->data->operator=='^')){
		calculate(node->left);
		calculate(node->right);
		return NULL;
	}
	return NULL;
}
int remove_zeros(struct Node* node){
	if(node==NULL){
		return 0;
	}
	if(node->data->type==number && node->data->value==0){
		return 1;
	}
	int left=remove_zeros(node->left);
	int right=remove_zeros(node->right);
	if(left && right){
		struct Node* old_left=node->left;
		struct Node* old_right=node->right;
		node->data=node->left->data;
		node->right=node->left->right;
		node->left=node->left->left;
		delete_node(old_left);
		delete_node(old_right);
		return 1;
	}
	if(left){
		struct Node* old_left=node->left;
		struct Node* old_right=node->right;
		node->data=node->right->data;
		node->left=node->right->left;
		node->right=node->right->right;
		old_right->left=NULL;
		old_right->right=NULL;
		delete_node(old_left);
		delete_node(old_right);
		return 0;
	}
	if(right){
		struct Node* old_right=node->right;
		struct Node* old_left=node->left;
		node->data=node->left->data;
		node->right=node->left->right;
		node->left=node->left->left;
		old_left->right=NULL;
		old_left->left=NULL;
		delete_node(old_left);
		delete_node(old_right);
		return 0;
	}
	return 0;
}

int priority(struct Token token){

	if(token.type==operator){
		if(token.operator=='+' || token.operator=='-'){
			return 0;
		}
		if(token.operator=='*' || token.operator=='/'){
			return 1;
		}
		if(token.operator=='^'){
			return 2;
		}
	}
	if (token.type==bracket){
		return 3;
	}
	return 0;
}
void draw_expr(struct Node* node, int open_brackets, int closed_brackets){
	if(node->data->type!=operator){
		for(int i=0;i<open_brackets;i++)
			printf("(");
		enum stored_type type=node->data->type;
		switch(type){
			case variable:
				printf("%s", node->data->var_name->data);
				break;
			case number:
				if(node->data->value<0){
					printf("(%d)", node->data->value);
				}
				else{
					printf("%d", node->data->value);
				}
				
				break;
		}
		for(int i=0;i<closed_brackets;i++)
			printf(")");
		return;
	}
	if(node->data->operator=='+'){
		draw_expr(node->left, open_brackets, 0);
		printf("%c", node->data->operator);
		draw_expr(node->right, 0, closed_brackets);
		return;
		
	}
	if(node->left->data->type==operator){
		draw_expr(node->left, open_brackets+1, 0);
		printf(")");
	}
	else{
		draw_expr(node->left, open_brackets, 0);
	}
	printf("%c", node->data->operator);
	if(node->right->data->type==operator){		
		printf("(");
		draw_expr(node->right, 0, closed_brackets+1);
	}
	else{
		draw_expr(node->right, 0, closed_brackets);
	}

	return;
	
}
void set_levels(struct Token_list* tokens){
	int level=0;
	int count=0;
	struct Item* item=tokens->begin;
	while(item!=NULL){
		if(item->data.type==variable || item->data.type==number || (item->data.type==bracket && item->data.bracket=='(')){
			level++;
		}
		else{
			level--;
		}
		if(item->data.type!=bracket){
			item->data.level=level;
		}
		item=item->next;
	}
}
void remove_brackets(struct Token_list* list){
	struct Item* item=list->begin;
	struct Item* tmp;
	while(item!=NULL){
		tmp=item->next;
		if(item->data.type==bracket)
			delete_item(list, item);
		item=tmp;
	}
}

struct Node* build(struct Item* begin, struct Item* end){

	int min=begin->data.level;
	struct Item* index=begin;
	struct Item* i=begin;
	int stop=0;
	while(stop!=1){
		if(i==end){
			stop=1;
		}
		if(i->data.level<min){
			min=i->data.level;
			index=i;
			i=i->next;
			continue;
		}
		if(i->data.level==min){
			int pr_old=priority(index->data);
			int pr_new=priority(i->data);
			if(pr_new<pr_old)
				index=i;
			else{
				if((index->data.type==operator && i->data.type==operator) && (pr_old==pr_new && i->data.operator!='^'))
					index=i;
			}
		}
		i=i->next;
	}
	struct Node* node=create_node();
	node->data=&index->data;
	if(begin!=end){
		if(index->prev!=NULL)
			node->left=build(begin, index->prev);
		node->right=build(index->next, end);		
	}
	return node;

}
int main(){
	int running=1;
	char symbol;
	struct Char_vector* expression=create_char_vector(0);
	struct Token_list* tokens=create_token_list();
	int count=0;
	while(running){
		scanf("%c", &symbol);
		while(symbol!='\n'){
			push_char(expression, symbol);
			scanf("%c", &symbol);
		}
		if(strcmp(expression->data,"stop")==0 || strcmp(expression->data,"q")==0){
			running=0;
			continue;
		}
		count=parse(expression, tokens);
		if(count!=-1){
			set_levels(tokens);
			remove_brackets(tokens);
			struct Node* tree=build(tokens->begin, tokens->end);
			calculate(tree);
			remove_zeros(tree);
			printf("result:\n");
			draw_expr(tree,0,0);
			printf("\n");
			clear_char_vector(expression);
			clear_token_list(tokens);
			delete_node(tree);
		}
		else{
			printf("Failed to parse expression, try again\n");
		}
	
		
	}
	delete_char_vector(expression);
	delete_token_list(tokens);
}
