# CoMpk : Isolating Data in Private, and Secure Compartments

Operating systems provide inter-process isolation by virtualizing the memory allocated to each process, but intra-process isolation still remains a concern for developers to this day. An attacker may utilize untrusted parts of the application to gain access to private data within the virtual memory of a process. Hence, techniques that isolate application data from untrusted parts of the application itself are essential to ensure data safety. Such a technique should guarantee data isolation amongst different functioning parts of an application and incur a minimal run-time overhead in contrast with the unmodified, vanilla application. 

We propose CoMpk, an isolation technique that builds on existing techniques to provide a solution that is both, secure and efficient. We utilize static analysis techniques along with Intel x86 Memory Protection Keys (MPKs) to isolate data in the virtual memory of a process from certain parts of the application that are not trusted, and only allow access to user-defined sections of code termed as private functions. We aim to design a leak-proof process with minimal privilege switching. 

## Model 
<p align="center">
  <img src="https://i.ibb.co/n89mBcg/0001.jpg">
</p>

Using ERIM like wrappers and libmpk, we create a virtual key mechanism to hold 1024 keys through Intel's MPK(Memory protection key) as opposed to the traditional 16 keys. This is used in the source code which is passed through our compiler which performs static analysis to detect rogue calls and creates an executable. This executable automatically registers itself with our hypervisor for monitoring of system calls. Read our paper for more details.

## Files
- kernel - Modified ubuntu kernel which supports libmpk
- lib - LibMpk and ERIM Wrapper
- perf - Performance evaluation 
- testcode - Demo code
- CoMpk Paper - Final report on CoMpk by Arul T Sagayam, Jubayer Mahmod, Saim Ahmad

