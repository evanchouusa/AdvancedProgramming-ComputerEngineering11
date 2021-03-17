#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#define maxsize 10

//structure array
typedef struct waitlistperson
{
	char names[20];
	int groupsize;
	struct waitlistperson *next;
} RESSLOT;

//declare functions
void add(void);
int checkduplicates(char *str);
void delete(void);
void list(void);
void checksizes(void);

RESSLOT *head = NULL;
RESSLOT *tail = NULL;

int main(void)
{
	//initialize
	char input;

	printf("\nType 1 to add your name and number of people, type 2 to remove, type 3 to list, type 4 to check sizes, and type 0 to quit program\n"); //directions to waitlisting system

	//main loop
	while(1)
	{
		printf("\nEnter a command: \n"); //entering the correct commands
		scanf(" %c",&input);

		//switch statement for the 4 specific commands
		switch(input)
		{
			case '1':
				add();
				break;
			case '2':
				delete();
				break;
			case '3':
				list();
				break;
			case '4':
				checksizes(); //List group smaller than a certain size
				break;
			case '0':
				printf("\nQuitting...\n");
				return 0;
			default:
				printf("\nPlease enter one of the correct commands\n");
		}
	}
}

//function will add party into restaurant waitlist and reservation
void add(void)
{
	RESSLOT *p;
	if((p=(RESSLOT *)malloc(sizeof(RESSLOT))) ==  (RESSLOT *) NULL)
	{
		printf("\nNo reservation available\n"); //Memory cannot be allocated
		return;
	}

	//initialize
	char name [20];
	int group;
	int i;

	//this is if reservation is not full
	printf("\nPlease enter a name\n"); //entering name of party
	scanf("%s", name);
	
	//checking for any duplicate names
	i = checkduplicates(name);
	if(i == 0)
		return;

	//entering number of people in the specific party
	printf("\nPlease enter number of people in your party\n");
	scanf("%d",&group);

	while(group < 1)
	{
		printf("\nPlease use a valid size for reservation group\n");
		scanf("%d",&group);
	}
	
	//if list is empty
	if(head == NULL)
	{
		head = p;
		tail = p;
	}

	//this is if there are elements in waitlist
	else
	{
		tail->next = p;
	}

	//inserting input into the structure/waitlist
	strcpy(p->names,name);
	p->groupsize = group;
	tail = p;
	p->next = NULL;
	return;
}

//function will check for any duplicate name
int checkduplicates(char *str)
{
	RESSLOT *p = head;
	while(p != NULL)
	{
		if(strcmp(str,p->names)!=0) //check for duplicates
		{
			p = p->next;
		}
		else
		{
			printf("\nThis name is already taken. Please choose another name.\n");
			return 0;
		}
	}
	return 1;
}

//function will call party into restaurant depending on how many people the party can sit
void delete(void)
{
	//check to see if there are any reservations already
	if(head == NULL)
	{
		printf("\nThere are no reservations to remove\n");
		return;
	}

	//initialize
	int party; //variable how many slots are left

	//enter size wanting to be removed on waitlist/reservation
	printf("\nEnter the size of the party wanting to remove\n");
	scanf("%d",&party);

	//assign both pointers to the head
	RESSLOT *p = head;
	RESSLOT *q = head;

	//code to delete specific wanted party in order from oldest
	while(p != NULL)
	{
		//Remove if enough slots for group
		if(party >=  p->groupsize)
		{
			printf("\nReservation for %s with a party of %d is deleted\n", p->names, p->groupsize);
			party -= p->groupsize; //update list
			
			//one slot
			if(head->next == NULL )
			{
				head = NULL;
				tail = NULL;
				free(p);
				return;
			}

			//head slot
			else if (head == p)
			{
				head = p->next;
				free(q);
				q=head;
				p=head;
			}

			//last slot
			else if (tail == p)
			{
				tail = q;
				q->next = NULL;
				free(p);
				return;
			}

			//middle slot
			else
			{
				q->next = p->next;
				free(p);
				p = q->next;
			}
		}
		else
		{
			q=p;
			p=p->next;
		}

		//finished if there are no more slots left
		if(party<1)
			return;
	}
	return;
}

//function will list out all the parties on the waitlist and reservation
void list(void)
{
	RESSLOT *p = head;

	//initialize
	int i = 1;

	//check to see if there are any reservations made at all
	if(p == NULL){
		printf("\nNo reservations have been made\n");
		return;
	}

	//code for listing name and number of party
	while(p != NULL)
	{
		printf("\nName: %s, Party Number: %d\n", p->names, p->groupsize);
		i++;
		p = p->next;
	}
	return;
}

//function will show the entries in the list that have a size less than or equal to the entered party size
void checksizes(void)
{
	RESSLOT *p = head;

	//check to see if there are any reservations made at all
	if(p == NULL){
		printf("\nNo reservations have been made\n");
		return;
	}

	//initialize
	int i=1;
	int duplicates;

	//code to enter max partysize
	printf("\nPlease enter the largest size of party you would like to list\n");
	scanf("%d",&duplicates);
	
	//show group sizes smaller or equal to entered partysize
	while(p != NULL)
	{
		if(p->groupsize <= duplicates)
			printf("\nName: %s, Party Number: %d\n", p->names, p->groupsize);
		i++;
		p = p->next;
	}
	return;
}
