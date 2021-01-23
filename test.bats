#!/usr/bin/env bats

setup() {
	make philo_one
	# make philo_two
	# make philo_three
}

teardown() {
	make fclean
}

@test "test with 5 800 200 200" {
	./one 5 800 200 200
	echo "hello"
	if [ $? -ne 0 ]; then
		echo "return value not equal 0" "${output}";
		exit 1;
	fi
	dead_found=$(echo "${output}" | grep "died" | wc -l)
	if [ "${dead_found}" == "0" ]; then
        echo "fail" "${output}";
		exit 1;
    fi
}

@test "test with 5 800 200 200 2" {
	./one 5 800 200 200 2
	if [ $? -ne 0 ]; then
		echo "return value not equal 0" "${output}";
		exit 1;
	fi
	dead_found=$(echo "${output}" | grep "died" | wc -l)
	if [ "${dead_found}" == "0" ]; then
        echo "fail" "${output}";
		exit 1;
    fi
	dead_found=$(echo "${output}" | grep "eating" | wc -l)
	if [ "${dead_found}" != "10" ]; then
        echo "fail" "${output}";
		exit 1;
    fi
}
@test "test with 4 410 200 200" {
	./one 4 410 200 200
	if [ $? -ne 0 ]; then
		echo "return value not equal 0" "${output}";
		exit 1;
	fi
	dead_found=$(echo "${output}" | grep "died" | wc -l)
	if [ "${dead_found}" == "0" ]; then
        echo "fail" "${output}";
		exit 1;
    fi
}
@test "test with 4 310 200 100" {
	./one 5 800 200 200
	if [ $? -ne 0 ]; then
		echo "return value not equal 0" "${output}";
		exit 1;
	fi
	dead_found=$(echo "${output}" | grep "died" | wc -l)
	if [ "${dead_found}" != "0" ]; then
        echo "fail" "${output}";
		exit 1;
    fi
}