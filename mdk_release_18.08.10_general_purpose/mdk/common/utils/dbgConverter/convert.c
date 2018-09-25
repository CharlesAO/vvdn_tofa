/*
 * convert.c
 *
 *  Created on: Apr 10, 2014
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "errors.h"
#include "config.h"
#include "parser.h"

/* Array, which will contains available events */
char *events[MAX_EVENTS_NUMBER];

void freeEventsArrMemory(int numOfAllocated)
{
	int i;

	for (i = 0; i < numOfAllocated; i++) {
		free(events[i]);
	}
}

int allocEventsArrMemory()
{
	int i;

	for (i = 0; i < MAX_EVENTS_NUMBER; i++) {
		events[i] = (char *)malloc(MAX_EVENT_NAME_SIZE * sizeof(char));
		if (events[i] == NULL) {
			printf("Cannot allocate memory for event number %d\n", i);
			freeEventsArrMemory(i - 1);
			return -ENOMEMORY;
		}
	}
	return ENONE;
}

int createVcdFile(char *outputFile)
{
	FILE *fp;
	time_t myTime;

	printf("Creating vcd file..\n");

	fp = fopen(outputFile, "a");
	if (fp == NULL) {
		printf("Can't open output file!\n");
		return -EFILEOPEN;
	}

	/* Creating vcd file header */
	fprintf(fp, "$date\n  ");
	myTime = time(NULL);
	fprintf(fp, "%s", ctime(&myTime));
	fprintf(fp, "$end\n");
	fprintf(fp, "$version\n  ");
	fprintf(fp, TRACER_VERSION);
	fprintf(fp, "\n$end\n");
	fprintf(fp, "$timescale\n  ");
	fprintf(fp, TIME_SCALE);
	fprintf(fp, "\n$end\n");
	fclose(fp);

	return ENONE;
}

int convertBinToVcd(char *inputFile, char *outputFileVcd,
		char *outputFileCsv, char **eventsHeaders)
{
	unsigned int inputBuf[INPUT_BUFFER_SIZE];
	int ret;

	ret = parseInputFile(inputBuf, inputFile);
	if (ret) {
		printf("Input file not parsed!\n");
		return -EPARSE;
	}

	remove(outputFileVcd);
	remove(outputFileCsv);
	if (outputFileVcd != NULL) {
		ret = createVcdFile(outputFileVcd);
		if (ret) {
			printf("Error in creating output file!\n");
			return -EFILECREATE;
		}
	}

	/* Allocate memory for array of events */
	ret = allocEventsArrMemory();
	if (ret) {
		printf("No memory allocated!\n");
		return -ENOMEMORY;
	}
	/* Parse available events from header files */
	ret = parseAvailableEvents(eventsHeaders);
	if (ret) {
		printf("Error parsing available events!\n");
		freeEventsArrMemory(MAX_EVENTS_NUMBER);
		return -EPARSE;
	}

	if (outputFileVcd != NULL) {
		ret = registerUsedEvents(inputBuf, outputFileVcd);
		if (ret) {
			printf("Error in registering used events!\n");
			freeEventsArrMemory(MAX_EVENTS_NUMBER);
			return -EPARSE;
		}
	}
	ret = getEvents(inputBuf, outputFileVcd, outputFileCsv);
	if (ret) {
		printf("Error in getting events in the time!\n");
		freeEventsArrMemory(MAX_EVENTS_NUMBER);
		return -EPARSE;
	}

	freeEventsArrMemory(MAX_EVENTS_NUMBER);

	return ENONE;
}
