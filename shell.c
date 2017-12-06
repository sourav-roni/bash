/*Name : Sourav Pal
  Roll No. : 14CS10062
  Assignment No. : 2
  Question No. : 2
  Date : 30/01/2017
*/

//including the header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>
#include <fcntl.h>

//defining macros
#define BUFFER_SIZE 10000
#define ARR_SIZE 10000
#define MAXPATH 10000

//Global Variables
static char perms_buff[300];
char pathname[MAXPATH];

//helper function
const char *get_perms(mode_t mode)
{
    char ftype = '?';

    if (S_ISREG(mode)) ftype = '-';
    if (S_ISLNK(mode)) ftype = 'l';
    if (S_ISDIR(mode)) ftype = 'd';
    if (S_ISBLK(mode)) ftype = 'b';
    if (S_ISCHR(mode)) ftype = 'c';
    if (S_ISFIFO(mode)) ftype = '|';

    sprintf(perms_buff, "%c%c%c%c%c%c%c%c%c%c %c%c%c", ftype,
        mode & S_IRUSR ? 'r' : '-',
        mode & S_IWUSR ? 'w' : '-',
        mode & S_IXUSR ? 'x' : '-',
        mode & S_IRGRP ? 'r' : '-',
        mode & S_IWGRP ? 'w' : '-',
        mode & S_IXGRP ? 'x' : '-',
        mode & S_IROTH ? 'r' : '-',
        mode & S_IWOTH ? 'w' : '-',
        mode & S_IXOTH ? 'x' : '-',
        mode & S_ISUID ? 'U' : '-',
        mode & S_ISGID ? 'G' : '-',
        mode & S_ISVTX ? 'S' : '-');

    return (const char *)perms_buff;
}

//helper function for ls
static int only (const struct dirent *unused)
{
    if(unused->d_name[0]=='.')
    {
        return 0;
    }
    else
    {
    	return 1;
    } 
}

//helper function for ls
void lsme(char* args[],int flag)
{
    int f_count,i;
    struct direct **currentfiles;
    struct stat statbuf;
    char datestring[256];
    struct passwd pwent;
    struct passwd *pwentp;
    struct group grp;
    struct group *grpt;
    struct tm time;
    char buf[1024];

    if(getcwd(pathname, sizeof(pathname))==NULL)
    {
        perror("Reriving pathname failed : ");
        return;
    }

    //f_count is used to get the number of files in the current working directory
    f_count = scandir(pathname, &currentfiles, only, alphasort);
    if(f_count > 0)
    {
        printf("total %d\n",f_count);
        for (i=0; i<f_count; ++i)
        {
            if (stat(currentfiles[i]->d_name, &statbuf) == 0)
            {
                if(flag)
                {
                    /* Print out type, permissions, and number of links. */
                    printf("%10.10s", get_perms(statbuf.st_mode));
                    printf(" %d", statbuf.st_nlink);

                    if (!getpwuid_r(statbuf.st_uid, &pwent, buf, sizeof(buf), &pwentp))
                        printf(" %s", pwent.pw_name);
                    else
                        printf(" %d", statbuf.st_uid);

                    if (!getgrgid_r (statbuf.st_gid, &grp, buf, sizeof(buf), &grpt))
                        printf(" %s", grp.gr_name);
                    else
                        printf(" %d", statbuf.st_gid);

                    /* Print size of file. */
                    printf(" %5d", (int)statbuf.st_size);

                    localtime_r(&statbuf.st_mtime, &time);
                    /* Get localized date string. */
                    strftime(datestring, sizeof(datestring), "%I:%M%p : %x", &time);

                    printf(" %s %s\n", datestring, currentfiles[i]->d_name);
                }
                else
                {
                    printf("%s\t", currentfiles[i]->d_name);
                }
            }

            free (currentfiles[i]);
        }

        free(currentfiles);
        printf("\n");
    }
}

void copyit(char * file1, char * file2)
{
	/*char new;
	//FILE * f1;
	//FILE * f2;
	int ipfd, opfd;
	int rone, wtwo;
	//printf("I am here\n");
	char temp[100];
	ipfd = open(file1, O_RDONLY);
   	if (ipfd < 0) 
   	{
    	perror("Opening file1 in read mode failed : ");
      	return;
   	} 
   	else
   	{
   		opfd = open(file2, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	   	if (opfd < 0) 
	   	{
	    	perror("Opening file2 in write mode failed : ");
	      	return;
	   	} 
	   	else
	   	{
	   		int copying = 1;
	   		while(copying)
	   		{
	   			rone = read(ipfd,temp,sizeof(temp));
		   		if(rone>0)
		   		{
		   			wtwo = write(opfd,temp,sizeof(rone));
		   			if (wtwo<0)
		   			{
		   				copying = 0;
		   				perror("Writing failed :");
		   			}
		   		}
		   		else
		   		{

		   			copying = 0;
		   			//perror("Read failed : ");
		   		}
	   		}
	   	}
   	}*/
   	char ch;
    FILE* f1,*f2;
    f1 = fopen(file1,"r");
    f2 = fopen(file2,"w");
    if(f2==NULL)
	printf("File doesn't exist\n");
    while((ch=getc(f1))!=EOF)
    {
        putc(ch,f2);
    }
    fclose(f1);
    fclose(f2);
   	return;
}

//helper function to check whether the user has requested a background process
int checkbg(char ** args, int argcount, int * dec)
{
	if(strcmp(args[argcount-1],"&")==0)
	{
		args[argcount-1] = NULL;
		*dec = 1;
		return 1;
	}
	else
	{
		int len = strlen(args[argcount-1]);
		//printf("%c\n", args[argcount-1][len-1]);
		if (args[argcount-1][len-1]=='&')
		{
			args[argcount-1][len-1]='\0';
			*dec = 0;
			return 1;
		}
	}
	*dec = 0;
	return 0;
}

//main function
int main(int argc, char const *argv[])
{
	printf("We are in shell\n");
	char current_working_directory[BUFFER_SIZE];
	char command[BUFFER_SIZE];
	char * status;
	char * args[BUFFER_SIZE];
	char temp_curr_dir[BUFFER_SIZE];
	int running = 1,i;
	int error;
	//int ip, op;
	int ifd1, ifd, ofd1, ofd;
	ifd1 = dup(0);
	ofd1 = dup(1);
	while(running)
	{
		//open(0);
		//open(1);
		//dup(ip);
		//dup(op);
		close(0);
		dup(ifd1);
		close(1);
		dup(ofd1);
		getcwd(current_working_directory,BUFFER_SIZE);
		if(current_working_directory==NULL)
		{
			continue;
		}
		printf("%s>",current_working_directory);
		fgets(command,BUFFER_SIZE,stdin);
		//printf("%s",command);
		char * parsed_tokens;
		int argcount= 0;
		parsed_tokens = strtok_r(command," \t\n",&status);
		int in, in_index, out, out_index;
		in = in_index = out = out_index = -1;
		int p1, p2, pi1, pi2;
		p1 = p2 = pi1 = pi2 = -1;
		int fd1[2], fd2[2];
		//the command line arguments are being parsed
		while(parsed_tokens != NULL)
		{	
			args[argcount] = (char *)malloc(1*sizeof(parsed_tokens));
			//args[argcount] = parsed_tokens;
			strcpy(args[argcount],parsed_tokens);
			//printf("%s\n", args[argcount]);
			if(strcmp(args[argcount],"<")==0)
			{
				in = 1;
				in_index = argcount +1;
			}
			if(strcmp(args[argcount],">")==0)
			{
				out = 1;
				out_index= argcount + 1;
			}
			if(strcmp(args[argcount],"|")==0)
			{
				if(p1==-1)
				{
					p1 = 1;
					pi1 = argcount;
				}
				else
				{
					p2 = 1;
					pi2 = argcount;
				}
			}
			argcount++;
			//printf("%s\n", parsed_tokens);
			parsed_tokens = strtok_r(NULL," \t\n",&status);
		}
		//printf("YO\n");
		//printf("%d\n",argcount );
		//printf("p1 = %d, pi1 = %d, p2 = %d, pi2 = %d\n", p1, pi1, p2, pi2 );
		//printf("in = %d, in_index = %d, out = %d, out_index = %d\n", in, in_index, out, out_index);

		if (argcount==0)
		{
			continue;
		}
		int dec = -1;
		int background = checkbg(args,argcount,&dec);
		//printf("bg = %d\n",background);
		//printf("arg bef = %d\n", argcount);
		
		if(dec==1)
		{
			argcount--;
		}
		//printf("arg aft = %d\n", argcount);
		int bfor;
		//enter if there is a background process
		if(background==1 ) 
		{
			bfor = fork();
			if(bfor!=0)
			{
				continue;
			}
			else
			{
				if(in==1&&args[in_index]!=NULL)
				{
					ifd = open(args[in_index], O_RDONLY);
					if(ifd<0)
					{
						perror("opening in read mode failed :");
						for (i = 0; i < argcount; ++i)
						{
							args[i] = NULL;
						}
						exit(0);
					}
					else
					{
						close(0);    /* Close the file descriptor for stdin */
						/* Duplicate ifd at the lowest-numbered unused descriptor */
					    dup(ifd);    
					    close(ifd);  /* ifd is no longer needed */
					}
				}
				if(out==1&&args[out_index]!=NULL)
				{
					ofd = open(args[out_index], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
					if(ofd<0)
					{
						perror("opening in write mode failed :");
						for (i = 0; i < argcount; ++i)
						{
							args[i] = NULL;
						}
						exit(0);
					}
					else
					{
						close(1);    /* Close the file descriptor for stdout */
						/* Duplicate ofd at the lowest-numbered unused descriptor */
    					dup(ofd);    
    					close(ofd);  /* ofd is no longer needed */
					}
				}
				if(p1==1)
				{
					if(p2!=1)
					{
						pipe(fd1);
						int ch1 = fork();
						if(ch1==0)
						{
							//printf("in child 1\n");
							close(1);
							dup(fd1[1]);
							close(fd1[0]);
							close(fd1[1]);
							char * first[pi1+1];
							int j;
							for (j = 0; j < pi1; ++j)
							{
								first[j] = (char *)malloc(sizeof(args[j])+1);
								strcpy(first[j],args[j]);
							}
							first[pi1] = NULL;
							execvp(first[0],first);
							//execvp(args[0],args);
							perror("execvp failed : ");
							exit(0);
						}
						else
						{
							int pst;
							waitpid(ch1,&pst,0);
							//wait(&pst);
							int ch2 = fork();
							//printf("in parent\n");
							if(ch2==0)
							{
								//printf("in child 2\n");
								close(0);
								dup(fd1[0]);
								close(fd1[0]);
								close(fd1[1]);
									int temp1 = argcount-pi1;
									char * second[temp1];
									int j;
									for (j = 0; j < temp1-1 ; ++j)
									{
										second[j] = (char *)malloc(sizeof(args[pi1+j+1])+1);
										strcpy(second[j],args[pi1+j+1]);
									}
									second[temp1-1] = NULL;
									execvp(second[0],second);
								//execvp(args[2],args);
								perror("execvp failed : ");	
								exit(0);
							}
							else
							{
								//printf("in parent\n");
								int pst1;
								waitpid(ch2,&pst1,0);
								for (i = 0; i < argcount; ++i)
								{
									args[i] = NULL;
								}
								exit(0);
							}
						}
					}
					else
					{
						//two level of pipeline
						pipe(fd1);
						pipe(fd2);
						int chb1 = fork();
						if(chb1==0)
						{
							//printf("in child 1\n");
							close(1);
							dup(fd1[1]);
							close(fd1[0]);
							close(fd1[1]);
							char * first[pi1+1];
							int j;
							for (j = 0; j < pi1; ++j)
							{
								first[j] = (char *)malloc(sizeof(args[j])+1);
								strcpy(first[j],args[j]);
							}
							first[pi1] = NULL;
							execvp(first[0],first);
							//execvp(args[0],args);
							perror("execvp failed : ");
							exit(0);
						}
						else
						{
							int srt;
							waitpid(chb1,&srt,0);
							int chb2 = fork();
							//printf("in parent\n");
							if(chb2==0)
							{
								//printf("in child 2\n");
								close(0);
								dup(fd1[0]);
								close(fd1[0]);
								close(fd1[1]);
								close(1);
								dup(fd2[1]);
								close(fd2[0]);
								close(fd2[1]);
								int temp1 = pi2-pi1;
								char * second[temp1];
								int j;
								for (j = 0; j < temp1-1 ; ++j)
								{
									second[j] = (char *)malloc(sizeof(args[pi1+j+1])+1);
									strcpy(second[j],args[pi1+j+1]);
								}
								second[temp1-1] = NULL;
								execvp(second[0],second);
								//execvp(args[2],args);
								perror("execvp failed : ");	
								exit(0);
							}
							else
							{
								int srt1;
								waitpid(chb2,&srt1,0);
								int chb3 = fork();
								//printf("in parent\n");
								if(chb3==0)
								{
									//printf("in child 3\n");
									close(0);
									dup(fd2[0]);
									close(fd2[0]);
									close(fd2[1]);
									int temp2 = argcount-pi2;
									char * third[temp2];
									int j;
									for (j = 0; j < temp2-1 ; ++j)
									{
										third[j] = (char *)malloc(sizeof(args[pi2+j+1])+1);
										strcpy(third[j],args[pi2+j+1]);
									}
									third[temp2-1] = NULL;
									execvp(third[0],third);
									//execvp(args[4],args);
									perror("execvp failed : ");
									exit(0);
								}
								else
								{
									//printf("in parent\n");
									int srt2;
									waitpid(chb3,&srt2,0);
									for (i = 0; i < argcount; ++i)
									{
										args[i] = NULL;
									}
									exit(0);
								}
							}
						}
					}
				}      // compare the first argument with various built in commands
				if(strcmp(args[0],"exit")==0)
				{
					printf("BYE\n");
					//running = 0;
					exit(0);
				}
				else if (strcmp(args[0],"pwd")==0)
				{
					getcwd(temp_curr_dir,BUFFER_SIZE);
					if(temp_curr_dir==NULL)
					{
						perror("pwd failed : ");
					}
					else
					{
						printf("%s\n", temp_curr_dir);
					}
				}
				else if (strcmp(args[0],"cd")==0)
				{
					error = chdir(args[1]);
					if(error)
					{
						perror("cd failed : ");
					}
				}
				else if(strcmp(args[0],"mkdir")==0)
				{
					error = mkdir(args[1],S_IRWXU);
					if (error!=0)
					{
						perror("mkdir failed :");
					}
				}
				else if (strcmp(args[0],"rmdir")==0)
				{
					error = rmdir(args[1]);
					if (error!=0)
					{
						perror("rmdir failed :");
					}	
				}
				else if (strcmp(args[0],"ls")==0)
				{
					if (args[1]!=NULL&&args[1]!="\n")
					{
						if(strcmp(args[1],"-l")==0)
						{
							//printf("Trying to do ls -l\n");
							lsme(args,1);
						}	
						else
						{
							//printf("Trying to do ls\n");
							lsme(args,0);
						}
					}
					else
					{
						//printf("Trying to do ls\n");
						lsme(args,0);
					}
				}
				else if(strcmp(args[0],"cp")==0)
				{
					if(argcount!=3)
					{
						if(argcount<3)
						{
							printf("Error : Less than three arguments\n");
						}
						else
						{
							printf("Error : More than three arguments\n");
						}
					}
					else
					{
						char * file1, *file2;
						struct stat first, second;
						file1 = (char *)malloc(1*(sizeof(args[1])));
						file2 = (char *)malloc(1*(sizeof(args[2])));
						strcpy(file1,args[1]);
						strcpy(file2,args[2]);
						if(access(file1,F_OK)!=0)
						{
							perror("File1 does not exist :");
							continue;
						}
						else if(access(file1,R_OK)!=0)
						{
							perror("File1 does not have read permission :");
							continue;	
						}
						else if(access(file2,F_OK)!=0)
						{
							printf("%s does not exist\n", file2);
							copyit(file1,file2);
						}
						else if(access(file2,W_OK)!=0)
						{
							perror("File2 does not have write permission : ");
							continue;
						}
						else
						{
							stat(file1, &first);
					        stat(file2, &second);
					        //Performing the time stamp comparison
					        if(difftime(first.st_mtime,second.st_mtime)<0)
					        {
					            printf("Error: %s is more recent than %s\n",file2,file1);
					        }
					        else
					        {
					        	copyit(file1,file2);
					        }
						}
					}
				}
				else
				{
					int pid = fork();
					if(pid==0)
					{
						execvp(args[0],args);
						perror("execvp failed : ");
						printf("\n");
						exit(-1);
					}
					else
					{
						int pstat;
						waitpid(pid, &pstat, 0);
					}
				}
				
				for (i = 0; i < argcount; ++i)
				{
					args[i] = NULL;
				}
				exit(0);
			}
		}
		else
		{
			    if(in==1&&args[in_index]!=NULL)
				{
					ifd = open(args[in_index], O_RDONLY);
					if(ifd<0)
					{
						perror("opening in read mode failed :");
						for (i = 0; i < argcount; ++i)
						{
							args[i] = NULL;
						}
						continue;
					}
					else
					{
						close(0);    /* Close the file descriptor for stdin */
						/* Duplicate ifd at the lowest-numbered unused descriptor */
					    dup(ifd);    
					    close(ifd);  /* ifd is no longer needed */
					}
				}
				if(out==1&&args[out_index]!=NULL)
				{
					ofd = open(args[out_index], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
					//printf("I am here\n");
					if(ofd<0)
					{
						perror("opening in write mode failed :");
						for (i = 0; i < argcount; ++i)
						{
							args[i] = NULL;
						}
						continue;
					}
					else
					{
						close(1);    /* Close the file descriptor for stdout */
						/* Duplicate ofd at the lowest-numbered unused descriptor */
    					dup(ofd);    
    					close(ofd);  /* ofd is no longer needed */
					}
				}
				if(p1==1)
				{
					if(p2!=1)//if there is only ien level of pipe
					{
						pipe(fd1);
						int ch1 = fork();
						if(ch1==0)
						{
							//printf("in child 1\n");
							close(1);
							dup(fd1[1]);
							close(fd1[0]);
							close(fd1[1]);
							char * first[pi1+1];
							int j;
							for (j = 0; j < pi1; ++j)
							{
								first[j] = (char *)malloc(sizeof(args[j])+1);
								strcpy(first[j],args[j]);
							}
							first[pi1] = NULL;
							execvp(first[0],first);
							perror("execvp failed 1 : ");
							exit(-1);
						}
						else
						{
							int pst;
							waitpid(ch1,&pst,0);
							int ch2 = fork();
							/*if(pst==-1&&ch2==0)
							{
								execvp(args[2],args);
								perror("execvp failed 2 : ");	
								exit(0);
							}*/
							//if(pst==0&&ch2==0)
							//{
								if(ch2==0)
								{
									//printf("in child 2\n");
									close(0);
									dup(fd1[0]);
									close(fd1[0]);
									close(fd1[1]);
									int temp1 = argcount-pi1;
									char * second[temp1];
									int j;
									for (j = 0; j < temp1-1 ; ++j)
									{
										second[j] = (char *)malloc(sizeof(args[pi1+j+1])+1);
										strcpy(second[j],args[pi1+j+1]);
									}
									second[temp1-1] = NULL;
									execvp(second[0],second);
									//execvp(args[2],args);
									perror("execvp failed 2 : ");	
									exit(0);
								}
							//}
							//printf("in parent\n");
							if(ch2!=0)
							{
								//printf("in parent\n");
								int pst1;
								waitpid(ch2,&pst1,0);
								for (i = 0; i < argcount; ++i)
								{
									args[i] = NULL;
								}
								continue;
							}
						}
					}
					else
					{
						//in case there is a two level pipe
						//printf("ha haskj jkshak\n");
						pipe(fd1);
						pipe(fd2);
						int chb1 = fork();
						if(chb1==0)
						{
							//printf("in child 1\n");
							close(1);
							dup(fd1[1]);
							close(fd1[0]);
							close(fd1[1]);
							char * first[pi1+1];
							int j;
							for (j = 0; j < pi1; ++j)
							{
								first[j] = (char *)malloc(sizeof(args[j])+1);
								strcpy(first[j],args[j]);
							}
							first[pi1] = NULL;
							execvp(first[0],first);
							//execvp(args[0],args);
							perror("execvp failed : ");
							exit(0);
						}
						else
						{
							int srt;
							waitpid(chb1,&srt,0);
							int chb2 = fork();
							//printf("in parent\n");
							if(chb2==0)
							{
								//printf("in child 2\n");
								close(0);
								dup(fd1[0]);
								close(fd1[0]);
								close(fd1[1]);
								close(1);
								dup(fd2[1]);
								close(fd2[0]);
								close(fd2[1]);
								int temp1 = pi2-pi1;
								char * second[temp1];
								int j;
								for (j = 0; j < temp1-1 ; ++j)
								{
									second[j] = (char *)malloc(sizeof(args[pi1+j+1])+1);
									strcpy(second[j],args[pi1+j+1]);
								}
								second[temp1-1] = NULL;
								execvp(second[0],second);
								//execvp(args[2],args);
								perror("execvp failed : ");	
								exit(0);
							}
							else
							{
								int srt1;
								waitpid(chb2,&srt1,0);
								int chb3 = fork();
								//printf("in parent\n");
								if(chb3==0)
								{
									//printf("in child 3\n");
									close(0);
									dup(fd2[0]);
									close(fd2[0]);
									close(fd2[1]);
									int temp2 = argcount-pi2;
									char * third[temp2];
									int j;
									for (j = 0; j < temp2-1 ; ++j)
									{
										third[j] = (char *)malloc(sizeof(args[pi2+j+1])+1);
										strcpy(third[j],args[pi2+j+1]);
									}
									third[temp2-1] = NULL;
									execvp(third[0],third);
									//execvp(args[4],args);
									perror("execvp failed : ");
									exit(0);
								}
								else
								{
									//printf("in parent\n");
									int srt2;
									waitpid(chb3,&srt2,0);
									for (i = 0; i < argcount; ++i)
									{
										args[i] = NULL;
									}
									continue;
								}
							}
						}
					}
				}                 //compare the first argument with the various built in commands
			if(strcmp(args[0],"exit")==0)
			{
				printf("BYE\n");
				//running = 0;
				exit(0);
			}
			else if (strcmp(args[0],"pwd")==0)
			{
				getcwd(temp_curr_dir,BUFFER_SIZE);
				if(temp_curr_dir==NULL)
				{
					perror("pwd failed : ");
				}
				else
				{
					printf("%s\n", temp_curr_dir);
				}
			}
			else if (strcmp(args[0],"cd")==0)
			{
				error = chdir(args[1]);
				if(error)
				{
					perror("cd failed : ");
				}
			}
			else if(strcmp(args[0],"mkdir")==0)
			{
				error = mkdir(args[1],S_IRWXU);
				if (error!=0)
				{
					perror("mkdir failed :");
				}
			}
			else if (strcmp(args[0],"rmdir")==0)
			{
				error = rmdir(args[1]);
				if (error!=0)
				{
					perror("rmdir failed :");
				}	
			}
			else if (strcmp(args[0],"ls")==0)
			{
				if (args[1]!=NULL&&args[1]!="\n")
				{
					if(strcmp(args[1],"-l")==0)
					{
						//printf("Trying to do ls -l\n");
						lsme(args,1);
					}	
					else
					{
						//printf("Trying to do ls\n");
						lsme(args,0);
					}
				}
				else
				{
					//printf("Trying to do ls\n");
					lsme(args,0);
				}
			}
			else if(strcmp(args[0],"cp")==0)
			{
				//check the number of arguments
				if(argcount!=3)
				{
					if(argcount<3)
					{
						printf("Error : Less than three arguments\n");
					}
					else
					{
						printf("Error : More than three arguments\n");
					}
				}
				else
				{
					char * file1, *file2;
					struct stat first, second;
					file1 = (char *)malloc(1*(sizeof(args[1])));
					file2 = (char *)malloc(1*(sizeof(args[2])));
					strcpy(file1,args[1]);
					strcpy(file2,args[2]);
					if(access(file1,F_OK)!=0)
					{
						perror("File1 does not exist :");
						continue;
					}
					else if(access(file1,R_OK)!=0)
					{
						perror("File1 does not have read permission :");
						continue;	
					}
					else if(access(file2,F_OK)!=0)
					{
						printf("%s does not exist\n", file2);
						copyit(file1,file2);//copyit is defined above and it simply copies the file1 to file2
					}
					else if(access(file2,W_OK)!=0)
					{
						perror("File2 does not have write permission : ");
						continue;
					}
					else
					{
						stat(file1, &first);
				        stat(file2, &second);
				        //Performing the time stamp comparison
				        if(difftime(first.st_mtime,second.st_mtime)<0)
				        {
				            printf("Error: %s is more recent than %s\n",file2,file1);
				        }
				        else
				        {
				        	copyit(file1,file2);//copyit is defined above and it simply copies the file1 to file2
				        }
					}
				}
			}
			else
			{
				//fork and create a new process to execute the new command which is not a built in command
				int pid = fork();
				if(pid==0)
				{
					execvp(args[0],args);
					perror("execvp failed : ");
					printf("\n");
					exit(-1);
				}
				else
				{
					int pstat;
					waitpid(pid, &pstat, 0);
				}
			}
			
			//set the char * to null to make them ready for next turn
			for (i = 0; i < argcount; ++i)
			{
				args[i] = NULL;
			}
		}
	}
	return 0;
}
