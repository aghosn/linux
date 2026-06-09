/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Themis capability hypervisor — guest-side platform detection.
 *
 * Mirror of themis_abi::cpuid::{LEAF_BASE, LEAF_FEATURES, feature_bits::*}.
 * The capavisor always advertises LEAF_BASE and LEAF_FEATURES on every
 * Themis domain (independent of the confidential-computing mode handled
 * by arch/x86/coco/core.c::themis_coco_init()).
 */
#ifndef _ASM_X86_THEMIS_PLATFORM_H
#define _ASM_X86_THEMIS_PLATFORM_H

#include <linux/types.h>

/* CPUID leaves. */
#define THEMIS_CPUID_LEAF_BASE		0x40000000U
#define THEMIS_CPUID_LEAF_FEATURES	0x40000001U

/*
 * LEAF_BASE signature in EBX:ECX:EDX = ASCII "ThemisCapa\0\0",
 * little-endian.  Defined here so both themis_platform.c and any
 * future early-boot probe can compare against the same values.
 */
#define THEMIS_SIG_BASE_EBX		0x6d656854U /* "Them" */
#define THEMIS_SIG_BASE_ECX		0x61436973U /* "isCa" */
#define THEMIS_SIG_BASE_EDX		0x20206170U /* "pa  " */

/* LEAF_FEATURES bits (EAX), kept in sync with themis-abi feature_bits::*. */
#define THEMIS_FEATURE_SYNC_SWITCH		(1U << 0)
#define THEMIS_FEATURE_DOORBELL_HYPERCALL	(1U << 1)

#ifdef CONFIG_THEMIS_GUEST
void themis_platform_init(void);
bool themis_on_themis(void);
u32  themis_feature_bits(void);

static inline bool themis_has_feature(u32 feature)
{
	return (themis_feature_bits() & feature) == feature;
}
#else
static inline void themis_platform_init(void) { }
static inline bool themis_on_themis(void) { return false; }
static inline u32  themis_feature_bits(void) { return 0; }
static inline bool themis_has_feature(u32 feature) { return false; }
#endif

#endif /* _ASM_X86_THEMIS_PLATFORM_H */
