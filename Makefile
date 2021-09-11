main.o:

	clear

	#gcc -c main.c -g3 -Wall -Wextra -fsanitize=address
	#gcc -c file.c -g3 -Wall -Wextra -fsanitize=address
	#gcc -c util.c -g3 -Wall -Wextra -fsanitize=address
	gcc main.c file.c util.c -s -o small -g
	# rm main.o
	# rm file.o
	# rm util.o
	
	./small

build:
	clear
	gcc main.c file.c util.c -s -o small
# test:
# 
	# clear
# 
	# #gcc -c main.c -g3 -Wall -Wextra -fsanitize=address
	# #gcc -c file.c -g3 -Wall -Wextra -fsanitize=address
	# #gcc -c util.c -g3 -Wall -Wextra -fsanitize=address
	# gcc main.c file.c util.c -s -o small -g3 -Wall -Wextra -fsanitize=address
	# # rm main.o
	# # rm file.o
	# # rm util.o
	# 
	# ./small
