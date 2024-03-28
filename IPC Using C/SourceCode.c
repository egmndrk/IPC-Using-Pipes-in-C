#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


//Function to calculate number of digits.
int nrDigits(int inp) {
    if (inp == 0) return 1;
    int digits = 0;
    while (inp != 0) {
        inp /= 10;
        digits++;
    }
    return digits;
}

//Function to check if the number is prime.
int isPrime(int inp) {
    if (inp <= 1) return 0;
    for (int i = 2; i * i <= inp; i++) {
        if (inp % i == 0) return 0;
    }
    return 1;
}
//Main function
int main(int argc, char *argv[]) {

    //Creating four pipes.
    int pfd1[2];
    int pfd2[2];
    int pfd3[2];
    int pfd4[2];
    pipe(pfd1);
    pipe(pfd2);
    pipe(pfd3);
    pipe(pfd4);
    

    //Creating first child proccess P2.
    pid_t pidP2;
    pidP2=fork();
    //Checking if P2 created successfully.
    if (pidP2 == -1) {
        perror("Cannot Create Child");
        return 1;
    }
    //Child proccess P2.
    if (pidP2 == 0) {
        
        //Closing unnecessary end of pipes.
        close(pfd1[1]); //Closing the write-end of first pipe.
        close(pfd2[0]); //Closing the read-end of second pipe.
        close(pfd3[1]); //Closing the write-end and read-end of third pipe.
        close(pfd3[0]); 
        close(pfd4[1]); //Closing the write-end and read-end of fourth pipe.
        close(pfd4[0]); 
        
        //Initialize necessary variables to use.
        int a=0;
        int digitsCount1=0;
        int digitsCount2=0;
        int digitsCount3=0;
        int digitsCount4=0;
        int digitsCount5=0;
        int digitsN[]={0,0,0,0,0};
        
        //Read integers from P1 one by one and put them into nrDigits(int) function.
        while (read(pfd1[0], &a, sizeof(int)) > 0) {
            int digitNumber = nrDigits(a);
            //Set the variables using the output of nrDigits(int) function.
            if(digitNumber==1) digitsCount1++;
            else if(digitNumber==2) digitsCount2++;
            else if(digitNumber==3) digitsCount3++;
            else if(digitNumber==4) digitsCount4++;
            else if(digitNumber==5) digitsCount5++;
        }
        close(pfd1[0]);
        
        //Set the variables using the output of nrDigits(int) function.
        digitsN[0] = digitsCount1;
        digitsN[1] = digitsCount2;
        digitsN[2] = digitsCount3;
        digitsN[3] = digitsCount4;
        digitsN[4] = digitsCount5;
        
        //Write nrDigits(int) output to pipe for P1 to Read.
        write(pfd2[1], digitsN, sizeof(digitsN));
        close(pfd2[1]);
        
        
    } else {
        //Creating second child proccess P3.
        pid_t pidP3;
        pidP3=fork();
        //Checking if P3 created successfully.
        if (pidP3 == -1) {
            perror("Cannot Create Child");
            return 1;
        }
        //Child proccess P3.
        if (pidP3 == 0) {
            //Closing unnecessary end of pipes.
            close(pfd1[1]); //Closing the write-end and read-end of first pipe.
            close(pfd1[0]); 
            close(pfd2[1]); //Closing the write-end and read-end of second pipe.
            close(pfd2[0]);
            close(pfd3[1]); //Closing the write-end of third pipe.
            close(pfd4[0]); //Closing the read-end of fourth pipe.
            
            //Initialize necessary variables to use.
            int a=0;
            int numberOfPrime = 0;

            //Read integers from P1 one by one and put them into isPrime(int) function.
            while (read(pfd3[0], &a, sizeof(int)) > 0) {
                if (isPrime(a)) numberOfPrime++;
            }
                close(pfd3[0]);
                
            //Write isPrime(int) output to pipe for P1 to Read.    
            write(pfd4[1], &numberOfPrime, sizeof(int));
            close(pfd4[1]);

           
        } else {
            //Parent process P1.
            
            //Closing unnecessary end of pipes.
            close(pfd1[0]); //Closing the read-end and read-end of first pipe.
            close(pfd2[1]); //Closing the write-end and read-end of second pipe.
            close(pfd3[0]); //Closing the read-end and read-end of third pipe.
            close(pfd4[1]); //Closing the write-end and read-end of fourth pipe.
            
           //Using file methods for P1 to read integers from the txt files. 
           int totalNumbers=0;
            FILE *fp = fopen(argv[1], "r");
            if (fp == NULL) {
                perror("File could not open, try again!");
                return 1;
            }
            
            //Using another file method to read from txt file and Write to pipes.
            int a=0;
            while (fscanf(fp, "%d", &a) == 1) {
                totalNumbers++;
                //Write to pipe for P2
                write(pfd1[1], &a , sizeof(int));
                //Write to pipe for P3
                write(pfd3[1], &a , sizeof(int));
            }
            close(pfd1[1]);
            close(pfd3[1]);
            

            //Waiting Child proccesses P2 and P3 to continue.
            waitpid(pidP2, NULL, 0);
            waitpid(pidP3, NULL, 0);

            //Initialize necessary variables to use.
            int digitsCount[]={0,0,0,0,0};
            int numberOfPrime=0;
            
            //Taking the integers from the pipe which P2 sent.
            read(pfd2[0], digitsCount, sizeof(digitsCount));
            close(pfd2[0]);
            
            
            //Taking the integers from the pipe which P3 sent.
            read(pfd4[0], &numberOfPrime, sizeof(int));
            close(pfd4[0]);
            

            //Printing all the results.
            printf("Input file: %s\n", argv[1]);
            printf("1 digits - %d\n", digitsCount[0]);
            printf("2 digits - %d\n", digitsCount[1]);
            printf("3 digits - %d\n", digitsCount[2]);
            printf("4 digits - %d\n", digitsCount[3]);
            printf("5 digits - %d\n", digitsCount[4]);
            printf("Primes - %d\n", numberOfPrime);
            printf("NonPrimes - %d\n", (totalNumbers-numberOfPrime));
            

            
        }
    }

    return 0;
}