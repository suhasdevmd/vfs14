#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "./../include/allheaders.h"




int get_file_descriptor_index(){

	int i=0;
	while(fd_index[i]!=0)
	{
		i++;
	}

	return i;
}



int getFreeListIndex(){
	int j=0;

	while(free_list[j]!=0)
	{
		j++;
	}

	return j;
}


// func to print to file


int printToFile(char *out){

	char out_file[200];
	strcpy(out_file,out);
	int i;



	FILE *fp=fopen(out_file,"w");

	if(fp==NULL)
	{
		printf("listdir_FAILURE %s\n",ERR_VFS_LISTDIR_04);// cannot create output file
		return 0;
	
	}
	else
	{

		

		//strcat(substring," found in the following locations : \n\n");
		//fwrite(substring,strlen(substring),1,fp);
		


		for(i=0;i<listindex;i++){
			//strcat(i+1,listPaths[i]);
			fwrite(listarr[i],strlen(listarr[i]),1,fp);
			fwrite("\n",1,1,fp);

		}
		fclose(fp);
	}		


	return 1;
}











void make_dir ( char *parent_path, char *dir_name)
{

	// to check if the path and directory exists
	// call insertTonarry();


	char path[200];
	char dirname[200];
	int i,bst;
	struct file_descriptor fd_dir;


	// to check if the path and directory exists
	strcpy(path,parent_path);
	strcpy(dirname,dir_name);


	if(mount_status==0)
	{
		printf("makedir_FAILURE %s\n",ERR_VFS_MAKEDIR_05);// vfs not mounted
		return;
	}


	if(strlen(path)==0 || strlen(dirname)==0)
	{
		printf("makedir_FAILURE %s\n",ERR_VFS_MAKEDIR_00);// vfs insufficient arguments
		return;
	}


	

	for(i=0;i<strlen(dirname);i++)
	{
		if(dirname[i]=='/')
		{
			printf("makedir_FAILURE %s\n",ERR_VFS_MAKEDIR_02);// invalid character in dir name
			return;
		}
	}



	

	strcpy(fd_dir.file_path,path);
	strcpy(fd_dir.file_name,dirname);

	fd_dir.file_size=0;
	strcpy(fd_dir.file_type,"directory");
	fd_dir.location_block_number=-1;
	i=get_file_descriptor_index();

	if(i>no_of_blocks)
	{
		printf("makedir_FAILURE %s\n",ERR_VFS_MAKEDIR_01);// file system full
		return;
	}


	fd_dir.file_descriptor_index=i;

	bst=insertToBst(fd_dir);

	if(bst==0)
	{
		fd_index[i]=1;
		f_desc[i]=fd_dir;
		//printf("\nNOW GOING INTO NARRAY\n");
		insertToNarry(fd_dir);
		//printf("i value in make directory : %d",i);
	}
	else 
	{
		clearBst();
		printf("makedir_FAILURE %s\n",ERR_VFS_MAKEDIR_03);// directory already exists
		return;
	}



	printf("%s\n","makedir_SUCCESS");

	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();
	
}



void delete_dir ( char *path )
{
	char dir_path[200];
	strcpy(dir_path,path);
	int i;

	



	
	if(strlen(dir_path)==0)
	{
		printf("deletedir_FAILURE %s\n",ERR_VFS_DELETEDIR_00);// insufficient arguments
		return;
	}



	if(mount_status==0)
	{
		printf("deletedir_FAILURE %s\n",ERR_VFS_DELETEDIR_04); //vfs not mounted
		return;
	}




	if(strcmp(dir_path,"ROOT/")==0)
		del_root(dir_path);
	else 
	{	
		int a=searchIfPathExists(dir_path);
		//printf("\naaaaaaaaaaaaaaa=============    %d\n",a);
		if(a==1){
			
			
			//printf("\nyes path exists\n");
			char sendPath[200];
			strcpy(sendPath,dir_path);
			if(list_dir_for_deletion(sendPath)==1)
			{
			
				del_dir_file(dir_path);//naray deletion
				//printf("\n deleted from narray major achievement\n");
				//for(i=0;i<fd_count;i++)
				//printf("\n		name :%s   and path=%s \n",Fdesc[i].file_name,Fdesc[i].file_path);
			}
			else 
			{
				printf("deletedir_FAILURE %s\n",ERR_VFS_DELETEDIR_02); // directory is not empty
				return;
			}


		}
		else 
		{	
			printf("deletedir_FAILURE %s\n",ERR_VFS_DELETEDIR_01); //cannot find specified dir
			return;
		}

	}
			
		for(i=0;i<fd_count;i++)
		{ 
			fd_index[i]=0;
			clearFD(i);
			//free_list[deleteHash(Fdesc[i].location_block_number)]=0;
			//printf("cleared %d index\n",i);
			deleteHash(Fdesc[i]);	
			deleteBST(Fdesc[i]);	
		
		}


	printf("%s\n","deletedir_SUCCESS>");

	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();


}




// char* getNewPath(int count,char oldPath[] ,char *apendPath) is to initialised globally

void move_dir ( char *source_dir_path, char *dest_dir_path )
{
	char destPath[200];
	char sendpath[200];
	strcpy(destPath,dest_dir_path);
	char sourcePath[200];
	int count=0;
	char prevpath[200];
	char path[200];
	char *tokens[200];


	strcpy(sourcePath,source_dir_path);


	//puts(destPath);


	if(strlen(sourcePath)==0 || strlen(destPath)==0)
	{
		printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_00); //Insufficient arguments
		return;
	}



	if(mount_status==0)
	{
		printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_08); //vfs not mounted
		return;
	}



	char destpath[200];
	char srcpath[200];
	strcpy(destpath,destPath);
	strcpy(srcpath,sourcePath);
	int move=invalidMove(destpath,srcpath);
	if(move==1) 
	{		
		printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_06); //cannot move parent to child
		return;
	}



	if(strcmp(destPath,sourcePath)==0)
	{ 
		
		// error code not there in errorcodes.h
		printf("movedir_FAILURE <SAME_SOURCE_AND_DEST_PATH>\n");
		return;
	}
	else{
		int i=searchIfPathExists(sourcePath);
		if(i==0)
		{
			printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_01); //cannot find specified source dir
			return;
		}		

		int j=searchIfPathExists(destPath);
		if(j==0)
		{
			printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_02); //cannot find specified dest dir
			return;
		}

		if(i==1 &&j==1){

			//printf("\n before nary deletion .******************************");
			
			struct file_descriptor *fd_src=searchAndGetFD(sourcePath);
			struct file_descriptor *fd_dst=searchAndGetFD(destPath);

			if(fd_src==NULL);
			else if(strcmp(fd_src->file_type,"file")==0)
			{
				printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_04); // source cannot be file
				return;
			}


			if(fd_dst==NULL);
			else if(strcmp(fd_dst->file_type,"file")==0)
			{
				printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_07); //destination cannot be file
				return;
			}





			//dest dir already has a source directory  ------- 

			char check_dest[200];
			strcpy(check_dest,destPath);
			strcat(check_dest,fd_src->file_name);

			struct file_descriptor *fd_chk=searchAndGetFD(check_dest);


			if(fd_chk!=NULL)
			{
				printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_05); //dest already has source dir
				return;
			}
			


			strcpy(sendpath,sourcePath);
			del_dir_file(sendpath);
			//displayNaryMain();

			//printf("\n\n\n\n \n\n\n\n\n deleted from Nary .******************************\n\n\n\n\n");

			deleteHashWithPath(sourcePath);	

			deleteB(sourcePath);
	
		
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
					////printf("\nNOW GOING INTO NARRAY\n");
					insertToNarry(Fdesc[i]);// getting inserted into hash thru narray
				}
				else clearBst();						
			}
		}

	}


	printf("%s\n","movedir_SUCCESS");
	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();
	

}




void list_dir ( char *dir_name, int flag, char *output_file_name ){

	char dirname[200];
	char outfile[200];

		if(mount_status==0)
	{
		printf("listdir_FAILURE %s\n",ERR_VFS_LISTDIR_03); //vfs not mounted
		return;
	}

	strcpy(dirname,dir_name);
	strcpy(outfile,output_file_name);
	int i;

	
	if(strlen(dirname)==0 || strlen(outfile)==0)
	{
		printf("listdir_FAILURE %s\n",ERR_VFS_LISTDIR_00); //insufficient arguments
		return;
	}

	
	
	if(strcmp(dirname,"ROOT")==0){
		listarr[400]=NULL;
		listindex=0;
		listDirRoot();
		printf("listdir_SUCCESS\n");
		return;

	}


	//printf("--------->  > > > > %s\n",dirname);
	
	struct file_descriptor *fd=searchAndGetFD(dirname);
	
	//printf("***** ---> %s\n",fd->file_type);


	if(fd==NULL)//;
	//else if(fd->location_block_number!=-1)
	{
		printf("listdir_FAILURE %s\n",ERR_VFS_LISTDIR_01); // cannot find specified path or dir
		return;
	}


	


	if(fd->location_block_number!=-1)
	{
		printf("listdir_FAILURE %s\n",ERR_VFS_LISTDIR_01); // cannot find specified path or dir
		return;
	}
	

	if(strcmp(dirname,"ROOT")==0)
		i=1;
	else i=searchIfPathExists(dirname);
	if(i==1)
	{	
		//printf("\n\n\n\n\n\n");
		//printf("list in dir\n\n\n");
		listarr[400]=NULL;
		listindex=0;
		if(flag==0)
			list_dir_normal(dirname);	
		else if (flag==1)
 			recursive_list(dirname);
		else 
		{
			printf("listdir_FAILURE %s\n",ERR_VFS_LISTDIR_02); //invalid flag 								
			return;
		}
		
		int kl=printToFile(output_file_name);
		
		if(kl==0)return;

	}
	else
	{
		printf("listdir_FAILURE %s\n",ERR_VFS_LISTDIR_01); //cannot find specified path or dir
		return;
	}


	
	printf("%s\n","listdir_SUCCESS");
	//printf("\n Displaying the contents of Data structure . \n");
	//displayNaryMain();
	//displayHash();
	//displayBSTMain();

}






int invalidMove(char destpath[],char srcpath[])
{
	char *tokensrc[100];
	char *tokendest[100];
	int countsrc=0;
	int countdest=0;
	tokensrc[countsrc]=strtok(srcpath,"/");
 	while(tokensrc[countsrc]!=NULL)
        { 
           countsrc++;
           tokensrc[countsrc]=strtok(NULL,"/");//storing the tokens in the array of char pointers pointing to each token
           
        }
	int i;
	//for(i=0;i<countsrc;i++)
	//	printf("%s",tokensrc[i]);


	tokendest[countdest]=strtok(destpath,"/");
 	while(tokendest[countdest]!=NULL)
        { 
           countdest++;
           tokendest[countdest]=strtok(NULL,"/");//storing the tokens in the array of char pointers pointing to each token
           
        }

	//for(i=0;i<countdest;i++)
	//printf("%s",tokendest[i]);



	if(countdest>countsrc)
	{
		
		for(i=0;i<countsrc;i++)
		{	
			if(strcmp(tokensrc[i],tokendest[i])==0);
			else break;	
	
		}

		if(i==countsrc)
		return 1;
	}
	return 0;

}
