/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmedeiro <lmedeiro@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 18:20:34 by lmedeiro          #+#    #+#             */
/*   Updated: 2023/02/03 21:48:23 by lmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//No server é de mandar o sinal, aqui no client recebe-se o sinal. Essa parte é do
//bônus (signal_receive), que ele manda a confirmação que a msg foi recebida. 

#include "../includes/minitalk.h"

//vai mandar o a msg pro cliente

void	error(char *message)
{
	ft_putstr_fd(message, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

static void	print_signal(int signal, siginfo_t *info, void *ucontext)
{
	static int	shift_bits;
	static char	letter;

	// o caracter ´eum arary que recebe 0 e 1. o shift bit seria um miniarray. 
// se o if fosse de zero, nao faira diferença, no caso seria o sigusr2.
	(void)ucontext;
	if (signal == SIGUSR1)
		letter += (0b00000001 << shift_bits); //o letter é um caracter só.
	if (shift_bits == 7) //verifica se chegou na ultima osição, significa qu eo carcater ta completo. //AQUI JÁ RECEU OS 8 BITS DO CARCATER E ENTRE AQUI. 
	{
		if (letter) //se for uma letra normal, ele imprime. se nao, entra no else de baixo, pq terminou a msg. é raba zaero. 
			ft_putchar_fd(letter, 1);
		else
		{
			if (kill(info->si_pid, SIGUSR2)) // aqui siginifica qu etemrinou a msg toda. pq o '\0' é giaul a zero que é o sigusr2;
				error("Failed to send signal.\n");
		}	
		letter = 0;
		shift_bits = 0;
	}
	else
		shift_bits++;
	if (kill(info->si_pid, SIGUSR1)) //os if (verifica se chegou o sinal) e o kill junto fazem um preocesos juntos. o client espera receber o sinal que foi recebido. TRATAMENTO DE ERRO
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
	printf("PID: %d\n", (int)getpid());
	while (1)
		pause();
	return (0);
}
