typedef struct {
  union {
    volatile unsigned long long int timerValue;
    struct {
      volatile unsigned long int timerValueLow;
      volatile unsigned long int timerValueHigh;
    } separated;
  } startTime;

  union {
    volatile unsigned long long int timerValue;
    struct {
      volatile unsigned long int timerValueLow;
      volatile unsigned long int timerValueHigh;
    } separated;
  } stopTime;
} zenTimerType;

#define START_ZEN_TIME(userTimer) { \
  userTimer.startTime.timerValue = 0; \
  userTimer.stopTime.timerValue = 0; \
  \
  asm volatile ("rdtsc; movl %%edx, %0; movl %%eax, %1" \
		: "=r" (userTimer.startTime.separated.timerValueHigh), \
		"=r" (userTimer.startTime.separated.timerValueLow) \
		: \
		: "%edx", "%eax"); }

#define STOP_ZEN_TIME(userTimer,totalTime) { \
  asm volatile ("rdtsc; movl %%edx, %0; movl %%eax, %1" \
		: "=r" (userTimer.stopTime.separated.timerValueHigh), \
		"=r" (userTimer.stopTime.separated.timerValueLow) \
		: \
		: "%edx", "%eax"); \
  \
  totalTime = userTimer.stopTime.timerValue - userTimer.startTime.timerValue; }
