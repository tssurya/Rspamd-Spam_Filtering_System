
/* This file was generated automatically by the Snowball to ANSI C compiler */

#include "/home/linux/rspamd/rspamd-0.8.1/contrib/snowball/runtime/header.h"

#ifdef __cplusplus
extern "C" {
#endif
extern int russian_KOI8_R_stem(struct SN_env * z);
#ifdef __cplusplus
}
#endif
static int r_tidy_up(struct SN_env * z);
static int r_derivational(struct SN_env * z);
static int r_noun(struct SN_env * z);
static int r_verb(struct SN_env * z);
static int r_reflexive(struct SN_env * z);
static int r_adjectival(struct SN_env * z);
static int r_adjective(struct SN_env * z);
static int r_perfective_gerund(struct SN_env * z);
static int r_R2(struct SN_env * z);
static int r_mark_regions(struct SN_env * z);
#ifdef __cplusplus
extern "C" {
#endif


extern struct SN_env * russian_KOI8_R_create_env(void);
extern void russian_KOI8_R_close_env(struct SN_env * z);


#ifdef __cplusplus
}
#endif
static const symbol s_0_0[6] = { 0xC3, 0x97, 0xC3, 0x9B, 0xC3, 0x89 };
static const symbol s_0_1[8] = { 0xC3, 0x89, 0xC3, 0x97, 0xC3, 0x9B, 0xC3, 0x89 };
static const symbol s_0_2[8] = { 0xC3, 0x99, 0xC3, 0x97, 0xC3, 0x9B, 0xC3, 0x89 };
static const symbol s_0_3[2] = { 0xC3, 0x97 };
static const symbol s_0_4[4] = { 0xC3, 0x89, 0xC3, 0x97 };
static const symbol s_0_5[4] = { 0xC3, 0x99, 0xC3, 0x97 };
static const symbol s_0_6[10] = { 0xC3, 0x97, 0xC3, 0x9B, 0xC3, 0x89, 0xC3, 0x93, 0xC3, 0x98 };
static const symbol s_0_7[12] = { 0xC3, 0x89, 0xC3, 0x97, 0xC3, 0x9B, 0xC3, 0x89, 0xC3, 0x93, 0xC3, 0x98 };
static const symbol s_0_8[12] = { 0xC3, 0x99, 0xC3, 0x97, 0xC3, 0x9B, 0xC3, 0x89, 0xC3, 0x93, 0xC3, 0x98 };

static const struct among a_0[9] =
{
/*  0 */ { 6, s_0_0, -1, 1, 0},
/*  1 */ { 8, s_0_1, 0, 2, 0},
/*  2 */ { 8, s_0_2, 0, 2, 0},
/*  3 */ { 2, s_0_3, -1, 1, 0},
/*  4 */ { 4, s_0_4, 3, 2, 0},
/*  5 */ { 4, s_0_5, 3, 2, 0},
/*  6 */ { 10, s_0_6, -1, 1, 0},
/*  7 */ { 12, s_0_7, 6, 2, 0},
/*  8 */ { 12, s_0_8, 6, 2, 0}
};

static const symbol s_1_0[4] = { 0xC3, 0x80, 0xC3, 0x80 };
static const symbol s_1_1[4] = { 0xC3, 0x85, 0xC3, 0x80 };
static const symbol s_1_2[4] = { 0xC3, 0x8F, 0xC3, 0x80 };
static const symbol s_1_3[4] = { 0xC3, 0x95, 0xC3, 0x80 };
static const symbol s_1_4[4] = { 0xC3, 0x85, 0xC3, 0x85 };
static const symbol s_1_5[4] = { 0xC3, 0x89, 0xC3, 0x85 };
static const symbol s_1_6[4] = { 0xC3, 0x8F, 0xC3, 0x85 };
static const symbol s_1_7[4] = { 0xC3, 0x99, 0xC3, 0x85 };
static const symbol s_1_8[4] = { 0xC3, 0x89, 0xC3, 0x88 };
static const symbol s_1_9[4] = { 0xC3, 0x99, 0xC3, 0x88 };
static const symbol s_1_10[6] = { 0xC3, 0x89, 0xC3, 0x8D, 0xC3, 0x89 };
static const symbol s_1_11[6] = { 0xC3, 0x99, 0xC3, 0x8D, 0xC3, 0x89 };
static const symbol s_1_12[4] = { 0xC3, 0x85, 0xC3, 0x8A };
static const symbol s_1_13[4] = { 0xC3, 0x89, 0xC3, 0x8A };
static const symbol s_1_14[4] = { 0xC3, 0x8F, 0xC3, 0x8A };
static const symbol s_1_15[4] = { 0xC3, 0x99, 0xC3, 0x8A };
static const symbol s_1_16[4] = { 0xC3, 0x85, 0xC3, 0x8D };
static const symbol s_1_17[4] = { 0xC3, 0x89, 0xC3, 0x8D };
static const symbol s_1_18[4] = { 0xC3, 0x8F, 0xC3, 0x8D };
static const symbol s_1_19[4] = { 0xC3, 0x99, 0xC3, 0x8D };
static const symbol s_1_20[6] = { 0xC3, 0x85, 0xC3, 0x87, 0xC3, 0x8F };
static const symbol s_1_21[6] = { 0xC3, 0x8F, 0xC3, 0x87, 0xC3, 0x8F };
static const symbol s_1_22[4] = { 0xC3, 0x81, 0xC3, 0x91 };
static const symbol s_1_23[4] = { 0xC3, 0x91, 0xC3, 0x91 };
static const symbol s_1_24[6] = { 0xC3, 0x85, 0xC3, 0x8D, 0xC3, 0x95 };
static const symbol s_1_25[6] = { 0xC3, 0x8F, 0xC3, 0x8D, 0xC3, 0x95 };

static const struct among a_1[26] =
{
/*  0 */ { 4, s_1_0, -1, 1, 0},
/*  1 */ { 4, s_1_1, -1, 1, 0},
/*  2 */ { 4, s_1_2, -1, 1, 0},
/*  3 */ { 4, s_1_3, -1, 1, 0},
/*  4 */ { 4, s_1_4, -1, 1, 0},
/*  5 */ { 4, s_1_5, -1, 1, 0},
/*  6 */ { 4, s_1_6, -1, 1, 0},
/*  7 */ { 4, s_1_7, -1, 1, 0},
/*  8 */ { 4, s_1_8, -1, 1, 0},
/*  9 */ { 4, s_1_9, -1, 1, 0},
/* 10 */ { 6, s_1_10, -1, 1, 0},
/* 11 */ { 6, s_1_11, -1, 1, 0},
/* 12 */ { 4, s_1_12, -1, 1, 0},
/* 13 */ { 4, s_1_13, -1, 1, 0},
/* 14 */ { 4, s_1_14, -1, 1, 0},
/* 15 */ { 4, s_1_15, -1, 1, 0},
/* 16 */ { 4, s_1_16, -1, 1, 0},
/* 17 */ { 4, s_1_17, -1, 1, 0},
/* 18 */ { 4, s_1_18, -1, 1, 0},
/* 19 */ { 4, s_1_19, -1, 1, 0},
/* 20 */ { 6, s_1_20, -1, 1, 0},
/* 21 */ { 6, s_1_21, -1, 1, 0},
/* 22 */ { 4, s_1_22, -1, 1, 0},
/* 23 */ { 4, s_1_23, -1, 1, 0},
/* 24 */ { 6, s_1_24, -1, 1, 0},
/* 25 */ { 6, s_1_25, -1, 1, 0}
};

static const symbol s_2_0[4] = { 0xC3, 0x85, 0xC3, 0x8D };
static const symbol s_2_1[4] = { 0xC3, 0x8E, 0xC3, 0x8E };
static const symbol s_2_2[4] = { 0xC3, 0x97, 0xC3, 0x9B };
static const symbol s_2_3[6] = { 0xC3, 0x89, 0xC3, 0x97, 0xC3, 0x9B };
static const symbol s_2_4[6] = { 0xC3, 0x99, 0xC3, 0x97, 0xC3, 0x9B };
static const symbol s_2_5[2] = { 0xC3, 0x9D };
static const symbol s_2_6[4] = { 0xC3, 0x80, 0xC3, 0x9D };
static const symbol s_2_7[6] = { 0xC3, 0x95, 0xC3, 0x80, 0xC3, 0x9D };

static const struct among a_2[8] =
{
/*  0 */ { 4, s_2_0, -1, 1, 0},
/*  1 */ { 4, s_2_1, -1, 1, 0},
/*  2 */ { 4, s_2_2, -1, 1, 0},
/*  3 */ { 6, s_2_3, 2, 2, 0},
/*  4 */ { 6, s_2_4, 2, 2, 0},
/*  5 */ { 2, s_2_5, -1, 1, 0},
/*  6 */ { 4, s_2_6, 5, 1, 0},
/*  7 */ { 6, s_2_7, 6, 2, 0}
};

static const symbol s_3_0[4] = { 0xC3, 0x93, 0xC3, 0x91 };
static const symbol s_3_1[4] = { 0xC3, 0x93, 0xC3, 0x98 };

static const struct among a_3[2] =
{
/*  0 */ { 4, s_3_0, -1, 1, 0},
/*  1 */ { 4, s_3_1, -1, 1, 0}
};

static const symbol s_4_0[2] = { 0xC3, 0x80 };
static const symbol s_4_1[4] = { 0xC3, 0x95, 0xC3, 0x80 };
static const symbol s_4_2[4] = { 0xC3, 0x8C, 0xC3, 0x81 };
static const symbol s_4_3[6] = { 0xC3, 0x89, 0xC3, 0x8C, 0xC3, 0x81 };
static const symbol s_4_4[6] = { 0xC3, 0x99, 0xC3, 0x8C, 0xC3, 0x81 };
static const symbol s_4_5[4] = { 0xC3, 0x8E, 0xC3, 0x81 };
static const symbol s_4_6[6] = { 0xC3, 0x85, 0xC3, 0x8E, 0xC3, 0x81 };
static const symbol s_4_7[6] = { 0xC3, 0x85, 0xC3, 0x94, 0xC3, 0x85 };
static const symbol s_4_8[6] = { 0xC3, 0x89, 0xC3, 0x94, 0xC3, 0x85 };
static const symbol s_4_9[6] = { 0xC3, 0x8A, 0xC3, 0x94, 0xC3, 0x85 };
static const symbol s_4_10[8] = { 0xC3, 0x85, 0xC3, 0x8A, 0xC3, 0x94, 0xC3, 0x85 };
static const symbol s_4_11[8] = { 0xC3, 0x95, 0xC3, 0x8A, 0xC3, 0x94, 0xC3, 0x85 };
static const symbol s_4_12[4] = { 0xC3, 0x8C, 0xC3, 0x89 };
static const symbol s_4_13[6] = { 0xC3, 0x89, 0xC3, 0x8C, 0xC3, 0x89 };
static const symbol s_4_14[6] = { 0xC3, 0x99, 0xC3, 0x8C, 0xC3, 0x89 };
static const symbol s_4_15[2] = { 0xC3, 0x8A };
static const symbol s_4_16[4] = { 0xC3, 0x85, 0xC3, 0x8A };
static const symbol s_4_17[4] = { 0xC3, 0x95, 0xC3, 0x8A };
static const symbol s_4_18[2] = { 0xC3, 0x8C };
static const symbol s_4_19[4] = { 0xC3, 0x89, 0xC3, 0x8C };
static const symbol s_4_20[4] = { 0xC3, 0x99, 0xC3, 0x8C };
static const symbol s_4_21[4] = { 0xC3, 0x85, 0xC3, 0x8D };
static const symbol s_4_22[4] = { 0xC3, 0x89, 0xC3, 0x8D };
static const symbol s_4_23[4] = { 0xC3, 0x99, 0xC3, 0x8D };
static const symbol s_4_24[2] = { 0xC3, 0x8E };
static const symbol s_4_25[4] = { 0xC3, 0x85, 0xC3, 0x8E };
static const symbol s_4_26[4] = { 0xC3, 0x8C, 0xC3, 0x8F };
static const symbol s_4_27[6] = { 0xC3, 0x89, 0xC3, 0x8C, 0xC3, 0x8F };
static const symbol s_4_28[6] = { 0xC3, 0x99, 0xC3, 0x8C, 0xC3, 0x8F };
static const symbol s_4_29[4] = { 0xC3, 0x8E, 0xC3, 0x8F };
static const symbol s_4_30[6] = { 0xC3, 0x85, 0xC3, 0x8E, 0xC3, 0x8F };
static const symbol s_4_31[6] = { 0xC3, 0x8E, 0xC3, 0x8E, 0xC3, 0x8F };
static const symbol s_4_32[4] = { 0xC3, 0x80, 0xC3, 0x94 };
static const symbol s_4_33[6] = { 0xC3, 0x95, 0xC3, 0x80, 0xC3, 0x94 };
static const symbol s_4_34[4] = { 0xC3, 0x85, 0xC3, 0x94 };
static const symbol s_4_35[6] = { 0xC3, 0x95, 0xC3, 0x85, 0xC3, 0x94 };
static const symbol s_4_36[4] = { 0xC3, 0x89, 0xC3, 0x94 };
static const symbol s_4_37[4] = { 0xC3, 0x91, 0xC3, 0x94 };
static const symbol s_4_38[4] = { 0xC3, 0x99, 0xC3, 0x94 };
static const symbol s_4_39[4] = { 0xC3, 0x94, 0xC3, 0x98 };
static const symbol s_4_40[6] = { 0xC3, 0x89, 0xC3, 0x94, 0xC3, 0x98 };
static const symbol s_4_41[6] = { 0xC3, 0x99, 0xC3, 0x94, 0xC3, 0x98 };
static const symbol s_4_42[6] = { 0xC3, 0x85, 0xC3, 0x9B, 0xC3, 0x98 };
static const symbol s_4_43[6] = { 0xC3, 0x89, 0xC3, 0x9B, 0xC3, 0x98 };
static const symbol s_4_44[4] = { 0xC3, 0x8E, 0xC3, 0x99 };
static const symbol s_4_45[6] = { 0xC3, 0x85, 0xC3, 0x8E, 0xC3, 0x99 };

static const struct among a_4[46] =
{
/*  0 */ { 2, s_4_0, -1, 2, 0},
/*  1 */ { 4, s_4_1, 0, 2, 0},
/*  2 */ { 4, s_4_2, -1, 1, 0},
/*  3 */ { 6, s_4_3, 2, 2, 0},
/*  4 */ { 6, s_4_4, 2, 2, 0},
/*  5 */ { 4, s_4_5, -1, 1, 0},
/*  6 */ { 6, s_4_6, 5, 2, 0},
/*  7 */ { 6, s_4_7, -1, 1, 0},
/*  8 */ { 6, s_4_8, -1, 2, 0},
/*  9 */ { 6, s_4_9, -1, 1, 0},
/* 10 */ { 8, s_4_10, 9, 2, 0},
/* 11 */ { 8, s_4_11, 9, 2, 0},
/* 12 */ { 4, s_4_12, -1, 1, 0},
/* 13 */ { 6, s_4_13, 12, 2, 0},
/* 14 */ { 6, s_4_14, 12, 2, 0},
/* 15 */ { 2, s_4_15, -1, 1, 0},
/* 16 */ { 4, s_4_16, 15, 2, 0},
/* 17 */ { 4, s_4_17, 15, 2, 0},
/* 18 */ { 2, s_4_18, -1, 1, 0},
/* 19 */ { 4, s_4_19, 18, 2, 0},
/* 20 */ { 4, s_4_20, 18, 2, 0},
/* 21 */ { 4, s_4_21, -1, 1, 0},
/* 22 */ { 4, s_4_22, -1, 2, 0},
/* 23 */ { 4, s_4_23, -1, 2, 0},
/* 24 */ { 2, s_4_24, -1, 1, 0},
/* 25 */ { 4, s_4_25, 24, 2, 0},
/* 26 */ { 4, s_4_26, -1, 1, 0},
/* 27 */ { 6, s_4_27, 26, 2, 0},
/* 28 */ { 6, s_4_28, 26, 2, 0},
/* 29 */ { 4, s_4_29, -1, 1, 0},
/* 30 */ { 6, s_4_30, 29, 2, 0},
/* 31 */ { 6, s_4_31, 29, 1, 0},
/* 32 */ { 4, s_4_32, -1, 1, 0},
/* 33 */ { 6, s_4_33, 32, 2, 0},
/* 34 */ { 4, s_4_34, -1, 1, 0},
/* 35 */ { 6, s_4_35, 34, 2, 0},
/* 36 */ { 4, s_4_36, -1, 2, 0},
/* 37 */ { 4, s_4_37, -1, 2, 0},
/* 38 */ { 4, s_4_38, -1, 2, 0},
/* 39 */ { 4, s_4_39, -1, 1, 0},
/* 40 */ { 6, s_4_40, 39, 2, 0},
/* 41 */ { 6, s_4_41, 39, 2, 0},
/* 42 */ { 6, s_4_42, -1, 1, 0},
/* 43 */ { 6, s_4_43, -1, 2, 0},
/* 44 */ { 4, s_4_44, -1, 1, 0},
/* 45 */ { 6, s_4_45, 44, 2, 0}
};

static const symbol s_5_0[2] = { 0xC3, 0x80 };
static const symbol s_5_1[4] = { 0xC3, 0x89, 0xC3, 0x80 };
static const symbol s_5_2[4] = { 0xC3, 0x98, 0xC3, 0x80 };
static const symbol s_5_3[2] = { 0xC3, 0x81 };
static const symbol s_5_4[2] = { 0xC3, 0x85 };
static const symbol s_5_5[4] = { 0xC3, 0x89, 0xC3, 0x85 };
static const symbol s_5_6[4] = { 0xC3, 0x98, 0xC3, 0x85 };
static const symbol s_5_7[4] = { 0xC3, 0x81, 0xC3, 0x88 };
static const symbol s_5_8[4] = { 0xC3, 0x91, 0xC3, 0x88 };
static const symbol s_5_9[6] = { 0xC3, 0x89, 0xC3, 0x91, 0xC3, 0x88 };
static const symbol s_5_10[2] = { 0xC3, 0x89 };
static const symbol s_5_11[4] = { 0xC3, 0x85, 0xC3, 0x89 };
static const symbol s_5_12[4] = { 0xC3, 0x89, 0xC3, 0x89 };
static const symbol s_5_13[6] = { 0xC3, 0x81, 0xC3, 0x8D, 0xC3, 0x89 };
static const symbol s_5_14[6] = { 0xC3, 0x91, 0xC3, 0x8D, 0xC3, 0x89 };
static const symbol s_5_15[8] = { 0xC3, 0x89, 0xC3, 0x91, 0xC3, 0x8D, 0xC3, 0x89 };
static const symbol s_5_16[2] = { 0xC3, 0x8A };
static const symbol s_5_17[4] = { 0xC3, 0x85, 0xC3, 0x8A };
static const symbol s_5_18[6] = { 0xC3, 0x89, 0xC3, 0x85, 0xC3, 0x8A };
static const symbol s_5_19[4] = { 0xC3, 0x89, 0xC3, 0x8A };
static const symbol s_5_20[4] = { 0xC3, 0x8F, 0xC3, 0x8A };
static const symbol s_5_21[4] = { 0xC3, 0x81, 0xC3, 0x8D };
static const symbol s_5_22[4] = { 0xC3, 0x85, 0xC3, 0x8D };
static const symbol s_5_23[6] = { 0xC3, 0x89, 0xC3, 0x85, 0xC3, 0x8D };
static const symbol s_5_24[4] = { 0xC3, 0x8F, 0xC3, 0x8D };
static const symbol s_5_25[4] = { 0xC3, 0x91, 0xC3, 0x8D };
static const symbol s_5_26[6] = { 0xC3, 0x89, 0xC3, 0x91, 0xC3, 0x8D };
static const symbol s_5_27[2] = { 0xC3, 0x8F };
static const symbol s_5_28[2] = { 0xC3, 0x91 };
static const symbol s_5_29[4] = { 0xC3, 0x89, 0xC3, 0x91 };
static const symbol s_5_30[4] = { 0xC3, 0x98, 0xC3, 0x91 };
static const symbol s_5_31[2] = { 0xC3, 0x95 };
static const symbol s_5_32[4] = { 0xC3, 0x85, 0xC3, 0x97 };
static const symbol s_5_33[4] = { 0xC3, 0x8F, 0xC3, 0x97 };
static const symbol s_5_34[2] = { 0xC3, 0x98 };
static const symbol s_5_35[2] = { 0xC3, 0x99 };

static const struct among a_5[36] =
{
/*  0 */ { 2, s_5_0, -1, 1, 0},
/*  1 */ { 4, s_5_1, 0, 1, 0},
/*  2 */ { 4, s_5_2, 0, 1, 0},
/*  3 */ { 2, s_5_3, -1, 1, 0},
/*  4 */ { 2, s_5_4, -1, 1, 0},
/*  5 */ { 4, s_5_5, 4, 1, 0},
/*  6 */ { 4, s_5_6, 4, 1, 0},
/*  7 */ { 4, s_5_7, -1, 1, 0},
/*  8 */ { 4, s_5_8, -1, 1, 0},
/*  9 */ { 6, s_5_9, 8, 1, 0},
/* 10 */ { 2, s_5_10, -1, 1, 0},
/* 11 */ { 4, s_5_11, 10, 1, 0},
/* 12 */ { 4, s_5_12, 10, 1, 0},
/* 13 */ { 6, s_5_13, 10, 1, 0},
/* 14 */ { 6, s_5_14, 10, 1, 0},
/* 15 */ { 8, s_5_15, 14, 1, 0},
/* 16 */ { 2, s_5_16, -1, 1, 0},
/* 17 */ { 4, s_5_17, 16, 1, 0},
/* 18 */ { 6, s_5_18, 17, 1, 0},
/* 19 */ { 4, s_5_19, 16, 1, 0},
/* 20 */ { 4, s_5_20, 16, 1, 0},
/* 21 */ { 4, s_5_21, -1, 1, 0},
/* 22 */ { 4, s_5_22, -1, 1, 0},
/* 23 */ { 6, s_5_23, 22, 1, 0},
/* 24 */ { 4, s_5_24, -1, 1, 0},
/* 25 */ { 4, s_5_25, -1, 1, 0},
/* 26 */ { 6, s_5_26, 25, 1, 0},
/* 27 */ { 2, s_5_27, -1, 1, 0},
/* 28 */ { 2, s_5_28, -1, 1, 0},
/* 29 */ { 4, s_5_29, 28, 1, 0},
/* 30 */ { 4, s_5_30, 28, 1, 0},
/* 31 */ { 2, s_5_31, -1, 1, 0},
/* 32 */ { 4, s_5_32, -1, 1, 0},
/* 33 */ { 4, s_5_33, -1, 1, 0},
/* 34 */ { 2, s_5_34, -1, 1, 0},
/* 35 */ { 2, s_5_35, -1, 1, 0}
};

static const symbol s_6_0[6] = { 0xC3, 0x8F, 0xC3, 0x93, 0xC3, 0x94 };
static const symbol s_6_1[8] = { 0xC3, 0x8F, 0xC3, 0x93, 0xC3, 0x94, 0xC3, 0x98 };

static const struct among a_6[2] =
{
/*  0 */ { 6, s_6_0, -1, 1, 0},
/*  1 */ { 8, s_6_1, -1, 1, 0}
};

static const symbol s_7_0[8] = { 0xC3, 0x85, 0xC3, 0x8A, 0xC3, 0x9B, 0xC3, 0x85 };
static const symbol s_7_1[2] = { 0xC3, 0x8E };
static const symbol s_7_2[2] = { 0xC3, 0x98 };
static const symbol s_7_3[6] = { 0xC3, 0x85, 0xC3, 0x8A, 0xC3, 0x9B };

static const struct among a_7[4] =
{
/*  0 */ { 8, s_7_0, -1, 1, 0},
/*  1 */ { 2, s_7_1, -1, 2, 0},
/*  2 */ { 2, s_7_2, -1, 3, 0},
/*  3 */ { 6, s_7_3, -1, 1, 0}
};

static const unsigned char g_v[] = { 35, 130, 34, 18 };

static const symbol s_0[] = { 0xC3, 0x81 };
static const symbol s_1[] = { 0xC3, 0x91 };
static const symbol s_2[] = { 0xC3, 0x81 };
static const symbol s_3[] = { 0xC3, 0x91 };
static const symbol s_4[] = { 0xC3, 0x81 };
static const symbol s_5[] = { 0xC3, 0x91 };
static const symbol s_6[] = { 0xC3, 0x8E };
static const symbol s_7[] = { 0xC3, 0x8E };
static const symbol s_8[] = { 0xC3, 0x8E };
static const symbol s_9[] = { 0xC3, 0x89 };

static int r_mark_regions(struct SN_env * z) {
    z->I[0] = z->l; /* $pV = <integer expression>, line 61 */
    z->I[1] = z->l; /* $p2 = <integer expression>, line 62 */
    {   int c1 = z->c; /* do, line 63 */
        {    /* gopast */ /* grouping v, line 64 */
            int ret = out_grouping_U(z, g_v, 192, 220, 1);
            if (ret < 0) goto lab0;
            z->c += ret;
        }
        z->I[0] = z->c; /* setmark pV, line 64 */
        {    /* gopast */ /* non v, line 64 */
            int ret = in_grouping_U(z, g_v, 192, 220, 1);
            if (ret < 0) goto lab0;
            z->c += ret;
        }
        {    /* gopast */ /* grouping v, line 65 */
            int ret = out_grouping_U(z, g_v, 192, 220, 1);
            if (ret < 0) goto lab0;
            z->c += ret;
        }
        {    /* gopast */ /* non v, line 65 */
            int ret = in_grouping_U(z, g_v, 192, 220, 1);
            if (ret < 0) goto lab0;
            z->c += ret;
        }
        z->I[1] = z->c; /* setmark p2, line 65 */
    lab0:
        z->c = c1;
    }
    return 1;
}

static int r_R2(struct SN_env * z) {
    if (!(z->I[1] <= z->c)) return 0; /* $p2 <= <integer expression>, line 71 */
    return 1;
}

static int r_perfective_gerund(struct SN_env * z) {
    int among_var;
    z->ket = z->c; /* [, line 74 */
    if (z->c - 1 <= z->lb || z->p[z->c - 1] >> 5 != 4 || !((25166336 >> (z->p[z->c - 1] & 0x1f)) & 1)) return 0; /* substring, line 74 */
    among_var = find_among_b(z, a_0, 9); /* substring, line 74 */
    if (!(among_var)) return 0;
    z->bra = z->c; /* ], line 74 */
    switch(among_var) {
        case 0: return 0;
        case 1:
            {   int m1 = z->l - z->c; (void)m1; /* or, line 78 */
                if (!(eq_s_b(z, 2, s_0))) goto lab1; /* literal, line 78 */
                goto lab0;
            lab1:
                z->c = z->l - m1;
                if (!(eq_s_b(z, 2, s_1))) return 0; /* literal, line 78 */
            }
        lab0:
            {   int ret = slice_del(z); /* delete, line 78 */
                if (ret < 0) return ret;
            }
            break;
        case 2:
            {   int ret = slice_del(z); /* delete, line 85 */
                if (ret < 0) return ret;
            }
            break;
    }
    return 1;
}

static int r_adjective(struct SN_env * z) {
    int among_var;
    z->ket = z->c; /* [, line 90 */
    if (z->c - 3 <= z->lb || z->p[z->c - 1] >> 5 != 4 || !((2271009 >> (z->p[z->c - 1] & 0x1f)) & 1)) return 0; /* substring, line 90 */
    among_var = find_among_b(z, a_1, 26); /* substring, line 90 */
    if (!(among_var)) return 0;
    z->bra = z->c; /* ], line 90 */
    switch(among_var) {
        case 0: return 0;
        case 1:
            {   int ret = slice_del(z); /* delete, line 99 */
                if (ret < 0) return ret;
            }
            break;
    }
    return 1;
}

static int r_adjectival(struct SN_env * z) {
    int among_var;
    {   int ret = r_adjective(z); /* call adjective, line 104 */
        if (ret == 0) return 0;
        if (ret < 0) return ret;
    }
    {   int m_keep = z->l - z->c;/* (void) m_keep;*/ /* try, line 111 */
        z->ket = z->c; /* [, line 112 */
        if (z->c - 1 <= z->lb || z->p[z->c - 1] >> 5 != 4 || !((671113216 >> (z->p[z->c - 1] & 0x1f)) & 1)) { z->c = z->l - m_keep; goto lab0; } /* substring, line 112 */
        among_var = find_among_b(z, a_2, 8); /* substring, line 112 */
        if (!(among_var)) { z->c = z->l - m_keep; goto lab0; }
        z->bra = z->c; /* ], line 112 */
        switch(among_var) {
            case 0: { z->c = z->l - m_keep; goto lab0; }
            case 1:
                {   int m1 = z->l - z->c; (void)m1; /* or, line 117 */
                    if (!(eq_s_b(z, 2, s_2))) goto lab2; /* literal, line 117 */
                    goto lab1;
                lab2:
                    z->c = z->l - m1;
                    if (!(eq_s_b(z, 2, s_3))) { z->c = z->l - m_keep; goto lab0; } /* literal, line 117 */
                }
            lab1:
                {   int ret = slice_del(z); /* delete, line 117 */
                    if (ret < 0) return ret;
                }
                break;
            case 2:
                {   int ret = slice_del(z); /* delete, line 124 */
                    if (ret < 0) return ret;
                }
                break;
        }
    lab0:
        ;
    }
    return 1;
}

static int r_reflexive(struct SN_env * z) {
    int among_var;
    z->ket = z->c; /* [, line 131 */
    if (z->c - 3 <= z->lb || (z->p[z->c - 1] != 145 && z->p[z->c - 1] != 152)) return 0; /* substring, line 131 */
    among_var = find_among_b(z, a_3, 2); /* substring, line 131 */
    if (!(among_var)) return 0;
    z->bra = z->c; /* ], line 131 */
    switch(among_var) {
        case 0: return 0;
        case 1:
            {   int ret = slice_del(z); /* delete, line 134 */
                if (ret < 0) return ret;
            }
            break;
    }
    return 1;
}

static int r_verb(struct SN_env * z) {
    int among_var;
    z->ket = z->c; /* [, line 139 */
    if (z->c - 1 <= z->lb || z->p[z->c - 1] >> 5 != 4 || !((51443235 >> (z->p[z->c - 1] & 0x1f)) & 1)) return 0; /* substring, line 139 */
    among_var = find_among_b(z, a_4, 46); /* substring, line 139 */
    if (!(among_var)) return 0;
    z->bra = z->c; /* ], line 139 */
    switch(among_var) {
        case 0: return 0;
        case 1:
            {   int m1 = z->l - z->c; (void)m1; /* or, line 145 */
                if (!(eq_s_b(z, 2, s_4))) goto lab1; /* literal, line 145 */
                goto lab0;
            lab1:
                z->c = z->l - m1;
                if (!(eq_s_b(z, 2, s_5))) return 0; /* literal, line 145 */
            }
        lab0:
            {   int ret = slice_del(z); /* delete, line 145 */
                if (ret < 0) return ret;
            }
            break;
        case 2:
            {   int ret = slice_del(z); /* delete, line 153 */
                if (ret < 0) return ret;
            }
            break;
    }
    return 1;
}

static int r_noun(struct SN_env * z) {
    int among_var;
    z->ket = z->c; /* [, line 162 */
    if (z->c - 1 <= z->lb || z->p[z->c - 1] >> 5 != 4 || !((60991267 >> (z->p[z->c - 1] & 0x1f)) & 1)) return 0; /* substring, line 162 */
    among_var = find_among_b(z, a_5, 36); /* substring, line 162 */
    if (!(among_var)) return 0;
    z->bra = z->c; /* ], line 162 */
    switch(among_var) {
        case 0: return 0;
        case 1:
            {   int ret = slice_del(z); /* delete, line 169 */
                if (ret < 0) return ret;
            }
            break;
    }
    return 1;
}

static int r_derivational(struct SN_env * z) {
    int among_var;
    z->ket = z->c; /* [, line 178 */
    if (z->c - 5 <= z->lb || (z->p[z->c - 1] != 148 && z->p[z->c - 1] != 152)) return 0; /* substring, line 178 */
    among_var = find_among_b(z, a_6, 2); /* substring, line 178 */
    if (!(among_var)) return 0;
    z->bra = z->c; /* ], line 178 */
    {   int ret = r_R2(z); /* call R2, line 178 */
        if (ret == 0) return 0;
        if (ret < 0) return ret;
    }
    switch(among_var) {
        case 0: return 0;
        case 1:
            {   int ret = slice_del(z); /* delete, line 181 */
                if (ret < 0) return ret;
            }
            break;
    }
    return 1;
}

static int r_tidy_up(struct SN_env * z) {
    int among_var;
    z->ket = z->c; /* [, line 186 */
    if (z->c - 1 <= z->lb || z->p[z->c - 1] >> 5 != 4 || !((151011360 >> (z->p[z->c - 1] & 0x1f)) & 1)) return 0; /* substring, line 186 */
    among_var = find_among_b(z, a_7, 4); /* substring, line 186 */
    if (!(among_var)) return 0;
    z->bra = z->c; /* ], line 186 */
    switch(among_var) {
        case 0: return 0;
        case 1:
            {   int ret = slice_del(z); /* delete, line 190 */
                if (ret < 0) return ret;
            }
            z->ket = z->c; /* [, line 191 */
            if (!(eq_s_b(z, 2, s_6))) return 0; /* literal, line 191 */
            z->bra = z->c; /* ], line 191 */
            if (!(eq_s_b(z, 2, s_7))) return 0; /* literal, line 191 */
            {   int ret = slice_del(z); /* delete, line 191 */
                if (ret < 0) return ret;
            }
            break;
        case 2:
            if (!(eq_s_b(z, 2, s_8))) return 0; /* literal, line 194 */
            {   int ret = slice_del(z); /* delete, line 194 */
                if (ret < 0) return ret;
            }
            break;
        case 3:
            {   int ret = slice_del(z); /* delete, line 196 */
                if (ret < 0) return ret;
            }
            break;
    }
    return 1;
}

extern int russian_KOI8_R_stem(struct SN_env * z) {
    {   int c1 = z->c; /* do, line 203 */
        {   int ret = r_mark_regions(z); /* call mark_regions, line 203 */
            if (ret == 0) goto lab0;
            if (ret < 0) return ret;
        }
    lab0:
        z->c = c1;
    }
    z->lb = z->c; z->c = z->l; /* backwards, line 204 */

    {   int mlimit; /* setlimit, line 204 */
        int m2 = z->l - z->c; (void)m2;
        if (z->c < z->I[0]) return 0;
        z->c = z->I[0]; /* tomark, line 204 */
        mlimit = z->lb; z->lb = z->c;
        z->c = z->l - m2;
        {   int m3 = z->l - z->c; (void)m3; /* do, line 205 */
            {   int m4 = z->l - z->c; (void)m4; /* or, line 206 */
                {   int ret = r_perfective_gerund(z); /* call perfective_gerund, line 206 */
                    if (ret == 0) goto lab3;
                    if (ret < 0) return ret;
                }
                goto lab2;
            lab3:
                z->c = z->l - m4;
                {   int m_keep = z->l - z->c;/* (void) m_keep;*/ /* try, line 207 */
                    {   int ret = r_reflexive(z); /* call reflexive, line 207 */
                        if (ret == 0) { z->c = z->l - m_keep; goto lab4; }
                        if (ret < 0) return ret;
                    }
                lab4:
                    ;
                }
                {   int m5 = z->l - z->c; (void)m5; /* or, line 208 */
                    {   int ret = r_adjectival(z); /* call adjectival, line 208 */
                        if (ret == 0) goto lab6;
                        if (ret < 0) return ret;
                    }
                    goto lab5;
                lab6:
                    z->c = z->l - m5;
                    {   int ret = r_verb(z); /* call verb, line 208 */
                        if (ret == 0) goto lab7;
                        if (ret < 0) return ret;
                    }
                    goto lab5;
                lab7:
                    z->c = z->l - m5;
                    {   int ret = r_noun(z); /* call noun, line 208 */
                        if (ret == 0) goto lab1;
                        if (ret < 0) return ret;
                    }
                }
            lab5:
                ;
            }
        lab2:
        lab1:
            z->c = z->l - m3;
        }
        {   int m_keep = z->l - z->c;/* (void) m_keep;*/ /* try, line 211 */
            z->ket = z->c; /* [, line 211 */
            if (!(eq_s_b(z, 2, s_9))) { z->c = z->l - m_keep; goto lab8; } /* literal, line 211 */
            z->bra = z->c; /* ], line 211 */
            {   int ret = slice_del(z); /* delete, line 211 */
                if (ret < 0) return ret;
            }
        lab8:
            ;
        }
        {   int m6 = z->l - z->c; (void)m6; /* do, line 214 */
            {   int ret = r_derivational(z); /* call derivational, line 214 */
                if (ret == 0) goto lab9;
                if (ret < 0) return ret;
            }
        lab9:
            z->c = z->l - m6;
        }
        {   int m7 = z->l - z->c; (void)m7; /* do, line 215 */
            {   int ret = r_tidy_up(z); /* call tidy_up, line 215 */
                if (ret == 0) goto lab10;
                if (ret < 0) return ret;
            }
        lab10:
            z->c = z->l - m7;
        }
        z->lb = mlimit;
    }
    z->c = z->lb;
    return 1;
}

extern struct SN_env * russian_KOI8_R_create_env(void) { return SN_create_env(0, 2, 0); }

extern void russian_KOI8_R_close_env(struct SN_env * z) { SN_close_env(z, 0); }

