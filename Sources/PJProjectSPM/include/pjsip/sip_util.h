/* 
 * Copyright (C) 2008-2011 Teluu Inc. (http://www.teluu.com)
 * Copyright (C) 2003-2008 Benny Prijono <benny@prijono.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */
#ifndef __PJSUA_H__
#define __PJSUA_H__

/**
 * @file pjsua.h
 * @brief PJSUA API.
 */


/* Include all PJSIP core headers. */
#include <pjsip.h>

/* Include all PJMEDIA headers. */
#include <pjmedia.h>

/* Include all PJMEDIA-CODEC headers. */
#include <pjmedia-codec.h>

/* Videodev too */
#include <pjmedia_videodev.h>

/* Include all PJSIP-UA headers */
#include <pjsip_ua.h>

/* Include all PJSIP-SIMPLE headers */
#include <pjsip_simple.h>

/* Include all PJNATH headers */
#include <pjnath.h>

/* Include all PJLIB-UTIL headers. */
#include <pjlib-util.h>

/* Include all PJLIB headers. */
#include <pjlib.h>


PJ_BEGIN_DECL


/**
 * @defgroup PJSUA_LIB PJSUA API - High Level Softphone API
 * @brief Very high level API for constructing SIP UA applications.
 * @{
 *
 * @section pjsua_api_intro A SIP User Agent API for C/C++
 *
 * PJSUA API is very high level API for constructing SIP multimedia user agent
 * applications. It wraps together the signaling and media functionalities
 * into an easy to use call API, provides account management, buddy
 * management, presence, instant messaging, along with multimedia
 * features such as conferencing, file streaming, local playback,
 * voice recording, and so on.
 *
 * @subsection pjsua_for_c_cpp C/C++ Binding
 * Application must link with <b>pjsua-lib</b> to use this API. In addition,
 * this library depends on the following libraries:
 *  - <b>pjsip-ua</b>, 
 *  - <b>pjsip-simple</b>, 
 *  - <b>pjsip-core</b>, 
 *  - <b>pjmedia</b>,
 *  - <b>pjmedia-codec</b>, 
 *  - <b>pjlib-util</b>, and
 *  - <b>pjlib</b>, 
 *
 * so application must also link with these libraries as well. For more 
 * information, please refer to 
 * <A HREF="http://www.pjsip.org/using.htm">Getting Started with PJSIP</A>
 * page.
 *
 * @section pjsua_samples
 *
 * Few samples are provided:
 *
  - @ref page_pjsip_sample_simple_pjsuaua_c \n
    Very simple SIP User Agent with registration, call, and media, using
    PJSUA-API, all in under 200 lines of code.

  - @ref page_pjsip_samples_pjsua \n
    This is the reference implementation for PJSIP and PJMEDIA.
    PJSUA is a console based application, designed to be simple enough
    to be readble, but powerful enough to demonstrate all features
    available in PJSIP and PJMEDIA.\n

 * @section root_using_pjsua_lib Using PJSUA API
 *
 * Please refer to @ref PJSUA_LIB_BASE on how to create and initialize the API.
 * And then see the Modules on the bottom of this page for more information
 * about specific subject.
 */ 



/*****************************************************************************
 * BASE API
 */

/**
 * @defgroup PJSUA_LIB_BASE PJSUA-API Basic API
 * @ingroup PJSUA_LIB
 * @brief Basic application creation/initialization, logging configuration, etc.
 * @{
 *
 * The base PJSUA API controls PJSUA creation, initialization, and startup, and
 * also provides various auxiliary functions.
 *
 * @section using_pjsua_lib Using PJSUA Library
 *
 * @subsection creating_pjsua_lib Creating PJSUA
 *
 * Before anything else, application must create PJSUA by calling 
 * #pjsua_create().
 * This, among other things, will initialize PJLIB, which is crucial before 
 * any PJLIB functions can be called, PJLIB-UTIL, and create a SIP endpoint.
 *
 * After this function is called, application can create a memory pool (with
 * #pjsua_pool_create()) and read configurations from command line or file to
 * build the settings to initialize PJSUA below.
 *
 * @subsection init_pjsua_lib Initializing PJSUA
 *
 * After PJSUA is created, application can initialize PJSUA by calling
 * #pjsua_init(). This function takes several optional configuration settings 
 * in the argument, if application wants to set them.
 *
 * @subsubsection init_pjsua_lib_c_cpp PJSUA-LIB Initialization (in C)
 * Sample code to initialize PJSUA in C code:
 \code

 #include <pjsua-lib/pjsua.h>

 #define THIS_FILE  __FILE__

 static pj_status_t app_init(void)
 {
    pjsua_config         ua_cfg;
    pjsua_logging_config log_cfg;
    pjsua_media_config   media_cfg;
    pj_status_t status;

    // Must create pjsua before anything else!
    status = pjsua_create();
    if (status != PJ_SUCCESS) {
        pjsua_perror(THIS_FILE, "Error initializing pjsua", status);
        return status;
    }

    // Initialize configs with default settings.
    pjsua_config_default(&ua_cfg);
    pjsua_logging_config_default(&log_cfg);
    pjsua_media_config_default(&media_cfg);

    // At the very least, application would want to override
    // the call callbacks in pjsua_config:
    ua_cfg.cb.on_incoming_call = ...
    ua_cfg.cb.on_call_state = ..
    ...

    // Customize other settings (or initialize them from application specific
    // configuration file):
    ...

    // Initialize pjsua
    status = pjsua_init(&ua_cfg, &log_cfg, &media_cfg);
    if (status != PJ_SUCCESS) {
          pjsua_perror(THIS_FILE, "Error initializing pjsua", status);
          return status;
    }
    .
    ...
 }
 \endcode
 *
 *


 * @subsection other_init_pjsua_lib Other Initialization
 *
 * After PJSUA is initialized with #pjsua_init(), application will normally
 * need/want to perform the following tasks:
 *
 *  - create SIP transport with #pjsua_transport_create(). Application would
 *    to call #pjsua_transport_create() for each transport types that it
 *    wants to support (for example, UDP, TCP, and TLS). Please see
 *    @ref PJSUA_LIB_TRANSPORT section for more info.
 *  - create one or more SIP accounts with #pjsua_acc_add() or
 *    #pjsua_acc_add_local(). The SIP account is used for registering with
 *    the SIP server, if any. Please see @ref PJSUA_LIB_ACC for more info.
 *  - add one or more buddies with #pjsua_buddy_add(). Please see
 *    @ref PJSUA_LIB_BUDDY section for more info.
 *  - optionally configure the sound device, codec settings, and other
 *    media settings. Please see @ref PJSUA_LIB_MEDIA for more info.
 *
 *
 * @subsection starting_pjsua_lib Starting PJSUA
 *
 * After all initializations have been done, application must call
 * #pjsua_start() to start PJSUA. This function will check that all settings
 * have been properly configured, and apply default settings when they haven't,
 * or report error status when it is unable to recover from missing settings.
 *
 * Most settings can be changed during run-time. For example, application
 * may add, modify, or delete accounts, buddies, or change media settings
 * during run-time.
 *
 * @subsubsection starting_pjsua_lib_c C Example for Starting PJSUA
 * Sample code:
 \code
 static pj_status_t app_run(void)
 {
    pj_status_t status;

    // Start pjsua
    status = pjsua_start();
    if (status != PJ_SUCCESS) {
        pjsua_destroy();
        pjsua_perror(THIS_FILE, "Error starting pjsua", status);
        return status;
    }

    // Run application loop
    while (1) {
        char choice[10];
        
        printf("Select menu: ");
        fgets(choice, sizeof(choice), stdin);
        ...
    }
 }
 \endcode

 */

/** Constant to identify invalid ID for all sorts of IDs. */
enum pjsua_invalid_id_const_
{
    PJSUA_INVALID_ID = -1
};

/** Disabled features temporarily for media reorganization */
#define DISABLED_FOR_TICKET_1185        0

/** Call identification */
typedef int pjsua_call_id;

/** Account identification */
typedef int pjsua_acc_id;

/** Buddy identification */
typedef int pjsua_buddy_id;

/** File player identification */
typedef int pjsua_player_id;

/** File recorder identification */
typedef int pjsua_recorder_id;

/** Conference port identification */
typedef int pjsua_conf_port_id;

/** Opaque declaration for server side presence subscription */
typedef struct pjsua_srv_pres pjsua_srv_pres;

/** Forward declaration for pjsua_msg_data */
typedef struct pjsua_msg_data pjsua_msg_data;

/** Forward declaration for pj_stun_resolve_result */
typedef struct pj_stun_resolve_result pj_stun_resolve_result;

/**
 * Initial memory block for PJSUA.
 */
#ifndef PJSUA_POOL_LEN
#   define PJSUA_POOL_LEN               1000
#endif

/**
 * Memory increment for PJSUA.
 */
#ifndef PJSUA_POOL_INC
#   define PJSUA_POOL_INC               1000
#endif

/**
 * Initial memory block for PJSUA account.
 */
#ifndef PJSUA_POOL_LEN_ACC
#   define PJSUA_POOL_LEN_ACC   512
#endif

/**
 * Memory increment for PJSUA account.
 */
#ifndef PJSUA_POOL_INC_ACC
#   define PJSUA_POOL_INC_ACC   256
#endif

/**
 * Maximum proxies in account.
 */
#ifndef PJSUA_ACC_MAX_PROXIES
#   define PJSUA_ACC_MAX_PROXIES    8
#endif

/**
 * Default value of SRTP mode usage. Valid values are PJMEDIA_SRTP_DISABLED, 
 * PJMEDIA_SRTP_OPTIONAL, and PJMEDIA_SRTP_MANDATORY.
 */
#ifndef PJSUA_DEFAULT_USE_SRTP
    #define PJSUA_DEFAULT_USE_SRTP  PJMEDIA_SRTP_DISABLED
#endif

/**
 * Default value of secure signaling requirement for SRTP.
 * Valid values are:
 *      0: SRTP does not require secure signaling
 *      1: SRTP requires secure transport such as TLS
 *      2: SRTP requires secure end-to-end transport (SIPS)
 */
#ifndef PJSUA_DEFAULT_SRTP_SECURE_SIGNALING
    #define PJSUA_DEFAULT_SRTP_SECURE_SIGNALING 1
#endif

/**
 * Controls whether PJSUA-LIB should add ICE media feature tag
 * parameter (the ";+sip.ice" parameter) to Contact header if ICE
 * is enabled in the config.
 *
 * Default: 1
 */
#ifndef PJSUA_ADD_ICE_TAGS
#   define PJSUA_ADD_ICE_TAGS           1
#endif

/**
 * Timeout value used to acquire mutex lock on a particular call.
 *
 * Default: 2000 ms
 */
#ifndef PJSUA_ACQUIRE_CALL_TIMEOUT
#   define PJSUA_ACQUIRE_CALL_TIMEOUT 2000
#endif

/**
 * Is video enabled.
 */
#ifndef PJSUA_HAS_VIDEO
#   define PJSUA_HAS_VIDEO              PJMEDIA_HAS_VIDEO
#endif


/**
 * Interval between two keyframe requests, in milliseconds.
 *
 * Default: 3000 ms
 */
#ifndef PJSUA_VID_REQ_KEYFRAME_INTERVAL
#   define PJSUA_VID_REQ_KEYFRAME_INTERVAL      3000
#endif


/**
 * Specify whether timer heap events will be polled by a separate worker
 * thread. If this is set/enabled, a worker thread will be dedicated to
 * poll timer heap events only, and the rest worker thread(s) will poll
 * ioqueue/network events only.
 *
 * Note that if worker thread count setting (i.e: pjsua_config.thread_cnt)
 * is set to zero, this setting will be ignored.
 *
 * Default: 0 (disabled)
 */
#ifndef PJSUA_SEPARATE_WORKER_FOR_TIMER
#   define PJSUA_SEPARATE_WORKER_FOR_TIMER      0
#endif


/**
 * Specify whether pjsua should disable automatically sending initial
 * answer 100/Trying for incoming calls. If disabled, application can
 * later send 100/Trying if it wishes using pjsua_call_answer().
 *
 * Default: 0 (automatic sending enabled)
 */
#ifndef PJSUA_DISABLE_AUTO_SEND_100
#   define PJSUA_DISABLE_AUTO_SEND_100  0
#endif


/**
 * Default options that will be passed when creating ice transport.
 * See #pjmedia_transport_ice_options.
 */
#ifndef PJSUA_ICE_TRANSPORT_OPTION
#   define PJSUA_ICE_TRANSPORT_OPTION   0
#endif

/**
 * Interval of checking for any new ICE candidate when trickle ICE is active.
 * Trickle ICE gathers local ICE candidates, such as STUN and TURN candidates,
 * in the background, while SDP offer/answer negotiation is being performed.
 * Later, when any new ICE candidate is found, the endpoint will convey
 * the candidate to the remote endpoint via SIP INFO.
 *
 * Default: 100 ms
 */
#ifndef PJSUA_TRICKLE_ICE_NEW_CAND_CHECK_INTERVAL
#   define PJSUA_TRICKLE_ICE_NEW_CAND_CHECK_INTERVAL    100
#endif


/**
 * This enumeration represents pjsua state.
 */
typedef enum pjsua_state
{
    /**
     * The library has not been initialized.
     */
    PJSUA_STATE_NULL,

    /**
     * After pjsua_create() is called but before pjsua_init() is called.
     */
    PJSUA_STATE_CREATED,

    /**
     * After pjsua_init() is called but before pjsua_start() is called.
     */
    PJSUA_STATE_INIT,

    /**
     * After pjsua_start() is called but before everything is running.
     */
    PJSUA_STATE_STARTING,

    /**
     * After pjsua_start() is called and before pjsua_destroy() is called.
     */
    PJSUA_STATE_RUNNING,

    /**
     * After pjsua_destroy() is called but before the function returns.
     */
    PJSUA_STATE_CLOSING

} pjsua_state;


/**
 * Logging configuration, which can be (optionally) specified when calling
 * #pjsua_init(). Application must call #pjsua_logging_config_default() to
 * initialize this structure with the default values.
 */
typedef struct pjsua_logging_config
{
    /**
     * Log incoming and outgoing SIP message? Yes!
     */
    pj_bool_t   msg_logging;

    /**
     * Input verbosity level. Value 5 is reasonable.
     */
    unsigned    level;

    /**
     * Verbosity level for console. Value 4 is reasonable.
     */
    unsigned    console_level;

    /**
     * Log decoration.
     */
    unsigned    decor;

    /**
     * Optional log filename.
     */
    pj_str_t    log_filename;

    /**
     * Additional flags to be given to #pj_file_open() when opening
     * the log file. By default, the flag is PJ_O_WRONLY. Application
     * may set PJ_O_APPEND here so that logs are appended to existing
     * file instead of overwriting it.
     *
     * Default is 0.
     */
    unsigned    log_file_flags;

    /**
     * Optional callback function to be called to write log to
     * application specific device. This function will be called for
     * log messages on input verbosity level.
     */
    void       (*cb)(int level, const char *data, int len);


} pjsua_logging_config;


/**
 * Use this function to initialize logging config.
 *
 * @param cfg   The logging config to be initialized.
 */
PJ_DECL(void) pjsua_logging_config_default(pjsua_logging_config *cfg);


/**
 * Use this function to duplicate logging config.
 *
 * @param pool      Pool to use.
 * @param dst       Destination config.
 * @param src       Source config.
 */
PJ_DECL(void) pjsua_logging_config_dup(pj_pool_t *pool,
                                       pjsua_logging_config *dst,
                                       const pjsua_logging_config *src);


/**
 * Structure to be passed on MWI callback.
 */
typedef struct pjsua_mwi_info
{
    pjsip_evsub     *evsub;     /**< Event subscription session, for
                                     reference.                         */
    pjsip_rx_data   *rdata;     /**< The received NOTIFY request.       */
} pjsua_mwi_info;


/**
 * Structure to be passed on registration callback.
 */
typedef struct pjsua_reg_info
{
    struct pjsip_regc_cbparam   *cbparam;   /**< Parameters returned by
                                                 registration callback. */
    pjsip_regc                  *regc;      /**< Client registration 
                                                 structure. */  
    pj_bool_t                    renew;     /**< Non-zero for registration and 
                                                 zero for unregistration. */
} pjsua_reg_info;


/**
 * Media stream info.
 */
typedef struct pjsua_stream_info
{
    /** Media type of this stream. */
    pjmedia_type type;

    /** Stream info (union). */
    union {
        /** Audio stream info */
        pjmedia_stream_info     aud;

        /** Video stream info */
        pjmedia_vid_stream_info vid;
    } info;

} pjsua_stream_info;


/**
 * Media stream statistic.
 */
typedef struct pjsua_stream_stat
{
    /** RTCP statistic. */
    pjmedia_rtcp_stat   rtcp;

    /** Jitter buffer statistic. */
    pjmedia_jb_state    jbuf;

} pjsua_stream_stat;


/**
 * Structure to be passed to on stream precreate callback.
 * See on_stream_precreate().
 */
typedef struct pjsua_on_stream_precreate_param
{
    /**
     * Stream index in the media session, read-only.
     */
    unsigned            stream_idx;

    /**
     * Parameters that the stream will be created from.
     */
    pjsua_stream_info   stream_info;
} pjsua_on_stream_precreate_param;


/**
 * Structure to be passed to on stream created callback.
 * See on_stream_created2().
 */
typedef struct pjsua_on_stream_created_param
{
    /**
     * The audio media stream, read-only.
     */
    pjmedia_stream      *stream;

    /**
     * Stream index in the audio media session, read-only.
     */
    unsigned             stream_idx;

    /**
     * Specify if PJSUA should take ownership of the port returned in
     * the port parameter below. If set to PJ_TRUE,
     * pjmedia_port_destroy() will be called on the port when it is
     * no longer needed.
     *
     * Default: PJ_FALSE
     */
    pj_bool_t            destroy_port;

    /**
     * On input, it specifies the audio media port of the stream. Application
     * may modify this pointer to point to different media port to be
     * registered to the conference bridge.
     */
    pjmedia_port        *port;

} pjsua_on_stream_created_param;


/** 
 * Enumeration of media transport state types.
 */
typedef enum pjsua_med_tp_st
{
    /** Null, this is the state before media transport is created. */
    PJSUA_MED_TP_NULL,

    /**
     * Just before media transport is created, which can finish
     * asynchronously later.
     */
    PJSUA_MED_TP_CREATING,

    /** Media transport creation is completed, but not initialized yet. */
    PJSUA_MED_TP_IDLE,

    /** Initialized (media_create() has been called). */
    PJSUA_MED_TP_INIT,

    /** Running (media_start() has been called). */
    PJSUA_MED_TP_RUNNING,

    /** Disabled (transport is initialized, but media is being disabled). */
    PJSUA_MED_TP_DISABLED

} pjsua_med_tp_st;


/**
 * Structure to be passed on media transport state callback.
 */
typedef struct pjsua_med_tp_state_info
{
    /**
     * The media index.
     */
    unsigned             med_idx;

    /**
     * The media transport state
     */
    pjsua_med_tp_st      state;

    /**
     * The last error code related to the media transport state.
     */
    pj_status_t          status;

    /**
     * Optional SIP error code.
     */
    int                  sip_err_code;

    /**
     * Optional extended info, the content is specific for each transport type.
     */
    void                *ext_info;

} pjsua_med_tp_state_info;


/**
  * Type of callback to be called when media transport state is changed.
  *
  * @param call_id      The call ID.
  * @param info         The media transport state info.
  *
  * @return             The callback must return PJ_SUCCESS at the moment.
  */
typedef pj_status_t
(*pjsua_med_tp_state_cb)(pjsua_call_id call_id,
                         const pjsua_med_tp_state_info *info);


/**
 * Typedef of callback to be registered to #pjsua_resolve_stun_servers()
 * and to be called when STUN resolution completes.
 */
typedef void (*pj_stun_resolve_cb)(const pj_stun_resolve_result *result);


/**
 * This enumeration specifies the options for custom media transport creation.
 */
typedef enum pjsua_create_media_transport_flag
{
   /**
    * This flag indicates that the media transport must also close its
    * "member" or "child" transport when pjmedia_transport_close() is
    * called. If this flag is not specified, then the media transport
    * must not call pjmedia_transport_close() of its member transport.
    */
   PJSUA_MED_TP_CLOSE_MEMBER = 1

} pjsua_create_media_transport_flag;


/**
 * Specify SRTP media transport settings.
 */
typedef struct pjsua_srtp_opt
{
    /**
     * Specify the number of crypto suite settings. If set to zero, all
     * available cryptos will be enabled. Note that available crypto names
     * can be enumerated using pjmedia_srtp_enum_crypto().
     *
     * Default is zero.
     */
    unsigned                     crypto_count;

    /**
     * Specify individual crypto suite setting and its priority order.
     *
     * Notes for DTLS-SRTP keying:
     *  - Currently only supports these cryptos: AES_CM_128_HMAC_SHA1_80,
     *    AES_CM_128_HMAC_SHA1_32, AEAD_AES_256_GCM, and AEAD_AES_128_GCM.
     *  - SRTP key is not configurable.
     */
    pjmedia_srtp_crypto          crypto[PJMEDIA_SRTP_MAX_CRYPTOS];

    /**
     * Specify the number of enabled keying methods. If set to zero, all
     * keyings will be enabled. Maximum value is PJMEDIA_SRTP_MAX_KEYINGS.
     * Note that available keying methods can be enumerated using
     * pjmedia_srtp_enum_keying().
     *
     * Default is zero (all keyings are enabled with priority order:
     * SDES, DTLS-SRTP).
     */
    unsigned                     keying_count;

    /**
     * Specify enabled keying methods and its priority order. Keying method
     * with higher priority will be given earlier chance to process the SDP,
     * for example as currently only one keying is supported in the SDP offer,
     * keying with first priority will be likely used in the SDP offer.
     */
    pjmedia_srtp_keying_method   keying[PJMEDIA_SRTP_KEYINGS_COUNT];

} pjsua_srtp_opt;


/**
 * This enumeration specifies the contact rewrite method.
 */
typedef enum pjsua_contact_rewrite_method
{
    /**
      * The Contact update will be done by sending unregistration
      * to the currently registered Contact, while simultaneously sending new
      * registration (with different Call-ID) for the updated Contact.
      */
    PJSUA_CONTACT_REWRITE_UNREGISTER = 1,

    /**
      * The Contact update will be done in a single, current
      * registration session, by removing the current binding (by setting its
      * Contact's expires parameter to zero) and adding a new Contact binding,
      * all done in a single request.
      */
    PJSUA_CONTACT_REWRITE_NO_UNREG = 2,

    /**
      * The Contact update will be done when receiving any registration final
      * response. If this flag is not specified, contact update will only be
      * done upon receiving 2xx response. This flag MUST be used with
      * PJSUA_CONTACT_REWRITE_UNREGISTER or PJSUA_CONTACT_REWRITE_NO_UNREG
      * above to specify how the Contact update should be performed when
      * receiving 2xx response.
      */
    PJSUA_CONTACT_REWRITE_ALWAYS_UPDATE = 4

} pjsua_contact_rewrite_method;


/**
 * This enumeration specifies the operation when handling IP change.
 */
typedef enum pjsua_ip_change_op {
    /**
     * Hasn't start ip change process.
     */
    PJSUA_IP_CHANGE_OP_NULL,

    /**
     * The restart listener process.
     */
    PJSUA_IP_CHANGE_OP_RESTART_LIS,

    /**
     * The shutdown transport process.
     */
    PJSUA_IP_CHANGE_OP_ACC_SHUTDOWN_TP,

    /**
     * The update contact process.
     */
    PJSUA_IP_CHANGE_OP_ACC_UPDATE_CONTACT,

    /**
     * The hanging up call process.
     */
    PJSUA_IP_CHANGE_OP_ACC_HANGUP_CALLS,

    /**
     * The re-INVITE call process.
     */
    PJSUA_IP_CHANGE_OP_ACC_REINVITE_CALLS,

    /**
     * The ip change process has completed.
     */
    PJSUA_IP_CHANGE_OP_COMPLETED

} pjsua_ip_change_op;


/**
 * This will contain the information of the callback \a on_ip_change_progress.
 */
typedef union pjsua_ip_change_op_info {
    /**
     * The information from listener restart operation.
     */
    struct {
        int transport_id;
    } lis_restart;

    /**
     * The information from shutdown transport.
     */
    struct {
        int acc_id;
    } acc_shutdown_tp;

    /**
     * The information from updating contact.
     */
    struct {
        pjsua_acc_id acc_id;
        pj_bool_t is_register;  /**< SIP Register if PJ_TRUE.       */
        int code;               /**< SIP status code received.      */
    } acc_update_contact;

    /**
     * The information from hanging up call operation.
     */
    struct {
        pjsua_acc_id acc_id;
        pjsua_call_id call_id;
    } acc_hangup_calls;

    /**
     * The information from re-Invite call operation.
     */
    struct {
        pjsua_acc_id acc_id;
        pjsua_call_id call_id;
    } acc_reinvite_calls;
} pjsua_ip_change_op_info;


/**
 * This enumeration specifies DTMF method.
 */
typedef enum pjsua_dtmf_method {
    /**
     * Send DTMF using RFC2833.
     */
    PJSUA_DTMF_METHOD_RFC2833,

    /**
     * Send DTMF using SIP INFO.
     * Notes:
     * - This method is not finalized in any standard/rfc, however it is 
     *   commonly used.
     * - Warning: in case the remote doesn't support SIP INFO, response might 
     *   not be sent and the sender will deal this as timeout and disconnect
     *   the call.
     */
    PJSUA_DTMF_METHOD_SIP_INFO

} pjsua_dtmf_method;


/**
 * Constant to specify unknown duration in \a pjsua_dtmf_info and
 * \a pjsua_dtmf_event.
 */
#define PJSUA_UNKNOWN_DTMF_DURATION     ((unsigned)-1)


/**
 * This will contain the information of the callback \a on_dtmf_digit2.
 */
typedef struct pjsua_dtmf_info {
    /**
     * The method used to send DTMF.
     */
    pjsua_dtmf_method method;

    /**
     * DTMF ASCII digit.
     */    
    unsigned digit;

    /**
     * DTMF signal duration. If the duration is unknown, this value is set to
     * PJSUA_UNKNOWN_DTMF_DURATION.
     */
    unsigned duration;

} pjsua_dtmf_info;


/**
 * This will contain the information of the callback \a on_dtmf_event.
 */
typedef struct pjsua_dtmf_event {
    /**
     * The method used to send DTMF.
     */
    pjsua_dtmf_method method;

    /**
     * The timestamp identifying the begin of the event. Timestamp units are
     * expressed in milliseconds.
     * Note that this value should only be used to compare multiple events
     * received via the same method relatively to each other, as the time-base
     * is randomized.
     */
    unsigned timestamp;

    /**
     * DTMF ASCII digit.
     */
    unsigned digit;

    /**
     * DTMF signal duration in milliseconds. Interpretation of the duration
     * depends on the flag PJMEDIA_STREAM_DTMF_IS_END.
     * If PJMEDIA_STREAM_DTMF_IS_END is set, this contains the total duration
     * of the DTMF signal or PJSUA_UNKNOWN_DTMF_DURATION if the duration is
     * unknown.
     * If PJMEDIA_STREAM_DTMF_IS_END is not set, this contains the duration
     * of the DTMF signal received up to this point in time.
     * A duration of "0" indicates an infinitely long duration.
     */
    unsigned duration;

    /**
     * Flags indicating additional information about the DTMF event.
     * If PJMEDIA_STREAM_DTMF_IS_UPDATE is set, the event was already
     * indicated earlier. The new indication contains an updated event
     * duration.
     * If PJMEDIA_STREAM_DTMF_IS_END is set, the event has ended and this
     * indication contains the final event duration. Note that end
     * indications might get lost. Hence it is not guaranteed to receive
     * an event with PJMEDIA_STREAM_DTMF_IS_END for every event.
     */
    unsigned flags;
} pjsua_dtmf_event;


/**
 * Call settings.
 */
typedef struct pjsua_call_setting
{
    /**
     * Bitmask of #pjsua_call_flag constants.
     *
     * Default: PJSUA_CALL_INCLUDE_DISABLED_MEDIA
     */
    unsigned         flag;

    /**
     * This flag controls what methods to request keyframe are allowed on
     * the call. Value is bitmask of #pjsua_vid_req_keyframe_method.
     *
     * Default: (PJSUA_VID_REQ_KEYFRAME_SIP_INFO | 
     *           PJSUA_VID_REQ_KEYFRAME_RTCP_PLI)
     */
    unsigned         req_keyframe_method;

    /**
     * Number of simultaneous active audio streams for this call. Setting
     * this to zero will disable audio in this call.
     *
     * Default: 1
     */
    unsigned         aud_cnt;

    /**
     * Number of simultaneous active video streams for this call. Setting
     * this to zero will disable video in this call.
     *
     * Default: 1 (if video feature is enabled, otherwise it is zero)
     */
    unsigned         vid_cnt;

    /**
     * Media direction. This setting will only be used if the flag
     * PJSUA_CALL_SET_MEDIA_DIR is set, and it will persist for subsequent
     * offers or answers. 
     * For example, a media that is set as PJMEDIA_DIR_ENCODING can only
     * mark the stream in the SDP as sendonly or inactive, but will not
     * become sendrecv in subsequent offers and answers.
     * Application can update the media direction in any API or callback
     * that accepts pjsua_call_setting as a parameter, such as via
     * pjsua_call_reinvite/update() or in on_call_rx_offer/reinvite()
     * callback.
     *
     * The index of the media dir will correspond to the provisional media
     * in pjsua_call_info.prov_media.
     * For offers that involve adding new medias (such as initial offer),
     * the index will correspond to all new audio media first, then video.
     * For example, for a new call with 2 audios and 1 video, media_dir[0]
     * and media_dir[1] will be for the audios, and media_dir[2] video.
     *
     * Default: PJMEDIA_DIR_ENCODING_DECODING
     */
    pjmedia_dir      media_dir[PJMEDIA_MAX_SDP_MEDIA];

} pjsua_call_setting;


/**
 * This structure describes application callback to receive various event
 * notification from PJSUA-API. All of these callbacks are OPTIONAL,
 * although definitely application would want to implement some of
 * the important callbacks (such as \a on_incoming_call).
 */
typedef struct pjsua_callback
{
    /**
     * Notify application when call state has changed.
     * Application may then query the call info to get the
     * detail call states by calling  pjsua_call_get_info() function.
     *
     * @param call_id   The call index.
     * @param e         Event which causes the call state to change.
     */
    void (*on_call_state)(pjsua_call_id call_id, pjsip_event *e);

    /**
     * Notify application on incoming call.
     *
     * @param acc_id    The account which match the incoming call.
     * @param call_id   The call id that has just been created for
     *                  the call.
     * @param rdata     The incoming INVITE request.
     */
    void (*on_incoming_call)(pjsua_acc_id acc_id, pjsua_call_id call_id,
                             pjsip_rx_data *rdata);

    /**
     * This is a general notification callback which is called whenever
     * a transaction within the call has changed state. Application can
     * implement this callback for example to monitor the state of
     * outgoing requests, or to answer unhandled incoming requests
     * (such as INFO) with a final response.
     *
     * @param call_id   Call identification.
     * @param tsx       The transaction which has changed state.
     * @param e         Transaction event that caused the state change.
     */
    void (*on_call_tsx_state)(pjsua_call_id call_id,
                              pjsip_transaction *tsx,
                              pjsip_event *e);

    /**
     * Notify application when media state in the call has changed.
     * Normal application would need to implement this callback, e.g.
     * to connect the call's media to sound device. When ICE is used,
     * this callback will also be called to report ICE negotiation
     * failure. When DTLS-SRTP is used, this callback will also be called
     * to report DTLS negotiation failure.
     *
     * @param call_id   The call index.
     */
    void (*on_call_media_state)(pjsua_call_id call_id);


    /**
     * Notify application when a call has just created a local SDP (for 
     * initial or subsequent SDP offer/answer). Application can implement
     * this callback to modify the SDP, before it is being sent and/or
     * negotiated with remote SDP, for example to apply per account/call
     * basis codecs priority or to add custom/proprietary SDP attributes.
     *
     * @param call_id   The call index.
     * @param sdp       The SDP has just been created.
     * @param pool      The pool instance, application should use this pool
     *                  to modify the SDP.
     * @param rem_sdp   The remote SDP, will be NULL if local is SDP offerer.
     */
    void (*on_call_sdp_created)(pjsua_call_id call_id,
                                pjmedia_sdp_session *sdp,
                                pj_pool_t *pool,
                                const pjmedia_sdp_session *rem_sdp);

    /**
     * Notify application when an audio media session is about to be created
     * (as opposed to on_stream_created() and on_stream_created2() which are
     * called *after* the session has been created). The application may change
     * some stream info parameter values, i.e: jb_init, jb_min_pre, jb_max_pre,
     * jb_max, use_ka, rtcp_sdes_bye_disabled, jb_discard_algo (audio),
     * codec_param->enc_fmt (video).
     *
     * @param call_id       Call identification.
     * @param param         The on stream precreate callback parameter.
     */
    void (*on_stream_precreate)(pjsua_call_id call_id,
                                pjsua_on_stream_precreate_param *param);

    /**
     * Notify application when audio media session is created and before it is
     * registered to the conference bridge. Application may return different
     * audio media port if it has added media processing port to the stream.
     * This media port then will be added to the conference bridge instead.
     *
     * Note: if implemented, on_stream_created2() callback will be called
     * instead of this one. 
     *
     * @param call_id       Call identification.
     * @param strm          Audio media stream.
     * @param stream_idx    Stream index in the audio media session.
     * @param p_port        On input, it specifies the audio media port of the
     *                      stream. Application may modify this pointer to
     *                      point to different media port to be registered
     *                      to the conference bridge.
     */
    void (*on_stream_created)(pjsua_call_id call_id,
                              pjmedia_stream *strm,
                              unsigned stream_idx,
                              pjmedia_port **p_port);

    /**
     * Notify application when audio media session is created and before it is
     * registered to the conference bridge. Application may return different
     * audio media port if it has added media processing port to the stream.
     * This media port then will be added to the conference bridge instead.
     *
     * @param call_id       Call identification.
     * @param param         The on stream created callback parameter.
     */
    void (*on_stream_created2)(pjsua_call_id call_id,
                               pjsua_on_stream_created_param *param);

    /**
     * Notify application when audio media session has been unregistered from
     * the conference bridge and about to be destroyed.
     *
     * @param call_id       Call identification.
     * @param strm          Audio media stream.
     * @param stream_idx    Stream index in the audio media session.
     */
    void (*on_stream_destroyed)(pjsua_call_id call_id,
                                pjmedia_stream *strm,
                                unsigned stream_idx);

    /**
     * Notify application upon incoming DTMF digits using RFC 2833 payload 
     * formats. This callback will not be called if app implements \a
     * on_dtmf_digit2() or \a on_dtmf_event().
     *
     * @param call_id   The call index.
     * @param digit     DTMF ASCII digit.
     */
    void (*on_dtmf_digit)(pjsua_call_id call_id, int digit);

    /**
     * Notify application upon incoming DTMF digits using the method specified 
     * in \a pjsua_dtmf_method. This callback will not be called if app
     * implements \a on_dtmf_event().
     *
     * @param call_id   The call index.
     * @param info      The DTMF info.
     */
    void (*on_dtmf_digit2)(pjsua_call_id call_id, const pjsua_dtmf_info *info);

    /**
     * Notify application upon incoming DTMF digits using the method specified 
     * in \a pjsua_dtmf_method. Includes additional information about events
     * received via RTP.
     *
     * @param call_id   The call index.
     * @param event     The DTMF event.
     */
    void (*on_dtmf_event)(pjsua_call_id call_id,
                          const pjsua_dtmf_event *event);

    /**
     * Notify application on call being transferred (i.e. REFER is received).
     * Application can decide to accept/reject transfer request
     * by setting the code (default is 202). When this callback
     * is not defined, the default behavior is to accept the
     * transfer. See also on_call_transfer_request2() callback for
     * the version with \a pjsua_call_setting in the argument list.
     *
     * @param call_id   The call index.
     * @param dst       The destination where the call will be 
     *                  transferred to.
     * @param code      Status code to be returned for the call transfer
     *                  request. On input, it contains status code 202.
     */
    void (*on_call_transfer_request)(pjsua_call_id call_id,
                                     const pj_str_t *dst,
                                     pjsip_status_code *code);

    /**
     * Notify application on call being transferred (i.e. REFER is received).
     * Application can decide to accept/reject transfer request
     * by setting the code (default is 202). When this callback
     * is not defined, the default behavior is to accept the
     * transfer.
     *
     * @