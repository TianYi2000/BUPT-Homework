// grep -v usr < /etc/passwd | wc -l > r.txt; cat r.txt

#include <stdio.h>  // for fprintf
#include <stdlib.h>  // for exit
#include <fcntl.h>  // for open
#include <unistd.h>  // for fork, dup2, close, execlp
#include <sys/wait.h>  // for wait

int main ()
{
	int fd[2], sv, file = -1;
	pipe (fd);
	if (fork () == 0)
	{
		file = open ("/etc/passwd", O_RDONLY);
		if (file == -1){
			fprintf(stderr, "** bad command open: %m\n");
			exit(1);
		}	
		dup2 (file, 0);		//输入重定向为文件/etc/passwd
		dup2 (fd[1], 1);	//输出重定向为管道write端
		close (fd[0]);		
		close (fd[1]);
		execlp ("grep", "grep", "-v", "usr", NULL);
		fprintf(stderr, "** bad command grep: %m\n");
		exit(1);
	}
	else if (fork() == 0)
	{
		file = open ("r.txt", O_WRONLY | O_CREAT, 0666);
		if (file == -1){
			fprintf(stderr, "** bad command open: %m\n");
			exit(1);
		}
		dup2 (file, 1);		//输出重定向为文件r.txt
		dup2 (fd[0], 0);	//输入重定向为管道read端
		close (fd[0]);
		close (fd[1]);
		execlp ("wc", "wc", "-l", NULL);
		fprintf(stderr, "** bad command wc: %m\n");
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	wait(&sv);
	wait(&sv);
	execlp ("cat", "cat", "r.txt", NULL);
	fprintf(stderr, "** bad command cat: %m\n");
	exit(1);
}
