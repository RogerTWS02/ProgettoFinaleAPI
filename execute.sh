clear
test=111
./main < test/open_$test.txt > output.txt
diff test/open_$test.output.txt output.txt
