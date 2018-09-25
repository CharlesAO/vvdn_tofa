// Configure the function durations
#define LOOP_BASE (666 * 13)
#define MAX_FUNC_CNT (16)
#define LOOP_LENGTH_FUNC0(n) ((MAX_FUNC_CNT-n)*(LOOP_BASE))
#define LOOP_LENGTH_FUNC(n) (LOOP_LENGTH_FUNC0(n) + LOOP_LENGTH_FUNC0(n+1))
