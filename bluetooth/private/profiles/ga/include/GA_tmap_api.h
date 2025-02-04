
/**
 *  \file GA_tmap_api.h
 *
 *  \brief This file defines the GA Telephony and Media Audio Profile(TMAP)
 *  Client and Server Interface - includes Data Structures and
 *  Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_TMAP_API_
#define _H_GA_TMAP_API_

/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_bearer_api.h"
#include "GA_property_config.h"

/**
 * \addtogroup bt_ga_appl_profiles Application Profiles
 * \ingroup bt_ga_profiles
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * GA Application Profiles.
 */

/**
 * \defgroup tmap_module Telephony and Media Audio Profile (TMAP)
 * \ingroup bt_ga_appl_profiles
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * GA TMAP (TMAP) module to the Application and other upper
 * layers of the stack.
 */

/* --------------------------------------------- Global Definitions */

/**
 * \defgroup tmap_defines Defines
 * \{
 * \brief This section describes the various Defines in EtherMind GA
 * TMAP Layer.
 */

/**
 * \defgroup tmap_error_code Error Codes
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of TMAP.
 */

/** \} */

/**
 * \defgroup tmap_constants Constants
 * \{
 * \brief This section describes the various Constants in EtherMind GA
 * TMAP Layer.
 */

/**
 * \name TMAP Roles
 * \{
 * \brief Bitmask Field
 * 0 = Server does not support. \n
 * 1 = Server supports.
 */

/** TMAP Role - Call Gateway (CG) */
#define TMAP_ROLE_CG    (0x0001 << 0U)
/** TMAP Role - Call Terminal (CT) */
#define TMAP_ROLE_CT    (0x0001 << 1U)
/** TMAP Role - Unicast Media Sender (UMS) */
#define TMAP_ROLE_UMS   (0x0001 << 2U)
/** TMAP Role - Unicast Media Receiver (UMR) */
#define TMAP_ROLE_UMR   (0x0001 << 3U)
/** TMAP Role - Broadcast Media Sender (BMS) */
#define TMAP_ROLE_BMS   (0x0001 << 4U)
/** TMAP Role - Broadcast Media Receiver (BMR) */
#define TMAP_ROLE_BMR   (0x0001 << 5U)

/** \} */

/**
 * \name Client - General Macros
 * \{
 * \brief Initialization and other General Macros offered by the module.
 */

/**
 * Total number of Characteristic IDs,
 * \ref tmap_constants
 */
#define TMAP_CHAR_ID_COUNT               0x01U

/** \} */

/**
 * \name Client - TMAP Char ID
 * \{
 * This section lists the Characteristic ID references.
 */

/**
 * TMAP Char ID: Role,
 * \ref GA_CHAR_PROP_TMAP_ROLE
 */
#define TMAP_CHAR_ID_ROLE       0U

/** \} */

/** \} */

/**
 * \defgroup tmap_events Events
 * \{
 * \brief This section lists the Asynchronous and Synchronous Events
 * notified to Application by EtherMind GA TMAP Layer.
 */

/**
 * \defgroup tmap_client_events Client
 * \{
 * \brief This section describes the Telephony and Media Audio Profile Events
 * for Client.
 */

/**
 * \name TMAP Client Events - Setup
 * \{
 */

/**
 * \brief This event indicates the setup of a TMAS context.
 * Callback is triggered once discovery of service and char is complete. \n
 * Callback is triggered with the following values as parameters in the
 * \ref TMAP_NTF_CB callback. \n
 * TMAP Role is also read and notified during the process of discovery.
 *
 * \param [in] device Pointer to peer device handle \ref
 * \param [in] evt_id \ref TMAP_SETUP_CNF
 * \param [in] evt_status \ref GA_SUCCESS : Service Discovery Complete \n
 *                        \ref GA_FAILURE : Others \n
 * \param [in] evt_data Pointer to object of type \ref TMAP_EVT \n
 *                      If status is \ref GA_SUCCESS \n
 *                         - tmap_handle: TMAP Handle for the Device \n
 *                         - data - Pointer to object of type \ref TMAP_ROLE \n
 *                         - len - \ref sizeof ( \ref TMAP_ROLE ) \n
 *                         .
 *                      If status is \ref GA_FAILURE, \n
 *                         - tmap_handle: TMAP Handle for the Device \n
 *                         - data - NULL \n
 *                         - len - 0 \n
 *                         .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define TMAP_SETUP_CNF                            0x01U

/** \} */

/**
 * \name TMAP Client Events - Release
 * \{
 */

/**
 * \brief This event indicates the release of a TMAS context.
 * Callback is triggered once the context is cleared of any discovered
 * handles and properties. \n
 * Callback is triggered with the following values as parameters in the
 * \ref TMAP_NTF_CB callback.
 *
 * \param [in] device Pointer to peer device handle \ref
 * \param [in] evt_id \ref TMAP_RELEASE_CNF
 * \param [in] evt_status \ref GA_SUCCESS on release \n
 * \param [in] evt_data Pointer to object of type \ref TMAP_EVT \n
 *                         - tmap_handle: TMAP Handle for the Device \n
 *                         - data - NULL \n
 *                         - len - 0 \n
 *                         .
 */
#define TMAP_RELEASE_CNF                          0x02U

/** \} */

/** \} */

/** \} */

/* --------------------------------------------- Data types /Structures */

/**
 * \defgroup tmap_structures Structures
 * \{
 * \brief This section describes the various Data-Types and Structures in
 * EtherMind GA TMAP Layer.
 */

/**
 * The field in the handle indicates the Handle ID.
 * The application needs to provide this as a input param for all the calls
 * to have transaction on the required TMAP handle.
 */
typedef UINT8 TMAP_HANDLE;

/** Event that will be notified by the TMAP module. */
typedef UINT8 TMAP_EVT_ID;

/** Roles defined for TMAP */
typedef UINT16 TMAP_ROLE;

/** TMAP Event */
typedef struct _TMAP_EVT
{
    /** TMAP Handle on which TMAP event is receievd */
    TMAP_HANDLE   tmap_handle;

    /** Data associated with the event */
    void       * data;

    /** Length of the event data */
    UINT16       len;

}TMAP_EVT;

/** \} */

/* --------------------------------------------- Macros */


/** \} */

/* --------------------------------------------- Callback Interfaces */
/**
 * \defgroup tmap_cb Application Callback
 * \{
 * \brief This section describes the Notification Callback Interfaces offered
 * to the application by EtherMind GA TMAP Layer.
 */

/**
 * TMAP Callback to be registered by the Application. \n
 * TMAP Layer calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param device     peer device info
 * \param evt_id     current event ID
 * \param evt_status status of the current event
 * \param evt_data   current event data
 */
typedef GA_RESULT (* TMAP_NTF_CB)
                  (
                      GA_ENDPOINT * device,
                      TMAP_EVT_ID   evt_id,
                      UINT16        evt_status,
                      TMAP_EVT    * evt_data
                  );

/** \} */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup tmap_api_defs API Definitions
 * \{
 * \brief This section describes the various APIs exposed by
 * EtherMind GA TMAP Layer to the Application and other upper
 * layers of the stack.
 */

/**
 * \defgroup tmap_common_api Common
 * \{
 * \brief This section describes the Telephony and Media Audio Profile APIs
 * common for Client and Server.
 */

/**
 * \name TMAP Common APIs - Init
 * \brief This section describes the Telephony and Media Audio Profile Init APIs
 * \{
 */

/**
 *  \brief To Init the TMAP Client and Server Module.
 *
 *  \par Description:
 *       This function enables to initialize the TMAP Client and Server
 *       Module. \n
 *       This function enables to register a callback with TMAP Module.
 *       This callback will be triggered whenever there are events generated
 *       either due to requests/responses or notifications from peer.
 *
 *  \param [in] cb
 *         TMAP Callback.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *
 *  \sa tmap_error_code
 */
GA_RESULT GA_tmap_init
          (
              /* IN */ TMAP_NTF_CB cb
          );

/** \} */

/**
 * \name TMAP Common APIs - De-Init
 * \brief This section describes the Telephony and Media Audio Profile
 * De-Init APIs.
 * \{
 */

/**
 *  \brief To Shutdown the TMAP Client and Server Module.
 *
 *  \par Description:
 *       This function enables to Shutdown the TMAP Client and Server
 *       module. \n
 *       This function de-references the callback registered with GA TMAP
 *       Module. No events generated at the TMAP layer will be triggered
 *       post this function call.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *
 *  \sa tmap_error_code
 */
GA_RESULT GA_tmap_shutdown(void);

/** \} */

/**
 * \name TMAP Common APIs - Init
 * \brief This section describes the Telephony and Media Audio Profile
 * Init APIs.
 * \{
 */
/**
 *  \brief To register TMAP Role.
 *
 *  \par Description:
 *       This function registers TMAP Profile/TMAP Service and Char. TMAP Role
 *       is also registered in case of a Server.
 *
 *  \param [in] client \n
 *         \ref GA_TRUE - Refers the device to be registered as a Client. \n
 *         \ref GA_FALSE - Refers the device to be registerd as a Server. \n
 *
 *  \param [in] role
 *         Refer \ref TMAP_ROLE.
 *         In case client is \ref GA_TRUE, role shall be ignored.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *
 *  \sa tmap_error_code
 */
GA_RESULT GA_tmap_register_role
          (
              /* IN */  UINT8     client,
              /* IN */  TMAP_ROLE role
          );
/** \} */

/** \} */

#ifdef TMAP_CLIENT

/**
 * \defgroup tmap_client_api Client
 * \{
 * \brief This section describes the Telephony and Media Audio Profile APIs
 * for Client.
 */

/**
 * \name TMAP Client APIs - Setup
 * \{
 */

/**
 *  \brief To Setup Telephony and Media Audio Service(TMAS) Context.
 *
 *  \par Description:
 *       This function enables to discover TMAS context with given device.
 *       Internally, TMAP service and char for the service is discovered.
 *       TMAP Role is also read and the same is notified during the process
 *       of discover.
 *
 *  \param [in] device
 *         Peer Device with which the context needs to be discovered.
 *
 *  \param [out] tmap_handle
 *         TMAP Handle.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h. \n
 *          If \ref GA_SUCCESS, \ref TMAP_SETUP_CNF is notified on
 *          completion with status as success or failure.
 *
 *  \sa tmap_client_events
 *  \sa tmap_error_code
 */
GA_RESULT GA_tmap_setup
          (
              /* IN */   GA_ENDPOINT * device,
              /* OUT */  TMAP_HANDLE * tmap_handle
          );

/** \} */

/**
 * \name TMAP Client APIs - Setup
 * \{
 */

/**
 * \cond EXCLUDE_PART
 */
#ifdef TMAP_SUPPORT_CONTEXT_MANAGE
/**
 *  \brief To manage the GA TMAP characteristic handles of a device.
 *
 *  \par Description:
 *       The routine enables the application to get/set the service and
 *       characteristic handle ranges from/to a device context.
 *
 *  \param [in] set
 *         \ref GA_TRUE for Context SET and \ref GA_FALSE for Context GET.
 *
 *  \param [in] device
 *         Remote device endpoint.
 *
 *  \param [inout] tmap_handle
 *         Context handle for the endpoint.
 *
 *  \param [inout] range
 *         Service range array for TMAS.
 *
 *  \param [inout] info
 *         Array of characteristic handles for TMAS.
 *
 *  \param [inout] info_count
 *         Count of characteristics handles in above parameter.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *
 *  \sa tmap_error_code
 */
GA_RESULT GA_tmap_manage_context_info
          (
              /* IN     */ UINT8              set,
              /* IN     */ GA_ENDPOINT      * device,
              /* INOUT  */ TMAP_HANDLE      * tmap_handle,
              /* INOUT  */ GA_BRR_SVC_RANGE ** range,
              /* INOUT  */ GA_CHAR_INFO     ** info,
              /* INOUT  */ UINT16           * info_count
          );

/**
 *  \brief To get the GA TMAP characteristic handles of a device.
 *
 *  \par Description:
 *       The routine enables the application to get the service and
 *       characteristic handle ranges from a device context.
 *
 *  \param [in] d
 *         Remote device endpoint.
 *
 *  \param [inout] h
 *         Context handle for the endpoint.
 *
 *  \param [inout] r
 *         Service range array for TMAS.
 *
 *  \param [inout] i
 *         Array of characteristic handles for TMAS.
 *
 *  \param [inout] c
 *         Count of characteristics handles in above parameter.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *
 *  \sa tmap_error_code
 */
#define GA_tmap_get_context_info(d, h, r, i, c)              \
        GA_tmap_manage_context_info                          \
        (                                                    \
            GA_FALSE,                                        \
            (d),                                             \
            (h),                                             \
            (r),                                             \
            (i),                                             \
            (c)                                              \
        )

/**
 *  \brief To set the GA TMAP characteristic handles of a device.
 *
 *  \par Description:
 *       The routine enables the application to set the service and
 *       characteristic handle ranges to a device context.
 *
 *  \param [in] d
 *         Remote device endpoint.
 *
 *  \param [inout] h
 *         Context handle for the endpoint.
 *
 *  \param [inout] r
 *         Service range array for TMAS.
 *
 *  \param [inout] i
 *         Array of characteristic handles for TMAS.
 *
 *  \param [inout] c
 *         Count of characteristics handles in above parameter.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *
 *  \sa tmap_error_code
 */
#define GA_tmap_set_context_info(d, h, r, i, c)              \
        GA_tmap_manage_context_info                          \
        (                                                    \
            GA_TRUE,                                         \
            (d),                                             \
            (h),                                             \
            (r),                                             \
            (i),                                             \
            (c)                                              \
        )

#endif /* TMAP_SUPPORT_CONTEXT_MANAGE */

/**
 * \endcond
 */

/** \} */

/**
 * \name TMAP Client APIs - Release
 * \{
 */

/**
 *  \brief Close or Release the given TMAP context.
 *
 *  \par Description:
 *       When 'release' is set to \ref GA_TRUE, this routine initiates the
 *       release procedure for the context. Once release is done, the context
 *       is freed up and the setup must be freshly done by calling
 *       \ref GA_tmap_setup() if required for the same device again. \n
 *       If the 'release' parameter is set to \ref GA_FALSE, this API just
 *       frees the context without the release procedure.
 *
 *  \param [in] tmap_handle
 *         TMAP Context for the endpoint to be released/freed.
 *
 *  \param [in] release
 *         \ref GA_TRUE : Indicates release with freeing of context \n
 *         \ref GA_FALSE : Indicates only freeing of context
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h. \n
 *          If \ref GA_SUCCESS, \ref TMAP_RELEASE_CNF is notified on
 *          completion with status as success or failure.
 *
 *  \sa tmap_client_events
 *  \sa tmap_error_code
 */
GA_RESULT GA_tmap_terminate
          (
              /* IN */  TMAP_HANDLE tmap_handle,
              /* IN */  UCHAR       release
          );

/**
 *  \brief Release the given TMAP context.
 *
 *  \par Description:
 *       This routine initiates the release procedure for the context. Once
 *       release is done, the context is freed up and the setup must be freshly
 *       done by calling \ref GA_tmap_setup() if required for the
 *       same device again.
 *
 *  \param [in] tmap_handle
 *         TMAP Context for the endpoint to be released.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h. \n
 *          If \ref GA_SUCCESS, \ref TMAP_RELEASE_CNF is notified on
 *          completion with status as success or failure.
 *
 *  \sa tmap_client_events
 *  \sa tmap_error_code
 */
#define GA_tmap_release(tmap_handle)                         \
        GA_tmap_terminate                                    \
        (                                                    \
            (tmap_handle),                                   \
            GA_TRUE                                          \
        )

/**
 *  \brief Free the given TMAP context.
 *
 *  \par Description:
 *       This routine frees up the given context of the TMAP.
 *
 *  \param [in] tmap_handle
 *         TMAP Context for the endpoint to be freed.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *
 *  \sa tmap_error_code
 */
#define GA_tmap_close(tmap_handle)                           \
        GA_tmap_terminate                                    \
        (                                                    \
            (tmap_handle),                                   \
            GA_FALSE                                         \
        )

/** \} */

/** \} */

#endif /* TMAP_CLIENT */

/** \} */

#ifdef __cplusplus
};
#endif

/** \} */
/** \} */

#endif /* _H_GA_TMAP_API_ */
