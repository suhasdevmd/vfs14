#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "./../include/allheaders.h"


struct dirNode *root=NULL;
int k=0;

void displayNaryForSearch(struct dirNode *d,char substring[]);


struct dirNode * getnode()//function to create a node of narytree
{	//printf("getting node\n");
   struct dirNode* temp;
   temp=(struct dirNode*)malloc(sizeof(struct dirNode));
   temp->firstChild==NULL;
   temp->rightSibling==NULL;
	//printf("got narry node\n");
   return temp;
	
}

void unmountNaryRoot(){
root=NULL;
}


void naryRoot()
{	
	//printf("inside nary root\n");
	//struct dirNode *root;
	root=getnode();
	strcpy(root->fileDesc.file_name,"ROOT");
	strcpy(root->fileDesc.file_path,"");
	strcpy(root->fileDesc.file_type,"");
	root->fileDesc.file_size=0;
	root->fileDesc.location_block_number=-1;
	//printf("passed narry root step.\n");
}

void displayNAry(struct dirNode *d)//function to display the filedescriptor names stored in the nodes of narytree in preorder manner
{

	if(d!=NULL)
	{
		printf("%s\n",d->fileDesc.file_name);
		displayNAry(d->rightSibling);
		displayNAry(d->firstChild);
	}
}          




void printreclist(struct dirNode *d)//function to display the filedescriptor names stored in the nodes of narytree in preorder manner
{

	if(d!=NULL)
	{
		listarr[listindex]=d->fileDesc.file_name;
		listindex=listindex+1;//printf("\t%s",start->fileDesc.file_name);
		//printf("%s\n",d->fileDesc.file_name);
		printreclist(d->rightSibling);
		printreclist(d->firstChild);
	}
}          




void unmountNary(struct dirNode *d)//function to store the filedescriptor names stored in the nodes of narytree to the fd array
{
	
	if(d!=NULL)
	{
		f_desc[k]=d->fileDesc;
		f_desc[k].file_descriptor_index=k;
		fd_index[k]=1;
		k++;
		unmountNary(d->rightSibling);
		unmountNary(d->firstChild);
	}
} 



void unmount_filesystem()
{

	unmountNary(root);
	k=0;
}



void insertToNarry(struct file_descriptor fd)//function to insert the nodes other than root node
{
	//printf("insetion process started\n");	
	struct dirNode *temp=NULL,*start=NULL;
  	int j,status=0,count=0,i=1,match=0,match1=0;
  	char s[100]="";	
   	char path[100];
   	char *tokens[100];
   	temp=getnode();
	temp->fileDesc=fd;
	insertToHash(fd);
   	strcpy(path,fd.file_path);//copy the filedescriptor path in the path char array
   	tokens[count]=strtok(path,"/");//breaking the path into tokens
    	start=root;

	//printf("passed allocation Step\n");

        while(tokens[count]!=NULL)
        { 
           count++;
           tokens[count]=strtok(NULL,"/");//storing the tokens in the array of char pointers pointing to each token
           
        }
    
        if(tokens[i]==NULL)//when path contains only ROOT
        {
      
    	   
       		if(start->firstChild==NULL)//checking and storing in the first child of the root if its null
               {
                 start->firstChild=temp;
                 
                
               }
               else//if the first child of the root is not null,traverse through right siblings and store if a right sibling is null
               {
                start=start->firstChild;
                while(status==0){
               if(start->rightSibling==NULL){
              
                start->rightSibling=temp;
                status=1;
               }
               else{
                start=start->rightSibling;
               }
               
              }
             
          }
               
    }

    else //when path contains ROOT/... ie it doesnot contain only the ROOT
      { 
	if(start->firstChild!=NULL){
        start=start->firstChild;
         while(i<count)//checking from token[i=1] as token[i=0] contains root and its checking done in IF statement b4.
          {
             while((strcmp(tokens[i],start->fileDesc.file_name)!=0) && start->rightSibling!=NULL){//compare d present token with d firstchild,traverse thru right
                  if(start->rightSibling==NULL){
                     match=1;match1=1;
                     break;
                     }
                    start=start->rightSibling;
                   
              }
                 if(match==1)
                  break;
                   //to go to the next token
                   if(start->firstChild!=NULL)//after the match is found go to the first child 
                  { start=start->firstChild;}
                   else if(start->firstChild==NULL && (i==count-1)){
                    start->firstChild=temp;
		temp->rightSibling=NULL;
		temp->firstChild=NULL;
                    status=1;
                    break;
                    }
                   else if(start->firstChild==NULL && (i<count-1)){
                   match1=1;
                   i++;
                  break;
                  }
                   
               
                  i++;
          }
       
  	}else
	{
	match1=1;
	}

       if(match1==1){
		if(match==1){
		start->rightSibling=getnode();
		start=start->rightSibling;
		}
		else{
		start->firstChild=getnode();
		start=start->firstChild;
		}
		strcpy(start->fileDesc.file_type,"directory");
        	start->fileDesc.file_size=0;
        	start->fileDesc.location_block_number=-1;
         	strcpy(start->fileDesc.file_name,tokens[i]);
		for(j=0;j<i-1;j++){
			strcat(s,tokens[j]);
			strcat(s,"/");
		}
		strcat(s,tokens[i-1]);

		// adding the intermediate directories to the fd array

		int index=get_file_descriptor_index();

		start->fileDesc.file_descriptor_index=index;
		fd_index[index]=1;
		f_desc[index]=start->fileDesc;

         	strcpy(start->fileDesc.file_path,s);
		
		
		int abc=insertToBst(start->fileDesc);// inserting to hash table
		if(abc==0)		
		{insertToHash(start->fileDesc);}else clearBst();
          	i++;
        	while(i<count){
           		start->firstChild=getnode();
           		start=start->firstChild;
			strcpy(start->fileDesc.file_type,"directory");
        		start->fileDesc.file_size=0;
        		start->fileDesc.location_block_number=-1;
         		strcpy(start->fileDesc.file_name,tokens[i]);
			strcpy(s,"");	
			for(j=0;j<i-1;j++){
				strcat(s,tokens[j]);
				strcat(s,"/");
			}
			strcat(s,tokens[i-1]);

			int indx=get_file_descriptor_index();

			start->fileDesc.file_descriptor_index=indx;
			fd_index[indx]=1;
			f_desc[indx]=start->fileDesc;


         		strcpy(start->fileDesc.file_path,s);
			int def=insertToBst(start->fileDesc);// inserting to hash table
			if(def==0)		
			{insertToHash(start->fileDesc);}else clearBst();

			 i++;
        	} 
			  start->firstChild=temp;
			temp->rightSibling=NULL;
			temp->firstChild=NULL;
                    // printf("\n%s",start->firstChild->fileDesc.fileName);
                     status=1;
              


	}
       
        if(status==0){
         while(start->rightSibling!=NULL)
         start=start->rightSibling;
         start->rightSibling=temp;//when all the tokens are matched we insert our new folder
	temp->rightSibling=NULL;
	temp->firstChild=NULL;
     }
       }
    //printf("insertion complete \n");

//printf("DISPLAYING NARRAY\n");
//displayNAry(root); // to check the insertion in n-ary
//displayHash();   // to check insertion in hashtable
}            
   







// code for deletion  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// added on 5th Nov 2012 - Suhas

void traverseForDeletion(struct dirNode *d)
{
    
	if(d!=NULL)
	{
      		fd_count++;
      		traverseForDeletion(d->rightSibling);
      		traverseForDeletion(d->firstChild);
   	}
}




void copy_fd(struct file_descriptor *Fdescs, struct dirNode *d,int num){
     
     	if(d!=NULL)
	{
      		strcpy(Fdescs[baseFdescarray].file_name,d->fileDesc.file_name);
      		strcpy(Fdescs[baseFdescarray].file_path,d->fileDesc.file_path);
      		strcpy(Fdescs[baseFdescarray].file_type,d->fileDesc.file_type);
      		Fdescs[baseFdescarray].location_block_number=d->fileDesc.location_block_number;
		Fdescs[baseFdescarray].file_descriptor_index=d->fileDesc.file_descriptor_index;
      		Fdescs[baseFdescarray].file_size=d->fileDesc.file_size;
      		baseFdescarray++;
      
      		if((baseFdescarray>1)||(num==1))
      		copy_fd(Fdesc,d->rightSibling,num);
      		copy_fd(Fdesc,d->firstChild,num);
   	}
}






void del_root(char path[])
{
	baseFdescarray=0;
 	fd_count=0; //count of files and folders to be deleted
	Fdesc=(struct file_descriptor *)NULL;// files to be deleted


 	struct dirNode *start,*temp;
 	start=root;
 	temp=start->firstChild;
 
	if(start->firstChild!=NULL)
	{
		traverseForDeletion(start->firstChild);
  		Fdesc=(struct file_descriptor*)malloc((fd_count)*sizeof(struct file_descriptor));
  		copy_fd(Fdesc,temp,1);
  		free(temp);
  		start->firstChild=NULL;
	}
	else;
	//printf("No files or directories are present under ROOT to be deleted\n");
}





void del_dir_file(char path[])
{


//displayNAry(root);

 
	
	
	baseFdescarray=0;
 	fd_count=0; //count of files and folders to be deleted
	Fdesc=(struct file_descriptor *)NULL;// files to be deleted

	


	struct dirNode *start,*prev,*t,*temp;
 	int count=0,i=1, j=0,k=0;
 	char *tokens[100];
 	tokens[count]=strtok(path,"/");
	start=root;
	

	while(tokens[count]!=NULL)
     	{ 
           count++;
           tokens[count]=strtok(NULL,"/");//storing the tokens in the array of char pointers pointing to each token
           
      	}
	
  
  	if(tokens[i]!=NULL)
   	{  
     
      		if(count>2) 
      			start=start->firstChild;
       
		
         	while(i<count-1)//checking from token[i=1] as token[i=0] contains root and its checking done in IF statement b4.
          	{
			
			
             		while((strcmp(tokens[i],start->fileDesc.file_name)!=0) && start->rightSibling!=NULL){//compare the present token with the firstchild,traverse thru rightsibling til a match  
			
                   	start=start->rightSibling;
                   
              		}
			
                   	if(start->firstChild!=NULL && i<(count-2))//after the match is found go to the first child 
                  	{ 
                     		start=start->firstChild;
                  	}
             		i++;
	
           	} 
        
           	prev=start;



           	temp=start->firstChild;
                

            	while((strcmp(tokens[i],temp->fileDesc.file_name)!=0) && temp->rightSibling!=NULL){//compare the present token with the firstchild,traverse thru rightsibling til a match 
      
               

                 k++;
                prev=temp;

                temp=temp->rightSibling;
                   
             }
            
    
           t=temp;
           
           
           if(t->firstChild!=NULL)
           	traverseForDeletion(t->firstChild);
           if(t!=root) 
            	fd_count=fd_count+1;
           
           Fdesc=(struct file_descriptor*)malloc((fd_count)*sizeof(struct file_descriptor));
           copy_fd(Fdesc,t,0);
           if(k==0) 
		{
           prev->firstChild=temp->rightSibling;
	   }
          else
             prev->rightSibling=temp->rightSibling;
             free(temp);
     }
                
 	



}

//char *listarr[400];
//int listindex=0;
void list_dir_normal(char path[])//function to display the filedescriptor names stored in the nodes of narytree in preorder manner
{



 struct dirNode *start;
 int count=0,i=1, j=0;
 char *tokens[100];
 char s[200];
 char rupali[200];
 strcpy(rupali,path);
 tokens[count]=strtok(path,"/");//breaking the path into tokens








//if(strcmp(rupali,"ROOT")!=0)
//{
    start=root;
    while(tokens[count]!=NULL)
     { 
           count++;
           tokens[count]=strtok(NULL,"/");//storing the tokens in the array of char pointers pointing to each token
           
     }
//}
	//printf("TOKENS: ");
	for(j=0; j<count; j++)
	{
		//printf("\t%s",tokens[j]);
	}

	
	//printf("\n");
	//listIndex=count;
	//printf("Count: %d\n",count);
     	strcpy(s,tokens[count-1]);
	//printf("\nLast token=%s\n",s);

  	if(!strcmp(start->fileDesc.file_name,s))
   	{
     		start=start->firstChild;
		//printf("List: ");
     		while(start!=NULL)
		{
       		 	
			listarr[listindex]=start->fileDesc.file_name;
			listindex=listindex+1;//printf("\t%s",start->fileDesc.file_name);
       		  	start=start->rightSibling;
       		}
		//printf("\n");
	
  	}
  	else
   	{
	
     		start=start->firstChild;
	
     	///printf("%s",start->fileDesc.fileName);
	
        while(i<count-1)//checking from token[i=1] as token[i=0] contains root and its checking done in IF statement b4.
        {
		
             while((strcmp(tokens[i],start->fileDesc.file_name)!=0) && start->rightSibling!=NULL){//compare the present token with the firstchild,traverse thru rightsibling til a match  
			
                   start=start->rightSibling;
                   
              }
                   if(start->firstChild!=NULL)//after the match is found go to the first child 
                  { 
                     start=start->firstChild;
                  }
             i++;
         }
         while(strcmp(start->fileDesc.file_name,s) && start->rightSibling!=NULL)
            start=start->rightSibling;
          if(start->firstChild==NULL);
           //printf("No files or Directories present\n");
          else
            {
               start=start->firstChild;
		//printf("List: ");
               do{
         
		listarr[listindex]=start->fileDesc.file_name;
			listindex=listindex+1;//printf("\t%s",start->fileDesc.file_name);

		//printf("\t%s",start->fileDesc.file_name);
         start=start->rightSibling;
                 }while(start!=NULL);
		//printf("\n");
           }
  	}
}



int list_dir_for_deletion(char path[])//function to display the filedescriptor names stored in the nodes of narytree in preorder manner
{
 struct dirNode *start;
 int count=0,i=1, j=0;
 char *tokens[100];
 char s[200];
 tokens[count]=strtok(path,"/");//breaking the path into tokens
    start=root;
    while(tokens[count]!=NULL)
     { 
           count++;
           tokens[count]=strtok(NULL,"/");//storing the tokens in the array of char pointers pointing to each token
           
      }
	//printf("TOKENS: ");
	for(j=0; j<count; j++)
	{
		//printf("\t%s",tokens[j]);
	}
	//printf("\n");
	//listindex=count;
	//printf("Count: %d\n",count);
     	strcpy(s,tokens[count-1]);
	//printf("\nLast token=%s\n",s);
  if(!strcmp(start->fileDesc.file_name,s))
   {
	start=start->firstChild;
	//printf("List: ");
     	while(start!=NULL)
	{
        	//printf("\t%s",start->fileDesc.file_name);
        	start=start->rightSibling;
      	}
	//printf("\n");
	
  }
  else
   {
	
     start=start->firstChild;
	
     ///printf("%s",start->fileDesc.fileName);
	
         while(i<count-1)//checking from token[i=1] as token[i=0] contains root and its checking done in IF statement b4.
          {
		
             while((strcmp(tokens[i],start->fileDesc.file_name)!=0) && start->rightSibling!=NULL){//compare the present token with the firstchild,traverse thru rightsibling til a match  
			
                   start=start->rightSibling;
                   
              }
              
		if(start->firstChild!=NULL)//after the match is found go to the first child 
                  { 
                     start=start->firstChild;
                  }
             	i++;
          }
         
	while(strcmp(start->fileDesc.file_name,s) && start->rightSibling!=NULL)
            start=start->rightSibling;
          if(start->firstChild==NULL)
           	return 1;  
          else{
 	  start=start->firstChild;
		//printf("List: ");
               do{
 	        //printf("\t%s",start->fileDesc.file_name);
 	        start=start->rightSibling;
                 }while(start!=NULL);
		//printf("\n");
        
          	return 0; 
  }}
return 1;
}



                       
void recursive_list(char path[])
{
   struct dirNode *start;
 int count=0,i=1, j=0;
 char *tokens[100];
 char s[200];
 tokens[count]=strtok(path,"/");//breaking the path into tokens
    start=root;
    while(tokens[count]!=NULL)
     { 
           count++;
           tokens[count]=strtok(NULL,"/");//storing the tokens in the array of char pointers pointing to each token
           
      }
	//printf("TOKENS: ");
	for(j=0; j<count; j++)
	{
		//printf("\t%s",tokens[j]);
	}
	//printf("\n");
	//printf("Count: %d\n",count);
     	strcpy(s,tokens[count-1]);
	//printf("\nLast token=%s\n",s);
  if(!strcmp(start->fileDesc.file_name,s))
   {
	
     start=start->firstChild;
     //printf("LIST : ");
     printreclist(start);
   }
else
   {
	
     start=start->firstChild;
     	
     ///printf("%s",start->fileDesc.fileName);
	
         while(i<count-1)//checking from token[i=1] as token[i=0] contains root and its checking done in IF statement b4.
          {
		
             while((strcmp(tokens[i],start->fileDesc.file_name)!=0) && start->rightSibling!=NULL){//compare the present token with the firstchild,traverse thru rightsibling til a match  
			
                   start=start->rightSibling;
                   
              }
                   if(start->firstChild!=NULL)//after the match is found go to the first child 
                  { 
                     start=start->firstChild;
                  }
             i++;
           }
         while(strcmp(start->fileDesc.file_name,s) && start->rightSibling!=NULL)
            start=start->rightSibling;
          if(start->firstChild==NULL);
           //printf("No files or Directories present\n");
          else
            {
               start=start->firstChild;
		//printf("List: ");
                printreclist(start);
                
           }
      
        }
}

// new code for make directory added at 11 ,6th nov.

char* getNewPath(int count,char oldPath[] ,char *apendPath)
{
	int num,i=0,j=0;
        char *token,*temp=NULL,*copy=NULL;
        int tokenLen;
       //printf("****");
        char temppath[400];
        num=count;
	copy=oldPath;
        //puts(copy);
	token = strtok(oldPath,"/");
        
       while(i<num){
        if(i>0)
        token=strtok(copy,"/");
	tokenLen = strlen(token);
	copy = copy + (tokenLen + 1);
        //puts(copy);
        
       // puts(token);
        i++;
        }
        //puts(copy);
        //printf("***\n");
        strcpy(temppath,apendPath);
        //puts(apendPath);
	//strcpy(temppath,apendPath);
        strcat(temppath,"/");
        strcat(temppath,copy);
       // printf("\n*****\n");
        //puts(temppath);
        //printf("******\n");
        temp=temppath;
  

  return temp;
}


//char *listNames[100];  int listNamesIndex; 
void searchSubstringFile(char substring[])
{
	listNamesIndex=0;
	*listNames=NULL;
	displayNaryForSearch(root,substring);
	


}

void listDirRoot()
{
 	struct dirNode *start;
     	start=root;
	if(start->firstChild==NULL);
	//printf("nothing inside root");
    	else{
		start=start->firstChild;      
         	while(start->rightSibling!=NULL)
		{		
			listarr[listindex]=start->fileDesc.file_name;
			listindex=listindex+1;		
			start=start->rightSibling;

		}
		listarr[listindex]=start->fileDesc.file_name;
			listindex=listindex+1;	
  
	}

}




void displayNaryForSearch(struct dirNode *d,char substring[])
{
	if(d!=NULL)
	{	

		char fileName[200];
		strcpy(fileName,d->fileDesc.file_name);
		if(d->firstChild==NULL && d->fileDesc.location_block_number!=-1)
		{
			int match=matchStrings(fileName,substring);
			if(match==1)
			{
				listNames[listNamesIndex]=d->fileDesc.file_name;
				listNamesIndex++;
			}
		}
		//printf("going to right tree\n");
		displayNaryForSearch(d->rightSibling,substring);
		//printf("going to left tree\n");

		displayNaryForSearch(d->firstChild,substring);
		//printf("done to rigth tree\n");

	}


}          




int matchStrings(char s1[], char s2[])
{
	int l1 = strlen(s1);
	int l2 = strlen(s2);
 

	int i;
 
	for(i = 0; i < l2; i++)
	{
		if(s1[i]==s2[i])
		{
			i++;
		}
		else
		{
			return 0;
		}
	}
	return 1;
}




void displayNaryMain(){

//printf("\n DISPLAYING NARY ********************************* \n");
displayNAry(root);

}








