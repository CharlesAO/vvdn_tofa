#ifndef RTEMS_FATAL_EXTENSION
#define RTEMS_FATAL_EXTENSION


#include <rtems.h>

// User extension to be able to catch abnormal terminations
void __attribute__((weak,unused)) Fatal_extension(
  Internal_errors_Source  the_source,
  bool                    is_internal,
  Internal_errors_t       the_error
);

#endif
