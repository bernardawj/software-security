# Software Security
This repository contains my learning progress for the module `Software Security`

## Memory Layout
- Calling Function
- Called Function
- Returning Function

### Calling Function
- Arguments are stored in reverse onto the stack frame in this manner
  - It will then jump to the called function's address
```c
/* Top of stack
 * Arg3
 * Arg2
 * Arg1 
 * The return address
 * Bottom of stack
 */

int main(int arg1, int arg2, int arg3) {
    // code..
}
```

## Type of Attacks
- Buffer Overflow

### Buffer Overflow
- Overflowing the buffer to maliciously input code for execution

#### Requirements
- Code to be executed must be in machine-code
- Constructed code should not contain all-zero bytes
  - In some languages, zero-bytes refers to NULL, which is used as a string terminator or to indicate the end of data

#### Process
- Code Injection (to access shell)
```c
#include <stdio.h>

int main() {
    char *name[2];
    name[0] = "/bin/bash";
    name[1] = NULL;
    execve(name[0], name, NULL);
}
```

#### Factors
- Finding the return address is not an easy feat
  - We can increase the chances by using NOP sleds