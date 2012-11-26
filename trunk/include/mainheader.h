struct mainheader{
char file_sys_label[100];
int max_file_desc;
int used_file_desc;
struct file_descriptor file_desc;
int free_list;
};


struct mainheader mheader;
