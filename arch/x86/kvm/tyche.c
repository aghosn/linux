#include "tyche.h"

// ———————————————————————————— Global Variables ———————————————————————————— //

int tyche_hypercall_count = 0;
EXPORT_SYMBOL(tyche_hypercall_count);

// ——————————————————————————————— Tyche API ———————————————————————————————— //

tyche_hypercall_hook hypercall_hook = NULL;

void tyche_handle_hypercall(struct kvm_vcpu *vcpu) {
    // Keep track of the number of hypercalls
    tyche_hypercall_count++;
    
    if (hypercall_hook != NULL) {
        (*hypercall_hook)(vcpu);
    }
}

void tyche_register_hypercall_hook(tyche_hypercall_hook hook) {
    hypercall_hook = hook;
}
EXPORT_SYMBOL(tyche_register_hypercall_hook);

void tyche_init(void) {
    tyche_hypercall_count = 0;
}
EXPORT_SYMBOL(tyche_init);

