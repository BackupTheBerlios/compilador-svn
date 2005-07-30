#define MAX_STRING_SIZE	50
#define MAX_QUEUE_SIZE	30

typedef struct _StringQueue {
	int queueEnd;
	char charVector[MAX_QUEUE_SIZE][MAX_STRING_SIZE];
} StringQueue;

int StringQueue_insert(StringQueue * sq, const char element[MAX_STRING_SIZE]);
void StringQueue_getNext(StringQueue * sq, char e[MAX_STRING_SIZE]);
void StringQueue_getElementByIndex(StringQueue * sq, int pos, char element[MAX_STRING_SIZE]);
void StringQueue_initialize(StringQueue * sq);
