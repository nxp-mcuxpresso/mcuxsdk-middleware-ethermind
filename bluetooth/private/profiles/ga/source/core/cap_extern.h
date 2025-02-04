/**
*  \file cap_extern.h
*
*  \brief This file has the extern of the CAP module data.
*/

/*
*  Copyright (C) 2020. Mindtree Ltd.
*  All rights reserved.
*/

#ifndef _H_CAP_EXTERN_
#define _H_CAP_EXTERN_


/* --------------------------------------------- Extern Variables */
#ifdef BAP_UCS
/** Local Stream end points */
GA_DECLARE_GLOBAL_ARRAY(CAP_STREAM_ENDPOINT, sep, GA_CONFIG_LIMITS(GA_MAX_SEP));
#endif /* BAP_UCS */

/** CAP Contexts */
GA_DECLARE_GLOBAL_ARRAY(CAP_CONTEXT, cap, GA_CONFIG_LIMITS(GA_MAX_CONTEXTS));

#ifdef BAP_UCC
/* CAP Stream Contexts */
GA_DECLARE_GLOBAL_ARRAY(CAP_STREAM_CONTEXT, cap_stream, GA_MAX_STREAM_CONTEXTS);
#endif /* BAP_UCC */

/** Registered Content Control Services */
GA_DECLARE_GLOBAL_ARRAY(UINT16, cap_ccs, GA_CONFIG_LIMITS(GA_MAX_CCS));

/* CAP SetCoordinator Sets */
GA_DECLARE_GLOBAL_ARRAY(CAP_SET_INFO, cap_set, GA_CONFIG_LIMITS(GA_MAX_SETS));

/** Number of CCS registered */
extern UINT8 cap_ccs_count;

/** CAP Profile ID with the bearer */
extern GA_BRR_PRF_HANDLE cap_id;

/** GA profile Callback - CAP Generic */
extern GA_PROFILE_CB cap_cb;

#ifdef BAP_PACS_CLIENT
/** GA Profile Callback - PAC Client */
extern GA_PROFILE_CB bac_cb;
#endif /* BAP_PACS_CLIENT */

#ifdef BAP_PACS_SERVER
/** GA Profile Callback - PAC Server */
extern GA_PROFILE_CB bas_cb;
#endif /* BAP_PACS_SERVER */

#ifdef BAP_UCC
/** GA Profile Callback - Unicast Client */
extern GA_PROFILE_CB ucc_cb;
#endif /* BAP_UCC */

#ifdef BAP_UCS
/** GA Profile Callback - Unicast Server */
extern GA_PROFILE_CB ucs_cb;
#endif /* BAP_UCS */

#ifdef BAP_BROADCAST_SOURCE
/** GA Profile Callback - Broadcast Source */
extern GA_PROFILE_CB bcsrc_cb;
#endif /* BAP_BROADCAST_SOURCE */

#ifdef BAP_BROADCAST_SINK
/** GA Profile Callback - Broadcast Sink */
extern GA_PROFILE_CB bcsnk_cb;
#endif /* BAP_BROADCAST_SINK */

/** GA Profile Callback - VCP Volume Controller */
extern GA_PROFILE_CB vcc_cb;
/** GA Profile Callback - VCP Volume Renderer */
extern GA_PROFILE_CB vcs_cb;

/** GA Profile Callback - MICP Microphone Controller */
extern GA_PROFILE_CB micc_cb;
/** GA Profile Callback - MICP Microphone Device */
extern GA_PROFILE_CB mics_cb;

/** GA Profile Callback - CSIP Set Coordinator */
extern GA_PROFILE_CB csic_cb;
/** GA Profile Callback - CSIP Set Member */
extern GA_PROFILE_CB csis_cb;

/** CAP Service ID with the bearer */
extern GA_BRR_SVC_INST cap_cas_id;

/** CAS Service ID with the bearer */
extern GA_BRR_SVC_INST cap_csis_id;

/** CAP Set Member parameters */
extern GA_SET_MEMBER cap_csmem;

/** CAP CSIP session */
extern CAP_CSIP_SESSION cap_csip;

/** CSIS Member Handle for CAP */
extern CSIP_MEMBER_HANDLE cap_csis_handle;

/* Global mutex variable for CAP */
GA_DEFINE_MUTEX_TYPE(extern, cap_mutex)

#endif /* _H_CAP_EXTERN_ */

