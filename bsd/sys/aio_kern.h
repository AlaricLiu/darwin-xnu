/*
 * Copyright (c) 2006 Apple Computer, Inc. All Rights Reserved.
 * 
 * @APPLE_LICENSE_OSREFERENCE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code 
 * as defined in and that are subject to the Apple Public Source License 
 * Version 2.0 (the 'License'). You may not use this file except in 
 * compliance with the License.  The rights granted to you under the 
 * License may not be used to create, or enable the creation or 
 * redistribution of, unlawful or unlicensed copies of an Apple operating 
 * system, or to circumvent, violate, or enable the circumvention or 
 * violation of, any terms of an Apple operating system software license 
 * agreement.
 *
 * Please obtain a copy of the License at 
 * http://www.opensource.apple.com/apsl/ and read it before using this 
 * file.
 *
 * The Original Code and all software distributed under the License are 
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
 * Please see the License for the specific language governing rights and 
 * limitations under the License.
 *
 * @APPLE_LICENSE_OSREFERENCE_HEADER_END@
 */
/* 
 *	File:	sys/aio_kern.h
 *	Author:	Jerry Cottingham [jerryc@apple.com]
 *
 *	Header file for kernel only portion of POSIX Asynchronous IO APIs
 *
 */ 

#include <sys/aio.h>

#ifndef _SYS_AIO_KERN_H_
#define	_SYS_AIO_KERN_H_

#ifdef KERNEL

struct aio_workq_entry
{
	TAILQ_ENTRY( aio_workq_entry )	aio_workq_link;
	struct proc						*procp;		/* user proc that queued this request */
	user_addr_t						uaiocbp;  	/* pointer passed in from user land */
	user_addr_t						fsyncp;		/* not NULL means this request must complete */
												/* before an aio_fsync call can proceed. */
	vm_map_t						aio_map;	/* user land map we have a reference to */
	user_ssize_t					returnval;	/* return value from read / write request */	
	int								errorval;	/* error value from read / write request */
	int								flags;		
	long							group_tag;	/* identifier used to group IO requests */
	struct user_aiocb				aiocb;		/* copy of aiocb from user land */
};
typedef struct aio_workq_entry aio_workq_entry;

/*
 * definitions for aio_workq_entry.flags
 */
#define	AIO_READ		0x00000001	
#define	AIO_WRITE		0x00000002
#define	AIO_FSYNC		0x00000004	/* aio_fsync with op = O_SYNC */
#define	AIO_DSYNC		0x00000008	/* aio_fsync with op = O_DSYNC (not supported yet) */
#define	AIO_LIO			0x00000010	/* lio_listio generated IO */
#define	AIO_DO_FREE		0x00000800	/* entry needs to be freed */
#define	AIO_COMPLETION	0x00001000	/* entry is in completion processing (not freeable yet) */
#define	AIO_DISABLE		0x00002000	/* process is trying to exit or exec and we need */
									/* 	to disable normal completion notification */
#define	AIO_WAITING		0x00004000	/* process is trying to exit, exec, or close and is */
									/* 	waiting for one or more active IO requests to */
									/*	complete */

/*
 * Prototypes
 */

__private_extern__ void		
_aio_close(struct proc *p, int fd);

__private_extern__ void		
_aio_exit(struct proc *p);

__private_extern__ void		
_aio_exec(struct proc *p);

__private_extern__ void		
_aio_create_worker_threads(int num);

__private_extern__ void		
aio_init(void);

task_t						
get_aiotask(void);

#endif /* KERNEL */

#endif /* _SYS_AIO_KERN_H_ */