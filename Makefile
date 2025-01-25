NAME = philo

SRCS  = main.c
SRCS += track/track.c track/log.c track/check.c track/t_tracker.c
SRCS += t_philosopher/t_philosopher.c t_philosopher/unwind.c t_philosopher/philosopher_routine.c
SRCS += time/ft_time.c
SRCS += t_table/t_big_red_button.c t_table/table_init.c t_table/init_table_fields.c t_table/table_cleanup.c
SRCS += t_config/t_config.c t_config/checked_atou.c t_error/error_repr.c
SRCS += t_message_queue/t_message_queue.c

SRCS := $(SRCS:%=src/%)

CC = cc
CFLAGS  = -Wall -Wextra -g3
CFLAGS += -Werror
CFLAGS += -pthread
CPPFLAGS = -Isrc -MMD -MP

ifneq ($(DEBUG),)
CPPFLAGS += -D DEBUG
endif

LDFLAGS =
LDLIBS = -lpthread

OBJS := $(SRCS:%=build/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all
all: build

.PHONY: build
build: $(NAME)

.PHONY: run
run: build
	./$(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

build/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: re
re: fclean build

.PHONY: clean
clean:
	rm -rf build/src

.PHONY: fclean
fclean: clean
	rm -rf build
	rm -rf $(NAME)

.PHONY: update
update: clean
	mkdir -p build
	bear  --output build/compile_commands.json -- make build

# aliases
.PHONY: b c u r
b: build
c: clean
u: update
r: run

-include $(DEPS)
