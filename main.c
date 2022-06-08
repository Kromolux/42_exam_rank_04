#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int	ft_strlen(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_write(char *s1, char *s2)
{
	write(STDERR_FILENO, s1, ft_strlen(s1));
	write(STDERR_FILENO, s2, ft_strlen(s2));
	write(STDERR_FILENO, "\n", 1);
}

void	ft_close(int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = -1;
}

int	main(int argc, char **argv, char **envp)
{
	if (argc < 2)
		return (0);
	int i = 0, cmd = 1, pid = 0, fd_in[2] = {-1, -1}, fd_out[2] = {-1, -1};
	while(argv[i])
	{
		i++;
		while (argv[i] && argv[i][0] == ';')
			i++;
		if (!argv[i])
			break;
		if (strcmp(argv[i], "cd") == 0)
		{
			cmd = i;
			while (argv[i] && argv[i][0] != ';')
				i++;
			if (i - cmd != 2)
				ft_write("error: cd: bad arguments", "");
			else if (chdir(argv[cmd + 1]) == -1)
				ft_write("error: cd: cannot change directory to ", argv[cmd + 1]);
		}
		else
		{
			cmd = i;
			i++;
			while (argv[i])
			{
				if (argv[i][0] == '|')
				{
					if (pipe(fd_out) == -1)
						ft_write("error: fatal", "");
					break;
				}
				else if (argv[i][0]  == ';')
					break;
				i++;
			}
			pid = fork();
			if (pid == 0)
			{
				if (fd_out[1] != -1)
				{
					close(fd_out[0]);
					dup2(fd_out[1], STDOUT_FILENO);
					close(fd_out[1]);
				}
				if (fd_in[0] != -1)
				{
					close(fd_in[1]);
					dup2(fd_in[0], STDIN_FILENO);
					close(fd_in[0]);
				}
				argv[i] = NULL;
				if (execve(argv[cmd], &argv[cmd], envp) == -1)
					ft_write("error: cannot execute ", argv[cmd]);
				exit(0);
			}
			else if (pid == -1)
				ft_write("error: fatal", "");
			ft_close(&fd_in[0]);
			ft_close(&fd_in[1]);
			fd_in[0] = fd_out[0];
			fd_in[1] = fd_out[1];
			fd_out[0] = -1;
			fd_out[1] = -1;
			if (pid > 0)
				waitpid(pid, NULL, WUNTRACED);
		}
	}
	return (0);
}
