//////////////////////////////////////////////////////
//
//  Header Files Inclusion
//
//////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdbool.h>

//////////////////////////////////////////////////////
//
//  User Defined Macros
//
//////////////////////////////////////////////////////

#define MAXINODE 5
#define MAXFILESIZE 50
#define MAXOPENFILES 5

#define READ 1
#define WRITE 2
#define EXECUTE 4

#define START 0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCCESS 0

#define REGULARFILE 1
#define SPECIALFILE 2

//////////////////////////////////////////////////////
//
//  User Defined Macros for error handling
//
//////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8

//////////////////////////////////////////////////////
//
//  Structure Name :  BootBlock
//  Description :     It holds the imformation to 
//                    boot the operating System
//
//////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];           // dummy structure coz booting is not gonna happen in this project coz project is running on RAM
};

//////////////////////////////////////////////////////
//
//  Structure Name : SuperBlock
//  Description :    It holds information of complete 
//                   file system
//
//////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeTnodes;
};

//////////////////////////////////////////////////////
//
//  Structure Name : Inode 
//  Description :    It holds information of file
//
//////////////////////////////////////////////////////

#pragma pack(1)
struct Inode
{
    char FileName[20];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;
    char *Buffer;
    struct Inode *next;
};

typedef struct Inode INODE;
typedef struct Inode * PINODE;
typedef struct Inode ** PPINODE;

//////////////////////////////////////////////////////
//
//  Structure Name : FileTable
//  Description :    It holds information of opened 
//                   files
//
//////////////////////////////////////////////////////

#pragma pack(1)
struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;
};

typedef struct FileTable FILETABLE;
typedef struct FileTable * PFILETABLE;

//////////////////////////////////////////////////////
//
//  Structure Name : UAREA
//  Description :    It holds information of process
//
//////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
};

//////////////////////////////////////////////////////
//
//  Global variables used in the project
//
//////////////////////////////////////////////////////

struct BootBlock bootobj;
struct SuperBlock superobj;
struct UAREA uareaobj;

PINODE head = NULL;

//////////////////////////////////////////////////////
//
//  Function Name :   InitialiseUAREA()
//  Description :     It is used to initialize UAREA
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            31/07/2026
//
//////////////////////////////////////////////////////

void InitialiseUAREA()
{
    int i = 0;
    strcpy(uareaobj.ProcessName,"Myexe");

    for(i = 0; i < MAXOPENFILES; i++)
    {
        uareaobj.UFDT[i] = NULL;
    }

    printf("Marvellous CVSF : UAREA gets initialised successfully\n");
}

///////////////////////////////////////////////////////////
//
//  Function Name :   InitialiseSuperBlock()
//  Description :     It is used to initialize SuperBlock
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            31/07/2026
//
///////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeTnodes = MAXINODE;

    printf("Marvellous CVSF : Super Block gets initialised successfully\n");
}

/////////////////////////////////////////////////////////////////
//
//  Function Name :   CreateDILB()
//  Description :     It is used to create linked list of inodes
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            31/07/2026
//
/////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 0;

    PINODE temp = NULL;
    PINODE newn = NULL;

    temp = head;

    for(i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        newn->InodeNumber = i;
        strcpy(newn->FileName,"\0");
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->FileType = 0;
        newn->ReferenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp->next = newn;
            temp = temp->next;
        }       
    }

    printf("Marvellous CVSF : DILB gets created successfully\n");
}

/////////////////////////////////////////////////////////////////
//
//  Function Name :   StartAuxillaryDataInitialisation()
//  Description :     It is used to call on such functions which
//                    are used to initialised auxillary data
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            31/07/2026
//
/////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitialisation()
{   // sequence matters 
    strcpy(bootobj.Information,"Booting Process of Marvellous CVFS is completed");

    printf("%s\n",bootobj.Information);

    InitialiseUAREA();

    InitialiseSuperBlock();

    CreateDILB();
}

/////////////////////////////////////////////////////////////////
//
//  Function Name :   DisplayHelp()
//  Description :     It is used to display help to the user of 
//                    project
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            01/08/2026
//
/////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("---------------------------------------------------------------------\n");
    printf("--------------------- Marvellous CVFS Help Page ---------------------\n");
    printf("---------------------------------------------------------------------\n");

    printf("man    : It is used to display the manual page\n");
    printf("clear  : It is used to clear the terminal screen\n");
    printf("creat  : It is used to create new regualar file\n");
    printf("write  : it is used to write the data into the file\n");
    printf("read   : It is used to read the data from file\n");
    printf("open   : it is used to open the file\n");
    printf("stat   : It is used to display statistical information of file\n");
    printf("inlink : It is used to delete the file\n");
    printf("exit   : It is used to terminate the Marvellous CVFS\n");

     printf("---------------------------------------------------------------------\n");
}

/////////////////////////////////////////////////////////////////
//
//  Function Name :   ManPageDisplay()
//  Description :     It is used to display man page of specific 
//                    command
//  Input :           Name of command
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            01/08/2026
//
/////////////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    if(strcmp(Name,"exit") == 0)
    {
        printf("About : It is used to terminate the project\n");
        printf("Usage : exit\n");
    }
    else if(strcmp(Name,"ls") == 0)
    {
        printf("About : It is used to list all files from current directory\n");
        printf("Usage : ls\n");
    }
    else if(strcmp(Name,"clear") == 0)
    {
        printf("About : It is used to clear the terminal\n");
        printf("Usage : ls\n");
    }
    else if(strcmp(Name,"creat") == 0)
    {
        printf("About : It is used to create new filel\n");
        printf("Usage : creat File_name Permission\n");
        printf("File_Name : Name of file that we want to create");
        printf("Permission : Permission of new file\n");
        printf("Permission : Read -> 1\n");
        printf("Permission : Write -> 2\n");
        printf("Permission : Read + Write -> 3\n");
    }
    else if(strcmp(Name,"unlink") == 0)
    {
        printf("About : It is used to delete existing filel\n");
        printf("Usage : unlink File_name\n");
        
        printf("File_Name : Name of file that we want to delete");
    }
    else if(strcmp(Name,"stat") == 0)
    {
        printf("About : It is used to get information of file\n");
        printf("Usage : stat File_name\n");

        printf("File_Name : Name of file whose information should be fetched");
    }
    else
    {
        printf("No manual entry found of %s\n",Name);
    }
}

/////////////////////////////////////////////////////////////////
//
//  Function Name :   IsFileExist()
//  Description :     It is used to check whether the file is 
//                    present or not 
//  Input :           Name of file
//  Output :          True if present
//                    False if not present
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            01/08/2026
//
/////////////////////////////////////////////////////////////////

bool IsFileExist(
                    char name[]          // Name of file
               )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if(strcmp(temp->FileName,name) == 0)
        {
            bFlag = true;
            break;
        }

        temp = temp->next;
    }
    return bFlag;
}

/////////////////////////////////////////////////////////////////
//
//  Function Name :   CreateFile()
//  Description :     It is used to create new file 
//  Input :           Name of file & Permissions
//  Output :          File Descriptor
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            01/08/2026
//
/////////////////////////////////////////////////////////////////

int CreateFile(
                   char name[],        // Name of file
                   int permission      // File permission
              )
{
    int i = 0;

    PINODE temp = head;

    if(superobj.FreeTnodes == 0)
    {
        return ERR_NO_INODES;
    }
    
    // If permission value is wrong 
    // Permission = 1 -> READ
    // Permission = 2 -> WRITE
    // Permission = 3 -> READ + WRITE

    if(permission < 1 || permission > 3)                // filter
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    // Serach for empty inode

    while(temp != NULL)
    {
        if(temp->FileType == 0)
        {
            break;
        }

        temp = temp->next;
    }

    // Rare case
    if(temp == NULL)
    {
        return ERR_NO_INODES;
    }

    // Search empty UFDT entry
    // Reserved first 3 FD
    for(i = 3; i < MAXINODE ; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate memory for file table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Initialise File Table
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = permission;

    // Connect File Tbale with inode
    uareaobj.UFDT[i]->ptrinode = temp;

    // Initialise all members of inode
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName,name);

    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;

    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;

    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;

    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;

    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    // Allocate memory for files data (Data Block)

    uareaobj.UFDT[i]->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);

    superobj.FreeTnodes--;

    return i;
}

/////////////////////////////////////////////////////////////////
//
//  Function Name :   LsFile()
//  Description :     It is used to display names of all files 
//  Input :           None
//  Output :          None
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            01/08/2026
//
/////////////////////////////////////////////////////////////////

void LsFile()
{
    PINODE temp = head;

    printf("-------------------------------------------------------------------\n");
    printf("---------------- Marvellous CVFS Files Information ----------------\n");
    printf("-------------------------------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%s\n",temp->FileName);
        }

        temp = temp->next;
    }
}

/////////////////////////////////////////////////////////////////
//
//  Function Name :   LsFile_All()
//  Description :     It is used to display all details of 
//                    all files 
//  Input :           None
//  Output :          None
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            02/08/2026
//
/////////////////////////////////////////////////////////////////

void LsFile_All()
{
    PINODE temp = head;

    printf("-------------------------------------------------------------------\n");
    printf("---------------- Marvellous CVFS Files Information ----------------\n");
    printf("-------------------------------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%s %d %d\n",temp->FileName, temp->InodeNumber, temp->ActualFileSize);
        }

        temp = temp->next;
    }
}

/////////////////////////////////////////////////////////////////
//
//  Function Name :   stat_file()
//  Description :     It is used to display all details of 
//                    specific files 
//  Input :           File Name
//  Output :          Exit status of function
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            02/08/2026
//
/////////////////////////////////////////////////////////////////

int stat_file(char name[])
{
    PINODE temp = NULL;
    int Permission = 0;
    int Type = 0;

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    temp = head;

    while(temp != NULL)
    {
        if(strcmp(temp->FileName, name) == 0)
        {
            printf("-----------------------------------------------------------------\n");
            printf("---------------- Statistical Information of File ----------------\n");
            printf("-----------------------------------------------------------------\n");

            printf("File Name : %s\n",temp->FileName);

            printf("Inode number : %d\n", temp->InodeNumber);

            printf("File Size : %d\n", temp->FileSize);

            printf("Actual File Size : %d\n", temp->ActualFileSize);

            printf("Reference Count : %d\n", temp->ReferenceCount);

            Permission = temp->Permission;

            if(Permission == READ)
            {
                printf("File Permission : Read Only\n");
            }
            else if(Permission == WRITE)
            {
                printf("File Permission : Write\n");
            }
            else if(Permission == READ + WRITE)
            {
                printf("File Permission : Read + Write\n");
            }

            Type = temp->FileType;

            if(Type == REGULARFILE)
            {
                printf("File Type : Regular File\n");
            }
            else if(Type == SPECIALFILE)
            {
                printf("File Type : Special File\n");
            }
            
            printf("-----------------------------------------------------------------\n");

            break;
        }

        temp = temp->next;
    }
    return EXECUTE_SUCCESS;
}

/////////////////////////////////////////////////////////////////
//
//  Function Name :   unlink_file()
//  Description :     It is used to delete the specific file
//  Input :           File Name
//  Output :          Exit status of function
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            02/08/2026
//
/////////////////////////////////////////////////////////////////

int unlink_file(
                    char name[]             // name of file
               )
{
    int i = 0;

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }
    
    // Travel the UFDT
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName,name) == 0)
            {
                // Deallocate memory of Buffer
                free(uareaobj.UFDT[i]->ptrinode->Buffer);

                uareaobj.UFDT[i]->ptrinode->Buffer = NULL; // IMPORTANT, tell buffer it is empty

                strcpy(uareaobj.UFDT[i]->ptrinode->FileName,"\0");

                uareaobj.UFDT[i]->ptrinode->FileSize = 0;

                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;

                uareaobj.UFDT[i]->ptrinode->FileType = 0;

                uareaobj.UFDT[i]->ptrinode->ReferenceCount = 0;

                uareaobj.UFDT[i]->ptrinode->Permission = 0;

                // Deallocate memory of filetable

                free(uareaobj.UFDT[i]);

                uareaobj.UFDT[i] = NULL;  // IMPORTANT, tell FileTable it is empty

                superobj.FreeTnodes++;

                break;  // IMPORTANT
            }
        } // End of if
    } // End of for
    
    return EXECUTE_SUCCESS;

}// End of unlink_file()

/////////////////////////////////////////////////////////////////
//
//  Function Name :   write_file()
//  Description :     It is used to write the data into 
//                    specific file
//  Input :           File Descriptor,
//                    Data that we want to write,
//                    Size of data
//  Output :          Number of bytes successfully written
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            02/08/2026
//
/////////////////////////////////////////////////////////////////

int write_file(
                    int fd,
                    char * data,
                    int size
              )
{
    int offset = 0;

    printf("File Descriptor : %d\n",fd);
    printf("Data that we want to write : %s\n",data);
    printf("Size of data : %d\n",size);

    // if fd is invalid
    if(fd < 0 || fd > MAXOPENFILES)      // filter
    {
        return ERR_INVALID_PARAMETER;
    }

    // If writting permission is not there
    if(uareaobj.UFDT[fd]->ptrinode->Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Check the space is there or not
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    //offset = uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->WriteOffset;

    // Actual data writting
    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->WriteOffset, data, size);

    // Update the write offset
    uareaobj.UFDT[fd]->WriteOffset = uareaobj.UFDT[fd]->WriteOffset + size;

    // Update actual file size
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = uareaobj.UFDT[fd]->ptrinode->ActualFileSize + size;

    return size;
}

/////////////////////////////////////////////////////////////////
//
//  Function Name :   read_file()
//  Description :     It is used to read the data from 
//                    specific file
//  Input :           File Descriptor,
//                    Address of empty buffer,
//                    Size of data
//  Output :          Number of bytes successfully read
//  Author :          Shaikh Tarannum Jakirhusen
//  Date :            02/08/2026
//
/////////////////////////////////////////////////////////////////

int read_file(
                int fd,
                char *data,
                int size
             )
{
    // invalid fd
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }
    
    if(size < 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Filter for permission
    if(uareaobj.UFDT[fd]->ptrinode->Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insufficient data
    if((MAXFILESIZE - uareaobj.UFDT[fd]->ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    // Read the data

    strncpy(data,uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->ReadOffset,size);

    uareaobj.UFDT[fd]->ReadOffset = uareaobj.UFDT[fd]->ReadOffset + size;

    return size;
}

////////////////////////////////////////////////////////////////
//
//  Entry Point function of the CVFS project
//
////////////////////////////////////////////////////////////////


int main()
{
    // Input Command
    char str[80] = {'\0'};

    // Tokenised command
    char Command[5][20] = {{'\0'}};

    // Data for write system call
    char InputBuffer[MAXFILESIZE] = {'\0'};

    int iCount = 0, iRet = 0;

    int size = 0;

    char *EmptyBuffer = NULL;

    StartAuxillaryDataInitialisation();

    printf("-----------------------------------------------------------------\n");
    printf("-------------- Marvellous CVFS Started Successfully -------------\n");
    printf("-----------------------------------------------------------------\n");

    // Infinite Listening Shell
    while(1)
    {
        fflush(stdin);                      // before use old input buffer washed out - fflush

        strcpy(str,"");                    // old data from str is cleared

        printf("\nMarvellous CVFS : > ");
        fgets(str,sizeof(str),stdin);

        iCount = sscanf(str,"%s %s %s %s %s",Command[0],Command[1],Command[2],Command[3],Command[4]);       // 5 %s coz we have taken [5][20]
        
        fflush(stdin);                     // after use flushed data

        //Commandx = strlwr(Command[0]);

        if(iCount == 1)
        {
            // Marvellous CVFS : > exit
            if(strcmp(Command[0],"exit") == 0)
            {
                printf("Thank you for using Marvellous CVFS\n");
                printf("Deallocating all the resources of Marvellous\n");

                break;
            }
            // Marvellous CVFS : > help
            else if(strcmp(Command[0],"help") == 0)
            {
                DisplayHelp();
            }
            // Marvellous CVFS : > clear
            else if(strcmp(Command[0],"clear") == 0)
            {
                #ifdef _WIN32                     // WIN32 - if your platform is windows then it will come in this if , otherwise it will go in #else
                    system("cls");
                #else
                    system("clear");
                #endif
            }
            // Marvellous CVFS : > ls
            else if(strcmp(Command[0],"ls") == 0)
            {
               LsFile();
            }
            else
            {
                printf("Command not found\n");
                printf("Please refer help option to get more information\n");
                printf("Please refer manual page of command using man\n");
            }
        }
        else if(iCount == 2)
        {
            // Marvellous CVFS : > man open
            if(strcmp(Command[0],"man") == 0)
            {
                ManPageDisplay(Command[1]);
            }
             // Marvellous CVFS : > ls -a   -> for all 
            else if((strcmp(Command[0],"ls") == 0) && (strcmp(Command[1],"-a") == 0))
            {
                LsFile_All();
            }
            // Marvellous CVFS : > stat Ganesh.txt
            else if(strcmp(Command[0],"stat") == 0)
            {
                iRet = stat_file(Command[1]);

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File not exist\n");
                }
            }
            // Marvellous CVFS : > unlink Ganesh.txt
            else if(strcmp(Command[0],"unlink") == 0)
            {
                iRet = unlink_file(Command[1]);

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File not exist\n");
                }
            }
            // Marvellous CVFS : > write 1      -> 1 is fd
            else if(strcmp(Command[0],"write") == 0)
            {
                printf("Enter the data that you want to write into the file\n");

                fgets(InputBuffer,MAXFILESIZE,stdin);

                size = strlen(InputBuffer);

                iRet = write_file(atoi(Command[1]), InputBuffer, size-1);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameters\n");

                    printf("Please refer man page for more information\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File not exist\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : There is no permission to write the data\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : There is no space to write the data\n");
                }
                else
                {
                    printf("%d Bytes gets successfully written into the file\n",iRet);
                }
            }
            else
            {
                printf("Command not found\n");
                printf("Please refer help option to get more information\n");
                printf("Please refer manual page of command using man\n");
            }
        }
        else if(iCount == 3)
        {
            // Marvellous CVFS : > create Ganesh.txt 3
            if(strcmp(Command[0],"creat") == 0)
            {
                iRet = CreateFile(Command[1], atoi(Command[2]));

                if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because there is no free inode \n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because parameters of command are invalid\n");
                    printf("Please use man page to get actual parameters\n");
                }
                else if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because the file name is already present\n");
                    printf("Please use ls command to check names of all files\n");
                }
                else if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because the UFDT is full\n");
                    printf("Please close some opened files\n");
                }
                else
                {
                    printf("File Successfully Created with FD : %d\n",iRet);
                }
            }
            // Marvellous CVFS : > read 3 10 
            else if(strcmp(Command[0],"read") == 0)
            {
                EmptyBuffer = (char*)malloc(atoi(Command[2]));

                iRet = read_file(atoi(Command[1]), EmptyBuffer, atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameter\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File not Exist\n");
                }
                else if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("Error : Insufficient data\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Permission denied\n");
                }
                else
                {
                    printf("Read operation is successful\n");

                    printf("Data from file is : \n");

                    printf("%s\n",EmptyBuffer);
                }
            }
            else
            {
                printf("Command not found\n");
                printf("Please refer help option to get more information\n");
                printf("Please refer manual page of command using man\n");
            }
        }
        else if(iCount == 4)
        {
            
        }
        else
        {
            printf("Command not found\n");
            printf("Please refer help option to get more information\n");
            printf("Please refer manual page of command using man\n");
        }

    } // End of while 

    return 0;
}