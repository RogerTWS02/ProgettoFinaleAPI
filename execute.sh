clear
test=9
./main < test/open_$test.txt > output.txt
diff test/open_$test.output.txt output.txt
# 9, 15, 21, 49, 75, 84, 97, da 100 in poi, solo penultime stazioni
