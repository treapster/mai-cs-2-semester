struct Token_vector
{
	struct Token* data;
	int capacity;
	int size;
};

struct Token_vector* create_token_vector(int n)
{
	if(n>0)
	{
		struct Token_vector* vector=(struct Token_vector*)malloc(sizeof(struct Token_vector));
		vector->data=(struct Token*)malloc(sizeof(struct Token)*2*n);
		vector->capacity=2*n;
		vector->size=n;
		return vector;
	}
	else
	{
		n=1;
		struct Token_vector* vector=(struct Token_vector*)malloc(sizeof(struct Token_vector));
		vector->data=(struct Token*)malloc(sizeof(struct Token)*2*n);
		vector->capacity=2*n;
		vector->size=0;
		return vector;
	}
	
}
void push_token(struct Token_vector* vector, struct Token token)
{
	if(vector->size<vector->capacity-1)
	{
		vector->data[vector->size]=token;
		vector->size++;

	}
	else
	{
		vector->data=(struct Token*)realloc(vector->data,vector->capacity*2*sizeof(struct Token));
		vector->capacity*=2;
		vector->data[vector->size]=token;
		vector->size++;
		//fill_with_zeros(&(vector->data[vector->size]), vector->capacity/2);
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
	if(vector->data[vector->size-1].type==variable)
	{
		delete_char_vector(vector->data[vector->size-1].var_name);
	}
	vector->size--;
	if(vector->size<(vector->capacity)/4)
	{
		vector->data=(struct Token*)realloc(vector->data,sizeof(struct Token)*(vector->capacity/2));
		vector->capacity/=2;
	}
}
void clear_token_vector(struct Token_vector* vector)
{
	for(int i=0;i<vector->size;i++)
	{
		if(vector->data[i].type==variable)
		{
			delete_char_vector(vector->data[i].var_name);
		}
	}
	free(vector->data);
	vector->data=(struct Token*)malloc(sizeof(struct Token)*2);
	vector->size=0;
	vector->capacity=2;
}
void delete_token_vector(struct Token_vector* vector)
{
	for(int i=0;i<vector->size;i++)
	{
		if(vector->data[i].type==variable)
		{
			delete_char_vector(vector->data[i].var_name);
		}
	}
	free(vector->data);
	vector->size=0;
	vector->capacity=0;
	free(vector);
}