#ifndef	__MINI_H__
#define __MINI_H__

struct mini_device{
	char *name;
	struct device dev;
	struct resource *resources;
	int res_num;
};

int mini_device_register(struct mini_device *dev);
void mini_device_unregister(struct mini_device *dev);

typedef int (*probe_t) (struct mini_device *dev);
typedef int (*remove_t) (struct mini_device *dev);
struct mini_driver{
	char *name;
	probe_t probe;
	remove_t remove;
	struct device_driver drv;
};

int mini_driver_register(struct mini_driver *dri);
void mini_driver_unregister(struct mini_driver *dri);

#endif
