#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "./../include/allheaders.h"
#include<unistd.h>


void create_vfs(char filelabel[],long int size)
{
	FILE *fp;
	char a[1000000]={'a'};
	int i,ss;

	//mount_status=0;


	if(strlen(filelabel)==0 || size==0)
	{
		printf("createvfs_FAILURE %s\n",ERR_VFS_CREATE_00); //Insufficient Arguments
		return;
	}

	if(size<1 || size>1024)
	{
		printf("createvfs_FAILURE %s\n",ERR_VFS_CREATE_04); //Invalid Size
		return;
	}


	if(strlen(filelabel)>30)
	{
		printf("createvfs_FAILURE %s\n",ERR_VFS_CREATE_05); //Label too large
		return;
	}


	for(i=0;i<strlen(filelabel);i++)
	{
		if(filelabel[i]=='/')
		{
			printf("createvfs_FAILURE %s\n",ERR_VFS_CREATE_03); //Invalid character in name
			return;
		}
	}



	//printf("%d\n ------ > is the block size",BLOCK_SIZE);

	no_of_blocks=(int)(size*1024)/BLOCK_SIZE;

	
	char path[100]="../";
	strcat(path,filelabel);




	if(access(path,F_OK)!=-1)
	{
		printf("createvfs_FAILURE %s\n",ERR_VFS_CREATE_01);// File already exists
		return;
	}





	fp=fopen(path,"wb+");

	if(fp==NULL)
	{
		printf("createvfs_FAILURE %s\n",ERR_VFS_CREATE_02);// cannot create datafile
	}
	else
	{
		//allocating memory 
		//fseek(fp,size,SEEK_SET);
		rewind(fp);
		//fwrite(suhas,sizeof(char)*1000000,1,fp);
		//fwrite(a,size,1,fp);




		fseek(fp, size*1024, SEEK_SET);
		fputc('\n', fp);
		//fclose(fp);

		//for(ss=0;ss<1000000;ss++)
		//{
		//	putc('a',fp);
		//}


	}


	//fp=fopen(path,"wb");

	//initializing the main header structure

	strcpy(mheader.file_sys_label,filelabel);
	mheader.used_file_desc=0;
	mheader.max_file_desc=MAX_FILE_DESC;
	//struct file_descriptor fd;
	//struct freelist fl;
	//mheader.file_desc=fd;
	//mheader.free_list=fl;




	//writing the mainheader structure into the memory

	fseek(fp,0,SEEK_SET);
	fwrite(&mheader,sizeof(struct mainheader),1,fp);




	// declared file descriptor array in the header file


	
	for(i=0;i<mheader.max_file_desc;i++)
	{
		//f_desc[i].location_block_number=i;

		//setting the block number and isFree to 0 to indicate its free
		//free_list[i].block_number=i;
		free_list[i]=0;
		fd_index[i]=0;
	}



	




	
	fseek(fp,sizeof(struct mainheader),SEEK_SET);
	fwrite(&f_desc,(sizeof(struct file_descriptor))*(mheader.max_file_desc),1,fp);
	fwrite(&free_list,(sizeof(int))*(mheader.max_file_desc),1,fp);
	fwrite(&fd_index,(sizeof(int))*(mheader.max_file_desc),1,fp);



	fclose(fp);


	printf("%s\n","createvfs_SUCCESS");
	mount_status=0;






}


// function for mounting the VFS

void mount_vfs(char filelabel[]){

	char path[100]="../";
	int i;
	FILE *fp;

	
	if(strlen(filelabel)==0)
	{
		printf("mountvfs_FAILURE %s\n",ERR_VFS_MOUNT_00); //Insufficient arguments
		return;
	}


	strcat(path,filelabel);
	
	
	//if(access(path,F_OK)!=­‐1);
	//else 
	//{
	//	printf("%s\n","mountvfs_FAILURE <DATA_FILE_NOT_FOUND>");
	//	return;
	//}

	//printf("-----> %s\n",path);

	if(access(path,F_OK)!=-1)
	{
	}
	else
	{
		printf("mountvfs_FAILURE %s\n",ERR_VFS_MOUNT_01); //Data file not found
		return;
	}	


	if(mount_status==1)
	{
		printf("mountvfs_FAILURE %s\n",ERR_VFS_MOUNT_03);// vfs already mounted
		return;
	}


	fp=fopen(path,"rb+");
	

	if(fp==NULL)
	{
		printf("mountvfs_FAILURE %s\n",ERR_VFS_MOUNT_02); // Cannot read from file
		return;
	}
	//else
	//{	//mounting
		
		//mount_status=1;


		


		naryRoot();// create root for narry
		getBstRoot();// create root for bst

		fread(&mheader,sizeof(struct mainheader),1,fp);
		fread(&f_desc,(sizeof(struct file_descriptor))*(mheader.max_file_desc),1,fp);
		fread(&free_list,(sizeof(int))*(mheader.max_file_desc),1,fp);
		fread(&fd_index,(sizeof(int))*(mheader.max_file_desc),1,fp);
	
		//printf("Started mounting --- .\n");
		
		

		//int la;	
		//for(la=0;la<25;la++) {printf("%d .. %s\n",la,f_desc[la].file_name);}
		//for(la=0;la<25;la++) {printf(" - --------------> %d..%d\n",la,fd_index[la]);}

		for(i=1;i<mheader.max_file_desc;i++){
			if(fd_index[i]!=0)
			{
				struct file_descriptor fd;
				//fd=f_desc[i];


				strcpy(fd.file_name,f_desc[i].file_name);
				strcpy(fd.file_path,f_desc[i].file_path);
				strcpy(fd.file_type,f_desc[i].file_type);
				fd.file_size=f_desc[i].file_size;
				fd.location_block_number=f_desc[i].location_block_number;
				fd.file_descriptor_index=f_desc[i].file_descriptor_index;

				//printf("------>>>>>> %d\n",fd_index[i]);

				//printf("----> suhasssssssssssssssss ---> %s\n",fd.file_name);				


				int bst=insertToBst(fd);
				if(bst==0){
					
					//printf("\nNOW GOING INTO NARRAY\n");
					insertToNarry(fd);

				
				
				}else clearBst();
			}
		}


		fclose(fp);

		
	
		//printf("Completed mounting.\n");
		mount_status=1;
		printf("%s\n","mountvfs_SUCCESS");
	//}

	
}


// function for clearing the file descriptor

void clearFD(int index)
{
	fd_index[index]=0;
	strcpy(f_desc[index].file_path,"");
	strcpy(f_desc[index].file_name,"");
	strcpy(f_desc[index].file_type,"");
	f_desc[index].file_size=0;
	f_desc[index].location_block_number=0;
	f_desc[index].file_descriptor_index=0;

}



// function for unmounting the VFS


void unmount_vfs(char filelabel[]){

	char path[100]="../";
	FILE *fp;
	int i;
	




	if(strlen(filelabel)==0)
	{
		printf("unmountvfs_FAILURE %s\n",ERR_VFS_UNMOUNT_00); //Insufficient arguments
		return;
	}


	
	if(mount_status==0)
	{
		printf("unmountvfs_FAILURE %s\n",ERR_VFS_UNMOUNT_03); //vfs not mounted
		return;
	}


	strcat(path,filelabel);


	if(access(path,F_OK)!=-1)
	{
	}
	else
	{
		printf("unmountvfs_FAILURE %s\n",ERR_VFS_UNMOUNT_01); // Data file not found
		return;
	}






	fp=fopen(path,"rb+");

	if(fp==NULL)
	{
		printf("unmountvfs_FAILURE %s\n",ERR_VFS_UNMOUNT_02);// Cannot write to file
		return;
	}
	else
	{
		
		rewind(fp);
		int l;		
		

		//printf("before unmounting\n");
		//for(l=0;l<25;l++) printf("%d..%s",l,f_desc[l].file_name);
		// initializing the f_desc and fd_index to null values 


		//for(i=0;i<1024;i++)
		//{

		//	if(fd_index[i]!=0)
		//	printf("----> suhas dev ----> %s\n",f_desc[i].file_name);
		//}


		for(i=0;i<1024;i++)
		{

			clearFD(i);
			//fd_index[i]=0;
			//strcpy(f_desc[i].file_path,"");
			//strcpy(f_desc[i].file_name,"");
			//strcpy(f_desc[i].file_type,"");
			//f_desc[i].file_size=0;
			//f_desc[i].location_block_number=0;
			//f_desc[i].file_descriptor_index=0;
		}
		
		//printf("after  clearing the file descriptor array\n");		
		
		//for(l=0;l<25;l++) printf("%d .. %s\n",l,f_desc[l].file_name);

	
		
	
		unmount_filesystem();
		

		//printf("after unmounting the file descriptor array\n");
		
		//for(l=0;l<25;l++) printf("%d .. %s",l,f_desc[l].file_name);
	
		fseek(fp,0,SEEK_SET);
		fwrite(&mheader,sizeof(struct mainheader),1,fp);
		fwrite(&f_desc,(sizeof(struct file_descriptor))*(mheader.max_file_desc),1,fp);
		fwrite(&free_list,(sizeof(int))*(mheader.max_file_desc),1,fp);
		fwrite(&fd_index,(sizeof(int))*(mheader.max_file_desc),1,fp);
	}
	unmountNaryRoot();
	unmountHash();
	unmountBSTRoot();
	fclose(fp);
	mount_status=0;

	printf("%s\n","unmountvfs_SUCCESS");

}

