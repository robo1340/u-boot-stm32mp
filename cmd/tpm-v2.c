// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2018 Bootlin
 * Author: Miquel Raynal <miquel.raynal@bootlin.com>
 */

#include <common.h>
#include <dm.h>
#include <log.h>
#include <tpm-common.h>
#include <tpm-v2.h>
#include "tpm-user-utils.h"

static int do_tpm2_startup(cmd_tbl_t *cmdtp, int flag, int argc,
			   char * const argv[])
{
	enum tpm2_startup_types mode;

	if (argc != 2)
		return CMD_RET_USAGE;

	if (!strcasecmp("TPM2_SU_CLEAR", argv[1])) {
		mode = TPM2_SU_CLEAR;
	} else if (!strcasecmp("TPM2_SU_STATE", argv[1])) {
		mode = TPM2_SU_STATE;
	} else {
		printf("Couldn't recognize mode string: %s\n", argv[1]);
		return CMD_RET_FAILURE;
	}

	return report_return_code(tpm2_startup(mode));
}

static cmd_tbl_t tpm2_commands[] = {
	U_BOOT_CMD_MKENT(info, 0, 1, do_tpm_info, "", ""),
	U_BOOT_CMD_MKENT(init, 0, 1, do_tpm_init, "", ""),
	U_BOOT_CMD_MKENT(startup, 0, 1, do_tpm2_startup, "", ""),
};

cmd_tbl_t *get_tpm_commands(unsigned int *size)
{
	*size = ARRAY_SIZE(tpm2_commands);

	return tpm2_commands;
}

U_BOOT_CMD(tpm, CONFIG_SYS_MAXARGS, 1, do_tpm, "Issue a TPMv2.x command",
"<command> [<arguments>]\n"
"\n"
"info\n"
"    Show information about the TPM.\n"
"init\n"
"    Initialize the software stack. Always the first command to issue.\n"
"startup <mode>\n"
"    Issue a TPM2_Startup command.\n"
"    <mode> is one of:\n"
"        * TPM2_SU_CLEAR (reset state)\n"
"        * TPM2_SU_STATE (preserved state)\n"
);