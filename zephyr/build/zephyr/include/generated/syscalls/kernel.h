
/* auto-generated by gen_syscalls.py, don't edit */
#ifndef Z_INCLUDE_SYSCALLS_KERNEL_H
#define Z_INCLUDE_SYSCALLS_KERNEL_H


#ifndef _ASMLANGUAGE

#include <syscall_list.h>
#include <syscall.h>

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void z_impl_k_object_access_grant(void * object, struct k_thread * thread);
static inline void k_object_access_grant(void * object, struct k_thread * thread)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke2(*(uintptr_t *)&object, *(uintptr_t *)&thread, K_SYSCALL_K_OBJECT_ACCESS_GRANT);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_object_access_grant(object, thread);
}


extern void z_impl_k_object_release(void * object);
static inline void k_object_release(void * object)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&object, K_SYSCALL_K_OBJECT_RELEASE);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_object_release(object);
}


extern void * z_impl_k_object_alloc(enum k_objects otype);
static inline void * k_object_alloc(enum k_objects otype)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (void *) arch_syscall_invoke1(*(uintptr_t *)&otype, K_SYSCALL_K_OBJECT_ALLOC);
	}
#endif
	compiler_barrier();
	return z_impl_k_object_alloc(otype);
}


extern k_tid_t z_impl_k_thread_create(struct k_thread * new_thread, k_thread_stack_t * stack, size_t stack_size, k_thread_entry_t entry, void * p1, void * p2, void * p3, int prio, u32_t options, k_timeout_t delay);
static inline k_tid_t k_thread_create(struct k_thread * new_thread, k_thread_stack_t * stack, size_t stack_size, k_thread_entry_t entry, void * p1, void * p2, void * p3, int prio, u32_t options, k_timeout_t delay)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = delay;
		uintptr_t more[] = {
			*(uintptr_t *)&p2,
			*(uintptr_t *)&p3,
			*(uintptr_t *)&prio,
			*(uintptr_t *)&options,
			parm0.split.lo,
			parm0.split.hi
		};
		return (k_tid_t) arch_syscall_invoke6(*(uintptr_t *)&new_thread, *(uintptr_t *)&stack, *(uintptr_t *)&stack_size, *(uintptr_t *)&entry, *(uintptr_t *)&p1, (uintptr_t) &more, K_SYSCALL_K_THREAD_CREATE);
	}
#endif
	compiler_barrier();
	return z_impl_k_thread_create(new_thread, stack, stack_size, entry, p1, p2, p3, prio, options, delay);
}


extern int z_impl_k_thread_stack_space_get(const struct k_thread * thread, size_t * unused_ptr);
static inline int k_thread_stack_space_get(const struct k_thread * thread, size_t * unused_ptr)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke2(*(uintptr_t *)&thread, *(uintptr_t *)&unused_ptr, K_SYSCALL_K_THREAD_STACK_SPACE_GET);
	}
#endif
	compiler_barrier();
	return z_impl_k_thread_stack_space_get(thread, unused_ptr);
}


extern int z_impl_k_thread_join(struct k_thread * thread, k_timeout_t timeout);
static inline int k_thread_join(struct k_thread * thread, k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = timeout;
		return (int) arch_syscall_invoke3(*(uintptr_t *)&thread, parm0.split.lo, parm0.split.hi, K_SYSCALL_K_THREAD_JOIN);
	}
#endif
	compiler_barrier();
	return z_impl_k_thread_join(thread, timeout);
}


extern s32_t z_impl_k_sleep(k_timeout_t timeout);
static inline s32_t k_sleep(k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = timeout;
		return (s32_t) arch_syscall_invoke2(parm0.split.lo, parm0.split.hi, K_SYSCALL_K_SLEEP);
	}
#endif
	compiler_barrier();
	return z_impl_k_sleep(timeout);
}


extern s32_t z_impl_k_usleep(s32_t us);
static inline s32_t k_usleep(s32_t us)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (s32_t) arch_syscall_invoke1(*(uintptr_t *)&us, K_SYSCALL_K_USLEEP);
	}
#endif
	compiler_barrier();
	return z_impl_k_usleep(us);
}


extern void z_impl_k_busy_wait(u32_t usec_to_wait);
static inline void k_busy_wait(u32_t usec_to_wait)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&usec_to_wait, K_SYSCALL_K_BUSY_WAIT);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_busy_wait(usec_to_wait);
}


extern void z_impl_k_yield();
static inline void k_yield()
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke0(K_SYSCALL_K_YIELD);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_yield();
}


extern void z_impl_k_wakeup(k_tid_t thread);
static inline void k_wakeup(k_tid_t thread)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&thread, K_SYSCALL_K_WAKEUP);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_wakeup(thread);
}


extern k_tid_t z_impl_k_current_get();
static inline k_tid_t k_current_get()
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (k_tid_t) arch_syscall_invoke0(K_SYSCALL_K_CURRENT_GET);
	}
#endif
	compiler_barrier();
	return z_impl_k_current_get();
}


extern void z_impl_k_thread_abort(k_tid_t thread);
static inline void k_thread_abort(k_tid_t thread)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&thread, K_SYSCALL_K_THREAD_ABORT);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_thread_abort(thread);
}


extern void z_impl_k_thread_start(k_tid_t thread);
static inline void k_thread_start(k_tid_t thread)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&thread, K_SYSCALL_K_THREAD_START);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_thread_start(thread);
}


extern k_ticks_t z_impl_k_thread_timeout_expires_ticks(struct k_thread * t);
static inline k_ticks_t k_thread_timeout_expires_ticks(struct k_thread * t)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (k_ticks_t) arch_syscall_invoke1(*(uintptr_t *)&t, K_SYSCALL_K_THREAD_TIMEOUT_EXPIRES_TICKS);
	}
#endif
	compiler_barrier();
	return z_impl_k_thread_timeout_expires_ticks(t);
}


extern k_ticks_t z_impl_k_thread_timeout_remaining_ticks(struct k_thread * t);
static inline k_ticks_t k_thread_timeout_remaining_ticks(struct k_thread * t)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (k_ticks_t) arch_syscall_invoke1(*(uintptr_t *)&t, K_SYSCALL_K_THREAD_TIMEOUT_REMAINING_TICKS);
	}
#endif
	compiler_barrier();
	return z_impl_k_thread_timeout_remaining_ticks(t);
}


extern int z_impl_k_thread_priority_get(k_tid_t thread);
static inline int k_thread_priority_get(k_tid_t thread)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke1(*(uintptr_t *)&thread, K_SYSCALL_K_THREAD_PRIORITY_GET);
	}
#endif
	compiler_barrier();
	return z_impl_k_thread_priority_get(thread);
}


extern void z_impl_k_thread_priority_set(k_tid_t thread, int prio);
static inline void k_thread_priority_set(k_tid_t thread, int prio)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke2(*(uintptr_t *)&thread, *(uintptr_t *)&prio, K_SYSCALL_K_THREAD_PRIORITY_SET);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_thread_priority_set(thread, prio);
}


extern void z_impl_k_thread_deadline_set(k_tid_t thread, int deadline);
static inline void k_thread_deadline_set(k_tid_t thread, int deadline)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke2(*(uintptr_t *)&thread, *(uintptr_t *)&deadline, K_SYSCALL_K_THREAD_DEADLINE_SET);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_thread_deadline_set(thread, deadline);
}


extern void z_impl_k_thread_suspend(k_tid_t thread);
static inline void k_thread_suspend(k_tid_t thread)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&thread, K_SYSCALL_K_THREAD_SUSPEND);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_thread_suspend(thread);
}


extern void z_impl_k_thread_resume(k_tid_t thread);
static inline void k_thread_resume(k_tid_t thread)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&thread, K_SYSCALL_K_THREAD_RESUME);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_thread_resume(thread);
}


extern int z_impl_k_is_preempt_thread();
static inline int k_is_preempt_thread()
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke0(K_SYSCALL_K_IS_PREEMPT_THREAD);
	}
#endif
	compiler_barrier();
	return z_impl_k_is_preempt_thread();
}


extern void z_impl_k_thread_custom_data_set(void * value);
static inline void k_thread_custom_data_set(void * value)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&value, K_SYSCALL_K_THREAD_CUSTOM_DATA_SET);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_thread_custom_data_set(value);
}


extern void * z_impl_k_thread_custom_data_get();
static inline void * k_thread_custom_data_get()
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (void *) arch_syscall_invoke0(K_SYSCALL_K_THREAD_CUSTOM_DATA_GET);
	}
#endif
	compiler_barrier();
	return z_impl_k_thread_custom_data_get();
}


extern int z_impl_k_thread_name_set(k_tid_t thread_id, const char * value);
static inline int k_thread_name_set(k_tid_t thread_id, const char * value)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke2(*(uintptr_t *)&thread_id, *(uintptr_t *)&value, K_SYSCALL_K_THREAD_NAME_SET);
	}
#endif
	compiler_barrier();
	return z_impl_k_thread_name_set(thread_id, value);
}


extern int z_impl_k_thread_name_copy(k_tid_t thread_id, char * buf, size_t size);
static inline int k_thread_name_copy(k_tid_t thread_id, char * buf, size_t size)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke3(*(uintptr_t *)&thread_id, *(uintptr_t *)&buf, *(uintptr_t *)&size, K_SYSCALL_K_THREAD_NAME_COPY);
	}
#endif
	compiler_barrier();
	return z_impl_k_thread_name_copy(thread_id, buf, size);
}


extern void z_impl_k_timer_start(struct k_timer * timer, k_timeout_t duration, k_timeout_t period);
static inline void k_timer_start(struct k_timer * timer, k_timeout_t duration, k_timeout_t period)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = duration;
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm1;
		parm1.val = period;
		arch_syscall_invoke5(*(uintptr_t *)&timer, parm0.split.lo, parm0.split.hi, parm1.split.lo, parm1.split.hi, K_SYSCALL_K_TIMER_START);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_timer_start(timer, duration, period);
}


extern void z_impl_k_timer_stop(struct k_timer * timer);
static inline void k_timer_stop(struct k_timer * timer)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&timer, K_SYSCALL_K_TIMER_STOP);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_timer_stop(timer);
}


extern u32_t z_impl_k_timer_status_get(struct k_timer * timer);
static inline u32_t k_timer_status_get(struct k_timer * timer)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (u32_t) arch_syscall_invoke1(*(uintptr_t *)&timer, K_SYSCALL_K_TIMER_STATUS_GET);
	}
#endif
	compiler_barrier();
	return z_impl_k_timer_status_get(timer);
}


extern u32_t z_impl_k_timer_status_sync(struct k_timer * timer);
static inline u32_t k_timer_status_sync(struct k_timer * timer)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (u32_t) arch_syscall_invoke1(*(uintptr_t *)&timer, K_SYSCALL_K_TIMER_STATUS_SYNC);
	}
#endif
	compiler_barrier();
	return z_impl_k_timer_status_sync(timer);
}


extern k_ticks_t z_impl_k_timer_expires_ticks(struct k_timer * timer);
static inline k_ticks_t k_timer_expires_ticks(struct k_timer * timer)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (k_ticks_t) arch_syscall_invoke1(*(uintptr_t *)&timer, K_SYSCALL_K_TIMER_EXPIRES_TICKS);
	}
#endif
	compiler_barrier();
	return z_impl_k_timer_expires_ticks(timer);
}


extern k_ticks_t z_impl_k_timer_remaining_ticks(struct k_timer * timer);
static inline k_ticks_t k_timer_remaining_ticks(struct k_timer * timer)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (k_ticks_t) arch_syscall_invoke1(*(uintptr_t *)&timer, K_SYSCALL_K_TIMER_REMAINING_TICKS);
	}
#endif
	compiler_barrier();
	return z_impl_k_timer_remaining_ticks(timer);
}


extern void z_impl_k_timer_user_data_set(struct k_timer * timer, void * user_data);
static inline void k_timer_user_data_set(struct k_timer * timer, void * user_data)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke2(*(uintptr_t *)&timer, *(uintptr_t *)&user_data, K_SYSCALL_K_TIMER_USER_DATA_SET);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_timer_user_data_set(timer, user_data);
}


extern void * z_impl_k_timer_user_data_get(struct k_timer * timer);
static inline void * k_timer_user_data_get(struct k_timer * timer)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (void *) arch_syscall_invoke1(*(uintptr_t *)&timer, K_SYSCALL_K_TIMER_USER_DATA_GET);
	}
#endif
	compiler_barrier();
	return z_impl_k_timer_user_data_get(timer);
}


extern s64_t z_impl_k_uptime_ticks();
static inline s64_t k_uptime_ticks()
{
#ifdef CONFIG_USERSPACE
	u64_t ret64;
	if (z_syscall_trap()) {
		(void)arch_syscall_invoke1((uintptr_t)&ret64, K_SYSCALL_K_UPTIME_TICKS);
		return (s64_t)ret64;
	}
#endif
	compiler_barrier();
	return z_impl_k_uptime_ticks();
}


extern void z_impl_k_queue_init(struct k_queue * queue);
static inline void k_queue_init(struct k_queue * queue)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&queue, K_SYSCALL_K_QUEUE_INIT);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_queue_init(queue);
}


extern void z_impl_k_queue_cancel_wait(struct k_queue * queue);
static inline void k_queue_cancel_wait(struct k_queue * queue)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&queue, K_SYSCALL_K_QUEUE_CANCEL_WAIT);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_queue_cancel_wait(queue);
}


extern s32_t z_impl_k_queue_alloc_append(struct k_queue * queue, void * data);
static inline s32_t k_queue_alloc_append(struct k_queue * queue, void * data)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (s32_t) arch_syscall_invoke2(*(uintptr_t *)&queue, *(uintptr_t *)&data, K_SYSCALL_K_QUEUE_ALLOC_APPEND);
	}
#endif
	compiler_barrier();
	return z_impl_k_queue_alloc_append(queue, data);
}


extern s32_t z_impl_k_queue_alloc_prepend(struct k_queue * queue, void * data);
static inline s32_t k_queue_alloc_prepend(struct k_queue * queue, void * data)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (s32_t) arch_syscall_invoke2(*(uintptr_t *)&queue, *(uintptr_t *)&data, K_SYSCALL_K_QUEUE_ALLOC_PREPEND);
	}
#endif
	compiler_barrier();
	return z_impl_k_queue_alloc_prepend(queue, data);
}


extern void * z_impl_k_queue_get(struct k_queue * queue, k_timeout_t timeout);
static inline void * k_queue_get(struct k_queue * queue, k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = timeout;
		return (void *) arch_syscall_invoke3(*(uintptr_t *)&queue, parm0.split.lo, parm0.split.hi, K_SYSCALL_K_QUEUE_GET);
	}
#endif
	compiler_barrier();
	return z_impl_k_queue_get(queue, timeout);
}


extern int z_impl_k_queue_is_empty(struct k_queue * queue);
static inline int k_queue_is_empty(struct k_queue * queue)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke1(*(uintptr_t *)&queue, K_SYSCALL_K_QUEUE_IS_EMPTY);
	}
#endif
	compiler_barrier();
	return z_impl_k_queue_is_empty(queue);
}


extern void * z_impl_k_queue_peek_head(struct k_queue * queue);
static inline void * k_queue_peek_head(struct k_queue * queue)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (void *) arch_syscall_invoke1(*(uintptr_t *)&queue, K_SYSCALL_K_QUEUE_PEEK_HEAD);
	}
#endif
	compiler_barrier();
	return z_impl_k_queue_peek_head(queue);
}


extern void * z_impl_k_queue_peek_tail(struct k_queue * queue);
static inline void * k_queue_peek_tail(struct k_queue * queue)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (void *) arch_syscall_invoke1(*(uintptr_t *)&queue, K_SYSCALL_K_QUEUE_PEEK_TAIL);
	}
#endif
	compiler_barrier();
	return z_impl_k_queue_peek_tail(queue);
}


extern int z_impl_k_futex_wait(struct k_futex * futex, int expected, k_timeout_t timeout);
static inline int k_futex_wait(struct k_futex * futex, int expected, k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = timeout;
		return (int) arch_syscall_invoke4(*(uintptr_t *)&futex, *(uintptr_t *)&expected, parm0.split.lo, parm0.split.hi, K_SYSCALL_K_FUTEX_WAIT);
	}
#endif
	compiler_barrier();
	return z_impl_k_futex_wait(futex, expected, timeout);
}


extern int z_impl_k_futex_wake(struct k_futex * futex, bool wake_all);
static inline int k_futex_wake(struct k_futex * futex, bool wake_all)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke2(*(uintptr_t *)&futex, *(uintptr_t *)&wake_all, K_SYSCALL_K_FUTEX_WAKE);
	}
#endif
	compiler_barrier();
	return z_impl_k_futex_wake(futex, wake_all);
}


extern s32_t z_impl_k_stack_alloc_init(struct k_stack * stack, u32_t num_entries);
static inline s32_t k_stack_alloc_init(struct k_stack * stack, u32_t num_entries)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (s32_t) arch_syscall_invoke2(*(uintptr_t *)&stack, *(uintptr_t *)&num_entries, K_SYSCALL_K_STACK_ALLOC_INIT);
	}
#endif
	compiler_barrier();
	return z_impl_k_stack_alloc_init(stack, num_entries);
}


extern int z_impl_k_stack_push(struct k_stack * stack, stack_data_t data);
static inline int k_stack_push(struct k_stack * stack, stack_data_t data)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke2(*(uintptr_t *)&stack, *(uintptr_t *)&data, K_SYSCALL_K_STACK_PUSH);
	}
#endif
	compiler_barrier();
	return z_impl_k_stack_push(stack, data);
}


extern int z_impl_k_stack_pop(struct k_stack * stack, stack_data_t * data, k_timeout_t timeout);
static inline int k_stack_pop(struct k_stack * stack, stack_data_t * data, k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = timeout;
		return (int) arch_syscall_invoke4(*(uintptr_t *)&stack, *(uintptr_t *)&data, parm0.split.lo, parm0.split.hi, K_SYSCALL_K_STACK_POP);
	}
#endif
	compiler_barrier();
	return z_impl_k_stack_pop(stack, data, timeout);
}


extern int z_impl_k_mutex_init(struct k_mutex * mutex);
static inline int k_mutex_init(struct k_mutex * mutex)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke1(*(uintptr_t *)&mutex, K_SYSCALL_K_MUTEX_INIT);
	}
#endif
	compiler_barrier();
	return z_impl_k_mutex_init(mutex);
}


extern int z_impl_k_mutex_lock(struct k_mutex * mutex, k_timeout_t timeout);
static inline int k_mutex_lock(struct k_mutex * mutex, k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = timeout;
		return (int) arch_syscall_invoke3(*(uintptr_t *)&mutex, parm0.split.lo, parm0.split.hi, K_SYSCALL_K_MUTEX_LOCK);
	}
#endif
	compiler_barrier();
	return z_impl_k_mutex_lock(mutex, timeout);
}


extern int z_impl_k_mutex_unlock(struct k_mutex * mutex);
static inline int k_mutex_unlock(struct k_mutex * mutex)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke1(*(uintptr_t *)&mutex, K_SYSCALL_K_MUTEX_UNLOCK);
	}
#endif
	compiler_barrier();
	return z_impl_k_mutex_unlock(mutex);
}


extern int z_impl_k_sem_init(struct k_sem * sem, unsigned int initial_count, unsigned int limit);
static inline int k_sem_init(struct k_sem * sem, unsigned int initial_count, unsigned int limit)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke3(*(uintptr_t *)&sem, *(uintptr_t *)&initial_count, *(uintptr_t *)&limit, K_SYSCALL_K_SEM_INIT);
	}
#endif
	compiler_barrier();
	return z_impl_k_sem_init(sem, initial_count, limit);
}


extern int z_impl_k_sem_take(struct k_sem * sem, k_timeout_t timeout);
static inline int k_sem_take(struct k_sem * sem, k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = timeout;
		return (int) arch_syscall_invoke3(*(uintptr_t *)&sem, parm0.split.lo, parm0.split.hi, K_SYSCALL_K_SEM_TAKE);
	}
#endif
	compiler_barrier();
	return z_impl_k_sem_take(sem, timeout);
}


extern void z_impl_k_sem_give(struct k_sem * sem);
static inline void k_sem_give(struct k_sem * sem)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&sem, K_SYSCALL_K_SEM_GIVE);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_sem_give(sem);
}


extern void z_impl_k_sem_reset(struct k_sem * sem);
static inline void k_sem_reset(struct k_sem * sem)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&sem, K_SYSCALL_K_SEM_RESET);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_sem_reset(sem);
}


extern unsigned int z_impl_k_sem_count_get(struct k_sem * sem);
static inline unsigned int k_sem_count_get(struct k_sem * sem)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (unsigned int) arch_syscall_invoke1(*(uintptr_t *)&sem, K_SYSCALL_K_SEM_COUNT_GET);
	}
#endif
	compiler_barrier();
	return z_impl_k_sem_count_get(sem);
}


extern int z_impl_k_msgq_alloc_init(struct k_msgq * msgq, size_t msg_size, u32_t max_msgs);
static inline int k_msgq_alloc_init(struct k_msgq * msgq, size_t msg_size, u32_t max_msgs)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke3(*(uintptr_t *)&msgq, *(uintptr_t *)&msg_size, *(uintptr_t *)&max_msgs, K_SYSCALL_K_MSGQ_ALLOC_INIT);
	}
#endif
	compiler_barrier();
	return z_impl_k_msgq_alloc_init(msgq, msg_size, max_msgs);
}


extern int z_impl_k_msgq_put(struct k_msgq * msgq, void * data, k_timeout_t timeout);
static inline int k_msgq_put(struct k_msgq * msgq, void * data, k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = timeout;
		return (int) arch_syscall_invoke4(*(uintptr_t *)&msgq, *(uintptr_t *)&data, parm0.split.lo, parm0.split.hi, K_SYSCALL_K_MSGQ_PUT);
	}
#endif
	compiler_barrier();
	return z_impl_k_msgq_put(msgq, data, timeout);
}


extern int z_impl_k_msgq_get(struct k_msgq * msgq, void * data, k_timeout_t timeout);
static inline int k_msgq_get(struct k_msgq * msgq, void * data, k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = timeout;
		return (int) arch_syscall_invoke4(*(uintptr_t *)&msgq, *(uintptr_t *)&data, parm0.split.lo, parm0.split.hi, K_SYSCALL_K_MSGQ_GET);
	}
#endif
	compiler_barrier();
	return z_impl_k_msgq_get(msgq, data, timeout);
}


extern int z_impl_k_msgq_peek(struct k_msgq * msgq, void * data);
static inline int k_msgq_peek(struct k_msgq * msgq, void * data)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke2(*(uintptr_t *)&msgq, *(uintptr_t *)&data, K_SYSCALL_K_MSGQ_PEEK);
	}
#endif
	compiler_barrier();
	return z_impl_k_msgq_peek(msgq, data);
}


extern void z_impl_k_msgq_purge(struct k_msgq * msgq);
static inline void k_msgq_purge(struct k_msgq * msgq)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&msgq, K_SYSCALL_K_MSGQ_PURGE);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_msgq_purge(msgq);
}


extern u32_t z_impl_k_msgq_num_free_get(struct k_msgq * msgq);
static inline u32_t k_msgq_num_free_get(struct k_msgq * msgq)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (u32_t) arch_syscall_invoke1(*(uintptr_t *)&msgq, K_SYSCALL_K_MSGQ_NUM_FREE_GET);
	}
#endif
	compiler_barrier();
	return z_impl_k_msgq_num_free_get(msgq);
}


extern void z_impl_k_msgq_get_attrs(struct k_msgq * msgq, struct k_msgq_attrs * attrs);
static inline void k_msgq_get_attrs(struct k_msgq * msgq, struct k_msgq_attrs * attrs)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke2(*(uintptr_t *)&msgq, *(uintptr_t *)&attrs, K_SYSCALL_K_MSGQ_GET_ATTRS);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_msgq_get_attrs(msgq, attrs);
}


extern u32_t z_impl_k_msgq_num_used_get(struct k_msgq * msgq);
static inline u32_t k_msgq_num_used_get(struct k_msgq * msgq)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (u32_t) arch_syscall_invoke1(*(uintptr_t *)&msgq, K_SYSCALL_K_MSGQ_NUM_USED_GET);
	}
#endif
	compiler_barrier();
	return z_impl_k_msgq_num_used_get(msgq);
}


extern int z_impl_k_pipe_alloc_init(struct k_pipe * pipe, size_t size);
static inline int k_pipe_alloc_init(struct k_pipe * pipe, size_t size)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke2(*(uintptr_t *)&pipe, *(uintptr_t *)&size, K_SYSCALL_K_PIPE_ALLOC_INIT);
	}
#endif
	compiler_barrier();
	return z_impl_k_pipe_alloc_init(pipe, size);
}


extern int z_impl_k_pipe_put(struct k_pipe * pipe, void * data, size_t bytes_to_write, size_t * bytes_written, size_t min_xfer, k_timeout_t timeout);
static inline int k_pipe_put(struct k_pipe * pipe, void * data, size_t bytes_to_write, size_t * bytes_written, size_t min_xfer, k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = timeout;
		uintptr_t more[] = {
			parm0.split.lo,
			parm0.split.hi
		};
		return (int) arch_syscall_invoke6(*(uintptr_t *)&pipe, *(uintptr_t *)&data, *(uintptr_t *)&bytes_to_write, *(uintptr_t *)&bytes_written, *(uintptr_t *)&min_xfer, (uintptr_t) &more, K_SYSCALL_K_PIPE_PUT);
	}
#endif
	compiler_barrier();
	return z_impl_k_pipe_put(pipe, data, bytes_to_write, bytes_written, min_xfer, timeout);
}


extern int z_impl_k_pipe_get(struct k_pipe * pipe, void * data, size_t bytes_to_read, size_t * bytes_read, size_t min_xfer, k_timeout_t timeout);
static inline int k_pipe_get(struct k_pipe * pipe, void * data, size_t bytes_to_read, size_t * bytes_read, size_t min_xfer, k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = timeout;
		uintptr_t more[] = {
			parm0.split.lo,
			parm0.split.hi
		};
		return (int) arch_syscall_invoke6(*(uintptr_t *)&pipe, *(uintptr_t *)&data, *(uintptr_t *)&bytes_to_read, *(uintptr_t *)&bytes_read, *(uintptr_t *)&min_xfer, (uintptr_t) &more, K_SYSCALL_K_PIPE_GET);
	}
#endif
	compiler_barrier();
	return z_impl_k_pipe_get(pipe, data, bytes_to_read, bytes_read, min_xfer, timeout);
}


extern size_t z_impl_k_pipe_read_avail(struct k_pipe * pipe);
static inline size_t k_pipe_read_avail(struct k_pipe * pipe)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (size_t) arch_syscall_invoke1(*(uintptr_t *)&pipe, K_SYSCALL_K_PIPE_READ_AVAIL);
	}
#endif
	compiler_barrier();
	return z_impl_k_pipe_read_avail(pipe);
}


extern size_t z_impl_k_pipe_write_avail(struct k_pipe * pipe);
static inline size_t k_pipe_write_avail(struct k_pipe * pipe)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (size_t) arch_syscall_invoke1(*(uintptr_t *)&pipe, K_SYSCALL_K_PIPE_WRITE_AVAIL);
	}
#endif
	compiler_barrier();
	return z_impl_k_pipe_write_avail(pipe);
}


extern int z_impl_k_poll(struct k_poll_event * events, int num_events, k_timeout_t timeout);
static inline int k_poll(struct k_poll_event * events, int num_events, k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm0;
		parm0.val = timeout;
		return (int) arch_syscall_invoke4(*(uintptr_t *)&events, *(uintptr_t *)&num_events, parm0.split.lo, parm0.split.hi, K_SYSCALL_K_POLL);
	}
#endif
	compiler_barrier();
	return z_impl_k_poll(events, num_events, timeout);
}


extern void z_impl_k_poll_signal_init(struct k_poll_signal * signal);
static inline void k_poll_signal_init(struct k_poll_signal * signal)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&signal, K_SYSCALL_K_POLL_SIGNAL_INIT);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_poll_signal_init(signal);
}


extern void z_impl_k_poll_signal_reset(struct k_poll_signal * signal);
static inline void k_poll_signal_reset(struct k_poll_signal * signal)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke1(*(uintptr_t *)&signal, K_SYSCALL_K_POLL_SIGNAL_RESET);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_poll_signal_reset(signal);
}


extern void z_impl_k_poll_signal_check(struct k_poll_signal * signal, unsigned int * signaled, int * result);
static inline void k_poll_signal_check(struct k_poll_signal * signal, unsigned int * signaled, int * result)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke3(*(uintptr_t *)&signal, *(uintptr_t *)&signaled, *(uintptr_t *)&result, K_SYSCALL_K_POLL_SIGNAL_CHECK);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_poll_signal_check(signal, signaled, result);
}


extern int z_impl_k_poll_signal_raise(struct k_poll_signal * signal, int result);
static inline int k_poll_signal_raise(struct k_poll_signal * signal, int result)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke2(*(uintptr_t *)&signal, *(uintptr_t *)&result, K_SYSCALL_K_POLL_SIGNAL_RAISE);
	}
#endif
	compiler_barrier();
	return z_impl_k_poll_signal_raise(signal, result);
}


extern void z_impl_k_str_out(char * c, size_t n);
static inline void k_str_out(char * c, size_t n)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		arch_syscall_invoke2(*(uintptr_t *)&c, *(uintptr_t *)&n, K_SYSCALL_K_STR_OUT);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_str_out(c, n);
}


extern int z_impl_k_float_disable(struct k_thread * thread);
static inline int k_float_disable(struct k_thread * thread)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke1(*(uintptr_t *)&thread, K_SYSCALL_K_FLOAT_DISABLE);
	}
#endif
	compiler_barrier();
	return z_impl_k_float_disable(thread);
}


#ifdef __cplusplus
}
#endif

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif

#endif
#endif /* include guard */
