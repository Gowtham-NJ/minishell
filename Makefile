# ───────────────────────────
# Minishell — clean Makefile core
# ───────────────────────────

NAME      := minishell
BONUS     := minishell_bonus

CC        := cc
CFLAGS    := -Wall -Wextra -Werror
CPPFLAGS  := -Iinclude -I. -I$(LIBFT_DIR)
DEPFLAGS  := -MMD -MP

SRC_DIR   := src
OBJ_DIR   := build

LIBFT_DIR := libft
LIBFT     := $(LIBFT_DIR)/libft.a
LDFLAGS   := -L$(LIBFT_DIR) -no-pie
LDLIBS    := -lft -lreadline

include source.mk      # provides CFILES_M and CFILES_B (with src/... paths)

# object lists (mirror src/ tree under build/)
OBJS_M := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CFILES_M))
OBJS_B := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CFILES_B))

DEPS_M := $(OBJS_M:.o=.d)
DEPS_B := $(OBJS_B:.o=.d)

.PHONY: all bonus clean fclean re libft
all: $(NAME)
bonus: $(BONUS)

# link rules (explicit)
$(NAME): $(LIBFT) $(OBJS_M)
	$(CC) $(CFLAGS) $(OBJS_M) $(LDFLAGS) $(LDLIBS) -o $@

$(BONUS): $(LIBFT) $(OBJS_B)
	$(CC) $(CFLAGS) $(OBJS_B) $(LDFLAGS) $(LDLIBS) -o $@

# compile (shared)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# utilities
clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(BONUS)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
libft: $(LIBFT)

# include auto-generated deps (safe if missing)
-include $(DEPS_M) $(DEPS_B)
