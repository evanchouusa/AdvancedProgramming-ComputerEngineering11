#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define maxsize 10

//structure array
typedef struct waitlistperson
{
	char names[20];
	int groupsize;
	struct waitlistperson *next;
} SLOT;

typedef struct waitlistgroup
{
	SLOT *head;
	SLOT *tail;
}RESSLOT;

//declare functions
void * AutoSave (void * arg);
void listAutosave (char *name);
void readfile(char *fname);
void savefile(char *fname);
void add(char *name, int group);
void delete(void);
int checkduplicates(char *str);
void list(void);
void checksizes(void);
void freeall(void);
void free_recur(int place, SLOT *begin);
void showListBackward(int list, SLOT *wl);
void showNameBackward(int list, SLOT *wl, char *ptr);

RESSLOT waitl[4];
pthread_mutex_t lock;

int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		printf("The name of the file is missing!\n");
		readfile("init");
        return 1;
	}
	else
	{
		readfile(argv[1]);
	}
    
	//Create thread
	pthread_t thr;
	if(argc == 3)
	{
		pthread_create(&thr,NULL,AutoSave,(void *) argv[2]);
	}
	else
	{
		pthread_create(&thr,NULL,AutoSave,(void *) "temp.bin");
	}
	pthread_mutex_init(&lock,NULL);
    
    //initialize
    char input;
    char name [20];
    int group;
    int i;

	printf("\nType 1 to add your name and number of people, 2 to remove, 3 to list, 4 to check sizes, 5 to show list backward, 6 to show names backward, 7 to autosave, and 0 to quit program\n"); //directions to waitlisting system
    
	//main loop
	while(1)
	{
        printf("\nEnter a command: \n"); //entering the correct commands
		fflush(stdin);
		scanf(" %c",&input);
        
        //switch statement for the specific commands
		switch(input)
		{
			case '1':
				//this is if reservations is not full
				printf("\nPlease enter a name\n"); //entering name of party
				scanf("%s",name);
                
				//entering number of people in the specific party
				printf("\nPlease enter number of people in the party\n");
				scanf("%d",&group);
                
				while(group < 1)
				{
					printf("\nPlease use a valid size for reservation group\n");
					scanf("%d",&group);
				}
                
				//Check for any duplicate names
				i = checkduplicates(name);
				if(i == 1)
				{
                    continue;
				}
                add(name, group);
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
			case '5':
				showListBackward(0, waitl[0].head);
				break;
			case '6':
				showNameBackward(0, waitl[0].head, waitl[0].head->names);
				putchar('\n');
				break;
			case '7':
				//List all waitlists in autosave file
				if(argc == 3)
				{
					listAutosave(argv[2]);
				}
				else
				{
					listAutosave("temp.bin");
				}
				break;
			case '0':
				//end program and make file
				if(argc == 1)
				{
					savefile("init");
					printf("\nSuccessfully saved the waitlist\n");
				}
				//update file
				else
				{
					savefile(argv[1]);
					printf("\nSuccessfully updated the waitlist\n");
				}
				//End thread
				pthread_mutex_lock(&lock);
				printf("\nStarting to join thread\n");
				pthread_cancel(thr);
				printf("\nSuccessfully in joining thread\n");
				pthread_mutex_unlock(&lock);
				printf("\nSuccessfully in unlocking thread\n");
				free_recur(0, waitl[0].head);
				return 0;
			default:
				printf("\nPlease enter one of the correct commands\n");
		}
	}
}

//AutoSave File
void *AutoSave (void * arg)
{
	int i;
	SLOT *p;
	FILE *outfp;
	while(1)
	{
		sleep(15);
		if((outfp = fopen((char *) arg, "wb")) == NULL)
		{
			printf("\nError in opening file. Starting with empty list\n");
		}
		else
		{
			pthread_mutex_lock(&lock);
			for(i=0;i<4;i++)
			{
				p = waitl[i].head;
				while(p != NULL)
				{
					fwrite(p,sizeof(SLOT),1,outfp);
					p = p->next;
				}
			}
			printf("\nAutosave is successful\n");
			pthread_mutex_unlock(&lock);
		}
		fclose(outfp);
	}
}

//function will list reservations in autosave file
void listAutosave (char *name)
{
	SLOT p;
	FILE *infp;
	pthread_mutex_lock(&lock);
	if((infp = fopen(name,"rb")) == NULL)
	{
		printf("\nError opening file\n");
		return;
	}
	pthread_mutex_unlock(&lock);
	while(fread(&p,sizeof(SLOT),1,infp) == 1)
	{
		printf("\nName: %s, Party Number: %d\n",p.names,p.groupsize);
	}
	return;
}

//function to read file from argument
void readfile(char *fname)
{
	//Make sure file opens successfully
	FILE *infp;
	if((infp = fopen(fname, "r")) == NULL)
	{
		printf("\nError opening file. Starting with empty list\n");
		return;
	}
	fseek(infp,35,SEEK_SET);
	char name[20];
	int group;
	int i;
	while(fscanf(infp,"%s%d",name,&group) == 2)
	{
		//check for duplicates
		i = checkduplicates(name);
		if(i == 0)
		{
			continue;
		}
		add(name, group);
	}
	printf("\nWaiting list file successfully loaded\n");
	list();
	fclose(infp);
	return;
}

//updating or create file
void savefile(char *fname)
{
	FILE *outfp;
	if((outfp = fopen(fname, "w")) == NULL)
	{
		printf("\nError saving waitlist to file\n");
		return;
	}
	//Header format
	fprintf(outfp,"Name\tGroup Size\n------------------\n");
	//insert name and group size in rows
	int i;
	SLOT *p;
	for(i=0;i<4;i++)
	{
		p = waitl[i].head;
		while(p!=NULL)
		{
			fprintf(outfp,"%s\t%d\n",p->names,p->groupsize);
			p = p->next;
		}
	}
	fclose(outfp);
	return;
}

//function will add party into restaurant waitlist and reservation
void add(char *name, int group)
{
	SLOT *p;
	if((p=(SLOT *)malloc(sizeof(SLOT))) == (SLOT *) NULL)
	{
		printf("\nMemory cannot be allocated\n"); //Memory cannot be allocated
		return;
	}
    
    //initialize
	int i;
    
	i = (group < 7) ? (group-1)/2 : 3;

    //link the structure
	pthread_mutex_lock(&lock);
    
	//if list is empty
	if(waitl[i].head == NULL)
	{
		waitl[i].head = p;
		waitl[i].tail = p;
	}
    
	//this is if there are elements in waitlist
	else
	{
		waitl[i].tail->next = p;
	}
    
	//inserting input into the structure/waitlist
	strcpy(p->names,name);
	p->groupsize = group;
	waitl[i].tail = p;
	p->next = NULL ;
	pthread_mutex_unlock(&lock);
	printf("\nInserting into waitlist is successful\n");
	return;
}

//function will check for any duplicate name
int checkduplicates(char *str)
{
	SLOT *p;
	int i;
	for(i=0;i<4;i++)
	{
		p = waitl[i].head;
		while(p != NULL)
		{
			if(strcmp(str,p->names)==0) //check for duplicates
			{
				printf("\nThis name is already taken. Please choose another name.\n");
				return 0;
			}
			p = p->next;
		}
	}
	return 1;
}

//function will call party into restaurant depending on how many people the party can sit
void delete(void)
{
    //initialize
	int party;
    int i;
    
    //enter size wanting to be removed on waitlist/reservation
	printf("\nEnter the size of the party wanting to remove\n");
	scanf("%d",&party);
    
    //declare pointers
	SLOT *p;
	SLOT *q;
    
    //code to delete specific wanted party in order from oldest
	for(i=0;i<4;i++)
	{
		//remove if enough slots for group
		switch(i)
		{
			case 0:
				if(party < 1)
				{
					printf("\nNo more further slots to remove\n");
					return;
				}
				break;
			case 1:
				if(party < 3)
				{
					printf("\nNo more further slots to remove\n");
					return;
				}
				break;
			case 2:
				if(party < 5)
				{
					printf("\nNo more further slots to remove\n");
					return;
				}
				break;
			case 3:
				if(party < 7)
				{
					printf("\nNo more further slots to remove\n");
					return;
				}
				break;
		}
        
        //assign both pointers to the head
		p = waitl[i].head;
		q = waitl[i].head;
		while(p != NULL)
		{
			//Remove if enough slots for group
			if(party >= p->groupsize)
			{
				printf("\nReservation for %s with a party of %d is deleted\n",p->names,p->groupsize);
				party -= p->groupsize; //update list
				pthread_mutex_lock(&lock);
				
                //one slot
				if(waitl[i].head->next == NULL )
				{
					waitl[i].head = NULL;
					waitl[i].tail = NULL;
					free(p);
					break;
				}
				//head slot
				else if (waitl[i].head == p)
				{
					waitl[i].head = p->next;
					free(q);
					q=waitl[i].head;
					p=q;
				}
				//last slot
				else if (waitl[i].tail == p)
				{
					waitl[i].tail = q;
					q->next = NULL;
					free(p);
					break;
				}
				//middle slot
				else
				{
					q->next = p->next;
					free(p);
					p = q->next;
				}
				pthread_mutex_unlock(&lock);
			}
			else
			{
				q=p;
				p=p->next;
			}
		}
	}
	return;
}

//function will list out all the parties on the waitlist and reservation
void list(void)
{
    //initialize
	int i,j;
    
	for(i=0;i<4;i++)
	{
		SLOT *p = waitl[i].head;
		j = 1;
        
		//check to see if there are any reservations made at all
		if(p == NULL)
		{
			printf("\nNo reservations have been made\n");
            return;
        }
		
		//code for listing name and number of party
        while(p != NULL)
        {
            printf("In division %d, the number %d slot is waitlisted by %s with group size %d.\n", i, j , p->names, p->groupsize);
            j++;
            p = p->next;
        }
	}
	return;
}

//function will show the entries in the list that have a size less than or equal to the entered party size
void checksizes(void)
{
    //initialize
	SLOT *p ;
	int j=1;
	int i;
	int duplicates;
    
	//code to enter max partysize
	printf("\nPlease enter the largest size of party you would like to list\n");
	scanf("%d",&duplicates);

    
	//show group sizes smaller or equal to entered partysize
	for(i=0;i<4;i++)
	{
		p = waitl[i].head;
		while(p != NULL)
		{
			if(p->groupsize <= duplicates)
			{
				printf("In division %d, the number %d slot is waitlisted by %s with group size %d.\n", i, j , p->names, p->groupsize);
			}
			j++;
			p = p->next;
		}
	
		//this will stop group size search if less than next range's group size
		switch(i)
		{
			case 0:
				if(duplicates < 3)
				{
					return;
				}
				break;
			case 1:
				if(duplicates < 5)
				{
					return;
				}
				break;
			case 2:
				if(duplicates < 7)
				{
					return;
				}
				break;
		}
	}
}

//funciton will free all slots recursively
void free_recur(int place, SLOT *p)
{
	if(p->next == NULL && place != 3)
	{
		free_recur(place+1,waitl[place+1].head);
	}
	else if (p->next != NULL)
	{
		free_recur(place, p->next);
	}
	free(p);
	return;
}

//function will use recursive funciton to show list backwards
void showListBackward(int i, SLOT *p)
{
    //initialize
	int j = i;
	
	if(p->next == NULL && i != 0)
	{
		j++;
		showListBackward(j,waitl[j].head);
	}
	else if(p -> next != NULL)
	{
		showListBackward(i,p->next);
	}
	printf("%s\n", p->names);
	return;
}

//function will use recursive function to show names backwards
void showNameBackward(int i, SLOT* p, char *ptr)
{
	if(p->next == NULL && i !=3 && *ptr == '\0')
	{
		showNameBackward(i+1,waitl[i+1].head,waitl[i+1].head->names);
		putchar('\n');
	}
	else if(p->next != NULL && *ptr == '\0')
	{
		showNameBackward(i,p->next,p->next->names);
		putchar('\n');
	}
	else if(*ptr != '\0')
	{
		showNameBackward(i,p, ptr+1);
		putchar(*ptr);
	}
	else
	{
		putchar('\n');
	}
	return;
}

