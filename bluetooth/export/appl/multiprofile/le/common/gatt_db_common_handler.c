
/**
 *  \file gatt_db_common_handler.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#ifdef ATT
/* --------------------------------------------- External Global Variables */
/* Service Instances */
extern UINT16 GATT_SER_GAP_INST;
extern UINT16 GATT_SER_GATT_INST;
extern UINT16 GATT_SER_BATTERY_INST;
extern UINT16 GATT_SER_DEV_INFO_INST;

/* Characteristic Instances */
extern UINT16 GATT_CHAR_BATTERY_LVL_INST;
/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */

API_RESULT gatt_db_battery_level_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16 retval;
    UINT16 config;

    retval = API_SUCCESS;

    /**
     *  Note: Adding a generic handler for Battery Level Notification.
     *  Incase the profile does not support Battery Service. Please add a dummy
     *  GATT_CHAR_PXR_BATTERY_LVL_INST set to 0xFF in corresponding gatt_db.h to
     *  resolve the build issue.
     */
    if ((GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ == params->db_op) &&
       (GATT_CHAR_BATTERY_LVL_INST == handle->char_id))
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        retval = appl_battery_level_handler (handle, config);
    }

    return retval;
}

API_RESULT gatt_db_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    /* GAP Service Specific GATT Handlers */
    if (GATT_SER_GAP_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* GATT Service Specific GATT Handlers */
    else if (GATT_SER_GATT_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* Battery Service Specific GATT Handlers */
    else if (GATT_SER_BATTERY_INST == handle->service_id)
    {
        retval = gatt_db_battery_level_handler(handle, params);
    }
    /* Device Info Service Specific GATT Handlers */
    else if (GATT_SER_DEV_INFO_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
#ifdef PXR
    else if ((GATT_SER_PXR_LINK_LOSS_INST == handle->service_id)    ||
             (GATT_SER_PXR_IMMDT_ALRT_INST == handle->service_id)   ||
             (GATT_SER_PXR_TX_POWER_LVL_INST == handle->service_id))
    {
        retval = gatt_db_pxr_gatt_char_handler(handle, params);
    }
#endif /* PXR */
#ifdef BT_HRS
    else if (GATT_SER_HRS_HEART_RATE_INST == handle->service_id)
    {
        retval = gatt_db_hrs_gatt_char_handler(handle, params);
    }
#endif /* BT_HRS */
#ifdef VSE
    else if (GATT_SER_VSE_INST == handle->service_id)
    {
        retval = gatt_db_vse_gatt_char_handler(handle, params);
    }
#endif /* VSE */
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return retval;
}

#endif /* (defined ATT && defined PXR) */