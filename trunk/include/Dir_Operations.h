#include<stdio.h>
#include<string.h>

void make_dir ( char *parent_path, char *dir_name);
void delete_dir ( char *path );
void move_dir ( char *source_dir_path, char *dest_dir_path );
void list_dir ( char *dir_name, int flag, char *output_file_name );
