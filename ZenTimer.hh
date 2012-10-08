#ifndef TIMER_H
#define TIMER_H

// zentimer.h
//
// Written by: Mark Johnstone (markj@cs.utexas.edu)
// Modified by: Scott F. Kaplan (sfkaplan@cs.utexas.edu)
//
// Last Updated: July 21, 1997
//
// Inspired by: "Zen of Code Optimization" by Michael Abrash
//
// For use on: Intel (tm) Pentium(tm) and P6(tm) processor machines ONLY
//
// Purpose: Alows timing of the number of cycles that a particular
//          piece of code took to execute on a pentium machine.  Usable
//          in user mode with no special privileges
//
// Copyright (c) 1995 Mark Johnstone
//
// Restrictions: Please feel free to use this timer in any way you choose,
//               and to make any changes that you find usefull.  All I ask
//               is that you keep the references to my name and that of
//               Michael Abrash (who inspired this work) in the resulting
//               code.
//
// Warranty: NONE AT ALL!!!


// This code implements a simple class library that makes use of an
// undocumented feature of the Intel Pentium (and P6) processors.
// This feature is a 64 bit register that increments once for every
// clock cycle.  On a 100 MHZ machine this register can count for
// about 5,800 years.

// To use this file, you need a modified version of gas (the GNU assembler)
// The patch file has been successfully used with:
// GNU assembler version cygnus/linux-2.5.2l.15 (i586-unknown-linux),
// using BFD version cygnus/linux-2.5.2l.11

// HOW TO USE THE ZEN TIMER:
// 
// The zen timer can be used by simply creating an instance of the class
// zen_timer and then calling the methods "timer_on", "timer_off", and
// "timer_print".
//
// To create a timer, you simply create an instance of the class zen_timer.
// If an optional file name is passed to the constructor of the zen_timer,
// then a record will be written to that file for every interval timed by
// that instance of the timer.  There are a couple of utilities that are
// provided with this timer to make use of this the file (time2ascii and
// time2histo).
//
// timer_on:
//
// The method "void timer_on(void)" turns on the timer for this particular
// instance of the zen timer (note: you *can* have several timers running
// at the same or different times.)
//
// timer_off:
//
// The method "void timer_off(void)" turns off the timer for this particular
// instance of the zen timer.  The timer must have been turned on prior to
// calling this method.  The number of clock cycles between the call to this
// method and the last call to timer_on for this instance of the timer are
// computed, and if a time file is being created then this interval is written
// out to the time file.  In either case, this interval is processed to provide
// the average interval across all calls to this timer instance.
//
// timer_pause:
// timer_resume:
//
// The method "void timer_pause(void)" pauses the recording of time for a
// particular timer.  The parallel routine "void timer_resume(void) turns this
// timer back on.  This is usefull if you want to time a particular routine,
// but not any of the routines that this routine calls.  You do this by
// calling timer_pause just before any calls that you don't want to time,
// and calling timer_resume just after the call.
// 
//
// timer_print:
//
// the method "void timer_print(void)" prints out some statistical information
// about this instance of the zen timer.  Currently this information goes to
// standard error.  This method prints out the number of intervals timed by
// this instance of the timer, as well as the minimum interval, maximum
// interval, and average interval length.
//
// timer_reset:
//
// the method "void timer_reset(void)" resets the timer to the initial state.
// Only use this method if for some reason you want to throw away the
// information that you have previously recorded.
// NOTE: it is not necessary to call this method when you first instantiate
// an instance of the object.
//
// getTime:
//
// the method "unsigned long long getTime()" returns the timing result from
// a single timing instance.  The method ensures that this timer has
// only performed one measurement, and then returns the number of
// cycles for that measurement.

// The zen timer requires a patched version of the gnu assembler to work.
// Here is the patch file.  Simply cut it out of this file, remove the
// comment characters in the left two columns, and run it through patch.
//
// Note that while three undocumented opcodes are defined in this patch file
// "wrmsr", "rdmsr", and "rdtsc", only rdtsc is used in this file.  The two
// other opcodes are usable in kernel mode only.
// 
// RCS file: /r/cvs/tools/binutils2/include/opcode/i386.h,v
// retrieving revision 1.1.1.1
// diff -c -r1.1.1.1 i386.h
// *** /tmp/T0a28802	Thu Sep  7 22:44:33 1995
// --- i386.h	Thu Sep  7 22:14:39 1995
// ***************
// *** 429,435 ****
//   {"int3", 0, 0xcc, _, NoModrm, { 0, 0, 0} },
//   {"into", 0, 0xce, _, NoModrm, { 0, 0, 0} },
//   {"iret", 0, 0xcf, _, NoModrm, { 0, 0, 0} },
// ! /* i386sl (and i486sl?) only */
//   {"rsm", 0, 0x0faa, _, NoModrm,{ 0, 0, 0} },
//   
//   {"boundl", 2, 0x62, _, Modrm, { Reg32, Mem, 0} },
// --- 429,435 ----
//   {"int3", 0, 0xcc, _, NoModrm, { 0, 0, 0} },
//   {"into", 0, 0xce, _, NoModrm, { 0, 0, 0} },
//   {"iret", 0, 0xcf, _, NoModrm, { 0, 0, 0} },
// ! /* i386sl, i486sl(?) and later 486's, and Pentium only */
//   {"rsm", 0, 0x0faa, _, NoModrm,{ 0, 0, 0} },
//   
//   {"boundl", 2, 0x62, _, Modrm, { Reg32, Mem, 0} },
// ***************
// *** 729,734 ****
// --- 729,746 ----
//   {"wbinvd", 0, 0x0f09, _, NoModrm, { 0, 0, 0} },
//   {"invlpg", 1, 0x0f01, 7, Modrm, { Mem, 0, 0} },
//   
// + /* late 486 and Pentium extension */
// + 
// + {"cpuid", 0, 0x0fa2, _, NoModrm, { 0, 0, 0 } },
// + 
// + /* Pentium extensions (no CMPXCHG8B, I haven't yet figured out whether
// +    there's some arcane way to specify its operand or whether the
// +    rest of the assembler needs to be modified to support it) */
// + 
// + {"wrmsr", 0, 0x0f30, _, NoModrm, { 0, 0, 0 } },
// + {"rdmsr", 0, 0x0f31, _, NoModrm, { 0, 0, 0 } },
// + {"rdtsc", 0, 0x0f32, _, NoModrm, { 0, 0, 0 } },
// + 
//   {"", 0, 0, 0, 0, { 0, 0, 0} }	/* sentinel */
//   };
//   #undef _
// ***************
// *** 754,759 ****
// --- 766,772 ----
//     {"ds", SReg2, 3}, {"fs", SReg3, 4}, {"gs", SReg3, 5},
//     /* control registers */
//     {"cr0", Control, 0},   {"cr2", Control, 2},   {"cr3", Control, 3},
// +   {"cr4", Control, 4},
//     /* debug registers */
//     {"db0", Debug, 0},   {"db1", Debug, 1},   {"db2", Debug, 2},
//     {"db3", Debug, 3},   {"db6", Debug, 6},   {"db7", Debug, 7},
// 

// For more information about undocumented features of Intel processors,
// see the WEB page: http://x86.metronet.com/

// ---------- Start of Zen Timer Code -------------

#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class zen_timer {
  int output_file;
  bool                      timer_in_use;
  unsigned long long int    min_time;
  unsigned long long int    max_time;
  unsigned long long int    total_time;
  int                       number_of_calls;
  
  // Here I use a kind of tacky hack to get at the 64-bit time register
  // in the Pentium.  I declare a unsigned long long int (64-bit) as part
  // of a union and then declare two unsigned long ints (32-bit) to 
  // overlay this memory.  I then have the two halves of the register
  // pair stored into the memory for the entire 64-bit int.  It would
  // be better to move the entire 64 bits in one operation, but I don't
  // know how to do that (anyone care to teach me?)
  union {
    volatile unsigned long long int last_timer_value;
    struct {
      volatile unsigned long int   last_timer_value_low;
      volatile unsigned long int   last_timer_value_high;
    } on;
  };
  
  union {
    volatile unsigned long long int current_timer_value;
    struct {
      volatile unsigned long int current_timer_value_low;
      volatile unsigned long int current_timer_value_high;
    } off;
  };
  
  unsigned long long int total_pause_interval;
  unsigned int number_of_pauses;
  
  union {
    volatile unsigned long long int start_pause_interval;
    struct {
      volatile unsigned long int start_pause_interval_low;
      volatile unsigned long int start_pause_interval_high;
    } start_pause;
  };
  
  union {
    volatile unsigned long long int end_pause_interval;
    struct {
      volatile unsigned long int end_pause_interval_low;
      volatile unsigned long int end_pause_interval_high;
    } end_pause;
  };
  
public:
  inline zen_timer(char *file_name = NULL); // writes each time sample to
  // file_name
  
  inline ~zen_timer();               // closes the output file.
  
  inline void timer_on(void);        // turn on the timer
  
  inline void timer_off(void);       // turn off the timer
  
  inline void timer_pause(void);     // pause the recording of the time
                                     // interval
  
  inline void timer_resume(void);    // resume the recording of the time
                                     // interval
  
  inline void timer_reset(void);     // reset the timer to time=0, no calls
  
  inline void timer_print(void);     // print the avg number of
                                     // cycles/call

  inline unsigned long long getTime (); // Return the result of a
					// single timing
};


#define OVERHEAD (13)           // 14 cycles of overhead when run in user
                                // mode

#define PAUSE_OVERHEAD (31)  // 31 cycles of overhead when the
                             // timer is paused and resumed
                             // USE THIS OVERHEAD IF YOU HAVE A FUNCTION CALL

//#define PAUSE_OVERHEAD (27)   // It appears that if the interval
                                // being timed contains a function
                                // call then different code for the
                                // timer is generated, and a
                                // different overhead should be used.
                                // USE THIS OVERHEAD IF YOU DON'T HAVE A
                                // FUNCTION CALL.


inline zen_timer::zen_timer(char *file_name)
{
   timer_in_use = false;
   min_time = 0xffffffffffffffffll;
   max_time = 0;
   total_time = 0;
   number_of_calls = 0;
   current_timer_value = 0;
   last_timer_value = 0;

   // If this timer is created with a file name argument, then
   // open the dribble file.

   if (file_name != NULL) {
      output_file = open(file_name,O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (output_file == -1) {
	 cerr << "couldn't open output file " << file_name << endl;
	 exit(-1);
      }
   } else {
      output_file = 0;
   }
}


inline zen_timer::~zen_timer()
{
   if (output_file) {
      close(output_file);
   }
}


inline void zen_timer::timer_on(void)
{
   assert(timer_in_use == false);

   timer_in_use = true;
   total_pause_interval = 0;
   number_of_pauses = 0;

   // This inline assembly code calls the undocumented opcode "rdtsc" to
   // read the value of the tsc counter.  Then the value of this register
   // pair is saved.
   asm volatile ("rdtsc;
                  movl %%edx, %0;
                  movl %%eax, %1" 
		 : "=r" (on.last_timer_value_high),
		   "=r" (on.last_timer_value_low)
		 : /* no inputs   */
		 : "%edx", "%eax");
}


inline void zen_timer::timer_off(void) {
   assert(timer_in_use == true);

   asm volatile ("rdtsc;
                  movl %%edx, %0;
                  movl %%eax, %1"
		 : "=r" (off.current_timer_value_high),
		   "=r" (off.current_timer_value_low)
		 : /* no inputs */
		 : "%edx", "%eax");

   timer_in_use = false;
   unsigned long long int time;
   unsigned long long int output_time;

   time = current_timer_value - last_timer_value;
   time -= total_pause_interval;

   total_time += time;
   if (time > max_time) {
      max_time = time;
   }
   if (time < min_time) {
      min_time = time;
   }
   number_of_calls++;
   if (output_file) {
      output_time = time - OVERHEAD;
      write(output_file,(char *) &output_time, sizeof(unsigned long long int));
   }
}


inline void zen_timer::timer_pause(void) {
   assert(timer_in_use == true);

   asm volatile ("rdtsc;
                  movl %%edx, %0;
                  movl %%eax, %1"
		 : "=r" (start_pause.start_pause_interval_high),
		   "=r" (start_pause.start_pause_interval_low)
		 : /* no inputs */
		 : "%edx", "%eax");
}


inline void zen_timer::timer_resume(void) {
   assert(timer_in_use == true);

   unsigned long long int time;

   number_of_pauses++;
   asm volatile ("rdtsc;
                  movl %%edx, %0;
                  movl %%eax, %1"
		 : "=r" (end_pause.end_pause_interval_high),
		   "=r" (end_pause.end_pause_interval_low)
		 : /* no inputs */
		 : "%edx", "%eax");

   time = end_pause_interval - start_pause_interval;
#if (defined (PAUSE_OVERHEAD) && (PAUSE_OVERHEAD != 0))
   total_pause_interval += (time + PAUSE_OVERHEAD);
#else
   total_pause_interval += time;
#endif
}


inline void zen_timer::timer_reset(void) {
   timer_in_use = false;
   total_time = 0;
   number_of_calls = 0;
   total_pause_interval = 0;
   number_of_pauses++;
}


inline void zen_timer::timer_print(void) {
   if (timer_in_use == true) {
      cerr << "Error: Tried to print Zen Timer information while timer was running" << endl;
   }
   if (number_of_calls == 0) {
      cerr << "timer was not called." << endl;
   } else {
      cerr << endl;
      cerr << "timer was called " << number_of_calls << " times" << endl;
      cerr << endl;
      cerr << "for an average of " 
           << (total_time - (OVERHEAD * number_of_calls)) / number_of_calls
	   << " cycles per call" << endl;
      cerr << "the minimum time was: " << (min_time - OVERHEAD) << endl;
      cerr << "the maximum time was: " << (max_time - OVERHEAD) << endl;
      cerr << endl;
   }      
}


inline unsigned long long zen_timer::getTime () {
  if (number_of_calls != 1) {
    cerr << "getTime() can only be used on a timer called once."
	 << endl;
    exit(-1);
  } else {
    assert ((min_time == max_time) && (max_time == total_time));
    return (total_time - OVERHEAD);
  }
}



#endif
