PHILO_ONE_DIR		=	philo_one/
PHILO_TWO_DIR		=	philo_two/
PHILO_THREE_DIR		=	philo_three/

SRCS				=	main.c \
						help.c \
						philo.c

PHILO_ONE_SRCS		= 	$(addprefix $(PHILO_ONE_DIR), $(SRCS))
PHILO_TWO_SRCS		=	$(addprefix $(PHILO_TWO_DIR), $(SRCS))
PHILO_THREE_SRCS	=	$(addprefix $(PHILO_THREE_DIR), $(SRCS))

PHILO_ONE_OBJ 		=	$(PHILO_ONE_SRCS:.c=.o)
PHILO_TWO_OBJ 		=	$(PHILO_TWO_SRCS:.c=.o)
PHILO_THREE_OBJ		=	$(PHILO_THREE_SRCS:.c=.o)

REBUILD_ONE			:=	$(shell find philo_one/ -name philo_one | wc -l)
REBUILD_TWO			:=	$(shell find philo_two/ -name philo_two | wc -l)
REBUILD_THREE		:=	$(shell find philo_three/ -name philo_three | wc -l)

PHILO_ONE_BIN		=	$(addprefix $(PHILO_ONE_DIR), philo_one)
PHILO_TWO_BIN		=	$(addprefix $(PHILO_TWO_DIR), philo_two)
PHILO_THREE_BIN		=	$(addprefix $(PHILO_THREE_DIR), philo_three)

PHILO_ONE_LINK		=	one
PHILO_TWO_LINK		=	two
PHILO_THREE_LINK	=	three

CC					=	gcc
LDFLAGS				=	-pthread
GFLAGS				=	-Wall -Werror -Wextra -g

CC					:= 	$(CC) $(COMPILER) $(GFLAGS) $(LDFLAGS)



%.o: %.c
	@$(CC) -c $< -o $@

all: philo_one philo_two philo_three

philo_one: $(PHILO_ONE_OBJ)
	@$(CC) $(PHILO_ONE_OBJ) -o $(PHILO_ONE_BIN)
	@ln -s $(PHILO_ONE_BIN) $(PHILO_ONE_LINK)
	@echo "\e[1;32mPhilo one was created\e[0m"

philo_two: $(PHILO_TWO_OBJ)
	@$(CC) $(PHILO_TWO_OBJ) -o $(PHILO_TWO_BIN)
	@ln -s $(PHILO_TWO_BIN) $(PHILO_TWO_LINK)
	@echo "\e[1;32mPhilo two was created\e[0m"

philo_three: $(PHILO_THREE_OBJ)
	@$(CC) $(PHILO_THREE_OBJ) -o $(PHILO_THREE_BIN)
	@ln -s $(PHILO_THREE_BIN) $(PHILO_THREE_LINK)
	@echo "\e[1;32mPhilo three was created\e[0m"

clean:
	@rm -f $(PHILO_ONE_DIR)*.o
	@rm -f $(PHILO_TWO_DIR)*.o
	@rm -f $(PHILO_THREE_DIR)*.o
	@echo "\e[1;31mAll object files deleted\e[0m"

fclean: clean
	@rm -f $(PHILO_ONE_BIN) $(PHILO_TWO_BIN) $(PHILO_THREE_BIN)
	@rm -f $(PHILO_ONE_LINK) $(PHILO_TWO_LINK) $(PHILO_THREE_LINK)
	@echo "\e[1;31mAll binary files deleted\e[0m"

re: fclean
ifeq ($(REBUILD_ONE), 2)
	$(MAKE) philo_one
endif
ifeq ($(REBUILD_TWO), 2)
	$(MAKE) philo_two
endif
ifeq ($(REBUILD_THREE), 2)
	$(MAKE) philo_three
endif

.PHONY: all philo_one philo_two philo_three clean fclean re