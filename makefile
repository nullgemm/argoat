CC = gcc
FLAGS = -std=c99 -pedantic -Wall -Wextra -Werror=vla -Werror -g

BIND = bin
OBJD = obj
SRCD = src
TEST = test
INCL = -I$(SRCD)

TESTS = $(BIND)/test1
# TESTS+= $(BIND)/test2

.PHONY:all
all: $(TESTS)

$(OBJD)/%.o: %.c
	@echo "building source object $@"
	@mkdir -p $(@D)
	@$(CC) $(INCL) $(FLAGS) -c -o $@ $<

$(BIND)/test%: $(OBJD)/$(SRCD)/argoat.o $(OBJD)/$(TEST)/test%.o
	@echo "compiling $@"
	@mkdir -p $(BIND)
	@$(CC) $(INCL) $(FLAGS) -o $@ $^

clean:
	@echo "cleaning workspace"
	@rm -rf $(BIND) $(OBJD)
