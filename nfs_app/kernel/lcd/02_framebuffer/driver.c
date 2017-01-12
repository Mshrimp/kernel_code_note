#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/clk.h>
#include <linux/fb.h>
#include <linux/dma-mapping.h>

#include <plat/fb.h>
#include <plat/regs-fb.h>
#include <plat/regs-fb-v4.h>

#define	DEV_NAME	"test_dev"
#define __WDT_HZ	3125

struct test_lcd_dev{
	void *reg;
	int reg_len;

	struct clk *lcd;
	struct clk *sclk_fimd;

	unsigned int fb_phys;
	void *fb_virt;
	int fb_len;
	int xres;
	int yres;
	int bpp;
	int max_bpp;

	struct fb_info *info;
	struct s3c_fb_platdata *pd;
};

void lcd_reg_init(struct test_lcd_dev *tdev)
{
	struct s3c_fb_platdata *pd = tdev->pd;
	struct fb_videomode *vt = pd->vtiming;
	struct s3c_fb_pd_win *win = pd->win[0];
	u32 val;
	pd->setup_gpio();

	//iowrite32(pd->vidcon0, tdev->reg + VIDCON0);
	//val = ioread32(tdev->reg + VIDCON0);
	val = VIDCON0_CLKVAL_F(15) | pd->vidcon0;
	iowrite32(val, tdev->reg + VIDCON0);
	
	val = pd->vidcon1;
	iowrite32(val, tdev->reg + VIDCON1);

#if 1
	val = VIDTCON0_VBPD(vt->upper_margin - 1) | 
			VIDTCON0_VFPD(vt->lower_margin - 1) | VIDTCON0_VSPW(vt->vsync_len - 1);
	iowrite32(val, tdev->reg + VIDTCON0);

	val = VIDTCON1_HBPD(vt->left_margin - 1) | 
			VIDTCON1_HFPD(vt->right_margin - 1) | VIDTCON1_HSPW(vt->hsync_len - 1);
	iowrite32(val, tdev->reg + VIDTCON1);	

	val = VIDTCON2_LINEVAL(vt->yres - 1) | VIDTCON2_HOZVAL(vt->xres - 1);
	iowrite32(val, tdev->reg + VIDTCON2);
#else
	val = (20<<16)|(22<<8)|1; 
	iowrite32(val, tdev->reg + VIDTCON0);

	val = (43<<16)|(209<<8)|1;
	iowrite32(val, tdev->reg + VIDTCON1);	

	val = (479<<11)|799;
	iowrite32(val, tdev->reg + VIDTCON2);
#endif


	val = WINCONx_WSWP | WINCON0_BPPMODE_24BPP_888 | WINCONx_ENWIN;
	iowrite32(val, tdev->reg + WINCON(0));

	val = VIDOSDxA_TOPLEFT_X(0) | VIDOSDxA_TOPLEFT_Y(0);
	iowrite32(val, tdev->reg + VIDOSD_BASE);

	val = VIDOSDxB_BOTRIGHT_X(win->xres) | VIDOSDxB_BOTRIGHT_Y(win->yres);
	iowrite32(val, tdev->reg + VIDOSD_BASE + 4);
	
	val = tdev->fb_phys;
	iowrite32(val, tdev->reg + VIDW_BUF_START(0));

	val = tdev->fb_phys + tdev->fb_len;
	iowrite32(val, tdev->reg + VIDW_BUF_END(0));

	val = VIDW_BUF_SIZE_PAGEWIDTH(tdev->xres * 4);
	iowrite32(val, tdev->reg + VIDW_BUF_SIZE(0));

	val = ioread32(tdev->reg + SHADOWCON) | SHADOWCON_CHx_ENABLE(0); 
	iowrite32(val, tdev->reg + SHADOWCON);

	val = ioread32(tdev->reg + VIDCON0) | VIDCON0_ENVID | VIDCON0_ENVID_F;
	iowrite32(val, tdev->reg + VIDCON0);
}

struct fb_ops fbops = {
	.owner      = THIS_MODULE,
	.fb_fillrect    = cfb_fillrect,   /* Needed !!! */
	.fb_copyarea    = cfb_copyarea,   /* Needed !!! */
	.fb_imageblit   = cfb_imageblit,  /* Needed !!! */
};

void fb_info_init(struct fb_info *info, struct test_lcd_dev *tdev)
{
	info->var.xres = tdev->xres;
	info->var.yres = tdev->yres;
	info->var.bits_per_pixel = tdev->bpp;

	info->fix.smem_start = tdev->fb_phys;
	info->fix.smem_len = tdev->fb_len;
	info->fix.line_length = tdev->xres * tdev->max_bpp / 8;

	info->screen_base = tdev->fb_virt;
	info->screen_size = tdev->fb_len;

	info->fbops = &fbops;

}

int test_probe (struct platform_device *dev)		//插入;
{
	int ret;
	struct fb_info *info;
	struct s3c_fb_platdata *pd = dev->dev.platform_data;
	struct test_lcd_dev *tdev;
	struct s3c_fb_pd_win *win = pd->win[0];
	struct resource *res;
	printk("test probe driver start\n");

//	tdev = kzalloc(sizeof(struct test_lcd_dev), GFP_KERNEL);
	info = framebuffer_alloc(sizeof(struct test_lcd_dev), &dev->dev);	
	if(IS_ERR_OR_NULL(info))
	{
		ret = -ENOMEM;
		goto ERROR_kzalloc;
	}

	tdev = info->par;
	tdev->info = info;
	tdev->pd = pd;

	platform_set_drvdata(dev, tdev);

	res = platform_get_resource(dev, IORESOURCE_MEM, 0);	
	if(IS_ERR_OR_NULL(res))
	{
		ret = -EBUSY;
		goto ERROR_get_mem_resource;
	}

	tdev->reg_len = resource_size(res);
	tdev->reg = ioremap(res->start, tdev->reg_len);
	if(IS_ERR_OR_NULL(tdev->reg))
	{
		ret = -ENOMEM;
		goto ERROR_ioremap;
	}

	tdev->lcd = clk_get(&dev->dev, "lcd");
	if(IS_ERR(tdev->lcd))
	{
		printk("%d\n", __LINE__);
		ret = -EBUSY;
		goto ERROR_clk_lcd;
	}
	clk_enable(tdev->lcd);

	tdev->sclk_fimd = clk_get(&dev->dev, "sclk_fimd");
	if(IS_ERR(tdev->sclk_fimd))
	{
		printk("%d\n", __LINE__);
		ret = -EBUSY;
		goto ERROR_clk_fimd;
	}
	clk_enable(tdev->sclk_fimd);
	
	tdev->xres = win->xres;
	tdev->yres = win->yres;
	tdev->bpp = win->default_bpp;
	tdev->max_bpp = win->max_bpp;
	tdev->fb_len = win->xres * win->yres * win->max_bpp / 8;
	tdev->fb_virt = dma_alloc_coherent(NULL, 
			tdev->fb_len, &tdev->fb_phys, GFP_KERNEL);
	if(IS_ERR_OR_NULL(tdev->fb_virt))
	{
		printk("%d\n", __LINE__);
		ret = -ENOMEM;
		goto ERROR_dma_alloc;
	}
	printk("fb_virt = %p fb_phys = %x len = %d xres = %d yres = %d bpp = %d "
			"max_bpp = %d\n", 
			tdev->fb_virt, tdev->fb_phys, tdev->fb_len, 
			tdev->xres, tdev->yres, tdev->bpp, win->max_bpp);

	lcd_reg_init(tdev);

	fb_info_init(info, tdev);

	ret = register_framebuffer(info);
	if(IS_ERR_VALUE(ret))
	{
		goto ERROR_framebuffer_register;
	}

	return 0;
ERROR_framebuffer_register:
	dma_free_coherent(NULL, tdev->fb_len, tdev->fb_virt, tdev->fb_phys);
ERROR_dma_alloc:
	clk_disable(tdev->sclk_fimd);
	clk_put(tdev->sclk_fimd);
ERROR_clk_fimd:
	clk_disable(tdev->lcd);
	clk_put(tdev->lcd);
ERROR_clk_lcd:
	iounmap(tdev->reg);
ERROR_ioremap:
ERROR_get_mem_resource:
	kfree(tdev);
ERROR_kzalloc:
	return ret;
}

int test_remove (struct platform_device *dev)	//拔出;
{
	struct test_lcd_dev *tdev;
	struct fb_info *info;
	printk("test remove driver end\n");
	tdev = platform_get_drvdata(dev);
	info = tdev->info;
	unregister_framebuffer(info);
	dma_free_coherent(NULL, tdev->fb_len, tdev->fb_virt, tdev->fb_phys);
	clk_disable(tdev->sclk_fimd);
	clk_put(tdev->sclk_fimd);
	clk_disable(tdev->lcd);
	clk_put(tdev->lcd);
	iounmap(tdev->reg);
	framebuffer_release(info);
	//kfree(tdev);
	return 0;
}

struct platform_device_id idtable[] = {		//当前驱动支持的设备列表;
	{"exynos4-fb", 0},
	{},
};

struct platform_driver tdri = {
	.driver = {
		.name = "test-dri", 	
	},
	.probe = test_probe,
	.remove = test_remove,
	.id_table = idtable,
};

int test_init(void)			
{
	int ret;
	printk("module init\n");
	ret = platform_driver_register(&tdri);
	if(IS_ERR_VALUE(ret))
	{
		return ret;
	}
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	platform_driver_unregister(&tdri);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

