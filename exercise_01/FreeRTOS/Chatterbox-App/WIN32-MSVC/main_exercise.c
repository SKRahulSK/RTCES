/*
 * FreeRTOS Kernel V10.1.1
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

 /******************************************************************************
  * NOTE: Windows will not be running the FreeRTOS project threads continuously, so
  * do not expect to get real time behaviour from the FreeRTOS Windows port, or
  * this project application.  Also, the timing information in the FreeRTOS+Trace
  * logs have no meaningful units.  See the documentation page for the Windows
  * port for further information:
  * http://www.freertos.org/FreeRTOS-Windows-Simulator-Emulator-for-Visual-Studio-and-Eclipse-MingW.html
  *
  * NOTE 2:  This file only contains the source code that is specific to exercise 2
  * Generic functions, such FreeRTOS hook functions, are defined
  * in main.c.
  ******************************************************************************
  *
  * NOTE:  Console input and output relies on Windows system calls, which can
  * interfere with the execution of the FreeRTOS Windows port.  This demo only
  * uses Windows system call occasionally.  Heavier use of Windows system calls
  * can crash the port.
  */

  /* Standard includes. */
#include <stdio.h>


/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* Priorities at which the tasks are created.
 */
#define TASK_CHATTERBOX_PRIORITY_TASK1 1
#define TASK_CHATTERBOX_PRIORITY_TASK2 1
#define TASK_CHATTERBOX_PRIORITY_TASK3 2

/*Macros*/
#define TASK_CHATTERBOX_STOP_COUNT 5

 /* Output frequencey, time between the output of sting on the console for each task
  */
#define TASK_CHATTERBOX_OUTPUT_FREQUENCY_MS 1000
  /*-----------------------------------------------------------*/

  /*
	*  Data structure to pass the required data to the task
	*  cTaskString - This is used to display the task number of eack task
	*  uTaskFlag - This is used to distiguish  between the task which run infinitely (task1 and task2) and the task(task3) which stops after 5 executions.
	*  pTaskHandle - This is used internally to stop a particular task whenever required.
	*/
typedef struct {
	char  cTaskString[6];
	unsigned  uTaskFlag;
	TaskHandle_t pTaskHandle;
} taskParams_t;

/*
 * C function (prototype) for task
 */

void vChatterboxTask(void* pvParameters);

/*-----------------------------------------------------------*/

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void main_exercise(void)
{
	/*
	 * Initialize data structures for all the three tasks
	 */
	taskParams_t task1_params = {"TASK1", 0, NULL }, task2_params = { "TASK2",0, NULL }, task3_params = { "TASK3", 1, NULL };

	/*
	 * Create the three task instances.
	 */
	xTaskCreate(vChatterboxTask, "Chatterbox Task1", 1000, &task1_params, TASK_CHATTERBOX_PRIORITY_TASK1, &task1_params.pTaskHandle);
	xTaskCreate(vChatterboxTask, "Chatterbox Task2", 1000, &task2_params, TASK_CHATTERBOX_PRIORITY_TASK2, &task2_params.pTaskHandle);
	xTaskCreate(vChatterboxTask, "Chatterbox Task3", 1000, &task3_params, TASK_CHATTERBOX_PRIORITY_TASK3, &task3_params.pTaskHandle);

	/*
	 * Start the task instances.
	 */
	vTaskStartScheduler();
	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details. */
	for (;; );
}
/*-----------------------------------------------------------*/

/*
 * C function for tasks
 */

void vChatterboxTask(void* pvParameters)
{

	taskParams_t *task_params = (taskParams_t *)pvParameters;
	
	//uTaskCount - This is used internally to count the number of executions.
	unsigned  uTaskCount = 0;
	
	TickType_t xLastWakeTime;
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	
	while (1)
	{

		printf("%s\n", task_params->cTaskString);
		/*
		 * Check if the task is Task number3 and increment the execution count
		 */
		if (task_params->uTaskFlag == 1)
		{
			uTaskCount++;
			if (uTaskCount == TASK_CHATTERBOX_STOP_COUNT)
			{
				/* Stop the task three, once the the execution count reches 5*/
				printf("Stopping Execution of %s now\n", task_params->cTaskString);
				vTaskDelete(task_params->pTaskHandle);
				task_params->pTaskHandle = NULL;
			}
		}
		// Wait for the next cycle.
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TASK_CHATTERBOX_OUTPUT_FREQUENCY_MS));
	}
}
