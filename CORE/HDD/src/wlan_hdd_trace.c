/*
 * Copyright (c) 2014 The Linux Foundation. All rights reserved.
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * This file was originally distributed by Qualcomm Atheros, Inc.
 * under proprietary terms before Copyright ownership was assigned
 * to the Linux Foundation.
 */

/*========================================================================
  \file  wlan_hdd_trace.c

  \brief WLAN Host Device Driver trace implementation

  ========================================================================*/

#include "vos_trace.h"
#include "vos_types.h"
#include "wlan_hdd_trace.h"
#include "wlan_hdd_main.h"

#if defined( WLAN_DEBUG )
static tANI_U8 *hddTraceGetEventString(tANI_U32 code)
{
	switch (code) {
		CASE_RETURN_STRING(TRACE_CODE_HDD_OPEN_REQUEST);
		CASE_RETURN_STRING(TRACE_CODE_HDD_STOP_REQUEST);
		CASE_RETURN_STRING(TRACE_CODE_HDD_TX_TIMEOUT);
		CASE_RETURN_STRING(TRACE_CODE_HDD_P2P_DEV_ADDR_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_SETSUSPENDMODE_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_SETROAMTRIGGER_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_GETROAMTRIGGER_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_SETROAMSCANPERIOD_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_GETROAMSCANPERIOD_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_SETROAMDELTA_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_GETROAMDELTA_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_GETBAND_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_GETCOUNTRYREV_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_SETROAMSCANCHANNELS_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_GETROAMSCANCHANNELS_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_HOSTAPD_OPEN_REQUEST);
		CASE_RETURN_STRING(TRACE_CODE_HDD_HOSTAPD_STOP_REQUEST);
		CASE_RETURN_STRING(TRACE_CODE_HDD_HOSTAPD_UNINIT_REQUEST);
		CASE_RETURN_STRING(TRACE_CODE_HDD_SOFTAP_TX_TIMEOUT);
		CASE_RETURN_STRING(TRACE_CODE_HDD_HOSTAPD_SET_MAC_ADDR);
		CASE_RETURN_STRING(TRACE_CODE_HDD_HOSTAPD_P2P_SET_NOA_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_HOSTAPD_P2P_SET_PS_IOCTL);
		CASE_RETURN_STRING
		    (TRACE_CODE_HDD_HOSTAPD_SET_SAP_CHANNEL_LIST_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_ADD_VIRTUAL_INTF);
		CASE_RETURN_STRING(TRACE_CODE_HDD_DEL_VIRTUAL_INTF);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CHANGE_VIRTUAL_INTF);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_START_AP);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_CHANGE_BEACON);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_STOP_AP);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_CHANGE_BSS);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_ADD_KEY);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_GET_KEY);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_SET_DEFAULT_KEY);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_CONNECT);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_DISCONNECT);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_JOIN_IBSS);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_LEAVE_IBSS);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_SET_WIPHY_PARAMS);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_SET_TXPOWER);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_GET_TXPOWER);
		CASE_RETURN_STRING(TRACE_CODE_HDD_REMAIN_ON_CHANNEL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_REMAINCHANREADYHANDLER);
		CASE_RETURN_STRING
		    (TRACE_CODE_HDD_CFG80211_CANCEL_REMAIN_ON_CHANNEL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_ACTION);
		CASE_RETURN_STRING(TRACE_CODE_HDD_MGMT_TX_CANCEL_WAIT);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_GET_STA);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_SET_POWER_MGMT);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_DEL_STA);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_ADD_STA);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_SET_PMKSA);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_UPDATE_FT_IES);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CFG80211_TDLS_MGMT);
		CASE_RETURN_STRING(TRACE_CODE_HDD_UNSUPPORTED_IOCTL);
		CASE_RETURN_STRING
		    (TRACE_CODE_HDD_SETROAMSCANCHANNELMINTIME_IOCTL);
		CASE_RETURN_STRING
		    (TRACE_CODE_HDD_GETROAMSCANCHANNELMINTIME_IOCTL);
		CASE_RETURN_STRING(TRACE_CODE_HDD_STORE_JOIN_REQ);
		CASE_RETURN_STRING(TRACE_CODE_HDD_CLEAR_JOIN_REQ);
		CASE_RETURN_STRING(TRACE_CODE_HDD_ISSUE_JOIN_REQ);

	default:
		return ("UNKNOWN");
		break;
	}
}
#endif

void hddTraceDump(void *pMac, tpvosTraceRecord pRecord, tANI_U16 recIndex)
{
	hddLog(LOGE, "%04d    %012llu  S%d    %-14s  %-30s(0x%x) ",
	       recIndex, pRecord->time, pRecord->session,
	       "HDD Event:", hddTraceGetEventString(pRecord->code),
	       pRecord->data);
}

void hddTraceInit()
{
	vosTraceRegister(VOS_MODULE_ID_HDD, (tpvosTraceCb) & hddTraceDump);
}
