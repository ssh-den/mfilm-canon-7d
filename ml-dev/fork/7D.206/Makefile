MODULE_NAME = filmos
TOP_DIR = ../..
CORE_BUILD = $(TOP_DIR)/platform/7D.206/build
FILMOS_SOURCES = filmroll fos_gfx fos_film fos_ui fos_shelf fos_pages fos_hud fos_print fos_dark fos_dev fos_lab fos_looks fos_jpeg
MODULE_OBJS = $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(FILMOS_SOURCES)))
MODULE_OBJS += $(BUILD_DIR)/fos-libm.a $(CORE_BUILD)/gcc-libgcc.a
MODULE_CFLAGS += -I$(TOP_DIR)/platform/7D.206 -I$(TOP_DIR)/platform/7D.206/include \
    -DCONFIG_7D=1 -DCONFIG_7D_206=1 -DCONFIG_FW_VERSION=206 -DCONFIG_7D206_FULL_CORE \
    -DFEATURE_FILMOS_MODULE -include compat.h

include $(TOP_DIR)/modules/Makefile
.DEFAULT_GOAL := $(BUILD_DIR)/module_complete

# GCC 14 can keep fos_copy out of line under -Os.
$(BUILD_DIR)/fos_dark.o: CFLAGS += -Wno-error=inline

$(BUILD_DIR)/fos-libm.a: $(CORE_BUILD)/newlib-libm.a | $(BUILD_DIR)
	$(OBJCOPY) --redefine-sym __errno=fos_errno $< $@

# Keep the arithmetic helpers local and drop unwind sections TCC cannot load.
$(BUILD_DIR)/filmos.mo: $(MODULE_OBJS) Makefile | $(BUILD_DIR)
	$(LD) -r -o $@.tmp $(MODULE_OBJS)
	$(OBJCOPY) --wildcard --keep-global-symbol='__module_*' --keep-global-symbol=filmos_overlays \
	    --remove-section='.ARM.exidx*' --remove-section='.rel.ARM.exidx*' \
	    --remove-section='.ARM.extab*' --remove-section='.rel.ARM.extab*' $@.tmp $@
	$(STRIP) --strip-debug --strip-unneeded --discard-all --discard-locals $@
