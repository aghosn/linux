// Tyche Monitor

#include "tyche.h"

#define EXCEPTION_BITMAP 0x00004004

// ——————————————————————————————— Tyche API ———————————————————————————————— //

void __attribute__((optimize("O0"))) tyche_configure_exception_bitmap(u32 *eb)
{
	// setupt VMX to trap on all supported interrupts
	*eb = *eb | TYCHE_IDT_DIVIDE_ERROR | TYCHE_IDT_DEBUG |
	      TYCHE_IDT_NON_MASKABLE | TYCHE_IDT_BREAKPOINT |
	      TYCHE_IDT_OVERFLOW | TYCHE_IDT_BOUND_RANGE |
	      TYCHE_IDT_INVALID_OP | TYCHE_IDT_NOT_AVAILABLE |
	      TYCHE_IDT_DOUBLE_FAULT | TYCHE_IDT_INVALID_TSS |
	      TYCHE_IDT_NOT_PRESENT | TYCHE_IDT_STACK_SEGMENT |
	      TYCHE_IDT_GENERAL_PROT | TYCHE_IDT_PAGE_FAULT |
	      TYCHE_IDT_X86_FLOAT | TYCHE_IDT_ALIGN_CHECK |
	      TYCHE_IDT_MACHINE_CHECK | TYCHE_IDT_SIMD_FLOAT | TYCHE_IDT_VIRT |
	      TYCHE_IDT_VMM_COMM | TYCHE_IDT_SECURITY;
}

void tyche_vmx_inspect_vmcs(struct kvm_vcpu *vcpu)
{
	// Volatile makes things easier to debug (we can't set O0 here for obscure reasons)
	volatile int exception_bitmap;
	volatile int pin_based_exec_ctrl;
	volatile int cpu_based_exec_ctrl;
	volatile int cpu_based_2nd_exec_ctrl;
	char ept_enabled = 'n';
	char secondary_cpu_ctrls_enabled = 'n';
	struct vcpu_vmx *vmx;
	vmx = to_vmx(vcpu);

	pin_based_exec_ctrl = vmx_pin_based_exec_ctrl(
		vmx); // This one is not actually reading the register
	cpu_based_exec_ctrl = exec_controls_get(vmx);
	cpu_based_2nd_exec_ctrl = secondary_exec_controls_get(vmx);
	exception_bitmap = vmcs_read32(EXCEPTION_BITMAP);

	if (cpu_based_exec_ctrl & (1 << 31)) {
		// Secondary controls enabled
		secondary_cpu_ctrls_enabled = 'y';

		if (cpu_based_2nd_exec_ctrl & (1 << 1)) {
			ept_enabled = 'y';
		}
	}

	printk(KERN_NOTICE "[TYCHE] VMCS pin-based exec ctrl: 0x%08x\n",
	       pin_based_exec_ctrl);
	printk(KERN_NOTICE "[TYCHE] VMCS cpu-based exec ctrl: 0x%08x\n",
	       cpu_based_exec_ctrl);
	printk(KERN_NOTICE "[TYCHE] VMCS 2nd cpu-b exec ctrl: 0x%08x\n",
	       cpu_based_2nd_exec_ctrl);
	printk(KERN_NOTICE "[TYCHE] VMCS exception bitmap:    0x%08x\n",
	       exception_bitmap);
	printk(KERN_NOTICE "[TYCHE] 2nd cpu ctrl: %c\n",
	       secondary_cpu_ctrls_enabled);
	printk(KERN_NOTICE "[TYCHE] ept:          %c\n", ept_enabled);
}

void tyche_setup_vmcs(void)
{
	u32 exception_bitmap;

	exception_bitmap = vmcs_read32(EXCEPTION_BITMAP);
	tyche_configure_exception_bitmap(&exception_bitmap);
	vmcs_write32(EXCEPTION_BITMAP, exception_bitmap);
}

void tyche_vmx_hypercall_init(struct kvm_vcpu *vcpu)
{
	tyche_vmx_inspect_vmcs(vcpu);
    tyche_setup_vmcs();
	tyche_vmx_inspect_vmcs(vcpu);
}

void tyche_vmx_init(void)
{
	tyche_init();
	tyche_register_hypercall_hook(&tyche_vmx_hypercall_init);
}
