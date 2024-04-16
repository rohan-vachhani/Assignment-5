#include <stdbool.h>
#include "heap.h"

typedef struct node
{
    int data;
    bool mark;
    int referenceCount;
    struct node *next_1;
    struct node *next_2;
    struct node *next_3;
} Node;

 
Node *array[8];
int val[]={1, 2, 3, 5, 7, 8, 9, 10};


void print_node(int i);
void edgeSet(int so,int dest1,int dest2,int dest3);
void print_allNodes(Node* root);
void adjacency_list();
bool free_ref_count(int);
void reference_counting();
void adjacency_Matrix();
void mark_method(Node* root);
void sweep_method();


int main()
{	
	int i;
	for(i=0;i<8;i++)
	{
		Node* new_node =(Node*) alloc(sizeof(Node));
		new_node->data=val[i];
		new_node->next_1=NULL;
		new_node->next_2=NULL;
		new_node->next_3=NULL;
		new_node->referenceCount=0;
		new_node->mark=false;		
		array[i]=new_node;
	}
	
	Node*root_1=array[3];
	array[3]->referenceCount+=1;
	Node*root_2=array[0];
	array[0]->referenceCount+=1;
	
	edgeSet(0,1,6,7);
	edgeSet(2,5,7,-1);
	edgeSet(3,0,-1,-1);
	edgeSet(4,0,5,-1);
	edgeSet(5,6,2,-1);
	
	printf("\nAll nodes through Root-1:\n");
	print_allNodes(root_1);
		
	printf("\nAll nodes through Root-2:\n");
	print_allNodes(root_2);

	printf("\nAdjacency list :\n");
	adjacency_list();

	printf("\nAdjacency matrix:\n");
	adjacency_Matrix();

	int inp;
	printf("\nSelect garbage collection method:\n1. Mark Sweep\n2. Reference Counting\n> ");
	scanf("%d", &inp);

	switch (inp) {
		case 1:
			printf("\nCalling the mark and sweep garbage collector\n");
			mark_method(root_1);
			mark_method(root_2);
			sweep_method();
			break;

		case 2:
			printf("\nCalling reference counting garbage collector\n");
			reference_counting();
			break;
	}

	printf("\nAdjacency list after removal of garbage:\n");
	adjacency_list();
	
	printf("\nAdjacency matrix after removal of garbage:\n");
	adjacency_Matrix();

    return 0;
}



void print_node(int i)
{
	printf("value=%d reference_count=%d freed_size=%d\n",array[i]->data,array[i]->referenceCount,sizeof(Node));
}


void edgeSet(int so,int dest1,int dest2,int dest3)
{
	if(dest1!=-1)
	{
		array[so]->next_1=array[dest1];
		array[dest1]->referenceCount+=1;
	}
	if(dest2!=-1)
	{
		array[so]->next_2=array[dest2];
		array[dest2]->referenceCount+=1;
	}
	if(dest3!=-1)
	{
		array[so]->next_3=array[dest3];
		array[dest3]->referenceCount+=1;
	}
	
}


void print_allNodes(Node* root)
{
	if(root!=NULL)
	{
		printf("value=%d : referenceCount=%d\n",root->data,root->referenceCount);
	}
	if(root==NULL)
	{
		return;
	}
	print_allNodes(root->next_1);
	print_allNodes(root->next_2);
	print_allNodes(root->next_3);
}


void adjacency_list()
{
	int i=0;
	for(i=0;i<8;i++)
	{
		if(array[i]!=NULL)
		{
			printf("%d: ",array[i]->data);
			if(array[i]->next_1!=NULL)
			{
				printf("%d -> ",array[i]->next_1->data);
			}
			if(array[i]->next_2!=NULL)
			{
				printf("%d -> ",array[i]->next_2->data);
			}
			if(array[i]->next_3!=NULL)
			{
				printf("%d -> ",array[i]->next_3->data);
			}
			printf("NULL\n");
		}
	}
}


bool free_ref_count(int i) {
	bool child0 = false;
	if (array[i] -> referenceCount == 0) {
		if(array[i]->next_1!=NULL)
		{
			array[i]->next_1->referenceCount-=1;
			if (array[i]->next_1->referenceCount == 0) child0 = true;
		}
		if(array[i]->next_2!=NULL)
		{
			array[i]->next_2->referenceCount-=1;
			if (array[i]->next_2->referenceCount == 0) child0 = true;
		}
		if(array[i]->next_3!=NULL)
		{
			array[i]->next_3->referenceCount-=1;
			if (array[i]->next_3->referenceCount == 0) child0 = true;
		}
		printf("Garbage: ");
		print_node(i);
		free(array[i]);
		array[i]=NULL;
	}
	return child0;
}


void reference_counting()
{
	bool done = true;
	while (done) {
		for (int i = 0; i < 8 && done; i++) {
			if (array[i]) {
				done = !free_ref_count(i);
			}
		}
		done = !done;
	} 
}


void adjacency_Matrix()
{
	int adm[8][8];
	int i,j;
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			adm[i][j]=0;
		}	
	}
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			
		if(array[j]!=NULL&&array[i]!=NULL)
		{
			
			if(array[i]->next_1!=NULL)
			{
				if(array[i]->next_1->data==array[j]->data&&i!=j)
				{
					adm[i][j]=1;
				}
			}
			if(array[i]->next_2!=NULL)
			{
				if(array[i]->next_2->data==array[j]->data&&i!=j)
				{
					adm[i][j]=1;
				}
			}
			if(array[i]->next_3!=NULL)
			{
				if(array[i]->next_3->data==array[j]->data&&i!=j)
				{
					adm[i][j]=1;
				}
			}
		}
		
		}
	}
	
	printf("\t");

	for (i = 0; i < 8; i++) {
		printf("%d ", val[i]);
	}
	printf("\n\n");

	for(i=0;i<8;i++)
	{
		printf("%d\t", val[i]);
		for(j=0;j<8;j++)
		{
			printf("%d ",adm[i][j]);
		}
		printf("\n");
	}
}


void mark_method(Node* root)
{
	if(root!=NULL && root->mark != true)
	{
		root->mark=true;
		mark_method(root->next_1);
		mark_method(root->next_2);
		mark_method(root->next_3);
	}
}


void sweep_method()
{
	int i;
	for(i=0;i<8;i++)
	{
		if(array[i]->mark==false)
		{
			if(array[i]->next_1!=NULL)
			{
				array[i]->next_1->referenceCount-=1;
			}
			if(array[i]->next_2!=NULL)
			{
				array[i]->next_2->referenceCount-=1;
			}
			if(array[i]->next_3!=NULL)
			{
				array[i]->next_3->referenceCount-=1;
			}
			printf("Garbage: ");
			print_node(i);
			free(array[i]);
			array[i]=NULL;
		}
		else {
			array[i]->mark = false;
		}
	}
}