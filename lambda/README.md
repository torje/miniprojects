# Lambdas in C

Okay, I lie, this is not lambdas, it is runtime created functions. It works by
using a compiler to compile the supplied code into a library while the program
is running, then linking in the library and function.

There is two modes to demonstrate this in the program now, apply to a range of
doubles ( 0 to 100) and a filter over an array of scientific classification of
animals. The crude way the choice between what the program will do with your
function depends on the return type. If it is bool is applied as a filter to
animals and double is applied to the array of numbers.

The parameter name is fixed as `a`, the first argument is expected to be return
type, the second parameter type, and the third a return statement.

Oh, and if you thought about safety, no there is none here. you can write an
entire library of functions in here and make any call you imagine.
For a starting point, try these invocations:

```
cmake . -B build && make -C build

./build/lambda "bool" "Animal" "return 0 == strcmp(a.Kingdom,\"Animalia\");"

./build/lambda "double" "double" "return a * a;"

./build/lambda "double" "double" "return sin(a/M_PI);"
```

## Does this have any useful application?

Probably not in this form, but on the other hand,
[cling](https://github.com/root-project/cling) exists, which I guess do
something similar, just way cleaner:

> Cling is an interactive C++ interpreter, built on top of Clang and LLVM
> compiler infrastructure. Cling implements the read-eval-print loop (REPL)
> concept, in order to leverage rapid application development. Implemented as a
> small extension to LLVM and Clang, the interpreter reuses their strengths
> such as the praised concise and expressive compiler diagnostics.

But if you really wanted to you could use this. So if we imagine that we have
something that is expensive to restart and need to redefine functions during
runtime, this works. But a cleaner solution is to build the library separately
and have the machinery pick it up on change and apply the function. But for my
demonstration purpose this is good enough.

## Files generated runtime

liblambda.so and lambda.c are created in the folder you run in.
