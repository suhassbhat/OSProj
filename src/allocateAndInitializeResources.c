#include <stdio.h>
#include <stdlib.h>

/**
 * Function to allocate resources. i.e 2D Array to max, allocated, and need (with number of rows equal to number of processes and 
 * number of columns equal to number of resource types), also allocates 1D array to avaialbe (with number of elements equal to number of resource types)
 * @param max [Pointer to Integer Pointer] Indicates max resources required for all the processes
 * @param allocated [Pointer to Integer Pointer] Indicates allocated resources for all the processes
 * @param need [Pointer to Integer Pointer] Indicates number of resources still required for all the processes
 * @param available [Integer Pointer] Indicates number of available resources
 * @param nProcesses [Integer] Number of Processes
 * @param nResources [Integer] Number of Resource types
 */
void allocateResources (int *** max, int *** allocated, int *** need, int ** available, int nProcesses, int nResources)
{
	int i;
	if(nProcesses <= 0 || nResources <= 0)              //If number of processes or number of resource types is negative or zero, throw error
	{
		printf("Error: Invalid number of Processes or Resource types\n"); 
		exit(EXIT_FAILURE);
	}
	*max = (int **)calloc(nProcesses,sizeof(int *));       //Allocating rows (= Number of processes) to max
	*allocated = (int **)calloc(nProcesses,sizeof(int *)); //Allocating rows (= Number of processes) to allocated
	*need = (int **)calloc(nProcesses,sizeof(int *));      //Allocating rows (= Number of processes) to need 
	*available = (int *)calloc(nResources,sizeof(int));    //Allocating 1D Array with size equal number of resource types to available

	if(max == NULL || allocated == NULL || need == NULL || available == NULL)  //If any of the above allocation failed, throw error
	{
		printf("Error: Resource allocation failed\n");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < nProcesses; i++)
	{
		(*max)[i] = (int *)calloc(nResources,sizeof(int));        //Allocating columns (= Number of resource types) to i th row of max
		(*allocated)[i] = (int *)calloc(nResources,sizeof(int));  //Allocating columns (= Number of resource types) to i th row of allocated
		(*need)[i] = (int *)calloc(nResources,sizeof(int));       //Allocating columns (= Number of resource types) to i th row of need

		if((*max)[i] == NULL || (*allocated)[i] == NULL || (*need)[i] == NULL)    //If any of the above allocation failed, throw error
		{
			printf("Error: Resource allocation failed\n");
			exit(EXIT_FAILURE);
		}
	}
}

/**
 * Function to initialize 2D matrix of max, indicating max resources of each resource type required by a process,
 * This method directly takes input from the user and intializes the matrix.
 * @param max [2D Matrix (size : nProcesses * nResources)] Indicates max resources required for all the processes
 * @param nProcesses [Integer] Number of Processes
 * @param nResources [Integer] Number of Resource types
 */ 
void inputMax(int ** max, int nProcesses, int nResources)
{
	int i,j;
	printf("\nEnter the Maximum number of Resources of each Resource type for each process\n");

	for(i = 0; i < nProcesses; i++)
	{
		printf("\nEnter Maximum number of instance of Resources required for Process %d\n",i+1);

		for(j = 0; j < nResources; j++)
		{
			printf("Enter maximum number of intances of Resource %d required by Process %d : \t",j + 1, i + 1);
			scanf("%d",&(max[i][j]));         // Inputs max resources for (i + 1)th process and (j + 1)th resource in max[i][j]

			if (max[i][j] < 0)   //If max resources is less than zero, throw error  											 
			{
				printf("Error: Invalid number of Resources entered\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

/**
 * Function to initialize 2D matrix of allocated, indicating number of resources of each resource type already allocated to the process,
 * This method directly takes input from the user and intializes the matrix. 
 * @param max [2D Matrix (size : nProcesses * nResources)] Indicates max resources required for all the processes
 * @param allocated [2D Matrix (size : nProcesses * nResources)] Indicates allocated resources for all the processes
 * @param nProcesses [Integer] Number of Processes
 * @param nResources [Integer] Number of Resource types
 */ 
void inputAllocated(int ** max, int ** allocated, int nProcesses, int nResources)
{
	int i,j;
	printf("\nEnter number of Resources of each Resource type already allocated to each process\n");
	for(i = 0; i < nProcesses; i++)
	{
		printf("\nEnter number of instances of Resources allocated for Process %d\n",i+1);
		for(j = 0; j < nResources; j++)
		{
			printf("Enter number of intances of Resource %d already allocated to Process %d : \t",j + 1, i + 1);
			scanf("%d",&(allocated[i][j]));      // Inputs allocated resources for (i + 1)th process and (j + 1)th resource in allocated[i][j]
																		
			/* If allocated resources is greater than max 
			   required resources OR allocated resources 
			   is less than zero, throw error 	*/
			if (allocated[i][j] > max[i][j] || allocated[i][j] < 0) 											 
			{
				printf("Error: Invalid number of Resources entered\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

/**
 * Function to initialize 2D matrix of need, indicating number of resources of each resource still needed by the process to complete 
 * its execution, This method computes the need matrix by subtracting max matrix with allocated matrix. The resulting matrix is need matrix 
 * @param max [2D Matrix (size : nProcesses * nResources)] Indicates max resources required for all the processes
 * @param allocated [2D Matrix (size : nProcesses * nResources)] Indicates allocated resources for all the processes
 * @param need [2D Matrix (size : nProcesses * nResources)] Indicates number of resources still required for all the processes
 * @param nProcesses [Integer] Number of Processes
 * @param nResources [Integer] Number of Resource types
 */ 
void computeNeed(int ** max, int ** allocated, int ** need, int nProcesses, int nResources)
{
	int i,j;

	for(i = 0; i < nProcesses; i++)
	{
		for(j = 0; j < nResources; j++)
		{																	
			/* Need of (i + 1)th          max number of (j + 1)th                   numbrer of already allocated   
			process for (j + 1)th    =    resource required by (i + 1)th     -      (j + 1)th resource type to  
			resource is                   process                                   (i + 1)th process              */
			need[i][j] = max[i][j] - allocated[i][j];  
		}
	}
}

/**
 * Function to initialize 1D array of available, indicating available resources of each type in the system. This method inputs total number
 * resources in the system and then computes available by subtracting the total resources by sum of resources allocated to all processes.
 * @param allocated [2D Matrix (size : nProcesses * nResources)] Indicates allocated resources for all the processes
 * @param available [1D Array (size : nProcesses)] Indicates number of available resources
 * @param nProcesses [Integer] Number of Processes
 * @param nResources [Integer] Number of Resource types
 */ 
void inputAndComputeAvailable(int ** allocated, int * available, int nProcesses, int nResources)
{
	int * totalAllocated;
	int * totalResources;
	int i,j;
	totalAllocated = (int *)calloc(nResources,sizeof(int));   //Allocating 1D Array with size equal number of resource types
	totalResources = (int *)calloc(nResources,sizeof(int));   //Allocating 1D Array with size equal number of resource types

	for(i = 0; i < nResources; i++)
	{
		totalAllocated[i] = 0;    //Initializing totalAllocated array to zero
	}

	for(i = 0; i < nProcesses; i++) 
	{
		for(j = 0; j < nResources; j++)
		{
			totalAllocated[j] = totalAllocated[j] + allocated[i][j];  //Computing total allocated Resources of each type
		}
	}

	printf("\nEnter Total Resources (before allocation) in the system\n");
	
	for(i = 0; i < nResources; i++)
	{
		printf("Enter number of intances of Resource %d : \t",i + 1);
		scanf("%d",&(totalResources[i]));       //Inputs total instances of the (i + 1)th Resource in the system before allocation 

		/*If total Resources in the System is less than total allocated 
		Resources OR if total Resources is less than zero, throw error */
		if(totalResources[i] < totalAllocated[i] || totalResources[i] < 0)
		{
			printf("Error: Invalid number of Resources entered\n");
			exit(EXIT_FAILURE);
		}

		/* Number of available                Total number of (i + 1)th              Total number of allocated   
		   instances of (i + 1)th     =       Resource in the system         -       resources of (i + 1)th   
		   resource                           before allocation                      type to all processes     */
		available[i] = totalResources[i] - totalAllocated[i];    
	}
	free(totalResources);      //Frees memory allocated to totalResources array
	free(totalAllocated);      //Frees memory allocated to totalAllocated array
}

/**
 * Function to initialize resources. i.e. Max resources required by the process, Number of Allocated resources to the process, 
 * Number of resources needed by the process, and Number of available resources
 * @param max [2D Matrix (size : nProcesses * nResources)] Indicates max resources required for all the processes
 * @param allocated [2D Matrix (size : nProcesses * nResources)] Indicates allocated resources for all the processes
 * @param need [2D Matrix (size : nProcesses * nResources)] Indicates number of resources still required for all the processes
 * @param available [1D Array (size : nProcesses)] Indicates number of available resources
 * @param nProcesses [Integer] Number of Processes
 * @param nResources [Integer] Number of Resource types
 */ 
void initializeResources(int ** max, int ** allocated, int ** need, int * available, int nProcesses, int nResources)
{
	inputMax(max,nProcesses,nResources);                                   //Inputs Max requried resources and intializes max matrix
	inputAllocated(max,allocated,nProcesses,nResources);                   //Inputs Allocated resources and intializes allocated matrix
	computeNeed(max,allocated,need,nProcesses,nResources);                 //Computes Need from Max and allocated and intializes need matrix
	inputAndComputeAvailable(allocated,available,nProcesses,nResources);   /*Inputs total Resources in the System and compute available resources
                                                                             from it. And thus, intializes available array */
}

/**
 * Frees memory of all the important data structues to which memory was allocated.
 * @param max [2D Matrix (size : nProcesses * nResources)] Indicates max resources required for all the processes
 * @param allocated [2D Matrix (size : nProcesses * nResources)] Indicates allocated resources for all the processes
 * @param need [2D Matrix (size : nProcesses * nResources)] Indicates number of resources still required for all the processes
 * @param available [1D Array (size : nProcesses)] Indicates number of available resources
 * @param nProcesses [Integer] Number of Processes
 */
void freeResources(int ** max, int ** allocated, int ** need, int * available, int nProcesses)
{
	int i;


	for(i = 0; i < nProcesses; i++)   //Frees ith row of max, allocated, need matrix
	{
		free(max[i]);
		free(allocated[i]);
		free(need[i]);
	}

	free(available);            //Frees memory allocated to available array
	
	/*Frees memory allocated to max, allocated and need which 
	are now essentially an array of integer pointers*/
	free(max);
	free(allocated);
	free(need);            
}
