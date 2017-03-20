/* Copyright (c) 2012,2014-2016 The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/seq_file.h>
#include <linux/err.h>
#include <linux/stacktrace.h>
#include <wcnss_wlan.h>
#include <linux/spinlock.h>
#ifdef	CONFIG_WCNSS_SKB_PRE_ALLOC
#include <linux/skbuff.h>
#endif
#include <linux/debugfs.h>
static DEFINE_SPINLOCK(alloc_lock);

#ifdef CONFIG_SLUB_DEBUG
#define WCNSS_MAX_STACK_TRACE			64
#endif

#define PRE_ALLOC_DEBUGFS_DIR		"cnss-prealloc"
#define PRE_ALLOC_DEBUGFS_FILE_OBJ	"status"

static struct dentry *debug_base;

struct wcnss_prealloc {
	int occupied;
	unsigned int size;
	void *ptr;
#ifdef CONFIG_SLUB_DEBUG
	unsigned long stack_trace[WCNSS_MAX_STACK_TRACE];
	struct stack_trace trace;
#endif
};

#ifdef CONFIG_WCNSS_SKB_PRE_ALLOC
/* pre-alloced memory for skb */
static struct wcnss_prealloc wcnss_skb_allocs[] = {
	{0, 64 * 1024, NULL},
	{0, 64 * 1024, NULL},
	{0, 64 * 1024, NULL},
	{0, 64 * 1024, NULL},
	{0, 64 * 1024, NULL},
	{0, 64 * 1024, NULL},
	{0, 64 * 1024, NULL},
	{0, 64 * 1024, NULL},
	{0, 128 * 1024, NULL},
	{0, 128 * 1024, NULL},
};
#endif

/* pre-alloced mem for WLAN driver */
static struct wcnss_prealloc wcnss_allocs[] = {
	{0, 8, NULL},
};

#ifdef CONFIG_WCNSS_SKB_PRE_ALLOC
int cnss_skb_prealloc_init(void)
{
	int i;
	
	for (i = 0; i < ARRAY_SIZE(wcnss_skb_allocs); i++) {
		wcnss_skb_allocs[i].occupied = 0;
		wcnss_skb_allocs[i].ptr =
			dev_alloc_skb(wcnss_skb_allocs[i].size);
		if (wcnss_skb_allocs[i].ptr == NULL)
			return -ENOMEM;
	}

	return 0;
}
#else
int cnss_skb_prealloc_init(void)
{
	return 0;
}
#endif

int wcnss_prealloc_init(void)
{
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(wcnss_allocs); i++) {
		wcnss_allocs[i].occupied = 0;
		wcnss_allocs[i].ptr = kmalloc(wcnss_allocs[i].size, GFP_KERNEL);
		if (wcnss_allocs[i].ptr == NULL)
			return -ENOMEM;
	}
	ret = cnss_skb_prealloc_init();

	return ret;
}

#ifdef CONFIG_WCNSS_SKB_PRE_ALLOC
void cnss_skb_prealloc_deinit(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(wcnss_skb_allocs); i++) {
		dev_kfree_skb(wcnss_skb_allocs[i].ptr);
		wcnss_skb_allocs[i].ptr = NULL;
	}
}
#else
void cnss_skb_prealloc_deinit(void) {}
#endif

void wcnss_prealloc_deinit(void)
{
	int i = 0;

	for (i = 0; i < ARRAY_SIZE(wcnss_allocs); i++) {
		kfree(wcnss_allocs[i].ptr);
		wcnss_allocs[i].ptr = NULL;
	}

	cnss_skb_prealloc_deinit();
}

#ifdef CONFIG_SLUB_DEBUG
static void wcnss_prealloc_save_stack_trace(struct wcnss_prealloc *entry)
{
	struct stack_trace *trace = &entry->trace;

	memset(&entry->stack_trace, 0, sizeof(entry->stack_trace));
	trace->nr_entries = 0;
	trace->max_entries = WCNSS_MAX_STACK_TRACE;
	trace->entries = entry->stack_trace;
	trace->skip = 2;

	save_stack_trace(trace);

	return;
}
#else
static inline void wcnss_prealloc_save_stack_trace(struct wcnss_prealloc *entry)
{
	return;
}
#endif

void *wcnss_prealloc_get(unsigned int size)
{
	int i = 0;
	unsigned long flags;

	spin_lock_irqsave(&alloc_lock, flags);
	for (i = 0; i < ARRAY_SIZE(wcnss_allocs); i++) {
		if (wcnss_allocs[i].occupied)
			continue;

		if (wcnss_allocs[i].size >= size) {
			/* we found the slot */
			wcnss_allocs[i].occupied = 1;
			spin_unlock_irqrestore(&alloc_lock, flags);
			wcnss_prealloc_save_stack_trace(&wcnss_allocs[i]);
			return wcnss_allocs[i].ptr;
		}
	}
	spin_unlock_irqrestore(&alloc_lock, flags);

	printk(KERN_WARNING "wcnss: %s: prealloc not available for size: %d\n",
			__func__, size);
	WARN_ON(1);

	return NULL;
}


int wcnss_prealloc_put(void *ptr)
{
	int i = 0;
	unsigned long flags;

	spin_lock_irqsave(&alloc_lock, flags);
	for (i = 0; i < ARRAY_SIZE(wcnss_allocs); i++) {
		if (wcnss_allocs[i].ptr == ptr) {
			wcnss_allocs[i].occupied = 0;
			spin_unlock_irqrestore(&alloc_lock, flags);
			return 1;
		}
	}
	spin_unlock_irqrestore(&alloc_lock, flags);

	return 0;
}


#ifdef CONFIG_WCNSS_SKB_PRE_ALLOC
struct sk_buff *wcnss_skb_prealloc_get(unsigned int size)
{
	int i = 0;
	unsigned long flags;

	spin_lock_irqsave(&alloc_lock, flags);
	for (i = 0; i < ARRAY_SIZE(wcnss_skb_allocs); i++) {
		if (wcnss_skb_allocs[i].occupied)
			continue;

		if (wcnss_skb_allocs[i].size > size) {
			/* we found the slot */
			wcnss_skb_allocs[i].occupied = 1;
			spin_unlock_irqrestore(&alloc_lock, flags);
			wcnss_prealloc_save_stack_trace(&wcnss_allocs[i]);
			return wcnss_skb_allocs[i].ptr;
		}
	}
	spin_unlock_irqrestore(&alloc_lock, flags);

	printk(KERN_WARNING "wcnss: %s: prealloc not available for size: %d\n",
	       __func__, size);

	return NULL;
}

int wcnss_skb_prealloc_put(struct sk_buff *skb)
{
	int i = 0;
	unsigned long flags;

	spin_lock_irqsave(&alloc_lock, flags);
	for (i = 0; i < ARRAY_SIZE(wcnss_skb_allocs); i++) {
		if (wcnss_skb_allocs[i].ptr == skb) {
			wcnss_skb_allocs[i].occupied = 0;
			spin_unlock_irqrestore(&alloc_lock, flags);
			return 1;
		}
	}
	spin_unlock_irqrestore(&alloc_lock, flags);

	return 0;
}
#endif


int wcnss_pre_alloc_init(void)
{
	int ret;

	ret = wcnss_prealloc_init();
	if (ret) {
		pr_err("%s: Failed to init the prealloc pool\n", __func__);
		return ret;
	}

	debug_base = debugfs_create_dir(PRE_ALLOC_DEBUGFS_DIR, NULL);
	if (IS_ERR_OR_NULL(debug_base)) {
		pr_err("%s: Failed to create debugfs dir\n", __func__);
	}

	return ret;
}

void wcnss_pre_alloc_exit(void)
{
	wcnss_prealloc_deinit();
	debugfs_remove_recursive(debug_base);
}

