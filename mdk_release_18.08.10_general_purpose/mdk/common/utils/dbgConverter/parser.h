/*
 * parser.h
 *
 *  Created on: Apr 10, 2014
 */

#ifndef PARSER_H_
#define PARSER_H_

#define MAX_LINE_SIZE 80
#define MAX_EVENTS_NUMBER 256
#define MAX_USED_EVENTS 256
#define MAX_EVENT_TYPE_SIZE 8
#define MAX_EVENT_NAME_SIZE 32
#define START_COND_INDICATOR "START"
#define STOP_COND_INDICATOR "STOP"
#define START_TYPE 1
#define STOP_TYPE 0
#define RESERVED_EVENT_ID 0x00000000
#define ENUM_TERMINANT "}"
#define RESERVED_EVENT_NAME "LOG_EVENT_RESERVED"
#define OUTPUT_FILENAME_SIZE 32
#define EVENT_NAME_VALID 0
#define EVENT_NAME_NOT_VALID 1
#define ENUM_TYPE_FOUNDED 0
#define ENUM_TYPE_NOT_FOUNDED 1

int parseInputFile(unsigned int *inputBuf, char *inputFile);
int parseAvailableEvents(char **eventsHeaders);
int registerUsedEvents(unsigned int *inputBuf, char *outputFile);
int getEvents(unsigned int *inputBuf, char *outputFileVcd, char *outputFileCsv);

#endif /* PARSER_H_ */
