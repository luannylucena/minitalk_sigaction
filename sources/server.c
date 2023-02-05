/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luanny <luanny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 18:20:34 by lmedeiro          #+#    #+#             */
/*   Updated: 2023/02/05 19:46:10 by luanny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

void	error(char *message)
{
	ft_putstr_fd(message, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

// o programa vai executar essa função quando receber o sinal do client.
static void	print_signal(int signal, siginfo_t *info, void *ucontext)
{
	static int	shift_bits; //conta os bits deslocados
	static char	letter; //armazena o caracter

	// o caracter é um array que recebe 0 e 1. Q shift bit seria um miniarray. 
    // se o if fosse de zero, nao faria diferença, no caso seria o sigusr2.
	(void)ucontext;
	if (signal == SIGUSR1)
		letter += (0b00000001 << shift_bits); //o letter é um caracter só.
	if (shift_bits == 7) //verifica se chegou na ultima posição, significa que o carcater ta completo. Já recebeu os 8 bits do caracter e entra aqui. 
	{
		if (letter) //se for uma letra normal, ele imprime. se nao, entra no else de baixo, pq terminou a msg. É o barra zero. 
			ft_putchar_fd(letter, 1);
		else
		{
			if (kill(info->si_pid, SIGUSR2)) // aqui terminou a msg toda. pq o '\0' é igual a zero que é o sigusr2;
				error("Failed to send signal.\n");
		}	
		letter = 0; // para setar a variável e começar a letura dos 8 bits novamente, sem que haja sobreposição das letras. Mesmo coisa na linha abaixo.
		shift_bits = 0;
	}
	else
		shift_bits++;
	if (kill(info->si_pid, SIGUSR1)) //os if (verifica se chegou o sinal) e o kill fazem o preoceso juntos. O client espera receber o sinal que foi recebido. (TRATAMENTO DE ERRO)
		error("Failed to send signal.\n");
}

int	main(void)
{
	struct sigaction	action;

	///Se SA_SIGINFO for especificado em sa_flags , então deve ser usado sa_sigaction (em vez de sa_handler), que especifica a função de manipulação de sinal para sinal. Usa 3 parametros, não só um.

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
