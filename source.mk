# sources.mk — only lists files, no rules

# directories
EXPAND     := src/expand
EXPAND_ENV     := src/expand/expand_env
HEREDOC    := src/heredoc
EXEC       := src/exec
EXE_EXPORT := src/exec/exe_export
EXPORT     := src/exec/exe_export/export
UNSET      := src/exec/exe_export/unset
PARSER     := src/parser
PLAN       := src/plan
EXECUTOR   := src/executor
ROOT       := src
APP        := src/app
INPUT      := src/input
SIGNAL     := src/signal
PROMPT     := src/prompt
QUOTES     := src/quotes
POOL     := src/pool

# groups

SRC_ROOT := \
  $(ROOT)/ft_make_env_copy.c \
  $(ROOT)/ft_make_env_copy_utils.c \
  $(ROOT)/ft_atoi_long_long.c \
  $(ROOT)/ft_atoi_long_long_check.c \
  $(ROOT)/lexer.c \
  $(ROOT)/lexer_utils.c \
  $(ROOT)/ctx.c \
  $(ROOT)/status.c \
  $(ROOT)/child_helpers.c \
  $(ROOT)/main.c

SRC_EXECUTOR := \
  $(EXECUTOR)/eval_pipeline.c \
  $(EXECUTOR)/free_ast.c \
  $(EXECUTOR)/run.c \
  $(EXECUTOR)/run_helper.c

SRC_APP := \
  $(APP)/loop.c \
  $(APP)/loop_helper.c \
  $(APP)/loop_utils.c

SRC_INPUT := \
  $(INPUT)/input.c

SRC_SIGNAL := \
  $(SIGNAL)/setup.c \
  $(SIGNAL)/setup_helper.c

SRC_PLAN := \
  $(PLAN)/eval.c \
  $(PLAN)/build_plan.c \
  $(PLAN)/redirs.c \
  $(PLAN)/child.c \
  $(PLAN)/child_utils.c \
  $(PLAN)/build_plan_utils.c

SRC_EXEC := \
  $(EXEC)/exec_error.c \
  $(EXEC)/assign.c \
  $(EXEC)/eval_core_utils.c \
  $(EXEC)/ast_build_b.c \
  $(EXEC)/builtins_core.c \
  $(EXEC)/syntax_err.c \
  $(EXEC)/redir_apply.c \
  $(EXEC)/builtins_impl.c \
  $(EXEC)/ft_save.c \
  $(EXEC)/mem.c \
  $(EXEC)/ast_build_a.c \
  $(EXEC)/exec_path.c \
  $(EXEC)/eval_core.c \
  $(EXEC)/ast_print.c

SRC_EXE_EXPORT := \
  $(EXE_EXPORT)/ft_exit.c \
  $(EXE_EXPORT)/ft_echo.c \
  $(EXE_EXPORT)/ft_env.c \
  $(EXE_EXPORT)/ft_cd.c \
  $(EXE_EXPORT)/ft_cd_helper.c \
  $(EXE_EXPORT)/ft_env_set.c
# add $(EXE_EXPORT)/ft_pwd.c only if it exists

SRC_EXPORT := \
  $(EXPORT)/ft_export_utils_2.c \
  $(EXPORT)/ft_export_utils.c \
  $(EXPORT)/ft_export.c \
  $(EXPORT)/ft_export_check.c

SRC_UNSET := \
  $(UNSET)/ft_unset.c \
  $(UNSET)/ft_unset_utils.c \
  $(UNSET)/ft_unset_check.c

SRC_EXPAND := \
  $(EXPAND)/glob_collect.c \
  $(EXPAND)/glob_pattern.c \
  $(EXPAND)/status_apply.c \
  $(EXPAND)/glob_path.c \
  $(EXPAND)/glob_cmd_replace.c \
  $(EXPAND)/status.c \
  $(EXPAND)/glob_ast.c \
  $(EXPAND)/glob_cmd_core.c \
  $(EXPAND)/glob_chars.c

SRC_EXPAND_ENV := \
  $(EXPAND_ENV)/expand_buf.c \
  $(EXPAND_ENV)/expand_word.c \
  $(EXPAND_ENV)/expand_word_utils.c \
  $(EXPAND_ENV)/expand_cmdsubst_child.c \
  $(EXPAND_ENV)/expand_cmdsubst_parent.c \
  $(EXPAND_ENV)/expand_ifs.c \
  $(EXPAND_ENV)/expand_argv.c \
  $(EXPAND_ENV)/expand_apply.c \
  $(EXPAND_ENV)/quote_scan.c \
  $(EXPAND_ENV)/expand_env_utils.c \

SRC_HEREDOC := \
  $(HEREDOC)/body_child.c \
  $(HEREDOC)/collect.c \
  $(HEREDOC)/parse.c \
  $(HEREDOC)/body_parent.c \
  $(HEREDOC)/body_utils.c \
  $(HEREDOC)/body.c \
  $(HEREDOC)/release.c

SRC_PARSER := \
  $(PARSER)/parser_entry.c \
  $(PARSER)/parser_core.c \
  $(PARSER)/parser_nodes_compound.c \
  $(PARSER)/parser_argv.c \
  $(PARSER)/parser_redir.c \
  $(PARSER)/parser_nodes_simple.c

SRC_PROMPT := \
  $(PROMPT)/prompt.c \
  $(PROMPT)/prompt_utils.c

SRC_POOL := \
  $(POOL)/pool.c \
  $(POOL)/pool_helper.c

SRC_QUOTES := \
  $(QUOTES)/ft_quotes_parent.c \
  $(QUOTES)/ft_quotes_child.c \
  $(QUOTES)/ft_quotes_utils.c

# master list exported to Makefile
CFILES := \
  $(SRC_ROOT) \
  $(SRC_EXECUTOR) \
  $(SRC_PLAN) \
  $(SRC_EXEC) \
  $(SRC_EXE_EXPORT) \
  $(SRC_EXPORT) \
  $(SRC_UNSET) \
  $(SRC_EXPAND) \
  $(SRC_HEREDOC) \
  $(SRC_PARSER) \
  $(SRC_APP) \
  $(SRC_INPUT) \
  $(SRC_SIGNAL) \
  $(SRC_EXPAND_ENV) \
  $(SRC_PROMPT) \
  $(SRC_POOL) \
  $(SRC_QUOTES)

# The one file that differs:
SRC_M = src/lexer_helper_bonus.c
SRC_B = src/lexer_helper_bonus.c

# Compose per-build lists (no self-reference)
CFILES_M := $(CFILES) $(SRC_M)
CFILES_B := $(CFILES) $(SRC_B)
