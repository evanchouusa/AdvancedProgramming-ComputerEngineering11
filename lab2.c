#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//declare global variable
char schedule[10][20];
int size[10];
int counter=0;

//declare functions
void add();
void delete();
void list();

int main(void){
	//initialize
	int i;
	int quit; //variable for quit option
	int input; //address of entering specific commands

	printf("\nType 1 to add your name and number of people, type 2 to remove, type 3 to list, and type 0 to quit program\n"); //directions to waitlisting system
	
	for(i=0;i<10;i++){ 
		schedule[i][0] = '\0';
	}

	//main loop
	while(quit!=1){ //Entering the correct commands
		printf("\nEnter a command: \n");
		scanf("%d", &input);

		//switch statement for the four specific commands
		switch(input){
			case 1:
				add();
				break;
			case 2:
				delete();
				break;
			case 3:
				list();
				break;
			case 0:
				printf("\nQuitting...\n");
				quit = 1;
				break;
			default:
					printf("\nPlease enter one of the correct commands\n");
		}
	}
	return 0;
}

void add(){ //function will add party into restaurant waitlist and reservation
	//initialize
	int i;
	char requestName[10];

	//check to see if reservation is full and tell the user
	if(counter==10)
		printf("\nNo reservation available\n");
	else{ //this is if reservation is not full
		counter++;
		printf("\nPlease enter a name\n"); //entering name of party
		scanf("%s", schedule[counter-1]);
		printf("\nPlease enter number of people in your party\n"); //entering number of people in the specific party
		scanf("%d", &size[counter-1]);
	}
}

void delete(){ //function will call party into restaurant depending on how many people the party can sit
	//initialize 
	int i,j;
	int party;
	int flag=0;

	//check to see if there are any reservations already
	if(counter==0){
		printf("\nThere are no reservations to remove\n");
		return;
	}

	//enter size wanting to be removed on waitlis/reservation
	printf("\nEnter the size of your party\n");
	scanf("%d", &party);

	//code to delete specific wanted party
	for(i=0;i<counter;i++){
		if(size[i]<=party){
			printf("\nReservation for %s with a party of %d is deleted\n", schedule[i], size[i]); //letting user know which party is deleted
			party-=size[i]; //subtract the number of people sat

			//shifting following waitlist/reservation up
		   for(j=i;j<counter;j++){
			   size[j]=size[j+1];
			   strcpy(schedule[j], schedule[j+1]);
		   }
		   
		   //finding all parties needed to be deleted including following ones
		   size[counter]=0;
		   schedule[counter][0]='\0'; //continuing from current slot as the next slot is shifted up
		   i--;
		   counter--;
		   flag=1;
		   if(party!=0){
			   printf("\nNow looking to seat %d more people\n", party);
		   }
		}
		//No more slots left
		if(!party)
			return;
	}
	return;
}

void list(){ //function will list out all the parties on the waitlist and reservation
	//initialize
	int i;

	//check to see if there are any reservations made at all
	if(counter==0){
		printf("\nNo reservations have been made\n");
		return;
	}

	//code for listing name and number of party
	else{
		for(i=0;i<counter;i++){
			printf("\nName: %s, Party Number: %d\n", schedule[i], size[i]);
		}
	}
}

