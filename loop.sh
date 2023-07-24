clear
for test in {1..111}
do
   ./main < test/open_$test.txt > output.txt
   diff test/open_$test.output.txt output.txt
done
