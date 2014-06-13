#define MAX_CHAR 80
#define MAX_NAME 30
#define MAX_LENGTH 500

#define MAX_LINE 21
#define MAX_COL 80
#define MIN_LINE 2
#define MIN_COL 1


typedef struct _node{
	int data;
	int col;
	int line;
	struct _node* next;
}node;

node* head;

enum MODES{
	COMMAND_MODE,
	INPUT_MODE
};

void insertData(int data, int line, int col);
void deleteData(int line, int col);
void printList(node* head, FILE* f);
