// SPDX-License-Identifier: GPL-2.0
/*
 * Themis capability hypervisor — paravirtualization detection.
 *
 * Probes CPUID leaf 0x40000000 for the "ThemisCapa" signature and, if
 * present, captures the feature bitmap from leaf 0x40000001.  Both
 * leaves are advertised on every Themis domain regardless of the
 * confidential-computing mode; CoCo-specific bits live separately in
 * arch/x86/coco/core.c.
 *
 * Callers query the result via themis_on_themis() and themis_has_feature().
 */

#include <linux/cache.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/types.h>

#include <asm/cpuid/api.h>
#include <asm/themis_platform.h>

static bool themis_present __ro_after_init;
static u32  themis_features __ro_after_init;

bool themis_on_themis(void)
{
	return themis_present;
}
EXPORT_SYMBOL_GPL(themis_on_themis);

u32 themis_feature_bits(void)
{
	return themis_features;
}
EXPORT_SYMBOL_GPL(themis_feature_bits);

void __init themis_platform_init(void)
{
	u32 eax, ebx, ecx, edx;

	cpuid(THEMIS_CPUID_LEAF_BASE, &eax, &ebx, &ecx, &edx);
	if (ebx != THEMIS_SIG_BASE_EBX ||
	    ecx != THEMIS_SIG_BASE_ECX ||
	    edx != THEMIS_SIG_BASE_EDX)
		return;

	themis_present = true;

	cpuid(THEMIS_CPUID_LEAF_FEATURES, &eax, &ebx, &ecx, &edx);
	themis_features = eax;

	pr_info("Themis: capability hypervisor detected, features=%#x\n",
		themis_features);
}
