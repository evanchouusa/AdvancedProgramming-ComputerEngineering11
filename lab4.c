#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

//defining size and length
#define SIZE 10
#define LENGTH 20

//declare global variable
int counter = 0;

//declare functions
void add();
void delete();
void list();
void checksizes();
int checkduplicates();

//union reference
typedef union moreInfo  
{
    char names[LENGTH];
    int years;
    float average;
} EXTRA;

//structure array
typedef struct entry  
{
    char name[LENGTH];
    int number;
    int special;
    EXTRA extraInfo;
} ENTRY;

ENTRY wait_list[SIZE];

int main()
{
	//initialize
	int input; //address of entering specific commands
	printf("\nType 1 to add name your name and number of people, type 2 to remove, type 3 to list, type 4 to check sizes, type 0 to quit program\n");
	//while loop ensures that the program always runs the menu after a function is complete
	while(1)
	{
		printf("\nEnter a Command\n");
		scanf("%d", &input);
		
		//switch statement for the four specific commands
		switch(input)
		{
			case 1:
			{
				add();
				break;
			}
		
			case 2:
			{
				delete();
				break;
			}

			case 3:
			{
				list();
				break;
			}

			case 4:
			{
				checksizes();
				break;
			}

			case 0:
			{
				printf("Quitting...\n");
				return 0;
			}

			default:
			{
				printf("Please enter one of the correct commands\n");
			}
		}
	}
	return 0;
}

//function will add party into restaurant waitlist and reservation
void add()
{
	//initialize
	ENTRY *p = wait_list;
	p += counter;
    int partySize;
    int tempSpecial;
    int tempType;
    char temp[LENGTH];
	
	//counter has to be less than size or reservation is full 
	if(counter < SIZE)
	{
		printf("\nEnter party name:\n");
		scanf("%s", temp);

		while(1)
        {
            //checking for duplicate names
            if(checkduplicates(temp) == 0)
            {
                break;
            }
            printf("\nThis name is already taken. Please choose another name\n");
            scanf("%s", temp);
        }
    
		//Enter Party Number
		printf("\nPlease enter number of people in your party\n");
		scanf("%d", &partySize);
		//Special Occation scan
        printf("\nIs there a special occasion?\n\nEnter 1 for yes\nEnter 2 for no\n");
        scanf("%d", &tempSpecial);

		//This is if the individual enters 1
        if(tempSpecial == 1)
        {
            printf("\n1. Birthday\n2. Anniversary\n");
			printf("\nPlease choose one of the above options for occasion\n");
            scanf("%d", &tempType);
            p->special = tempType;
            if(p->special == 1)
            {
                char tempBirthday[LENGTH];
                printf("\nEnter the name of the person:\n");
                scanf("%s", tempBirthday);
                strcpy(p->extraInfo.names, tempBirthday);
            }
            else if(p->special == 2)
            {
                int marriedYears;
                printf("\nEnter the year of the anniversary:\n");
                scanf("%d", &marriedYears);
                p->extraInfo.years = marriedYears;
                printf("\nCelebrating %d years\n", p->extraInfo.years);
              
            }
        }
		//If no special occiasion, they will be asked about average age
        else if(tempSpecial == 2)
        {
            printf("\nEnter the average age of the party:\n");
            scanf("%f", &(p->extraInfo.average));
            p->special = 0;
        }

		p->number = partySize;
		strcpy(p->name, temp);
		counter++;
	}
	
	//check to see if reservation is full and tell the user
	else
	{
		printf("\nNo reservation available\n");
	}
	return;
}

void delete()
{
    //initialize
    int i;
    int j;
    int tableSize;
    int flag = 0;  //if same size
    
	//two pointers starting at same location
    ENTRY *p = wait_list;
    ENTRY *p1 = wait_list;

    //check to see if there are any reservations already
    if(counter == 0)
    {
        printf("\nList is empty. There is nothing to delete.\n");
        return;
    }

    //Enter size of party you want remove
    printf("\nEnter the size of the party you want to be remove\n");
    scanf("%d", &tableSize);
  
    //Code to delete specific wanted party
    for(i = 0; i < counter && tableSize != 0; i++, p++)
    {
        if(tableSize >=  p->number)
        {
            tableSize -= p->number; //subtract the number of people sat
            flag = 1;

			//Lets user know who has been removed
            printf("\nReservation for %s with a party of %d is deleted\n", p->name, p->number);
            j = i;
            p1 = p;
            //shifting everyone on list up who hasn't been deleted
            while(j < counter - 1)
            {
                strcpy(p1->name, (p1+1)->name); //shift name array
                p1->number = (p1+1)->number; //shift number array
                p1->special = (p1+1)->special; //shift special occasion selection
                p1->extraInfo = (p1+1)->extraInfo; //shift union
                j++;
                p1++;
            }
            counter--;
            i--;
            p--;
        }
        if(flag == 0)
            printf("\nThere are no parties that will fit that table\n");
    }
    return;
}

//function will list out all the parties on the waitlist and reservation
void list()
{
	//initialize
	int i;
	ENTRY *p = wait_list;

	//code for listing
	if (counter != 0)
	{
		for (i = 0; i < counter; i++, p++)
		{
            switch(p->special)
            {
				//list birthday
                case 1: 
                {
                    printf("\nName: %s, party of %d\n", p->name, p->number);
                    printf("\nBirthday is under the name: %s\n", p->extraInfo.names);
                    break;
                }
				//list anniversary
                case 2: 
                {
                    printf("\nName: %s, party of %d\n", p->name, p->number);
                    printf("\nCelebrating anniversary of %d years\n", p->extraInfo.years);
                    break;
                }
				//case for average age
                case 0: 
                {
                    printf("\nName: %s, party of %d\n", p->name, p->number);
                    printf("\nAverage age of group: %f\n", p->extraInfo.average);
                    break;
                }
            }
        }
	}
	//checks to see if there are any reservations made at all
	else 
    {
		printf("\nNo Reservations have been made\n");
	}
	return;
}

//function will check for any duplicate names
int checkduplicates(char q[LENGTH])
{
	ENTRY *p = wait_list;
	int i;
    
	for (i = 0; i < SIZE; i++)
	{
		if(strcmp(p->name, q) == 0) //checks for duplicates
		{
			return 1; 
		}
		p++;
	}
	return 0;
}

//function will show the entries in the list that have a size less than or equal to the entered party size
void checksizes()
{
	//initialize
    ENTRY *p = wait_list;
	int sizeCheck;
    int i = 0;
    int flag = 0;
	printf("\nEnter the size of the party:\n");
	scanf("%d", &sizeCheck);
    
	//check list of names for those that fit the entered party size
	while(i < counter) 
	{
		if(p->number <= sizeCheck) //checks to see if sizes are smaller than input
		{
			printf("\nName: %s, Party Number: %d\n", p->name, p->number);
			flag = 1; 
        }
		p++;
		i++;
	}

	if(flag == 0) 
	{
		printf("\nNo reservations have been made\n");
	}
	return;
} 
