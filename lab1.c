#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// prototype declaration
int division(int, int);

int main()
{
    int i, divisor, quotient, sum, answer;
    sum = 0;
    srand((int)time(NULL));
    
	//forloop to count number of correct answers
    for (i=0; i<10; i++)
    {
        
        divisor = rand() % 12 + 1;
        quotient = rand() % 13;
        answer = division(quotient, divisor);
        sum += answer;
    }
    
	//output number of correct answers
    printf("You scored %d out of 10\n", sum);
    return 0;
}


int division(int quotient, int divisor)
{
    int enterednumber = 0;
    int dividend;
    
    dividend = divisor * quotient;

	//output question    
    printf("%d/%d\n", dividend, divisor);
	//input quotient guess
    printf("Enter the answer: ");
    scanf("%d", &enterednumber);

	//check if input is correct or wrong
    if (enterednumber == quotient)
    {
        printf("Correct!\n");
        return 1;
    }
    
    else
    {
        printf("Wrong!\n");
		printf("The correct answer is %d\n", quotient);
        return 0;
    }
}
