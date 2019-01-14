/*
 * main.c
 *
 *  Created on: Jan 13, 2019
 *      Author: Batuhan Düzgün
 *      OpenMP and OpenMPI Hybrid Architecture
 */

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "omp.h"

#define ARRAY_SIZE  10000
#define MASTER 0
#define OFFSET_VALUE_TAG 1
#define DATA_VALUE_TAG 2

float data[ARRAY_SIZE];

int main(int argc, char *argv[])
{
	void initDataArray();
	void printDataArray();
	void update(int myoffset, int chunk, int myid);
	int checkTaskCountSize(int taskCount, int taskId, int *errorCode);

	int taskId, taskCount, taskCounter, chunkSize, offset, errorCode, sourceTaskId, destinationTaskId;

	MPI_Status status;

	errorCode = 0;

	MPI_Init(&argc, & argv);
	MPI_Comm_size(MPI_COMM_WORLD, &taskCount);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskId);

	int validCountSize = checkTaskCountSize(taskCount, taskId, &errorCode);

	if(validCountSize < 0)
	{
		MPI_Abort(MPI_COMM_WORLD, errorCode);
		exit(0);
	}

	chunkSize = ARRAY_SIZE / taskCount;

	if(taskId == MASTER)
	{
		printf("[MASTER] Task Id: %d\n", taskId);

		initDataArray();

		printDataArray();

		// Send subarray to worker task via master task!
		offset = chunkSize;
		for(taskCounter=1; taskCounter < taskCount; taskCounter++)
		{
			MPI_Send(&offset, 1, MPI_INT, taskCounter, 1, MPI_COMM_WORLD);
			MPI_Send(&data[offset], chunkSize, MPI_FLOAT, taskCounter, 2, MPI_COMM_WORLD);
			offset = offset + chunkSize;
		}

		// Master process own work !
		offset = 0;
		update(offset, chunkSize, taskId);

		// Master listen messages from worker tasks!
		for(taskCounter=1; taskCounter < taskCount; taskCounter++)
		{
			MPI_Recv(&offset, 1, MPI_INT, taskCounter, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(&data[offset], chunkSize, MPI_FLOAT, taskCounter, 2, MPI_COMM_WORLD, &status);
		}

		printDataArray();
	}

	if(taskId > MASTER)
	{
		printf("[WORKER] Task Id: %d\n", taskId);

		// Worker task receives data and offset values from master task!
		sourceTaskId = MASTER;
		MPI_Recv(&offset, 1, MPI_INT, sourceTaskId, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&data[offset], chunkSize, MPI_FLOAT, sourceTaskId, 2, MPI_COMM_WORLD, &status);

		// Then Worker task updates own data! There is not any synchronization or communication necessary
		update(offset, chunkSize, taskId);

		// Worker task send updated data to master task!
		destinationTaskId = MASTER;
		MPI_Send(&offset, 1, MPI_INT, destinationTaskId, 1, MPI_COMM_WORLD);
		MPI_Send(&data[offset], chunkSize, MPI_FLOAT, destinationTaskId, 2, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}

void update(int myoffset, int chunk, int myid)
{
	int i;
	//int threadId;

	#pragma omp parallel for shared(data)
	for (i = myoffset; i < myoffset + chunk; i++) {
		//threadId = omp_get_thread_num();
		data[i] = data[i] + (0.5f);
		//printf("TaskId: %d - ThreadId: %d\n", myid, threadId);
	}
}

void initDataArray()
{
	int i;

	// Initialize Array on shared-memory based multi-thread approach!
	#pragma omp parallel for shared(data)
	for(i=0; i < ARRAY_SIZE; i++)
	{
		data[i] = i * 1.0f;
	}
}

void printDataArray()
{
	int i;

	// We print some of data to check results whether true or not!
	for(i=ARRAY_SIZE-50; i < ARRAY_SIZE; i++)
	{
		printf("%f ", data[i]);
	}
	printf("\n");
}

int checkTaskCountSize(int taskCount, int taskId, int *errorCode)
{
	if(taskCount % 4 != 0)
	{
		if(taskId == MASTER)
		{
			printf("Quitting. Task count must be greater than 3! Current Task Count: %d\n",taskCount);
			return (-1);
		}
	}

	return (1);
}
