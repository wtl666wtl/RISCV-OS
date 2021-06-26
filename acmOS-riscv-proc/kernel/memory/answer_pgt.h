void enable_paging() {
    // TODO: Homework 2: Enable paging
    // After initializing the page table, write to register SATP register for kernel registers.
    // Flush the TLB to invalidate the existing TLB Entries
    // Suggested: 2 LoCs
    w_satp((uint64)kernel_pagetable);
    flush_tlb();
}

// Return the address of the PTE in page table *pagetable*
// The Risc-v Sv48 scheme has four levels of page table.
// For VA:
//   47...63 zero
//   39...47 -- 9  bits of level-3 index
//   30...38 -- 9  bits of level-2 index
//   21...29 -- 9  bits of level-1 index
//   12...20 -- 9  bits of level-0 index
//   0...11  -- 12 bits of byte offset within the page
// Return the last-level page table entry.
static pte_t* pt_query(pagetable_t pagetable, vaddr_t va, int alloc) {
    if (va >= MAXVA) BUG_FMT("get va[0x%lx] >= MAXVA[0x%lx]", va, MAXVA);
    // Suggested: 18 LoCs
    uint64 *p;
    uint64 *pgtab = pagetable;

    for(int i = 3; i > 0; i--) {
        p = &pgtab[PX(i, va)];
        if (*p & PTE_V) {
            pgtab = (uint64 *) ((char *) ((uint64) (*p) & ~0xFFF));
        } else {
            if(!alloc || (pgtab = (uint64 *)mm_kalloc()) == NULL)return NULL;
            memset(pgtab, 0, PGSIZE);
            *p = ((uint64)pgtab) | alloc;
        }
    }
    return &pgtab[PX(0, va)];
}
int pt_map_pages(pagetable_t pagetable, vaddr_t va, paddr_t pa, uint64 size, int perm){
    // Suggested: 11 LoCs
    char *a, *last;
    pte_t *pte;
    a = (char *)PGROUNDDOWN((uint64)va);
    last = (char *)PGROUNDDOWN((uint64)va + size - 1);
    //printk("va:%u pa:%u\n",a, pa);
    for(;;) {
        if((pte = pt_query(pagetable, (uint64)a, perm | PTE_V)) == NULL)return -1;
        //printk("va:%u pa:%u\n",a, pa);
        *pte = PA2PTE(pa) | perm | PTE_V;//May OK?
        if(a == last)break;
        a += PGSIZE;
        pa += PGSIZE;
    }
    return 0; // Do not modify
}

paddr_t pt_query_address(pagetable_t pagetable, vaddr_t va){
    // Suggested: 3 LoCs
    //printk("%u %u\n",pagetable, va);
    pte_t *pte = pt_query(pagetable, va, 0);
    if(pte == NULL || (*pte & PTE_V) == 0)return NULL;
    paddr_t pa = (uint64)PTE2PA(*pte) + (va & 0xFFF);
    //printk("%u %u\n",pte, pa);
    return pa;
}

int pt_unmap_addrs(pagetable_t pagetable, vaddr_t va){
    // Suggested: 2 LoCs
    pte_t *pte = pt_query(pagetable, va, 0);
    //printk("%u %d\n",(*pte),PTE_V);
    if(pte == NULL)BUG("Unmap Error!");
    else *pte = (*pte) & (~PTE_V);
    //printk("%u\n",(*pte) & PTE_V);
    return 0; // Do not modify
}

int pt_map_addrs(pagetable_t pagetable, vaddr_t va, paddr_t pa, int perm){
    // Suggested: 2 LoCs
    pt_map_pages(pagetable, va, pa, 1, perm);
    return 0; // Do not modify
}