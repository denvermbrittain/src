/*	$OpenBSD: pmap.h,v 1.1 2004/07/25 11:06:42 miod Exp $	*/
/*
 * Mach Operating System
 * Copyright (c) 1991 Carnegie Mellon University
 * Copyright (c) 1991 OMRON Corporation
 * All Rights Reserved.
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 *
 */
#ifndef _M88K_PMAP_H_
#define _M88K_PMAP_H_

#include <machine/mmu.h>
#include <machine/pcb.h>

/*
 * PMAP structure
 */

/* #define PMAP_USE_BATC */
struct pmap {
	sdt_entry_t		*pm_stab;	/* virtual pointer to sdt */
	u_int32_t		pm_apr;
	int			pm_count;	/* reference count */
	/* cpus using of this pmap; NCPU must be <= 32 */
	u_int32_t		pm_cpus;
	struct simplelock	pm_lock;
	struct pmap_statistics	pm_stats;	/* pmap statistics */
#ifdef	PMAP_USE_BATC
	u_int32_t		pm_ibatc[BATC_MAX];	/* instruction BATCs */
	u_int32_t		pm_dbatc[BATC_MAX];	/* data BATCs */
#endif
};

#define PMAP_NULL ((pmap_t) 0)

/* 	The PV (Physical to virtual) List.
 *
 * For each vm_page_t, pmap keeps a list of all currently valid virtual
 * mappings of that page. An entry is a pv_entry_t; the list is the
 * pv_head_table. This is used by things like pmap_remove, when we must
 * find and remove all mappings for a particular physical page.
 */
/* XXX - struct pv_entry moved to vmparam.h because of include ordering issues */

typedef struct pmap *pmap_t;
typedef struct pv_entry *pv_entry_t;

#ifdef	_KERNEL

extern	pmap_t		kernel_pmap;
extern	struct pmap	kernel_pmap_store;
extern	caddr_t		vmmap;

#define	pmap_kernel()			(&kernel_pmap_store)
#define pmap_resident_count(pmap)	((pmap)->pm_stats.resident_count)
#define	pmap_wired_count(pmap)		((pmap)->pm_stats.wired_count)
#define pmap_phys_address(frame)        ((paddr_t)(ptoa(frame)))

#define pmap_copy(dp,sp,d,l,s)		do { /* nothing */ } while (0)
#define pmap_update(pmap)		do { /* nothing (yet) */ } while (0)

#define	pmap_clear_modify(pg)		pmap_unsetbit(pg, PG_M)
#define	pmap_clear_reference(pg)	pmap_unsetbit(pg, PG_U)

#define pmap_proc_iflush(p,va,len)	do { /* nothing */ } while (0)

void pmap_bootstrap(vaddr_t);
void pmap_cache_ctrl(pmap_t, vaddr_t, vaddr_t, u_int);
boolean_t pmap_unsetbit(struct vm_page *, int);

#endif	/* _KERNEL */

#endif /* _M88K_PMAP_H_ */
