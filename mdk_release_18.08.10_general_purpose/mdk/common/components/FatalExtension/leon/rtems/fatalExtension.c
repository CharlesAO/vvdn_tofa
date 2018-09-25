#include <rtems.h>
#include <rtems/bspIo.h>

#include "fatalExtension.h"

// User extension to be able to catch abnormal terminations
void __attribute__((weak,unused)) Fatal_extension(
    Internal_errors_Source  the_source,
    bool                    is_internal,
    Internal_errors_t       the_error
    )
{
    switch(the_source)
    {
        case RTEMS_FATAL_SOURCE_EXIT:
            if(the_error){
                printk("Exited with error code %d\n", the_error);
            }
            break; // normal exit
        case RTEMS_FATAL_SOURCE_ASSERT:
            printk("%s : %d in %s \n",
                ((rtems_assert_context *)the_error)->file,
                ((rtems_assert_context *)the_error)->line,
                ((rtems_assert_context *)the_error)->function);
            break;
        case RTEMS_FATAL_SOURCE_EXCEPTION:
            rtems_exception_frame_print((const rtems_exception_frame *) the_error);
            break;
        default:
            printk("\nFatal %serror %s\n", is_internal?"internal ":"", rtems_fatal_source_text(the_source));
            printk ("Error %d \n", the_error);
            if(is_internal)
            {
                printk ("Internal error %s \n", rtems_internal_error_text(the_error));
            }
            break;
    }
}

