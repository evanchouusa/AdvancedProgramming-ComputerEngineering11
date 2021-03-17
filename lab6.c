#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10
#define LENGTH 20

//structure arrays
typedef struct entry 
{
    char name[LENGTH];
    int number;
    struct entry *next;
} NODE;

typedef struct list
{
    NODE *head;
    NODE *tail;
} LIST;

LIST lists[4] = { {NULL, NULL},
                  {NULL, NULL},
                  {NULL, NULL},
                  {NULL, NULL}
                };

//global variable
int counter = 0;

//declare functions
void insert();
void delete();
void list();
void searchsize();
int check_duplicate();
void free_nodes();
void change_size();

int main()
{
    //initialize
    int input;

    //main loop
    while(1)
    {
        printf("\nType 1 to add your name and number of people, type 2 to remove, type 3 to list, type 4 to check sizes, type 5 to change the size of previous entered party, and type 0 to quit program\n");
		printf("\nEnter a command: \n"); //entering the correct commands
    
        scanf("%d", &input);
        
        //this switch statement calls a function based on user selection, quits the program, or defaults because of an invalid input
        switch(input)
        {

            case 1:
            {
                insert();
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
                searchsize();
                break;
            }
            
            case 5:
            {
                change_size();
                break;
            }

            case 0:
            {
                printf("\nQuitting...\n");
                free_nodes();
                printf("Successful in freeing the nodes\n");
                return 0;
            }

            default:
            {
                printf("\nPlease enter one of the correct commands\n");
            }
        }
    }
    return 0;
}

//function will add party into restaurant waitlist and reservation
void insert()
{
    NODE *head;
    NODE *tail;
    int tempSize;
    int bin;
    char tempName[LENGTH];
    
    printf("\nPlease enter a name\n");
    scanf("%s", tempName);
    while(1) //make sure there are no duplicates
    {
        if(check_duplicate(tempName) == 0) //checking for any duplicate names
        {
            break;
        }
        printf("\nThis name is already taken. Please enter a different name\n");
        scanf("%s", tempName);
    }
    
    printf("\nPlease enter number of people in your party\n");
    scanf("%d", &tempSize);
    if(tempSize <= 2) //for parties of 1 or 2
    {
        bin = 0;
    }
        
    else if(tempSize >= 3 && tempSize <= 4) //for parties of 3 or 4
    {
        bin = 1;
    }
    
    else if(tempSize >= 5 && tempSize <= 6) //for parties of 5 or 6
    {
        bin = 2;
    }
    
    else //for parties greater than 6
    {
        bin = 3;
    }
    
    NODE *ptr;
    ptr = (NODE*) malloc(sizeof(NODE)); 
    
    if(ptr == NULL)
    {
        printf("\nFailure of memory allocation\n"); //Memory cannot be allocated
    }
    
    else //inserting input into the structure/waitlist
    {
        strcpy(ptr->name, tempName);
        ptr->number = tempSize;
        counter++;
    }
    
    if(lists[bin].head == NULL) //if list is empty
    {
        ptr->next = NULL;
        lists[bin].head = ptr;
        lists[bin].tail = ptr;
    }
    
    else //this is if there are elements in the waitlist
    {
        ptr->next = NULL;
        lists[bin].tail->next = ptr;
        lists[bin].tail = ptr;
    }
    return;
}

//function will call party into restaurant depending on how many people the party can sit
void delete()
{
    NODE *p; //declaring the variable
    NODE *q;
    int partay = -1;
    int flag;
    int i;
    int peoples = 0; //declaring the variables for either entering numbers or traversing the 2D array

    if (counter == 0)
    {
        printf("\nThere are no reservations to remove\n");
        return;
    }

    printf("\nEnter the size of the party wanting to remove\n");
    scanf("%d", &peoples);
    
    //comparing the value entered to the group ranges
    if (peoples <= 2)
        partay = 1;
    
    if (peoples <= 4 || peoples > 2)
        partay = 2;
    
    if (peoples <= 6 || peoples > 4)
        partay = 3;
    
    if (peoples > 6)
        partay = 4;
    
    //traversing
    for (i = 0; i < partay; i++)
    {
        p = lists[i].head; //pointers point to the beginning of each group
        q = lists[i].head;
        
        while (p != NULL)
        {
            if (p->number <= peoples)
            {
                printf("\nReservation for %s with a party of %d is deleted\n", p->name, p->number);
				//benihana style seating
                peoples -= p->number; 
                counter--;
                flag = 1;
                
                if(p == lists[i].head) //when p is at the top
                {
                    lists[i].head = p->next;
                    free(p); //free node
                }
                else
                {
                    q->next = p->next;
                    free(p); //free node
                }
            }
            q = p;
            p = p->next;
        }
    }
    if (flag == 0)
        printf("\nThere are no reservations with that size\n");
    return;
}

//function will list out all the parties on the waitlist and reservation
void list()
{
	//initialize pointers
    NODE *head;
    NODE *tail;

	//initialize
    int p = 0;
    int q = 0;
    int i;
    
    for(i = 0; i < 4; i++)
    {
        head = lists[i].head;
        tail = lists[i].tail;
        
        if(i == 0)
        {
            p = 1;
            q = 2;
        }
        else if(i == 1)
        {
            p = 3;
            q = 4;
        }
        else if(i == 2)
        {
            p = 5;
            q = 6;
        }
        else
        {
            p = 6;
        }
        
        if(i < 3)
        {
            printf("\n");
            printf("GROUPS OF %d TO %d:\n", p, q);
        }
        else
        {
            printf("\n");
            printf("GROUPS OF %d+:\n", p);
        }
    
        if(head == NULL)
        {
            printf("\nThe waitlist for this range group is empty\n");
        }

        //checks to see if there are any reservations made at all
        else
        {
            NODE *p;
            p = head;
            while(p != NULL)
            {
                //code for listing name and number of party
                printf("%s, party of %d\n", p->name, p->number);
                p = p->next;
            }
        }
    }
    return;
}

//function will check for any duplicate name
int check_duplicate(char q[LENGTH])
{
   int i;
   for(i = 0; i < 4; i++)
   {
        NODE *p = lists[i].head;

        while(p != NULL)
        {
            if(strcmp(p->name, q) == 0) //check for duplicates 
            {
                 return 1; 
            }
            p = p->next;
        }
   }
    return 0;
}

//function will show the entries in the list that have a size less than or equal to the entered party size
void searchsize()
{
    NODE *p;
    int searchedSize, i, success=1, flag=0;
    if(counter == 0)
    {
        printf("\nNo reservations have been made\n");
        return;
    }
    else
    {
        printf("\nPlease enter the largest size of party you would like to list\n");
        scanf("%d", &searchedSize);

        if(searchedSize <= 2) //initialize flag into array
            flag = 1;
        
        else if(searchedSize <= 4 && searchedSize > 2)
            flag = 2;
        
        else if(searchedSize <= 6 && searchedSize > 4)
            flag = 3;
        
        else if(searchedSize > 6)
            flag = 4;

        for(i = 0; i < flag; i++)
        {
            p = lists[i].head; //go through all of the ranges
            while(p != NULL)
            {
                if(p->number <= searchedSize)
                {
                    printf("\n%s, party of %d\n", p->name, p->number);
                    success = 0;
                }
                p = p->next;
            }
        }
        if(success == 1)
            printf("Nothing will fit in that table\n");
    }
} 

//function will free the nodes
void free_nodes()
{
    int i;
    for (i = 0; i < 4; i++)
    {
        NODE *p = lists[i].head;
        while(p != NULL)
        {
            printf("Node %s free\n", p->name);
            free(p);
            p = p->next;
        }
    }
    return;
}

//Extra Credit
//function will allow the user to change the size of party
void change_size()
{
    //initialize
    NODE *p;
    NODE *q;
    int i;
    int j;
    char nameses[20];
    int ogSize;
    int newSize;

    //if list is empty
    if(counter == 0)
    {
        printf("No reservations have been made\n");
        return;
    }

	printf("\nPlease understand that you will be put to the end of the waitlist in whatever group range you've changed to\n");
    
	//Enter original name
	printf("\nEnter the name you are waitlisted under that you want to change\n");
    scanf("%s", nameses);

	//Enter original party
    printf("\nEnter the original size of your group/party\n");
    scanf("%d", &ogSize);

    //Enter new size
    printf("\nEnter the size you want to change to\n");
    scanf("%d", &newSize);

    while(1 > newSize)
    {
        printf("\nPlease enter a valid size\n");
        scanf("%d", &newSize);
    }

    //search for name
    for(i = 0; i < 4; i++)
    {
        p = lists[i].head;
        q = p;
        while(p != NULL)
        {
            if(strcmp(nameses, p->name)==0)
            {
                if(ogSize != p->number)
                {
                    printf("The original group/party size you entered does not match the one in the waitlist system\n");
                    return;
                }
                if(newSize == p->number)
                {
                    printf("\nThe new group/party size you entered is the same as the original\n");
                    return;
                }
                if(lists[i].head == p)
                {
                    lists[i].head = p->next;
                }
                if(lists[i].tail == p)
                {
                    if(q != lists[i].tail)
                    {
                        lists[i].tail = q;
                    }
                    else
                    {
                        lists[i].tail = NULL;
                    }
                }

                q->next = p->next;
                p->next = NULL;
                j = (newSize < 7)?(newSize - 1)/2:3;

                //copy data
                p->number = newSize;
                p->next = NULL;

                //relink
                if(lists[j].head == NULL)
                {
                    lists[j].head = p;
                }
                else
                {
                    lists[j].tail->next = p;
                }
                lists[j].tail = p;
                printf("\nParty size change successful\n");
                return;
            }
            q = p;
            p = p->next;
        }
    }
    printf("\nName not found\n");
    return;
}
