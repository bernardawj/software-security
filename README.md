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
- Memory Exploits

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

#### Techniques
- Stack Smashing
- Heap Overflow
- Read Overflow
- Stale Memory

##### Read Overflow
- Heartbleed
  - Using the `heartbeat` function to identify that the SSL connection is still open
  - Even though the code has limited the buffer size, it did not verify the length was accurate

##### Stale Memory
- Pointer is freed but program is still using it
- Attacker can arrange for the freed memory to be re-allocated to access attacker-controlled data

#### Factors
- Finding the return address is not an easy feat
  - We can increase the chances by using NOP sleds

### Memory Exploits

#### Techniques
- Format String Vulnerabilities
```c
void safe() {
    char buf[80];
    if (fgets(buf, sizeof(buf), stdin) == NULL) return;
    printf("%s", buf); // Format specifiers will be printed as string
}

void vulnerable() {
    char buf[80];
    if (fgets(buf, sizeof(buf), stdin) == NULL) return;
    printf(buf); // Format specifiers in buffer can be utilised as a loophole for attack
}
```

### Memory Safety
- How to achieve?

#### How to Achieve?
- Using a pointer that "belongs" to that pointer
- Create pointers through standard means
```c
p = malloc(...)

p = &x

p = &buf[5]
```
- Combines two ideas
  - Temporal Safety
  - Spatial Safety

##### Spatial Safety
- View pointers as triples (p, b, e)
  - **p** is the actual pointer
  - **b** is the base of the memory region it may access
  - **e** is the extent (bounds) of that region
- **Access allowed** if b <= p <= e - sizeof(typeof(p))
  - Basically, you can think of it like an array;
    - **b** is the start of the array (memory address)
    - **e** is the end of the array (memory address)
    - **p** is the pointer that points at the memory address within the array (from start to end of the memory address)

##### Temporal Safety
- Beware of dangling pointers
- Accessing of pointers that has been de-referenced is a violation of temporal memory safety
- Accessing of un-initialized pointers is another violation as well

#### Tools
- GNU Debugger
```shell
  # Opens up a listener and lets you see what is inside the program
  gdb `pgrep wisdom-alt.c`
  
  # Add a breakpoint at line 92 of the source code `wisdom-alt.c`
  break wisdom-alt.c:92
  
  # Continues to resume normal execution
  continue
  cont
  c
  
  # Move to next line of code
  next
  
  # List all breakpoints
  info break
  
  # Delete breakpoint
  del 1
  
  # Print
  print r # Values
  print &r # Memory address
```