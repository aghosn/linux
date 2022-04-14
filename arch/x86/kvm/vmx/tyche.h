// Tyche Monitor

#ifndef TYCHE_VMX
#define TYCHE_VMX

#include <linux/kvm_host.h>
#include <linux/kernel.h>
#include "vmx_ops.h"
#include "vmx.h"
#include "../tyche.h"

// —————————————————————————————— Definitions ——————————————————————————————— //

// IDT indexes for all supported interrupts
#define TYCHE_IDT_DIVIDE_ERROR  (1 << 0)  // #DE
#define TYCHE_IDT_DEBUG         (1 << 1)  // #DB
#define TYCHE_IDT_NON_MASKABLE  (1 << 2)  // ?
#define TYCHE_IDT_BREAKPOINT    (1 << 3)  // #BP
#define TYCHE_IDT_OVERFLOW      (1 << 4)  // #OF
#define TYCHE_IDT_BOUND_RANGE   (1 << 5)  // #BR
#define TYCHE_IDT_INVALID_OP    (1 << 6)  // #UD
#define TYCHE_IDT_NOT_AVAILABLE (1 << 7)  // #NM
#define TYCHE_IDT_DOUBLE_FAULT  (1 << 8)  // #DF
// Deprecated                   (1 << 9)
#define TYCHE_IDT_INVALID_TSS   (1 << 10) // #TS
#define TYCHE_IDT_NOT_PRESENT   (1 << 11) // #NP
#define TYCHE_IDT_STACK_SEGMENT (1 << 12) // #SS
#define TYCHE_IDT_GENERAL_PROT  (1 << 13) // #GP
#define TYCHE_IDT_PAGE_FAULT    (1 << 14) // #PF
#define TYCHE_IDT_X86_FLOAT     (1 << 16) // #MF
#define TYCHE_IDT_ALIGN_CHECK   (1 << 17) // #AC
#define TYCHE_IDT_MACHINE_CHECK (1 << 18) // #MC
#define TYCHE_IDT_SIMD_FLOAT    (1 << 19) // #XF
#define TYCHE_IDT_VIRT          (1 << 20) // ?
// Reserved                     (1 << 21-28)
#define TYCHE_IDT_VMM_COMM      (1 << 29) // #VC
#define TYCHE_IDT_SECURITY      (1 << 30) // #SX
// Reserved                     (1 << 31)
// User-defined interrupts      (1 << 32-256)

// ———————————————————————————— Shared Variables ———————————————————————————— //

/* extern int tyche_hypercall_count; */

// ——————————————————————————————— Prototypes ——————————————————————————————— //

/** Configure the exception bitmap to trap on most (all?) interrupts */
void tyche_configure_exception_bitmap(u32* eb);

/** Initializes Tyche VMX support. */
void tyche_vmx_init(void);

#endif
