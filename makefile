CC = gcc
FLAGS = -std=c99 -pedantic -Wall -Wno-unused-parameter -Wextra -Werror=vla -Werror -g

BIND = bin
OBJD = obj
SRCD = src
TEST = test
INCL = -I$(SRCD)

TESTS = $(BIND)/test1
TESTS+= $(BIND)/test2
TESTS+= $(BIND)/test3

.PHONY: all
all: $(TESTS) run

$(OBJD)/%.o: %.c
	@echo "building source object $@"
	@mkdir -p $(@D)
	@$(CC) $(INCL) $(FLAGS) -c -o $@ $<

$(BIND)/test%: $(OBJD)/$(SRCD)/argoat.o $(OBJD)/$(TEST)/test%.o
	@echo "compiling $@"
	@mkdir -p $(BIND)
	@$(CC) $(INCL) $(FLAGS) -o $@ $^

run:
	./$(TEST)/test1.sh
	./$(TEST)/test2.sh
	./$(TEST)/test3.sh

clean:
	@echo "cleaning workspace"
	@rm -rf $(BIND) $(OBJD)
