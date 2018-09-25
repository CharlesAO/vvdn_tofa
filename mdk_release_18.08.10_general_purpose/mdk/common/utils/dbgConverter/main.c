/*
 * main.c
 *
 *  Created on: Apr 10, 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "convert.h"

#define MAX_EVENTS_HEADERS 6
#define MAX_EVENT_HEADER_NAME_SIZE 64
#define DEF_HEADER_SET 0
#define DEF_HEADER_NOT_SET 1

static char *eventsHeaders[MAX_EVENTS_HEADERS];
static int eventsHeaderCount = 1;

void printHelp(void)
{
	printf("This is tool for converting debug input file to .vcd/.csv files\n");
	printf("For converting to .vcd file use this command line:\n");
	printf("./moviDebugConverter <inputFile> -defhdr <dbgLogEvent.h path> -vcd <outputFile.vcd>\n");
	printf("For converting to .csv file use this command line:\n");
	printf("./moviDebugConverter <inputFile> -defhdr <dbgLogEvent.h path> -csv <outputFile.csv)\n");
	printf("For converting to .vcd and .csv files use this command line:\n");
	printf("./moviDebugConverter <inputFile> -defhdr <dbgLogEvent.h path> -vcd <outputFile.vcd> -csv <outputFile.csv>\n");
	printf("For parsing user defined event headers file(s) use this command line:\n");
	printf("./moviDebugConverter <inputFile> -defhdr <dbgLogEvent.h path> -vcd/-csv <outputFile.vcd/csv> -evhdr <eventHeaderile.h>\n");
}

void freeEventsHeaderArray(int numberOfAllocated)
{
	int i;

	for (i = 0; i < numberOfAllocated; i++) {
		free(eventsHeaders[i]);
	}

	return;
}

int allocEventsHeaderArray(void)
{
	int i;

	for (i = 0; i < MAX_EVENTS_HEADERS; i++) {
		eventsHeaders[i] = malloc(MAX_EVENT_HEADER_NAME_SIZE * sizeof(char));
		if (eventsHeaders[i] == NULL) {
			printf("No memory allocated for events header %d\n", i);
			freeEventsHeaderArray(i - 1);
			return -ENOMEMORY;
		}
	}

	return ENONE;
}

int main(int argc, char *argv[])
{
	char *inputFile, *outputFileVcd = NULL, *outputFileCsv = NULL;
	int ret, i, defHeaderSet = DEF_HEADER_NOT_SET;

	if ((strcmp(argv[1], "-help") == 0) || (strcmp(argv[1], "--help") == 0)) {
		printHelp();
		return ENONE;
	}

	printf("Starting Movidius Debug Converter.\n");

	ret = allocEventsHeaderArray();
	if (ret) {
		printf("Error in memory allocation!\n");
		return -ENOMEMORY;
	}

	inputFile = argv[1];
	for (i = 2; i < argc; i++) {
		if (strcmp(argv[i], "-vcd") == 0)
			outputFileVcd = argv[i + 1];
		else if (strcmp(argv[i], "-csv") == 0)
			outputFileCsv = argv[i + 1];
		else if (strcmp(argv[i], "-evhdr") == 0) {
			if (eventsHeaderCount == MAX_EVENTS_HEADERS) {
				printf("Too many events headers passed!\n");
				freeEventsHeaderArray(MAX_EVENTS_HEADERS);
				return -EPARSE;
			}
			sprintf(eventsHeaders[eventsHeaderCount], "%s", argv[i + 1]);
			eventsHeaderCount++;
		} else if (strcmp(argv[i], "-defhdr") == 0) {
			sprintf(eventsHeaders[0], "%s", argv[i + 1]);
			defHeaderSet = DEF_HEADER_SET;
		}
	}
	eventsHeaders[eventsHeaderCount] = NULL;

	if (outputFileVcd == NULL && outputFileCsv == NULL) {
		printf("No output file selected!\n");
		freeEventsHeaderArray(MAX_EVENTS_HEADERS);
		return -EFILECREATE;
	} else if (defHeaderSet == DEF_HEADER_NOT_SET) {
		printf("Default event header file not passed!\n");
		return -EPARSE;
	}
	ret = convertBinToVcd(inputFile, outputFileVcd, outputFileCsv, eventsHeaders);
	if (ret) {
		printf("Converting not completed!\n");
		remove(outputFileVcd);
		remove(outputFileCsv);
		freeEventsHeaderArray(MAX_EVENTS_HEADERS);
		return -ECONVERT;
	}
	printf("Converting completed!\n");

	freeEventsHeaderArray(MAX_EVENTS_HEADERS);

	return ENONE;
}
