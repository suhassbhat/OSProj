#include <stdio.h>
#include <stdlib.h>
#include "UI.c"
/**
 * Does an iteration over the process to see which process's need can be satisfied with the available resources and can be executed.
 * @param need [2D Matrix (size : nProcesses * nResources)] Indicates number of resources still required for all the processes
 * @param available [1D Array (size : nProcesses)] Indicates number of available resources
 * @param nProcesses [Integer] Number of Processes
 * @param nResources [Integer] Number of Resource types 
 * @return [Integer] Returns the process number/id that can be safely executed OR returns -1 if no process could be executed and indicates unsafe state 
 */
int iterativeCheck(int ** need, int  * available, int nProcesses, int nResources)
{
	int i,j,flag;
	for(i = 0; i < nProcesses; i++)         //Iterating over all process
 	{
		flag = 0;           //Initializing flag to 0

		/*If the process has been already executed or if a process need for a 
		certain resource is greater than the availability of the resource, 
		set the flag to one indicating the current process can't be executed in this cycle. 
		i.e.for ith process if need[i][j] <= available[j]  for all j = 1 to nResources*/	
		for(j = 0; j < nResources; j++)
		{
			if(need[i][j] == -1 || need[i][j] > available[j])
			{
				flag = 1;         
			}
		}

		if(flag == 0)          //If flag is still 0, this process is safe to execute, hence return process id/number
		{
			return i;
		}
	}
	return -1;     //If we reach here, it indicates that none of the process are safe to execute and system is unsafe, thus return -1
}

/**
 * This method frees allocated resources of a given process and stores it in available indicating the resources available for other processes.
 * This process also updates the need table for the process to -1 indicating the process has completed its execution
 * @param allocated [2D Matrix (size : nProcesses * nResources)] Indicates allocated resources for all the processes
 * @param need [2D Matrix (size : nProcesses * nResources)] Indicates number of resources still required for all the processes
 * @param available [1D Array (size : nProcesses)] Indicates number of available resources
 * @param nResources [Integer] Number of Resource types
 * @param processNo The process number for which the allocated resources need to be freed
 */
 void freeAllocated (int ** allocated, int ** need, int *available, int nResources, int processNo)
 {
 	int i;
 	for(i = 0; i < nResources; i++)
 	{
 		need[processNo][i] = -1;    //Need of the completed process is updated to -1 which indicates completion
 		available[i] = available[i] + allocated[processNo][i];  //Makes the allocated resouces available for other processes
 		allocated[processNo][i] = 0;             //Updates the allocated resources to zero
 	}
 } 


/**
 * Checks if urgent request of resourcesby any process can be served, (i.e. Requested resource is < available resource)
 * if yes, then the request is served else request is declined
 * @param allocated [2D Matrix (size : nProcesses * nResources)] Indicates allocated resources for all the processes
 * @param need [2D Matrix (size : nProcesses * nResources)] Indicates number of resources still required for all the processes
 * @param available [1D Array (size : nProcesses)] Indicates number of available resources
 * @param nProcesses [Integer] Number of Processes
 * @param nResources [Integer] Number of Resource types
 */
void urgentRequest(int ** allocated, int ** need, int * available, int nProcesses, int nResources)
{
	int processNo,i;
	int * urgentResource;

	printf("Enter the process number which has made the urgent request");
	scanf("%d",&processNo);
	processNo = processNo - 1;
	if(processNo < 0 || processNo >= nProcesses)   //If invalid process number, deny urgent request
	{
		printf("Invalid process number. Urgent Request denied\n");
		return;
	}

	if(need[processNo][0] == -1)                          //If process already executed, deny urgent request
	{
		printf("Process already executed. Urgent Request denied\n");
		return;	
	}
	urgentResource = (int *) calloc(nResources,sizeof(int));
	for(i = 0; i < nResources; i++)  //Input the number of instances of resources urgently required
	{
		printf("Enter instances of Resource %d that is urgently required\n", i + 1);
		scanf("%d",&(urgentResource[i]));
		if(urgentResource[i] + allocated[processNo][i] > need[processNo][i])   //If requested instances are more than the need of the process, deny the request 
		{
			printf("Invalid Request, more than the need of the process, Request denied\n");
			free(urgentResource);
			return;	
		}
		if(urgentResource[i] > available[i])    //If requested instances are more than available instances, deny the request
		{
			printf("Request cannot be satisfied due to unavailability of Resources. Request denied\n");
			free(urgentResource);
			return;
		}
	}
	/*If program reaches her it means the request can be satisfied, hence increase 
	allocated resources and reduce the available resources and need of the proces*/
	for(i = 0; i < nResources; i++)
	{
		allocated[processNo][i] = allocated[processNo][i] + urgentResource[i]; //Increasing the allocated resource by requested number of resources
		need[processNo][i] = need[processNo][i] - urgentResource[i];   //Reducing the need of the process by the requested resources
		available[i] = available[i] - urgentResource[i];    //Reducing the available instances by the requested resources
	} 
	printf("Request can be satisfied and hence requested resources are allocated\n");
	free(urgentResource);
}

/**
 * Checks if the inputed process number can be executed sucessfully given the current circumstances.
 * @param need [2D Matrix (size : nProcesses * nResources)] Indicates number of resources still required for all the processes
 * @param available [1D Array (size : nProcesses)] Indicates number of available resources
 * @param nProcesses [Integer] Number of Processes
 * @param nResources [Integer] Number of Resource types
 * @return [Integer] The function returns the process number if it is sucessfully executed, -1 if the process is already executed or if the 
 * available resources is less than the needed resources.
 */
int processCheck(int ** need, int * available, int nProcesses, int nResources)
{
	int processNo, flag, i;
	printf("\nEnter the process number of the process to be checked :\t");
	scanf("%d",&processNo);
	processNo = processNo -1;
	if(processNo < 0 || processNo >= nProcesses)   //If invalid process number, return -1
	{
		printf("Invalid process number.\n");
		return -1;
	}

	if(need[processNo][0] == -1)                //If process already executed, return -1
	{
		printf("Process already executed.\n");
		return -1;	
	}
	flag = 0;

	// Loop over every resource type to check if the process's every resource types's need can be satisfied by the availble resources  
	for(i = 0; i < nResources; i++)
	{
		if(need[processNo][i] > available[i])
		{
			
			flag = 1;
		}
	}
	/*If flag stays 0 at the end of loop, it indicates that the process can be satisfied by the available resources, hence return 
	process number so that the process can be executed*/
	if(flag == 0)
	{
		return processNo;   
	}
	/*If flag changes to 1, one or more resource type's avaialble instances cannot satisfy the need of the process, hence
	process cannot be executed and return -1 */
	else
	{
		printf("The process cannot be executed as available instances of Resources cannot satisfy the need of the process\n");
		return -1;
	}
}

/**
 * Executes banker's algorithm and determines if the sytem is in the safe state or not.
 * @param allocated [2D Matrix (size : nProcesses * nResources)] Indicates allocated resources for all the processes
 * @param need [2D Matrix (size : nProcesses * nResources)] Indicates number of resources still required for all the processes
 * @param available [1D Array (size : nProcesses)] Indicates number of available resources
 * @param nProcesses [Integer] Number of Processes
 * @param nResources [Integer] Number of Resource types
 */
void bankersAlgo(int ** allocated, int ** need, int * available, int nProcesses, int nResources)
{
	int processNo, nCompleted, choice;          
	int * safetySequence;
	nCompleted = 0;                //Initializes number of completed processes to zero
	safetySequence = (int *)calloc(nProcesses, sizeof(int));    //Allocates an 1D Array to store the safety sequence
	while(nCompleted != nProcesses)
	{
		choice = UI(nCompleted);
		if(choice == 1)
		{
			processNo = iterativeCheck(need,available,nProcesses,nResources);   /*Call to iterativeCheck checks which process can be executed safely
																 				avoiding deadlock. If no process could be run, returns -1*/
		}
		if(choice == 2)																				
		{
			urgentRequest(allocated,need,available,nProcesses,nResources);   // Checks if urgent request of resources by any process can be served
			continue;		
		}

		if(choice == 3)
		{
			processNo = processCheck(need,available,nProcesses,nResources);  /*Checks if prcess could be executed, if yes, returns the processNo, if 
																			   no, returns -1 */
			if(processNo == -1)
			{
				continue;
			}
		}		

		/*If processNo is -1, it means no process in the current iteration
		 could be executed and hence call to unsafe prints the system is unsafe and
		processes executed until the moment and exits */
		if (processNo == -1)                                  
		{
			unsafe(safetySequence,nCompleted);               
			free(safetySequence);
			return;
		}
		else
		{
			freeAllocated(allocated,need,available,nResources,processNo);    /*Frees the resources allocated to the process that just finished 
																		  execution and makes it available for the other processes. 
																		  updates need of the executed process to -1, indicating completion*/
			printf("Process %d Completed its execution\n",processNo + 1);
			safetySequence[nCompleted] = processNo;                  //Stores the process's id number in the safety sequence 																
			nCompleted++;   									//Incrementing number of completed processes
		}
	}
	safe(safetySequence,nCompleted);             /*The algorithm reaches here only if all the processes are complete, hence the call to safe
												   prints that the system is safe and the safety sequecne*/
	free(safetySequence); 
}