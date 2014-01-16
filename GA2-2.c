#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
typedef struct
{
double* std;//standard of each robber..group size 
int* his;//history of two previous games..group size 
int* position;//allocate a position in the robber team for each robber for random partener combination..
int* AL;//4 digits to express the integer contain the history of previous two game..size is 4..
int* BL;// same as AL size is also 4..
} GRP;

GRP* alloc_GA(int n)// give all the initial values a random 0 or 1..
{
GRP* p=malloc(sizeof(GRP));
int i,h;
p->std = malloc(n*sizeof(double));
p->his = malloc(n*sizeof(int));
p->position = malloc(n*sizeof(int));
p->AL = malloc(4*sizeof(int));
p->BL = malloc(4*sizeof(int));

srand48(time(NULL));
for(i=0;i<n;i++)
  {
  h = lrand48()%16;
  p->std[i] = 0.5;
  p->his[i] = h;
  p->position[i] = i;
    if(i<=3)
    {
    p->AL[i] = 0;
    p->BL[i] = 0;
    }
  }
return p;
}
/*
void fittness_GA(GRP* p,int i)
{
check_GA(p,i);
}*/

int year_GA(int a, int b)
{//1 is confess, 0 is not..
//a is the behave of himself, b is his robbery partener..
int year;
if(a==0 && b==0)
  {year = 2;}//1
else if(a==0 && b==1)
  {year = 5;}//5
else if(a==1 && b==0)
  {year = 5;}//0
else
  {year = 6;}//3
return year;
}

void sorting_GA(GRP *p,int n)
{
int i,a,b,tmp;
  for(i=0;i<n*2;i++)
  {
  a=lrand48()%n;
  b=lrand48()%n;
  tmp = p->position[a];
  p->position[a] = p->position[b];
  p->position[b] = tmp;
  }
}

void check_GA(GRP* p,int i)
{
switch(i)
  {
  case 0: p->AL[0]=0, p->AL[1]=0, p->AL[2]=0, p->AL[3]=0; break;
  case 1: p->AL[0]=0, p->AL[1]=0, p->AL[2]=0, p->AL[3]=1; break;
  case 2: p->AL[0]=0, p->AL[1]=0, p->AL[2]=1, p->AL[3]=0; break;
  case 3: p->AL[0]=0, p->AL[1]=0, p->AL[2]=1, p->AL[3]=1; break;
  case 4: p->AL[0]=0, p->AL[1]=1, p->AL[2]=0, p->AL[3]=0; break;
  case 5: p->AL[0]=0, p->AL[1]=1, p->AL[2]=0, p->AL[3]=1; break;
  case 6: p->AL[0]=0, p->AL[1]=1, p->AL[2]=1, p->AL[3]=0; break;
  case 7: p->AL[0]=0, p->AL[1]=1, p->AL[2]=1, p->AL[3]=1; break;
  case  8: p->AL[0]=1, p->AL[1]=0, p->AL[2]=0, p->AL[3]=0; break;
  case  9: p->AL[0]=1, p->AL[1]=0, p->AL[2]=0, p->AL[3]=1; break;
  case 10: p->AL[0]=1, p->AL[1]=0, p->AL[2]=1, p->AL[3]=0; break;
  case 11: p->AL[0]=1, p->AL[1]=0, p->AL[2]=1, p->AL[3]=1; break;
  case 12: p->AL[0]=1, p->AL[1]=1, p->AL[2]=0, p->AL[3]=0; break;
  case 13: p->AL[0]=1, p->AL[1]=1, p->AL[2]=0, p->AL[3]=1; break;
  case 14: p->AL[0]=1, p->AL[1]=1, p->AL[2]=1, p->AL[3]=0; break;
  case 15: p->AL[0]=1, p->AL[1]=1, p->AL[2]=1, p->AL[3]=1; break;
  }
}

int rtrn_GA(int a,int b,int c,int d)
{
int y;
y=a*8 + b*4 + c*2 + d;
return y;
}

void decide_GA(GRP* p, double ra, double rb, int A,int B)
{//this part seems confusing, but very effiency..
int xa,xb;
xa = p->his[A]%4;
xb = p->his[B]%4;
//1 is confess, 0 is not
//smaller than std is confess, larger than std is not
if(ra>= p->std[A] && rb>= p->std[B])//A,B not confess, both get new 00
  {
  p->his[A] = xa*4;
  p->his[B] = xb*4;
  }
else if(ra>= p->std[A] && rb< p->std[B])//A not, B confess, A get new 01, B get new 10
  {
  p->his[A] = xa*4+1;
  p->his[B] = xb*4+2;
  }
else if(ra< p->std[A] && rb>= p->std[B])//A confess, B not, A get new 10, B get new 01
  {
  p->his[A] = xa*4+2;
  p->his[B] = xb*4+1;
  }
else //(ra< p->std[A] && rb< p->std[B]) means A confess, B confess, get new 11
  {
  p->his[A] = xa*4+3;
  p->his[B] = xb*4+3;
  }
}


void cross_GA(GRP* p,int a,int b,int son,double R,int mut_p, double mut_r)//a and b exchange a part of genes
{//c is the number of the son from a and b, 2 digits from A and 2 from B..
//
int tmp0,tmp1;
check_GA(p,p->his[b]);//get four binary digits of b
p->BL[0] = p->AL[0];
p->BL[1] = p->AL[1];
p->BL[2] = p->AL[2];
p->BL[3] = p->AL[3];
check_GA(p,p->his[a]);

if(R>=0.5)//A and B change first 2 digits
  {
  tmp0 = p->BL[0];
  tmp1 = p->BL[1];
  p->BL[0] = p->AL[0];
  p->BL[1] = p->AL[1];
  p->AL[0] = tmp0;
  p->AL[1] = tmp1;
  }
else//A and B change last 2 digits
  {
  tmp0 = p->BL[2];
  tmp1 = p->BL[3];
  p->BL[2] = p->AL[2];
  p->BL[3] = p->AL[3];
  p->AL[2] = tmp0;
  p->AL[3] = tmp1;
  }
  
p->his[a] = rtrn_GA(p->AL[0],p->AL[1],p->AL[2],p->AL[3]);
p->his[b] = rtrn_GA(p->BL[0],p->BL[1],p->BL[2],p->BL[3]);

if(R<mut_r)//judge if mutation will happen..
  {
  if(p->AL[mut_p]==1)
    {
    p->AL[mut_p]=0;
    }
  else
    {
    p->AL[mut_p]=1;
    }
  printf("Mutation happened at position %d.\n",mut_p);
  }
  
p->his[son] = rtrn_GA(p->AL[0],p->AL[1],p->AL[2],p->AL[3]);
}

/*void print_GA(GRP* p, int n)// print all the values out..
{
int i,j;
printf("The group is:\n");
for(i=0;i<n;i++)
  {
  p->cnt[i]=count_GA(p,i);
  printf("counting result of No.%d is %d, genes:",i,p->cnt[i]);
  for(j=0;j<p->len;j++)
    {
    printf(" %d",p->grp[i][j]);
    }
    printf(";\n");
  }
}*/

void free_GA(GRP* p)//doing memory free..
{
free(p->std);
free(p->his);
free(p->position);
free(p->AL);
free(p->BL);
free(p);
}

int main(int argc, char *argv[])
{
GRP* p;
int i,j,itr,g,n,A,B,son,mut_p,year1,year2;
double cross_r, mut_r,R,ra=0.5,rb=0.5;
/*-----------------checking the input values----------------*/
if( argv[6]!= NULL || argv[1]==NULL || argv[2]==NULL || argv[3]==NULL || argv[4]==NULL || argv[5]==NULL)
  {
  printf("Please input command in this way:\nGA 10(population size) 10(number of generations) ");
  printf("100(number of iterations) 0.95(crossover rate) 0.1(mutation rate)\n");
  exit(0);
  }
n = atoi(argv[1]);
g = atoi(argv[2]);
itr = atoi(argv[3]);
cross_r = atof(argv[4]);
mut_r = atof(argv[5]);
if (cross_r>1.0 || mut_r>1.0 || mut_r>cross_r)
  {
  printf("Crossover rate must larger than mutation rate and both not larger than 1.\n");
  exit(0);
  }
/*-----------------checking the input values----------------*/

p = alloc_GA(n);
srand48(time(NULL));

for(i=0;i<itr;i++)
  {
  printf("In iteration %d:\n",i+1);
  sorting_GA(p,n);

  for(j=0;j<n/2;j++)//each of robber should face PD once in one iteration..
    {
    A = p->position[2*j];
    B = p->position[2*j+1];
    ra = drand48();
    rb = drand48();
    
    decide_GA(p,ra,rb,A,B);
    }
  
  for(j=0;j<g;j++)//g generations happened in one iteration..
    {
    A = lrand48()%n;
    B = lrand48()%n;
    while(B==A)
    {B = lrand48()%n;}
    
    R = drand48();//for judgement of crossover and mutation..
    
    if(R<cross_r)//judge if crossover will happen..
      {
      son = lrand48()%n;
      mut_p = lrand48()%4;// this value tells the position of mutation happen..
      cross_GA(p,A,B,son,R,mut_p,mut_r);
      }
    else printf("No crossover and mutation happened.\n");
    }
    
  for(j=0;j<n;j++)//fittness function..
    {
    check_GA(p,p->his[j]);
    year1 = year_GA(p->AL[0],p->AL[1]);
    year2 = year_GA(p->AL[2],p->AL[3]);
    if	   (year1>year2 && p->AL[0]==0)
      {
      p->std[j] = p->std[j];
      }
    else if(year1>year2 && p->AL[0]==1)
      {
      p->std[j] = p->std[j];
      }
    else if(year1<year2 && p->AL[2]==0)
      {
      p->std[j] = p->std[j];
      }
    else if(year1<year2 && p->AL[2]==1)
      {
      p->std[j] = p->std[j];
      }
    }
  }

free_GA(p);

return 0;
/*
  {
  for(j=0;j<n;j++)
    {
    if( count_GA(p,j) <= std )
      {
      flag = 1;
      }
    }
  if(flag==0)// when all the guys in the group hold a count value larger than standard, standard+1..
  {std = std + 1;}
*/
}
