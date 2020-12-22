#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
enum stored_type
{
	variable, number, operator, bracket, unknown
};
enum stored_type type_of_char(char symbol)
{
	if((symbol>='a' && symbol <='z') || (symbol>='A' && symbol<='Z'))
	{
		return variable;
	}
	if(symbol>='0' && symbol<='9')
	{
		return number;
	}
	if(symbol=='*' || symbol=='/' || symbol =='+' || symbol=='-' || symbol=='^')
	{
		return operator;
	}
	if(symbol=='(' || symbol==')')
	{
		return bracket;
	}
	return unknown;
}
struct Char_vector
{
	char* data;
	int capacity;
	int occupied;
};
struct Char_vector* create_char_vector(int n)
{
	if(n>0)
	{
		struct Char_vector* vector=(struct Char_vector*)malloc(sizeof(struct Char_vector));
		vector->data=(char*)malloc(sizeof(char)*2*n);
		vector->capacity=2*n;
		vector->occupied=n;
		return vector;
	}
	else
	{
		n=1;
		struct Char_vector* vector=(struct Char_vector*)malloc(sizeof(struct Char_vector));
		vector->data=(char*)malloc(sizeof(char)*2*n);
		vector->capacity=2*n;
		vector->occupied=0;
		return vector;
	}
	
}
void push_char(struct Char_vector* vector, char symbol)
{
	if(vector->occupied<vector->capacity-1)
	{
		vector->data[vector->occupied]=symbol;
		vector->data[vector->occupied+1]=0;
		vector->occupied++;
	}
	else
	{
		vector->data=(char*)realloc(vector->data,vector->capacity*2*sizeof(char));
		vector->capacity*=2;
		vector->data[vector->occupied]=symbol;
		vector->occupied++;
	}

}
char get_char(struct Char_vector* vector, int position)
{
	return vector->data[position];
}
void assign_char(struct Char_vector* vector, int position, char symbol)
{
	vector->data[position]=symbol;
}
void pop_char(struct Char_vector* vector)
{
	vector->data[vector->occupied]=0;
	vector->occupied--;
	if(vector->occupied<(vector->capacity)/4)
	{
		vector->data=(char*)realloc(vector->data,sizeof(char)*(vector->capacity/2));
		vector->capacity/=2;
	}
}
void delete_char_vector(struct Char_vector* vector)
{
	free(vector->data);
	vector->occupied=0;
	vector->capacity=0;
	free(vector);
}
struct Token
{
enum stored_type type;
union
{
	char var_name[30];
	int value;
	char operator;
	char bracket;

};
};
void fill_with_zeros(struct Token* tokens, int size)
{
	for(int i=0;i<size;i++)
	{
		tokens[i].type=unknown;
		tokens[i].value=0;
	}
}
/*
struct Token_vector
{
	struct Token* data;
	int capacity;
	int occupied;
};

struct Token_vector* create_token_vector(int n)
{
	if(n>0)
	{
		struct Token_vector* vector=(struct Token_vector*)malloc(sizeof(struct Token_vector));
		vector->data=(struct Token*)malloc(sizeof(struct Token)*2*n);
		fill_with_zeros(vector->data, 2*n);
		vector->capacity=2*n;
		vector->occupied=n;
		return vector;
	}
	else
	{
		n=1;
		struct Token_vector* vector=(struct Token_vector*)malloc(sizeof(struct Token_vector));
		vector->data=(struct Token*)malloc(sizeof(struct Token)*2*n);
		fill_with_zeros(vector->data, 2*n);
		vector->capacity=2*n;
		vector->occupied=0;
		return vector;
	}
	
}
void push_token(struct Token_vector* vector, struct Token token)
{
	if(vector->occupied<vector->capacity-1)
	{
		vector->data[vector->occupied]=token;
		vector->occupied++;

	}
	else
	{
		vector->data=(struct Token*)realloc(vector->data,vector->capacity*2*sizeof(struct Token));
		vector->capacity*=2;
		vector->data[vector->occupied]=token;
		vector->occupied++;
		fill_with_zeros(&vector->data[vector->occupied], vector->capacity/2);
	}

}
struct Token get_token(struct Token_vector* vector, int position)
{
	return vector->data[position];
}
void assign_token(struct Token_vector* vector, int position, struct Token token)
{
	vector->data[position]=token;
}
void pop_token(struct Token_vector* vector)
{
	vector->occupied--;
	if(vector->occupied<(vector->capacity)/4)
	{
		vector->data=(struct Token*)realloc(vector->data,sizeof(struct Token)*(vector->capacity/2));
		vector->capacity/=2;
	}
}
void delete_token_vector(struct Token_vector* vector)
{
	free(vector->data);
	vector->occupied=0;
	vector->capacity=0;
	free(vector);
}
*/
int parse(char* expression, struct Token* tokens)
{
	int i=0;
	int token_count=0;
	int letter_count=0;
	char symbol;
	enum stored_type type;
	while(expression[i]!=0)
	{
		symbol=expression[i];
		type=type_of_char(symbol);
		if(type!=unknown)
		{
			if(tokens[token_count].type==unknown)
			{
				tokens[token_count].type=type;
				letter_count=0;
			}
			if(tokens[token_count].type!=type)
			{
				token_count++;
				letter_count=0;
				continue;
			}
			switch(type)
			{
				case variable:
					tokens[token_count].var_name[letter_count]=symbol;
					tokens[token_count].var_name[letter_count+1]=0;
					letter_count++;
					break;
				case number:
					tokens[token_count].value*=10;
					tokens[token_count].value+=symbol-'0';
					break;
				case operator:
					tokens[token_count].operator=symbol;
					token_count++;
					break;
				case bracket:
					tokens[token_count].bracket=symbol;
					token_count++;
					break;
			}
			i++;
			
		}
		else
		{
			printf("%c is unknown symbol\n", symbol);
			return 0;
		}
	}
	return ++token_count;
}
void print(struct Token* tokens, int count)
{
	enum stored_type type;
	for(int i=0; i<count;i++)
	{
		type=tokens[i].type;
		switch(type)
			{
				case variable:
					printf("%s is a variable name\n", tokens[i].var_name);
					break;
				case number:
					printf("%d is a number\n", tokens[i].value);
					break;
				case operator:
					printf("%c is an operator\n", tokens[i].operator);
					break;
				case bracket:
					printf("%c is a bracket\n", tokens[i].bracket);
					break;
			}
	}
}

int main()
{
	int running=1;
	char symbol;
	int max_expr=256;
	int max_tokens=256;
	char expression[max_expr];
	struct Token tokens[max_tokens];
	int count=0;
	fill_with_zeros(tokens, max_tokens);
	while(running)
	{
		scanf("%s", expression);	
		if(strcmp(expression,"stop")==0 || strcmp(expression,"q")==0)
		{
			running=0;
			continue;
		}
		count=parse(expression, tokens);
		print(tokens, count);
		fill_with_zeros(tokens, max_tokens);
	}
}
