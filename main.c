/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkaufman <rkaufman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 18:25:18 by rkaufman          #+#    #+#             */
/*   Updated: 2022/06/08 20:46:21 by rkaufman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#define	READ 0
#define	WRITE 1

void	ft_write_fd(char *s);
void	ft_write_fd_nl(char *s);
void	ft_close(int *fd);

int	main(int argc, char **argv, char **envp)
{
	if (argc < 2)
		return (0);

	int i = 0;
	int	cmd = 1;
	int	fd_in[2] = {-1, -1};
	int	fd_out[2] = {-1, -1};
	int	pid;

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
				ft_write_fd("error: cd: bad arguments\n");
			else if (chdir(argv[cmd + 1]) == -1)
			{
				ft_write_fd("error: cd: cannot change directory to ");
				ft_write_fd_nl(argv[cmd + 1]);
			}
		}
		else
		{
			cmd = i;
			i++;
			while (argv[i])
			{
				if (argv[i][0] == '|')
				{
					if (fd_out[0] != -1)
					{
						fd_in[0] = fd_out[0];
						fd_in[1] = fd_out[1];
					}
					if (pipe(fd_out) == -1)
						ft_write_fd("error: fatal\n");
					break;
				}
				else if (argv[i][0]  == ';')
					break;
				i++;
			}
			pid = fork();
			if (pid == 0)
			{
				if (fd_out[0] != -1)
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
				{
					ft_write_fd("error: cannot execute ");
					ft_write_fd_nl(argv[cmd]);
				}
				exit(0);
			}
			else if (pid == -1)
				ft_write_fd("error: fatal\n");
			else
			{
				ft_close(&fd_in[0]);
				ft_close(&fd_in[1]);
				fd_in[0] = fd_out[0];
				fd_in[1] = fd_out[1];
				fd_out[0] = -1;
				fd_out[1] = -1;
				waitpid(pid, NULL, WUNTRACED);
			}
		}
	}
	return (0);
}

int	ft_strlen(char *s)
{
	int i = 0;
	while (*s)
	{
		s++;
		i++;
	}
	return (i);
}

void	ft_write_fd(char *s)
{
	write(STDERR_FILENO, s, ft_strlen(s));
}

void	ft_write_fd_nl(char *s)
{
	ft_write_fd(s);
	write(2, "\n", 1);
}

void	ft_close(int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = -1;
}
