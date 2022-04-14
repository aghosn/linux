// Tyche Monitor

#ifndef TYCHE
#define TYCHE

#include <linux/kernel.h>
#include <linux/kvm_host.h>

// ————————————————————————————————— Types —————————————————————————————————— //

typedef void (*tyche_hypercall_hook)(struct kvm_vcpu *vcpu);

// ———————————————————————————— Shared Variables ———————————————————————————— //

extern int tyche_hypercall_count;

// ——————————————————————————————— Prototypes ——————————————————————————————— //

/** Initializes tyche. */
void tyche_init(void);

/** Tyche hypercall handler. */
void tyche_handle_hypercall(struct kvm_vcpu *vcpu);

/** Registers hooks to be called on hypercalls. */
void tyche_register_hypercall_hook(tyche_hypercall_hook hook);

#endif
