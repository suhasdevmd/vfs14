#include<stdio.h>
#include<string.h>


void add_file ( char *dest_dir_name, char *file_name, char *data );
void list_file ( char *file_path, char *output_file_name );
void update_file ( char *file_path, char *data );
void remove_file ( char *file_path);
void move_file ( char *source_file_path, char *dest_file_path );
void copy_file ( char *source_file_path, char *dest_file_path );
void export_file ( char *vfs_file_path, char *harddisk_path );
void search_file ( char *file_name, char *output_File_name );

void remove_file_new ( char *file_path);

