/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmedeiro <lmedeiro@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 22:36:00 by lmedeiro          #+#    #+#             */
/*   Updated: 2023/02/01 00:20:12 by lmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

void	error(char *message)
{
	ft_putstr_fd(message, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

// Essa função checa se o argumento é o argumeto é valido. No caso, o argv[1]. 
// Checa se é maior que 7 ou se é letra (isdigit)

static int	check_args(char *pid)
{
	int	i;

	i = 0;
	while (pid[i])
	{
		if (ft_isdigit(pid[i]))
			i++;
		else
			return (1);
	}
	return (0);
}

// Faz a conversão dos bits e manda essa conversão pro server.

void    send_char(pid_t pid, char *message)
{
	int	i;
	int	kill_status;
	int	bits_shifted; //conta os bits deslocados.

	i = 0;
	while (message[i] != '\0')
	{
		bits_shifted = 0;
		while (bits_shifted < 8)
		{
			if ((message[i] >> bits_shifted) & 0b00000001)
				kill_status = kill(pid SIGUSR1); //1
			else
				kill_status = kill(pid, SIGUSR2); //0
			if (kill_status == -1)
				error("kill error.\n");
			bits_shifted++;
			usleep(500); 
		}
		i++;
	}
}


//int main(int argc, char **argv)
//{
    
  //  pid_t pid; //é da própria biblioteca.
    
   // if (argc != 3)
    //    error("Invalid number of arguments.\n");
   // if (ft_strlen(argv[1]) > 7 || check_args(argv[1]))
   //     error("Invalid PID.\n");
  //  pid = ft_atoi(argv[1]); //pq faz aqui? pq escreve de um pra outro??? 
   // send_char(pid, argv[2]);
   //// return(0);
//}

int	main(int argc, char **argv)
{
	struct sigaction	action;
	pid_t				pid;

	g_char = 0;
	pid = ft_atoi(argv[1]);
	if (argc != 3)
		error("Invalid number of arguments.\n");
	if (ft_strlen(argv[1]) > 7 || check_args(argv[1]))
		error("Invalid PID.\n");
	ft_bzero(&action, sizeof (struct sigaction));
	action.sa_handler = &print_char;
	sigaction(SIGUSR1, &action, NULL);
	sigaction(SIGUSR2, &action, NULL);
	while (*argv[2])
	{
		tell_signal(pid, *argv[2]);
		argv[2]++;
	}	
	send_char(pid, '\n');
	send_char(pid, '\0');
	usleep(100);
}