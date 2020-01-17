#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<climits>
#include "Bplustree.h"
int No_of_tree=0;
int MaxBPTree;
struct linkedlist {
    BPTree root;
    struct linkedlist *next;
} *head = NULL;

void insertintolinkedlist() {
   linkedlist* newLink = (struct linkedlist*) malloc(sizeof(struct linkedlist));
   linkedlist* ptr=head;
   if(head==NULL)
   {
     newLink->next = NULL;
     newLink->root.sizeofBPTree=0;
     newLink->root.root=NULL;
     newLink->root.MaxsizeofBPTree=MaxBPTree;
     head=newLink;
   }
   else
   {
     while(!(ptr->next==NULL))
     {
       ptr=ptr->next;
     }
     newLink->next = NULL;
     newLink->root.sizeofBPTree=0;
     newLink->root.root=NULL;
     newLink->root.MaxsizeofBPTree=2*(ptr->root.MaxsizeofBPTree);
     ptr->next = newLink;
   }
   ++No_of_tree;

}
void display() {
   struct linkedlist* ptr;
   ptr = head;
   while (ptr != NULL) {
    ptr->root.display(ptr->root.getRoot());
    ptr = ptr->next;
    cout<<"-------------------\n";
   }
}
void insert_in_BPtree();
void make();
void merge(linkedlist* ptr);
void mmerge(int arr[],int arr1[], int arr2[], int n1, int n2);
int main()
{
	string command;
	int x,m;
	bool close = false;
  insertintolinkedlist();
  cout<<"Enter the max degree\n";
	cin>>command;
	stringstream max(command);
	max>>MAX;
	cin.clear();
	cin.ignore(1);

  cout<<"Enter Max size of primary tree\n";
  cin>>MaxBPTree;
  head->root.MaxsizeofBPTree=MaxBPTree;
  cout<<"Enter '1' to Insert \nEnter '2' to Display\nEnter '3' to Exit\n";
  int end_loop=1;
  while(end_loop)
  {
    cout<<"->";
    cin>>x;
    switch (x)
    {
      case 1:
      {
        insert_in_BPtree();
        break;
      }
      case 2:
      {
        display();
        break;
      }
      case 3:
      {
        end_loop=0;
        break;
      }
      default:
      {
        cout<<"Wrong option\nEnter '1' to Insert \nEnter '2' to Display\nEnter '3' to Exit\n";
        break;
      }
    }

  }
}
void insert_in_BPtree()
{
  int m;
  if(head==NULL)
  insertintolinkedlist();
  bool full=true;
  linkedlist *p;
  p=head;
  while (p!=NULL) {
    if(p->root.sizeofBPTree<p->root.MaxsizeofBPTree)
    {full=false;break;}
    p=p->next;
  }
  if(full)
  {
    linkedlist* Link1 = (struct linkedlist*) malloc(sizeof(struct linkedlist));
    Link1->next = head;
    Link1->root.sizeofBPTree=0;
    Link1->root.root=NULL;
    head=Link1;
    make();

  }
  if(head->root.sizeofBPTree>=head->root.MaxsizeofBPTree)
  {
    linkedlist *ptr=head;
    if(ptr->next==NULL || ptr->next->root.getRoot()==NULL)
    {
        insertintolinkedlist();
        linkedlist *temp,*temp2;
        cout<<"Shifting the B+ tree one step lower\n";
        temp=ptr->next->next;
        temp2=ptr->next;
        ptr->next->next=ptr;
        ptr->next=temp;
        ptr=temp2;
        temp=NULL;
        if(ptr->next==head)
        {
          head=ptr;
        }
    }
    else
    {
      while(ptr->next!=NULL && ptr->next->root.getRoot()!=NULL)
        {
          merge(ptr);
          ptr=ptr->next;
        }

    }
  }
  cout<<"Insert: ";
  cin>>m;
  head->root.insert(m);
  make();
}
void merge(linkedlist* ptr)
{
  linkedlist* Link1 = (struct linkedlist*) malloc(sizeof(struct linkedlist));
  Link1->next = NULL;
  Link1->root.sizeofBPTree=0;
  Link1->root.root=NULL;
  Link1->root.MaxsizeofBPTree=ptr->root.MaxsizeofBPTree;

  linkedlist* Link2 = (struct linkedlist*) malloc(sizeof(struct linkedlist));
  Link2->next = NULL;
  Link2->root.sizeofBPTree=0;
  Link2->root.root=NULL;
  Link2->root.MaxsizeofBPTree=ptr->next->root.MaxsizeofBPTree;

  ptr->root.check=0;
  int test1=ptr->root.sizeofBPTree;
  ptr->root.checked(ptr->root.getRoot());

  ptr->next->root.check=0;
  int test2=ptr->next->root.sizeofBPTree;
  ptr->next->root.checked(ptr->next->root.getRoot());

  int temper[test1+test2];
  mmerge(temper,ptr->root.A,ptr->next->root.A,test1,test2);
  for(int i=0;i<test1+test2;++i)
  {
    Link2->root.insert(temper[i]);
  }
  Link2->next=ptr->next->next;
  Link1->next=Link2;
  head=Link1;
  free(ptr->next);
  free(ptr);
}
void mmerge(int arr[],int arr1[], int arr2[], int n1, int n2)
{
  int i, j, k;
  int L[n1], R[n2];
  for (i = 0; i < n1; i++)
    L[i] = arr1[i];
  for (j = 0; j < n2; j++)
    R[j] = arr2[j];
  i = 0;
  j = 0;
  k = 0;
  while (i < n1 && j < n2)
  {
    if (L[i] <= R[j])
    {
      arr[k] = L[i];
      i++;
    }
    else
    {
      arr[k] = R[j];
      j++;
    }
    k++;
  }
  while (i < n1)
  {
    arr[k] = L[i];
    i++;
    k++;
  }
  while (j < n2)
  {
    arr[k] = R[j];
    j++;
    k++;
  }
}

void make()
{
linkedlist *ptr;
ptr=head;
ptr->root.MaxsizeofBPTree=MaxBPTree;
while(ptr->next!=NULL)
  {
  ptr->next->root.MaxsizeofBPTree=2*ptr->root.MaxsizeofBPTree;
  ptr=ptr->next;
  }
}
