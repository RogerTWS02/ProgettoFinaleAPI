bash debug_compile.sh
valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./main < test/open_110.txt > output.txt
