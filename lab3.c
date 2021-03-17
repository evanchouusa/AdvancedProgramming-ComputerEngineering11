#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#define maxsize 10

//structure array
typedef struct waitlistperson
{
	char names[20];
	int groupsize;
} RESSLOT;

RESSLOT slots [maxsize];

//declare functions
void add(void);
void delete(void);
void list(void);
void checksizes(void);
int checkduplicates(char *str);

//global variables
int currentsize=0;
char name[20];

int main(void)
{
	//initialize
	int i;
	char input;

	RESSLOT *personp = slots; //assign pointer

	for(i=0;i<maxsize;i++,personp++)
	{
		personp->groupsize = 0;
		personp->names[0] = '\0';
	}

	printf("\nType 1 to add your name and number of people, type 2 to remove, type 3 to list, 4 to check sizes, and type 0 to quit program\n"); //directions to waitlisting program

	//Main loop
	while(1)
	{
		//Entering the correct commands
		printf("\nEnter a command: \n");
		scanf(" %c",&input);

		switch(input)
		{
			case '1':
				//insert waitlist
				add();
				break;
			case '2':
				//remove waitlist
				delete();
				break;
			case '3':
				//show waitlist
				list();
				break;
			case '4':
				//List groups smaller than a size
				checksizes();
				break;
			case '0':
				//end program
				return 0;
			default:
				printf("\nPlease enter one of the correct commands\n");
		}
	}
}

//Function to add yourself to waitlist
void add(void){

	//initialize
	int group;
	int i;

	RESSLOT *personp = slots;

	//Only insert if slots are open
	if(currentsize < maxsize)
	{
		//Enter names
		printf("Please enter your name\n");
		scanf("%s",name);

		//Check for used names and inform the user
		i = checkduplicates(name);
		if(i == 0)
			return;

		//Enter group size
		printf("Please enter the size of your party\n");
		scanf("%d",&group);
		while(group < 1)
		{
			printf("Please enter a valid group size.\n");
			scanf("%d",&group);
		}

		//Put name and pary size at end of list and increment counter
		personp += currentsize;
		strcpy(personp->names,name);
		personp->groupsize = group;
		currentsize++;
		printf("Your Waitlist was successfully.\n");
	}
	else
		//tell user if  full wait list
		printf("Sorry, the waitlist is full.\n");
	return;
}

//Check for duplicate names
int checkduplicates(char *str){

	RESSLOT *personp = slots;
	
	//initialize
	int i=0;
	while(i<currentsize)
	{
		if(strcmp(str,personp->names)!=0)
		{
			i++,personp++;
		}
		else
		{
			printf("This name is already taken. Please retry and enter a different name.\n");
			return 0;
		}
	}
	return 1;
}

//Remove waitlist with opening slots
void delete(void){

	RESSLOT *personp = slots;
	RESSLOT *personq = slots;

	//initialize
	int i,j;
	int party;

	//Check to see if there are any parties to remove
	if(currentsize==0){
		printf("There are no reservations to remove\n");
		return;
	}

	//Enter party number you want to remove
	printf("Please enter the party size or number of people you want to remove.\n");
	scanf("%d",&party);

	//Check for each wait list in order
	for(i=0;i<currentsize;i++,personp++)
	{
		//Remove if enough slots for group
		if(party >=  personp->groupsize)
		{
			printf("%s with groupsize %d has been  placed on a reservation and is now removed from waitlist.\n",personp->names,personp->groupsize);
			//Update slots left, shift other slots up
			party -= personp->groupsize;
			for(j=i;j<currentsize-1;j++)
			{
				personq[j] = personq[j+1];
			}
			//Continue from current slot as next slot is shifted up
			i--,personp--;
			currentsize --;
		}
		//Done if there are no more slots
		if(!party)
			return;
	}
	return;
}

//List all wait list slots and their status
void list(void){

	RESSLOT *personp = slots;

	//initialize
	int i;

	//check to see if there are any reservations made at all
	if(currentsize==0){
		printf("No reservations have been made\n");
		return;
	}

	//Show all used waitlisted slots with group size
	for(i=0;i<currentsize;i++,personp++)
	{
		printf("The number %d slot is waitlisted by %s with group size %d.\n", i+1 ,personp->names, personp->groupsize);
	}

	// Show free wait list slots
	while(i < maxsize)
	{
		printf("The number %d slot in waitlist is free.\n",i+1);
		i++;
	}
	return;
}

//List all wait listed reservation smaller than a designated groupsize
void checksizes(void)
{
	RESSLOT *personp = slots;

	//initialize
	int i;
	int invalid;

	//check to see if there are any reservations made at all
	if(currentsize==0){
		printf("No reservations have been made\n");
		return;
	}

	//Enter max groupsize for list
	printf("Please enter the largest group size you would like to list out\n");
	scanf("%d",&invalid);

	//Show waitlisted reservation smaller than group size
	for(i=0;i<currentsize;i++,personp++)
	{
		if(personp->groupsize <= invalid)
			printf("The number %d slot is waitlisted by %s with group size %d.\n", i+1 ,personp->names, personp->groupsize);
	}
	return;
}
