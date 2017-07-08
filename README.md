![Image of Tiger](images/Tiger.jpg)

# Tiger Engine
Creates object oriented C code and converts it to a JSON file

## The top three reasons to use the Tiger Engine

1. It is designed to be cross-platform and is written entirely in C.
2. Generating a JSON file is native (The engine’s objects are all designed around the JSON format).
3. The engine is object oriented but never exposes the malloc() and free() functions.

## What is the Tiger Engine and who is it for?

* It is designed to extend and replace methods that do not work in Apple’s Cocoa API.
* Anyone using Cocoa in Objective-C or Swift should pick it up quickly.
* The engine’s source code should compile on any IDE that supports the C99 specification.
* Makes copies of each object so there are no confusing pointer counts.
* Uses stacks so you can push and pop objects in batches or not at all.
* Create an array of variables without specifying how many there will be at compile time.
* Reads and writes JSON files and ASCII text files.
* It is open source so if you need a new function you can request it or make your own.
* Uses the MIT source code license so you can use it with any type of project.

## Here’s a “Hello World” snippet of code

```c
TIGStringStartStack(NULL);
	TIGValue *theString = TIGStringInput(NULL, “Hello World”);
	printf(“%s\n”, TIGStringOutput(theString));
TIGStringEndStack(NULL);
```
Check out the main.c file for a list of functions with comprehensive examples. Also make sure to comment out #define TIG_DEBUG in the TigerEngine.h when deploying an application.

Check out our website at: www.solarfusionsoftware.com

If you use the Tiger Engine let us know on our website so we can let others know.
