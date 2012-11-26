struct dirNode{
struct file_descriptor fileDesc;
struct dirNode *firstChild;
struct dirNode *rightSibling;
};

struct dirNode * getnode();
void naryRoot();
void insertToNarry(struct file_descriptor fd);
void displayNAry(struct dirNode *d);
void unmount_filesystem();
void unmountNaryRoot();
void list_dir_normal(char path[]);
char* getNewPath(int count,char oldPath[] ,char *apendPath);
void searchSubstringFile(char substring[]);

char *listNames[100];  
int listNamesIndex;
void displayNaryMain();
