#define BLOCK_SIZE 1024

struct block{
long int block_number;
char buffer[1024];
};

struct block *block_ptr_array[1024];
