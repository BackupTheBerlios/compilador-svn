#include <stdlib.h>
#include <string.h>
#include "stringqueue.h"

int StringQueue_getIndex(StringQueue * sq, const char element[MAX_STRING_SIZE]) {
	int i;
	for (i = 0; i <= sq->queueEnd; i++) {
		if (strcmp(sq->charVector[i], element) == 0) {
			return i;
		}
	}
	return -1;
}

int StringQueue_insert(StringQueue * sq, const char element[MAX_STRING_SIZE]) {
	sq->queueEnd++;
	strcpy(sq->charVector[sq->queueEnd], element);
	return sq->queueEnd;
}

void StringQueue_getNext(StringQueue * sq, char element[MAX_STRING_SIZE]) {
	strcpy(element, sq->charVector[sq->queueEnd]);
	sq->queueEnd--;
}

void StringQueue_getElementByIndex(StringQueue * sq, int pos, char element[MAX_STRING_SIZE]) {
	strcpy(element, sq->charVector[pos]);
}

int StringQueue_getSize(const StringQueue * sq) {
	return sq->queueEnd;
}

void StringQueue_initialize(StringQueue * sq) {
	sq = (StringQueue *)malloc(sizeof(StringQueue));
	sq->queueEnd = -1;
}
