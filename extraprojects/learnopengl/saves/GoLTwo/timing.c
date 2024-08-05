/*
 * timing utilities
 */
#define TIMING_MILLION 1000000
#define TIMING_BILLION 1000000000

typedef struct TimingTimer {
    u64 start_sec;
    u64 start_mil;
    u64 start_mic;
    u64 start_nan;

    u64 elapsed_sec;
    u64 elapsed_mil;
    u64 elapsed_mic;
    u64 elapsed_nan;

    u64 last_read_mil;
} TimingTimer;

/*
 * default times in milliseconds
 */
static struct timespec _timingInternalClock;

static u64 _timingInternalStartSec;
static u64 _timingInternalNowSec;

static u64 _timingInternalStart;
static u64 _timingInternalNow;

static u64 _timingInternalStartNS;
static u64 _timingInternalNowNS;

u64
timingUpdateTime()
{
    clock_gettime(CLOCK_REALTIME, &_timingInternalClock);
    u64 time_nan            = _timingInternalClock.tv_sec * TIMING_BILLION + _timingInternalClock.tv_nsec;
    _timingInternalNowNS  = time_nan;
    _timingInternalNow    = time_nan / TIMING_MILLION;
    _timingInternalNowSec = time_nan / TIMING_BILLION;
}

u64
timingGetTime()
{
    return _timingInternalNow;
}

u64
timingGetTimeNS()
{
    return _timingInternalNowNS;
}

u64
timingStartTimer(TimingTimer *timer)
{
    clock_gettime(CLOCK_REALTIME, &_timingInternalClock);
    u64 time_nan           = _timingInternalClock.tv_sec * TIMING_BILLION + _timingInternalClock.tv_nsec;
    timer->start_nan = time_nan;
    timer->start_mic = time_nan / 1000;
    timer->start_mil = time_nan / TIMING_MILLION;
    timer->start_sec = time_nan / TIMING_BILLION;

    timer->elapsed_nan = 0;
    timer->elapsed_mic = 0;
    timer->elapsed_mil = 0;
    timer->elapsed_sec = 0;

    timer->last_read_mil = timer->start_mil;

    return  timer->start_mil;
}

u64
timingReadTimer(TimingTimer *timer)
{
    clock_gettime(CLOCK_REALTIME, &_timingInternalClock);
    u64 time_nan           = _timingInternalClock.tv_sec * TIMING_BILLION + _timingInternalClock.tv_nsec;

    timer->elapsed_nan = time_nan - timer->start_nan;
    timer->elapsed_mic = time_nan / 1000 - timer->start_mic;
    timer->elapsed_mil = time_nan / TIMING_MILLION - timer->start_mil;
    timer->elapsed_sec = time_nan / TIMING_BILLION - timer->start_sec;

    u64 delta_t_mil = timer->elapsed_mil - timer->last_read_mil;
    timer->last_read_mil = timer->elapsed_mil;
    return delta_t_mil;
}
