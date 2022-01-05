#include <stdio.h>
#include <stdlib.h>
#include "src/allocateAndInitializeResources.c"
#include "src/bankersAlgo.c"

/**
 * Main function, takes number of input and number of resources, allocates the required data structures and calls banker's algorithm
 * @return [Integer] Returns EXIT_SUCCESS(0) on sucessfully execution, else returns something else
 */
int main() 
{
	int nProcesses,nResources;
	int ** need;
	int ** max;
	int ** allocated;
	int * available;
	int * safetySequence;
	printf("Enter number of Processes\n");
	scanf("%d",&nProcesses);   //Inputs number of processes and stores it in nProcesses
	printf("Enter number of Resource types\n");
	scanf("%d",&nResources);   //Inputs number of resource types and stores it in nResources
	allocateResources(&max,&allocated,&need,&available,nProcesses,nResources);   /* Calls allocateResources to allocate resources to 
	                                                                            need, max, allocated and available */
	initializeResources(max,allocated,need,available,nProcesses,nResources); /* Calls initializeResources to intialize all the 
																				required data structures */
	bankersAlgo(allocated,need,available,nProcesses,nResources);         /* Executes banker's algorithm */
	freeResources(max,allocated,need,available,nProcesses);                  /* Frees memory from all the allocated resources back 
																				the system */
	return EXIT_SUCCESS;
}