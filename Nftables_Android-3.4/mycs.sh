#~!/bin/sh

find . -iname '*.c' -o -iname '*.s' -o -iname '*.h' -o -iname '*.hpp' > cscope.files
cscope -b -i cscope.files -f cscope.out
ctags -L cscope.files
