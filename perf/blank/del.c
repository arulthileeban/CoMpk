#include <stdint.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#define SIZE_OF_STAT 100
#define BOUND_OF_LOOP 50


#define DOMAIN 15
#define M_TIMES 1



#include<mpt/pkey.h>
#include <mpt/mpt.h>
#include"mpk_apis.h"


       char* addr[DOMAIN];
int vpkeys[DOMAIN];
int ii;
void static inline Filltimes(uint64_t **times) {
	int i, j,m, p;
	unsigned long long start, end;
	unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
	volatile int variable = 0;

	asm volatile ("CPUID\n\t"
	 "RDTSC\n\t"
	 "mov %%edx, %0\n\t"
	 "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
	"%rax", "%rbx", "%rcx", "%rdx");
	
	asm volatile("RDTSCP\n\t"
	 "mov %%edx, %0\n\t"
	 "mov %%eax, %1\n\t"
	"CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
	"%rbx", "%rcx", "%rdx");
	
	asm volatile ("CPUID\n\t"
	 "RDTSC\n\t"
	 "mov %%edx, %0\n\t"
	 "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
	"%rax", "%rbx", "%rcx", "%rdx");
	
	asm volatile("RDTSCP\n\t"
	 "mov %%edx, %0\n\t"
	 "mov %%eax, %1\n\t"
	"CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
	"%rbx", "%rcx", "%rdx");
	for (j=0; j<BOUND_OF_LOOP; j++) {
	    for (i =0; i<SIZE_OF_STAT; i++) {
	        variable = 0;
		
	        asm volatile ("CPUID\n\t"
	                    "RDTSC\n\t"
	                    "mov %%edx, %0\n\t"
	                    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r"
	                    (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");
	/***********************************/



	/***********************************/

    asm volatile("RDTSCP\n\t"
	 "mov %%edx, %0\n\t"
	 "mov %%eax, %1\n\t"
	 "CPUID\n\t": "=r" (cycles_high1), "=r"
	(cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");
	start = ( ((uint64_t)cycles_high << 32) | cycles_low );
	end = ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );
	if ( (end - start) < 0) {
	printf("\n\n Time is not right\n loop(%d) stat(%d) start = %llu, end = %llu, variable = %u\n", j, i, start, end, variable);
		times[j][i] = 0;
			}
		else
	{
		times[j][i] = end - start;
	 }
	  }
	}
	   return;
 }







int main(void)
{
int i = 0, j = 0, spurious = 0, k =0;
uint64_t **times;
uint64_t *variances;
_init(1);


    for(ii = 0; ii< DOMAIN; ii++) {
    vpkeys[ii] = rwmmap((void**)&addr[ii]);
    }

    printf("Creating 15 domains each with one page memory.........\n");






uint64_t *min_values;
uint64_t max_dev = 0, min_time = 0, max_time = 0, prev_min =0, tot_var=0,
	max_dev_all=0, var_of_vars=0, var_of_mins=0;

    printf("Start measuring...\n");
    times = malloc(BOUND_OF_LOOP*sizeof(uint64_t*));
if (!times) {
printf("unable to allocate memory for times\n");
return 0;
}

for (j=0; j<BOUND_OF_LOOP; j++) {
        times[j] = malloc(SIZE_OF_STAT*sizeof(uint64_t));
        if (!times[j]) {
            printf("unable to allocate memory for times[%d]\n", j);
            for (k=0; k<j; k++)
                free(times[k]);
            return 0;
            
            }
        }

 variances = malloc(BOUND_OF_LOOP*sizeof(uint64_t));
    if (!variances) {
printf("unable to allocate memory for variances\n");
return 0;
}

        min_values = malloc(BOUND_OF_LOOP*sizeof(uint64_t));
        if (!min_values) {
         printf("unable to allocate memory for min_values\n");
        return 0;
 }


Filltimes(times);/* This is my utimate bro*/

FILE *fd = fopen("data_libmpk_1.txt", "w");


for(j =0; j< BOUND_OF_LOOP; j++){
    for(i=0; i<SIZE_OF_STAT; i++) {
    
       printf( "%lu\n",times[j][i]);
        fprintf(fd, "%lu ",times[j][i] );
    }

        fprintf(fd, "\n" );
}

fclose(fd);
for (j=0; j<BOUND_OF_LOOP; j++) {
	    free(times[j]);
}
	    free(times);
	    free(variances);
	    free(min_values);
    for(ii=0; ii<DOMAIN; ii++)
        DESTROY_DOMAIN(vpkeys[ii]);
	return 0;
}


