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

### Type Safety
- Each object are defined with a data type
  - e.g. `int p = 1` which is an integer, and not `int p = "hello"`
- Operations on the object must always be compatible with the initial defined object's data type, so that it will not cause an error during run-time
```c
int p = 1;
p = 5;
```

### Avoiding Exploitation
- Stack Smashing

#### Stack Smashing
![image](https://github.com/bernardawj/software-security/assets/19328469/2150ed23-c031-4062-bbb6-800df8c3be07)
- We can detect `stack smashing` through the concept of `stack canary`
  - We can place a `stack canary` (something like a password salt) after the buffer, then cross-compare it if the values are the same
    - If attackers overrun the buffer, it will be replaced with values that are different from the initial salt
    ![image](https://github.com/bernardawj/software-security/assets/19328469/fd447e79-f78b-422f-9922-7b9ff67fc598)

### Secure Coding
- **Principle** is a design goal with many possible outcomes
  - Validating your inputs
- **Rule** is a specific practice that adheres to sound design principles
  - Enforce input compliance
  - Use safe string functions
  - Don't forget NUL terminator

#### Example: Enforce input compliance
```c
int main() {
	char buf[100], *p;
	int i, len;
	while (1) {
		p = fgets(buf, sizeof(buf), stdin);
		if (p == NULL) return 0;
		len = atoi(p);
		p = fgets(buf, sizeof(buf), stdin);
		if (p == NULL) return 0;
		// `len` may exceed actual message length, so we will need to validate
		// if it exceeds then it is a buffer overflow attack
		// we can sanitize the input (line 13) to be compliant to the rule
		len = MIN(ln, strlen(buf));
		for (i = 0; i < len; i++) {
			if (!iscntrl(buf[i])) putchar(buf[i]);
			else putchar('.');
		}
		printf("\n");
	}
}
```
```c
// Possible overflow if integer value is more than the array size
char unsafe_digit_to_char(int i) {
	char convert[] = "0123456789";
	return convert[i];
}

// Sanitize input
char safe_digit_to_char(int i) {
	char convert[] = "0123456789";
	if (i < 0 || i > 9) {
		return '?';
	}
	return convert[i];
}
```

#### Example: Use safe string functions
```c
void traditional_unsafe() {
	char str[4];
	char buf[10] = "fine";
	strcpy(str, "hello"); // Overflows str
	strcat(buf, "day to you"); // Overflows buf
}

void modern_safe() {
	char str[4];
	char buf[10] = "fine";
	strlcpy(str, "hello", sizeof(str)); // Fails
	strlcat(buf, "day to you", sizeof(buf)); // Fails
}
```

#### Example: Don't forget NUL terminator
```c
void never_factor_nul() {
	char str[3];
	strcpy(str, "bye"); // Write overflow
	int x = strlen(str); // Read overflow
}

void factor_nul() {
	char str[3];
	strlcpy(str, "bye", 3); // Blocked
	int x = strlen(str); // Returns 2
}
```
### Web
- Server
- Resources
- HTTP Requests

#### Server
- Apache httpd
- Nginx
- IIS

#### Resources
- http://www.cs.umd.edu/~mwh/index.html
  - http:// - The protocol or scheme
  - www.cs.umd.edu/ - The hostname that will be translated by the DNS into IP address
  - ~mwh/index.html - Path to a resource

#### HTTP Requests
1. Client (user) types in the URL in the browser and enters, this is a HTTP Request to the server
2. Server takes the request with its headers and body and perform its backend processing and sends back a HTTP Response to the client

### OWASP
- SQL Injection
- Cross-site Scripting (XSS)
- Session Hijacking
- Cross-site Request Forgery (CSRF)

#### SQL
- SQL consists of ACID transactions
  - A - Atomicity
    - Transactions either complete or not complete
  - C - Consistency
    - Data stored is always in a valid state
  - I - Isolation
    - Results from the transaction are not visible until persisted
  - D - Durability
    - Commited transactions persists, even when there is a power failure
- DBMS
  - Database Management Systems
    - Applies the ACID transaction theories

##### SQL Injection
- Attack
```php
# Backend query in PHP lang
$result = mysql_query("select * from Users
					  where (name = '$user' and password = '$pass');");

# Attackers can send in specific value to retrieve all user data
# In the user field, type in `frank' or 1=1); --`
# Basically, it is able to retrieve all values due to 1=1 and the `--` which comments the `and` condition
$result = mysql_query("select * from Users
					  where (name = 'frank' or 1=1); -- and password = '$pass');");
```

- Prevention
  - Sanitization
    - Deleting
      - Delete the characters we don't want (e.g. `'`, `;`, and `--`)
      - But values such as `Peter O'Connor` will be affected
    - Escaping
      - Replace problematic characters with safe ones
      - e.g. `'` to `\'`
    - Prepared Statements
      - Treat user data according to its type
      - ![[Pasted image 20230528212619.png]]
      - ![[Pasted image 20230528212505.png]]
    - Mitigation
      - Perform input validation
      - Limit privileges
      - Encrypt sensitive data

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
