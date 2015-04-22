# CS-521-A
# Professor Rahman
#
# 10/19/14
# Author: Nicholas Bevacqua (nbevacqu) & Neal Trischitta (ntrischi) & Michael Peleshenko (mpeleshe)
#
# I pledge my honor that I have abided by the Stevens Honor System.
#
# Assignment  - Makefile

NAME := prog3
SRCDIR := src
SRC := $(wildcard $(SRCDIR)/*.c)
OBJDIR := obj
OBJ := $(addprefix $(OBJDIR)/,$(notdir $(SRC:.c=.o)))
BINDIR := bin
CC := gcc
CFLAGS += -Wall -pedantic-errors

RM := rm -f

all: $(OBJ)
	$(CC) -o $@ $(BINDIR)/$(NAME)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJDIR)/*.o
	$(RM) $(SRCDIR)/*~
	$(RM) $(SRCDIR)/\#*
	$(RM) $(BINDIR)/.*.swp
	$(RM) $(SRCDIR)/*.core

fclean: clean
	$(RM) $(BINDIR)/$(NAME)

re: fclean all