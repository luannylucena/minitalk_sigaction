## 42cursos - Minitalk

O objetivo deste projeto é desenvolver um pequeno programa de troca de dados utilizando sinais UNIX.

### Mandatório
- Produzir executáveis de `servidor` e `cliente`
- O cliente deve comunicar uma string passada como parâmetro para o servidor (referenciado pelo seu ID de processo), que então a exibirá
- Usar SOMENTE os sinais `SIGUSR1` e `SIGUSR2`

Rodando a aplicação:

1. Clone o repositório, usando o comando: `git clone https://github.com/hanshazairi/minitalk_sigaction`
2. Compile os arquivos em executáveis de servidor e cliente usando: `make -C 42-minitalk`
