#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#define BUF_SIZE 4096;

bool makeDirectory(const char* dir)
{
    struct stat s;
    if(stat(dir,&s)== -1)
    {
        return false;
    }
    return S_ISDIR(s.st_mode);
}

bool makeFile(const char* f)
{
    struct stat s;
    if(stat(f,&s) == -1)
    {
        return false;
    }
    return S_ISREG(s.st_mode);
}

int copy(char* from, char* to)
{
    int fd_from = open(from, O_RDONLY);
    int fd_to = open(to,0777);
    char buf[BUF_SIZE];
    int copied = 0;
    int to_copy=0;
    if (fd_from == -1)
    {
        return 1;
    }
    if(fd_to == -1)
    {
        return 2;
    }
    while((to_copy = read(fd_from, &buf, sizeof(buf)))>0)
    {
        copied=write(fd_to, &buf, to_copy);
        if (copied != to_copy) 
        {
            return (copied - to_copy);
        }
    }
    close(fd_from);
    close(fd_to);
    return 0;
}


int compare(char * from, char* to)
{
    struct stat from, to;
    int working;
    working = stat(from_file, &from);
    if (working < 0) 
    {
        return -1;
    }
    working = stat(to_file, &to);
    if (working < 0) 
    {
        return -2;
    }
    if ((from.st_mtime == to.st_mtime)) 
    {
        return 0;
    }
    return 1;
}
    //char buf[BUF_SIZE];
    //int inotify_fd = 0;
    //truct inotify_event *event = NULL;
    //char* pathname = ' ';
    //inotify_fd = inotify_init();
    //inotify_add_watch(inotify_fd, pathname, IN_ALL_EVENTS);
    //while (1) 
    //{
        //int n = read(inotify_fd, buf, BUF_SIZE);
        //char* p = buf;
        //while (p < buf + n) 
        //{
            //event = (struct inotify_event*)p;
            //uint32_t mask = event->mask;
            //if (mask & IN_MODIFY) 
            //{
                //printf("File has been modified\n");
            //}
            //if (mask & IN_CLOSE_WRITE) 
            //{
                //printf("File closed after write\n");
            //}
            //p += sizeof(struct inotify_event) + event->len;



void listFile(const char* dirname,const char* backname, char* forgzip)
{
    DIR* dir;
    struct dirent* d;
    if (!(dir = opendir(dirname)) or !(d=readdir(dir)))
    {
        if (errno = ENOENT)
        {
            perror("Directory doesnt't exist");
        }
        else
        {
            perror("Cannot read directory");
        }
        return;
    }

    while (d!= NULL)
    {
        char path[255] = {0};
        int way = snprintf(path, sizeof(path)-1,"%s/%s",dirname,d->d_name);
        char back[255] = {0};
        way = snprintf(back, sizeof(back)-1,"%s/%s",backname,d->dirname);
        char gzipp[255] = {0};
        way = snprintf(gzipp,sizeof(gzipp)-1,"%s/%s.gz",backname,d->d_name);
        if (makeDirectory(d))
        {
            if((d->d_type != DT_DIR) or (strcmp(d->d_name,".")==0) and (strcmp(d->d_name, "..") == 0))
                continue;
            if(mkdir(back,0777)!=0)
            {
                if(errno!=EEXIST)
                {
                   perror("Failed cataloge");
                   exit(1); 
                }
            }
            listFile(path,back,gzipp);

        }
        else
        {
            if(!makeFile(gzipp)!=0)//gzip is not found
            {
                pid_t pid = fork();
                if (pid < 0)
                {
                    perror("Fork failed");
                    exit(2);
                }
                else if (pid==0)
                {
                    if(copy(path,back) == 0)
                    {
                        exec_status = execlp("gzip","gzip",back,NULL);//change child to gzip
                        if (exec_status == -1)
                        {
                            perror("Gzip failed");
                            exit(3);

                        }
                    }
                }
            }
            else //gzip found
            {
                pid_t pid = fork();
                if (pid < 0)
                {
                    perror("Fork failed");
                    exit(4);
                }
                if(pid == 0)
                {
                    if(compare(path,back)!=0)
                    {
                        if (copy(path,back)==0)
                        {
                            exec_status = execlp("gzip","gzip","-d","-k","-f",gzipp,NULL);
                            if (exec_status == -1)
                            {
                                perror("Gzip failed");
                                exit(5);
                            }
                        }
                    }      
                }
            }
        }
    }
    closedir(dir);
}







int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        perror("Invalide number of arguments\n");
        exit(1);
    }
    if(makeDirectory(argv[2])==0)
    {
        mkdir(argv[2],0777);
    }
    listFiles(argv[1],argv[2],argv[0]);

    return 0;
}