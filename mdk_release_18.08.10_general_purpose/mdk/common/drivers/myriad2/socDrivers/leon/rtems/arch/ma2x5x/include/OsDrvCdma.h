///
/// @file OsDrvCdma.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvCdma OsCdma Driver
/// @{
/// @brief Public functions for RTEMS CMX DMA driver
///
///  Driver usage example:
///
///   #include "OsDrvCdma.h"
///   #define WAIT    1
///   #define NO_WAIT 0
///
///   static OsDrvCmxDmaTransaction list[LIST_SIZE]
///       __attribute__((section(".cmx_direct.bss")));
///   static OsDrvCmxDmaTransactionHnd dma_handle;
///
///   static OsDrvCmxDmaSetupStruct setup = {
///       .irq_priority = IRQ_PRIO,
///       .agent = DRV_CMX_DMA_AGENT1
///   };
///
///   // use NULL as parameter instead of config for default configuration
///   int32_t status = OsDrvCmxDmaInitialize(&config);
///
///   if (status == OS_MYR_DRV_SUCCESS) {
///     OsDrvCmxDmaCreateTransaction(&dma_handle, &list[0], src_buff, dst_buff,
///                                transfer_size);
///     for (i = 1; i < LIST_SIZE; i++)
///       OsDrvCmxDmaAddTransaction(&dma_handle, &list[i], src_buff, dst_buff,
///                               transfer_size);
///     // Blocking wait
///     OsDrvCmxDmaStartTransfer(&dma_handle, WAIT);
///     ...
///     // Non blocking
///     OsDrvCmxDmaStartTransfer(&dma_handle, NO_WAIT);
///     ...
///     // check transaction status
///     status = OsDrvCmxDmaGetTransactionStatus(&dma_handle, &tr_status);
///     if ((status == OS_MYR_DRV_SUCCESS) &&
///         (tr_status == OS_DRV_CMX_DMA_FINISHED)) {
///       //transaction finished
///     } else {
///       // transaction not finished yet
///     }
///   }
///


#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_RTEMS_INCLUDE_OSDRVCDMA_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_RTEMS_INCLUDE_OSDRVCDMA_H_

#ifdef USE_CMX_DMA_NEW_DRIVER

#include "OsDrvCdmaDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Initialize CMX DMA driver
///
/// This must be the first function called before using any other function form
/// the driver
///
/// @param [in] config Structure containing CMX DMA driver parameters
///
/// @return
///     OS_MYR_DRV_SUCCESS the driver was successfully initialized
///     OS_MYR_DRV_ERROR an error happened during driver initialization
///     OS_MYR_DRV_ALREADY_INITIALIZED the driver was already initialized by
///      a previous call to this function
///
int32_t OsDrvCmxDmaInitialize(OsDrvCmxDmaSetupStruct *config);

/// Configures transaction parameters
///
/// This function is optional. If it is not called the transaction will
/// use the default parameters set at it's creation.
///
/// @param [in] transaction the transaction that need to be configured
/// @param [in] params a structure containing the new configuration
///
/// @return
///     OS_MYR_DRV_SUCCESS the transaction was successfully configured
///     OS_MYR_DRV_ERROR the transaction couldn't be configured
///
int32_t OsDrvCmxDmaTransactionConfig(OsDrvCmxDmaTransaction *transaction,
                                     OsDrvCmxDmaConfParams *params);

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
///     OS_MYR_DRV_SUCCESS the transaction was successfully created
///     OS_MYR_DRV_ERROR the transaction could not be created
///
int32_t OsDrvCmxDmaCreateTransaction(OsDrvCmxDmaTransactionHnd *handle,
                                   OsDrvCmxDmaTransaction *transaction,
                                   uint8_t *src, uint8_t *dst, uint32_t size);

/// Creates and links a new 1D transaction to an existing transaction list
///
/// @param [out] handle for the list on which the new transaction will be added
/// @param [in] transaction the new transaction that need to be created and
///            added to the list
/// @param [in] src source address for the transaction
/// @param [in] dst destination address for the transaction
/// @param [in] size transaction size in bytes
///
/// @return
///     OS_MYR_DRV_SUCCESS the transaction was successfully added
///     OS_MYR_DRV_ERROR the new transaction could not be added
///
int32_t OsDrvCmxDmaAddTransaction(OsDrvCmxDmaTransactionHnd *handle,
                                  OsDrvCmxDmaTransaction *transaction,
                                  uint8_t *src, uint8_t *dst, uint32_t size);

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
///     OS_MYR_DRV_SUCCESS the transaction was successfully created
///     OS_MYR_DRV_ERROR the new transaction could not be created
///
int32_t OsDrvCmxDmaCreateStrideTransaction(
    OsDrvCmxDmaTransactionHnd *handle,
    OsDrvCmxDmaTransaction *transaction,
    uint8_t *src, uint8_t *dst,
    uint32_t src_width, uint32_t dst_width,
    uint32_t src_stride, uint32_t dst_stride,
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
///     OS_MYR_DRV_SUCCESS the transaction was successfully added
///     OS_MYR_DRV_ERROR the new transaction could not be added
///
int32_t OsDrvCmxDmaAddStrideTransaction(OsDrvCmxDmaTransactionHnd *handle,
                                        OsDrvCmxDmaTransaction *newTransaction,
                                        uint8_t *src, uint8_t *dst,
                                        uint32_t src_width, uint32_t dst_width,
                                        uint32_t src_stride,
                                        uint32_t dst_stride, uint32_t size);

/// @brief Creates a new 3D transaction.
/// @brief The function returns a handle to the new transaction.
/// @param [out] handle a handle to a transaction list containing only the newly
///             created transaction.
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
///     OS_MYR_DRV_SUCCESS the transaction was successfully created
///     OS_MYR_DRV_ERROR the new transaction could not be created
///

int32_t OsDrvCmxDmaCreate3DTransaction(
    OsDrvCmxDmaTransactionHnd *handle, OsDrvCmxDmaTransaction *transaction,
    uint8_t *src, uint8_t *dst, uint32_t src_width, uint32_t dst_width,
    uint32_t src_stride, uint32_t dst_stride, uint32_t num_planes,
    uint32_t src_plane_stride, uint32_t dst_plane_stride, uint32_t size);

/// @brief Creates and links a new 3D transaction to an existing transaction list.
/// @param [in] handle handle to the list on which the new transaction will be
///             added.
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
///             needs to be greater than zero for a 3D transaction.
/// @param [in] src_plane_stride plane stride size for source, defined as the
///             size in bytes from the start of a plane to the start of the
///             following plane.
/// @param [in] dst_plane_stride plane stride size for destination, defined as
///             the size in bytes from the start of a plane to the start of the
///             following plane.
///
/// @return
///     OS_MYR_DRV_SUCCESS the transaction was successfully added
///     OS_MYR_DRV_ERROR the new transaction could not be added
///
int32_t OsDrvCmxDmaAdd3DTransaction(
    OsDrvCmxDmaTransactionHnd *handle, OsDrvCmxDmaTransaction *newTransaction,
    uint8_t *src, uint8_t *dst, uint32_t src_width, uint32_t dst_width,
    uint32_t src_stride, uint32_t dst_stride, uint32_t num_planes,
    uint32_t src_plane_stride, uint32_t dst_plane_stride, uint32_t size);

/// Returns the current status of the transaction
///
/// @param [in] handle for a transaction or a list of transactions
/// @param [out] t_status transaction status; can be one of the following;
///     DRV_CMX_DMA_FINISHED - the transfer has finished
///     DRV_CMX_DMA_PENDING - transaction has been queued but it has not been
///     started yet
///
/// @return
///     OS_MYR_DRV_SUCCESS the status was retrieved successfully
///     OS_MYR_DRV_ERROR an error happened
///
int32_t OsDrvCmxDmaGetTransactionStatus(OsDrvCmxDmaTransactionHnd *handle,
                                        OsDrvCmxDmaTransactionStatus *t_status);

/// Create a new list by joining the lists given as parameters
///
/// @param [out] handle the handle for the new list
/// @param [in] list_cnt the number of lists to be joined
/// @param [in] ... handles for list_cnt lists to be joined
///
/// @return
///     OS_MYR_DRV_SUCCESS the transactions were successfully linked
///     OS_MYR_DRV_ERROR the transactions could not be linked
///
int32_t OsDrvCmxDmaLinkTransactions(OsDrvCmxDmaTransactionHnd *handle,
                                    uint32_t list_cnt, ...);

/// Start the DMA transfer for a transaction list
///
/// @param [in] handle for the transaction list
/// @param [in] wait if it's value is 0 this function will return immediately
///            after starting the transaction, otherwise the function will
///            block until the transfer finishes
///
/// @return
///     OS_MYR_DRV_SUCCESS the transfer was successfully started if wait is 0
//                      the transfer finished successfully if wait is non 0
///     OS_MYR_DRV_ERROR the transfer could not be started
///
int32_t OsDrvCmxDmaStartTransfer(OsDrvCmxDmaTransactionHnd *handle,
                                 uint32_t wait);

#ifdef __cplusplus
}
#endif

#endif // USE_CMX_DMA_NEW_DRIVER

#endif  //  COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_RTEMS_INCLUDE_OSDRVCDMA_H_


///@}
