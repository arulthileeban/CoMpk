
       #define _GNU_SOURCE
       #include <unistd.h>
       #include <sys/syscall.h>
       #include <stdio.h>
       #include <sys/mman.h>
	#include <unistd.h>
	#include <stdio.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <string.h>
	#include <arpa/inet.h>
	#include<pthread.h>
	#include <sys/types.h>
	#include <linux/userfaultfd.h>
	#include <errno.h>
	#include <stdlib.h>
	#include <fcntl.h>
	#include <signal.h>
	#include <poll.h>
	#include <string.h>
	#include <sys/mman.h>
	#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <stdio_ext.h>
#include<mpt/pkey.h>
#define SIZE_OF_STAT 100
#define BOUND_OF_LOOP 50


#define DOMAIN 15
#define M_TIMES 1



//#include<mpt/pkey.h>
//#include <mpt/mpt.h>



#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                  } while (0)




       int
       pkey_set_(int pkey, unsigned long rights, unsigned long flags)
       {
           unsigned int pkru = (rights << (2 * pkey));
           wrpkru(pkru);
	   return 0; // this is just to by pass compilation error for now
       }

       int
       pkey_mprotect_(void *ptr, size_t size, unsigned long orig_prot,
                     unsigned long pkey)
       {
           return syscall(SYS_pkey_mprotect, ptr, size, orig_prot, pkey);
       }

       int
       pkey_alloc_(void)
       {
           return syscall(SYS_pkey_alloc, 0, 0);
       }

       int
       pkey_free_(unsigned long pkey)
       {
           return syscall(SYS_pkey_free, pkey);
       }


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

for(m=0; m <M_TIMES; m++) {

/*Change permission to each domain*/
    for(p = 0; p<DOMAIN; p++) {
      pkey_mprotect_(addr[p], getpagesize(), PROT_READ | PROT_WRITE, vpkeys[p]);

        for(ii =0;ii< 4096;ii++) {
            addr[p][ii] = (0x41+p)%128; /*just to keep tthings ascii*/
                }
      //  for(ii =0;ii< 4096;ii++)
       // printf("%c",addr[p][ii]);
         //    printf("\n");
	     pkey_set_(vpkeys[p], PKEY_DISABLE_ACCESS, 0);
   // printf("ii=%d p = %d\n\n", ii, p);
    }

}

   /*Release keys and pages*/






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


uint64_t var_calc(uint64_t *inputs, int size)
{
int i;
uint64_t acc = 0, previous = 0, temp_var = 0;
for (i=0; i< size; i++) {
	if (acc < previous) goto overflow;
	previous = acc;
	acc += inputs[i];
	}
    acc = acc * acc;
    if (acc < previous) goto overflow;
            previous = 0;
    for (i=0; i< size; i++){
        if (temp_var < previous) goto overflow;
            previous = temp_var;
            temp_var+= (inputs[i]*inputs[i]);
            }
    temp_var = temp_var * size;
    if (temp_var < previous) goto overflow;
    temp_var =(temp_var - acc)/(((uint64_t)(size))*((uint64_t)(size)));
    return (temp_var); 
        overflow: printf("\n\n>>>>>>>>>>>>>> CRITICAL OVERFLOW ERROR IN var_calc!!!!!!\n\n");
    return -EINVAL;
    }


int status;



int main(void)
{
int i = 0, j = 0, spurious = 0, k =0;
uint64_t **times;
uint64_t *variances;
uint64_t *min_values; 

for(ii= 0; ii<DOMAIN; ii++){

vpkeys[ii] =  pkey_alloc_();
           if (vpkeys[ii] == -1)
            errExit("pkey_alloc");
}


uint64_t max_dev = 0, min_time = 0, max_time = 0, prev_min =0, tot_var=0, max_dev_all=0, var_of_vars=0, var_of_mins=0; 

    printf("Start measuring...\n");
    times = malloc(BOUND_OF_LOOP*sizeof(uint64_t*));
if (!times) {
printf("unable to allocate memory for times\n");
return 0;
}

for(ii =0; ii<DOMAIN; ii++){
        addr[ii] = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE,
                                 MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
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

FILE *fd = fopen("data.txt", "w");


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
for(ii =0; ii<DOMAIN; ii++)
    munmap(addr[ii],getpagesize());

 for(ii = 0; ii<DOMAIN ; ii++) {
    status = pkey_free_(vpkeys[ii]);
     if (status == -1)
      errExit("pkey_free");
        }




    return 0;
}


