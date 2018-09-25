/* otgfsm_types.h	Mon Mar 02 2015 17:09:17 chwon */

/*

Module:  otgfsm_types.h

Function:
	The externally useful types of the OTG FSM.

Version:
	V3.15b	Mon Mar 02 2015 17:09:17 chwon	Edit level 8

Copyright notice:
	This file copyright (C) 2005-2008, 2011, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	August 2005

Revision history:
   1.91h  Tue Aug 23 2005 15:53:26  tmm
	Module created.

   1.91i  Sun Sep 18 2005 23:15:02  chwon
	Fix OtgSt compare logic in __TMS_USBPUMP_OTGSTATE_IS_A_ROLE()

   1.97j  Tue Nov 13 2007 17:09:06  chwon
	3471: add a_srp_based_mode flag

   1.97k  Thu Mar 06 2008 16:24:40  chwon
	3567: add a_srp_trying and b_hnp_trying flag

   1.97k  Mon Mar 10 2008 10:44:54  chwon
	3567: revert changes -- remove a_srp_trying and b_hnp_trying flag

   3.01f  Tue Nov 08 2011 16:40:32  chwon
	9941: add USBPUMP_OTGFSM_EVAL_FN & USBPUMP_OTGFSM_IOCTL_FN definition.
	Change type from OTGTIMERID to USBPUMP_OTGTIMERID. Add OTG V2.0 flags
	in the USBPUMP_OTGVARS.

   3.15b  Mon Mar 02 2015 17:09:17  chwon
	19011: Add USBPUMP_OTGFSM_EVENT_NOTIFY_FN definition.

*/

#ifndef _OTGFSM_TYPES_H_		/* prevent multiple includes */
#define _OTGFSM_TYPES_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

/****************************************************************************\
|
|	Declare common central types used by OTGFSM implementation.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USBPUMP_OTGFSM);
__TMS_TYPE_DEF_STRUCT(USBPUMP_OTGVARS);

__TMS_TYPE_DEF_ARG(USBPUMP_OTGSTATE, UINT32);
__TMS_TYPE_DEF_ARG(USBPUMP_OTGTIMERID, UINT32);
__TMS_TYPE_DEF_ARG(OTGTIMERID, UINT32);	/* not used: backward compatibility */

__TMS_FNTYPE_DEF(
USBPUMP_OTGFSM_EVAL_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_OTGFSM *	/* pOtgFsm */
	));

/*
|| The user of this FSM needs to pass us IOCTLs.  We want the IOCTL function
|| to look like every other specialized IOCTL in the system, so we create
|| a type using the standard macro.  However, we'll only use this type
|| once, in the declaration of UsbPumpOtgFsm_Ioctl.
*/
__TMS_USBPUMP_DECLARE_IOCTL_FNTYPE(
USBPUMP_OTGFSM_IOCTL_FN,
	__TMS_USBPUMP_OTGFSM *	/* pOtgFsm */
	);

__TMS_FNTYPE_DEF(
USBPUMP_OTGFSM_EVENT_NOTIFY_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_OTGFSM *	/* pOtgFsm */,
	__TMS_UINT32		/* Event */
	));


/****************************************************************************\
|
|	The OTG states
|
\****************************************************************************/

#define	__TMS_OTGST_initial		0
#define	__TMS_OTGST_a_idle		1
#define	__TMS_OTGST_a_wait_vrise	2
#define	__TMS_OTGST_a_wait_bcon		3
#define	__TMS_OTGST_a_host		4
#define	__TMS_OTGST_a_suspend		5
#define	__TMS_OTGST_a_peripheral	6
#define	__TMS_OTGST_a_wait_vfall	7
#define	__TMS_OTGST_a_vbus_err		8

#define	__TMS_OTGST_b_idle		9
#define	__TMS_OTGST_b_srp_init		10
#define	__TMS_OTGST_b_srp_1		11	/* MCCI: for SRP data pulse */
#define	__TMS_OTGST_b_srp_2		12	/* MCCI: for SRP Vbus */
#define	__TMS_OTGST_b_peripheral	13
#define	__TMS_OTGST_b_wait_acon		14
#define	__TMS_OTGST_b_host		15

#define	__TMS_OTGST_carkit		16	/* added by MCCI */

#define	__TMS_OTGST_no_change		17


/* true if the FSM thinks we're an A-type device */
#define	__TMS_USBPUMP_OTGSTATE_IS_A_ROLE(otgSt)		\
	(__TMS_OTGST_a_idle <= (otgSt) &&		\
	 (otgSt) <= __TMS_OTGST_a_vbus_err)

/* true if the FSM thinks we're a B-type device */
#define	__TMS_USBPUMP_OTGSTATE_IS_B_ROLE(otgSt)		\
	(__TMS_OTGST_b_idle <= (otgSt) &&		\
	 (otgSt) <= __TMS_OTGST_b_host)

/* true if the FSM thinks we're a carkit (not USB) */
#define	__TMS_USBPUMP_OTGSTATE_IS_CARKIT(otgSt)		\
	((otgSt) == __TMS_OTGST_carkit)

/* use this to initialize an array for displaying states symbolically */
#define	__TMS_OTGST_VALUES__INIT	\
	"OTGST_initial",		\
	"OTGST_a_idle",			\
	"OTGST_a_wait_vrise",		\
	"OTGST_a_wait_bcon",		\
	"OTGST_a_host",			\
	"OTGST_a_suspend",		\
	"OTGST_a_peripheral",		\
	"OTGST_a_wait_vfall",		\
	"OTGST_a_vbus_err",		\
	"OTGST_b_idle",			\
	"OTGST_b_srp_init",		\
	"OTGST_b_srp_1",		\
	"OTGST_b_srp_2",		\
	"OTGST_b_peripheral",		\
	"OTGST_b_wait_acon",		\
	"OTGST_b_host",			\
	"OTGST_carkit"

/****************************************************************************\
|
|	The OTG state variables
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_OTGVARS)
	{
	/**********************\
	| THE STATE OF THE FSM |
	\**********************/
	__TMS_USBPUMP_OTGSTATE	otg_state;

	unsigned
		/*****************************************\
		| OUTPUTS TO TRANCEIVER OR OTHER SOFTWARE |
		\*****************************************/

		/*
		|| MCCI: the following bit is set by the FSM whenever
		|| it changes the value of State; it is reset as
		|| desired by the transceiver.
		*/
		delta_otg_state : 1,	/* o: set by FSM on any change
					|| in the OTG state variable;
					|| reset by transceiver as desired
					*/	

		/*
		|| 6.6.3.2: the transceiver should use drv_vbus to
		|| control Vbus at the connector.
		*/
		drv_vbus: 1,		/* o: drive VBUS */
		delta_drv_vbus: 1,	/* o: set by FSM on any change,
					|| reset by transceiver as desired
					*/
		/*
		|| 6.6.3.1: the transceiver should use chrg_vbus to
		|| control Vbus at the connector. [V1.0 only]
		*/
		chrg_vbus: 1,		/* o: charge VBUS */
		delta_chrg_vbus: 1,	/* o: set by FSM on any change,
					|| reset by transceiver as desired
					*/

		/*
		|| MCCI: the transceiver should use dschrg_vbus to
		|| control Vbus at the connector -- since we include
		|| SRP as part of the FSM, the FSM will activate this
		|| as needed. [V1.0 only]
		*/
		dischrg_vbus: 1,	/* o: discharge VBUS */
		delta_dischrg_vbus: 1,	/* o: set by FSM on any change,
					|| reset by transceiver as desired
					*/
		/*
		|| 6.6.2.2: set true when SRP is finished
		*/
		b_srp_done: 1,		/* o: set internally when SRP is done.
					|| i: can also be set by smart
					|| transceivers, if desired.
					*/
		delta_b_srp_done: 1,	/* o: set by FSM on any change,
					|| reset by transceiver as desired.
					|| [V1.0 only]
					*/
		/*
		|| 6.6.3.3: the transceiver should pull-up D+ as normal
		|| when loc_conn is set.
		*/
		loc_conn: 1,		/* o: pull-up D+ */
		delta_loc_conn: 1,	/* o: set by FSM on any change,
					|| reset by transceiver as desired
					*/
		/*
		|| 6.6.3.4: the HCD should start operating as a host
		|| when loc_sof is set.  The transceiver should start
		|| the process of generating root-hub status changes
		|| if loc_sof is set.
		*/
		loc_sof: 1,		/* o: send SOFs and run host */
		delta_loc_sof: 1,	/* o: set by FSM on any change,
					|| reset by transceiver as desired
					*/

		/*
		|| MCCI: by default, SRP initiation is managed from the
		|| FSM.  However, we do have a local "SRP start"
		|| variable, for use by an intelligent transceiver.
		*/
		/*
		|| [V2.0]: 7.4.2.1: data_pulse
		|| The ¡°data line pulsing¡± (data_pulse) signal is TRUE when
		|| a B-device is performing data line pulsing.
		*/
		loc_srp_start: 1,	/* o: start SRP */
		delta_loc_srp_start: 1,	/* o: set by FSM on any change,
					|| reset by transceiver as desired
					*/

		/*
		|| [V2.0]: 7.4.2.5:
		|| The ADP probe variable (adp_prb) is TRUE when the local
		|| device is doing ADP probing.
		|| An ADP-capable B-device shall first perform ADP sensing
		|| (adp_sns = TRUE). If the B-device sense comparator does not
		|| toggle for a time of TB_ADP_DETACH, the B-device shall then
		|| perform ADP probing and adp_prb will become TRUE.
		*/
		adp_prb: 1,		/* o: start ADP probing */
		delta_adp_prb: 1,	/* o: set by FSM on any change,
					|| reset by transceiver as desired
					*/

		/*
		|| [V2.0]: 7.4.2.6:
		|| The ADP sense variable (adp_sns) is TRUE when the local
		|| device is doing ADP sensing. This output is used in
		|| preference to adp_prb when the B-device detects that the
		|| A-device is doing ADP probing.
		*/
		adp_sns: 1,		/* o: start ADP sensing */
		delta_adp_sns: 1,	/* o: set by FSM on any change,
					|| reset by transceiver as desired
					*/

		/*
		|| MCCI: some OTG HCD/DCD hardare shares registers, and
		|| needs to be told "how to act".  So we generate run_dcd
		|| and run_hcd.
		*/
		run_dcd: 1,		/* o: run the DCD */
		delta_run_dcd: 1,	/* o: set by FSM on any change,
					|| reset by transceiver as desired
					*/

		run_hcd: 1,		/* o: run the HCD */
		delta_run_hcd: 1,	/* o: set by FSM on any change,
					|| reset by transceiver as desired
					*/

		/****************************\
		| INPUTS FROM OTHER SOFTWARE |
		\****************************/

		/*
		|| 6.6.1.17: the application signals that it wants to
		|| suspend the bus without ending the session. [V1.0 only]
		*/
		a_suspend_req: 1,	/* i: request to suspend A host */

		/*
		|| 6.6.2.1: set by the higher layers when they enable
		|| HNP for the b device.
		*/
		a_set_b_hnp_en: 1,	/* i: HNP is enabled for other side */

		/*
		|| 6.6.1.1: the local application wants the transceiver
		|| to stop driving Vbus
		*/
		a_bus_drop: 1,		/* i: request to stop Vbus */

		/*
		|| 6.6.1.2: the local application wants to run as host (lower
		|| priority than a_bus_drop.
		*/
		a_bus_req: 1,		/* i: request to run as host */

		/*
		|| 6.6.4: a_clr_err is set by the application when the
		|| OTGCD is in the vbus error state.  It's cleared by the
		|| FSM.
		*/
		a_clr_err: 1,		/* i: toggle to get us out of error state */

		/*
		|| 6.6.9: set by the application while it wants to use
		|| the bus.
		*/
		b_bus_req: 1,		/* i: b-bus request */


		/*
		|| MISSING from 1.2rc9 spec:  set by the DCD when the
		|| SET_FEATURE(b_hnp_enable) is received, and cleared on bus 
		|| reset.
		*/
		/*
		|| [V2.0] 7.4.3.3:
		|| The ¡°B-device set b_hnp_enable¡± (b_hnp_en) bit is TRUE
		|| when the B-device has accepted the SetFeature(b_hnp_enable).
		|| b_hnp_en is cleared on a bus reset or when b_sess_vld is
		|| FALSE.
		*/
		b_hnp_enable: 1,	/* i(dcd): HNP enabled as b device */

		/*
		|| MISSING from 1.2rc9 spec:  set by the DCD to request a
		|| wakeup without also requesting a transition to bus
		|| operation.  This is more or less equivalent to remote
		|| wakeup.
		*/
		b_sess_req: 1,		/* i(dcd): request a perhipheral session */

		/*
		|| [V2.0] 7.4.1.3:
		|| The power up value (power_up) is TRUE when the device first
		|| powers up its USB system.
		*/
		power_up: 1,		/* i: OTG device first power up */

		/*
		|| [V2.0] 7.4.1.4:
		|| The test device value (test_device) is TRUE when the OTG
		|| device or EH is attached to a test device.
		*/
		test_device: 1,		/* i: attached a test device */

		/****************************************************\
		| INPUTS FROM TRANSCEIVER OR OTHER BUS-STATE SENSORS |
		\****************************************************/

		/*
		|| 6.6.1.5: set by the transceiver, but the transceiver
		|| must consider the OTG FSM state in order to do this
		|| properly.
		*/
		a_conn: 1,		/* x: a-side connect */

		/*
		|| 6.6.1.6: set by the transceiver when Vbus is valid for
		|| a-session.
		*/
		a_sess_vld: 1,		/* x: Vbus is valid for a-session */

		/*
		|| 6.6.1.7: the transceiver should set a_srp_det if an
		|| srp is detected.  the FSM will clear it.
		*/
		a_srp_det: 1,		/* x: SRP detected */

		/*
		|| 6.6.1.8: indicates state of Vbus compared to VBUS_VALID.
		*/
		a_vbus_vld: 1,		/* x: Vbus is valid for A-session */

		/*
		|| 6.6.1.10: the FSM is in a_suspend state and either
		|| the transceiver or the root hub or the host controller
		|| detect a resume signalled by the device.
		*/
		b_bus_resume: 1,	/* x: detected remote wakeup */

		/*
		|| 6.6.1.11: the A device has detected that the bus is
		|| suspended while controlled by the B device.
		*/
		b_bus_suspend: 1,	/* x: detected suspend when a-dev is
					||    acting as a peripheral
					*/

		/*
		|| 6.6.1.12: the b_conn signal from the transceiver
		|| indicates that a B-device has connected to this
		|| device operating as a host.  There are many conditions
		|| on how the transceiver must handle this; see section 
		|| 6.6.1.12.
		*/
		b_conn: 1,		/* x: D+ observed high */

		/*
		|| 6.6.1.13: reset on entry to b_idle by FSM, set by 
		|| transceiver after being in SE0 state
		|| for more than TB_SE0_SRP milliseconds (2 ms).  This
		|| corresponds to b_se0_srp in the spec.  It would be nice
		|| if the FSM could help with this, but there seems no easy
		|| way without departing markedly from the spec's FSM.
		*/
		b_se0_srp: 1,		/* x: we've been at se0 long enough to SRP */

		/*
		|| 6.6.1.14: set by the transceiver based on level of
		|| VBUS.  This coud be conditioned with knowledge of the
		|| current state.
		*/
		b_sess_end: 1,		/* x: b-session end */

		/*
		|| [V2.0] 7.4.1.16:
		|| The ¡°B-device session end SRP¡± (b_ssend_srp) input is TRUE
		|| if the B-device detects that the voltage on VBUS has been
		|| below the Session Valid threshold (VOTG_SESS_VLD) for at
		|| least TB_SSEND_SRP.
		*/
		b_ssend_srp: 1,		/* x: b-session end and start SRP */

		/*
		|| 6.6.1.15: set by transceiver when Vbus is greater than
		|| VB_SESS_VLD
		*/
		b_sess_vld: 1,		/* x: b session valid */

		/*
		|| 6.6.1.16: (MCCI) the id input is FALSE when a Mini-A plug
		|| is inserted, otherwise TRUE.  However, the 1301 has two
		|| outputs:  id_float and id_mid.  id_float and id_mid are
		|| FALSE when a mini-A plug is inserted; id_float and id_mid
		|| is FALSE and id_mid is TRUE when a car-kit connector
		|| is inserted; and both are TRUE otherwise.  So in the
		|| FSM, id == (/id_float && /id_mid) - except that if we
		|| see id_float && /id_mid in a_idle or b_idle, we go to
		|| the car-kit state.
		*/
		id_float: 1,		/* x: ID pin is floating */
		id_mid: 1,		/* x: ID is "mid-impedance */

		/*
		|| [V2.0] 7.4.1.2:
		|| The ADP change value (adp_change) is TRUE when an OTG device,
		|| EH or SRP-capable peripheral-only B-device is doing ADP
		|| probing (adp_prb = TRUE) and two successive ADP measurement
		|| values are different by more than CADP_THR.
		|| The ADP change value (adp_change) is TRUE when a B-device is
		|| doing ADP sensing of an ADP-capable A-device and then detects
		|| that the ADP probing signal is no longer present.
		*/
		adp_change: 1,		/* x: ADP change */

		/********************************\
		| INPUTS FROM DCD or TRANSCEIVER |
		\********************************/

		/*
		|| 6.6.1.3: a_bus_resume is set when the transceiver or
		|| DCD detect a resume state on the bus.  Sometimes this 
		|| is any activity, sometimes only if a K state is seen;
		|| must consider OTG FSM state, per section 6.6.1.3
		*/
		a_bus_resume: 1,	/* x or i(dcd): a-side bus resumed */

		/*
		|| 6.6.1.4: this must set by the transceiver or DCD when the 
		|| bus is suspended,
		*/
		a_bus_suspend: 1,	/* x or i(dcd): a-side bus suspended */

		/********************\
		| INPUTS FROM TIMERS |
		\********************/
		/*
		|| 6.6.5.1: timer waiting for VBUS to become valid.
		*/
		a_wait_vrise_tmout: 1,	/* t: Vrise timer timed out */

		/*
		|| 6.6.5.2: set to timeout Vbus ON -> device connect.
		|| Platforms are allowed to adjust or even disable this
		|| timer, if desired.
		*/
		a_wait_bcon_tmout: 1,	/* t: b-connect timeout */

		/*
		|| 6.6.5.3: Timer waiting from a_suspend to disconnect.
		*/
		a_aidl_bdis_tmout: 1,	/* t: disconnect timeout */

		/*
		|| [V2.0] 7.4.5.2:
		|| This timer is used by an A-device in the a_wait_vfall state
		|| while waiting for the voltage on VBUS to fall below the
		|| VBUS leakage voltage VOTG_VBUS_LKG.
		*/
		a_wait_vfall_tmout: 1,	/* t: Vbus falling timeout */

		/*
		|| [V2.0] 7.4.5.6:
		|| This timer is started by an A-device when it enters the
		|| a_suspend state if HNP has been enabled (a_set_b_hnp_en =
		|| TRUE). If the A-device does not detect a disconnect before
		|| TA_AIDL_BDIS (a_aidl_bdis_tmout = TRUE), then the A-device
		|| is allowed to stop waiting for a disconnect and end the
		|| session.
		*/
		a_bidl_adis_tmout: 1,	/* t: a-disconnect timeout */

		/*
		|| 6.6.5.4: Timer waiting for A device to signal a connection
		*/
		b_ase0_brst_tmout: 1,	/* t: single-ended 0 / reset timeout */

		/*
		|| MCCI: data pulse time for SRP has elapsed.
		*/
		b_datapulse_tmout: 1,	/* t: b datapulse timeout */

		/*
		|| MCCI: vbuschrg timeout has elapsed.
		*/
		b_vbuschrg_tmout: 1	/* t: b vbus charge timeout */
		;

	/*
	|| The variables in this group are from MCCI's implementation of
	|| the FSM.
	*/
	unsigned
		/*
		|| if the FSM is in a state where it is impossible to 
		|| connect as a device, it will set b_conn_en to zero.
		|| This in turn keeps b_conn from glitching high when
		|| it's not permitted to do so.
		||
		|| the transceiver can use this variable to determine whether
		|| a D+ or D+ pullup being active should result in setting
		|| b_conn.
		*/
		b_conn_en: 1,		/* set true when b_conn is enabled */
		delta_b_conn_en: 1,	/* o: set by FSM on any change,
					|| reset by transceiver as desired
					*/

		/*
		|| if the FSM is in a state where it is impossible to 
		|| connect as a host, it will set a_conn_en to zero. 
		|| This in turn keeps a_conn from glitching high when
		|| it's not permitted to do so.
		*/
		a_conn_en: 1,		/* set true when a_conn is enabled */
		delta_a_conn_en: 1,	/* o: set by FSM on any change,
					|| reset by transceiver as desired
					*/

		/*
		|| This is an output from the FSM, and it indicates that
		|| b_conn must have a long debounce.  It is set if
		|| we enter OGTST_wait_bconn from OTGST_wait_vrise, per
		|| section 6.6.1.12 (yes, probably the states should have
		|| been split in the OTG FSM).
		*/
		b_conn_long: 1,		/* set true with b_connect must
					|| have long debounce
					*/

		/*
		|| See section 6.6.1.12 for discussion, but this is the
		|| indication to the transceiver that the modified timing
		|| applies.
		*/
		b_ldis_dischg: 1,	/* set true when b_conn debounce must
					|| include TLDIS_DSCHG
					*/

		/*
		|| This is an input from the client, and it indicates that
		|| the FSM is acting as SRP based or Insertion based OTG device.
		*/
		a_srp_based_mode: 1	/* set true when FSM is SRP based mode */
		;
	};

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define OTGST_initial	\
   __TMS_OTGST_initial
# define OTGST_a_idle	\
   __TMS_OTGST_a_idle
# define OTGST_a_wait_vrise	\
   __TMS_OTGST_a_wait_vrise
# define OTGST_a_wait_bcon	\
   __TMS_OTGST_a_wait_bcon
# define OTGST_a_host	\
   __TMS_OTGST_a_host
# define OTGST_a_suspend	\
   __TMS_OTGST_a_suspend
# define OTGST_a_peripheral	\
   __TMS_OTGST_a_peripheral
# define OTGST_a_wait_vfall	\
   __TMS_OTGST_a_wait_vfall
# define OTGST_a_vbus_err	\
   __TMS_OTGST_a_vbus_err
# define OTGST_b_idle	\
   __TMS_OTGST_b_idle
# define OTGST_b_srp_init	\
   __TMS_OTGST_b_srp_init
# define OTGST_b_srp_1	\
   __TMS_OTGST_b_srp_1
# define OTGST_b_srp_2	\
   __TMS_OTGST_b_srp_2
# define OTGST_b_peripheral	\
   __TMS_OTGST_b_peripheral
# define OTGST_b_wait_acon	\
   __TMS_OTGST_b_wait_acon
# define OTGST_b_host	\
   __TMS_OTGST_b_host
# define OTGST_carkit	\
   __TMS_OTGST_carkit
# define OTGST_no_change	\
   __TMS_OTGST_no_change
# define USBPUMP_OTGSTATE_IS_A_ROLE(otgSt)	\
   __TMS_USBPUMP_OTGSTATE_IS_A_ROLE(otgSt)
# define USBPUMP_OTGSTATE_IS_B_ROLE(otgSt)	\
   __TMS_USBPUMP_OTGSTATE_IS_B_ROLE(otgSt)
# define USBPUMP_OTGSTATE_IS_CARKIT(otgSt)	\
   __TMS_USBPUMP_OTGSTATE_IS_CARKIT(otgSt)
# define OTGST_VALUES__INIT	\
   __TMS_OTGST_VALUES__INIT
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of otgfsm_types.h ****/
#endif /* _OTGFSM_TYPES_H_ */
