struct node{
	struct file_descriptor f_desc;
	char fname[10];
	struct node* next;
	struct node * previous;
};

int findIndex(char fname[]);
void  insertFile(struct file_descriptor f_desc);
void displayHash();
void unmountHash();
void searchPaths(char fname[]);
void deleteHashWithPath(char path1[]);
void deleteHash(struct file_descriptor fd);




char *listPaths[100];  
int listPathsIndex;
