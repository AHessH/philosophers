PHILO_ONE =		philo_one/help.c \
				philo_one/main.c

PHILO_TWO =		philo_two/help.c \
				philo_two/main.c

PHILO_THREE =	philo_three/help.c \
				philo_three/main.c

WORK_FOLDERS =	philo_one \
				philo_two \
				philo_three

REBUILD_ONE := $(shell find philo_one/ -name philo_one | wc -l)
REBUILD_TWO := $(shell find philo_two/ -name philo_two | wc -l)
REBUILD_THREE := $(shell find philo_three/ -name philo_three | wc -l)

CC = gcc
FLAGS = -Wall -Werror -Wextra -O2

clean:
	for dir in $(WORK_FOLDERS); do
		rm ${dir}/*.o
	done

fclean: clean
	rm philo_one/philo_one
	rm philo_two/philo_two
	rm philo_three/philo_three

re: fclean
ifeq ($(REBUILD_ONE), 2)
	echo one
endif
ifeq ($(REBUILD_TWO), 2)
	echo two
endif
ifeq ($(REBUILD_THREE), 2)
	echo three
endif