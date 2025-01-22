NAME = philo

SRCS = $(shell find src -name '*.c')

CC ?= cc
CFLAGS += -Wall -Wextra -g3
CFLAGS += -pthread
CFLAGS += -Werror
CPPFLAGS += -Isrc -MMD -MP

LDFLAGS += -lpthread

OBJS := $(SRCS:%=build/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all
all: build

.PHONY: build
build: $(NAME)

.PHONY: run
run: build
	./$(NAME)

# linking stage
$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

# compiling stage
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
