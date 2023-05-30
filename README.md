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

### Session Hijacking
- Session Cookies are **capabilities** that gives access to a site with the privilege of the user that has established that session
  - Stealing that cookie will allow an attacker to impersonate a legitimate user

#### Mitigation
- Timeout session IDs and delete them once the session ends

### Cross-site Request Forgery
- Attacker can make requests as if the user has intended to make them
- User will have "click a link" that is crafted by an attacker that accesses a vulnerable site

#### Protections
- Referrer
  - This can determine whether if the site is legitimate and is by the company and not some gibberish site link
- Secretized Links
  - Include a secret in every link/form
  - Cross compare the secret on the backend

### Security Engineering
- Phases
- Software vs. Hardware

#### Phases
- Requirements
  - Security Requirements
  - Abuse Cases
  - Architectural Risk Analysis
- Design
  - Architectural Risk Analysis
  - Security-oriented Design
- Implementation
  - Security-oriented Design
  - Code Review (with tools)
- Testing/Assurance
  - Risk-based Security Tests
  - Penetration Testing

#### Software vs. Hardware
- Even though we are mostly focusing on software in this module, we should also factor in hardware during real scenarios
- Software is **malleable** and easily changed
  - Advantageous to core functionality as it can be changed easily
  - However, it is **harmful to security and performance**
- Hardware is **fast**, but hard to change
  - Disadvantageous to evolution of product as it is hard to change
  - However, it is **advantageous to security** as it is considered as immutable

#### Threat Modelling
- Visualizes possible attack scenarios
  - Therefore, it must match with reality or the analysis will be wrong
- Threat-driven Design
  - Finding a good model
    - Compare against similar systems
    - Understand past attacks and attack patterns
    - Challenge assumptions in the created design
- Terminologies
  - Direct Leak
    - Refers to a situation where sensitive information is leaked to individuals during operation due to security misconfigurations
  - Side Channel
    - Refers to a situation where sensitive information is inferred due to unintended and indirect information
- Gold Standard
  - **Au**thentication
    - What I know (e.g. Passwords)
    - What I have (e.g. Smartphone)
    - What I am (e.g. Biometrics)
  - **Au**thorization
    - Defines when a **principal** may perform an action
      - e.g. Bob has access to his account but not Alice's account
  - **Au**dit
    - Retain enough information to be able to determine breach or misbehaviors
      - Log files contains these information that should be protected from tampering or illegal accesses
        - e.g. Account-related action that is logged

##### Example: Network User
- A network user can connect to the service via the network
  - Can measure the size and timing of request and responses
  - Can run parallel sessions
  - Can provide malformed inputs or malformed messages
  - Can drop or send extra messages
- Possible attacks
  - SQL Injection
  - XSS
  - CSRF
  - Buffer overruns

##### Example: Snooping User
- A snooping user on the same network as other users of some service (e.g. someone who is connected to an unencrypted Wi-Fi in the public coffeeshop)
  - Can read/measure other messages
  - Can intercept, duplicate and modify messages
- Possible attacks
  - Session Hijacking
  - Privacy-violating side-channel attack
  - Denial of Service

##### Example: Co-located User
- Internet user on the same machine as other users of some service (e.g. malware installed on a user's laptop)
  - Can read/write user files and memory
  - Can snoop keypresses and other events
  - Can erad/write user's display to spoof
- Possible attack
  - Password theft

#### Design Defects
- Flaws
  - Problems in the **design**
- Bugs
  - Problems in the **implementation**

##### Secure Software Design
- **Design** software architecture according to good *principle* and *rules*
- Design Categories
  - Favor Simplicity
    - Keep it so simple it is obviously correct
    - Use fail-safe defaults
      - Default key length that is secure (2048-bit RSA keys)
      - No default passwords
      - Whitelist valid objects, rather than blacklist invalid ones
    - Do not expect expert users
      - Consider mindset and abilities of system's users that will affect security
        - Favor simple user interfaces
  - Trust with Reluctance
    - Improve security by reducing the need of trust
      - Use **better design, better implementation process and not making unnecessary assumptions**
    - Least privilege
      - Give the system the least amount of privilege to perform an action
    - Compartmentalization
      - Isolate a system component to an isolated environment (e.g. sandbox) and reduce its privileges by making certain interactions impossible
  - Defense in Depth
    - Implement multiple layers of defense, akin to an onion.
    - Even if one layer is breached, additional layers provide protection.
    - Use community resources
      - Hardened code that were built and tested by the community
      - Vet designs publicly
      - Keep up with the latest attacks

#### Very Secure FTP Daemon
- VSFTPD
- Threat Model
  - Clients are untrusted until authenticated
  - Limited privileges after authentication
  - Possible attack goals
    - Steal or corrupt resources
    - Remote code injection
  - Circumstances
    - Client attacks server
    - Client attacks another client

### Program Analysis
- Static Analysis
  - Analyzes program's code without running it
  - Provides higher code coverage and certain guarantees based on analysis
  - Detects code smells or bad coding practices, and incomplete libraries
  - Design tradeoffs: precision, scalability, understandability
  - Aims to model all possible program runs
- Flow Analysis
  - Tainted Flow Analysis
    - Identifies risks such as unvalidated input, injection vulnerabilities, or insecure data handling
    - Distinguishes between legal flows and illegal flows
      - Legal flow: method can receive both tainted and untainted arguments
      - Illegal flow: method not supposed to receive tainted arguments
    - Can be performed using automated tools
  - Other techniques:
    - Context-Sensitive Analysis
      - Analyzes program behavior considering the context of function calls
    - Takes into account different call sites and their respective contexts
    - Enables more precise analysis by capturing context-specific information
    - Information Flow Analysis
      - Deals with implicit flows where one value influences another
      - Uses security labels or tags to represent sensitivity or security level

### Symbolic Execution
- Symbolic execution is a technique that lies between testing and static analysis
- It explores different execution paths of a program using symbolic values instead of concrete inputs
- Bugs reported during symbolic execution are real bugs in the program
- However, each test execution explores only one possible path, limiting the coverage
- Symbolic execution is effective in discovering complex bugs and path-specific vulnerabilities
- It can help uncover input validation errors, boundary cases, and hard-to-reach code paths
- Symbolic execution can also generate test cases that exercise specific program behaviors
- It is commonly used in vulnerability analysis, software testing, and program verification

#### Forking Execution
- Symbolic executors can **fork** at branch points
  - Happens when there are solutions to both the path condition and its negation

#### KLEE
- Executes LLVM bitcode symbolically
  - LLVM: Compiles source file to .bc file
  - KLEE: Runs the .bc file

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
