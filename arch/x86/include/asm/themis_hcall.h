/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Themis capability hypervisor — guest-side hypercall wrappers.
 *
 * Themis hypercalls use the System V argument convention:
 *   RAX = opcode, RDI/RSI/RDX/RCX/R8 = args, RAX = return.
 *
 * Only opcodes that a confidential guest needs to issue itself live here.
 * The full ABI is defined in themis-abi (themis/crates/themis-abi/src/lib.rs).
 */
#ifndef _ASM_X86_THEMIS_HCALL_H
#define _ASM_X86_THEMIS_HCALL_H

#include <linux/types.h>

/* Opcode mirror of themis_abi::opcodes::THEMIS_RING_DOORBELL. */
#define THEMIS_OP_RING_DOORBELL		0x23ULL

/**
 * themis_ring_doorbell - signal a doorbell registered by the parent domain.
 * @gpa:   doorbell guest-physical address (matches the GPA the parent
 *         registered via REGISTER_DOORBELL — e.g. the virtio notify region).
 * @value: doorbell payload (e.g. virtio queue index).
 *
 * Returns 0 on success, or a themis_abi::errors error code on failure
 * (for example -ENOENT-equivalent if no doorbell matches @gpa).
 *
 * Note: on success the capavisor context-switches to the parent before
 * returning; from the guest's perspective the VMCALL just appears to take
 * longer than a normal instruction.
 */
static __always_inline long themis_ring_doorbell(u64 gpa, u64 value)
{
	long ret;

	asm volatile("vmcall"
		     : "=a"(ret)
		     : "a"(THEMIS_OP_RING_DOORBELL),
		       "D"(gpa),
		       "S"(value)
		     : "memory");
	return ret;
}

#endif /* _ASM_X86_THEMIS_HCALL_H */
