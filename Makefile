local: compile tests
	rm tp3

tests_queryable_dict: compile_queryable_dict tests
	rm tp3

tests_internal_iterator: compile_internal_iterator tests
	rm tp3

tests_operable_dict: compile_operable_dict tests
	rm tp3

compile:
	gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -Wl,--wrap=malloc -o tp3 *.c

compile_queryable_dict:
	gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -Wl,--wrap=malloc -e main_queryable_dict -o tp3 *.c

compile_internal_iterator:
	gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -Wl,--wrap=malloc -e main_internal_iterator -o tp3 *.c

compile_operable_dict:
	gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -Wl,--wrap=malloc -e main_operable_dict -o tp3 *.c

tests:
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./tp3

time:
	gcc -O0 -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -Wl,--wrap=malloc -o tp3_bench *.c
	echo "first run"
	time ./tp3_bench 2>&1 1>/dev/null
	echo "second run (there should be no caching and take the same time)"
	time ./tp3_bench 2>&1 1>/dev/null
	rm tp3_bench

docker:
	docker build --tag udesa_tp3 .
	docker run --rm -it --entrypoint bash udesa_tp3

clean_docker:
	docker rmi -f $(docker images | grep udesa_tp3 | tr -s ' ' | cut -d ' ' -f 3)
