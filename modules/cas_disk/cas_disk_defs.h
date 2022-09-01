/*
 * Copyright(c) 2012-2022 Intel Corporation
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __CASDISK_DEFS_H__
#define __CASDISK_DEFS_H__

#include <linux/version.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kobject.h>
#include <linux/blkdev.h>

struct casdsk_module {
	struct mutex lock;

	struct list_head disk_list;
	uint32_t next_disk_id;
	int disk_major;
	int next_minor;

	struct kmem_cache *disk_cache;
	struct kmem_cache *exp_obj_cache;

	struct kobject kobj;
};

extern struct casdsk_module *casdsk_module;

#define CASDSK_LOGO "CAS Disk"

static inline struct block_device *open_bdev_exclusive(const char *path,
						       fmode_t mode,
						       void *holder)
{
	return blkdev_get_by_path(path, mode | FMODE_EXCL, holder);
}

static inline void close_bdev_exclusive(struct block_device *bdev, fmode_t mode)
{
	blkdev_put(bdev, mode | FMODE_EXCL);
}

static inline int bd_claim_by_disk(struct block_device *bdev, void *holder,
				   struct gendisk *disk)
{
	return bd_link_disk_holder(bdev, disk);
}

static inline void bd_release_from_disk(struct block_device *bdev,
					struct gendisk *disk)
{
	return bd_unlink_disk_holder(bdev, disk);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 3, 0)
	#define KRETURN(x)	({ return (x); })
	#define MAKE_RQ_RET_TYPE blk_qc_t
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
	#define KRETURN(x)	return
	#define MAKE_RQ_RET_TYPE void
#else
	#define KRETURN(x)	({ return (x); })
	#define MAKE_RQ_RET_TYPE int
#endif

#include "debug.h"

#endif
