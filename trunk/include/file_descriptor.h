#define MAX_FILE_DESC 1024

struct file_descriptor{
char file_name[100];
char file_path[100];
char file_type[10];
long int file_size;
long int location_block_number;
int file_descriptor_index;
};

struct file_descriptor f_desc[1024];

//struct free_management{


int fd_index[1024];
int free_list[1024];
int fd_count;
int baseFdescarray; //count of files and folders to be deleted
struct file_descriptor *Fdesc;  // files to be deleted

//};
//struct free_management free;
//int get_file_descriptor_index(); // to get the fd index
