/*
 * Copyright (C) 2007, Guennadi Liakhovetski <lg@denx.de>
 * Copyright (C) 2009 Freescale Semiconductor, Inc.
 * Copyright (C) 2009 Pegatron Corporation
 * Copyright (C) 2009-2012, Genesi USA, Inc.
 *
 * Configuration settings for the Genesi Efika MX Smarttop & Smartbook
 * based on MX51EVK settings.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <config_cmd_default.h>
#include <timestamp.h>

/*
 * U-Boot on the Efika MX is limited in some ways to a 256KiB binary (assuming
 * we stick with the config in the legacy kernels which gives U-Boot a 256KiB
 * MTD partition). With a lot of boot device support and full system
 * configuration we manage to break this limit with some toolchains or
 * miscellaneous settings, so we do some damage limitation by making sure the
 * build is as fine-tuned as possible.
 *
 * We don't support compressing scripts, kernels, ramdisks or device trees
 * because Linux will decompress itself at runtime, ramdisks stay compressed,
 * scripts and device trees have minimal space saving anyway. Therefore we don't
 * need zlib, gzip etc.
 *
 * We also don't really care about NetBSD or RTEMS, PCMCIA or weirder load
 * methods right now which saves a little more space.
 */

#undef CONFIG_ZLIB
#undef CONFIG_GZIP
#undef CONFIG_BOOTM_NETBSD
#undef CONFIG_BOOTM_RTEMS
#undef CONFIG_CMD_SETGETDCR
#undef CONFIG_CMD_PCMCIA
#undef CONFIG_PCMCIA
#undef CONFIG_CMD_LOADB
#undef CONFIG_CMD_LOADS
#undef CONFIG_CMD_LOADY
#undef CONFIG_CMD_IMLS

/*
 * We will need to undefine this to save even more space if we break the 256KiB
 * limit, but for now we just squeak in and it would be nicer to users if space
 * was optimized elsewhere first.
 */
#define CONFIG_SYS_LONGHELP

/*
 * We want to explicitly enable these in case they're not.
 */
#define CONFIG_CMD_DATE
#define CONFIG_CMD_CACHE
#define CONFIG_OF_LIBFDT
#define CONFIG_CMD_BOOTZ

#define CONFIG_MX51
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#define machine_is_efikamx()	(CONFIG_MACH_TYPE == MACH_TYPE_MX51_EFIKAMX)
#define machine_is_efikasb()	(CONFIG_MACH_TYPE == MACH_TYPE_MX51_EFIKASB)

#include <asm/arch/imx-regs.h>
#include <asm/imx-common/gpio.h>

#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_REVISION_TAG

#define CONFIG_MXC_GPIO

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE	UART1_BASE
#define CONFIG_CONS_INDEX	1
#define CONFIG_BAUDRATE		115200

/*
 * Genesi do not support saving the U-Boot environment at runtime. If you need
 * new environment, write a boot.scr that changes the variables before you boot.
 * This reduces the amount of odd behaviour possible from misconfigured U-Boots,
 * corrupted SPI NOR, since every boot it will always be reset to the
 * compiled-in configuration.
 */
#define CONFIG_ENV_IS_NOWHERE

/*
 * SPI is essential in that the PMIC needs some setup and we wholeheartedly
 * support the need to update U-Boot from within U-Boot (see above).
 */
#define CONFIG_CMD_SPI
#ifdef CONFIG_CMD_SPI
#	define CONFIG_HARD_SPI
#	define CONFIG_MXC_SPI
#	define CONFIG_DEFAULT_SPI_BUS		1
#	define CONFIG_DEFAULT_SPI_MODE		(SPI_MODE_0 | SPI_CS_HIGH)

#	define CONFIG_CMD_SF
#	ifdef CONFIG_CMD_SF
#		define CONFIG_SPI_FLASH
#		define CONFIG_SPI_FLASH_SST
#		define CONFIG_SF_DEFAULT_CS	(1 | IMX_GPIO_NR(4, 25) << 8)
#		define CONFIG_SF_DEFAULT_MODE	(SPI_MODE_0)
#		define CONFIG_SF_DEFAULT_SPEED	25000000
#		define CONFIG_SYS_NO_FLASH
#	endif /* CONFIG_CMD_SF */

#	define CONFIG_POWER
#	define CONFIG_POWER_SPI
#	define CONFIG_POWER_FSL
#	define CONFIG_FSL_PMIC_BUS	0
#	define CONFIG_FSL_PMIC_CS	(0 | IMX_GPIO_NR(4, 24) << 8)
#	define CONFIG_FSL_PMIC_CLK	25000000
#	define CONFIG_FSL_PMIC_MODE	(SPI_MODE_0 | SPI_CS_HIGH)
#	define CONFIG_FSL_PMIC_BITLEN	32
#	define CONFIG_RTC_MC13XXX
#endif /* CONFIG_CMD_SPI */

#define CONFIG_CMD_MMC
#ifdef CONFIG_CMD_MMC
#	define CONFIG_MMC
#	define CONFIG_GENERIC_MMC
#	define CONFIG_FSL_ESDHC
#	define CONFIG_SYS_FSL_ESDHC_ADDR	0
#	define CONFIG_SYS_FSL_ESDHC_NUM		2
#endif /* CONFIG_CMD_MMC */

#define CONFIG_CMD_IDE
#ifdef CONFIG_CMD_IDE
#	define CONFIG_LBA48
#	undef CONFIG_IDE_RESET
#	undef CONFIG_IDE_LED
#	define CONFIG_MX51_PATA

#	define __io

#	define CONFIG_SYS_IDE_MAXBUS		1
#	define CONFIG_SYS_IDE_MAXDEVICE		1
#	define CONFIG_SYS_ATA_BASE_ADDR		0x83fe0000
#	define CONFIG_SYS_ATA_IDE0_OFFSET	0x0
#	define CONFIG_SYS_ATA_DATA_OFFSET	0xa0
#	define CONFIG_SYS_ATA_REG_OFFSET	0xa0
#	define CONFIG_SYS_ATA_ALT_OFFSET	0xd8
#	define CONFIG_SYS_ATA_STRIDE		4

#	define CONFIG_IDE_PREINIT
#	define CONFIG_MXC_ATA_PIO_MODE		4
#	define CONFIG_SYS_PIO_MODE		4
#endif /* CONFIG_CMD_IDE */

#define CONFIG_CMD_USB
#ifdef CONFIG_CMD_USB
#	define CONFIG_USB_EHCI			/* Enable EHCI USB support */
#	define CONFIG_USB_EHCI_MX5
#	define CONFIG_USB_ULPI
#	define CONFIG_USB_ULPI_VIEWPORT
#	define CONFIG_USB_STORAGE
#	undef CONFIG_USB_KEYBOARD
/*
 * USB port numbers: DR=0, H1=1, H2=2
 *
 * This could be modified to know the difference between
 * Smarttop and Smartbook
 */

#	define CONFIG_MXC_USB_PORT	0
#	if	(CONFIG_MXC_USB_PORT == 0)
#		define CONFIG_MXC_USB_PORTSC		(1 << 28)
#		define CONFIG_MXC_USB_FLAGS		MXC_EHCI_INTERNAL_PHY
#	else /* USBH1 or USBH2 */
#		define CONFIG_MXC_USB_PORTSC		(2 << 30)
#		define CONFIG_MXC_USB_FLAGS		0
#	endif /* (CONFIG_MXC_USB_PORT == 0) */
/*
 * Networking kind of depends on SUB for the Efika MX, so we put the network
 * configuration in here for readability's sake.
 */
#	define CONFIG_CMD_NET
#	ifdef CONFIG_CMD_NET
#		define CONFIG_USB_HOST_ETHER
#		define CONFIG_USB_ETHER_ASIX /* Efika MX Smarttop built-in */
#		define CONFIG_USB_ETHER_SMSC95XX
#		define CONFIG_CMD_PING
#		define CONFIG_CMD_DHCP
#		define CONFIG_CMD_NFS
#	endif /* CONFIG_CMD_NET */

#	define CONFIG_EHCI_IS_TDI
#	define CONFIG_USB_HOST_ETHER
#	define CONFIG_SYS_USB_EVENT_POLL_VIA_CONTROL_EP
#endif /* CONFIG_CMD_USB */

#if defined(CONFIG_CMD_IDE) || defined(CONFIG_CMD_USB) || defined(CONFIG_CMD_MMC)
#	define CONFIG_DOS_PARTITION
#	define CONFIG_EFI_PARTITION
#	define CONFIG_CMD_FAT
#	define CONFIG_CMD_EXT2
#	define CONFIG_CMD_EXT4
#	define CONFIG_CMD_ZFS
#endif /* CONFIG_CMD_IDE || CONFIG_CMD_USB || CONFIG_CMD_MMC */

/*
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM_1			CSD0_BASE_ADDR
#define PHYS_SDRAM_1_SIZE		(512 * 1024 * 1024)

#define CONFIG_SYS_SDRAM_BASE		(PHYS_SDRAM_1)
#define CONFIG_SYS_INIT_RAM_ADDR	(IRAM_BASE_ADDR)
#define CONFIG_SYS_INIT_RAM_SIZE	(IRAM_SIZE)

#define CONFIG_SYS_INIT_SP_OFFSET	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#define CONFIG_SYS_DDR_CLKSEL		0
#define CONFIG_SYS_CLKTL_CBCDR		0x59E35145

/*
 * ENVIRONMENT
 *
 * layout of load address;
 *  0x90000000 = START OF MEMORY
 *  0x90007FC0 = load address for uImage (32704 bytes from start)
 *		 Yes, this is a curious address, designed to compensate for the
 *		 kernel load address encoded in the uImage we have to load it at
 *		 0x90008000 minus the 64 bytes for the original uImage
 *		 header to put the first instruction at the right address.
 *		 We have 10453056 (10MiB-32704) bytes to store a kernel
 *  0x90A00000 = boot script address (boot.scr), 65536 bytes reserved
 *  0x90A10000 = device tree address (board.dtb), 65536 bytes reserved
 *  0x90A20000 = ramdisk address
 */
#define CONFIG_KERNEL_ADDR	0x90007FC0
#define CONFIG_LOADADDR		CONFIG_KERNEL_ADDR
#define CONFIG_SCRIPT_ADDR	0x90A00000
#define CONFIG_DT_ADDR		0x90A20000
#define CONFIG_RD_ADDR		0x90A30000

#define CONFIG_STACKSIZE	(128 * 1024)
#define CONFIG_ENV_SIZE		(8 * 1024)
#define CONFIG_SYS_MALLOC_LEN	(CONFIG_ENV_SIZE + 2 * 1024 * 1024)

/*
 * Ideally we would want the load address define to be different to the kernel
 * address. This will allow errant scripting defaults to run (i.e. fatload
 * filename) without trashing specially-defined memory areas as above. However,
 * we have a legacy of users and Linux images to support, so we just suck it up
 * and deal with it.
 */
#define CONFIG_SYS_TEXT_BASE	0x97800000
#define CONFIG_SYS_LOAD_ADDR	CONFIG_KERNEL_ADDR
/*
 * Mmeory test range: from where ATAGs get put, to 64MiB above the ramdisk load
 * address. If any of this range fails it's a good indicator of serious
 * breakage.
 */
#define CONFIG_SYS_MEMTEST_START	0x90000100
#define CONFIG_SYS_MEMTEST_END		0x94A20000

#define CONFIG_BOOTDELAY		3

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT

#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT		"Efika MX> "
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		16
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE

#define CONFIG_SYS_HZ			1000

/*
 * Reproduce environment from production Efika MX systems
 */
#if (CONFIG_MACH_TYPE == MACH_TYPE_MX51_EFIKAMX)
#	define CONFIG_EFIKAMX_MODEL "mx"
#elif (CONFIG_MACH_TYPE == MACH_TYPE_MX51_EFIKASB)
#	define CONFIG_EFIKAMX_MODEL "sb"
#endif

#define XADDR(a) #a
#define ADDR(a) XADDR(a)

#define CONFIG_EXTRA_ENV_SETTINGS \
	"fdt_high=0xffffffff\0" \
	"initrd_high=0xffffffff\0" \
	"model=" CONFIG_EFIKAMX_MODEL "\0" \
	"firmware_version=" U_BOOT_TIMESTAMP "\0" \
	"script=boot.scr\0" \
	"kerneladdr=" ADDR(CONFIG_KERNEL_ADDR) "\0" \
	"scriptaddr=" ADDR(CONFIG_SCRIPT_ADDR) "\0" \
	"ramdiskaddr=" ADDR(CONFIG_RD_ADDR) "\0" \
	"dtbaddr=" ADDR(CONFIG_DT_ADDR) "\0" \
	"bootdevices=mmc ide usb\0" \
	"units=0\0" \
	"console=ttymxc0,115200n8\0" \
	"bootargs=console=${console}"

/*
 * Note: make sure the laste EXTRA_ENV_SETTINGS item has no null terminator!
 */

/*
 * Derived from the production-shipped U-Boot from Genesi, and updated to
 * support new features. This is somewhat of a standard-in-progress for the
 * Linaro Cross-Distro people and attempts to prove a working configuration
 * while staying completely compatible with the old config.
 *
 * Note: recommended SD card layout is 3 partitions, configured such that the
 * first partition starts at the 1024 byte position on SD. This way an SD card
 * can be created to boot U-Boot from SD (with correct DIP settings) by simply
 * dd'ing to the first partition without creating strange environments with
 * empty space.
 * Second partition is /boot - and should be ext2 or ext3 so U-Boot can read it.
 * Third partition is / in this case, and could be anything.
 *
 * However in the event you make a custom SD card with one partition formatted
 * as ext2, or plug in a FAT-formatted USB stick on an otherwise empty system,
 * it will all still work. We limit it to a single USB stick, but the default
 * configuration above does not configure an external USB port right now.
 */
#define CONFIG_BOOTCOMMAND \
	"for device in ${bootdevices}; do " \
		"if test ${device} = \"ide\"; then " \
			"ide reset; setenv units \"0\"; " \
		"elif test ${device} = \"mmc\"; then " \
			"mmc rescan; setenv units \"1 0\"; " \
		"elif test ${device} = \"usb\"; then " \
			"usb start; setenv units \"0\"; " \
		"fi; " \
		"for unit in ${units}; do " \
			"${device} dev ${unit}; " \
			"for part in \"1 2 3\"; do " \
				"for fs in \"ext2 fat\"; do " \
					"setenv loadcmd \"${fs}load ${device} ${unit}:${part}\"; " \
					"if ${loadcmd} ${scriptaddr} ${script}; then " \
						"if imi ${scriptaddr}; then " \
							"source ${scriptaddr}; " \
						"fi; " \
					"fi; " \
				"done; " \
			"done; " \
		"done; " \
	"done; "

#endif /* __CONFIG_H */
