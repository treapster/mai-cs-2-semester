#include<stdio.h>
int main()
{
  int i,j,max, min,  answer=0, minstring, maxcolumn;
  int a[8][8];
  int n;
  scanf("%d", &n);
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      scanf("%d", &a[i][j]);
  max=a[0][0];
  min=a[0][0];
  maxcolumn=0;
  minstring=0;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      {
	if(a[i][j]>max)
	  {
	    max=a[i][j];
	    maxcolumn=j;
	  }
	if(a[i][j]<min)
	  {
	    min=a[i][j];
	    minstring=i;
	  }
      }
  for(i=0;i<n;i++)
    answer+=a[minstring][i]*a[i][maxcolumn];
  printf("%d\n", answer);
}
