/**
 *  \file ots_extern.h
 *
 *  \brief This file defines the Object Transfer Server extern usage.
 */

 /*
  *  Copyright (C) 2020. Mindtree Ltd.
  *  All rights reserved.
  */


#ifndef _H_OTS_EXTERN_
#define _H_OTS_EXTERN_

/* --------------------------------------------- Header File Inclusion */

#ifdef OTP_SERVER

/* --------------------------------------------- Global Definitions */
/* Indicates number of applications registered with OTS module */
extern UINT8 ots_apps_in_use;

/* Number of simultaneous OTC Sessions using different Channel Identifiers */
extern OTP_OTS_ENTITY ots_entity[OTP_SE_OTS_MAX_INSTANCES];

/* OTS Callback registered by applications */
extern OTP_SE_NTF_CB_CTX ots_cb_ctx[OTP_SE_OTS_MAX_NTF_CB_CTX];

/* --------------------------------------------- Data types /Structures */
/* Callback registered with GA Bearer for Server Callbacks only */
GA_RESULT ots_ga_brr_server_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void          * context,
              /* IN */ OTP_SE_EVT_ID      event_type,
              /* IN */ UINT16          event_status,
              /* IN */ void          * event_data,
              /* IN */ UINT16          event_datalen
          );

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

#endif /* OTP_SERVER */

#endif /* _H_OTS_EXTERN_ */