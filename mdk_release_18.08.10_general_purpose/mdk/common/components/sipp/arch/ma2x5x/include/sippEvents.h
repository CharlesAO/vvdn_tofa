// -----------------------------------------------------------------------------
// Copyright (C) 2015 Movidius Ltd. All rights reserved
//
// Company          : Movidius
// Description      : SIPP Events for use in callback when pipelines report events
//
// -----------------------------------------------------------------------------

#ifndef _SIPP_EVENTS_H_
#define _SIPP_EVENTS_H_

#include <sippBaseTypes.h>

/////////////////////////////////////////////////////////////////////////////////
//  Global types / structure definitions
/////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// Decode Event list

typedef enum
{
    /* (0x0) Pipeline events */
    eSIPP_PIPELINE_FINALISED = 0x0, // Pipeline has been finalised
    eSIPP_PIPELINE_RESCHEDULED,     // Pipeline rescheduling complete
    eSIPP_PIPELINE_FRAME_DONE,      // Frame complete event for pipeline
    eSIPP_PIPELINE_ITERS_DONE,      // Iterations complete event for pipeline
    eSIPP_PIPELINE_SYNC_OP_DONE,    // Internal event passed to access scheduler to trigger it to update pipeline status
    eSIPP_PIPELINE_STARTED          // Pipeline has been internally scheduled and is to commence operation

} eSIPP_PIPELINE_EVENT;

//////////////////////////////////////////////////////////////
// Event data Types
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// SIPP Event info structure

typedef void SIPP_PIPELINE_EVENT_DATA;

//////////////////////////////////////////////////////////////
// Decode sequence info data

typedef struct
{
    u32 uDummy;

} SIPP_PIPELINE_FINALISED_DATA, *pSIPP_PIPELINE_FINALISED_DATA;


#endif /* _SIPP_EVENTS_H_ */

/* End of file */
