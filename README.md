
**Interprocess Communication Using Pipes in C:**
- This project demonstrates interprocess communication between three processes (P1, P2, P3) using pipes in the C programming language. The program reads integers from a text file and sends them to child processes P2 and P3 via pipes. P2 categorizes integers by the number of digits, while P3 computes the number of prime integers received. After completing their tasks, P2 and P3 send their results back to the parent process (P1), which then prints the formatted output.

**Features:**
- Utilizes four pipes for multi-directional communication: P1 to P2, P1 to P3, P2 to P1, P3 to P1.
- Child process P2 categorizes integers into groups based on the number of digits.
- Child process P3 checks if received integers are prime numbers.
- Parent process P1 orchestrates communication, reads from the input file, and prints the final results.

**Prerequisites:**
- GCC compiler
- Linux environment (for fork and pipe system calls)

**How to Use:**
- Compile the C program: _gcc main.c -o ipc_program_

**Run the program with an input file:**
- Run the program with an input file: _./ipc_program input.txt_ (Replace input.txt with your desired input file containing integers separated by spaces or newlines.)

**Example Output**
- After running the program, you will see output similar to the following:
Input file: input.txt
1 digits - 3
2 digits - 2
3 digits - 1
4 digits - 0
5 digits - 0
Primes - 4
NonPrimes - 2
- This output indicates the counts of integers with different digit lengths and the number of prime and non-prime integers in the input file.
