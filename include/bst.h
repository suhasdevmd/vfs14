struct bstnode 
{
	struct file_descriptor fd_node;
	struct bstnode *left;
	struct bstnode *right;
	struct bstnode *parent;
};


struct bstnode* getbstnode();
int insert(struct file_descriptor newfdnode);
void displayinorder(struct bstnode *nd);
void getBstRoot();
int insertToBst(struct file_descriptor fdnode);
void clearBst();
void unmountBSTRoot();
void deleteBST(struct file_descriptor fd);
struct file_descriptor* searchAndGetFD(char file_path[]);
int deleteB(char tmpf[]);
void displayBSTMain();
