#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<fcntl.h>
#include<pwd.h>
#include<signal.h>

#define BUFSIZE 1024

void main(){
	
	char *history[30];
	int h=0;	
	int history_cnt=0;
	int *ptr_h;
	ptr_h=&h;
	int h_loop=0;
	
	for(h_loop;h_loop<30;h_loop++){
		history[h_loop]=(char*)malloc(sizeof(char)*128);
	}


	while(1)
	{
		char buf[BUFSIZE]={0};
		char buf_forCD[BUFSIZE]={0};
		char *ptr=NULL;
		char *strptr[10]={NULL};
		int strptr_cnt=0;

		char buf_exec_cnt[BUFSIZE]={0};
		char *temp_exec_cnt=NULL;
		int exec_cnt=0;

		char buf_check_PandR[BUFSIZE];


		char *command[10]={NULL};
		char *filename;
		char *argv;
		int i=1;//for command option
		int state;

		int t;//for initialize of command pointer array 
		int Loop_p=0;//for loop of check of command has pipe;
		int Loop_r=0;
		char *ptr_p;
		char *ptr_r1;
		char *strptr_p[10];
		char *strptr_r[10];

		char for_cd[64];
		char *ptr_cd;
		char *ptr_CD;
		char *cd_path=NULL;
		char *temp_name1;
		char *temp_name2;

		int r=0;//for count of command has redirection
		int p=0;//for count of command has pipe
		
		
		printf("NEW$[%s]$ ",getcwd(NULL,0));
		gets(buf);

		if(strcmp(buf,"exit")==0){
			kill(getpid(),SIGKILL);
			kill(getppid(),SIGKILL);
		//	exit(0);
		}if(strcmp(buf,"")==0){
			continue;
		}


		strcpy(buf_forCD,buf);
		//line 90 has segmentation about I type the enter
		if(strcmp("cd",strtok_r(buf_forCD," ",&ptr_CD))==0){//cd command
			char* final_path=(char*)malloc(sizeof(char)*256);
			int path_cnt=0;

			strcpy(for_cd,buf);

			cd_path=strtok_r(for_cd," ",&ptr_cd);
			cd_path=strtok_r(NULL," ",&ptr_cd);



			/*history*/
			if(history_cnt>=30){
				history_cnt=history_cnt%30;
				free(history[history_cnt]);
				history[history_cnt]=(char*)malloc(sizeof(char)*128);
			}
			strcpy(history[history_cnt],buf);
			

			history_cnt++;

			/*history end*/

			if(cd_path==NULL){// execution the 'cd' command
				char *temptemp;
				char *ptr_temptemp;
				int ii=0;
				uid_t user_id;
				struct passwd *user_pw;


				temp_name1=(char*)malloc(sizeof(char)*64);

				
				user_id=getuid();
				user_pw = getpwuid(user_id);
				
				strcpy(temp_name1 ,user_pw->pw_dir);
				chdir(temp_name1);
				
				execl(temp_name1,temp_name1,NULL);

				
			}else{//execution 'cd some directory'->2 case
				//1)absolute path 2)relative path
				char absol[50]={0};
				char* ptr_ab=NULL;
				strcpy(absol,cd_path);

				if(strncmp(absol,"/",1)==0){//absolute path command 'cd /some1/some2...'

					chdir(cd_path);
					execl(cd_path,cd_path,NULL);
					
				}else{//relative path


					temp_name1=getcwd(NULL,0);
					temp_name2="/";
			

	
					while(*temp_name1!='\0'){
						*final_path=*temp_name1++;
						final_path++;
						path_cnt++;
					}
					while(*temp_name2!='\0'){
						*final_path=*temp_name2++;
						final_path++;
						path_cnt++;
					}
					while(*cd_path!='\0'){
						*final_path=*cd_path++;
						final_path++;
						path_cnt++;
					}
					*final_path = '\0';
					final_path = final_path-path_cnt;
					chdir(final_path);
					execl(temp_name1,temp_name1,NULL);
				}
			}
		}
		else{
			char temp_buf_h[128]={0};

			int his_exec_num=0;
			char *temp_c=NULL;
			/* parsing ";" to check how many runs to program */

			strcpy(buf_exec_cnt,buf);//copy for check number of ";"

			strptr[strptr_cnt]=strtok_r(buf_exec_cnt,";",&ptr);
			/*for history*/
			if(history_cnt>=30){
				history_cnt=history_cnt%30;
				free(history[history_cnt]);
				history[history_cnt]=(char*)malloc(sizeof(char)*128);
			
			}
			strcpy(history[history_cnt],strptr[strptr_cnt]);
	

			
			/*for history end*/

			if((strptr[strptr_cnt])!=NULL){
				exec_cnt++;
				/*history*/
				history_cnt++;
				/*history*/
			}
		


			/*history*/
			if(strcmp(strptr[strptr_cnt],"history")==0){
				int a=0;
				for(a;a<30;a++){
					if(strcmp(history[a],"")==0)
						break;
					printf("history[%d]=%s\n",a,history[a]);
				}
			}
		/* I can't success !num command

			strcpy(temp_buf_h,strptr[strptr_cnt]);
			if(strncmp(temp_buf_h,"!",1)==0){
				printf("temp_buf_h!!=%s\n",temp_buf_h);
				temp_c=strtok(temp_buf_h,"!");//temp_c=num
				//strcpy(temp_num,temp_c);
				//printf("temp_num=%s\n",temp_num);
				his_exec_num=atoi(temp_c);
				printf("his_exec_num=%d\n",his_exec_num);
				
				strcpy(strptr[strptr_cnt],"");
				printf("strptr[%d]=%s\n",strptr_cnt,strptr[strptr_cnt]);
				if(his_exec_num>=5){
					his_exec_num=his_exec_num%5;
				}
				strcpy(strptr[strptr_cnt],history[his_exec_num]);
				printf("strptr[%d]=%s\n",strptr_cnt,strptr[strptr_cnt]);

				//exit(0);
			}
		*/
			/*history end*/

			strptr_cnt++;

			while((strptr[strptr_cnt]=strtok_r(NULL,";",&ptr))!=NULL){
				/*for history*/
				char temp_buf_hh[128];

				//segment fault!!!
				strcpy(history[history_cnt],strptr[strptr_cnt]);
	
				history_cnt++;
	
				
				/*for history*/
				exec_cnt++;
	

				strptr_cnt++;
			}
	



			/*we check if command has & or not*/
		
			for(exec_cnt;exec_cnt>0;exec_cnt--){

				
				int amper=0;
				if(strstr(strptr[Loop_p],"&")!=NULL){
		
					strtok(strptr[Loop_p],"&");
					amper=1;
		
				}
				

			
				if(vfork()==0){//child for execution of command
					int command_cnt=0;
					char *last1;
					char *last0;
					int k=0;
					int loop_cnt=0;
					r=0;
					p=0;
					int rr=0;
					ptr_p=NULL;
					ptr_r1=NULL;
					char *ptr_temp=NULL;
					i=1;
					int temp;
					char *for_pipe[10]={NULL};
					char *for_redir[10]={NULL};


					int for_redir_check=0;

					int redir_check1=0;
					int redir_check2=0;
					int redir_check3=0;
					int redir_check4=0;

					int rerecheck=1;


					////////////////////for redirection check//////////////////////////////////////////

					for_redir[Loop_p]=(char*)malloc(strlen(strptr[Loop_p])+1);
					strcpy(for_redir[Loop_p],strptr[Loop_p]);
				
			


					//first, parsing by ">" and check command option and adjust the redirection
					
					strptr_r[r]=strtok_r(for_redir[Loop_p],">",&ptr_r1);
					if(strptr_r[r]!=NULL){
						r++;
					}
					while((strptr_r[r]=strtok_r(NULL,">",&ptr_r1))!=NULL){
			
						r++;
						ptr_temp=ptr_r1;
					}
					if(r>=2){

						if(strstr(strptr[Loop_p],">!")!=NULL){
							r=0;
							ptr_temp=(char*)malloc(strlen(strptr[Loop_p])+1);
			
							strcpy(ptr_temp,strptr[Loop_p]);
							
							if(strtok(ptr_temp,"!")!=NULL){
								r++;
							}
							while((strptr_r[r]=strtok(NULL,"!"))!=NULL){
								r++;
							}
							redir_check4=1;
							rerecheck=0;
						}
						if(strstr(strptr[Loop_p],">>")!=NULL){
							redir_check2=1;
							rerecheck=0;
			
						}if(rerecheck==1){
							redir_check1=1;
						}
						/*else{	
							redir_check1=1;
			
						}*/
					}
					if(r==1){
						
						strcpy(for_redir[Loop_p],strptr[Loop_p]);
						
						r=0;
						ptr_r1=NULL;
						strptr_r[r]=strtok_r(for_redir[Loop_p],"<",&ptr_r1);
						if(strptr_r[r]!=NULL){
							r++;
						}
						while((strptr_r[r]=strtok_r(NULL,"<",&ptr_r1))!=NULL){
			
							r++;
						}
						if(r>=2)
							redir_check3=1;
					}
					if(redir_check1==1||redir_check2==1||redir_check3==1||redir_check4==1){
					
					
			
						if((filename=strtok_r(/*buf*/strptr_r[/*command_cnt*/0]," ",&last0))!=NULL){
							command[0]=(char*)malloc(strlen(filename)+1);
							strcpy(command[0],filename);//command[0] is filename
							while((argv=strtok_r(NULL," ",&last0))!=NULL){
								command[i]=(char*)malloc(strlen(argv)+1);
								strcpy(command[i],argv);
								i++;
							}
							command_cnt=command_cnt+1;
						}
			

						if(redir_check1==1){// ">"
							int fd;
							fd=open(strptr_r[1], O_RDWR | O_CREAT | O_TRUNC, 0644) ;
							close(1);//close stdout
							dup(fd);
							close(fd);
						}else if(redir_check2==1){//">>"
							int fd;
							fd=open(strptr_r[1], O_RDWR | O_APPEND, 0644);
							if(fd<0){
								fd=open(strptr_r[1], O_RDWR | O_CREAT | O_TRUNC, 0644) ;
							}
							close(1);
							dup(fd);
							close(fd);


						}else if(redir_check3==1){//"<<"
							int fd;
							fd=open(strptr_r[1], O_RDONLY);
							close(0);
							dup(fd);
							close(fd);
						}
						//
						else if(redir_check4==1){//">!"
							int fd;
							fd=open(strptr_r[1], O_RDWR | O_CREAT , 0644) ;
							close(1);//close stdout
							dup(fd);
							close(fd);
						}
						Loop_p++;
						execvp(command[0],command);

					}
				
					//for pipe check	
					for_pipe[Loop_p]=(char*)malloc(strlen(strptr[Loop_p])+1);
					strcpy(for_pipe[Loop_p],strptr[Loop_p]);
			
				
			
					// command line is parsing by ";". So we check whether each command has |, >,>>....

					//check whether each command has "|"
					strptr_p[p]=strtok_r(for_pipe[Loop_p],"|",&ptr_p);
				
			

					if((strptr_p[p])!=NULL){
				                p++;
						loop_cnt++;
					
				        }
				
				
				

					while((strptr_p[p]=strtok_r(NULL,"|",&ptr_p))!=NULL){
					
			
						p++;
						loop_cnt++;
					
					}
					Loop_p++;				

			
					// parse the command and store
					for(t=0;t<10;t++){//initialize
						command[t]=NULL;
					}

					if((filename=strtok_r(/*buf*/strptr_p[/*command_cnt*/0]," ",&last1))!=NULL){
						command[0]=(char*)malloc(strlen(filename)+1);
						strcpy(command[0],filename);//command[0] is filename
						while((argv=strtok_r(NULL," ",&last1))!=NULL){
							command[i]=(char*)malloc(strlen(argv)+1);
							strcpy(command[i],argv);
							i++;
						}
						command_cnt=command_cnt+1;
					}
							
				
					if(p>=2){	
						for(k;k<p-1;k++){

							int fildes[2]={0};
							if(pipe(fildes)==-1){
								perror("fail to make pipe");
								exit(1);
							}
						
							if(vfork()==0){//first component of command
								close(1);
								dup(fildes[1]);
								close(fildes[1]);
								close(fildes[0]);
								execvp(command[0],command);

							}else{
								//char *command1[10]={NULL};
								char *filename1=NULL;
								//char *argv1;
								wait(&state);
								for(t=0;t<10;t++){//initialize
									command[t]=NULL;
								}
								i=1;
							

			
								// parse the command and store
								if((filename1=strtok(/*buf*/strptr_p[command_cnt]," "))!=NULL){

			
									command[0]=(char*)malloc(strlen(filename1)+1);
									strcpy(command[0],filename1);//command[0] is filename
									while((argv=strtok(NULL," "))!=NULL){
										command[i]=(char*)malloc(strlen(argv)+1);
										strcpy(command[i],argv);
										i++;
									}
									command_cnt=command_cnt+1;
								}
			
								close(0);
								dup(fildes[0]);
								close(fildes[0]);
								close(fildes[1]);
							}
						}
						execvp(command[0],command);
					}else{
						execvp(command[0],command);

					}

				}else{//parent*/
					if(amper==0)
						wait(&state);
				}
			}
		}
	}
	
}


