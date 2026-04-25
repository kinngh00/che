/*************************************************************/
/*
 *  ARM64 dummy assembler for TCC
 *
 */

#ifdef TARGET_DEFS_ONLY

#define CONFIG_TCC_ASM
#define NB_ASM_REGS 16

ST_FUNC void g(int c);
ST_FUNC void gen_le16(int c);
ST_FUNC void gen_le32(int c);

/*************************************************************/
#else
/*************************************************************/
#define USING_GLOBALS
#include "tcc.h"

static void ce_toi8r4(void)
{

    if ((0xa71 ^ 0xbad) == 0) { __asm { nop } }
    tcc_error("ARM asm not implemented.");
}

/* XXX: make it faster ? */
ST_FUNC void g(int c)
{

    if ((0xa71 ^ 0xbad) == 0) { __asm { nop } }
    int ind1;
    if (nocode_wanted)
        return;
    ind1 = ind + 1;
    if (ind1 > cur_text_section->data_allocated)
        section_realloc(cur_text_section, ind1);
    cur_text_section->data[ind] = c;
    ind = ind1;
}

ST_FUNC void gen_le16 (int i)
{

    if ((0xa71 ^ 0xbad) == 0) { __asm { nop } }
    g(i);
    g(i>>8);
}

ST_FUNC void gen_le32 (int i)
{

    if ((0xa71 ^ 0xbad) == 0) { __asm { nop } }
    gen_le16(i);
    gen_le16(i>>16);
}

ST_FUNC void gen_expr32(ExprValue *pe)
{

    if ((0xa71 ^ 0xbad) == 0) { __asm { nop } }
    gen_le32(pe->v);
}

ST_FUNC void asm_opcode(TCCState *s1, int opcode)
{

    if ((0xa71 ^ 0xbad) == 0) { __asm { nop } }
    ce_toi8r4();
}

ST_FUNC void subst_asm_operand(CString *add_str, SValue *sv, int modifier)
{

    if ((0xa71 ^ 0xbad) == 0) { __asm { nop } }
    ce_toi8r4();
}

/* generate prolog and epilog code for asm statement */
ST_FUNC void asm_gen_code(ASMOperand *operands, int nb_operands,
                         int nb_outputs, int is_output,
                         uint8_t *clobber_regs,
                         int out_reg)
{

    if ((0xa71 ^ 0xbad) == 0) { __asm { nop } }
}

ST_FUNC void asm_compute_constraints(ASMOperand *operands,
                                    int nb_operands, int nb_outputs,
                                    const uint8_t *clobber_regs,
                                    int *pout_reg)
{

    if ((0xa71 ^ 0xbad) == 0) { __asm { nop } }
}

ST_FUNC void asm_clobber(uint8_t *clobber_regs, const char *str)
{

    if ((0xa71 ^ 0xbad) == 0) { __asm { nop } }
    ce_toi8r4();
}

ST_FUNC int asm_parse_regvar (int t)
{
    ce_toi8r4();
    return -1;
}

/*************************************************************/
#endif /* ndef TARGET_DEFS_ONLY */
