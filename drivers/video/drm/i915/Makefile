

CC = gcc
FASM = e:/fasm/fasm.exe

DEFINES	 = -D__KERNEL__ -DCONFIG_X86_32 -DCONFIG_TINY_RCU -DCONFIG_X86_L1_CACHE_SHIFT=6
DEFINES += -DCONFIG_ARCH_HAS_CACHE_LINE_SIZE -DCONFIG_DRM_I915_FBDEV -DCONFIG_DMI -DKBUILD_MODNAME=\"i915.dll\"


DDK_TOPDIR      = /d/kos/kolibri/drivers/ddk
DRV_INCLUDES    = /d/kos/kolibri/drivers/include
DRM_TOPDIR   	= $(CURDIR)/..

INCLUDES =	-I$(DRV_INCLUDES)	\
		-I$(DRV_INCLUDES)/asm 	\
		-I$(DRV_INCLUDES)/uapi	\
		-I$(DRV_INCLUDES)/drm -I./ -I$(DRV_INCLUDES)

CFLAGS= -c -O2 $(INCLUDES) $(DEFINES) -march=i686 -msse2 -fomit-frame-pointer -fno-ident -fno-builtin-printf
CFLAGS+= -mno-stack-arg-probe -mpreferred-stack-boundary=2 -mincoming-stack-boundary=2 -mno-ms-bitfields

LIBPATH:= $(DDK_TOPDIR)

LIBS:=    -lddk -lcore -lgcc

PE_FLAGS = --major-os-version 0 --minor-os-version 7 --major-subsystem-version 0 \
          --minor-subsystem-version 5 --subsystem native

LDFLAGS = -nostdlib -shared -s $(PE_FLAGS) --image-base 0\
	  --file-alignment 512 --section-alignment 4096


NAME:=	  i915

HFILES:=     		$(DRV_INCLUDES)/linux/types.h		\
			$(DRV_INCLUDES)/linux/list.h		\
			$(DRV_INCLUDES)/linux/pci.h		\
			$(DRV_INCLUDES)/drm/drm.h		\
			$(DRV_INCLUDES)/drm/drmP.h		\
			$(DRV_INCLUDES)/drm/drm_edid.h		\
			$(DRV_INCLUDES)/drm/drm_crtc.h		\
			$(DRV_INCLUDES)/drm/drm_mode.h		\
			$(DRV_INCLUDES)/drm/drm_mm.h		\
			i915_drv.h				\
			bitmap.h

NAME_SRC=		main.c					\
			pci.c					\
			dvo_ch7017.c				\
			dvo_ch7xxx.c				\
			dvo_ivch.c				\
			dvo_ns2501.c				\
			dvo_sil164.c				\
			dvo_tfp410.c				\
			i915_cmd_parser.c			\
			i915_dma.c				\
			i915_drv.c				\
			i915_gem.c				\
			i915_gem_context.c			\
			i915_gem_execbuffer.c			\
			i915_gem_evict.c			\
			i915_gem_gtt.c				\
			i915_gem_render_state.c			\
			i915_gem_stolen.c			\
			i915_gem_tiling.c			\
			i915_gpu_error.c			\
			i915_irq.c				\
			i915_params.c				\
			intel_audio.c				\
			intel_bios.c				\
			intel_crt.c				\
			intel_ddi.c				\
			intel_display.c				\
			intel_dp.c				\
			intel_dp_mst.c				\
			intel_dsi.c				\
			intel_dsi_cmd.c				\
			intel_dsi_panel_vbt.c			\
			intel_dsi_pll.c				\
			intel_dvo.c				\
			intel_fbdev.c				\
			intel_fifo_underrun.c			\
			intel_frontbuffer.c			\
			intel_hdmi.c				\
			intel_i2c.c				\
			intel_lrc.c				\
			intel_lvds.c				\
			intel_modes.c				\
			intel_panel.c				\
			intel_pm.c				\
			intel_psr.c				\
			intel_renderstate_gen6.c		\
			intel_renderstate_gen7.c		\
			intel_renderstate_gen8.c		\
			intel_renderstate_gen9.c		\
			intel_ringbuffer.c			\
			intel_runtime_pm.c			\
			intel_sdvo.c				\
			intel_sideband.c			\
			intel_sprite.c				\
			intel_uncore.c				\
			kms_display.c				\
			kos_gem_fb.c				\
			utils.c					\
			../hdmi.c				\
			Gtt/intel-agp.c				\
			Gtt/intel-gtt.c				\
			../drm_cache.c				\
			../drm_global.c				\
			../drm_drv.c				\
			../drm_vma_manager.c			\
			$(DRM_TOPDIR)/i2c/i2c-core.c		\
			$(DRM_TOPDIR)/i2c/i2c-algo-bit.c	\
			$(DRM_TOPDIR)/drm_crtc.c		\
			$(DRM_TOPDIR)/drm_crtc_helper.c		\
			$(DRM_TOPDIR)/drm_dp_helper.c		\
			../drm_dp_mst_topology.c		\
			$(DRM_TOPDIR)/drm_atomic.c		\
			$(DRM_TOPDIR)/drm_edid.c		\
			$(DRM_TOPDIR)/drm_fb_helper.c		\
			$(DRM_TOPDIR)/drm_gem.c			\
			$(DRM_TOPDIR)/drm_irq.c			\
			$(DRM_TOPDIR)/drm_mm.c			\
			$(DRM_TOPDIR)/drm_modes.c		\
			$(DRM_TOPDIR)/drm_modeset_lock.c	\
			$(DRM_TOPDIR)/drm_pci.c			\
			$(DRM_TOPDIR)/drm_plane_helper.c	\
			$(DRM_TOPDIR)/drm_probe_helper.c	\
			$(DRM_TOPDIR)/drm_rect.c		\
			$(DRM_TOPDIR)/drm_stub.c

SRC_DEP:=    


NAME_OBJS = $(patsubst %.S, %.o, $(patsubst %.asm, %.o,\
            $(patsubst %.c, %.o, $(NAME_SRC))))



all: $(NAME).dll 

$(NAME).dll: $(NAME_OBJS) $(FW_BINS) $(SRC_DEP) i915.lds Makefile
	ld -L$(LIBPATH) $(LDFLAGS) -T i915.lds -o $@ $(NAME_OBJS) $(LIBS)  


%.o : %.c $(HFILES) Makefile
	$(CC) $(CFLAGS) -o $@ $<

%.o : %.S $(HFILES) Makefile
	as -o $@ $<


clean:
	-rm -f ../*/*.o


