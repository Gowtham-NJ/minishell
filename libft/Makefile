# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/07 01:05:25 by adjeuken          #+#    #+#              #
#    Updated: 2025/10/10 20:17:18 by vcerny           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = libft.a

CC          = cc
CFLAGS      = -Wall -Wextra -Werror


FT_PRINTF_FILES =  ft_printf/ft_flags.c  ft_printf/ft_input_string.c  ft_printf/ft_itoa_hex_split.c \
 ft_printf/ft_itoa_int_split.c ft_printf/ft_itoa_uint_split.c ft_printf/ft_pad_string.c ft_printf/ft_printf.c \
 ft_printf/ft_printf_hex.c ft_printf/ft_printf_int.c ft_printf/ft_printf_percent.c ft_printf/ft_printf_ptr.c \
 ft_printf/ft_printf_str.c ft_printf/ft_printf_uint.c   ft_printf/ft_pad_string_hex.c

LIBFT_FILES = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c\
ft_isascii.c ft_isdigit.c ft_isprint.c ft_isspace.c ft_itoa.c\
ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c\
ft_lstiter.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c\
ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c\
ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c\
ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c\
ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncat.c\
ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c\
ft_substr.c ft_tolower.c ft_toupper.c

GET_NEXT_LINE_FILES = \
	get_next_line/get_next_line.c get_next_line/get_next_line_utils.c

SRCS := $(LIBFT_FILES) $(FT_PRINTF_FILES) $(GET_NEXT_LINE_FILES)

# --- build dir & objects ---------------------------------------------------
OBJDIR := build
OBJS   := $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
DEPS   := $(OBJS:.o=.d)

# --- rules -----------------------------------------------------------------
all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $^

# Pattern rule: compile into build/<same/subdir>/file.o
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
