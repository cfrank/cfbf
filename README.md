CFBF

A simple Brainfuck interpreter

Usage:

```
make
./bin/cfbf --file="<file>"
-v -- Prints the current running program version
-l -- Prints some useless license information
-h -- Prints the help dialog
-f <file> -- Specify the Brainfuck file to interpret *Required*
```

This is a simple brainfuck interpreter which runs a 30,000 byte `uint8_t`
tape. It allows tape values to over/underflow since this is a requirement
for most programs I have seen.

For the loops, a O(n) function calculates the `jmp_ptr` for each loop command
it encounters and adds this information to the `cfbf_command`. Then when a
loop is encountered in the main interpretation loop, it can be referenced
without multiple `while` loop calls.

There are still many bugs which need to be hashed out, and debug support will
help with that in the future. Since I'm no Brainfuck expert, figuring out if
it's the program or the interpreters fault is a constant battle. And using `gdb`
and `valgrind` is not the best solution. So better debugging support is obviously
needed.

For anyone who wants to compare interpretation speed, inside the `examples`
folder is `bench.bf`

When compiled with the CFLAGS listed in the `Makefile` and run with the `time`
command I get the following output:

```
ZYXWVUTSRQPONMLKJIHGFEDCBA

real    0m1.845s
user    0m1.845s
sys     0m0.000s
```

There is probably room for improvment I'm sure.
