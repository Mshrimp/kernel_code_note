#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/device.h>
#include "mini.h"

extern struct bus_type mini;

void mini_release(struct device *dev)
{}

int mini_device_register(struct mini_device *dev)
{
	dev->dev.init_name = dev->name;
	dev->dev.bus = &mini;
	dev->dev.release = mini_release;

	return device_register(&dev->dev);
}
EXPORT_SYMBOL(mini_device_register);

void mini_device_unregister(struct mini_device *dev)
{
	device_unregister(&dev->dev);
}
EXPORT_SYMBOL(mini_device_unregister);

int mini_probe(struct device *dev)
{
	struct mini_device *mdev;
	struct mini_driver *mdrv;

	mdev = container_of(dev, struct mini_device, dev);
	mdrv = container_of(dev->driver, struct mini_driver, drv);

	return mdrv->probe(mdev);
}

int mini_remove(struct device *dev)
{
	struct mini_device *mdev;
	struct mini_driver *mdrv;

	mdev = container_of(dev, struct mini_device, dev);
	mdrv = container_of(dev->driver, struct mini_driver, drv);

	return mdrv->remove(mdev);
}

int mini_driver_register(struct mini_driver *dri)
{
	dri->drv.name = dri->name;
	dri->drv.bus = &mini;
	dri->drv.probe = mini_probe;
	dri->drv.remove = mini_remove;

	return driver_register(&dri->drv);
}
EXPORT_SYMBOL(mini_driver_register);

void mini_driver_unregister(struct mini_driver *dri)
{
	driver_unregister(&dri->drv);
}
EXPORT_SYMBOL(mini_driver_unregister);


MODULE_LICENSE("GPL");

