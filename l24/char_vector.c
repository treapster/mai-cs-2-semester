struct Char_vector
{
	char* data;
	int capacity;
	int size;
};
struct Char_vector* create_char_vector(int n)
{
	struct Char_vector* vector;
	if(n>0)
	{
		vector=(struct Char_vector*)malloc(sizeof(struct Char_vector));
		vector->data=(char*)malloc(sizeof(char)*2*n);
		vector->capacity=2*n;
		vector->size=n;
	}
	else
	{
		n=1;
		vector=(struct Char_vector*)malloc(sizeof(struct Char_vector));
		vector->data=(char*)malloc(sizeof(char)*2*n);
		vector->capacity=2*n;
		vector->size=0;
	}
	for(int i=0;i<2*n;i++)
	{
		vector->data[i]=0;
	}
	return vector;
	
}
void push_char(struct Char_vector* vector, char symbol)
{
	if(vector->size<vector->capacity-1)
	{
		vector->data[vector->size]=symbol;
		vector->data[vector->size+1]=0;
		vector->size++;
	}
	else
	{
		vector->data=(char*)realloc(vector->data,vector->capacity*2*sizeof(char));
		vector->capacity*=2;
		vector->data[vector->size]=symbol;
		vector->data[vector->size+1]=0;
		vector->size++;
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
	vector->data[vector->size]=0;
	vector->size--;
	if(vector->size<(vector->capacity)/4)
	{
		vector->data=(char*)realloc(vector->data,sizeof(char)*(vector->capacity/2));
		vector->capacity/=2;
	}
}
void clear_char_vector(struct Char_vector* vector)
{
	free(vector->data);
	vector->data=(char*)malloc(sizeof(char)*2);
	vector->size=0;
	vector->capacity=2;
}
void delete_char_vector(struct Char_vector* vector)
{
	free(vector->data);
	vector->size=0;
	vector->capacity=0;
	vector->data=NULL;
	free(vector);
}
