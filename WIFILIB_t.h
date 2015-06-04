/**
Atomix project, WIFILIB_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
 * Copyright (c) 2010 Stanford University
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * ========================================================================== */


/** @file orlib_types_wifi.h 
 * @brief This header file defines types specific to WiFi processing, 
 * and is used with WiFi-specific blocks in the library.
 * 
 * 	@author Jeffrey Mehlman
 * 	@author Manu Bansal
 *
 *	
 *
 * 	$Id$
 */
 
#ifndef ORLIB_TYPES_WIFI_H_
#define ORLIB_TYPES_WIFI_H_

/** Enumeration of all the WiFi modulation types */
typedef enum WifiGeeModulationTypes
{
	kWifiGeeBpsk  = 0,
	kWifiGeeQpsk  = 1,
	kWifiGee16qam = 2,
	kWifiGee64qam  = 3
} WifiGeeModulationTypes;

/** Enumeration of all the Wifi Data Rates */
typedef enum WifiGeeDataRates
{
	kWifiGee6mbps = 0,
	kWifiGee9mbps = 1,
	kWifiGee12mbps = 2,
	kWifiGee18mbps = 3,
	kWifiGee24mbps = 4,
	kWifiGee36mbps = 5,
	kWifiGee48mbps = 6,
	kWifiGee54mbps = 7,
	kWifiGeeRateInvalid = 8
} WifiGeeDataRates;

static inline WifiGeeDataRates getWifiGeeDataRate(Uint32 plcp_data_rate_4bits) {
	WifiGeeDataRates data_rate;
	switch(plcp_data_rate_4bits) {
	case 0xD:
		data_rate = kWifiGee6mbps;
		break;
	case 0xF:
		data_rate = kWifiGee9mbps;
		break;
	case 0x5:
		data_rate = kWifiGee12mbps;
		break;
	case 0x7:
		data_rate = kWifiGee18mbps;
		break;
	case 0x9:
		data_rate = kWifiGee24mbps;
		break;
	case 0xB:
		data_rate = kWifiGee36mbps;
		break;
	case 0x1:
		data_rate = kWifiGee48mbps;
		break;
	case 0x3:
		data_rate = kWifiGee54mbps;
		break;
	default:
		//				printf("Rate error: Not a valid rate.\n");
		data_rate = kWifiGeeRateInvalid;
	}
	return data_rate;
}

//typedef enum WifiGeeDataRates {
//	kWifiGee6mbps  = 0xD,
//	kWifiGee9mbps  = 0xF,
//	kWifiGee12mbps = 0x5,
//	kWifiGee18mbps = 0x7,
//	kWifiGee24mbps = 0x9,
//	kWifiGee36mbps = 0xB,
//	kWifiGee48mbps = 0x1,
//	kWifiGee54mbps = 0x3,
//} WifiGeeDataRates;


/** Structure which defines  */
typedef struct WifiGeeDataRatesInfo
{
	uint32_t num_data_bits_per_ofdm_symbol;
	uint32_t num_dword_containers_of_data_bits; //64 data bits per container
	uint32_t num_coded_bits_per_ofdm_symbol;
	uint32_t num_dword_containers_of_coded_bits; 
	WifiGeeModulationTypes modulation_type;
}WifiGeeDataRatesInfo;

static const WifiGeeDataRatesInfo kWifiGeeDataRatesInfo[8]= 
			{/* dbits|dwords| cbits | dwords | mod type | */
				{24,	1,		48,		1,	kWifiGeeBpsk},
				{36,	1,		48,		1,	kWifiGeeBpsk},
				{48,	1,		96,		2,	kWifiGeeQpsk},
				{72,	2,		96,		2,	kWifiGeeQpsk},
				{96,	2,		192,	3,	kWifiGee16qam},
				{144,	3,		192,	3,	kWifiGee16qam},
				{192,	3,		288,	5,	kWifiGee64qam},
				{216,	4,		288,	5,	kWifiGee64qam} 	}; 



#endif
