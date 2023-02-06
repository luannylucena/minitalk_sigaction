/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmedeiro <lmedeiro@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 18:20:34 by lmedeiro          #+#    #+#             */
/*   Updated: 2023/02/06 22:40:17 by lmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

void	error(char *message)
{
	ft_putstr_fd(message, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	print_signal(int signal, siginfo_t *info, void *ucontext)
{
	static int	shift_bits;
	static char	letter;

	(void)ucontext;
	if (signal == SIGUSR1)
		letter += (0b00000001 << shift_bits);
	if (shift_bits == 7)
	{
		if (letter)
			ft_putchar_fd(letter, 1);
		else
		{
			if (kill(info->si_pid, SIGUSR2))
				error("Failed to send signal.\n");
		}	
		letter = 0;
		shift_bits = 0;
	}
	else
		shift_bits++;
	if (kill(info->si_pid, SIGUSR1))
		error("Failed to send signal.\n");
}

int	main(void)
{
	struct sigaction	action;

	ft_bzero(&action, sizeof (struct sigaction));
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = print_signal;
	if (sigaction(SIGUSR1, &action, NULL))
		error("Failed to configure signal function.\n");
	if (sigaction(SIGUSR2, &action, NULL))
		error("Failed to configure signal function.\n");
	ft_printf("PID: %d\n", (int)getpid());
	while (1)
		pause();
	return (0);
}
