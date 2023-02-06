NAME		=	minitalk
LIBFT_PATH  =	./includes/libft/
PRINTF_PATH =	./includes/ft_printf/
INCLUDES	=	-I $(LIBFT_PATH)  -I $(LIBFT_PATH) -I $(PRINTF_PATH)  -I $(PRINTF_PATH)
CFLAGS		=	-Wall -Wextra -Werror $(INCLUDES)

SERVER_SRCS =	sources/server.c
CLIENT_SRCS =	sources/client.c

SERVER_OBJS = 	$(SERVER_SRCS:.c=.o)
CLIENT_OBJS = 	$(CLIENT_SRCS:.c=.o)

SERVER 		= 	server
CLIENT 		= 	client


all: libft 	printf $(NAME)

libft:
	$(MAKE) -C $(LIBFT_PATH) 

printf:
	$(MAKE) -C $(PRINTF_PATH)

$(NAME): $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_OBJS) $(LIBFT)
	$(CC) $< -o $@ -L$(LIBFT_PATH) -lft -L$(PRINTF_PATH) -lftprintf

$(CLIENT): $(CLIENT_OBJS) $(LIBFT)
	$(CC) $< -o $@ -L$(LIBFT_PATH) -lft -L$(PRINTF_PATH) -lftprintf

clean:
	$(RM) $(SERVER_OBJS) $(CLIENT_OBJS)
	$(MAKE) clean -C $(LIBFT_PATH)
	$(MAKE) clean -C $(PRINTF_PATH)

fclean:
	$(RM) $(SERVER_OBJS) $(CLIENT_OBJS)
	$(RM) $(SERVER) $(CLIENT)
	$(MAKE) fclean -C $(LIBFT_PATH)
	$(MAKE) fclean -C $(PRINTF_PATH)

re: fclean all

.PHONY: all clean fclean re
