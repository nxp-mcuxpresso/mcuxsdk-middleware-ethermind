
/**
 *  \file appl_hci.c
 *
 *  Source File for HCI Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_hci_le.h"
#include "appl_smp.h"

#include "appl_service.h"

#ifdef BT_VENDOR_SPECIFIC_INIT
#include "vendor_specific_init.h"
#endif /* BT_VENDOR_SPECIFIC_INIT */

#ifdef BT_LE
#ifdef APPL_GAP_CENTRAL
/* Enable to forward Advertisind report notification to handler */
/*#define APPL_HCI_LE_NOTIFY_ADV*/

/**
 * This flag enables the application to have a list for recevied
 * Connectable/Scannable ADV Reports from peer devices.
 */
#define APPL_HCI_LE_HAVE_PEER_ADV_LIST

/**
 * This flag enables the support for host application to initiate
 * address resolution of peer device, when role is GAP Central.
 * This is disabeled by default.
 * Dependency: APPL_GAP_CENTRAL
 */
/* #define APPL_RANDOM_ADDR_SUPPORT_ENABLE */
#endif /* APPL_GAP_CENTRAL */

/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */
BT_DEVICE_ADDR g_bd_addr;
UCHAR g_appl_bond_del_flag;

#ifdef APPL_RANDOM_ADDR_SUPPORT_ENABLE
UCHAR appl_in_rpa_resolution = BT_FALSE;
UCHAR appl_peer_rpa[BT_BD_ADDR_SIZE];
void appl_peer_rpa_resolution_handler(SMP_RPA_RESOLV_INFO * rpa_info, UINT16 status);
#endif /* APPL_RANDOM_ADDR_SUPPORT_ENABLE */

/* ----------------------------------------- Static Global Variables */
static const APPL_HCI_LE_OPCODE_NAME le_opcode_name_table[] =
{
    {0x2001U, "HCI_LE_SET_EVENT_MASK_OPCODE"                            },
    {0x2002U, "HCI_LE_READ_BUFFER_SIZE_OPCODE"                          },
    {0x2003U, "HCI_LE_READ_LOCAL_SUPPORTED_FEATURES_OPCODE"             },
    {0x2005U, "HCI_LE_SET_RANDOM_ADDRESS_OPCODE"                        },
    {0x2006U, "HCI_LE_SET_ADVERTISING_PARAMETERS_OPCODE"                },
    {0x2007U, "HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER_OPCODE"         },
    {0x2008U, "HCI_LE_SET_ADVERTISING_DATA_OPCODE"                      },
    {0x2009U, "HCI_LE_SET_SCAN_RESPONSE_DATA_OPCODE"                    },
    {0x200AU, "HCI_LE_SET_ADVERTISING_ENABLE_OPCODE"                    },
    {0x200BU, "HCI_LE_SET_SCAN_PARAMETERS_OPCODE"                       },
    {0x200CU, "HCI_LE_SET_SCAN_ENABLE_OPCODE"                           },
    {0x200DU, "HCI_LE_CREATE_CONNECTION_OPCODE"                         },
    {0x200EU, "HCI_LE_CREATE_CONNECTION_CANCEL_OPCODE"                  },
    {0x200FU, "HCI_LE_READ_WHITE_LIST_SIZE_OPCODE"                      },
    {0x2010U, "HCI_LE_CLEAR_WHITE_LIST_OPCODE"                          },
    {0x2011U, "HCI_LE_ADD_DEVICE_TO_WHITE_LIST_OPCODE"                  },
    {0x2012U, "HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST_OPCODE"             },
    {0x2013U, "HCI_LE_CONNECTION_UPDATE_OPCODE"                         },
    {0x2014U, "HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION_OPCODE"           },
    {0x2015U, "HCI_LE_READ_CHANNEL_CLASSIFICATION_OPCODE"               },
    {0x2016U, "HCI_LE_READ_REMOTE_USED_FEATURES_OPCODE"                 },
    {0x2017U, "HCI_LE_ENCRYPT_OPCODE"                                   },
    {0x2018U, "HCI_LE_RAND_OPCODE"                                      },
    {0x2019U, "HCI_LE_START_ENCRYPTION_OPCODE"                          },
    {0x201AU, "HCI_LE_LONG_TERM_KEY_REQUESTED_REPLY_OPCODE"             },
    {0x201BU, "HCI_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_OPCODE"    },
    {0x201CU, "HCI_LE_READ_SUPPORTED_STATES_OPCODE"                     },
    {0x201DU, "HCI_LE_RECEIVER_TEST_COMMAND_OPCODE"                     },
    {0x201EU, "HCI_LE_TRANSMITTER_TEST_COMMAND_OPCODE"                  },
    {0x201FU, "HCI_LE_TEST_END_OPCODE"                                  },

#ifdef BT_4_1
    {0x2020U, "HCI_LE_REMOTE_CONN_PARAM_REQ_RPLY_OPCODE"                },
    {0x2021U, "HCI_LE_REMOTE_CONN_PARAM_REQ_NEG_RPLY_OPCODE"            },
#endif /* BT_4_1 */

#ifdef BT_4_2
    {0x2022U, "HCI_LE_SET_DATA_LENGTH_OPCODE"                           },
    {0x2023U, "HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LEN_OPCODE"           },
    {0x2024U, "HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LEN_OPCODE"          },
    {0x2025U, "HCI_LE_READ_LOCAL_P_256_PUBLIC_KEY_OPCODE"               },
    {0x2026U, "HCI_LE_GENERATE_DHKEY_OPCODE"                            },
    {0x2027U, "HCI_LE_ADD_DEV_TO_RESOLVING_LIST_OPCODE"                 },
    {0x2028U, "HCI_LE_REMOVE_DEV_FROM_RESOLVING_LIST_OPCODE"            },
    {0x2029U, "HCI_LE_CLR_RESOLVING_LIST_OPCODE"                        },
    {0x202AU, "HCI_LE_READ_RESOLVING_LIST_SIZE_OPCODE"                  },
    {0x202BU, "HCI_LE_READ_PEER_RESOLVABLE_ADDR_OPCODE"                 },
    {0x202CU, "HCI_LE_READ_LOCAL_RESOLVABLE_ADDR_OPCODE"                },
    {0x202DU, "HCI_LE_SET_ADDR_RESOLUTION_ENABLE_OPCODE"                },
    {0x202EU, "HCI_LE_SET_RESOLVABLE_PRIVATE_ADDR_TIMEOUT_OPCODE"       },
    {0x202FU, "HCI_LE_READ_MAXIMUM_DATA_LENGTH_OPCODE"                  },
#endif /* BT_4_2 */

#ifdef BT_5_0
    { 0x2030U, "HCI_LE_READ_PHY_OPCODE" },
    { 0x2031U, "HCI_LE_SET_DEFAULT_PHY_OPCODE" },
    { 0x2032U, "HCI_LE_ENHANCED_RECEIVER_TEST_OPCODE" },
    { 0x2033U, "HCI_LE_ENHANCED_TRANSMITTER_TEST_OPCODE" },
    { 0x2034U, "HCI_LE_SET_PHY_OPCODE" },
    { 0x2035U, "HCI_LE_SET_ADVERTISING_SET_RANDOM_ADDRESS_OPCODE" },
    { 0x2036U, "HCI_LE_SET_EXTENDED_ADV_PARAMS_OPCODE" },
    { 0x2037U, "HCI_LE_SET_EXTENDED_ADVERTISING_DATA_OPCODE" },
    { 0x2038U, "HCI_LE_SET_EXTENDED_SCAN_RESPONSE_DATA_OPCODE" },
    { 0x2039U, "HCI_LE_SET_EXTENDED_ADVERTISE_ENABLE_OPCODE" },
    { 0x203AU, "HCI_LE_READ_MAX_ADV_DATA_LENGTH_OPCODE" },
    { 0x203BU, "HCI_LE_READ_NUM_SUPPORTED_ADV_SETS_OPCODE" },
    { 0x203CU, "HCI_LE_REMOVE_ADVERTISING_SET_OPCODE" },
    { 0x203DU, "HCI_LE_CLEAR_ADVERTISING_SETS_OPCODE" },
    { 0x203EU, "HCI_LE_SET_PERIODIC_ADV_PARAMS_OPCODE" },
    { 0x203FU, "HCI_LE_SET_PERIODIC_ADVERTISING_DATA_OPCODE" },
    { 0x2040U, "HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE_OPCODE" },
    { 0x2041U, "HCI_LE_SET_EXTENDED_SCAN_PARAMETERS_OPCODE" },
    { 0x2042U, "HCI_LE_SET_EXTENDED_SCAN_ENABLE_OPCODE" },
    { 0x2043U, "HCI_LE_EXTENDED_CREATE_CONNECTION_OPCODE" },
    { 0x2044U, "HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_OPCODE" },
    { 0x2045U, "HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_CANCEL_OPCODE" },
    { 0x2046U, "HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC_OPCODE" },
    { 0x2047U, "HCI_LE_ADD_DEV_TO_PERIODIC_ADVSER_LIST_OPCODE" },
    { 0x2048U, "HCI_LE_RM_DEV_FRM_PERIODIC_ADVSER_LIST_OPCODE" },
    { 0x2049U, "HCI_LE_CLEAR_PERIODIC_ADVERTISER_LIST_OPCODE" },
    { 0x204AU, "HCI_LE_READ_PERIODIC_ADVERTISER_LIST_SIZE_OPCODE" },
    { 0x204BU, "HCI_LE_READ_TRANSMIT_POWER_OPCODE" },
    { 0x204CU, "HCI_LE_READ_RF_PATH_COMPENSATION_OPCODE" },
    { 0x204DU, "HCI_LE_WRITE_RF_PATH_COMPENSATION_OPCODE" },
    { 0x204EU, "HCI_LE_SET_PRIVACY_MODE_OPCODE" },
#endif /* BT_5_0 */
#ifdef BT_5_1
    { 0x204FU, "HCI_LE_RECEIVER_TEST_V3_OPCODE" },
    { 0x2050U, "HCI_LE_TRANSMITTER_TEST_V3_OPCODE" },
    { 0x2051U, "HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_PARAMETERS_OPCODE" },
    { 0x2052U, "HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_ENABLE_OPCODE" },
    { 0x2053U, "HCI_LE_SET_CONNECTIONLESS_IQ_SAMPLING_ENABLE_OPCODE" },
    { 0x2054U, "HCI_LE_SET_CONNECTION_CTE_RECEIVE_PARAMETERS_OPCODE" },
    { 0x2055U, "HCI_LE_SET_CONNECTION_CTE_TRANSMIT_PARAMETERS_OPCODE" },
    { 0x2056U, "HCI_LE_SET_CONNECTION_CTE_REQUEST_ENABLE_OPCODE" },
    { 0x2057U, "HCI_LE_SET_CONNECTION_CTE_RESPONSE_ENABLE_OPCODE" },
    { 0x2058U, "HCI_LE_READ_ANTENNA_INFORMATION_OPCODE" },
    { 0x2059U, "HCI_LE_SET_PERIODIC_ADVERTISING_RECEIVE_ENABLE_OPCODE" },
    { 0x205AU, "HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_OPCODE" },
    { 0x205BU, "HCI_LE_PERIODIC_ADVERTISING_SET_INFO_TRANSFER_OPCODE" },
    { 0x205CU, "HCI_LE_SET_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_OPCODE" },
    { 0x205DU, "HCI_LE_SET_DEFAULT_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_OPCODE" },
    { 0x205EU, "HCI_LE_GENERATE_DHKEY_V2_OPCODE" },
    { 0x205FU, "HCI_LE_MODIFY_SLEEP_CLOCK_ACCURACY_OPCODE" },
#endif /* BT_5_1 */
#ifdef BT_5_2
    { 0x2061U, "HCI_LE_READ_ISO_TX_SYNC_OPCODE" },
    { 0x2062U, "HCI_LE_SET_CIG_PARAMETERS_OPCODE" },
    { 0x2063U, "HCI_LE_SET_CIG_PARAMETERS_TEST_OPCODE" },
    { 0x2064U, "HCI_LE_CREATE_CIS_OPCODE" },
    { 0x2065U, "HCI_LE_REMOVE_CIG_OPCODE" },
    { 0x2066U, "HCI_LE_ACCEPT_CIS_REQUEST_OPCODE" },
    { 0x2067U, "HCI_LE_REJECT_CIS_REQUEST_OPCODE" },
    { 0x2068U, "HCI_LE_CREATE_BIG_OPCODE" },
    { 0x2069U, "HCI_LE_CREATE_BIG_TEST_OPCODE" },
    { 0x206AU, "HCI_LE_TERMINATE_BIG_OPCODE" },
    { 0x206BU, "HCI_LE_BIG_CREATE_SYNC_OPCODE" },
    { 0x206CU, "HCI_LE_BIG_TERMINATE_SYNC_OPCODE" },
    { 0x206DU, "HCI_LE_REQUEST_PEER_SCA_OPCODE" },
    { 0x206EU, "HCI_LE_SETUP_ISO_DATA_PATH_OPCODE" },
    { 0x206FU, "HCI_LE_REMOVE_ISO_DATA_PATH_OPCODE" },
    { 0x2070U, "HCI_LE_ISO_TRANSMIT_TEST_OPCODE" },
    { 0x2071U, "HCI_LE_ISO_RECEIVE_TEST_OPCODE" },
    { 0x2072U, "HCI_LE_ISO_READ_TEST_COUNTERS_OPCODE" },
    { 0x2073U, "HCI_LE_ISO_TEST_END_OPCODE" },
    { 0x2074U, "HCI_LE_SET_HOST_FEATURE_OPCODE" },
    { 0x2075U, "HCI_LE_READ_ISO_LINK_QUALITY_OPCODE" },
    { 0x2076U, "HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL_OPCODE" },
    { 0x2077U, "HCI_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL_OPCODE" },
    { 0x2078U, "HCI_LE_SET_PATH_LOSS_REPORTING_PARAMETERS_OPCODE" },
    { 0x2079U, "HCI_LE_SET_PATH_LOSS_REPORTING_ENABLE_OPCODE" },
    { 0x207AU, "HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE_OPCODE" },
#endif /* BT_5_2 */
#ifdef BT_5_3
    { 0x207C, "HCI_LE_SET_DATA_RELATED_ADDRESS_CHANGES_OPCODE" },
    { 0x207D, "HCI_LE_SET_DEFAULT_SUBRATE_OPCODE" },
    { 0x207E, "HCI_LE_SUBRATE_REQUEST_OPCODE" },
#endif /* BT_5_3 */
};

/* Global to maintain connection initiation State */
static UCHAR appl_hci_le_conn_initiated = BT_FALSE;

#ifdef APPL_HCI_LE_HAVE_PEER_ADV_LIST
/* Global to configure the Peer ADV List size */
#define APPL_BLE_PEER_ADV_MAX_REPORT_COUNT   10U

/* BLE Adv Report */
typedef struct
{
    /* BLE ADV Report Event Type */
    UCHAR          event_type;

    /* Device Address */
    BT_DEVICE_ADDR dev_addr;

    /* RSSI value */
    UCHAR          rssi;

    /* Data */
    UCHAR          adv_data[HCI_MAX_ADVERTISING_DATA_LENGTH];

    /* Data Length */
    UCHAR          adv_len;
} APPL_BLE_PEER_ADV_REPORT;
#endif /* APPL_HCI_LE_HAVE_PEER_ADV_LIST */

#ifdef APPL_HCI_LE_HAVE_PEER_ADV_LIST
static APPL_BLE_PEER_ADV_REPORT appl_hci_le_peer_adv_report[APPL_BLE_PEER_ADV_MAX_REPORT_COUNT];
static UCHAR appl_hci_le_peer_adv_report_list_full;

/* ADV List DS access macros */
#define APPL_HCI_LE_GET_PEER_ADV_DATA(i)    appl_hci_le_peer_adv_report[(i)].adv_data
#define APPL_HCI_LE_GET_PEER_ADV_DATALEN(i) appl_hci_le_peer_adv_report[(i)].adv_len
#define APPL_HCI_LE_GET_PEER_ADV_ADDR(i)    appl_hci_le_peer_adv_report[(i)].dev_addr
#define APPL_HCI_LE_GET_PEER_ADV_TYPE(i)    appl_hci_le_peer_adv_report[(i)].event_type
#define APPL_HCI_LE_GET_PEER_ADV_RSSI(i)    appl_hci_le_peer_adv_report[(i)].rssi

API_RESULT appl_hci_le_init_adv_list(void);
API_RESULT appl_hci_le_add_peer_adv_report
(
    UCHAR * event_data,
    UCHAR event_len
);
API_RESULT appl_hci_le_find_free_adv_list_inst(UCHAR * free_index);
API_RESULT appl_hci_le_is_dev_in_adv_list
(
    BT_DEVICE_ADDR * bd_addr,
    UCHAR          * dev_index
);
API_RESULT appl_hci_le_delete_dev_in_adv_list(BT_DEVICE_ADDR * bd_addr);
API_RESULT appl_hci_le_display_complete_adv_list(void);
API_RESULT appl_hci_le_display_adv_report(UCHAR index);
#endif /* APPL_HCI_LE_HAVE_PEER_ADV_LIST */

/* ----------------------------------------- Functions */

API_RESULT appl_hci_le_event_indication_callback
           (
               UCHAR  event_type,
               UCHAR  * event_data,
               UCHAR  event_datalen
           )
{
    APPL_EVENT_PARAM    fsm_param;
    BT_DEVICE_ADDR      device_addr;
    DEVICE_HANDLE       dq_handle;
    UINT32              count;
    API_RESULT          retval;
    UINT16 connection_handle, value_2;
    UCHAR  status, value_1;

#ifdef BT_5_2
    UINT32 value_3;
#endif /* BT_5_2 */

    UCHAR    sub_event_code;
    UCHAR    num_reports;
    UCHAR    length_data;
    UCHAR    peer_addr_type;
    UCHAR    clock_accuracy;
    UCHAR    role;
    UCHAR    rssi;
    UINT16   conn_latency;
    UINT16   conn_interval, conn_interval_min, conn_interval_max;
    UINT16   supervision_timeout;
#ifndef APPL_HCI_LE_HAVE_PEER_ADV_LIST
    UCHAR  * data_param;
#endif //APPL_HCI_LE_HAVE_PEER_ADV_LIST
    UCHAR  * le_feature;
    UCHAR  * peer_addr;
    UCHAR  * random_number;
    UINT16   encripted_diversifier;
    BT_DEVICE_ADDR bd_addr;
    UCHAR    link_type;
    UCHAR  * adv_report;

#ifdef BT_4_2
    UINT16   max_tx_octets;
    UINT16   max_tx_time;
    UINT16   max_rx_octets;
    UINT16   max_rx_time;
    UCHAR  * local_p256_public_key;
    UCHAR  * dh_key;
    UCHAR  * local_resolvable_pvt_addr;
    UCHAR  * peer_resolvable_pvt_addr;
    UCHAR    direct_addr_type;
    UCHAR  * direct_addr;
#endif /* BT_4_2 */

#ifdef BT_5_0
    UCHAR    advertising_handle;
    UCHAR    no_comptd_ext_adv_evnts;
    UCHAR    tx_phy;
    UCHAR    rx_phy;
    UCHAR    num_report;
    UCHAR    advertising_sid;
    UCHAR    scanner_address_type;
    UCHAR    data_length;
    UCHAR    data_status;
    UCHAR    tx_power;
    UINT16   sync_handle;
    UCHAR    advertiser_phy;
    UINT16   advertising_interval;
    UCHAR    advertiser_clk_accuy;
#endif /* BT_5_0 */

    retval = API_SUCCESS;

    if (HCI_NUMBER_OF_COMPLETED_PACKETS_EVENT != event_type)
    {
        APPL_EVENT_PARAM_INIT(&fsm_param);

#ifndef APPL_LIMIT_LOGS
        if (HCI_LE_META_EVENT != event_type)
        {
            APPL_TRC("\n");
        }
#endif /* APPL_LIMIT_LOGS */

        /* Switch on the Event Code */
        switch (event_type)
        {
        case HCI_DISCONNECTION_COMPLETE_EVENT:
            APPL_TRC("Received HCI_DISCONNECTION_COMPLETE_EVENT In LE.\n");

            /* Status */
            hci_unpack_1_byte_param(&status, event_data);
            APPL_TRC("\tLE Status: 0x%02X\n", status);
            event_data += 1U;

            /* Connection Handle */
            hci_unpack_2_byte_param(&connection_handle, event_data);
            APPL_TRC("\tLE Connection Handle: 0x%04X\n", connection_handle);
            event_data += 2U;

            /* Reason */
            hci_unpack_1_byte_param(&value_1, event_data);
            APPL_TRC("\tLE Reason: 0x%02X\n", value_1);
            event_data += 1U;

            /* Get Device Handle of peer which is now disconnected */
            /* Disconnection is accompanied with reason for disconnection as event param */
            fsm_param.data_param = &value_1;

            retval = appl_get_handle_from_cxn_handle(connection_handle, &fsm_param.handle);

            if (API_SUCCESS != retval)
            {
                APPL_TRC("Spurious HCI Disconnect on BLE HCI handler\n");
                break;
            }

#ifdef APPL_FSM_SUPPORT
            /* Post Disconnection Event */
            appl_fsm_post_event(ev_appl_transport_disconnected_ind, &fsm_param);
#endif /* APPL_FSM_SUPPORT */

            /* reset this irrespective of status */
            appl_hci_le_conn_initiated = BT_FALSE;

            /* Inform service layer of transport disconnection */
            appl_service_transport_disconnect(&fsm_param.handle, value_1);

            /* Obtain Device Queue Handle from Application Handle */
            dq_handle = APPL_GET_DEVICE_HANDLE(fsm_param.handle);

            if (BT_TRUE == g_appl_bond_del_flag)
            {
                /* Delete SMP Bonding on Disconnection */
                retval = BT_smp_mark_device_untrusted_pl(&dq_handle);

                APPL_TRC("\n Marked Device as UnTrusted with Status: 0x%04X for Device Handle 0x%02X\n", retval, dq_handle);

                /* Reset the Bond Deletion Flag */
                g_appl_bond_del_flag = BT_FALSE;
            }

            retval = device_queue_get_link_type(&link_type, &dq_handle);

            if (API_SUCCESS == retval)
            {
                /**
                 * NOTE: Ideally, at this point, we should not be getting
                 * DQ_LINK_NONE as Link type. That would mean that the device
                 * entry in the Device Queue table is removed.
                 * TODO:
                 * Check the scenario where this can occur. Most Likely,
                 * this would occur for all Non-Bonded scenarios.
                 * But, even in those scenarios, the application list needs
                 * to be cleared.
                 */
                if ((DQ_LE_LINK == link_type) || (DQ_LINK_NONE == link_type))
                {
                    retval = appl_delete_device(connection_handle);
                    APPL_TRC("\nLE appl_delete_device returned with Status: 0x%04X\n", retval);

                    appl_smp_reset();
                }
                /**
                 * Ideally, this should happen from from the appl_service.c
                 * This location is treated as the commong location which received the ACL
                 * Disconnection for LE and BREDR Link type.
                 */
                else if (DQ_BR_LINK == link_type)
                {
                    retval = appl_delete_device(connection_handle);
                    APPL_TRC("\nLE appl_delete_device for BREDR Link returned with Status: 0x%04X\n", retval);
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }
            }
            break;

#ifdef APPL_LIMIT_LOGS
#ifdef APPL_TRC
#undef APPL_TRC
#define APPL_TRC(...)
#endif /* APPL_TRC */
#endif /* APPL_LIMIT_LOGS */

        case HCI_COMMAND_COMPLETE_EVENT:
            APPL_TRC("Received HCI_COMMAND_COMPLETE_EVENT In LE.\n");

            /* Number of Command Packets */
            hci_unpack_1_byte_param(&value_1, event_data);
            APPL_TRC("\t LE Num Command Packets: %d (0x%02X)\n", value_1, value_1);
            event_data += 1U;

            /* Command Opcode */
            hci_unpack_2_byte_param(&value_2, event_data);
            APPL_TRC("\tLE Command Opcode: 0x%04X -> %s\n",
                value_2, appl_hci_le_get_command_name(value_2));
            event_data += 2U;

            /* Command Status */
            hci_unpack_1_byte_param(&status, event_data);
            APPL_TRC("\tLE Command Status: 0x%02X\n", status);
            event_data += 1U;

            /* Check for HCI_LE_SET_ADVERTISING_DATA_OPCODE */
            if (HCI_LE_SET_ADVERTISING_DATA_OPCODE == value_2)
            {
            }
            /* Check for HCI_LE_SET_ADVERTISING_PARAMETERS_OPCODE */
            else if (HCI_LE_SET_ADVERTISING_PARAMETERS_OPCODE == value_2)
            {
            }
            else if (HCI_LE_SET_ADVERTISING_ENABLE_OPCODE == value_2)
            {
                if (API_SUCCESS == status)
                {
                    APPL_TRC("ADV Enable/Disable Returned Success...\n");
                    APPL_TRC("EtherMind Server is now Ready\n");
                }
            }
            /* Check for HCI_LE_SET_SCAN_PARAMETERS_OPCODE */
            else if (HCI_LE_SET_SCAN_PARAMETERS_OPCODE == value_2)
            {
            }
            else if (HCI_LE_SET_SCAN_ENABLE_OPCODE == value_2)
            {
                if (API_SUCCESS == status)
                {
#ifdef APPL_HCI_LE_HAVE_PEER_ADV_LIST
                    /* Clear the ADV Report List */
                    (BT_IGNORE_RETURN_VALUE) appl_hci_le_init_adv_list();
#endif /* APPL_HCI_LE_HAVE_PEER_ADV_LIST */

                    APPL_TRC("Enabled Scanning ...\n");
                }
            }

#ifdef SMP_PL_ENABLE_KEY_GENERATION
            else if (HCI_LE_RAND_OPCODE == value_2)
            {
                smp_rand_complete_pl(event_data, (event_datalen - 4U));
            }
#endif /* SMP_PL_ENABLE_KEY_GENERATION */

#if (defined SMP_PL_ENABLE_KEY_GENERATION || defined AES_CMAC_UTIL)
            else if (HCI_LE_ENCRYPT_OPCODE == value_2)
            {
#ifdef SMP_PL_ENABLE_KEY_GENERATION
                smp_encryption_complete_pl(event_data, (event_datalen - 4U));
#endif /* SMP_PL_ENABLE_KEY_GENERATION */
            }
#endif /* (defined SMP_PL_ENABLE_KEY_GENERATION || defined AES_CMAC_UTIL) */

            /* Tx Power Level for Proximity */
#ifdef PXR
            else if (HCI_READ_TRANSMIT_POWER_LEVEL_OPCODE == value_2)
            {
                /* Extract Connection Handle */
                hci_unpack_2_byte_param(&connection_handle, event_data);
                event_data += 2U;

                /* Extract Tx Power Level */
                hci_unpack_1_byte_param(&value_1, event_data);

                appl_pxr_get_tx_power_level_complete
                (
                    status,
                    connection_handle,
                    value_1
                );

                /**
                 * Re-adjusting the 'event_data' to print
                 * return parameters below correctly.
                 */
                event_data -= 2U;
            }
#endif /* PXR */
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            /* Command Return Parameters */
            if (4U < event_datalen)
            {
                APPL_TRC("\tLE Return Parameters: ");
                appl_dump_bytes(event_data, event_datalen - 4U);
                APPL_TRC("\n");
            }

#ifdef BT_VENDOR_SPECIFIC_INIT
            /* app_vendor_specific_init(NULL); */
#endif /* BT_VENDOR_SPECIFIC_INIT */
            break;

#ifdef APPL_LIMIT_LOGS
#ifdef APPL_TRC
#undef APPL_TRC
#define APPL_TRC printf
#endif /* APPL_TRC */
#endif /* APPL_LIMIT_LOGS */

        case HCI_COMMAND_STATUS_EVENT:
            APPL_TRC("Received HCI_COMMAND_STATUS_EVENT In LE.\n");

            /* Status */
            hci_unpack_1_byte_param(&status, event_data);
            APPL_TRC("\tLE Command Status: 0x%02X\n", status);
            event_data += 1U;

            /* Number of Command Packets */
            hci_unpack_1_byte_param(&value_1, event_data);
            APPL_TRC("\tLE Num Command Packets: %d (0x%02X)\n", value_1, value_1);
            event_data += 1U;

            /* Command Opcode */
            hci_unpack_2_byte_param(&value_2, event_data);
            APPL_TRC("\tLE Command Opcode: 0x%04X (%s)\n",
                value_2, appl_hci_le_get_command_name(value_2));
            event_data += 2U;

            if ((HCI_LE_CREATE_CONNECTION_OPCODE == value_2) && (0x00U == status))
            {
                /* Set the conn state */
                appl_hci_le_conn_initiated = BT_TRUE;
            }

            break;

        case HCI_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT:
            APPL_TRC("Received HCI_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT In LE\n");

            /* Status */
            hci_unpack_1_byte_param(&status, event_data);
            APPL_TRC("LE Status: 0x%02X\n", status);
            event_data++;

            /* Connection Handle */
            hci_unpack_2_byte_param(&value_2, event_data);
            APPL_TRC("\tLE Connection Handle: 0x%04X\n", value_2);

            break;


#ifdef HCI_LE_PING_SUPPORT
        case HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED_EVENT:
            APPL_TRC("Received HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED_EVENT.\n");

            /* Connection Handle */
            hci_unpack_2_byte_param(&connection_handle, event_data);
            APPL_TRC("\tConnection Handle: 0x%04X\n", connection_handle);

            break;
#endif /* HCI_LE_PING_SUPPORT */

        case HCI_LE_META_EVENT:
            /* APPL_TRC("Received HCI_LE_META_EVENT.\n"); */
            hci_unpack_1_byte_param(&sub_event_code, event_data);
            event_data = event_data + 1U;
            switch (sub_event_code)
            {
            case HCI_LE_CONNECTION_COMPLETE_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_CONNECTION_COMPLETE_SUBEVENT.\n");
                hci_unpack_1_byte_param(&status, event_data + 0U);
                hci_unpack_2_byte_param(&connection_handle, event_data + 1U);
                hci_unpack_1_byte_param(&role, event_data + 3U);
                hci_unpack_1_byte_param(&peer_addr_type, event_data + 4U);
                peer_addr = 5U + event_data;
                hci_unpack_2_byte_param(&conn_interval, event_data + 11U);
                hci_unpack_2_byte_param(&conn_latency, event_data + 13U);
                hci_unpack_2_byte_param(&supervision_timeout, event_data + 15U);
                hci_unpack_1_byte_param(&clock_accuracy, event_data + 17U);

                /* Print the parameters */
                APPL_TRC("status = 0x%02X\n", status);
                APPL_TRC("connection_handle = 0x%04X\n", connection_handle);

#ifndef APPL_LIMIT_LOGS
                APPL_TRC("role = 0x%02X\n", role);
                APPL_TRC("peer_addr_type = 0x%02X\n", peer_addr_type);
                APPL_TRC("peer_addr = \n");
                appl_dump_bytes(peer_addr, 6U);
                APPL_TRC("conn_interval = 0x%04X\n", conn_interval);
                APPL_TRC("conn_latency = 0x%04X\n", conn_latency);
                APPL_TRC("supervision_timeout = 0x%04X\n", supervision_timeout);
                APPL_TRC("clock_accuracy = 0x%02X\n", clock_accuracy);
#else /* APPL_LIMIT_LOGS */
                BT_IGNORE_UNUSED_PARAM(clock_accuracy);
                BT_IGNORE_UNUSED_PARAM(role);
#endif /* APPL_LIMIT_LOGS */
                /* reset this irrespective of the status */
                appl_hci_le_conn_initiated = BT_FALSE;

                /* Disconnection is accompanied with reason for disconnection as event param */
                fsm_param.data_param = event_data;
                fsm_param.data_len = event_datalen;

                BT_COPY_BD_ADDR(device_addr.addr, peer_addr);
                BT_COPY_TYPE(device_addr.type, peer_addr_type);

                BT_COPY_BD_ADDR_AND_TYPE(&g_bd_addr, &device_addr);

                /* Reset the Bond Deletion flag on new Connection event */
                g_appl_bond_del_flag = BT_FALSE;

                retval = device_queue_search_le_remote_addr
                (
                    &dq_handle,
                    &device_addr
                );

                if (API_SUCCESS == retval)
                {
                    (BT_IGNORE_RETURN_VALUE)appl_set_cxn_handle
                    (
                        connection_handle,
                        dq_handle,
                        &fsm_param.handle
                    );
                }

#ifdef SMP_PL_ENABLE_KEY_GENERATION
                BT_smp_generate_keys_pl(0x01U);
#endif /* SMP_PL_ENABLE_KEY_GENERATION */

#ifdef RCSP
                appl_rcs_update_connection_params
                (
                    conn_interval,
                    conn_latency,
                    supervision_timeout
                );
#endif /* RCSP */

                break;

            case HCI_LE_ADVERTISING_REPORT_SUBEVENT:
                /* Store the incoming data pointer to ADV report */
                adv_report = event_data;

                /* Number of Responses */
                hci_unpack_1_byte_param(&num_reports, event_data);
                event_data += 1U;

#ifndef APPL_HCI_LE_HAVE_PEER_ADV_LIST
                APPL_TRC("Subevent : HCI_LE_ADVERTISING_REPORT_SUBEVENT.\n");
                APPL_TRC("num_reports = 0x%02X\n", num_reports);
#endif /* APPL_HCI_LE_HAVE_PEER_ADV_LIST */

                BT_mem_set(&bd_addr, 0x0, sizeof(bd_addr));

                /* For each Response, Print the Inquiry Result */
                for (count = 0U; count < num_reports; count++)
                {
                    hci_unpack_1_byte_param(&event_type, event_data);
                    event_data += 1U;

                    hci_unpack_1_byte_param(&bd_addr.type, event_data);
                    event_data += 1U;

                    BT_COPY_BD_ADDR
                    (
                        bd_addr.addr,
                        event_data
                    );

                    event_data += BT_BD_ADDR_SIZE;
                    hci_unpack_1_byte_param(&length_data, event_data);
                    event_data += 1U;
#ifndef APPL_HCI_LE_HAVE_PEER_ADV_LIST
                    /* Save the Moved ADV data Pointer to another */
                    data_param = event_data;
#endif /* APPL_HCI_LE_HAVE_PEER_ADV_LIST */
                    event_data += length_data;
                    hci_unpack_1_byte_param(&rssi, event_data);
                    event_data += 1U;

#ifndef APPL_HCI_LE_HAVE_PEER_ADV_LIST
                    /* Print the parameters */
                    APPL_TRC("event_type = 0x%02X\n", event_type);
                    APPL_TRC("address_type = 0x%02X\n", bd_addr.type);
                    APPL_TRC("address = \n");
                    appl_dump_bytes(bd_addr.addr, BT_BD_ADDR_SIZE);
                    APPL_TRC("length_data = 0x%02X\n", length_data);
                    APPL_TRC("data = \n");
                    appl_dump_bytes(data_param, length_data);
                    APPL_TRC("rssi = 0x%02X\n", rssi);
#endif /* APPL_HCI_LE_HAVE_PEER_ADV_LIST */

#ifdef APPL_HCI_LE_NOTIFY_ADV
                    /* Notify received adv report */
                    appl_notify_adv_report
                    (
                        event_type,
                        &bd_addr,
                        data_param,
                        length_data
                    );
#endif /* APPL_HCI_LE_NOTIFY_ADV */
                }

                /* Break if the event type is Non Connectable */
                if (HCI_ADV_NONCONN_IND == event_type)
                {
                    break;
                }

#ifdef APPL_HCI_LE_HAVE_PEER_ADV_LIST
                /* Add the incoming ADV device to the List */
                retval = appl_hci_le_add_peer_adv_report
                         (
                             adv_report,
                             event_datalen
                         );

                if (API_SUCCESS != retval)
                {
                    break;
                }
#endif /* APPL_HCI_LE_HAVE_PEER_ADV_LIST */

#ifdef APPL_RANDOM_ADDR_SUPPORT_ENABLE
                /* Resolve the address if Resolvable Private Address */
                if (((bd_addr.type == BT_BD_RANDOM_ADDRESS_TYPE) &&
                    ((bd_addr.addr[BT_BD_ADDR_SIZE - 1U] & 0xC0U) == 0x40U)) &&
                    (HCI_ADV_IND == event_type))
                {
                    /* Check if address resolution is not in progress */
                    if (BT_FALSE == appl_in_rpa_resolution)
                    {
                        /* Copy the address */
                        BT_COPY_BD_ADDR(appl_peer_rpa, bd_addr.addr);

                        retval = BT_smp_resolve_device
                        (
                            appl_peer_rpa,
                            appl_peer_rpa_resolution_handler
                        );

                        if (API_SUCCESS == retval)
                        {
                            /* Update resolution status */
                            appl_in_rpa_resolution = BT_TRUE;

                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
#endif /* APPL_RANDOM_ADDR_SUPPORT_ENABLE */

                /* Initiate connection */
                if (BT_FALSE == appl_hci_le_conn_initiated)
                {
                    retval = appl_service_initiate_conn
                             (
                                 BT_BD_PUBLIC_ADDRESS_TYPE,
                                 bd_addr.type,
                                 bd_addr.addr,
                                 0x00U,
                                 BT_TRUE
                             );

                    if (API_SUCCESS == retval)
                    {
                        /* Set the Connection State */
                        appl_hci_le_conn_initiated = BT_TRUE;
                    }
                }

                break;

            case HCI_LE_CONNECTION_UPDATE_COMPLETE_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_CONNECTION_UPDATE_COMPLETE_SUBEVENT.\n");
                hci_unpack_1_byte_param(&status, event_data + 0U);
                hci_unpack_2_byte_param(&connection_handle, event_data + 1U);
                hci_unpack_2_byte_param(&conn_interval, event_data + 3U);
                hci_unpack_2_byte_param(&conn_latency, event_data + 5U);
                hci_unpack_2_byte_param(&supervision_timeout, event_data + 7U);

#ifndef APPL_LIMIT_LOGS
                /* Print the parameters */
                APPL_TRC("status = 0x%02X\n", status);
                APPL_TRC("connection_handle = 0x%04X\n", connection_handle);
                APPL_TRC("conn_interval = 0x%04X\n", conn_interval);
                APPL_TRC("conn_latency = 0x%04X\n", conn_latency);
                APPL_TRC("supervision_timeout = 0x%04X\n", supervision_timeout);
#endif /* APPL_LIMIT_LOGS */

#ifdef RCSP
                appl_rcs_conn_update_complete_event
                (
                    status,
                    conn_interval,
                    conn_latency,
                    supervision_timeout
                );
#endif /* RCSP */
                break;

            case HCI_LE_READ_REMOTE_USED_FEATURES_COMPLETE_SUBEVENT:
                APPL_TRC
                ("Subevent: HCI_LE_READ_REMOTE_USED_FEATURES_COMPLETE_SUBEVENT.\n");

                hci_unpack_1_byte_param(&status, event_data + 0U);
                hci_unpack_2_byte_param(&connection_handle, event_data + 1U);
                le_feature = 3U + event_data;

                /* Print the parameters */
                APPL_TRC("status = 0x%02X\n", status);
                APPL_TRC("connection_handle = 0x%04X\n", connection_handle);
                APPL_TRC("le_feature = \n");
                appl_dump_bytes(le_feature, 8U);
                break;

            case HCI_LE_LONG_TERM_KEY_REQUESTED_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_LONG_TERM_KEY_REQUESTED_SUBEVENT.\n");
                hci_unpack_2_byte_param(&connection_handle, event_data + 0U);
                random_number = 2U + event_data;
                hci_unpack_2_byte_param(&encripted_diversifier, event_data + 10U);

#ifndef APPL_LIMIT_LOGS
                /* Print the parameters */
                APPL_TRC("connection_handle = 0x%04X\n", connection_handle);
                APPL_TRC("random_number = \n");
                appl_dump_bytes(random_number, 8U);
                APPL_TRC("encripted_diversifier = 0x%04X\n", encripted_diversifier);
#else /* APPL_LIMIT_LOGS */
                BT_IGNORE_UNUSED_PARAM(random_number);
#endif /* APPL_LIMIT_LOGS */
                break;

#ifdef HCI_LL_TOPOLOGY_CONN_UPDATE_SUPPORT
            case HCI_LE_REMOTE_CONN_PARAM_REQ_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_REMOTE_CONN_PARAM_REQ_SUBEVENT.\n");
                hci_unpack_2_byte_param(&connection_handle, event_data + 0U);
                hci_unpack_2_byte_param(&conn_interval_min, event_data + 2U);
                hci_unpack_2_byte_param(&conn_interval_max, event_data + 4U);
                hci_unpack_2_byte_param(&conn_latency, event_data + 6U);
                hci_unpack_2_byte_param(&supervision_timeout, event_data + 8U);

#ifndef APPL_LIMIT_LOGS
                /* Print the parameters */
                APPL_TRC("connection_handle = 0x%04X\n", connection_handle);
                APPL_TRC("conn_interval_min = 0x%04X\n", conn_interval_min);
                APPL_TRC("conn_interval_max = 0x%04X\n", conn_interval_max);
                APPL_TRC("conn_latency = 0x%04X\n", conn_latency);
                APPL_TRC("supervision_timeout = 0x%04X\n", supervision_timeout);
#endif /* APPL_LIMIT_LOGS */

                /**
                 * TODO: Ideally, the application might not be aware of how many concurrent
                 *       connections are present, and if the parameters requested by remote
                 *       device are suitable or not.
                 *       The application should choose to have better parameters for connection
                 *       instead of replying with the incoming parameters from peer.
                 *
                 *       CE length is set default to 0.
                 */

                (BT_IGNORE_RETURN_VALUE)BT_hci_le_remote_conn_param_req_reply
                (
                    connection_handle,
                    conn_interval_min,
                    conn_interval_max,
                    conn_latency,
                    supervision_timeout,
                    0x0000U,
                    0x0000U
                );
                break;
#endif /* HCI_LL_TOPOLOGY_CONN_UPDATE_SUPPORT */

#ifdef BT_4_2
            case HCI_LE_DATA_LENGTH_CHANGE_SUBEVENT:

                APPL_TRC("Subevent : HCI_LE_DATA_LENGTH_CHANGE_SUBEVENT.\n");

                hci_unpack_2_byte_param(&connection_handle, event_data + 0U);
                hci_unpack_2_byte_param(&max_tx_octets, event_data + 2U);
                hci_unpack_2_byte_param(&max_tx_time, event_data + 4U);
                hci_unpack_2_byte_param(&max_rx_octets, event_data + 6U);
                hci_unpack_2_byte_param(&max_rx_time, event_data + 8U);

                /* Print the parameters */
                APPL_TRC("connection_handle = 0x%04X\n", connection_handle);
                APPL_TRC("max_tx_octets = 0x%04X\n", max_tx_octets);
                APPL_TRC("max_tx_time = 0x%04X\n", max_tx_time);
                APPL_TRC("max_rx_octets = 0x%04X\n", max_rx_octets);
                APPL_TRC("max_rx_time = 0x%04X\n", max_rx_time);

                break;

            case HCI_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_SUBEVENT.\n");
                hci_unpack_1_byte_param(&status, event_data);
                APPL_TRC("Status: 0x%02X\n", status);

                event_data += 1U;
                local_p256_public_key = event_data;

#ifndef APPL_LIMIT_LOGS
                APPL_TRC("Local P-256 public key = \n");
                appl_dump_bytes(local_p256_public_key, 64U);
#else /* APPL_LIMIT_LOGS */
                BT_IGNORE_UNUSED_PARAM(local_p256_public_key);
#endif /* APPL_LIMIT_LOGS */

                break;

            case HCI_LE_GENERATE_DHKEY_COMPLETE_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_GENERATE_DHKEY_COMPLETE_SUBEVENT.\n");

                hci_unpack_1_byte_param(&status, event_data);
                APPL_TRC("Status: 0x%02X\n", status);

                event_data += 1U;
                dh_key = event_data;

#ifndef APPL_LIMIT_LOGS
                APPL_TRC("DH Key = \n");
                appl_dump_bytes(dh_key, 32U);
#else /* APPL_LIMIT_LOGS */
                BT_IGNORE_UNUSED_PARAM(dh_key);
#endif /* APPL_LIMIT_LOGS */

                break;

            case HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVENT.\n");

                hci_unpack_1_byte_param(&status, event_data);
                hci_unpack_2_byte_param(&connection_handle, event_data + 1U);
                hci_unpack_1_byte_param(&role, event_data + 3U);
                hci_unpack_1_byte_param(&peer_addr_type, event_data + 4U);
                peer_addr = event_data + 5U;
                local_resolvable_pvt_addr = event_data + 11U;
                peer_resolvable_pvt_addr = event_data + 17U;
                hci_unpack_2_byte_param(&conn_interval, event_data + 23U);
                hci_unpack_2_byte_param(&conn_latency, event_data + 25U);
                hci_unpack_2_byte_param(&supervision_timeout, event_data + 27U);
                hci_unpack_1_byte_param(&clock_accuracy, event_data + 29U);

                /**
                 * If incoming peer rpa address is valid i.e. Non zero, then use that address as peer address.
                 * Else, the peer identity address is the original address used by the Peer Device.
                 */
                if (0 != (BT_BD_ADDR_IS_NON_ZERO(peer_resolvable_pvt_addr)))
                {
                    peer_addr_type = BT_BD_RANDOM_ADDRESS_TYPE;
                    BT_COPY_BD_ADDR(peer_addr, peer_resolvable_pvt_addr);
                }
                else
                {
                    /**
                     * Do Nothing...
                     * peer_addr_type has the AD type of Peer
                     * peer_addr has the AD of Peer
                     */
                }

                APPL_TRC("status = 0x%02X\n", status);
                APPL_TRC("connection_handle = 0x%04X\n", connection_handle);
#ifndef APPL_LIMIT_LOGS
                APPL_TRC("role = 0x%02X\n", role);
                APPL_TRC("peer_addr_type = 0x%02X\n", peer_addr_type);
                APPL_TRC("peer_addr = \n");
                appl_dump_bytes(peer_addr, 6U);
                APPL_TRC("local_resolvable_pvt_addr = \n");
                appl_dump_bytes(local_resolvable_pvt_addr, 6U);
                APPL_TRC("peer_resolvable_pvt_addr = \n");
                appl_dump_bytes(peer_resolvable_pvt_addr, 6U);
                APPL_TRC("conn_interval = 0x%04X\n", conn_interval);
                APPL_TRC("conn_latency = 0x%04X\n", conn_latency);
                APPL_TRC("supervision_timeout = 0x%04X\n", supervision_timeout);
                APPL_TRC("clock_accuracy = 0x%02X\n", clock_accuracy);
#else /* APPL_LIMIT_LOGS */
                BT_IGNORE_UNUSED_PARAM(local_resolvable_pvt_addr);
#endif /* APPL_LIMIT_LOGS */
                /* reset this irrespective of status */
                appl_hci_le_conn_initiated = BT_FALSE;

                BT_COPY_BD_ADDR(device_addr.addr, peer_addr);
                BT_COPY_TYPE(device_addr.type, peer_addr_type);

                BT_COPY_BD_ADDR_AND_TYPE(&g_bd_addr, &device_addr);

                /* Reset the Bond Deletion flag on new Connection event */
                g_appl_bond_del_flag = BT_FALSE;

                retval = device_queue_search_le_remote_addr
                (
                    &dq_handle,
                    &device_addr
                );

                if (API_SUCCESS == retval)
                {
                    (BT_IGNORE_RETURN_VALUE)appl_set_cxn_handle
                    (
                        connection_handle,
                        dq_handle,
                        &fsm_param.handle
                    );
                }

#ifdef SMP_PL_ENABLE_KEY_GENERATION
                BT_smp_generate_keys_pl(0x01U);
#endif /* SMP_PL_ENABLE_KEY_GENERATION */

                break;

            case HCI_LE_DIRECT_ADVERTISING_REPORT_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_DIRECT_ADVERTISING_REPORT_SUBEVENT.\n");

                /* Number of reports */
                hci_unpack_1_byte_param(&num_reports, event_data);
                APPL_TRC("num_reports = 0x%02X\n", num_reports);
                event_data += 1U;

                /*  Print the reports */
                for (count = 0U; count < num_reports; count++)
                {
                    hci_unpack_1_byte_param(&event_type, event_data);
                    event_data += 1U;
                    hci_unpack_1_byte_param(&bd_addr.type, event_data);
                    event_data += 1U;
                    BT_COPY_BD_ADDR(bd_addr.addr, event_data);
                    event_data += 6U;
                    hci_unpack_1_byte_param(&direct_addr_type, event_data);
                    event_data += 1U;
                    direct_addr = event_data;
                    event_data += 6U;
                    hci_unpack_1_byte_param(&rssi, event_data);
                    event_data += 1U;

                    /* Print the parameters */
                    APPL_TRC("event_type = 0x%02X\n", event_type);
                    APPL_TRC("address_type = 0x%02X\n", bd_addr.type);
                    APPL_TRC("address = \n");
                    appl_dump_bytes(bd_addr.addr, 6U);
                    APPL_TRC("direct_addr_type = 0x%02X\n", direct_addr_type);
                    APPL_TRC("direct_addr = \n");
                    appl_dump_bytes(direct_addr, 6U);
                    APPL_TRC("rssi = 0x%02X\n", rssi);
                }

                break;
#endif /* BT_4_2 */

#ifdef BT_5_0
            case HCI_LE_PHY_UPDATE_COMPLETE_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_PHY_UPDATE_COMPLETE_SUBEVENT.\n");
                hci_unpack_1_byte_param(&status, event_data + 0U);
                hci_unpack_2_byte_param(&connection_handle, event_data + 1U);
                hci_unpack_1_byte_param(&tx_phy, event_data + 3U);
                hci_unpack_1_byte_param(&rx_phy, event_data + 4U);

#ifndef APPL_LIMIT_LOGS
                /* Print the parameters */
                APPL_TRC("status: 0x%02X\n", status);
                APPL_TRC("connection_handle: 0x%04X\n", connection_handle);
                APPL_TRC("Tx PHY: 0x%02X\n", tx_phy);
                APPL_TRC("Rx PHY: 0x%02X\n", rx_phy);
#else /* APPL_LIMIT_LOGS */
                BT_IGNORE_UNUSED_PARAM(tx_phy);
                BT_IGNORE_UNUSED_PARAM(rx_phy);
#endif /* APPL_LIMIT_LOGS */
                break;

            case HCI_LE_EXTENDED_ADVERTISING_REPORT_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_EXTENDED_ADVERTISING_REPORT_SUBEVENT.\n");

                hci_unpack_1_byte_param(&num_report, event_data);
                APPL_TRC("num_reports = 0x%02X\n", num_report);
                event_data += 1U;

                /* Print the reports */
                for (count = 0U; count < num_report; count++)
                {
                    APPL_TRC("Report[%d]:\n", count);

                    /* Event Type: 2 Octet */
                    hci_unpack_2_byte_param(&value_2, event_data);
                    APPL_TRC("Event type: 0x%04X\n", value_2);
                    event_data += 2U;

                    /* Address Type: 1 Octet */
                    hci_unpack_1_byte_param(&value_1, event_data);
                    APPL_TRC("Address type: 0x%04X\n", value_1);
                    event_data += 1U;

                    /* Address: 6 Octet */
                    APPL_TRC("Address:\n");
                    appl_dump_bytes(event_data, BT_BD_ADDR_SIZE);
                    event_data += BT_BD_ADDR_SIZE;

                    /* Primary PHY: 1 Octet */
                    hci_unpack_1_byte_param(&value_1, event_data);
                    APPL_TRC("Primary PHY: 0x%04X\n", value_1);
                    event_data += 1U;

                    /* Secondary PHY: 1 Octet */
                    hci_unpack_1_byte_param(&value_1, event_data);
                    APPL_TRC("Secondary PHY: 0x%04X\n", value_1);
                    event_data += 1U;

                    /* Advertising SID: 1 Octet */
                    hci_unpack_1_byte_param(&value_1, event_data);
                    APPL_TRC("Advertising SID: 0x%04X\n", value_1);
                    event_data += 1U;

                    /* Tx Power: 1 Octet */
                    hci_unpack_1_byte_param(&value_1, event_data);
                    APPL_TRC("Tx Power: 0x%04X\n", value_1);
                    event_data += 1U;

                    /* RSSI: 1 Octet */
                    hci_unpack_1_byte_param(&value_1, event_data);
                    APPL_TRC("RSSI: 0x%04X\n", value_1);
                    event_data += 1U;

                    /* Periodic Advertising Interval: 2 Octet */
                    hci_unpack_2_byte_param(&value_2, event_data);
                    APPL_TRC("Periodic Advertising Interval: 0x%04X\n", value_2);
                    event_data += 2U;

                    /* Direct Address Type: 1 Octet */
                    hci_unpack_1_byte_param(&value_1, event_data);
                    APPL_TRC("Direct Address type: 0x%04X\n", value_1);
                    event_data += 1U;

                    /* Direct Address: 6 Octet */
                    APPL_TRC("Direct Address:\n");
                    appl_dump_bytes(event_data, BT_BD_ADDR_SIZE);
                    event_data += BT_BD_ADDR_SIZE;

                    /* Data Length: 1 Octet */
                    hci_unpack_1_byte_param(&value_1, event_data);
                    APPL_TRC("Data Length: 0x%04X\n", value_1);
                    event_data += 1U;

                    /* Data: Data Length Octet */
                    APPL_TRC("Data:\n");
                    appl_dump_bytes(event_data, value_1);
                    event_data += value_1;
                }
                break;

            case HCI_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED_SUBEVENT.\n");
                hci_unpack_1_byte_param(&status, event_data + 0U);
                hci_unpack_2_byte_param(&sync_handle, event_data + 1U);
                hci_unpack_1_byte_param(&advertising_sid, event_data + 3U);
                hci_unpack_1_byte_param(&peer_addr_type, event_data + 4U);
                peer_addr = event_data + 5U;
                hci_unpack_1_byte_param(&advertiser_phy, event_data + 11U);
                hci_unpack_2_byte_param(&advertising_interval, event_data + 12U);
                hci_unpack_1_byte_param(&advertiser_clk_accuy, event_data + 14U);

                /* Print the parameters */
                APPL_TRC("status: 0x%02X\n", status);
                APPL_TRC("Sync_Handle: 0x%04X\n", sync_handle);
                APPL_TRC("Periodic_Advertising_ID: 0x%02X\n", advertising_sid);
                APPL_TRC("Advertiser_Address_Type: 0x%02X\n", peer_addr_type);
                APPL_TRC("Advertiser_Address: \n");
                appl_dump_bytes(peer_addr, BT_BD_ADDR_SIZE);
                APPL_TRC("Advertiser_PHY: 0x%02X\n", advertiser_phy);
                APPL_TRC("Periodic_Advertising_Interval: 0x%04X\n", advertising_interval);
                APPL_TRC("Advertiser_Clock_Accuracy: 0x%02X\n", advertiser_clk_accuy);
                break;

            case HCI_LE_PERIODIC_ADVERTISING_REPORT_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_PERIODIC_ADVERTISING_REPORT_SUBEVENT.\n");
                hci_unpack_2_byte_param(&sync_handle, event_data);
                event_data += 2U;
                hci_unpack_1_byte_param(&tx_power, event_data);
                event_data += 1U;
                hci_unpack_1_byte_param(&rssi, event_data + 3U);
                event_data += 1U;
                /* One unused octet */
                event_data += 1U;

                hci_unpack_1_byte_param(&data_status, event_data);
                event_data += 1U;

                hci_unpack_1_byte_param(&data_length, event_data);
                event_data += 1U;

                /* Print the parameters */
                APPL_TRC("Sync Handle: 0x%04X\n", sync_handle);
                APPL_TRC("Tx_Power: 0x%02X\n", tx_power);
                APPL_TRC("RSSI: 0x%02X\n", rssi);
                APPL_TRC("Data Status: 0x%02X\n", data_status);
                APPL_TRC("Data Length: 0x%02X\n", data_length);
                APPL_TRC("Data:\n");
                appl_dump_bytes(event_data, data_length);
                break;

            case HCI_LE_PERIODIC_ADVERTISING_SYNC_LOST_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_PERIODIC_ADVERTISING_SYNC_LOST_SUBEVENT.\n");
                hci_unpack_2_byte_param(&sync_handle, event_data);

                /* Print the parameters */
                APPL_TRC("Sync_Handle = 0x%04X\n", sync_handle);
                break;

            case HCI_LE_SCAN_TIMEOUT_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_SCAN_TIMEOUT_SUBEVENT.\n");
                break;

            case HCI_LE_ADVERTISING_SET_TERMINATED_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_ADVERTISING_SET_TERMINATED_SUBEVENT.\n");
                hci_unpack_1_byte_param(&status, event_data + 0U);
                hci_unpack_1_byte_param(&advertising_handle, event_data + 1U);
                hci_unpack_2_byte_param(&connection_handle, event_data + 2U);
                hci_unpack_1_byte_param(&no_comptd_ext_adv_evnts, event_data + 4U);

                /* Print the parameters */
                APPL_TRC("status = 0x%02X\n", status);
                APPL_TRC("Advertising Handle: 0x%02X\n", advertising_handle);
                APPL_TRC("Connection Handle: 0x%04X\n", connection_handle);
                APPL_TRC("Num_Completed_Extended_Advertising_Events: 0x%02X\n",
                    no_comptd_ext_adv_evnts);
                break;

            case HCI_LE_SCAN_REQUEST_RECEIVED_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_SCAN_REQUEST_RECEIVED_SUBEVENT.\n");
                hci_unpack_1_byte_param(&advertising_handle, event_data + 0U);
                hci_unpack_1_byte_param(&scanner_address_type, event_data + 1U);
                peer_addr = event_data + 2U;

                /* Print the parameters */
                APPL_TRC("Advertising Handle: 0x%02X\n", advertising_handle);
                APPL_TRC("Scanner Address Type: 0x%02X\n", scanner_address_type);
                APPL_TRC("Scanner Address:\n");
                appl_dump_bytes(peer_addr, BT_BD_ADDR_SIZE);
                break;

            case HCI_LE_CHANNEL_SELECTION_ALGORITHM_SUBEVENT:
                APPL_TRC("Subevent : HCI_LE_CHANNEL_SELECTION_ALGORITHM_SUBEVENT.\n");
                hci_unpack_2_byte_param(&connection_handle, event_data + 0U);
                hci_unpack_1_byte_param(&value_1, event_data + 2U);

#ifndef APPL_LIMIT_LOGS
                /* Print the parameters */
                APPL_TRC("Connection Handle: 0x%04X\n", connection_handle);
                APPL_TRC("Channel Selection Algorithm: 0x%02X\n", value_1);
#endif /* APPL_LIMIT_LOGS */
                break;

#endif /* BT_5_0 */

#ifdef BT_5_2
        case HCI_LE_PATH_LOSS_THRESHOLD_SUBEVENT:
            APPL_TRC("Subevent : HCI_LE_PATH_LOSS_THRESHOLD_EVENT.\n");
            hci_unpack_2_byte_param(&connection_handle, event_data + 0);
            APPL_TRC("Connection Handle: 0x%04X\n", connection_handle);
            hci_unpack_1_byte_param(&value_1, event_data + 2);
            APPL_TRC("Current Path Loss: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 3);
            APPL_TRC("Zone Enterted: 0x%02X\n", value_1);
            break;

        /*case HCI_LE_TX_POWER_REPORTING_SUBEVENT:
            APPL_TRC("Subevent : HCI_LE_TRANSMIT_POWER_REPORTING_EVENT.\n");
            hci_unpack_1_byte_param(&status, event_data + 0);
            APPL_TRC("Status: 0x%02X\n", status);
            hci_unpack_2_byte_param(&connection_handle, event_data + 1);
            APPL_TRC("Connection Handle: 0x%04X\n", connection_handle);
            hci_unpack_1_byte_param(&value_1, event_data + 3);
            APPL_TRC("Reason: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 4);
            APPL_TRC("PHY: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 5);
            APPL_TRC("Transmit Power Level: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 6);
            APPL_TRC("Transmit Power Level Flag: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 7);
            APPL_TRC("Delta: 0x%02X\n", value_1);
            break;*/
		case HCI_LE_TRANSMIT_POWER_REPORTING_SUBEVENT:
            APPL_TRC("Subevent : HCI_LE_TRANSMIT_POWER_REPORTING_EVENT.\n");
            hci_unpack_1_byte_param(&status, event_data + 0);
            APPL_TRC("Status: 0x%02X\n", status);
            hci_unpack_2_byte_param(&connection_handle, event_data + 1);
            APPL_TRC("Connection Handle: 0x%04X\n", connection_handle);
            hci_unpack_1_byte_param(&value_1, event_data + 3);
            APPL_TRC("Reason: 0x%02X\n", value_1);

#ifdef APPL_HCI_LE_LEPC_DBGLOG
            if(value_1 == 0x00)
            {
				APPL_TRC("Local transmit power changed\n");
            }
            else if(value_1 == 0x01)
            {
				APPL_TRC("Remote transmit power changed\n");
            }
            else if(value_1 == 0x02)
            {
				APPL_TRC(" HCI_LE_Read_Remote_Transmit_Power_Level command completed \n");
            }
            else
            {
				APPL_TRC("Reserved for future use\n");
            }
#endif /*APPL_HCI_LE_LEPC_DBGLOG*/

            hci_unpack_1_byte_param(&value_1, event_data + 4);
            APPL_TRC("PHY: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 5);
            APPL_TRC("Transmit Power Level: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 6);
            APPL_TRC("Transmit Power Level Flag: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 7);
            APPL_TRC("Delta: 0x%02X\n", value_1);
            break;

        case HCI_LE_CIS_ESTABLISHED_SUBEVENT:
            APPL_TRC("Subevent : HCI_LE_CIS_ESTABLISHED_SUBEVENT.\n");
            hci_unpack_1_byte_param(&status, event_data + 0);
            APPL_TRC("Status: 0x%02X\n", status);
            hci_unpack_2_byte_param(&connection_handle, event_data + 1);
            APPL_TRC("Connection Handle: 0x%04X\n", connection_handle);
            hci_unpack_3_byte_param(&value_3, event_data + 3);
            APPL_TRC("CIG_Sync_Delay: 0x%08X\n", value_3);
            hci_unpack_3_byte_param(&value_3, event_data + 6);
            APPL_TRC("CIS_Sync_Delay: 0x%08X\n", value_3);
            hci_unpack_3_byte_param(&value_3, event_data + 9);
            APPL_TRC("Transport_Latency_M_To_S: 0x%08X\n", value_3);
            hci_unpack_3_byte_param(&value_3, event_data + 12);
            APPL_TRC("Transport_Latency_S_To_M: 0x%08X\n", value_3);
            hci_unpack_1_byte_param(&value_1, event_data + 15);
            APPL_TRC("PHY_M_To_S: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 16);
            APPL_TRC("PHY_S_To_M: 0x%02X\n", value_1);
            break;

        case HCI_LE_CIS_REQUEST_SUBEVENT:
            APPL_TRC("Subevent : HCI_LE_CIS_REQUEST_SUBEVENT.\n");
            hci_unpack_2_byte_param(&connection_handle, event_data + 0);
            APPL_TRC("ACL Connection Handle: 0x%04X\n", connection_handle);
            hci_unpack_2_byte_param(&connection_handle, event_data + 2);
            APPL_TRC("CIS Connection Handle: 0x%04X\n", connection_handle);
            hci_unpack_1_byte_param(&value_1, event_data + 4);
            APPL_TRC("CIG_ID: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 5);
            APPL_TRC("CIS_ID: 0x%02X\n", value_1);
            break;

        case HCI_LE_CREATE_BIG_COMPLETE_SUBEVENT:
            APPL_TRC("Subevent : HCI_LE_CREATE_BIG_COMPLETE_SUBEVENT.\n");
            hci_unpack_1_byte_param(&status, event_data + 0);
            APPL_TRC("Status: 0x%02X\n", status);
            hci_unpack_1_byte_param(&value_1, event_data + 1);
            APPL_TRC("BIG_Handle: 0x%02X\n", value_1);
            hci_unpack_3_byte_param(&value_3, event_data + 2);
            APPL_TRC("BIG_Sync_Delay: 0x%08X\n", value_3);
            hci_unpack_3_byte_param(&value_3, event_data + 5);
            APPL_TRC("Transport_Latency_BIG: 0x%08X\n", value_3);
            hci_unpack_1_byte_param(&value_1, event_data + 8);
            APPL_TRC("PHY: 0x%02X\n", value_1);
            hci_unpack_1_byte_param (&value_1, event_data + 9);
            APPL_TRC ("NSE: 0x%02X\n", value_1);
            hci_unpack_1_byte_param (&value_1, event_data + 10);
            APPL_TRC ("BN: 0x%02X\n", value_1);
            hci_unpack_1_byte_param (&value_1, event_data + 11);
            APPL_TRC ("PTO: 0x%02X\n", value_1);
            hci_unpack_1_byte_param (&value_1, event_data + 12);
            APPL_TRC ("IRC: 0x%02X\n", value_1);
            hci_unpack_2_byte_param (&value_2, event_data + 13);
            APPL_TRC ("Max_PDU: 0x%04X\n", value_2);
            hci_unpack_2_byte_param (&value_2, event_data + 15);
            APPL_TRC ("ISO_Inerval: 0x%04X\n", value_2);
            hci_unpack_1_byte_param(&value_1, event_data + 17);
            APPL_TRC("Num_BIS: 0x%02X\n", value_1);
            APPL_TRC("Connection Handles of BISes:");
            appl_dump_bytes(event_data + 18, (UINT16)(value_1 * 2));
            break;

        case HCI_LE_TERMINATE_BIG_COMPLETE_SUBEVENT:
            APPL_TRC("Subevent : HCI_LE_TERMINATE_BIG_COMPLETE_SUBEVENT.\n");
            hci_unpack_1_byte_param(&value_1, event_data + 0);
            APPL_TRC("BIG_Handle: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 1);
            APPL_TRC("Reason: 0x%02X\n", value_1);
            break;

        case HCI_LE_BIG_SYNC_ESTABLISHED_SUBEVENT:
            APPL_TRC("Subevent : HCI_LE_BIG_SYNC_ESTABLISHED_SUBEVENT.\n");
            hci_unpack_1_byte_param(&value_1, event_data + 0);
            APPL_TRC("Status: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 1);
            APPL_TRC("BIG_Handle: 0x%02X\n", value_1);
            hci_unpack_3_byte_param(&value_3, event_data + 2);
            APPL_TRC("Transport_Latency_BIG: 0x%08X\n", value_3);
            hci_unpack_1_byte_param(&value_1, event_data + 5);
            APPL_TRC("Num_BIS: 0x%02X\n", value_1);

            APPL_TRC("Connection Handles of BISes in the BIG\n");
            appl_dump_bytes(event_data + 6, (UINT16)(value_1 * 2));
            break;

        case HCI_LE_BIG_SYNC_LOST_SUBEVENT:
            APPL_TRC("Subevent : HCI_LE_BIG_SYNC_LOST_SUBEVENT.\n");
            hci_unpack_1_byte_param(&value_1, event_data + 0);
            APPL_TRC("BIG_Handle: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 1);
            APPL_TRC("Reason: 0x%02X\n", value_1);
            break;

        case HCI_LE_REQUEST_PEER_SCA_COMPLETE_SUBEVENT:
            APPL_TRC("Subevent : HCI_LE_REQUEST_PEER_SCA_COMPLETE_SUBEVENT.\n");
            hci_unpack_1_byte_param(&value_1, event_data + 0);
            APPL_TRC("Status: 0x%02X\n", value_1);
            hci_unpack_2_byte_param(&connection_handle, event_data + 1);
            APPL_TRC("Connection Handle: 0x%04X\n", connection_handle);
            hci_unpack_1_byte_param(&value_1, event_data + 3);
            APPL_TRC("SCA: 0x%02X\n", value_1);
            break;

        case HCI_LE_BIGINFO_ADVERTISING_REPORT_SUBEVENT:
            APPL_TRC("Subevent : HCI_LE_BIGINFO_ADVERTISING_REPORT_SUBEVENT.\n");
            hci_unpack_2_byte_param(&value_2, event_data + 0);
            APPL_TRC("Sync Handle: 0x%04X\n", value_2);
            hci_unpack_1_byte_param(&value_1, event_data + 2);
            APPL_TRC("Num BIS: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 3);
            APPL_TRC("NSE: 0x%02X\n", value_1);
            hci_unpack_2_byte_param(&value_2, event_data + 4);
            APPL_TRC("ISO Interval: 0x%04X\n", value_2);
            hci_unpack_1_byte_param(&value_1, event_data + 6);
            APPL_TRC("BN: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 7);
            APPL_TRC("PTO: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 8);
            APPL_TRC("IRC: 0x%02X\n", value_1);
            hci_unpack_2_byte_param(&value_2, event_data + 9);
            APPL_TRC("Max PDU: 0x%04X\n", value_2);
            hci_unpack_3_byte_param(&value_3, event_data + 11);
            APPL_TRC("SDU Interval: 0x%06X\n", value_3);
            hci_unpack_2_byte_param(&value_2, event_data + 14);
            APPL_TRC("Max SDU: 0x%04X\n", value_2);
            hci_unpack_1_byte_param(&value_1, event_data + 16);
            APPL_TRC("PHY: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 17);
            APPL_TRC("Framing: 0x%02X\n", value_1);
            hci_unpack_1_byte_param(&value_1, event_data + 18);
            APPL_TRC("Encryption: 0x%02X\n", value_1);
            break;
#endif /* BT_5_2 */

            default:
                APPL_TRC("Unknown LE SubEvent Code 0x%02X Received.\n",
                    sub_event_code);
                break;
            }
            break;

        default:
            APPL_TRC("LE: Unknown Event Code 0x%02X Received.\n", event_type);
            break;
        }
    }

    return retval;
}


#ifdef APPL_RANDOM_ADDR_SUPPORT_ENABLE
void appl_peer_rpa_resolution_handler (SMP_RPA_RESOLV_INFO * rpa_info, UINT16 status)
{
    BT_DEVICE_ADDR l_bd_addr;
    API_RESULT     retval;

    APPL_TRC("Resolution of RPA " BT_DEVICE_ADDR_FRMT_SPECIFIER " returned Status 0x%04X\n",
    BT_DEVICE_ADDR_PRINT_STR(&rpa_info->n_addr), status);

    /* Is resolution successful? */
    if (API_SUCCESS == status)
    {
        /* Yes, update the device information */
        BT_smp_update_peer_address(&rpa_info->bd_handle, &rpa_info->n_addr);

        /* Ger Local Handle */
        device_queue_get_local_addr(&rpa_info->bd_handle, &l_bd_addr);

        if (BT_FALSE == appl_hci_le_conn_initiated)
        {
            /* Create Connection Here */
            retval = appl_service_initiate_conn
                     (
                         l_bd_addr.type,
                         rpa_info->n_addr.type,
                         rpa_info->n_addr.addr,
                         0x00U,
                         BT_FALSE
                     );

            if (API_SUCCESS == retval)
            {
                /* Set the Connection State */
                appl_hci_le_conn_initiated = BT_TRUE;
            }
        }
    }

    /* Reset the IN Resolution state */
    appl_in_rpa_resolution = BT_FALSE;
}
#endif /* APPL_RANDOM_ADDR_SUPPORT_ENABLE */

char *appl_hci_le_get_command_name (UINT16 opcode)
{
    int i, size;
    char *command_name;

    command_name = "UNKNOWN";
    size = sizeof(le_opcode_name_table)/sizeof(APPL_HCI_LE_OPCODE_NAME);

    for (i = 0U; i < size; i++)
    {
        if (le_opcode_name_table[i].opcode == opcode)
        {
            command_name = le_opcode_name_table[i].command_name;
            break;
        }
    }

    return command_name;
}

#ifdef APPL_HCI_LE_HAVE_PEER_ADV_LIST
API_RESULT appl_hci_le_init_adv_list(void)
{
    /* Print the List before clearing it */
    (BT_IGNORE_RETURN_VALUE) appl_hci_le_display_complete_adv_list();
    APPL_TRC("\n[**NOTE**]: Dumping the ADV Report List before List Reset!\n");

    /* Initialize the fields with default values */
    BT_mem_set
    (
        appl_hci_le_peer_adv_report,
        0x0,
        sizeof(appl_hci_le_peer_adv_report)
    );

    appl_hci_le_peer_adv_report_list_full = BT_FALSE;

    return API_SUCCESS;
}

API_RESULT appl_hci_le_add_peer_adv_report
           (
               UCHAR * event_data,
               UCHAR event_len
           )
{
    UINT8          index = 0U;
    UINT32         count = 0U;
    UCHAR          num_reports;
    UCHAR          evt_type;
    UCHAR          length_data = 0U;
    UCHAR          rssi = 0x00U;
    UCHAR          * data_param;
    BT_DEVICE_ADDR bd_addr;
    API_RESULT     retval;

    retval = API_FAILURE;

    if (0U == event_len)
    {
        APPL_TRC("Adv Packet Length is 0\n");
        /* return retval; */
    }
    else
    {
        /* Number of Responses */
        hci_unpack_1_byte_param(&num_reports, event_data);
        event_data += 1U;

        for (count = 0U; count < num_reports; count++)
        {
            /* Derive the Event Type */
            hci_unpack_1_byte_param(&evt_type, event_data);
            event_data += 1U;

            /* Derive the peer BD ADDR */
            hci_unpack_1_byte_param(&bd_addr.type, event_data);
            event_data += 1U;

            BT_COPY_BD_ADDR
            (
                bd_addr.addr,
                event_data
            );
            event_data += BT_BD_ADDR_SIZE;

            /**
             * Validate is this device already exists in the list.
             *  If device is present in the list already, fetch the index at which
             *  the Adv packet is saved in the list.
             */
            if (API_SUCCESS != appl_hci_le_is_dev_in_adv_list
                               (
                                   &bd_addr,
                                   &index
                               ))
            {
                /**
                 * Device is not present in the list,
                 * add the device to the list.
                 */
                if (API_SUCCESS == appl_hci_le_find_free_adv_list_inst(&index))
                {
                    /* Save the Event type */
                    APPL_HCI_LE_GET_PEER_ADV_TYPE(index) = evt_type;

                    /* Save the Device address and Type */
                    BT_COPY_BD_ADDR_AND_TYPE
                    (
                        &APPL_HCI_LE_GET_PEER_ADV_ADDR(index),
                        &bd_addr
                    );

                    /* Derive the Length */
                    hci_unpack_1_byte_param(&length_data, event_data);
                    event_data += 1U;

                    /* Derive the Data parameter */
                    data_param = event_data;
                    event_data += length_data;

                    /* Store the ADV Data */
                    APPL_HCI_LE_GET_PEER_ADV_DATALEN(index) = length_data;
                    BT_mem_copy
                    (
                        APPL_HCI_LE_GET_PEER_ADV_DATA(index),
                        data_param,
                        APPL_HCI_LE_GET_PEER_ADV_DATALEN(index)
                    );

                    /* Save the RSSI */
                    hci_unpack_1_byte_param(&rssi, event_data);
                    event_data += 1U;
                    APPL_HCI_LE_GET_PEER_ADV_RSSI(index) = rssi;

                    /* New device is added to the list, display the same */
                    retval = appl_hci_le_display_adv_report((UCHAR)index);
                }
            }
            else
            {
                /* Device already present in list */
                retval = API_SUCCESS;

                /* Break from loop */
                break;
            }
        }
    }

    return retval;
}

API_RESULT appl_hci_le_find_free_adv_list_inst
           (
               UCHAR * free_index
           )
{
    UINT32         index = 0U;
    API_RESULT     retval;
    BT_DEVICE_ADDR * t_addr;

    retval = API_FAILURE;

    if (BT_TRUE != appl_hci_le_peer_adv_report_list_full)
    {
        /* return retval; */

        for (index = 0U; index < APPL_BLE_PEER_ADV_MAX_REPORT_COUNT; index++)
        {
            /* Fetch the Address */
            t_addr = &APPL_HCI_LE_GET_PEER_ADV_ADDR(index);

            /* Check if the BD Address is Non Zero */
            if (BT_TRUE != BT_BD_ADDR_IS_NON_ZERO(t_addr->addr))
            {
                retval = API_SUCCESS;

                break;
            }
        }

        /* Store the index */
        *(free_index) = (UCHAR)index;

        if (API_SUCCESS != retval)
        {
            if (APPL_BLE_PEER_ADV_MAX_REPORT_COUNT == *(free_index))
            {
                /* Device list is fully occupied, needs a reset of the list */
                APPL_TRC("ADV Report Device list is full!\n");
                APPL_TRC("Consider Disabling Scanning!\n");

                appl_hci_le_peer_adv_report_list_full = BT_TRUE;
            }
        }
    }

    return retval;
}

API_RESULT appl_hci_le_is_dev_in_adv_list
           (
               /* IN */  BT_DEVICE_ADDR * bd_addr,
               /* OUT */ UCHAR          * dev_index
           )
{
    UINT32     index = 0U;
    API_RESULT retval;

    /* NULL Check? */

    retval = API_FAILURE;

    /* Validate is device is already present in the Adv list */
    for (index = 0U; index < APPL_BLE_PEER_ADV_MAX_REPORT_COUNT; index++)
    {
        if(BT_TRUE ==
           BT_COMPARE_BD_ADDR_AND_TYPE
           (
                bd_addr,
                &APPL_HCI_LE_GET_PEER_ADV_ADDR(index)
           ))
        {
            retval = API_SUCCESS;
            break;
        }
    }

    /**
     * If Device was found, the Index value would be valid.
     * Else the the index will be APPL_BLE_PEER_ADV_MAX_REPORT_COUNT.
     */
    *(dev_index) = (UCHAR)index;

    return retval;
}

API_RESULT appl_hci_le_delete_dev_in_adv_list(BT_DEVICE_ADDR * bd_addr)
{
    UCHAR index = 0U;
    API_RESULT retval;

    /* Init */
    retval = API_FAILURE;

    for (index = 0U; index < APPL_BLE_PEER_ADV_MAX_REPORT_COUNT; index++)
    {
        /** Compare the device address and fetch the index to delete the entry
         *  in the list.
         */
        if (BT_TRUE ==
            BT_COMPARE_BD_ADDR_AND_TYPE
            (
                bd_addr,
                &APPL_HCI_LE_GET_PEER_ADV_ADDR(index)
            ))
        {
            /**
             * Reset is to be done by filling the fields with default(non used)
             * values.
             */
            BT_mem_set
            (
                &appl_hci_le_peer_adv_report[index],
                0x0,
                sizeof(APPL_BLE_PEER_ADV_REPORT)
            );

            /**
             * Reset List Full status if even one device is successfully
             * deleted.
             */
            appl_hci_le_peer_adv_report_list_full = BT_FALSE;

            retval = API_SUCCESS;
            break; /* return API_SUCCESS; */
        }
    }

    if (API_FAILURE == retval)
    {
        /* Device entry not found in the list */
        APPL_TRC("Device not present in Adv List\n");
    }

    return retval;
}

API_RESULT appl_hci_le_display_complete_adv_list(void)
{
    UCHAR          index;
    BT_DEVICE_ADDR * t_addr;
    API_RESULT     retval;

    retval = API_FAILURE;

    for (index = 0U; index < APPL_BLE_PEER_ADV_MAX_REPORT_COUNT; index++)
    {
        t_addr = &APPL_HCI_LE_GET_PEER_ADV_ADDR(index);

        /* Check if the BD Address is Non Zero */
        if (BT_TRUE == BT_BD_ADDR_IS_NON_ZERO(t_addr->addr))
        {
            retval = appl_hci_le_display_adv_report(index);
        }
    }

    return retval;
}

API_RESULT appl_hci_le_display_adv_report(UCHAR index)
{
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /* Validate is it is a valid index */
    if (index >= APPL_BLE_PEER_ADV_MAX_REPORT_COUNT)
    {
        APPL_TRC("\nWrong Device Index, doesnt exist in list\n");

        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
#ifndef APPL_LIMIT_LOGS
        APPL_TRC("\n%d. BLE ADV Report\n", (index + 1U));
        APPL_TRC("===================\n");
        /* Print the parameters */
        APPL_TRC("Event Type    : 0x%02X\n",
        APPL_HCI_LE_GET_PEER_ADV_TYPE(index));
#else /* APPL_LIMIT_LOGS */
        APPL_TRC("\n%d. BLE ADV Report[Ev Type 0x%02X]\n", (index + 1U), APPL_HCI_LE_GET_PEER_ADV_TYPE(index));
#endif /* APPL_LIMIT_LOGS */
        APPL_TRC("Device Address: " BT_DEVICE_ADDR_SPACED_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_PRINT_STR(&APPL_HCI_LE_GET_PEER_ADV_ADDR(index)));

#ifndef APPL_LIMIT_LOGS
        APPL_TRC("RSSI          : 0x%02X\n", APPL_HCI_LE_GET_PEER_ADV_RSSI(index));
        APPL_TRC("ADV Len       : 0x%02X\n", APPL_HCI_LE_GET_PEER_ADV_DATALEN(index));
        APPL_TRC("ADV Data      : \n");
        appl_dump_bytes
        (
            APPL_HCI_LE_GET_PEER_ADV_DATA(index),
            APPL_HCI_LE_GET_PEER_ADV_DATALEN(index)
        );
#endif /* APPL_LIMIT_LOGS */
    }

    return retval;
}
#endif /* APPL_HCI_LE_HAVE_PEER_ADV_LIST */

#endif /* BT_LE */
