#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "./../include/allheaders.h"


struct node *hashTable[27]={NULL};

void unmountHash()
{	
	int i;
	for(i=0;i<27;i++)
	hashTable[i]=NULL;
}


int findIndex(char fname[])
{	
	char a=toupper(fname[0]);
	int firstAlphabetASCII=a;
	int hashTableIndex;
	if(firstAlphabetASCII>=65&&firstAlphabetASCII<=90)
		hashTableIndex=firstAlphabetASCII-65;
	else 
		hashTableIndex=26;

	return hashTableIndex;

}




void insertToHash(struct file_descriptor f_desc)
{			
	int hashTableIndex=findIndex(f_desc.file_name);
	struct node *temp;
	temp=(struct node*)malloc(sizeof(struct node));
	temp->f_desc=f_desc;
	temp->next=NULL;
	temp->previous=NULL;
	
		if(hashTable[hashTableIndex]!=NULL)
	 		temp->next=hashTable[hashTableIndex];
		
		hashTable[hashTableIndex]=temp; 
		
}




void displayHash()
{
	//printf("\n ****************** HashTable : ********************************\n");	
	struct node *ptr;
	int i=0;
	while(i<27){	
		ptr=hashTable[i];
		//printf("%d\t",i);
		if(ptr!=NULL){

			while(ptr->next!=NULL){
				//printf("%s and path : %s\t",ptr->f_desc.file_name,ptr->f_desc.file_path);
				//printf("--->");
				ptr=ptr->next;
			}

		//printf("%s and path : %s\t",ptr->f_desc.file_name,ptr->f_desc.file_path);
		}
		//printf("\n");
		i++;
		}
	//printf("\n");	
}






// code for deletion in hash table ---------------------->>>>>>>>>>>>>>>>>
// added on 5th Nov 2012 - Suhas 


void deleteHash(struct file_descriptor fd)
{	
	char fname[200];
	char path[200];

	strcpy(fname,fd.file_name);
	strcpy(path,fd.file_path);
	int hashTableIndex=findIndex(fname);
	struct node *ptr,*ptrp;
   	ptr=hashTable[hashTableIndex];
  	ptrp=hashTable[hashTableIndex];



	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n ***** B4 Deletion in hash  ***** \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

		if(strcmp(ptr->f_desc.file_name,fname)==0 && strcmp(ptr->f_desc.file_path,path)==0) 		  
		{
			hashTable[hashTableIndex]=ptr->next;


			//printf("\n\n\n\n\n\n\n\ninside if of hash \n\n\n\n\n\n\n\n");
		}
		else{
			ptr=ptr->next;

			//printf("\n\n\n\n\n\n\n\ninside else of hash \n\n\n\n\n\n\n\n");

			while(ptr != NULL){

			//printf("\n\n\n\n\n\n\n\ninside while of hash \n\n\n\n\n\n\n\n");
				if(strcmp(ptr->f_desc.file_name,fname)==0 && strcmp(ptr->f_desc.file_path,path)==0)
				{
					ptrp->next=ptr->next;

					//printf("\n\n\n\n\n\n\n\ninside if of while in hash \n\n\n\n\n\n\n\n");

				}				
    				ptrp=ptr;
      				ptr=ptr->next;   
  			}
		}

	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n ***** Deletion in hash completed ***** \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");


}




//char *listPaths[100];  int listPathsIndex;
void searchPaths(char fname[]){
	int hashTableIndex=findIndex(fname);
	char path[200];
	struct node *temp=hashTable[hashTableIndex];


	//printf("\n\n\n\n\n\n\n\n  b4 searhc paths hash ---- >>>>> \n\n\n\n\n\n\n\n\n");



	while(temp!=NULL)
	{
		if(strcmp(temp->f_desc.file_name,fname)==0)
		{	
			
		//	strcpy(path,strcat("name = ",strcat(temp->f_desc.file_name,strcat(" and path= ",temp->f_desc.file_path))));
			listPaths[listPathsIndex]=temp->f_desc.file_path;
			listPathsIndex++;		
		}
		temp=temp->next;
	}

	//printf("\n\n\n\\nn\n\n\\n\n\n\\n\n\n\n\n\n Done search hash ..... **** \n\n\n\n\n\n\n\n\n\n\n\n");

}








void deleteHashWithPath(char path1[])
{

		//printf("\n\n\npath inside dele hash -------> %s\n\n\n\n",path1);	
	char *tokens[100];
	int count=0;
	tokens[count]=strtok(path1,"/");




	while(tokens[count]!=NULL)
     	{ 
           count++;
           tokens[count]=strtok(NULL,"/");//storing the tokens in the array of char pointers pointing to each token
           //printf("-------> > > Token : %s , count : %d",tokens[count],count);
		
      	}

	
	char fname[200];
	char path[200];
	int i;
	strcpy(path,tokens[0]);
	for(i=1;i<=count-2;i++)
	{
		strcat(path,"/");	
		strcat(path,tokens[i]);
	
	}

	//printf(" \n\n\n\n ------> tokenized path : %s",path);


	strcpy(fname,tokens[count-1]);

	//printf("\n\n\n --------> file name : %s ",fname);

	int hashTableIndex=findIndex(fname);

	struct node *ptr,*ptrp;
   	ptr=hashTable[hashTableIndex];
  	ptrp=hashTable[hashTableIndex];



	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n ***** B4 Deletion in hash  ***** \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

		if(strcmp(ptr->f_desc.file_name,fname)==0 && strcmp(ptr->f_desc.file_path,path)==0) 		  
		{
			hashTable[hashTableIndex]=ptr->next;


			//printf("\n\n\n\n\n\n\n\ninside if of hash \n\n\n\n\n\n\n\n");
		}
		else{
			ptr=ptr->next;

			//printf("\n\n\n\n\n\n\n\ninside else of hash \n\n\n\n\n\n\n\n");

			while(ptr != NULL){

			//printf("\n\n\n\n\n\n\n\ninside while of hash \n\n\n\n\n\n\n\n");
				if(strcmp(ptr->f_desc.file_name,fname)==0 && strcmp(ptr->f_desc.file_path,path)==0)
				{
					ptrp->next=ptr->next;

					//printf("\n\n\n\n\n\n\n\ninside if of while in hash \n\n\n\n\n\n\n\n");

				}				
    				ptrp=ptr;
      				ptr=ptr->next;   
  			}
		}

	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n ***** Deletion in hash completed ***** \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");


}
