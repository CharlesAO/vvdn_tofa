///
/// @file scCmxDma.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup scCmxDma scCmxDma Driver
/// @{
/// @brief     PC MODEL CMX DMA driver API
///


#ifndef PC_MODEL_COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_INCLUDE_SCCMXDMA_H_
#define PC_MODEL_COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_INCLUDE_SCCMXDMA_H_

#define USE_CMX_DMA_NEW_DRIVER
#ifdef USE_CMX_DMA_NEW_DRIVER

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdarg.h>
#include <stdint.h>
#include "scCmxDmaDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2: Defines
// ----------------------------------------------------------------------------
// Default source/destination plane stride and number of plane
// value used when calling the scCmxDmaCreateTransaction function
// for 1D or 2D DMA transaction.
#define SC_CMX_DMA_DEFAULT_NUM_PLANE (0)
#define SC_CMX_DMA_DEFAULT_PLANE_STRIDE (0)

// Valid range for the number of planes.
#define MIN_NUM_PLANES (1)
#define MAX_NUM_PLANES (256)

/// @brief Common return values for all BM layer SHAVE drivers
typedef enum
{
    PC_DRV_SUCCESS = 0,
    PC_DRV_ERROR,
    PC_DRV_NOT_INITIALIZED,
    PC_DRV_ALREADY_INITIALIZED,
    PC_DRV_RESOURCE_BUSY,
    PC_DRV_INVALID_PARAM,
    PC_DRV_RET_NB
} DRV_RETURN_TYPE;

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// Initializes CMX DMA driver
///
/// This must be the first function called before using any other function form
/// the driver
///
/// @param [in] config Structure containing CMX DMA driver parameters
///
/// @return
///     MYR_DRV_SUCCESS the driver was successfully initialized
///     MYR_DRV_ERROR an error happened during driver initialization
///     MYR_DRV_ALREADY_INITIALIZED the driver was already initialized by
///      a previous call to this function
///
int32_t ScCmxDmaInitialize(ScCmxDmaSetupStruct *config);

/// Configures transaction parameters
///
/// This function is optional. If it is not called the transaction will
/// use the default parameters set at it's creation.
///
/// @param [in] transaction the transaction that need to be configured
/// @param [in] params a structure containing the new configuration
///
/// @return
///     MYR_DRV_SUCCESS the transaction was successfully configured
///     MYR_DRV_ERROR the couldn't be configured
///
int32_t ScCmxDmaTransactionConfig(ScCmxDmaTransaction *transaction,
                                  ScCmxDmaConfParams *params);
/// Creates a new 1D transaction
///
/// The function returns a handle to the new transaction
///
/// @param [out] handle a handle to a transaction list containing only the newly
///             created transaction
/// @param [in] transaction the new transaction to be created
/// @param [in] src source address for the transaction
/// @param [in] dst destination address for the transaction
/// @param [in] size transaction size in bytes
///
/// @return
///     MYR_DRV_SUCCESS the transaction was successfully created
///     MYR_DRV_ERROR the transaction could not be created
///
int32_t ScCmxDmaCreateTransaction(ScCmxDmaTransactionHnd *handle,
                                  ScCmxDmaTransaction *transaction,
                                  uint8_t *src, uint8_t *dst,
                                  uint32_t size);

/// Creates a new 2D transaction
///
/// The function returns a handle to the new transaction
///
/// @param [out] handle a handle to a transaction list containing only the newly
///             created transaction
/// @param [in] transaction the new transaction to be created
/// @param [in] src source address for the transaction
/// @param [in] dst destination address for the transaction
/// @param [in] src_width line width for source in bytes
/// @param [in] dst_width line width for destination in bytes
/// @param [in] src_stride stride size for source, defined as the size in bytes
///            from the start of a line to the start of the following line
/// @param [in] dst_stride stride size for destination, defined as the size in
/// bytes
///            from the start of a line to the start of the following line
/// @param [in] size transaction size in bytes
///
/// @return
///     MYR_DRV_SUCCESS the transaction was successfully created
///     MYR_DRV_ERROR the new transaction could not be created
///
int32_t ScCmxDmaCreateStrideTransaction(ScCmxDmaTransactionHnd *handle,
                                        ScCmxDmaTransaction *transaction,
                                        uint8_t *src, uint8_t *dst,
                                        uint32_t src_width, uint32_t dst_width,
                                        uint32_t src_stride, uint32_t dst_stride,
                                        uint32_t size);

/// Creates and links a new 1D transaction to an existing transaction list
///
/// @param [out] handle for the list on which the new transaction will be added
/// @param [in] transaction the new transaction that need to be created and
///             added to the list
/// @param [in] src source address for the transaction
/// @param [in] dst destination address for the transaction
/// @param [in] size transaction size in bytes
///
/// @return
///     MYR_DRV_SUCCESS the transaction was successfully added
///     MYR_DRV_ERROR the new transaction could not be added
///
int32_t ScCmxDmaAddTransaction(ScCmxDmaTransactionHnd *handle,
                               ScCmxDmaTransaction *transaction,
                               uint8_t *src, uint8_t *dst,
                               uint32_t size);

/// Creates and links a new 2D transaction to an existing transaction list
///
/// @param [in] handle handle to the list on which the new transaction will be
/// added
/// @param [in] transaction the new trasaction to be added
/// @param [in] src source address for the transaction
/// @param [in] dst destination address for the transaction
/// @param [in] src_width line width for source in bytes
/// @param [in] dst_width line width for destination in bytes
/// @param [in] src_stride stride size for source, defined as the size in bytes
///            from the start of a line to the start of the following line
/// @param [in] dst_stride stride size for destination, defined as the size in
/// bytes
///            from the start of a line to the start of the following line
/// @param [in] size transaction size in bytes
///
/// @return
///     MYR_DRV_SUCCESS the transaction was successfully added
///     MYR_DRV_ERROR the new transaction could not be added
///
int32_t ScCmxDmaAddStrideTransaction(ScCmxDmaTransactionHnd *handle,
                                     ScCmxDmaTransaction *transaction,
                                     uint8_t *src, uint8_t *dst,
                                     uint32_t src_width, uint32_t dst_width,
                                     uint32_t src_stride, uint32_t dst_stride,
                                     uint32_t size);

/// @brief Creates a new 3D transaction.
/// @brief The function returns a handle to the new transaction.
/// @param [out] handle a handle to a transaction list containing only the newly
///                     created transaction.
/// @param [in] transaction the new 3D transaction to be created.
/// @param [in] src source address for the transaction.
/// @param [in] dst destination address for the transaction.
/// @param [in] src_width line width for source in bytes.
/// @param [in] dst_width line width for destination in bytes.
/// @param [in] src_stride stride size for source, defined as the size in bytes
///             from the start of a line to the start of the following line.
/// @param [in] dst_stride stride size for destination, defined as the size in
///             bytes from the start of a line to the start of the following line.
/// @param [in] num_planes number of planes of data to be transfered. The value
///             needs to be greater than zero for 3D transaction.
/// @param [in] src_plane_stride plane stride size for source, defined as the
///             size in bytes from the start of a plane to the start of the
///             following plane.
/// @param [in] dst_plane_stride plane stride size for destination, defined as
///             the size in bytes from the start of a plane to the start of the
///             following plane.
/// @param [in] size transaction size in bytes.
///
/// @return
///     MYR_DRV_SUCCESS the transaction was successfully created
///     MYR_DRV_ERROR the new transaction could not be created
///
int32_t ScCmxDmaCreate3DTransaction(
    ScCmxDmaTransactionHnd *handle, ScCmxDmaTransaction *transaction,
    uint8_t *src, uint8_t *dst, uint32_t src_width, uint32_t dst_width,
    uint32_t src_stride, uint32_t dst_stride, uint32_t num_planes,
    uint32_t src_plane_stride, uint32_t dst_plane_stride, uint32_t size);

/// @brief Creates and links a new 3D transaction to an existing transaction list.
/// @param [in] handle handle to the list on which the new transaction will be
///                    added.
/// @param [in] transaction the new 3D transaction to be added.
/// @param [in] src source address for the transaction.
/// @param [in] dst destination address for the transaction.
/// @param [in] src_width line width for source in bytes.
/// @param [in] dst_width line width for destination in bytes.
/// @param [in] src_stride stride size for source, defined as the size in bytes
///             from the start of a line to the start of the following line.
/// @param [in] dst_stride stride size for destination, defined as the size in
///             bytes from the start of a line to the start of the following line.
/// @param [in] num_planes number of planes of data to be transfered. The value
///             needs to be greater than zero for 3D transaction.
/// @param [in] src_plane_stride plane stride size for source, defined as the
///             size in bytes from the start of a plane to the start of the
///             following plane.
/// @param [in] dst_plane_stride plane stride size for destination, defined as
///             the size in bytes from the start of a plane to the start of the
///             following plane.
/// @param [in] size transaction size in bytes.
///
/// @return
///     MYR_DRV_SUCCESS the transaction was successfully added
///     MYR_DRV_ERROR the new transaction could not be added
///
int32_t ScCmxDmaAdd3DTransaction(
    ScCmxDmaTransactionHnd *handle, ScCmxDmaTransaction *transaction,
    uint8_t *src, uint8_t *dst, uint32_t src_width, uint32_t dst_width,
    uint32_t src_stride, uint32_t dst_stride, uint32_t num_planes,
    uint32_t src_plane_stride, uint32_t dst_plane_stride, uint32_t size);

/// Create a new list by joining the lists given as parameters
///
/// @param [out] handle the handle for the new list
/// @param [in] list_cnt the number of lists to be joined
/// @param [in] ... handles for list_cnt lists to be joined
///
/// @return
///     MYR_DRV_SUCCESS the transactions were successfully linked
///     MYR_DRV_ERROR the transactions could not be linked
///
int32_t ScCmxDmaLinkTransactions(ScCmxDmaTransactionHnd *handle,
                                 uint32_t list_cnt, ...);

/// Start the DMA transfer for a transaction list
///
/// @param [in] handle for the transaction list
///
/// @return
///     MYR_DRV_SUCCESS the transaction was successfully started
///     MYR_DRV_ERROR the couldn't be started
///
///     This function doesn't wait for the transfer to finish. It returns
///     immediately after the transfer starts.
///
int32_t ScCmxDmaStartTransfer(ScCmxDmaTransactionHnd *handle);

/// Waits for one or more transactions started by ScCmxDmaStartTransfer to
/// finish
///
/// @param [in] handle for a transaction or a list of transactions
///
/// @return
///     MYR_DRV_SUCCESS the transaction finished successfully
///     MYR_DRV_ERROR transfer finished with an error
///
int32_t ScCmxDmaWaitTransaction(ScCmxDmaTransactionHnd *handle);

/// Returns the current status of the transaction
///
/// @param [in] handle for a transaction or a list of transactions
/// @param [out] t_status transaction status; can be one of the following;
///     DRV_CMX_DMA_FINISHED - the transfer has finished
///     DRV_CMX_DMA_PENDING - transaction has been queued but it has not been
///     started yet
///
/// @return
///     MYR_DRV_SUCCESS the status was retrieved successfully
///     MYR_DRV_ERROR an error happened
///
int32_t ScCmxDmaGetTransactionStatus(ScCmxDmaTransactionHnd *handle,
                                     ScCmxDmaTransactionStatus *t_status);

#ifdef __cplusplus
}
#endif

#endif  // USE_CMX_DMA_NEW_DRIVER

#endif  // PC_MODEL_COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_INCLUDE_SCCMXDMA_H_


///@}
