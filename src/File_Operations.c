#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "./../include/allheaders.h"


void add_file ( char *dest_dir_name, char *file_name, char *data )
{
	
	char dir_path[200];
	char file[200];
	char data_file_path[1024];
	int i,j;


	// path of the vfs file system ******************
//	char vfs_path[100]="/home/suhasdev/Desktop/VfsProject/";
	char vfs_path[100]="../";
	strcat(vfs_path,mheader.file_sys_label);



	struct file_descriptor fd_dir;


	strcpy(dir_path,dest_dir_name);
	strcpy(file,file_name);
	strcpy(data_file_path,data);



	if(strlen(dir_path)==0 || strlen(file)==0 || strlen(data_file_path)==0)
	{
		printf("%s\n","addfile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
		return;
	}


	if(mount_status==0)
	{
		printf("%s\n","addfile_FAILURE <VFS_NOT_MOUNTED>");
		return;
	}



	for(i=0;i<strlen(file);i++)
	{
		if(file[i]=='/')
		{
			printf("%s\n","addfile_FAILURE <INVALID_CHARACTER_IN_FILENAME>");
			return;
		}
	}



	//to check if dir_path contains file name ------>

	//struct file_descriptor *fd_check=searchAndGetFD(dir_path);

	//if(fd_check->location_block_number!=-1)
	//{
	//	printf("%s\n","Source cannot be file name");
	//	return;
	//}


	
	// error code added (not there in errorcodes.h)
	struct file_descriptor *fd_add=searchAndGetFD(dir_path);


	if(strcmp(fd_add->file_type,"file")==0)
	{
		printf("addfile_FAILURE <SOURCE_PATH_CANNOT_BE_FILE>");
		return;
	}

	


	strcpy(fd_dir.file_path,dir_path);
	strcpy(fd_dir.file_name,file);
	//strcpy(data_file_pth,data_file_path);

	
	strcpy(fd_dir.file_type,"file");


	i=get_file_descriptor_index();
	j=getFreeListIndex();


	if(j>no_of_blocks)
	{
		printf("%s\n","addfile_FAILURE <FILE_SYSTEM_FULL>");
		return;
	}	

	

	fd_dir.location_block_number=j;
	fd_dir.file_descriptor_index=i;

	int bst=insertToBst(fd_dir);

	if(bst==0)
	{	
		

		//fd_index[i]=1;
		//f_desc[i]=fd_dir;
		//free_list[f_desc[i].location_block_number]=1;
		//printf("\nNOW GOING INTO NARRAY\n");
		//insertToNarry(fd_dir);
		//printf("i value in make directory : %d",i);
		//struct block *blk=(struct block *)malloc(sizeof(struct block));
		//block_ptr_array[fd_dir.location_block_number]=blk;
	
		
		//read from file and write into the block
		FILE *fp=fopen(data_file_path,"r");
		if(fp==NULL)
		{
			printf("%s\n","addfile_FAILURE <CANNOT_READ_FROM_DATAFILE>");
			return;
		}
		else
		{

			
			fseek(fp,0,SEEK_END);
			int total_size=ftell(fp);


			if(total_size > 1024)
			{
				printf("%s\n","addfile_FAILURE <FILE_TOO_LARGE>");
				return;
			}

			


			fd_dir.file_size=total_size;
			fd_index[i]=1;
			f_desc[i]=fd_dir;
			free_list[f_desc[i].location_block_number]=1;
			//printf("\nNOW GOING INTO NARRAY\n");
			insertToNarry(fd_dir);
			//printf("i value in make directory : %d",i);
			struct block *blk=(struct block *)malloc(sizeof(struct block));
			block_ptr_array[fd_dir.location_block_number]=blk;
			

			//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>> The total size value ::::: %d \n",total_size);

			

			rewind(fp);
			fread(blk->buffer,total_size,1,fp);
			//printf(" \n Data read from the data file : %s \n",blk->buffer);

			blk->block_number=fd_dir.location_block_number;

			//printf("\n The block number : %ld \n",blk->block_number);

			
			// writing the block value in the file system

			FILE *fptr;

			if((fptr=fopen(vfs_path,"rb+"))==NULL)
			{
				printf("addfile_FAILURE <VFS_DOESNOT_EXIST>\n");
			}
			else
			{
				rewind(fptr);

				fseek(fptr,sizeof(struct mainheader)+(sizeof(struct file_descriptor))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(fd_dir.location_block_number*sizeof(struct block)),SEEK_SET);
        			fwrite(blk,sizeof(struct block),1,fptr);
        			
			}
			
			fclose(fptr);
			fclose(fp);
			free(blk);	
		}

		
		


		

	}else
	{
		clearBst();
		printf("%s\n","addfile_FAILURE <FILE_ALREADY_EXISTS>");	
		return;
	}



	printf("%s\n","addfile_SUCCESS");
	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();

}




// function to check if the file name ends with .txt extension

//


int checkTxtFile(char fname[])
{
	int len = strlen(fname);
	int i;
	char* ext = ".txt";
	char* temp;
 
	for( i = 0; i < len; i++)
	{
		if(fname[i] == '.')
		{
			temp = fname+i;
			//puts(temp);
			if(strcmp(ext,temp) == 0)
			{
				return 1;
			}
			else
				return 0;
		}
	}
}





void list_file ( char *file_path, char *output_file_name )
{


	char file_pth[200];
	char output_file[200];
	//char content[1024];
	struct block *content=(struct block *)malloc(sizeof(struct block));	


	//char vfs_path[100]="/home/suhasdev/Desktop/VfsProject/";
	char vfs_path[100]="../";

	strcat(vfs_path,mheader.file_sys_label);

	strcpy(file_pth,file_path);
	strcpy(output_file,output_file_name);


	

	if(strlen(file_pth)==0 || strlen(output_file)==0)
	{
		printf("%s\n","listfile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
		return;
	}


	if(mount_status==0)
	{
		printf("%s\n","listfile_FAILURE <VFS_NOT_MOUNTED>");
		return;
	}
	



	// condition to check if the file is text file ?????? //

	//	 function for this should be written          //

	// ***************************************************//

	struct file_descriptor  *fdptr=searchAndGetFD(file_pth);



	int blk_no;

	if(fdptr==NULL)
	{
		printf("%s\n","listfile_FAILURE <SOURCE_FILE_PATH_NOT_FOUND>");
		return;
	}
	else
	{


		int validtxtfile=checkTxtFile(fdptr->file_name);

		if(validtxtfile)
		{

			blk_no=fdptr->location_block_number;


			//printf("\n\n The block number in list file : %d \n\n",blk_no);


			FILE *fp=fopen(vfs_path,"rb+");
			if(fp==NULL)
			{
				printf("listfile_FAILURE <VFS_DOESNOT_EXIST>\n");
				return;
			}
			else
			{
				rewind(fp);
				fseek(fp,sizeof(struct mainheader)+(sizeof(struct file_descriptor))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(blk_no*sizeof(struct block)),SEEK_SET);
				fread(content,sizeof(struct block),1,fp); // reading the block structure from file system		
				



				FILE *fp1=fopen(output_file,"w+");


				if(fp1==NULL)
				{
					printf("%s\n","listfile_FAILURE <CANNOT_CREATE_OUTPUTFILE>");
					return;
				}

				//printf("\n\nData to be written : %s \n",content->buffer);

				fwrite(content->buffer,strlen(content->buffer),1,fp1);
				//printf("\nstrlen==%d",strlen(content->buffer));
				fclose(fp1);
							
			}
			
			fclose(fp);
		}
		else 
		{
			printf("%s\n","listfile_FAILURE <NOT_A_TEXT_FILE>"); // to be verified		
			return;
		}
	}		


	printf("%s\n","listfile_SUCCESS");

	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();
}



void update_file ( char *file_path, char *data )
{



	char file_pth[200];
	char data_file_path[200];

	char data_read[1024];


	//char vfs_path[100]="/home/suhasdev/Desktop/VfsProject/";
	char vfs_path[100]="../";

	strcat(vfs_path,mheader.file_sys_label);

	strcpy(file_pth,file_path);
	strcpy(data_file_path,data);


	if(strlen(file_pth)==0 || strlen(data_file_path)==0)
	{
		printf("%s\n","updatefile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
		return;
	}


	if(mount_status==0)
	{
		printf("%s\n","updatefile_FAILURE <VFS_NOT_MOUNTED>");
		return;
	}



	

	struct block *blk=(struct block *)malloc(sizeof(struct block));


	struct file_descriptor  *fdptr=searchAndGetFD(file_pth);
	
	if(fdptr==NULL)
	{
		printf("%s\n","updatefile_FAILURE <INTERNAL_FILE_NOT_FOUND>");
		return;
	}
	else
	{
		
		FILE *fp=fopen(data_file_path,"r+");
		if(fp==NULL)
		{
			printf("%s\n","updatefile_FAILURE <EXTERNAL_FILE_NOT_FOUND>");
			return;			
		}
		else
		{
			
			fseek(fp,0,SEEK_END);
			int number=ftell(fp);
			//rewind(fp);

			rewind(fp);

			//fseek(fp,0,SEEK_SET);

			//printf("\nNumber of chars in the file : %d\n",number-1);

			if(number>1024)
			{
				printf("%s\n","updatefile_FAILURE <EXTERNAL_FILE_TOO_LARGE>");
				return;
			}

			
			fread(data_read,1,number-1,fp); // reading the data from the data file path
			data_read[number-1]='\0';
			
			// reading the data from the existing file and appending the read data

			int block_num=fdptr->location_block_number;
			FILE *fptr=fopen(vfs_path,"rb+");
			
			rewind(fptr);
			fseek(fptr,sizeof(struct mainheader)+(sizeof(struct file_descriptor))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(block_num*sizeof(struct block)),SEEK_SET);

			fread(blk,sizeof(struct block),1,fptr);


			//printf(" Before updation dataread=%s*****************---------------------> %s",data_read,blk->buffer);

			strcat(blk->buffer,data_read);


			// printf("After data updated ***********------------length==%d--->>>>>>>>>> %s",strlen(blk->buffer),blk->buffer);

	
			// writing the back the updated file 

			
			rewind(fptr);
			fseek(fptr,sizeof(struct mainheader)+(sizeof(struct file_descriptor))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(block_num*sizeof(struct block)),SEEK_SET);

			fwrite(blk,sizeof(struct block),1,fptr);
			fclose(fptr);
		


		}
		fclose(fp);
		
	}


	printf("%s\n","updatefile_SUCCESS");

	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();
	
}



void remove_file ( char *file_path)
{
	char dir_path[200];
	strcpy(dir_path,file_path);
	int i;

	// whether it should be || in place of && in if clause ?????????? ---- Suhas



	if(strlen(dir_path)==0)
	{
		printf("%s\n","removefile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
		return;
	}


	if(mount_status==0)
	{
		printf("%s\n","removefile_FAILURE <VFS_NOT_MOUNTED>");
		return;
	}





	if(strcmp(dir_path,"ROOT")==0 || strcmp(dir_path,"ROOT/")==0)
		del_root(dir_path);
	else 
	{	
		int a=searchIfPathExists(dir_path);
		//printf("\naaaaaaaaaaaaaaa=============    %d\n",a);
		if(a==1){
			//printf("\nyes path exists\n");
			del_dir_file(dir_path);//naray deletion
			//printf("\n deleted from narray major achievement\n");
			//for(i=0;i<fd_count;i++)
			//printf("\n		name :%s   and path=%s \n",Fdesc[i].file_name,Fdesc[i].file_path);



		}
		else 
		{	
			printf("%s\n","removefile_FAILURE <CANNOT_FIND_SPECIFIED_FILE>");
			return;
		}

	}
			for(i=0;i<fd_count;i++)
			{ 
				fd_index[i]=0;
				clearFD(i);
				free_list[Fdesc[i].location_block_number]=0;
				block_ptr_array[Fdesc[i].location_block_number]=NULL;
				//printf("cleared %d index\n",i);
				deleteHash(Fdesc[i]);	
				deleteBST(Fdesc[i]);	
		
			}
		
		//fd_count=0;
	//	Fdesc=NULL;	
	

	printf("%s\n","removefile_SUCCESS");

	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();

}

void move_file ( char *source_file_path, char *dest_file_path ){

	char sourcePath[200];
	strcpy(sourcePath,source_file_path);	
	char destPath[200];
	strcpy(destPath,dest_file_path);	
	struct file_descriptor * fdptr;
	struct file_descriptor FD;
	fdptr= searchAndGetFD(sourcePath);
	int i,j;
	
	char path[100];
	strcpy(path,destPath);
	char *tokens[100];
	int count=0;
	tokens[count]=strtok(path,"/");
	 while(tokens[count]!=NULL)
	        { 
	           count++;
	           tokens[count]=strtok(NULL,"/");//storing the tokens in the array of char pointers pointing to each token
	           
	        }
	
	
	char destFold[100];
	
	strcpy(destFold,tokens[0]);
	
	for(i=1;i<count-1;i++)
	{
		strcat(destFold,"/");
		strcat(destFold,tokens[i]);
	}		

	
	int a=searchIfPathExists(destFold);
	if(a==0)
	{
		printf("%s\n","movefile_FAILURE <CANNOT_FIND_DESTINATION_PATH>");
		return;	
	}
	
		//char vfs_path[100]="/home/suhasdev/Desktop/VfsProject/";
		char vfs_path[100]="../";


	strcat(vfs_path,mheader.file_sys_label);

	
	if(strlen(sourcePath)==0 || strlen(destPath)==0)
	{
		printf("%s\n","movefile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
		return;
	}


	if(mount_status==0)
	{
		printf("%s\n","movefile_FAILURE <VFS_NOT_MOUNTED>");
		return;
	}





	if(fdptr==NULL)
	{
		printf("%s\n","movefile_FAILURE <CANNOT_FIND_SOURCEFILE>");
		return;
	}	


		// to check if the dest file already exists. -- added by suhas 23 nov 2012 9 am


		struct file_descriptor * fdptr_dest;
		fdptr_dest=searchAndGetFD(destPath);

		

			
		strcpy(FD.file_name,tokens[count-1]);
		strcpy(FD.file_path,destFold);
		strcpy(FD.file_type,fdptr->file_type);
		FD.file_size=fdptr->file_size;
		//FD.location_block_number=fdptr->location_block_number;
		//FD.file_descriptor_index=fdptr->file_descriptor_index;
		
		i=get_file_descriptor_index();
		j=getFreeListIndex();


		FD.location_block_number=j;
		FD.file_descriptor_index=i;
		a=searchIfPathExists(destPath);
		if(a==1)
		{
			remove_file_new(dest_file_path);		
		}
		int bst=insertToBst(FD);

		if(bst==0)
		{	
		

			fd_index[i]=1;
			f_desc[i]=FD;
			free_list[f_desc[i].location_block_number]=1;
			//printf("\nNOW GOING INTO NARRAY\n");
			insertToNarry(FD);
			//printf("i value in make directory : %d",i);
			struct block *blk=(struct block *)malloc(sizeof(struct block));
			struct block *blk_new=(struct block *)malloc(sizeof(struct block));
	
	
	
			FILE *fp=fopen(vfs_path,"rb+");
	
			if(fp==NULL)
			{
				printf("movefile_FAILURE <VFS_DOESNOT_EXIST> \n");
				return;
			}
			else
			{
				rewind(fp);
				fseek(fp,sizeof(struct mainheader)+(sizeof(struct file_descriptor))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(fdptr->location_block_number*sizeof(struct block)),SEEK_SET);

				fread(blk,sizeof(struct block),1,fp);

				strcpy(blk_new->buffer,blk->buffer);
				
				blk_new->block_number=FD.location_block_number;
		
				block_ptr_array[FD.location_block_number]=blk_new;

				// write into the file blk_new
		
				rewind(fp);
				fseek(fp,sizeof(struct mainheader)+(sizeof(struct file_descriptor))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(blk_new->block_number*sizeof(struct block)),SEEK_SET);

				fwrite(blk_new,sizeof(struct block),1,fp);
				


			}
			fclose(fp);
		
		}else clearBst();
	
	

	remove_file_new (source_file_path);
	
	printf("%s\n","movefile_SUCCESS");

	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();















}

void move_fileold ( char *source_file_path, char *dest_file_path )
{
	char destPath[200];
	char sendpath[200];
	strcpy(destPath,dest_file_path);
	char sourcePath[200];
	int count=0;
	char prevpath[200];
	char path[200];
	char *tokens[200];
	strcpy(sourcePath,source_file_path);
	int i,j;


	

	if(strlen(sourcePath)==0 || strlen(destPath)==0)
	{
		printf("%s\n","movefile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
		return;
	}


	if(mount_status==0)
	{
		printf("%s\n","movefile_FAILURE <VFS_NOT_MOUNTED>");
		return;
	}




	if(strcmp(destPath,sourcePath)==0)
	{ printf("same source and destination path");}
	else{
		i=searchIfPathExists(sourcePath);
		if(i==0)
		{
			printf("%s\n","movefile_FAILURE <CANNOT_FIND_SOURCEFILE>");
			return;
		}


		j=searchIfPathExists(destPath);
		if(j==0)
		{
			printf("%s\n","movefile_FAILURE <CANNOT_FIND_DESTINATION_PATH>");
			return;
		}

		if(i==1 &&j==1){

			//printf("\n\n\n\n\n\n\n\n ######################## B4 deletion in file : \n\n\n\n\n\n\n\n");
			//displayNaryMain();
			//displayHash();
			//displayBSTMain();

			remove_file (source_file_path);
			//strcpy(sendpath,sourcePath);
			//del_dir_file(sendpath);
			//deleteHashWithPath(sourcePath);	
			//deleteB(sourcePath);

			//printf("\n\n\n\n\n\n\n\n ######################## After deletion in file : \n\n\n\n\n\n\n\n");
			//displayNaryMain();
			//displayHash();
			//displayBSTMain();



			strcpy(prevpath,Fdesc[0].file_path);
			strcpy(Fdesc[0].file_path,destPath);
			int bst1=insertToBst(Fdesc[0]);
			if(bst1==0)
			{
				//printf("\nNOW GOING INTO NARRAY\n");
				insertToNarry(Fdesc[0]);// getting inserted into hash thru narray
			} clearBst();

				
		
			tokens[count]=strtok(prevpath,"/");
					
			
			while(tokens[count]!=NULL)
     			{ 
           			count++;
           			tokens[count]=strtok(NULL,"/");//storing the tokens in the array of char pointers pointing to each token
           
			      }
		

			for(i=1;i<fd_count;i++){
				deleteHash(Fdesc[i]);	
				deleteBST(Fdesc[i]);
			}			


					
			for(i=1;i<fd_count;i++){	
				strcpy(sendpath,destPath);
				strcpy(Fdesc[i].file_path,getNewPath(count,Fdesc[i].file_path,sendpath));
			
				int bst2=insertToBst(Fdesc[i]);

				if(bst2==0)
				{
					//printf("\nNOW GOING INTO NARRAY\n");
					insertToNarry(Fdesc[i]);// getting inserted into hash thru nary
				}
				else clearBst();						
			}
		}

	}



	printf("%s\n","movefile_SUCCESS");

	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();

}


// added at 10:30
void copy_file ( char *source_file_path, char *dest_file_path )
{	

	char sourcePath[200];
	strcpy(sourcePath,source_file_path);	
	char destPath[200];
	strcpy(destPath,dest_file_path);	
	struct file_descriptor * fdptr;
	struct file_descriptor FD;
	fdptr= searchAndGetFD(sourcePath);
	int i,j;
	
	char path[100];
	strcpy(path,destPath);
	char *tokens[100];
	int count=0;
	tokens[count]=strtok(path,"/");
	 while(tokens[count]!=NULL)
	        { 
	           count++;
	           tokens[count]=strtok(NULL,"/");//storing the tokens in the array of char pointers pointing to each token
	           
	        }
	
	
	char destFold[100];
	
	strcpy(destFold,tokens[0]);
	
	for(i=1;i<count-1;i++)
	{
		strcat(destFold,"/");
		strcat(destFold,tokens[i]);
	}		

	
	int a=searchIfPathExists(destFold);
	if(a==0)
	{
		printf("%s\n","copyfile_FAILURE <CANNOT_FIND_DESTINATIONPATH>");
		return;	
	}
	
		//char vfs_path[100]="/home/suhasdev/Desktop/VfsProject/";
		char vfs_path[100]="../";


	strcat(vfs_path,mheader.file_sys_label);

	
	if(strlen(sourcePath)==0 || strlen(destPath)==0)
	{
		printf("%s\n","copyfile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
		return;
	}


	if(mount_status==0)
	{
		printf("%s\n","copyfile_FAILURE <VFS_NOT_MOUNTED>");
		return;
	}





	if(fdptr==NULL)
	{
		printf("%s\n","copyfile_FAILURE <CANNOT_FIND_SOURCEFILE>");
		return;
	}


	if(fdptr->location_block_number==-1)
	{
		printf("%s\n","copyfile_FAILURE <CANNOT_COPY_DIR_TO_FILE>");
		return;
	}	


		// to check if the dest file already exists. -- added by suhas 23 nov 2012 9 am


		struct file_descriptor * fdptr_dest;
		fdptr_dest=searchAndGetFD(destPath);

		

			
		strcpy(FD.file_name,tokens[count-1]);
		strcpy(FD.file_path,destFold);
		strcpy(FD.file_type,fdptr->file_type);
		FD.file_size=fdptr->file_size;
		//FD.location_block_number=fdptr->location_block_number;
		//FD.file_descriptor_index=fdptr->file_descriptor_index;
		
		i=get_file_descriptor_index();
		j=getFreeListIndex();


		// file system full

		if(j>no_of_blocks)
		{
			printf("%s\n","copyfile_FAILURE <FILE_SYSTEM_FULL>");
			return;
		}



		FD.location_block_number=j;
		FD.file_descriptor_index=i;
		a=searchIfPathExists(destPath);
		if(a==1)
		{
			remove_file_new(dest_file_path);		
		}
		int bst=insertToBst(FD);

		if(bst==0)
		{	
		

			fd_index[i]=1;
			f_desc[i]=FD;
			free_list[f_desc[i].location_block_number]=1;
			//printf("\nNOW GOING INTO NARRAY\n");
			insertToNarry(FD);
			//printf("i value in make directory : %d",i);
			struct block *blk=(struct block *)malloc(sizeof(struct block));
			struct block *blk_new=(struct block *)malloc(sizeof(struct block));
	
	
	
			FILE *fp=fopen(vfs_path,"rb+");
	
			if(fp==NULL)
			{
				printf("copyfile_FAILURE <VFS_DOESNOT_EXIST> \n");
				return;
			}
			else
			{
				rewind(fp);
				fseek(fp,sizeof(struct mainheader)+(sizeof(struct file_descriptor))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(fdptr->location_block_number*sizeof(struct block)),SEEK_SET);

				fread(blk,sizeof(struct block),1,fp);

				strcpy(blk_new->buffer,blk->buffer);
				
				blk_new->block_number=FD.location_block_number;
		
				block_ptr_array[FD.location_block_number]=blk_new;

				// write into the file blk_new
		
				rewind(fp);
				fseek(fp,sizeof(struct mainheader)+(sizeof(struct file_descriptor))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(blk_new->block_number*sizeof(struct block)),SEEK_SET);

				fwrite(blk_new,sizeof(struct block),1,fp);
				


			}
			fclose(fp);
		
		}else clearBst();
	
	

	printf("%s\n","copyfile_SUCCESS");


	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();

}



void export_file ( char *vfs_file_path, char *harddisk_path )
{

	char file_pth[200];
	char output_file[200];
	//char content[1024];
	
	struct block *content=(struct block *)malloc(sizeof(struct block));

	//char vfs_path[100]="/home/suhasdev/Desktop/VfsProject/";
	char vfs_path[100]="../";


	strcat(vfs_path,mheader.file_sys_label);

	strcpy(file_pth,vfs_file_path);
	strcpy(output_file,harddisk_path);


	if(strlen(file_pth)==0 || strlen(output_file)==0)
	{
		printf("%s\n","exportfile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
		return;
	}


	if(mount_status==0)
	{
		printf("%s\n","exportfile_FAILURE <VFS_NOT_MOUNTED>");
		return;
	}


	




	struct file_descriptor  *fdptr=searchAndGetFD(file_pth);

	int blk_no;

	if(fdptr==NULL)
	{
		printf("%s\n","exportfile_FAILURE <CANNOT_FIND_SOURCEFILE>");
		return;
	}
	else
	{


		// to check if the given path contains directory

		if(fdptr->location_block_number==-1)
		{
			printf("%s\n","exportfile_FAILURE <CANNOT_EXPORT_DIR>");
			return;			
		}		

		

		blk_no=fdptr->location_block_number;	
		FILE *fp=fopen(vfs_path,"rb+");
		if(fp==NULL)
		{
			printf("%s\n","exportfile_FAILURE  <CANNOT_FIND_SOURCEFILE>");
			return;
		}
		else
		{
			rewind(fp);
			fseek(fp,sizeof(struct mainheader)+(sizeof(struct file_descriptor))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(sizeof(int))*(mheader.max_file_desc)+(blk_no*sizeof(struct block)),SEEK_SET);
			fread(content,sizeof(struct block),1,fp);			
			fclose(fp);



			FILE *fp1=fopen(output_file,"wb+");

			if(fp1==NULL)
			{
				printf("%s\n","exportfile_FAILURE <CANNOT_CREATE_OUTPUTFILE>");
				return;
			}
	

			//printf("---------000000000000000000000------------ %d\n",fdptr->file_size);
	
			fwrite(content->buffer,fdptr->file_size,1,fp1);
			fclose(fp1);
					
		}

			
	}		

	free(content);	


	printf("%s\n","exportfile_SUCCESS");

	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();
	
}







//void searchSubstringFile(char substring[])
//void searchPaths(char fname[]);
//char *listPaths[100];  int listPathsIndex; char *listNames[100];  int listNamesIndex; add it to global 
void search_file ( char *file_name, char *output_File_name )
{	
	listPathsIndex=0;
	*listPaths=NULL;

	int i;
	char substring[200];
	char out_file[200];
	strcpy(substring,file_name);
	strcpy(out_file,output_File_name);


	if(strlen(substring)==0 || strlen(out_file)==0)
	{
		printf("%s\n","searchfile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
		return;
	}


	if(mount_status==0)
	{
		printf("%s\n","searchfile_FAILURE <VFS_NOT_MOUNTED>");
		return;
	}



	searchSubstringFile(substring);



	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n ----------------> > > > > after search sustring \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	for(i=0;i<listNamesIndex;i++){
		//printf("--->>>>>------- %s \n",listNames[i]);

	}



	listPathsIndex=0;
	*listPaths=NULL;
	int j,k;
	if(listNamesIndex!=0)
		searchPaths(listNames[0]);
	for(i=1;i<listNamesIndex;i++)
	{	k=0;
		for(j=0;j<i;j++)
		{
			if(strcmp(listNames[i],listNames[j])==0)
				k=1;
				
		}
		if(k==0)
		searchPaths(listNames[i]);	

	}

	//printf("\n\n\n\n\n\n\n\n\n\n listPathsInde : %d \n\n\n\n\n\n\n",listPathsIndex);


	//for(i=0;i<listPathsIndex;i++)
	//printf("------------------>>>>>>>>>>>>>>>>>>>>>>>> %s\n",listPaths[i]);
	//listPaths contains all the paths to be written in outputfile,listPathsIndex


	//printf("%s -- > out\n",out_file);

	FILE *fp=fopen(out_file,"w");

	if(fp==NULL)
	{
		printf("searchfile_FAILURE <CANNOT_CREATE_OUTPUT_FILE>\n");
		return;
	
	}
	else
	{

		strcat(substring," found in the following locations : \n\n");
		fwrite(substring,strlen(substring),1,fp);


		/*for(i=0;i<listPathsIndex;i++){
		char newpath[200],np[200];
		int k=4;
		strcpy(newpath,listPaths[i]);
		for(j=0;j<strlen(newpath)-4;j++){
			//strcat(i+1,listPaths[i]);
			np[j]=newpath[j+k];
			k++; 

			fwrite(np,strlen(np),1,fp);
			fwrite("\n",1,1,fp);

		}}*/


		for(i=0;i<listPathsIndex;i++){
			//strcat(i+1,listPaths[i]);
			fwrite(listPaths[i],strlen(listPaths[i]),1,fp);
			fwrite("\n",1,1,fp);

		}
		fclose(fp);
	}		

	

	printf("%s\n","searchfile_SUCCESS");

	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();

}





// funbction used in movefile ----

void remove_file_new ( char *file_path)
{
	char dir_path[200];
	strcpy(dir_path,file_path);
	int i;




	if(strcmp(dir_path,"ROOT")==0 || strcmp(dir_path,"ROOT/")==0)
		del_root(dir_path);
	else 
	{	
		int a=searchIfPathExists(dir_path);
		////printf("\naaaaaaaaaaaaaaa=============    %d\n",a);
		if(a==1){
			//printf("\nyes path exists\n");
			del_dir_file(dir_path);//naray deletion
			//printf("\n deleted from narray major achievement\n");
			//for(i=0;i<fd_count;i++)
			//printf("\n		name :%s   and path=%s \n",Fdesc[i].file_name,Fdesc[i].file_path);



		}
		else 
		{	
			//printf("%s\n","removefile_FAILURE <CANNOT_FIND_SPECIFIED_FILE>");
			return;
		}

	}
			for(i=0;i<fd_count;i++)
			{ 
				fd_index[i]=0;
				clearFD(i);
				free_list[Fdesc[i].location_block_number]=0;
				block_ptr_array[Fdesc[i].location_block_number]=NULL;
				//printf("cleared %d index\n",i);
				deleteHash(Fdesc[i]);	
				deleteBST(Fdesc[i]);	
		
			}
		
		//fd_count=0;
	//	Fdesc=NULL;	
	

	//printf("%s\n","removefile_SUCCESS");

	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();

}
















