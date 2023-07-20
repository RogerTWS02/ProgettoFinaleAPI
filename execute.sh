clear
test=106
./main < test/open_$test.txt > output.txt
diff test/open_$test.output.txt output.txt
#106, 110
