#include "file_descriptor.h"
#include "freelist.h"
#include "mainheader.h"
#include "block.h"
#include "Vfs_Operations.h"
#include "Dir_Operations.h"
#include "File_Operations.h"
#include "n-ary.h"
#include "HashTable.h"
#include "bst.h"


//int free_list[1024]={0};
int getFreeListIndex();

//int fd_index[1024]={0};
int get_file_descriptor_index(); // to get the fd index

int mount_status;
int no_of_blocks;
