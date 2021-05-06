//
// Created by Wenxin Zheng on 2021/4/21.
//

#ifndef ACMOS_SPR21_ANSWER_LOCKS_H
#define ACMOS_SPR21_ANSWER_LOCKS_H


int lock_init(struct lock *lock){
    /* Your code here */
    if(nlock >= MAXLOCKS) BUG("Max lock count reached.");
    lock->locked = 0;
    lock->cpuid = -1;
    locks[nlock++] = lock;
    return 0;
}

void acquire(struct lock *lock){
    /* Your code here */
    if(holding_lock(lock) == 0) BUG("Already hold this lock.");
    while(__sync_lock_test_and_set(&(lock->locked), 1) == 1);
    sync_synchronize();
    __sync_lock_test_and_set(&(lock->cpuid), cpuid());
}

// Try to acquire the lock once
// Return 0 if succeed, -1 if failed.
int try_acquire(struct lock *lock){
    /* Your code here */
    if(holding_lock(lock) == 0) BUG("Already hold this lock.");
    if(__sync_lock_test_and_set(&(lock->locked), 1) == 0) {
        sync_synchronize();
        __sync_lock_test_and_set(&(lock->cpuid), cpuid());
        return 0;
    }
    return -1;
}

void release(struct lock* lock){
    /* Your code here */
    if(lock->cpuid == cpuid()) {
        lock->cpuid = -1;
        sync_synchronize();
        __sync_lock_release(&(lock->locked));
    }
}

int is_locked(struct lock* lock){
    return lock->locked;
}

// private for spin lock
int holding_lock(struct lock* lock){
    /* Your code here */
    if(cpuid() == lock->cpuid)return 0;
    return -1;
}

#endif  // ACMOS_SPR21_ANSWER_LOCKS_H
