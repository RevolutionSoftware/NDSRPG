# NDSRPG
An free/open-source RPG written in C/ARM assembly for the Nintendo DS

To clone the _entire_ project you need to use `git clone --recursive https://github.com/RevolutionSoftware/NDSRPG.git`.

Coding-style:

C Standard: C11

C++ Standard: C++11

Tabs: 4-wide

Indentation: Tabs

Aligning: Tabs

Code should have a prefix-comment

e.g:

```
#include <stdio.h>
#include <stdint.h>

// function `fun` does _this_ and ~that~ ...
int32_t fun(uint32_t limit)
{
	for(int32_t i = 0; i < limit; ++i) {
		if (i % 2 == 0) {
			printf("%d is even\n", i);
		} else {
			printf("%d is odd\n", i);
		}
	}
	return 0;
}

```
