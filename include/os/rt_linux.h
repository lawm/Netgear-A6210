/****************************************************************************

	Module Name:
	rt_linux.h

	Abstract:
	Any OS related definition/MACRO is defined here.

	Revision History:
	Who        When          What
	---------  ----------    ----------------------------------------------

***************************************************************************/

#ifndef __RT_LINUX_H__
#define __RT_LINUX_H__

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/ethtool.h>
#include <linux/wireless.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/if_arp.h>
#include <linux/ctype.h>
#include <linux/vmalloc.h>
#ifdef RTMP_USB_SUPPORT
#include <linux/usb.h>
#endif /* RTMP_USB_SUPPORT */
#include <linux/wireless.h>
#include <net/iw_handler.h>

#ifdef INF_PPA_SUPPORT
#include <net/ifx_ppa_api.h>
#include <net/ifx_ppa_hook.h>
#endif /* INF_PPA_SUPPORT */

/* load firmware */
#define __KERNEL_SYSCALLS__
#include <linux/unistd.h>
#include <asm/uaccess.h>
#include <asm/types.h>
#include <asm/unaligned.h>	/* for get_unaligned() */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
#include <linux/pid.h>
#endif

#ifdef RT_CFG80211_SUPPORT
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,28))
#include <net/mac80211.h>
//#define EXT_BUILD_CHANNEL_LIST		/* must define with CRDA */
#else /* LINUX_VERSION_CODE */
#undef RT_CFG80211_SUPPORT
#endif /* LINUX_VERSION_CODE */
#endif /* RT_CFG80211_SUPPORT */


/* must put the definition before include "os/rt_linux_cmm.h" */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)
#define KTHREAD_SUPPORT 1
#endif /* LINUX_VERSION_CODE */

#ifdef KTHREAD_SUPPORT
#include <linux/err.h>
#include <linux/kthread.h>
#endif /* KTHREAD_SUPPORT */

#include "os/rt_linux_cmm.h"

#ifdef RT_CFG80211_SUPPORT
#include "cfg80211.h"
#endif /* RT_CFG80211_SUPPORT */

#include <linux/firmware.h>

#undef AP_WSC_INCLUDED
#undef STA_WSC_INCLUDED
#undef WSC_INCLUDED

#ifdef CONFIG_AP_SUPPORT
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
#endif /* CONFIG_STA_SUPPORT */


#ifdef KTHREAD_SUPPORT
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,4)
#error "This kerne version doesn't support kthread!!"
#endif
#endif /* KTHREAD_SUPPORT */

/*#ifdef RTMP_USB_SUPPORT // os abl move */
typedef struct usb_device	*PUSB_DEV;
typedef struct urb *purbb_t;
typedef struct usb_ctrlrequest devctrlrequest;
/*#endif */

/***********************************************************************************
 *	Profile related sections
 ***********************************************************************************/
#ifdef CONFIG_AP_SUPPORT

#ifdef RTMP_MAC_USB
#ifdef INF_AMAZON_SE
#define AP_PROFILE_PATH                 "/ramdisk/etc/Wireless/RT2870AP/RT2870AP.dat"
#define AP_RTMP_FIRMWARE_FILE_NAME "/ramdisk/etc/Wireless/RT2870AP/RT2870AP.bin"
#else
#define AP_PROFILE_PATH			"/etc/Wireless/RT2870AP/RT2870AP.dat"
#define AP_RTMP_FIRMWARE_FILE_NAME "/etc/Wireless/RT2870AP/RT2870AP.bin"
#endif
#define AP_DRIVER_VERSION			"3.0.0.0"
#ifdef MULTIPLE_CARD_SUPPORT
#define CARD_INFO_PATH			"/etc/Wireless/RT2870AP/RT2870APCard.dat"
#endif /* MULTIPLE_CARD_SUPPORT */
#endif /* RTMP_MAC_USB */

#endif /* CONFIG_AP_SUPPORT */


#ifdef CONFIG_STA_SUPPORT

#ifdef RTMP_MAC_USB
#define STA_PROFILE_PATH			"/etc/Wireless/RT2870STA/RT2870STA.dat"
#define STA_DRIVER_VERSION			"3.0.0.1"
#ifdef MULTIPLE_CARD_SUPPORT
#define CARD_INFO_PATH			"/etc/Wireless/RT2870STA/RT2870STACard.dat"
#endif /* MULTIPLE_CARD_SUPPORT */
#endif /* RTMP_MAC_USB */


extern const struct iw_handler_def rt28xx_iw_handler_def;

#ifdef SINGLE_SKU_V2
#define SINGLE_SKU_TABLE_FILE_NAME	"/etc/Wireless/RT2870STA/SingleSKU.dat"
#endif /* SINGLE_SKU_V2 */

#endif /* CONFIG_STA_SUPPORT */

#ifdef CONFIG_APSTA_MIXED_SUPPORT
extern	const struct iw_handler_def rt28xx_ap_iw_handler_def;
#endif /* CONFIG_APSTA_MIXED_SUPPORT */

/***********************************************************************************
 *	Compiler related definitions
 ***********************************************************************************/
#undef __inline
#define __inline static inline
#define IN
#define OUT
#define INOUT
#define NDIS_STATUS	 INT


/***********************************************************************************
 *	OS Specific definitions and data structures
 ***********************************************************************************/
typedef struct net_device_stats	NET_DEV_STATS;
typedef struct pci_dev 		* PPCI_DEV;
typedef struct net_device	* PNET_DEV;
typedef struct wireless_dev	* PWIRELESS_DEV;
typedef void				* PNDIS_PACKET;
typedef char				NDIS_PACKET;
typedef PNDIS_PACKET		* PPNDIS_PACKET;
typedef	ra_dma_addr_t		NDIS_PHYSICAL_ADDRESS;
typedef	ra_dma_addr_t		* PNDIS_PHYSICAL_ADDRESS;
typedef void				* NDIS_HANDLE;
typedef char 				* PNDIS_BUFFER;

typedef struct ifreq		NET_IOCTL;
typedef struct ifreq		* PNET_IOCTL;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
typedef	struct pid *		RTMP_OS_PID;
#else
typedef pid_t 				RTMP_OS_PID;
#endif

typedef struct semaphore	OS_SEM;

typedef int (*HARD_START_XMIT_FUNC)(struct sk_buff *skb, struct net_device *net_dev);


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
#define RT_MOD_INC_USE_COUNT() \
	if (!try_module_get(THIS_MODULE)) \
	{ \
		DBGPRINT(RT_DEBUG_ERROR, ("%s: cannot reserve module\n", __FUNCTION__)); \
		return -1; \
	}

#define RT_MOD_DEC_USE_COUNT() module_put(THIS_MODULE);
#else
#define RT_MOD_INC_USE_COUNT()	MOD_INC_USE_COUNT;
#define RT_MOD_DEC_USE_COUNT() MOD_DEC_USE_COUNT;
#endif

#define RTMP_INC_REF(_A)		0
#define RTMP_DEC_REF(_A)		0
#define RTMP_GET_REF(_A)		0


#if WIRELESS_EXT >= 12
/* This function will be called when query /proc */
struct iw_statistics *rt28xx_get_wireless_stats(
	IN struct net_device *net_dev);
#endif


/***********************************************************************************
 *	Network related constant definitions
 ***********************************************************************************/
#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

#define MAC_ADDR_LEN		6

#define NDIS_STATUS_SUCCESS                     0x00
#define NDIS_STATUS_FAILURE                     0x01
#define NDIS_STATUS_INVALID_DATA				0x02
#define NDIS_STATUS_RESOURCES                   0x03
#define NDIS_STATUS_MICERROR                   0x04

#define NDIS_SET_PACKET_STATUS(_p, _status)			do{} while(0)
#define NdisWriteErrorLogEntry(_a, _b, _c, _d)		do{} while(0)

/* statistics counter */
#define STATS_INC_RX_PACKETS(_pAd, _dev)
#define STATS_INC_TX_PACKETS(_pAd, _dev)

#define STATS_INC_RX_BYTESS(_pAd, _dev, len)
#define STATS_INC_TX_BYTESS(_pAd, _dev, len)

#define STATS_INC_RX_ERRORS(_pAd, _dev)
#define STATS_INC_TX_ERRORS(_pAd, _dev)

#define STATS_INC_RX_DROPPED(_pAd, _dev)
#define STATS_INC_TX_DROPPED(_pAd, _dev)


/***********************************************************************************
 *	Ralink Specific network related constant definitions
 ***********************************************************************************/
#ifdef CONFIG_STA_SUPPORT
#define NDIS_PACKET_TYPE_DIRECTED		0
#define NDIS_PACKET_TYPE_MULTICAST		1
#define NDIS_PACKET_TYPE_BROADCAST		2
#define NDIS_PACKET_TYPE_ALL_MULTICAST	3
#define NDIS_PACKET_TYPE_PROMISCUOUS	4
#endif /* CONFIG_STA_SUPPORT */

#ifdef DOT11_VHT_AC
#define MAX_PACKETS_IN_QUEUE				1024 /*(512)*/
#else
#define MAX_PACKETS_IN_QUEUE				(512)
#endif /* DOT11_VHT_AC */


/***********************************************************************************
 *	OS signaling related constant definitions
 ***********************************************************************************/


/***********************************************************************************
 *	OS file operation related data structure definitions
 ***********************************************************************************/
typedef struct file* RTMP_OS_FD;

typedef struct _OS_FS_INFO_
{
// jb was int				fsuid;
// jb was int				fsgid;
	kuid_t			fsuid;
	kgid_t			fsgid;
	mm_segment_t	fs;
} OS_FS_INFO;

#define IS_FILE_OPEN_ERR(_fd) 	((_fd == NULL) || IS_ERR((_fd)))


/***********************************************************************************
 *	OS semaphore related data structure and definitions
 ***********************************************************************************/
struct os_lock  {
	spinlock_t		lock;
	unsigned long  	flags;
};

typedef spinlock_t			OS_NDIS_SPIN_LOCK;

/* */
/*  spin_lock enhanced for Nested spin lock */
/* */
#define OS_NdisAllocateSpinLock(__lock)			\
{                                       		\
	spin_lock_init((spinlock_t *)(__lock));		\
}

#define OS_NdisFreeSpinLock(lock)				\
	do{}while(0)


#define OS_SEM_LOCK(__lock)						\
{												\
	spin_lock_bh((spinlock_t *)(__lock));		\
}

#define OS_SEM_UNLOCK(__lock)					\
{												\
	spin_unlock_bh((spinlock_t *)(__lock));		\
}


/* sample, use semaphore lock to replace IRQ lock, 2007/11/15 */
#ifdef MULTI_CORE_SUPPORT

#define OS_IRQ_LOCK(__lock, __irqflags)			\
{													\
	__irqflags = 0;									\
	spin_lock_irqsave((spinlock_t *)(__lock), __irqflags);			\
}

#define OS_IRQ_UNLOCK(__lock, __irqflag)			\
{													\
	spin_unlock_irqrestore((spinlock_t *)(__lock), __irqflag);			\
}
#else
#define OS_IRQ_LOCK(__lock, __irqflags)			\
{												\
	__irqflags = 0;								\
	spin_lock_bh((spinlock_t *)(__lock));		\
}

#define OS_IRQ_UNLOCK(__lock, __irqflag)		\
{												\
	spin_unlock_bh((spinlock_t *)(__lock));		\
}
#endif // MULTI_CORE_SUPPORT //
#define OS_INT_LOCK(__lock, __irqflags)			\
{												\
	spin_lock_irqsave((spinlock_t *)__lock, __irqflags);	\
}

#define OS_INT_UNLOCK(__lock, __irqflag)		\
{												\
	spin_unlock_irqrestore((spinlock_t *)(__lock), ((unsigned long)__irqflag));	\
}

#define OS_NdisAcquireSpinLock		OS_SEM_LOCK
#define OS_NdisReleaseSpinLock		OS_SEM_UNLOCK

/*
	Following lock/unlock definition used for BBP/RF register read/write.
	Currently we don't use it to protect MAC register access.

	For USB:
			we use binary semaphore to do the protection because all register
			access done in kernel thread and should allow task go sleep when
			in protected status.

	For PCI/PCI-E/RBUS:
			We use interrupt to do the protection because the register may accessed
			in thread/tasklet/timer/inteerupt, so we use interrupt_disable to protect
			the access.
*/
#define RTMP_MCU_RW_LOCK(_pAd, _irqflags)	\
	do{								\
		if (_pAd->infType == RTMP_DEV_INF_USB)	\
		{\
			RTMP_SEM_EVENT_WAIT(&_pAd->McuCmdSem, _irqflags);\
		}\
		else\
		{\
			RTMP_SEM_LOCK(&_pAd->McuCmdLock, _irqflags);\
		}\
	}while(0)

#define RTMP_MCU_RW_UNLOCK(_pAd, _irqflags)	\
	do{				\
		if(_pAd->infType == RTMP_DEV_INF_USB)\
		{	\
			RTMP_SEM_EVENT_UP(&_pAd->McuCmdSem);\
		}		\
		else\
		{\
			RTMP_SEM_UNLOCK(&_pAd->McuCmdLock, _irqflags);\
		}\
	}while(0)


#ifndef wait_event_interruptible_timeout
#define __wait_event_interruptible_timeout(wq, condition, ret) \
do { \
		wait_queue_t __wait; \
		init_waitqueue_entry(&__wait, current); \
		add_wait_queue(&wq, &__wait); \
		for (;;) { \
				set_current_state(TASK_INTERRUPTIBLE); \
				if (condition) \
						break; \
				if (!signal_pending(current)) { \
						ret = schedule_timeout(ret); \
						if (!ret) \
								break; \
						continue; \
				} \
				ret = -ERESTARTSYS; \
				break; \
		} \
		current->state = TASK_RUNNING; \
		remove_wait_queue(&wq, &__wait); \
} while (0)

#define wait_event_interruptible_timeout(wq, condition, timeout) \
({ \
		long __ret = timeout; \
		if (!(condition)) \
				__wait_event_interruptible_timeout(wq, condition, __ret); \
		__ret; \
})
#endif

#define OS_SEM_EVENT_INIT_LOCKED(_pSema) 	sema_init((_pSema), 0)
#define OS_SEM_EVENT_INIT(_pSema)			sema_init((_pSema), 1)
#define OS_SEM_EVENT_DESTORY(_pSema)		do{}while(0)
#define OS_SEM_EVENT_WAIT(_pSema, _status)	((_status) = down_interruptible((_pSema)))
#define OS_SEM_EVENT_UP(_pSema)				up(_pSema)

#define RTCMDUp					OS_RTCMDUp

#ifdef KTHREAD_SUPPORT
#define RTMP_WAIT_EVENT_INTERRUPTIBLE(_Status, _pTask) \
do { \
		wait_event_interruptible(_pTask->kthread_q, \
								 _pTask->kthread_running || kthread_should_stop()); \
		_pTask->kthread_running = FALSE; \
		if (kthread_should_stop()) \
		{ \
			(_Status) = -1; \
			break; \
		} \
		else (_Status) = 0; \
} while(0)
#endif

#ifdef KTHREAD_SUPPORT
#define WAKE_UP(_pTask) \
	do{ \
		if ((_pTask)->kthread_task) \
		{ \
			(_pTask)->kthread_running = TRUE; \
			wake_up(&(_pTask)->kthread_q); \
		} \
	}while(0)
#endif

/***********************************************************************************
 *	OS Memory Access related data structure and definitions
 ***********************************************************************************/
#define MEM_ALLOC_FLAG      (GFP_ATOMIC) /*(GFP_DMA | GFP_ATOMIC) */

#define NdisMoveMemory(Destination, Source, Length) memmove(Destination, Source, Length)
#define NdisCopyMemory(Destination, Source, Length) memcpy(Destination, Source, Length)
#define NdisZeroMemory(Destination, Length)         memset(Destination, 0, Length)
#define NdisFillMemory(Destination, Length, Fill)   memset(Destination, Fill, Length)
#define NdisCmpMemory(Destination, Source, Length)  memcmp(Destination, Source, Length)
#define NdisEqualMemory(Source1, Source2, Length)   (!memcmp(Source1, Source2, Length))
#define RTMPEqualMemory(Source1, Source2, Length)	(!memcmp(Source1, Source2, Length))

#define MlmeAllocateMemory(_pAd, _ppVA)		os_alloc_mem(_pAd, _ppVA, MGMT_DMA_BUFFER_SIZE)
#define MlmeFreeMemory(_pAd, _pVA)			os_free_mem(_pAd, _pVA)

#define COPY_MAC_ADDR(Addr1, Addr2)             memcpy((Addr1), (Addr2), MAC_ADDR_LEN)


/***********************************************************************************
 *	OS task related data structure and definitions
 ***********************************************************************************/
#define RTMP_OS_MGMT_TASK_FLAGS	CLONE_VM

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
/*typedef	struct pid *	THREAD_PID; // no use */
#define	THREAD_PID_INIT_VALUE	NULL
/* TODO: Use this IOCTL carefully when linux kernel version larger than 2.6.27, because the PID only correct when the user space task do this ioctl itself. */
/*#define RTMP_GET_OS_PID(_x, _y)    _x = get_task_pid(current, PIDTYPE_PID); */
#define RT_GET_OS_PID(_x, _y)		do{rcu_read_lock(); _x=(ULONG)current->pids[PIDTYPE_PID].pid; rcu_read_unlock();}while(0)
#ifdef OS_ABL_FUNC_SUPPORT
#define RTMP_GET_OS_PID(_a, _b)			RtmpOsGetPid(&_a, _b)
#else
#define RTMP_GET_OS_PID(_a, _b)			RT_GET_OS_PID(_a, _b)
#endif /* OS_ABL_FUNC_SUPPORT */
#define	GET_PID_NUMBER(_v)	pid_nr((_v))
#define CHECK_PID_LEGALITY(_pid)	if (pid_nr((_pid)) > 0)
#define KILL_THREAD_PID(_A, _B, _C)	kill_pid((_A), (_B), (_C))
#else
/*typedef	pid_t	THREAD_PID; // no use */
#define	THREAD_PID_INIT_VALUE	-1
#define RT_GET_OS_PID(_x, _pid)		_x = _pid
#define RTMP_GET_OS_PID(_x, _pid)		_x = _pid
#define	GET_PID_NUMBER(_v)	(_v)
#define CHECK_PID_LEGALITY(_pid)	if ((_pid) >= 0)
#define KILL_THREAD_PID(_A, _B, _C)	kill_proc((_A), (_B), (_C))
#endif

#define ATE_KILL_THREAD_PID(PID)		KILL_THREAD_PID(PID, SIGTERM, 1)

typedef int (*cast_fn)(void *);
typedef INT (*RTMP_OS_TASK_CALLBACK)(ULONG);

#ifdef WORKQUEUE_BH
typedef struct work_struct OS_NET_TASK_STRUCT;
typedef struct work_struct *POS_NET_TASK_STRUCT;
#else
typedef struct tasklet_struct OS_NET_TASK_STRUCT;
typedef struct tasklet_struct  *POS_NET_TASK_STRUCT;
#endif /* WORKQUEUE_BH */

/***********************************************************************************
 * Timer related definitions and data structures.
 **********************************************************************************/
#define OS_HZ			HZ

typedef struct timer_list	OS_NDIS_MINIPORT_TIMER;
typedef struct timer_list	OS_TIMER;

typedef void (*TIMER_FUNCTION)(unsigned long);


#define OS_WAIT(_time) \
{	\
	if (in_interrupt()) \
	{\
		RtmpusecDelay(_time * 1000);\
	}else	\
	{\
		int _i; \
		long _loop = ((_time)/(1000/OS_HZ)) > 0 ? ((_time)/(1000/OS_HZ)) : 1;\
		wait_queue_head_t _wait; \
		init_waitqueue_head(&_wait); \
		for (_i=0; _i<(_loop); _i++) \
			wait_event_interruptible_timeout(_wait, 0, ONE_TICK); \
	}\
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
#define RTMP_TIME_AFTER(a,b)		\
	(typecheck(unsigned long, (unsigned long)a) && \
	 typecheck(unsigned long, (unsigned long)b) && \
	 ((long)(b) - (long)(a) < 0))

#define RTMP_TIME_AFTER_EQ(a,b)	\
	(typecheck(unsigned long, (unsigned long)a) && \
	 typecheck(unsigned long, (unsigned long)b) && \
	 ((long)(a) - (long)(b) >= 0))
#define RTMP_TIME_BEFORE(a,b)	RTMP_TIME_AFTER_EQ(b,a)
#else
#define typecheck(type,x) \
({      type __dummy; \
		typeof(x) __dummy2; \
		(void)(&__dummy == &__dummy2); \
		1; \
})
#define RTMP_TIME_AFTER_EQ(a,b)	\
	(typecheck(unsigned long, (unsigned long)a) && \
	 typecheck(unsigned long, (unsigned long)b) && \
	 ((long)(a) - (long)(b) >= 0))
#define RTMP_TIME_BEFORE(a,b)	RTMP_TIME_AFTER_EQ(b,a)
#define RTMP_TIME_AFTER(a,b) time_after(a, b)
#endif

#define ONE_TICK 1

static inline void NdisGetSystemUpTime(ULONG *time)
{
	*time = jiffies;
}


/***********************************************************************************
 *	OS specific cookie data structure binding to RTMP_ADAPTER
 ***********************************************************************************/

struct os_cookie {

#ifdef RTMP_MAC_USB
	struct usb_device *pUsb_Dev;
#ifdef CONFIG_STA_SUPPORT
#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	struct usb_interface *intf;
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */
#endif /* CONFIG_STA_SUPPORT */
#endif /* RTMP_MAC_USB */

#ifdef WORKQUEUE_BH
	UINT32		     pAd_va;
#endif /* WORKQUEUE_BH */

	RTMP_NET_TASK_STRUCT rx_done_task;
	RTMP_NET_TASK_STRUCT cmd_rsp_event_task;
	RTMP_NET_TASK_STRUCT mgmt_dma_done_task;
	RTMP_NET_TASK_STRUCT ac0_dma_done_task;
#ifdef RALINK_ATE
	RTMP_NET_TASK_STRUCT ate_ac0_dma_done_task;
#endif /* RALINK_ATE */
	RTMP_NET_TASK_STRUCT ac1_dma_done_task;
	RTMP_NET_TASK_STRUCT ac2_dma_done_task;
	RTMP_NET_TASK_STRUCT ac3_dma_done_task;
	RTMP_NET_TASK_STRUCT hcca_dma_done_task;
	RTMP_NET_TASK_STRUCT tbtt_task;


#ifdef UAPSD_SUPPORT
	RTMP_NET_TASK_STRUCT uapsd_eosp_sent_task;
#endif /* UAPSD_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
#ifdef DFS_SUPPORT
#ifdef DFS_SOFTWARE_SUPPORT
	RTMP_NET_TASK_STRUCT pulse_radar_detect_task;
	RTMP_NET_TASK_STRUCT width_radar_detect_task;
#endif /* DFS_SOFTWARE_SUPPORT */
#endif /* DFS_SUPPORT */


#ifdef DFS_SUPPORT
	struct tasklet_struct	dfs_task;
#endif /* DFS_SUPPORT */

#endif /* CONFIG_AP_SUPPORT */

#ifdef RTMP_MAC_USB
	RTMP_NET_TASK_STRUCT null_frame_complete_task;
	RTMP_NET_TASK_STRUCT pspoll_frame_complete_task;
#endif /* RTMP_MAC_USB */

	RTMP_OS_PID			apd_pid; /*802.1x daemon pid */
	unsigned long			apd_pid_nr;
#ifdef CONFIG_AP_SUPPORT
#ifdef IAPP_SUPPORT
/*	RT_SIGNAL_STRUC			RTSignal; */
	RTMP_OS_PID			IappPid; /*IAPP daemon pid */
	unsigned long			IappPid_nr;
#endif /* IAPP_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */
	INT						ioctl_if_type;
	INT 					ioctl_if;
};

typedef struct os_cookie	* POS_COOKIE;



/***********************************************************************************
 *	OS debugging and printing related definitions and data structure
 ***********************************************************************************/
#define PRINT_MAC(addr)	\
	addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]

#ifdef DBG

extern ULONG RTDebugLevel;
extern ULONG RTDebugFunc;

#define DBGPRINT_RAW(Level, Fmt)    \
do {                                   \
	ULONG __gLevel = (Level) & 0xff;\
	ULONG __fLevel = ((Level) & 0xffffff00);\
	if (__gLevel <= RTDebugLevel)      \
	{                               \
		if ((RTDebugFunc == 0) || \
		((RTDebugFunc != 0) && (((__fLevel & RTDebugFunc)!= 0) || (__gLevel <= RT_DEBUG_ERROR))))\
		printk Fmt;               \
	}                               \
}while(0)

#define DBGPRINT(Level, Fmt) DBGPRINT_RAW(Level, Fmt)

#if 0
#define DBGPRINT_ERR(Fmt)           \
{ 	\
	printk("ERROR mt7662u_sta:"); \
	printk KERN_ERR Fmt;          \
}
#else
#define DBGPRINT_ERR(Fmt)        \
{                                \
    printk("ERROR mt766u_sta:"); \
    printk Fmt;                  \
}
#endif


#else
#define DBGPRINT(Level, Fmt)
#define DBGPRINT_RAW(Level, Fmt)
#define DBGPRINT_ERR(Fmt)
#endif

#undef  ASSERT
#ifdef DBG
#define ASSERT(x)                                                               \
{                                                                               \
	if (!(x))                                                                   \
	{                                                                           \
		printk(KERN_WARNING __FILE__ ":%d assert " #x "failed\n", __LINE__);    \
	}                                                                           \
}
#else
#define ASSERT(x)
#endif /* DBG */

void hex_dump(char *str, unsigned char *pSrcBufVA, unsigned int SrcBufLen);


/*********************************************************************************************************
	The following code are not revised, temporary put it here.
  *********************************************************************************************************/


/***********************************************************************************
 * Device DMA Access related definitions and data structures.
 **********************************************************************************/
ra_dma_addr_t linux_pci_map_single(void *handle, void *ptr, size_t size, int sd_idx, int direction);
void linux_pci_unmap_single(void *handle, ra_dma_addr_t dma_addr, size_t size, int direction);

#define PCI_MAP_SINGLE_DEV(_handle, _ptr, _size, _sd_idx, _dir)				\
	linux_pci_map_single(_handle, _ptr, _size, _sd_idx, _dir)

#define PCI_UNMAP_SINGLE(_pAd, _ptr, _size, _dir)						\
	linux_pci_unmap_single(((POS_COOKIE)(_pAd->OS_Cookie))->pci_dev, _ptr, _size, _dir)

#define PCI_ALLOC_CONSISTENT(_pci_dev, _size, _ptr) \
	pci_alloc_consistent(_pci_dev, _size, _ptr)

#define PCI_FREE_CONSISTENT(_pci_dev, _size, _virtual_addr, _physical_addr) \
	pci_free_consistent(_pci_dev, _size, _virtual_addr, _physical_addr)

#ifdef VENDOR_FEATURE2_SUPPORT
#define DEV_ALLOC_SKB(_pAd, _Pkt, _length)	\
	_Pkt = dev_alloc_skb(_length);			\
	if (_Pkt != NULL) {MEM_DBG_PKT_ALLOC_INC(_Pkt);};
#else

#define DEV_ALLOC_SKB(_pAd, _Pkt, _length)	\
	_Pkt = dev_alloc_skb(_length);
#endif /* VENDOR_FEATURE2_SUPPORT */

/*#define PCI_MAP_SINGLE(_handle, _ptr, _size, _dir) (ULONG)0 */
/*#define PCI_UNMAP_SINGLE(_handle, _ptr, _size, _dir) */


/*
 * ULONG
 * RTMP_GetPhysicalAddressLow(
 *   IN NDIS_PHYSICAL_ADDRESS  PhysicalAddress);
 */
#define RTMP_GetPhysicalAddressLow(PhysicalAddress)		(PhysicalAddress)

/*
 * ULONG
 * RTMP_GetPhysicalAddressHigh(
 *   IN NDIS_PHYSICAL_ADDRESS  PhysicalAddress);
 */
#define RTMP_GetPhysicalAddressHigh(PhysicalAddress)		(0)

/*
 * VOID
 * RTMP_SetPhysicalAddressLow(
 *   IN NDIS_PHYSICAL_ADDRESS  PhysicalAddress,
 *   IN ULONG  Value);
 */
#define RTMP_SetPhysicalAddressLow(PhysicalAddress, Value)	\
			PhysicalAddress = Value;

/*
 * VOID
 * RTMP_SetPhysicalAddressHigh(
 *   IN NDIS_PHYSICAL_ADDRESS  PhysicalAddress,
 *   IN ULONG  Value);
 */
#define RTMP_SetPhysicalAddressHigh(PhysicalAddress, Value)

#define NdisMIndicateStatus(_w, _x, _y, _z)



/***********************************************************************************
 * Device Register I/O Access related definitions and data structures.
 **********************************************************************************/

#ifdef RTMP_MAC_USB
#define RTMP_IO_FORCE_READ32(_A, _R, _pV)								\
	RTUSBReadMACRegister((_A), (_R), (PUINT32) (_pV))

#define RTMP_IO_FORCE_WRITE32(_A, _R, _V)	\
	do{\
		/*if ((_R) != 0x404)*/ /* TODO:shiang-6590, depends on sw porting guide, don't acccess it now */\
			RTUSBWriteMACRegister((_A), (_R), (UINT32) (_V), FALSE);		\
	}while(0)

#define RTMP_IO_READ8(_A, _R, _pV)								\
{																\
}

#define RTMP_IO_READ32(_A, _R, _pV)								\
	RTUSBReadMACRegister((_A), (_R), (PUINT32) (_pV))

#define RTMP_IO_WRITE32(_A, _R, _V)								\
	RTUSBWriteMACRegister((_A), (_R), (UINT32) (_V), FALSE)

#define RTMP_IO_WRITE8(_A, _R, _V)								\
{																\
	USHORT	_Val = _V;											\
	RTUSBSingleWrite((_A), (_R), (USHORT) (_Val), FALSE);		\
}


#define RTMP_IO_WRITE16(_A, _R, _V)								\
{																\
	RTUSBSingleWrite((_A), (_R), (USHORT) (_V), FALSE);			\
}

#define RTMP_SYS_IO_READ32
#define RTMP_SYS_IO_WRITE32
#endif /* RTMP_MAC_USB */

#define RTMP_USB_URB_DATA_GET(__pUrb)			((purbb_t)__pUrb)->context
#define RTMP_USB_URB_STATUS_GET(__pUrb)			((purbb_t)__pUrb)->status
#define RTMP_USB_URB_LEN_GET(__pUrb)			((purbb_t)__pUrb)->actual_length

/***********************************************************************************
 *	Network Related data structure and marco definitions
 ***********************************************************************************/
#define PKTSRC_NDIS             0x7f
#define PKTSRC_DRIVER           0x0f

#define RTMP_OS_NETDEV_STATE_RUNNING(_pNetDev)	((_pNetDev)->flags & IFF_UP)

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)
#define _RTMP_OS_NETDEV_GET_PRIV(_pNetDev)		(NULL /*(_pNetDev)->ml_priv*/)
#else
#define _RTMP_OS_NETDEV_GET_PRIV(_pNetDev)		((_pNetDev)->priv)
#endif

#define RTMP_OS_NETDEV_GET_DEVNAME(_pNetDev)	((_pNetDev)->name)
#define RTMP_OS_NETDEV_GET_PHYADDR(_pNetDev)	((_pNetDev)->dev_addr)

/* Get & Set NETDEV interface hardware type */
#define RTMP_OS_NETDEV_GET_TYPE(_pNetDev)			((_pNetDev)->type)
#define RTMP_OS_NETDEV_SET_TYPE(_pNetDev, _type)	((_pNetDev)->type = (_type))
#define RTMP_OS_NETDEV_SET_TYPE_MONITOR(_pNetDev)	RTMP_OS_NETDEV_SET_TYPE(_pNetDev, ARPHRD_IEEE80211_PRISM)

#define RTMP_OS_NETDEV_START_QUEUE(_pNetDev)	netif_start_queue((_pNetDev))
#define RTMP_OS_NETDEV_STOP_QUEUE(_pNetDev)	netif_stop_queue((_pNetDev))
#define RTMP_OS_NETDEV_WAKE_QUEUE(_pNetDev)	netif_wake_queue((_pNetDev))
#define RTMP_OS_NETDEV_CARRIER_OFF(_pNetDev)	netif_carrier_off((_pNetDev))

#define QUEUE_ENTRY_TO_PACKET(pEntry) \
	(PNDIS_PACKET)(pEntry)

#define PACKET_TO_QUEUE_ENTRY(pPacket) \
	(PQUEUE_ENTRY)(pPacket)

#ifdef CONFIG_5VT_ENHANCE
#define BRIDGE_TAG 0x35564252    /* depends on 5VT define in br_input.c */
#endif

#define GET_SG_LIST_FROM_PACKET(_p, _sc)	\
	rt_get_sg_list_from_packet(_p, _sc)

#define RELEASE_NDIS_PACKET(_pAd, _pPacket, _Status)                    \
{                                                                       \
		RTMPFreeNdisPacket(_pAd, _pPacket);                             \
}


/*
 * packet helper
 * 	- convert internal rt packet to os packet or
 *             os packet to rt packet
 */
#define RTPKT_TO_OSPKT(_p)		((struct sk_buff *)(_p))
#define OSPKT_TO_RTPKT(_p)		((PNDIS_PACKET)(_p))

#define GET_OS_PKT_DATAPTR(_pkt) \
		(RTPKT_TO_OSPKT(_pkt)->data)
#define SET_OS_PKT_DATAPTR(_pkt, _dataPtr)	\
		(RTPKT_TO_OSPKT(_pkt)->data) = (_dataPtr)

#define GET_OS_PKT_LEN(_pkt) \
		(RTPKT_TO_OSPKT(_pkt)->len)
#define SET_OS_PKT_LEN(_pkt, _len)	\
		(RTPKT_TO_OSPKT(_pkt)->len) = (_len)

#define GET_OS_PKT_DATATAIL(_pkt) \
		((unsigned char*)skb_tail_pointer(RTPKT_TO_OSPKT(_pkt)))
//		(RTPKT_TO_OSPKT(_pkt)->tail)

#define SET_OS_PKT_DATATAIL(_pkt, _start, _len)	\
		(skb_set_tail_pointer((RTPKT_TO_OSPKT(_pkt)), (_len)))
//		((RTPKT_TO_OSPKT(_pkt))->tail) = (PUCHAR)((_start) + (_len))

#define GET_OS_PKT_HEAD(_pkt) \
		(RTPKT_TO_OSPKT(_pkt)->head)

#define GET_OS_PKT_END(_pkt) \
		((unsigned char*)skb_end_pointer(RTPKT_TO_OSPKT(_pkt)))
//		(RTPKT_TO_OSPKT(_pkt)->end)

#define GET_OS_PKT_NETDEV(_pkt) \
		(RTPKT_TO_OSPKT(_pkt)->dev)
#define SET_OS_PKT_NETDEV(_pkt, _pNetDev)	\
		(RTPKT_TO_OSPKT(_pkt)->dev) = (_pNetDev)

#define GET_OS_PKT_TYPE(_pkt) \
		(RTPKT_TO_OSPKT(_pkt))

#define GET_OS_PKT_NEXT(_pkt) \
		(RTPKT_TO_OSPKT(_pkt)->next)


#define OS_PKT_CLONED(_pkt)		skb_cloned(RTPKT_TO_OSPKT(_pkt))
#define OS_PKT_COPY(_pkt)		skb_copy(RTPKT_TO_OSPKT(_pkt), GFP_ATOMIC)

#define OS_PKT_TAIL_ADJUST(_pkt, _removedTagLen)								\
	SET_OS_PKT_DATATAIL(_pkt, GET_OS_PKT_DATATAIL(_pkt), (-_removedTagLen));	\
	GET_OS_PKT_LEN(_pkt) -= _removedTagLen;

#define OS_PKT_HEAD_BUF_EXTEND(_pkt, _offset)								\
	skb_push(RTPKT_TO_OSPKT(_pkt), _offset)

#define OS_PKT_TAIL_BUF_EXTEND(_pkt, _Len)									\
	skb_put(RTPKT_TO_OSPKT(_pkt), _Len)

#define OS_PKT_RESERVE(_pkt, _Len)											\
	skb_reserve(RTPKT_TO_OSPKT(_pkt), _Len)

#define OS_PKT_COPY_EXPAND(_pkt, headroom, tailroom)									\
	skb_copy_expand(RTPKT_TO_OSPKT(_pkt), headroom, tailroom, GFP_ATOMIC)

#define RTMP_OS_PKT_INIT(__pRxPacket, __pNetDev, __pData, __DataSize)		\
{																			\
	PNDIS_PACKET __pRxPkt;													\
	__pRxPkt = RTPKT_TO_OSPKT(__pRxPacket);									\
	SET_OS_PKT_NETDEV(__pRxPkt, __pNetDev);									\
	SET_OS_PKT_DATAPTR(__pRxPkt, __pData);									\
	SET_OS_PKT_LEN(__pRxPkt, __DataSize);									\
	SET_OS_PKT_DATATAIL(__pRxPkt, __pData, __DataSize);						\
}

#ifdef VENDOR_FEATURE2_SUPPORT
#define OS_PKT_CLONE(_pAd, _pkt, _src, _flag)		\
	_src = skb_clone(RTPKT_TO_OSPKT(_pkt), _flag);	\
	if (_src != NULL) OS_NumOfPktAlloc ++;
#else

#define OS_PKT_CLONE(_pAd, _pkt, _src, _flag)		\
	_src = skb_clone(RTPKT_TO_OSPKT(_pkt), _flag);
#endif /* VENDOR_FEATURE2_SUPPORT */

#define get_unaligned32							get_unaligned
#define get_unalignedlong						get_unaligned

#define OS_NTOHS(_Val) \
		(ntohs((_Val)))
#define OS_HTONS(_Val) \
		(htons((_Val)))
#define OS_NTOHL(_Val) \
		(ntohl((_Val)))
#define OS_HTONL(_Val) \
		(htonl((_Val)))

#define CB_OFF  10
#define GET_OS_PKT_CB(_p)		(RTPKT_TO_OSPKT(_p)->cb)
#define PACKET_CB(_p, _offset)	((RTPKT_TO_OSPKT(_p)->cb[CB_OFF + (_offset)]))

/***********************************************************************************
 *	Other function prototypes definitions
 ***********************************************************************************/

#ifdef CONFIG_RAETH
#if !defined(CONFIG_RA_NAT_NONE)
extern int (*ra_sw_nat_hook_tx)(VOID *skb);
extern int (*ra_sw_nat_hook_rx)(VOID *skb);
#endif
#endif /* CONFIG_RAETH */

#if defined (CONFIG_WIFI_PKT_FWD)
extern int (*wf_fwd_rx_hook) (struct sk_buff *skb);
extern unsigned char (*wf_fwd_entry_insert_hook) (struct net_device *src, struct net_device *dest);
extern unsigned char (*wf_fwd_entry_delete_hook) (struct net_device *src, struct net_device *dest);
#endif /* CONFIG_WIFI_PKT_FWD */

void RTMP_GetCurrentSystemTime(LARGE_INTEGER *time);
int rt28xx_packet_xmit(VOID *skb);


#if LINUX_VERSION_CODE <= 0x20402	/* Red Hat 7.1 */
struct net_device *alloc_netdev(int sizeof_priv, const char *mask, void (*setup)(struct net_device *));
#endif /* LINUX_VERSION_CODE */



INT rt28xx_ioctl(PNET_DEV net_dev, struct ifreq *rq, INT cmd);
int rt28xx_send_packets(struct sk_buff *skb, struct net_device *ndev);

extern int ra_mtd_write(int num, loff_t to, size_t len, const u_char *buf);
extern int ra_mtd_read(int num, loff_t from, size_t len, u_char *buf);


#define GET_PAD_FROM_NET_DEV(_pAd, _net_dev)	\
	_pAd = RTMP_OS_NETDEV_GET_PRIV(_net_dev);
#define GET_WDEV_FROM_NET_DEV(_wdev, _net_dev)	\
	_wdev = RTMP_OS_NETDEV_GET_WDEV(_net_dev);


/*#ifdef RTMP_USB_SUPPORT */
/******************************************************************************

  	USB related definitions

******************************************************************************/

#define RTMP_USB_PKT_COPY(__pNetDev, __pNetPkt, __Len, __pData)			\
{																		\
	memcpy(skb_put(__pNetPkt, __Len), __pData, __Len);					\
	GET_OS_PKT_NETDEV(__pNetPkt) = __pNetDev;							\
}

typedef struct usb_device_id USB_DEVICE_ID;

// TODO: shiang-usw, fine tune BULKAGGRE_SIZE, origianl is 60
#ifdef INF_AMAZON_SE
#define BULKAGGRE_SIZE				30
#else
#define BULKAGGRE_SIZE				100 /* 100 */
#endif /* INF_AMAZON_SE */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)

#ifndef OS_ABL_SUPPORT
#define RTUSB_ALLOC_URB(iso)		usb_alloc_urb(iso, GFP_ATOMIC)
#define RTUSB_SUBMIT_URB(pUrb)		usb_submit_urb(pUrb, GFP_ATOMIC)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35)
#define RTUSB_URB_ALLOC_BUFFER(_dev, _size, _dma)	usb_alloc_coherent(_dev, _size, GFP_ATOMIC, _dma)
#define RTUSB_URB_FREE_BUFFER(_dev, _size, _addr, _dma)	usb_free_coherent(_dev, _size, _addr, _dma)
#else
#define RTUSB_URB_ALLOC_BUFFER(_dev, _size, _dma)	usb_buffer_alloc(_dev, _size, GFP_ATOMIC, _dma)
#define RTUSB_URB_FREE_BUFFER(_dev, _size, _addr, _dma)	usb_buffer_free(_dev, _size, _addr, _dma)
#endif /* LINUX_VERSION_CODE */

#define RTUSB_FILL_BULK_URB(_urb, _dev, _pipe, _buffer, _buffer_len, _complete_fn, _context) usb_fill_bulk_urb(_urb, _dev, _pipe, _buffer, _buffer_len, _complete_fn, _context)
#else

#define RTUSB_ALLOC_URB(iso)		rausb_alloc_urb(iso)
#define RTUSB_SUBMIT_URB(pUrb)		rausb_submit_urb(pUrb)
#define RTUSB_URB_ALLOC_BUFFER		rausb_buffer_alloc
#define RTUSB_URB_FREE_BUFFER		rausb_buffer_free
#endif /* OS_ABL_SUPPORT */

#else

#define RT28XX_PUT_DEVICE(dev_p)

#ifndef OS_ABL_SUPPORT
#define RTUSB_ALLOC_URB(iso)		usb_alloc_urb(iso)
#define RTUSB_SUBMIT_URB(pUrb)		usb_submit_urb(pUrb)
#else
#define RTUSB_ALLOC_URB(iso)		rausb_alloc_urb(iso)
#define RTUSB_SUBMIT_URB(pUrb)		rausb_submit_urb(pUrb)
#endif /* OS_ABL_SUPPORT */

#define RTUSB_URB_ALLOC_BUFFER(pUsb_Dev, BufSize, pDma_addr)         		kmalloc(BufSize, GFP_ATOMIC)
#define RTUSB_URB_FREE_BUFFER(pUsb_Dev, BufSize, pTransferBuf, Dma_addr)  	kfree(pTransferBuf)
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0) */


#ifndef OS_ABL_SUPPORT
#define RTUSB_FREE_URB(pUrb)	usb_free_urb(pUrb)
#else
#define RTUSB_FREE_URB(pUrb)	rausb_free_urb(pUrb)
#endif /* OS_ABL_SUPPORT */

/* unlink urb */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,7)

#ifndef OS_ABL_SUPPORT
#define RTUSB_UNLINK_URB(pUrb)		usb_kill_urb(pUrb)
#else
#define RTUSB_UNLINK_URB(pUrb)		rausb_kill_urb(pUrb)
#endif /* OS_ABL_SUPPORT */

#else
#define RTUSB_UNLINK_URB(pUrb)		usb_unlink_urb(pUrb)
#endif /* LINUX_VERSION_CODE */

/* Prototypes of completion funuc. */
#define RtmpUsbBulkOutDataPacketComplete		RTUSBBulkOutDataPacketComplete
#define RtmpUsbBulkOutMLMEPacketComplete		RTUSBBulkOutMLMEPacketComplete
#define RtmpUsbBulkOutNullFrameComplete			RTUSBBulkOutNullFrameComplete
#define RtmpUsbBulkOutRTSFrameComplete			RTUSBBulkOutRTSFrameComplete
#define RtmpUsbBulkOutPsPollComplete			RTUSBBulkOutPsPollComplete
#define RtmpUsbBulkRxComplete					RTUSBBulkRxComplete
#define RtmpUsbBulkCmdRspEventComplete			RTUSBBulkCmdRspEventComplete

#if ((LINUX_VERSION_CODE < KERNEL_VERSION(2, 5, 51)) || (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 18)))
#define RTUSBBulkOutDataPacketComplete(Status, pURB, pt_regs)    RTUSBBulkOutDataPacketComplete(pURB)
#define RTUSBBulkOutMLMEPacketComplete(Status, pURB, pt_regs)    RTUSBBulkOutMLMEPacketComplete(pURB)
#define RTUSBBulkOutNullFrameComplete(Status, pURB, pt_regs)     RTUSBBulkOutNullFrameComplete(pURB)
#define RTUSBBulkOutRTSFrameComplete(Status, pURB, pt_regs)      RTUSBBulkOutRTSFrameComplete(pURB)
#define RTUSBBulkOutPsPollComplete(Status, pURB, pt_regs)        RTUSBBulkOutPsPollComplete(pURB)
#define RTUSBBulkRxComplete(Status, pURB, pt_regs)               RTUSBBulkRxComplete(pURB)
#define RTUSBBulkCmdRspEventComplete(Status, pURB, pt_regs)		 RTUSBBulkCmdRspEventComplete(pURB)
#else
#define RTUSBBulkOutDataPacketComplete(Status, pURB, pt_regs)    RTUSBBulkOutDataPacketComplete(pURB, pt_regs)
#define RTUSBBulkOutMLMEPacketComplete(Status, pURB, pt_regs)    RTUSBBulkOutMLMEPacketComplete(pURB, pt_regs)
#define RTUSBBulkOutNullFrameComplete(Status, pURB, pt_regs)     RTUSBBulkOutNullFrameComplete(pURB, pt_regs)
#define RTUSBBulkOutRTSFrameComplete(Status, pURB, pt_regs)      RTUSBBulkOutRTSFrameComplete(pURB, pt_regs)
#define RTUSBBulkOutPsPollComplete(Status, pURB, pt_regs)        RTUSBBulkOutPsPollComplete(pURB, pt_regs)
#define RTUSBBulkRxComplete(Status, pURB, pt_regs)               RTUSBBulkRxComplete(pURB, pt_regs)
#define RTUSBBulkCmdRspEventComplete(Status, pURB, pt_regs)		 RTUSBBulkCmdRspEventComplete(pURB, pt_regs)
#endif

/*extern void dump_urb(struct urb *purb); */

#define InterlockedIncrement 	 	atomic_inc
#define NdisInterlockedIncrement 	atomic_inc
#define InterlockedDecrement		atomic_dec
#define NdisInterlockedDecrement 	atomic_dec
#define InterlockedExchange			atomic_set

typedef void USBHST_STATUS;
typedef INT32 URBCompleteStatus;
typedef struct pt_regs pregs;

USBHST_STATUS RTUSBBulkOutDataPacketComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkOutMLMEPacketComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkOutNullFrameComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkOutRTSFrameComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkOutPsPollComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkRxComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkCmdRspEventComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);

/* Fill Bulk URB Macro */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
#define RTUSB_FILL_TX_BULK_URB(pUrb,	\
				   pUsb_Dev,	\
				   uEndpointAddress,		\
				   pTransferBuf,			\
				   BufSize,				\
				   Complete,	\
				   pContext,		\
					TransferDma)	\
  			   do{	\
				   		usb_fill_bulk_urb(pUrb, pUsb_Dev, usb_sndbulkpipe(pUsb_Dev, uEndpointAddress),	\
								pTransferBuf, BufSize, Complete, pContext);	\
						pUrb->transfer_dma	= TransferDma;	\
				   		pUrb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;	\
				}while(0)

#define RTUSB_FILL_HTTX_BULK_URB(pUrb,	\
				pUsb_Dev,	\
				uEndpointAddress,		\
				pTransferBuf,			\
				BufSize,				\
				Complete,	\
				pContext,				\
				TransferDma)				\
  				do{	\
					usb_fill_bulk_urb(pUrb, pUsb_Dev, usb_sndbulkpipe(pUsb_Dev, uEndpointAddress),	\
								pTransferBuf, BufSize, Complete, pContext);	\
					pUrb->transfer_dma	= TransferDma; \
					pUrb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;	\
				}while(0)

#define RTUSB_FILL_RX_BULK_URB(pUrb,	\
				pUsb_Dev,				\
				uEndpointAddress,		\
				pTransferBuf,			\
				BufSize,				\
				Complete,				\
				pContext,				\
				TransferDma)			\
  				do{	\
					usb_fill_bulk_urb(pUrb, pUsb_Dev, usb_rcvbulkpipe(pUsb_Dev, uEndpointAddress),	\
								pTransferBuf, BufSize, Complete, pContext);	\
					pUrb->transfer_dma	= TransferDma;	\
					pUrb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;	\
				}while(0)
/* pRxContext->data_dma + pAd->NextRxBulkInPosition; */

#define RTUSB_URB_DMA_MAPPING(pUrb)	\
	{	\
		pUrb->transfer_dma	= 0;	\
		pUrb->transfer_flags &= (~URB_NO_TRANSFER_DMA_MAP);	\
	}

#else
#define RTUSB_FILL_TX_BULK_URB(pUrb,	\
				   pUsb_Dev,	\
				   uEndpointAddress,		\
				   pTransferBuf,			\
				   BufSize,				\
				   Complete,	\
				   pContext,	\
					TransferDma)	\
  				   do{	\
  				   		FILL_BULK_URB(pUrb, pUsb_Dev, usb_sndbulkpipe(pUsb_Dev, uEndpointAddress),	\
							pTransferBuf, BufSize, Complete, pContext);	\
				   }while(0)

#define RTUSB_FILL_HTTX_BULK_URB(pUrb,	\
				pUsb_Dev,				\
				uEndpointAddress,		\
				pTransferBuf,			\
				BufSize,				\
				Complete,				\
				   pContext,	\
					TransferDma)	\
  				do{	\
					FILL_BULK_URB(pUrb, pUsb_Dev, usb_sndbulkpipe(pUsb_Dev, uEndpointAddress),	\
								pTransferBuf, BufSize, Complete, pContext);	\
				}while(0)

#define RTUSB_FILL_RX_BULK_URB(pUrb,	\
				pUsb_Dev,	\
				uEndpointAddress,		\
				pTransferBuf,			\
				BufSize,				\
				Complete,	\
				   pContext,	\
					TransferDma)	\
  				do{	\
					FILL_BULK_URB(pUrb, pUsb_Dev, usb_rcvbulkpipe(pUsb_Dev, uEndpointAddress),	\
								pTransferBuf, BufSize, Complete, pContext);	\
				}while(0)

#define	RTUSB_URB_DMA_MAPPING(pUrb)
#endif /* LINUX_VERSION_CODE */


#define RTUSB_CONTROL_MSG(pUsb_Dev, uEndpointAddress, Request, RequestType, Value,Index, tmpBuf, TransferBufferLength, timeout, ret)	\
  		do{	\
			if ((RequestType == DEVICE_VENDOR_REQUEST_OUT) || (RequestType == DEVICE_CLASS_REQUEST_OUT))	\
				ret = USB_CONTROL_MSG(pUsb_Dev, usb_sndctrlpipe(pUsb_Dev, uEndpointAddress), Request, RequestType, Value, Index, tmpBuf, TransferBufferLength, timeout);	\
			else if (RequestType == DEVICE_VENDOR_REQUEST_IN)	\
				ret = USB_CONTROL_MSG(pUsb_Dev, usb_rcvctrlpipe(pUsb_Dev, uEndpointAddress), Request, RequestType, Value, Index, tmpBuf, TransferBufferLength, timeout);	\
			else	\
			{	\
				DBGPRINT(RT_DEBUG_ERROR, ("vendor request direction is failed\n"));	\
				ret = -1;	\
			}	\
		}while(0)

#define rtusb_urb_context  context
#define rtusb_urb_status   status

#define RTMP_OS_USB_CONTEXT_GET(__pURB)		__pURB->rtusb_urb_context
#define RTMP_OS_USB_STATUS_GET(__pURB)		__pURB->rtusb_urb_status

#ifndef OS_ABL_SUPPORT
#define USB_CONTROL_MSG		usb_control_msg

#else

#define USB_CONTROL_MSG		rausb_control_msg

/*extern int rausb_register(struct usb_driver * new_driver); */
/*extern void rausb_deregister(struct usb_driver * driver); */

extern struct urb *rausb_alloc_urb(int iso_packets);
extern void rausb_free_urb(VOID *urb);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
extern void rausb_put_dev(VOID *dev);
extern struct usb_device *rausb_get_dev(VOID *dev);
#endif /* LINUX_VERSION_CODE */

extern int rausb_submit_urb(VOID *urb);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
#ifndef gfp_t
#define gfp_t		INT32
#endif /* gfp_t */

extern void *rausb_buffer_alloc(VOID *dev,
								size_t size,
								ra_dma_addr_t *dma);
extern void rausb_buffer_free(VOID *dev,
								size_t size,
								void *addr,
								ra_dma_addr_t dma);
#endif /* LINUX_VERSION_CODE */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,7)
extern void rausb_kill_urb(VOID *urb);
#endif /* LINUX_VERSION_CODE */

extern int rausb_control_msg(VOID *dev,
							unsigned int pipe,
							__u8 request,
							__u8 requesttype,
							__u16 value,
							__u16 index,
							void *data,
							__u16 size,
							int timeout);

#endif /* OS_ABL_SUPPORT */

/*#endif // RTMP_USB_SUPPORT */

#ifdef RALINK_ATE
/******************************************************************************

  	ATE related definitions

******************************************************************************/
#define ate_print printk
#define ATEDBGPRINT DBGPRINT

#ifdef RTMP_MAC_USB
#ifdef CONFIG_AP_SUPPORT
#define EEPROM_BIN_FILE_NAME  "/etc/Wireless/RT2870AP/e2p.bin"
#endif /* CONFIG_AP_SUPPORT */
#ifdef CONFIG_STA_SUPPORT
#undef EEPROM_BIN_FILE_NAME /* Avoid APSTA mode re-define issue */
#define EEPROM_BIN_FILE_NAME  "/etc/Wireless/RT2870STA/e2p.bin"
#endif /* CONFIG_STA_SUPPORT */
#endif /* RTMP_MAC_USB */

#ifdef RTMP_USB_SUPPORT

#if ((LINUX_VERSION_CODE < KERNEL_VERSION(2, 5, 51)) || (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 18)))
/* Prototypes of completion funuc. */
#define ATE_RTUSBBulkOutDataPacketComplete(Status, pURB, pt_regs)    ATE_RTUSBBulkOutDataPacketComplete(pURB)
#else
#define ATE_RTUSBBulkOutDataPacketComplete(Status, pURB, pt_regs)    ATE_RTUSBBulkOutDataPacketComplete(pURB, pt_regs)
#endif /* LINUX_VERSION_CODE */

USBHST_STATUS ATE_RTUSBBulkOutDataPacketComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);

#endif /* RTMP_USB_SUPPORT */

#endif /* RALINK_ATE */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
INT RtmpOSNetDevOpsAlloc(
	IN PVOID *pNetDevOps);
#endif

#define RTMP_OS_MAX_SCAN_DATA_GET()		IW_SCAN_MAX_DATA

#include "os/rt_os.h"

#ifdef MULTI_INF_SUPPORT
#ifdef RTMP_USB_SUPPORT
INT __init rtusb_init(void);
VOID __exit rtusb_exit(void);
#endif /* RTMP_USB_SUPPORT */
#endif /* MULTI_INF_SUPPORT */

#endif /* __RT_LINUX_H__ */

