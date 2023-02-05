/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luanny <luanny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 22:36:00 by lmedeiro          #+#    #+#             */
/*   Updated: 2023/02/05 19:46:19 by luanny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minitalk.h"

int	g_stay;

// Essa função vai mandar o aviso que o sinal foi recebido pelo server (bônus)
static void	signal_receive(int signal)
{
	if (signal == SIGUSR1)
		g_stay = 0;
	else
		printf("Message sent successfully!\n");
}

void	error(char *message)
{
	ft_putstr_fd(message, STDERR_FILENO); // //é uma MACRO da biblioteca, que seria o 1. A saída (stdout).
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

static void	send_signal(int pid, char msg)
{
	int	count_bits;

	count_bits = 0;
	while (count_bits < 8)
	{
		if (g_stay == 0)
		{
			g_stay = 1;
			if ((msg >> count_bits) & 0b00000001)
			{
				if (kill(pid, SIGUSR1))
					error("Failed to send signal.\n");
			}
			else
			{
				if (kill(pid, SIGUSR2))
					error("Failed to send signal.\n");
			}
			count_bits++;
		}
	}
}

int	main(int argc, char **argv)
{
	struct sigaction	action;
	pid_t				pid; //struct da biblioteca signal.h

	if (argc != 3)
		error("Invalid number of arguments.\n");
	if (ft_strlen(argv[1]) > 7 || check_args(argv[1]))
		error("Invalid PID.\n");
	pid = ft_atoi(argv[1]); // //o pid vem depois das verificações para evitar erro de leitura e vazamentos (valgrind)
	ft_bzero(&action, sizeof (struct sigaction)); //iniciamos a struct com bzero para que não haja lixo de memória.
	action.sa_handler = &signal_receive;
	sigaction(SIGUSR1, &action, NULL);
	sigaction(SIGUSR2, &action, NULL);
	while (*argv[2])
	{
		send_signal(pid, *argv[2]);
		argv[2]++;
	}	
	send_signal(pid, '\n'); 
	send_signal(pid, '\0');
	usleep(100);
}
