/*
 * linux/drivers/misc/jz_security/common_func.c - Ingenic security driver
 *
 * Copyright (C) 2015 Ingenic Semiconductor Co., Ltd.
 * Author: liu yang <king.lyang@ingenic.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include "jz_security.h"
#include "pdma.h"
#include "secall.h"
#include "cpu_aes.h"

static void pdma_wait(void)
{
	__asm__ volatile (
			".set push\n\t"
			".set noreorder\n\t"
			".set mips32\n\t"
			"li $26, 0          \n\t"
			"mtc0   $26, $12        \n\t"
			"nop                \n\t"
			"1:             \n\t"
			"wait               \n\t"
			"b  1b          \n\t"
			"nop                \n\t"
			".set reorder           \n\t"
			".set pop           \n\t"
			);
}

static void load_pdma_firmware(void)
{
	int i;
	unsigned int *pdma_ins = (unsigned int *)pdma_wait;
	unsigned int *dst_ptr = (unsigned int *)(TCSM_BANK0);//cacheable

	 for (i = 0; i < 6; i++)
	         dst_ptr[i] = pdma_ins[i];
}

int init_seboot(void)
{
	struct sc_args *args;
	args = (struct sc_args *)GET_SC_ARGS();

	reset_mcu();
	load_pdma_firmware();
	boot_up_mcu();

	secall(args,SC_FUNC_TEST,0);

	return 0;
}

int change_aes_key(struct change_key_param *key_param)
{

	int ret = 0;
	int i = 0;
	struct sc_args *args = (struct sc_args *)GET_SC_ARGS();
	args->arg[0] = MCU_TCSM_PADDR(MCU_TCSM_NKU1);
	args->arg[1] = key_param->len;
	args->arg[2] = MCU_TCSM_PADDR(MCU_TCSM_INDATA);
	args->arg[3] = MCU_TCSM_PADDR(MCU_TCSM_OUTDATA);

	unsigned int *inku = (unsigned int *)(MCU_TCSM_NKU1);
	unsigned int *input_aes_packet = (unsigned int *)(MCU_TCSM_INDATA);

	inku[0] = RSA_WORD_LEN * 32;
	inku[1] = RSA_WORD_LEN * 32;
	for(i=0;i<RSA_WORD_LEN * 2;i++) {
		inku[i+2] = key_param->nku_kr[i]; /*load nku or nkr*/
	}

	for(i = 0; i < 31; i++) { //len=31
		input_aes_packet[i] = key_param->rsa_enc_data[i];
	}
	args->arg[1] |= (key_param->init_mode<<31); /*call for init*/
	ret = secall(args,SC_FUNC_INTERNAL_AES_CHANGE,0);

	ret = *(volatile unsigned int *)MCU_TCSM_RETVAL;
	ret &= 0xffff;
	if(ret != 0) {
		debug_printk("xxxx internal rsa faild!! !!!!! ret :%d \n", ret);
	}

	ret = secall(args,SC_FUNC_INTERNAL_AES_INIT,0);

	return ret;
}

int aes_use_internal_key(void *input, void *output, unsigned int len, unsigned int key, unsigned int crypt)
{
	unsigned int ret;
	struct sc_args *args = (struct sc_args *)GET_SC_ARGS();

	args->arg[0] = AES_BY_UKEY | crypt; //key: uk 1 or ck 2
	args->arg[2] = MCU_TCSM_PADDR(input); //input
	args->arg[3] = MCU_TCSM_PADDR(output); //output
	args->arg[4] = len; //len

	ret = secall(args, SC_FUNC_AESBYKEY, 0);

	return ret;
}
int do_rsa(struct rsa_param * rsa_p)
{
	int ret, i;
	struct sc_args *args;
	args = (struct sc_args *)GET_SC_ARGS();

	args->arg[0] = rsa_p->input_len;
	args->arg[1] = rsa_p->key_len;
	args->arg[2] = rsa_p->n_len;
	args->arg[3] = MCU_TCSM_PADDR(MCU_TCSM_INDATA + 0x200);
	args->arg[4] = MCU_TCSM_PADDR(MCU_TCSM_INDATA);
	args->arg[5] = MCU_TCSM_PADDR(MCU_TCSM_INDATA + 0x80);
	args->arg[6] = MCU_TCSM_PADDR(MCU_TCSM_INDATA + 0x100);
	args->arg[7] = MCU_TCSM_PADDR(MCU_TCSM_INDATA + 0x180);
	unsigned int *input = (unsigned int *)(MCU_TCSM_INDATA);
	unsigned int *key = (unsigned int *)(MCU_TCSM_INDATA + 0x80);
	unsigned int *n = (unsigned int *)(MCU_TCSM_INDATA + 0x100);
	unsigned int *output = (unsigned int *)(MCU_TCSM_INDATA + 0x180);
	unsigned int *output_len = (unsigned int *)(MCU_TCSM_INDATA + 0x200);

	for(i = 0; i < rsa_p->input_len; i++)
		input[i]= rsa_p->input[i];

	for(i = 0; i < rsa_p->key_len; i++)
		key[i] = rsa_p->key[i];

	for(i = 0; i < rsa_p->n_len; i++)
		n[i] = rsa_p->n[i];

	ret = secall(args,SC_FUNC_RSA,0);
	args->arg[0] = output_len[0];
	for(i = 0; i < args->arg[0]; i++){
		rsa_p->output[i]=output[i];
	}
	return ret;
}

void dump_aes_regs(void)
{

 debug_printk("AES_REG_ASCR:0x%08x\n", *(volatile unsigned int *)AES_REG_ASCR);
 debug_printk("AES_REG_ASSR:0x%08x\n", *(volatile unsigned int *)AES_REG_ASSR);
 debug_printk("AES_REG_ASIN:0x%08x\n", *(volatile unsigned int *)AES_REG_ASINTM);
 debug_printk("AES_REG_ASSA:0x%08x\n", *(volatile unsigned int *)AES_REG_ASSA);
 debug_printk("AES_REG_ASDA:0x%08x\n", *(volatile unsigned int *)AES_REG_ASDA);
 debug_printk("AES_REG_ASTC:0x%08x\n", *(volatile unsigned int *)AES_REG_ASTC);
 debug_printk("AES_REG_ASDI:0x%08x\n", *(volatile unsigned int *)AES_REG_ASDI);
 debug_printk("AES_REG_ASDO:0x%08x\n", *(volatile unsigned int *)AES_REG_ASDO);
 debug_printk("AES_REG_ASKY:0x%08x\n", *(volatile unsigned int *)AES_REG_ASKY);
 debug_printk("AES_REG_ASIV:0x%08x\n", *(volatile unsigned int *)AES_REG_ASIV);

}

int cpu_do_aes(struct aes_param * aes_p, unsigned int dmamode,  unsigned int cbcmode)
{
	int i;

	//dump_aes_regs();
	int cpumode = (dmamode == 1)?0:1;

	//setup cbcmpde
	if (cbcmode){
		/*set up iv*/
		for(i=0; i < 4; i++) {
			REG32(AES_REG_ASIV) = aes_p->iv[i];
		}

		REG32(AES_REG_ASCR) |= (0x1 << ASCR_EN_SFT_BIT) |
			(0x1 << ASCR_INITIV_SFT_BIT) |
			(aes_p->crypt << ASCR_DECE_SFT_BIT) |
			(cbcmode << ASCR_MODE_SFT_BIT) |
			(dmamode << ASCR_DMAE_SFT_BIT);
	}

	/*do not clear keys setted by mcu*/
	REG32(AES_REG_ASCR) |= (0x1 << ASCR_EN_SFT_BIT) |
		(0x1 << ASCR_KEYS_SFT_BIT) |
		(aes_p->crypt << ASCR_DECE_SFT_BIT) |
		(cbcmode << ASCR_MODE_SFT_BIT) |
		(dmamode << ASCR_DMAE_SFT_BIT);

	while ((REG32(AES_REG_ASSR) & AES_KEYDONE) == 0x0);

	REG32(AES_REG_ASSR) = AES_KEYDONE;

	if (dmamode){
		dma_cache_sync(NULL,(void *)(aes_p->input),aes_p->len, DMA_TO_DEVICE);
		REG32(AES_REG_ASSA) = (unsigned int )aes_p->input & 0x1fffffff;
		REG32(AES_REG_ASDA) = (unsigned int )aes_p->output & 0x1fffffff;
		REG32(AES_REG_ASTC) = aes_p->tc; /*n 128bit*/

	}else{
		for(i=0;i<4;i++){
			REG32(AES_REG_ASDI) = aes_p->input[i];
		}
	}

	REG32(AES_REG_ASCR) |= (0x1 << ASCR_EN_SFT_BIT) |
		(cpumode << ASCR_AESS_SFT_BIT) |
		(aes_p->crypt << ASCR_DECE_SFT_BIT) |
		(cbcmode << ASCR_MODE_SFT_BIT) |
		(dmamode << ASCR_DMAE_SFT_BIT) |
		(dmamode << ASCR_DMAS_SFT_BIT);

	if (dmamode){
		while ((REG32(AES_REG_ASSR) & AES_DMADONE) == 0x0);
		REG32(AES_REG_ASSR) = AES_DMADONE | AES_DONE;
		dma_cache_sync(NULL,(void *)(aes_p->output),aes_p->len, DMA_FROM_DEVICE);
	}else{
		while ((REG32(AES_REG_ASSR) & AES_DONE) == 0x0);

		REG32(AES_REG_ASSR) = AES_DONE;
		for(i=0;i<4;i++) {
			aes_p->output[i] = REG32(AES_REG_ASDO);
		}
	}
	return 0;
}
