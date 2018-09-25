///
/// @file status-checks.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup status-checks status-checks Driver
/// @{
/// @brief     Cross Status Checks
///

#ifndef __STATUS_CHECKS_H__
#define __STATUS_CHECKS_H__

#define RTEMS_CHECK_SC(code_, msg_)																OsCrossCheckStatusCode(code_, msg_)		
#define RTEMS_CHECK_SC_RV(code_, msg_)															OsCrossCheckStatusCode(code_, msg_)

#endif
///@}
