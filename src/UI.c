#include <stdio.h>
#include<stdlib.h>

/**
 * Method to print the system is safe and also prints the safety sequnce
 * @param safetySequence [Integer 1D Array] Safety sequnce is stored in the array in order of first safe process to execute to last process executed
 * @param n [Integer] Number of processes OR number of elements in the complete array
 */ 
void safe(int *safetySequence, int n)
{
	int i;
	printf("\nThe given system is safe.\n");
	printf("The Safe Sequence is : ");
	printf("<\t");
	for(i = 0; i < n; i++)
	{
		printf("%d\t",safetySequence[i] + 1);   //Prints safety sequence
	}
	printf(">\n");
	
}

/**
 * Method to print the system is unsafe and also prints the sequnce of processes successfully executed before becoming unsafe 
 * @param safetySequence [Integer 1D Array] Sequnce of processes executed is stored in the array in order of first safe process to execute 
 * to last process executed
 * @param n [Integer] Number of processes safely executed OR number of elements in the complete array
 */ 
void unsafe(int *safetySequence, int n)
{
	int i;
	printf("\nThe given system is unsafe.\n");
	printf("The Sequence of processes executed before entering unsafe state is : ");
	printf("<\t");
	for(i = 0; i < n; i++)
	{
		printf("%d\t",safetySequence[i] + 1);  //Prints processes that successfully executed before entering unsafe state
	}
	printf(">\n");
	
}

/**
 * User interface to allow urgent request and process check
 * @param interationNo [Integer] Denotes the current iteration / Number of processes completed
 * @return [Integer] returns the choice entered by the user
 */
int UI(int iterationNo)
{
	int choice;
	choice = 0;
	
	while(!(choice >= 1 && choice <=3))            //Loop until you get a valid choice from user
	{
		printf("\nIteration No : %d\n",iterationNo);
		printf("1)Do an iteration\n");
		printf("2)Urgent Request\n");
		printf("3)Check process\n");
		printf("Enter your choice : \t");
		scanf("%d",&choice);
		if(!(choice >= 1 && choice <=3))
		{
			printf("\nInvalid choice! Try Again.\n");
		}
	}
	return choice;
}