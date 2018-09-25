/*
 * parser.c
 *
 *  Created on: Apr 10, 2014
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "errors.h"
#include "config.h"
#include "parser.h"

#ifdef USE_BYTE_SWAP
	#include <byteswap.h>
#endif

/* Array, which will contains available events */
extern char *events[];
static int arrPosition;

static char *usedEvents[MAX_USED_EVENTS];
static int usedEventsPosition;

char *hexToBinaryString(unsigned int n)
{
	static char bin[33];
	int x;
	for (x = 0; x < 32; x++) {
		bin[x] = n & 0x80000000 ? '1' : '0';
		n <<= 1;
	}
	bin[x] = '\0';

	return(bin);
}

void writeEvents(unsigned int *inputBuf, int i, char *outputFileVcd, char *outputFileCsv)
{
	FILE *fp;
	char eventBuf[MAX_EVENT_NAME_SIZE];
	unsigned int eventId, dataBuf;
	static unsigned int lastTimestamp = 0xFFFF;
	int j;

	if (outputFileVcd != NULL) {
		fp = fopen(outputFileVcd, "a");

		eventId = inputBuf[i + 1];
		if (eventId == RESERVED_EVENT_ID)
			return;
		//clear eventBuf
		for (j = 0; j < MAX_EVENT_NAME_SIZE; j++) {
			eventBuf[j] = '\0';
		}
		sprintf(eventBuf, "%s", events[eventId]);
		dataBuf = inputBuf[i + 2];

		// Timestamp
		if ((inputBuf[i]) != lastTimestamp) {
			fprintf(fp, "#%u\n", inputBuf[i]);
			lastTimestamp = inputBuf[i];
		}
		// Event ID
		fprintf(fp, "%s_wave\n", events[eventId]);
		if (eventBuf[0] == '1') {
			fprintf(fp, "%s_start\n", events[eventId]);
			// Data
			fprintf(fp, "b%s", hexToBinaryString(dataBuf));
			fprintf(fp, " %s_data\n", eventBuf + 1);
		} else {
			fprintf(fp, "b%s", hexToBinaryString(dataBuf));
			fprintf(fp, " %s_data\n", eventBuf + 1);
		}

		fclose(fp);
	}
	if (outputFileCsv != NULL) {
		fp = fopen(outputFileCsv, "a");
		fprintf(fp, "%08u,%08u,%08X\n", inputBuf[i], inputBuf[i + 1], inputBuf[i + 2]);
		fclose(fp);
	}
}

int getEvents(unsigned int *inputBuf, char *outputFileVcd, char *outputFileCsv)
{
	unsigned int startPosition, readFromOffset;
	int i;

	printf("Getting events in time..\n");

	startPosition = inputBuf[0];
	readFromOffset = inputBuf[1];

	// Reading all bytes in order - timestamp, id, data
	if (readFromOffset) { // if the ring buffer is overloaded
		for (i = startPosition / 4; i < INPUT_BUFFER_SIZE; i += 3) {
			writeEvents(inputBuf, i, outputFileVcd, outputFileCsv);
		}
	}
	for (i = 3; i < startPosition / 4; i += 3) {
		writeEvents(inputBuf, i, outputFileVcd, outputFileCsv);
	}

	return ENONE;
}

void clearAllEvents(char *outputFile)
{
	int i;
	FILE *fp;

	fp = fopen(outputFile, "a");

	fprintf(fp, "#0\n");
	for (i = 0; i < usedEventsPosition; i++) {
		fprintf(fp, "0%s_wave\n", usedEvents[i]);
		fprintf(fp, "b00000000 %s_data\n", usedEvents[i]);
	}

	fclose(fp);
}

void addEventToUsedList(char *event)
{
	char eventBuf[MAX_EVENT_NAME_SIZE];
	int len, i;

	if (usedEventsPosition == MAX_USED_EVENTS) {
		printf("Used events are too many!\n");
		return;
	}

	// clear eventBuf
	for (i = 0; i < MAX_EVENT_NAME_SIZE; i++) {
	    eventBuf[i] = '\0';
	}
	// Get event name without first character (1/0)
	len = strlen(event);
	for (i = 1; i < len; i++) {
		eventBuf[i - 1] = event[i];
	}

	// Check if event is already in the list
	for (i = 0; i < usedEventsPosition; i++) {
		if (strcmp(usedEvents[i], eventBuf) == 0)
			return;
	}
	// Add the event name for registration
	sprintf(usedEvents[usedEventsPosition], "%s", eventBuf);
	usedEventsPosition++;
}

void freeUsedEventsArrMemory(int numOfAllocated)
{
	int i;

	for (i = 0; i < numOfAllocated; i++) {
		free(usedEvents[i]);
	}
}

int allocUsedEventsArrMemory()
{
	int i;

	for (i = 0; i < MAX_USED_EVENTS; i++) {
		usedEvents[i] = (char *)malloc(MAX_LINE_SIZE * sizeof(char));
		if (usedEvents[i] == NULL) {
			printf("Cannot allocate memory for event number %d\n", i);
			freeUsedEventsArrMemory(i - 1);
			return -ENOMEMORY;
		}
	}
	return ENONE;
}

int registerUsedEvents(unsigned int *inputBuf, char *outputFile)
{
	unsigned int startPosition, i, eventId, readFromOffset;
	FILE *fp;
	int ret;

	printf("Registering used events..\n");
	ret = allocUsedEventsArrMemory();
	if (ret) {
		printf("Error in memory allocation!\n");
		return -ENOMEMORY;
	}
	/* Read first element from the buffer, which
	 * contains the offset. If buffer is overloaded
	 * start reading from there.If buffer is not
	 * overloaded read the data until this offset.
	 */
	startPosition = inputBuf[0];
	readFromOffset = inputBuf[1];

	if (readFromOffset) {
		for (i = startPosition + 1; i < INPUT_BUFFER_SIZE; i += 3) {
			eventId = inputBuf[i];
			if (events[eventId] == NULL) {
				printf("No memory allocated for this event ID: %08X\n", eventId);
				freeUsedEventsArrMemory(MAX_USED_EVENTS);
				return -ENOMEMORY;
			} else if (eventId >= arrPosition) {
				printf("No such event ID: %08X\n", eventId);
				freeUsedEventsArrMemory(MAX_USED_EVENTS);
				return -ENOEVENT;
			} else if (eventId == RESERVED_EVENT_ID) {
				continue;
			}
			addEventToUsedList(events[eventId]);
		}
	}
	// Read the data (event ID), which is before start position
	for (i = 4; i < startPosition; i += 3) {
		eventId = inputBuf[i];
		if (events[eventId] == NULL) {
			printf("No memory allocated for this event ID: %08X\n", eventId);
			freeUsedEventsArrMemory(MAX_USED_EVENTS);
			return -ENOMEMORY;
		} else if (eventId >= arrPosition) {
			printf("No such event ID: %08X\n", eventId);
			freeUsedEventsArrMemory(MAX_USED_EVENTS);
			return -ENOEVENT;
		} else if (eventId == RESERVED_EVENT_ID) {
			continue;
		}
		addEventToUsedList(events[eventId]);
	}
	// Declare events in the .vcd file in 3 types
	fp = fopen(outputFile, "a");
	for (i = 0; i < usedEventsPosition; i++) {
		fprintf(fp, "$var reg 1 %s_wave %s_wave $end\n", usedEvents[i],
				usedEvents[i]);
		fprintf(fp, "$var event 1 %s_start %s_start $end\n", usedEvents[i],
				usedEvents[i]);
		fprintf(fp, "$var reg 32 %s_data %s_data [31:0] $end\n", usedEvents[i],
				usedEvents[i]);
	}
	fprintf(fp, "$enddefinitions $end\n");
	fclose(fp);

	clearAllEvents(outputFile);
	freeUsedEventsArrMemory(MAX_USED_EVENTS);

	return ENONE;
}

int fillEventsArray(char *line)
{
	int i, len, eventType, endOfString, validEventName;
	char eventName[MAX_EVENT_NAME_SIZE];

	len = strlen(line);

	/* we will need to add to the event name
	 * '1' or '0' and in worst case 'start' string
	 * and we will need at least 2 more characters.
	 * So the actual event name must be 2 characters smaller,
	 * than the maximum size.
	 */
	if (len - 1 > MAX_EVENT_NAME_SIZE - 2) {
		printf("Too large event name: %s\n", line);
		return -EPARSE;
	} else if (strcmp(line, ENUM_TERMINANT) == 0) {
		// enumeration type
		return ENONE;
	} else if (strcmp(line, RESERVED_EVENT_NAME ",") == 0) {
		sprintf(events[arrPosition], "%s", RESERVED_EVENT_NAME);
		arrPosition++;
		return ENONE;
	} else if (arrPosition == MAX_EVENTS_NUMBER) {
		printf("Array of available events is full!\n");
		printf("Cannot add this event: %s\n", line);
		return -ENOMEMORY;
	}

	/* Find if event is START or STOP */
	validEventName = EVENT_NAME_NOT_VALID;
	for (i = 0; i < len; i++) {
		if (line[i] == '_') {
			if (strncmp(line + (i + 1), START_COND_INDICATOR,
					strlen(START_COND_INDICATOR)) == 0) {
				endOfString = i;
				eventType = START_TYPE;
				validEventName = EVENT_NAME_VALID;
				break;
			} else if (strncmp(line + (i + 1), STOP_COND_INDICATOR,
					strlen(STOP_COND_INDICATOR)) == 0) {
				endOfString = i;
				eventType = STOP_TYPE;
				validEventName = EVENT_NAME_VALID;
				break;
			}
		}
	}

	if (validEventName == EVENT_NAME_NOT_VALID) {
		printf("Event name is not valid: %s\n", line);
		return -EPARSE;
	}
	// clear eventName
	for (i = 0; i < MAX_EVENT_NAME_SIZE; i++ ) {
		eventName[i] = '\0';
	}
	for (i = 0; i < endOfString; i++) {
		eventName[i] = line[i];
	}
	eventName[i] = '\0';
	/* Fill the array of available events with
	 * parsed event type and event name.
	 */
	sprintf(events[arrPosition], "%d%s", eventType, eventName);
	arrPosition++;

	return ENONE;
}

int parseAvailableEvents(char **eventsHeaders)
{
	FILE *fp;
	int ret, i;
	char line[MAX_LINE_SIZE];
	int enum_found;

	// clear line
	for (i = 0; i < MAX_LINE_SIZE; i++) {
		line[i] = '\0';
	}
	/* Parsing user defined events headers files (not always presented) */
	for (i = 0; eventsHeaders[i] != NULL; i++) {
		enum_found = ENUM_TYPE_NOT_FOUNDED;
		fp = fopen(eventsHeaders[i], "r");
		if (fp == NULL) {
			printf("Can't open user defined events header file: %s!\n", eventsHeaders[i]);
			return -EFILEOPEN;
		}

		while (fscanf(fp, "%s", line) != EOF) {
			if (strcmp(line, "enum") == 0) {
				fscanf(fp, "%s", line);
				if (strcmp(line, "{") == 0) {
					enum_found = ENUM_TYPE_FOUNDED;
					// Start of the enumeration
					while (strcmp(line, ENUM_TERMINANT) != 0) { // until end of the enum
						fscanf(fp, "%s", line);
						while ((strcmp(line, "//") == 0) || (strcmp(line, "=") == 0)) {
							fscanf(fp, "%[^\n]%*c", line); // skip until the end of line
							fscanf(fp, "%s", line);
						}
						ret = fillEventsArray(line);
						if (ret) {
							printf("Error in filling array of events!\n");
							fclose(fp);
							return -ENOMEMORY;
						}
					}
					break;
				}
			}
		}
		fclose(fp);
		if (enum_found == ENUM_TYPE_NOT_FOUNDED) {
			printf("Enum type not founded in file: %s\n", eventsHeaders[i]);
			return -EPARSE;
		}
	}

	return ENONE;
}

void changeEndianessType(unsigned int *inputBuf)
{
	int i;
	unsigned int buf[1];

	for (i = 0; i < INPUT_BUFFER_SIZE; i++) {
		buf[0] = __bswap_32 (inputBuf[i]);
		inputBuf[i] = buf[0];
	}
}

int parseInputFile(unsigned int *inputBuf, char *inputFile)
{
	FILE *fp;

	printf("Parsing input file..\n");

	fp = fopen(inputFile, "rb");
	if (fp == NULL) {
		printf("Can't open the input file!\n");
		return -EFILEOPEN;
	}

	/* Here we read INPUT_BUFFER_SIZE number of objects
	 * with size 4 bytes, because all our parameters
	 * (timestamp, event id and value) are 32bit numbers.
	 */
	fread(inputBuf, 4, INPUT_BUFFER_SIZE, fp);
#ifdef USE_BYTE_SWAP
	changeEndianessType(inputBuf);
#endif

	fclose(fp);

	return ENONE;
}
