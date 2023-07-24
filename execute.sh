clear
test=110
./main < test/open_$test.txt > output.txt
diff test/open_$test.output.txt output.txt
