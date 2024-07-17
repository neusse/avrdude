/*
 * AVRDUDE - A Downloader/Uploader for AVR device programmers
 * Copyright (C) 2024 Stefan Rueger <stefan.rueger@urclocks.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>

#include "libavrdude.h"

const AVR_opcode_data avr_opcodes[164] = {
  // Arithmetic and Logic Instructions
  {0xfc00, 0x0c00, 1, "0000 11rd dddd rrrr", OP_AVR1,    OPCODE_add, "add", "Rd, Rr", "Add without Carry", "Rd <-- Rd+Rr", "Z,C,N,V,S,H", {"1", "1", "1", "1"}, ""},
  {0xfc00, 0x1c00, 1, "0001 11rd dddd rrrr", OP_AVR1,    OPCODE_adc, "adc", "Rd, Rr", "Add with Carry", "Rd <-- Rd+Rr+C", "Z,C,N,V,S,H", {"1", "1", "1", "1"}, ""},
  {0xff00, 0x9600, 1, "1001 0110 KKdd KKKK", OP_AVR2nRC, OPCODE_adiw, "adiw", "Rd, K", "Add Immediate to Word", "Rd <-- Rd+1:Rd+K", "Z,C,N,V,S", {"2", "2", "2", "n/a"}, "d in {24, 26, 28, 30}"},
  {0xfc00, 0x1800, 1, "0001 10rd dddd rrrr", OP_AVR1,    OPCODE_sub, "sub", "Rd, Rr", "Subtract without Carry", "Rd <-- Rd-Rr", "Z,C,N,V,S,H", {"1", "1", "1", "1"}, ""},
  {0xf000, 0x5000, 1, "0101 KKKK dddd KKKK", OP_AVR1,    OPCODE_subi, "subi", "Rd, K", "Subtract Immediate", "Rd <-- Rd-K", "Z,C,N,V,S,H", {"1", "1", "1", "1"}, ""},
  {0xfc00, 0x0800, 1, "0000 10rd dddd rrrr", OP_AVR1,    OPCODE_sbc, "sbc", "Rd, Rr", "Subtract with Carry", "Rd <-- Rd-Rr-C", "Z,C,N,V,S,H", {"1", "1", "1", "1"}, ""},
  {0xf000, 0x4000, 1, "0100 KKKK dddd KKKK", OP_AVR1,    OPCODE_sbci, "sbci", "Rd, K", "Subtract Immediate with Carry", "Rd <-- Rd-K-C", "Z,C,N,V,S,H", {"1", "1", "1", "1"}, "d=16..31"},
  {0xff00, 0x9700, 1, "1001 0111 KKdd KKKK", OP_AVR2nRC, OPCODE_sbiw, "sbiw", "Rd, K", "Subtract Immediate from Word", "Rd+1:Rd <-- Rd+1:Rd-K", "Z,C,N,V,S", {"2", "2", "2", "n/a"}, "d in {24, 26, 28, 30}"},
  {0xfc00, 0x2000, 1, "0010 00rd dddd rrrr", OP_AVR1,    OPCODE_and, "and", "Rd, Rr", "Logical AND", "Rd <-- Rd & Rr", "Z,N,V,S", {"1", "1", "1", "1"}, ""},
  {0xf000, 0x7000, 1, "0111 KKKK dddd KKKK", OP_AVR1,    OPCODE_andi, "andi", "Rd, K", "Logical AND with Immediate", "Rd <-- Rd & K", "Z,N,V,S", {"1", "1", "1", "1"}, "d = 16..31"},
  {0xfc00, 0x2800, 1, "0010 10rd dddd rrrr", OP_AVR1,    OPCODE_or, "or", "Rd, Rr", "Logical OR", "Rd <-- Rd|Rr", "Z,N,V,S", {"1", "1", "1", "1"}, ""},
  {0xf000, 0x6000, 1, "0110 KKKK dddd KKKK", OP_AVR1,    OPCODE_ori, "ori", "Rd, K", "Logical OR with Immediate", "Rd <-- Rd|K", "Z,N,V,S", {"1", "1", "1", "1"}, ""},
  {0xfc00, 0x2400, 1, "0010 01rd dddd rrrr", OP_AVR1,    OPCODE_eor, "eor", "Rd, Rr", "Exclusive OR", "Rd <-- Rd^Rr", "Z,N,V,S", {"1", "1", "1", "1"}, ""},
  {0xfe0f, 0x9400, 1, "1001 010d dddd 0000", OP_AVR1,    OPCODE_com, "com", "Rd", "One's Complement", "Rd <-- $FF-Rd", "Z,C,N,V,S", {"1", "1", "1", "1"}, ""},
  {0xfe0f, 0x9401, 1, "1001 010d dddd 0001", OP_AVR1,    OPCODE_neg, "neg", "Rd", "Two's Complement", "Rd <-- $00-Rd", "Z,C,N,V,S,H", {"1", "1", "1", "1"}, ""},
  {0xf000, 0x6000, 1, "0110 KKKK dddd KKKK", OP_AVR1,    OPCODE_sbr, "sbr", "Rd, K", "Set Bit(s) in Register", "Rd <-- Rd or K", "Z,N,V,S", {"1", "1", "1", "1"}, "alias for ORI Rd, K"},
  {0xf000, 0x7000, 1, "0111 KKKK dddd KKKK", OP_AVR1,    OPCODE_cbr, "cbr", "Rd, K", "Clear Bit(s) in Register", "Rd <-- Rd & ($FFh-K)", "Z,N,V,S", {"1", "1", "1", "1"}, "alias for ANDI Rd, (0xFF - K)"},
  {0xfe0f, 0x9403, 1, "1001 010d dddd 0011", OP_AVR1,    OPCODE_inc, "inc", "Rd", "Increment", "Rd <-- Rd+1", "Z,N,V,S", {"1", "1", "1", "1"}, ""},
  {0xfe0f, 0x940a, 1, "1001 010d dddd 1010", OP_AVR1,    OPCODE_dec, "dec", "Rd", "Decrement", "Rd <-- Rd-1", "Z,N,V,S", {"1", "1", "1", "1"}, ""},
  {0xfc00, 0x2000, 1, "0010 00dd dddd dddd", OP_AVR1,    OPCODE_tst, "tst", "Rd", "Test for Zero or Minus", "Rd <-- Rd & Rd", "Z,N,V,S", {"1", "1", "1", "1"}, "alias for AND Rd, Rd"},
  {0xfc00, 0x2400, 1, "0010 01dd dddd dddd", OP_AVR1,    OPCODE_clr, "clr", "Rd", "Clear Register", "Rd <-- Rd xor Rd", "Z,N,V,S", {"1", "1", "1", "1"}, "alias for EOR Rd, Rd"},
  {0xff0f, 0xef0f, 1, "1110 1111 dddd 1111", OP_AVR1,    OPCODE_ser, "ser", "Rd", "Set Register", "Rd <-- $FF", "None", {"1", "1", "1", "1"}, "alias for LDI Rd, 0xFF"},
  {0xfc00, 0x9c00, 1, "1001 11rd dddd rrrr", OP_AVR4,    OPCODE_mul, "mul", "Rd, Rr", "Multiply Unsigned", "R1:R0 <-- Rd x Rr (UU)", "Z,C", {"2", "2", "2", "n/a"}, ""},
  {0xff00, 0x0200, 1, "0000 0010 dddd rrrr", OP_AVR4,    OPCODE_muls, "muls", "Rd, Rr", "Multiply Signed", "R1:R0 <-- Rd x Rr (SS)", "Z,C", {"2", "2", "2", "n/a"}, "d, r=16..31"},
  {0xff88, 0x0300, 1, "0000 0011 0ddd 0rrr", OP_AVR4,    OPCODE_mulsu, "mulsu", "Rd, Rr", "Multiply Signed with Unsigned", "R1:R0 <-- Rd x Rr (SU)", "Z,C", {"2", "2", "2", "n/a"}, "d, r=16..23"},
  {0xff88, 0x0308, 1, "0000 0011 0ddd 1rrr", OP_AVR4,    OPCODE_fmul, "fmul", "Rd, Rr", "Fractional Multiply Unsigned", "R1:R0 <-- Rd x Rr<<1 (UU)", "Z,C", {"2", "2", "2", "n/a"}, "r, d = 16..23"},
  {0xff88, 0x0380, 1, "0000 0011 1ddd 0rrr", OP_AVR4,    OPCODE_fmuls, "fmuls", "Rd, Rr", "Fractional Multiply Signed", "R1:R0 <-- Rd x Rr<<1 (SS)", "Z,C", {"2", "2", "2", "n/a"}, "r, d = 16..23"},
  {0xff88, 0x0388, 1, "0000 0011 1ddd 1rrr", OP_AVR4,    OPCODE_fmulsu, "fmulsu", "Rd, Rr", "Fractional Multiply Signed with Unsigned", "R1:R0 <-- Rd x Rr<<1 (SU)", "Z,C", {"2", "2", "2", "n/a"}, "r, d = 16..23"},
  {0xff0f, 0x940b, 1, "1001 0100 KKKK 1011", OP_AVR_XM,  OPCODE_des, "des", "K", "Data Encryption", "if (H = 0) then R15:R0 <-- Encrypt(R15:R0, K) if (H = 1) then R15:R0 <-- Decrypt(R15:R0, K)", "-", {"n/a", "1/2", "n/a", "n/a"}, ""},

  // Branch Instructions
  {0xf000, 0xc000, 1, "1100 kkkk kkkk kkkk", OP_AVR1,    OPCODE_rjmp, "rjmp", "k", "Relative Jump", "PC <-- PC+k+1", "None", {"2", "2", "2", "2"}, ""},
  {0xffff, 0x9409, 1, "1001 0100 0000 1001", OP_AVR2,    OPCODE_ijmp, "ijmp", "Z", "Indirect Jump to (Z)", "PC(15:0) <-- Z, PC(21:16) <-- 0", "None", {"2", "2", "2", "2"}, ""},
  {0xffff, 0x9419, 1, "1001 0100 0001 1001", OP_AVR_XL,  OPCODE_eijmp, "eijmp", "EIND:Z", "Extended Indirect Jump to (Z)", "PC(15:0) <-- Z, PC(21:16) <-- EIND", "None", {"2", "2", "2", "n/a"}, ""},
  {0xfe0e, 0x940c, 2, "1001 010k kkkk 110k  kkkk kkkk kkkk kkkk", OP_AVR_M, OPCODE_jmp, "jmp", "k", "Jump", "PC <-- k", "None", {"3", "3", "3", "n/a"}, ""},
  {0xf000, 0xd000, 1, "1101 kkkk kkkk kkkk", OP_AVR1,    OPCODE_rcall, "rcall", "k", "Relative Call Subroutine", "PC <-- PC+k+1", "None", {"3/4", "2/3", "2/3", "3"}, ""},
  {0xffff, 0x9509, 1, "1001 0101 0000 1001", OP_AVR2,    OPCODE_icall, "icall", "Z", "Indirect Call to (Z)", "PC(15:0) <-- Z, PC(21:16) <-- 0", "None", {"3/4", "2/3", "2/3", "3"}, ""},
  {0xffff, 0x9519, 1, "1001 0101 0001 1001", OP_AVR_XL,  OPCODE_eicall, "eicall", "EIND:Z", "Extended Indirect Call to (Z)", "PC(15:0) <-- Z, PC(21:16) <-- EIND", "None", {"4", "3", "3", "n/a"}, ""},
  {0xfe0e, 0x940e, 2, "1001 010k kkkk 111k  kkkk kkkk kkkk kkkk", OP_AVR_M, OPCODE_call, "call", "k", "call Subroutine", "PC <-- k, STACK <-- PC, SP <-- SP-2", "None", {"4/5", "3/4", "3/4", "n/a"}, ""},
  {0xffff, 0x9508, 1, "1001 0101 0000 1000", OP_AVR1,    OPCODE_ret, "ret", "-", "Subroutine Return", "PC <-- STACK", "None", {"4/5", "4/5", "4/5", "6"}, ""},
  {0xffff, 0x9518, 1, "1001 0101 0001 1000", OP_AVR1,    OPCODE_reti, "reti", "-", "Interrupt Return", "PC <-- STACK", "I", {"4/5", "4/5", "4/5", "6"}, ""},
  {0xfc00, 0x1000, 1, "0001 00rd dddd rrrr", OP_AVR1,    OPCODE_cpse, "cpse", "Rd, Rr", "Compare, Skip if Equal", "if(Rd=Rr)PC <-- PC+2or3", "None", {"1-3", "1-3", "1-3", "1/2"}, ""},
  {0xfc00, 0x1400, 1, "0001 01rd dddd rrrr", OP_AVR1,    OPCODE_cp, "cp", "Rd, Rr", "Compare", "Rd - Rr", "Z,C,N,V,S,H", {"1", "1", "1", "1"}, ""},
  {0xfc00, 0x0400, 1, "0000 01rd dddd rrrr", OP_AVR1,    OPCODE_cpc, "cpc", "Rd, Rr", "Compare with Carry", "Rd - Rr - C", "Z,C,N,V,S,H", {"1", "1", "1", "1"}, ""},
  {0xf000, 0x3000, 1, "0011 KKKK dddd KKKK", OP_AVR1,    OPCODE_cpi, "cpi", "Rd, K", "Compare with Immediate", "Rd - K", "Z,C,N,V,S,H", {"1", "1", "1", "1"}, "d = 16..31"},
  {0xfe08, 0xfc00, 1, "1111 110r rrrr 0bbb", OP_AVR1,    OPCODE_sbrc, "sbrc", "Rr, b", "Skip if Bit in Register Cleared", "if(Rr(b)=0)PC <-- PC+2or3", "None", {"1-3", "1-3", "1-3", "1/2"}, ""},
  {0xfe08, 0xfe00, 1, "1111 111r rrrr 0bbb", OP_AVR1,    OPCODE_sbrs, "sbrs", "Rr, b", "Skip if Bit in Register Set", "if(Rr(b)=1)PC <-- PC+2or3", "None", {"1-3", "1-3", "1-3", "1/2"}, ""},
  {0xff00, 0x9900, 1, "1001 1001 AAAA Abbb", OP_AVR1,    OPCODE_sbic, "sbic", "A, b", "Skip if Bit in I/O Register Cleared", "if(I/O(A,b)=0)PC <-- PC+2or3", "None", {"1-3", "2-4", "1-3", "1/2"}, ""},
  {0xff00, 0x9b00, 1, "1001 1011 AAAA Abbb", OP_AVR1,    OPCODE_sbis, "sbis", "A, b", "Skip if Bit in I/O Register Set", "If(I/O(A,b)=1)PC <-- PC+2or3", "None", {"1-3", "2-4", "1-3", "1/2"}, ""},
  {0xfc07, 0xf000, 1, "1111 00kk kkkk k000", OP_AVR1,    OPCODE_brcs, "brcs", "k", "Branch if Carry Set", "if(C=1)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBS 0, k (C Carry)"},
  {0xfc07, 0xf000, 1, "1111 00kk kkkk k000", OP_AVR1,    OPCODE_brlo, "brlo", "k", "Branch if Lower", "if(C=1)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBS 0, k (C Carry)"},
  {0xfc07, 0xf001, 1, "1111 00kk kkkk k001", OP_AVR1,    OPCODE_breq, "breq", "k", "Branch if Equal", "if(Z=1)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBS 1, k (Z Zero)"},
  {0xfc07, 0xf002, 1, "1111 00kk kkkk k010", OP_AVR1,    OPCODE_brmi, "brmi", "k", "Branch if Minus", "if(N=1)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBS 2, k (N Negative)"},
  {0xfc07, 0xf003, 1, "1111 00kk kkkk k011", OP_AVR1,    OPCODE_brvs, "brvs", "k", "Branch if Overflow Flag is Set", "if(V=1)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBS 3, k (V Overflow in two's complement)"},
  {0xfc07, 0xf004, 1, "1111 00kk kkkk k100", OP_AVR1,    OPCODE_brlt, "brlt", "k", "Branch if Less Than, Signed", "if(N^V=1)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBS 4, k (S Sign)"},
  {0xfc07, 0xf005, 1, "1111 00kk kkkk k101", OP_AVR1,    OPCODE_brhs, "brhs", "k", "Branch if Half Carry Flag Set", "if(H=1)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBS 5, k (H Half carry)"},
  {0xfc07, 0xf006, 1, "1111 00kk kkkk k110", OP_AVR1,    OPCODE_brts, "brts", "k", "Branch if T Flag Set", "if(T=1)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBS 6, k (T Transfer bit)"},
  {0xfc07, 0xf007, 1, "1111 00kk kkkk k111", OP_AVR1,    OPCODE_brie, "brie", "k", "Branch if Interrupt Enabled", "if(I=1)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBS 7, k (I Interrupt enable)"},
  {0xfc00, 0xf000, 1, "1111 00kk kkkk ksss", OP_AVR1,    OPCODE_brbs, "brbs", "s, k", "Branch if Status Flag Set", "if(SREG(s)=1)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, ""},
  {0xfc07, 0xf400, 1, "1111 01kk kkkk k000", OP_AVR1,    OPCODE_brcc, "brcc", "k", "Branch if Carry Cleared", "if(C=0)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBC 0, k (C Carry)"},
  {0xfc07, 0xf400, 1, "1111 01kk kkkk k000", OP_AVR1,    OPCODE_brsh, "brsh", "k", "Branch if Same or Higher", "if(C=0)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBC 0, k (C Carry)"},
  {0xfc07, 0xf401, 1, "1111 01kk kkkk k001", OP_AVR1,    OPCODE_brne, "brne", "k", "Branch if Not Equal", "if(Z=0)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBC 1, k (Z Zero)"},
  {0xfc07, 0xf402, 1, "1111 01kk kkkk k010", OP_AVR1,    OPCODE_brpl, "brpl", "k", "Branch if Plus", "if(N=0)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBC 2, k (N Negative)"},
  {0xfc07, 0xf403, 1, "1111 01kk kkkk k011", OP_AVR1,    OPCODE_brvc, "brvc", "k", "Branch if Overflow Flag is Cleared", "if(V=0)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBC 3, k (V Overflow in two's complement)"},
  {0xfc07, 0xf404, 1, "1111 01kk kkkk k100", OP_AVR1,    OPCODE_brge, "brge", "k", "Branch if Greater or Equal, Signed", "if(N^V=0)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBC 4, k (S Sign)"},
  {0xfc07, 0xf405, 1, "1111 01kk kkkk k101", OP_AVR1,    OPCODE_brhc, "brhc", "k", "Branch if Half Carry Flag Cleared", "if(H=0)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBC 5, k (H Half carry)"},
  {0xfc07, 0xf406, 1, "1111 01kk kkkk k110", OP_AVR1,    OPCODE_brtc, "brtc", "k", "Branch if T Flag Cleared", "if(T=0)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBC 6, k (T Transfer bit)"},
  {0xfc07, 0xf407, 1, "1111 01kk kkkk k111", OP_AVR1,    OPCODE_brid, "brid", "k", "Branch if Interrupt Disabled", "if(I=0)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, "alias for BRBC 7, k (I Interrupt enable)"},
  {0xfc00, 0xf400, 1, "1111 01kk kkkk ksss", OP_AVR1,    OPCODE_brbc, "brbc", "s, k", "Branch if Status Flag Cleared", "if(SREG(s)=0)thenPC <-- PC+k+1", "None", {"1/2", "1/2", "1/2", "1/2"}, ""},

  // Data Transfer Instructions
  {0xfc00, 0x2c00, 1, "0010 11rd dddd rrrr", OP_AVR1,    OPCODE_mov, "mov", "Rd, Rr", "Copy Register", "Rd <-- Rr", "None", {"1", "1", "1", "1"}, ""},
  {0xff00, 0x0100, 1, "0000 0001 dddd rrrr", OP_AVR25,   OPCODE_movw, "movw", "Rd, Rr", "Copy Register Pair", "Rd+1:Rd <-- Rr+1:Rr", "None", {"1", "1", "1", "n/a"}, ""},
  {0xf000, 0xe000, 1, "1110 KKKK dddd KKKK", OP_AVR1,    OPCODE_ldi, "ldi", "Rd, K", "Load Immediate", "Rd <-- K", "None", {"1", "1", "1", "1"}, "d=16..31"},
  {0xfe0f, 0x9000, 2, "1001 000d dddd 0000  kkkk kkkk kkkk kkkk", OP_AVR2nRC, OPCODE_lds, "lds", "Rd, k", "Load Direct from data space", "Rd <-- (k)", "None", {"2", "3", "3", "2"}, ""},
  {0xf800, 0xa000, 1, "1010 0kkk dddd kkkk", OP_AVR_RC,  OPCODE_lds_rc, "lds", "Rd, k", "Load Direct from data space", "Rd <-- (k)", "None", {"n/a", "n/a", "n/a", "2"}, "AVRrc only (TPI parts)"},
  {0xfe0f, 0x900c, 1, "1001 000d dddd 1100", OP_AVR2,    OPCODE_ld_1, "ld", "Rd, X", "Load Indirect", "Rd <-- (X)", "None", {"2", "2", "2", "1/2"}, ""},
  {0xfe0f, 0x900d, 1, "1001 000d dddd 1101", OP_AVR2,    OPCODE_ld_2, "ld", "Rd, X+", "Load Indirect and Post-Increment", "Rd <-- (X) X <-- X+1", "None", {"2", "2", "2", "2/3"}, ""},
  {0xfe0f, 0x900e, 1, "1001 000d dddd 1110", OP_AVR2,    OPCODE_ld_3, "ld", "Rd, -X", "Load Indirect and Pre-Decrement", "X <-- X-1, <-- X-1 Rd <-- (X) <-- (X)", "None", {"2", "3", "2", "2/3"}, ""},
  {0xfe0f, 0x8008, 1, "1000 000d dddd 1000", OP_AVR2,    OPCODE_ld_4, "ld", "Rd, Y", "Load Indirect", "Rd <-- (Y) <-- (Y)", "None", {"2", "2", "2", "1/2"}, "alias for LDD Rd, Y+0"},
  {0xfe0f, 0x9009, 1, "1001 000d dddd 1001", OP_AVR2,    OPCODE_ld_5, "ld", "Rd, Y+", "Load Indirect and Post-Increment", "Rd <-- (Y) Y <-- Y+1", "None", {"2", "2", "2", "2/3"}, ""},
  {0xfe0f, 0x900a, 1, "1001 000d dddd 1010", OP_AVR2,    OPCODE_ld_6, "ld", "Rd, -Y", "Load Indirect and Pre-Decrement", "Y <-- Y-1 Rd <-- (Y)", "None", {"2", "3", "2", "2/3"}, ""},
  {0xd208, 0x8008, 1, "10q0 qq0d dddd 1qqq", OP_AVR2nRC, OPCODE_ldd_1, "ldd", "Rd, Y+q", "Load Indirect with Displacement", "Rd <-- (Y+q)", "None", {"2", "3", "2", "n/a"}, ""},
  {0xfe0f, 0x8000, 1, "1000 000d dddd 0000", OP_AVR1,    OPCODE_ld_7, "ld", "Rd, Z", "Load Indirect", "Rd <-- (Z)", "None", {"2", "2", "2", "1/2"}, "alias for LDD Rd,Z+0"},
  {0xfe0f, 0x9001, 1, "1001 000d dddd 0001", OP_AVR1,    OPCODE_ld_8, "ld", "Rd, Z+", "Load Indirect and Post-Increment", "Rd <-- (Z), Z <-- Z+1", "None", {"2", "2", "2", "2/3"}, ""},
  {0xfe0f, 0x9002, 1, "1001 000d dddd 0010", OP_AVR1,    OPCODE_ld_9, "ld", "Rd, -Z", "Load Indirect and Pre-Decrement", "Z <-- Z-1, Rd <-- (Z)", "None", {"2", "3", "2", "2/3"}, ""},
  {0xd208, 0x8000, 1, "10q0 qq0d dddd 0qqq", OP_AVR2nRC, OPCODE_ldd_2, "ldd", "Rd, Z+q", "Load Indirect with Displacement", "Rd <-- (Z+q)", "None", {"2", "3", "2", "n/a"}, ""},
  {0xfe0f, 0x9200, 2, "1001 001d dddd 0000  kkkk kkkk kkkk kkkk", OP_AVR2nRC, OPCODE_sts, "sts", "k, Rr", "Store Direct to Data Space", "(k) <-- Rd", "None", {"2", "2", "2", "1"}, ""},
  {0xf800, 0xa800, 1, "1010 1kkk dddd kkkk", OP_AVR_RC,  OPCODE_sts_rc, "sts", "k, Rr", "Store Direct to Data Space", "(k) <-- Rd", "None", {"n/a", "n/a", "n/a", "1"}, "AVRrc only (TPI parts)"},
  {0xfe0f, 0x920c, 1, "1001 001r rrrr 1100", OP_AVR2,    OPCODE_st_1, "st", "X, Rr", "Store Indirect", "(X) <-- Rr", "None", {"2", "1", "1", "1"}, ""},
  {0xfe0f, 0x920d, 1, "1001 001r rrrr 1101", OP_AVR2,    OPCODE_st_2, "st", "X+, Rr", "Store Indirect and Post-Increment", "(X) <-- Rr, X <-- X+1", "None", {"2", "1", "1", "1"}, ""},
  {0xfe0f, 0x920e, 1, "1001 001r rrrr 1110", OP_AVR2,    OPCODE_st_3, "st", "-X, Rr", "Store Indirect and Pre-Decrement", "X <-- X-1, (X) <-- Rr", "None", {"2", "2", "1", "1"}, ""},
  {0xfe0f, 0x8208, 1, "1000 001r rrrr 1000", OP_AVR2,    OPCODE_st_4, "st", "Y, Rr", "Store Indirect", "(Y) <-- Rr", "None", {"2", "1", "1", "1"}, "alias for STD Y+0, Rr"},
  {0xfe0f, 0x9209, 1, "1001 001r rrrr 1001", OP_AVR2,    OPCODE_st_5, "st", "Y+, Rr", "Store Indirect and Post-Increment", "(Y) <-- Rr, Y <-- Y+1", "None", {"2", "1", "1", "1"}, ""},
  {0xfe0f, 0x920a, 1, "1001 001r rrrr 1010", OP_AVR2,    OPCODE_st_6, "st", "-Y, Rr", "Store Indirect and Pre-Decrement", "Y <-- Y-1, (Y) <-- Rr", "None", {"2", "2", "1", "1"}, ""},
  {0xd208, 0x8208, 1, "10q0 qq1r rrrr 1qqq", OP_AVR2nRC, OPCODE_std_1, "std", "Y+q, Rr", "Store Indirect with Displacement", "(Y+q) <-- Rr", "None", {"2", "2", "1", "n/a"}, ""},
  {0xfe0f, 0x8200, 1, "1000 001r rrrr 0000", OP_AVR1,    OPCODE_st_7, "st", "Z, Rr", "Store Indirect", "(Z) <-- Rr", "None", {"2", "1", "1", "1"}, "alias for STD Z+0, Rr"},
  {0xfe0f, 0x9201, 1, "1001 001r rrrr 0001", OP_AVR1,    OPCODE_st_8, "st", "Z+, Rr", "Store Indirect and Post-Increment", "(Z) <-- Rr Z <-- Z+1", "None", {"2", "1", "1", "1"}, ""},
  {0xfe0f, 0x9202, 1, "1001 001r rrrr 0010", OP_AVR1,    OPCODE_st_9, "st", "-Z, Rr", "Store Indirect and Pre-Decrement", "Z <-- Z-1", "None", {"2", "2", "1", "1"}, ""},
  {0xd208, 0x8200, 1, "10q0 qq1r rrrr 0qqq", OP_AVR2nRC, OPCODE_std_2, "std", "Z+q, Rr", "Store Indirect with Displacement", "(Z+q) <-- Rr", "None", {"2", "2", "1", "n/a"}, ""},
  {0xffff, 0x95c8, 1, "1001 0101 1100 1000", OP_AVR1nRC, OPCODE_lpm_1, "lpm", "-", "Load Program Memory", "R0 <-- (Z)", "None", {"3", "3", "3", "n/a"}, ""},
  {0xfe0f, 0x9004, 1, "1001 000d dddd 0100", OP_AVR25,   OPCODE_lpm_2, "lpm", "Rd, Z", "Load Program Memory", "Rd <-- (Z)", "None", {"3", "3", "3", "n/a"}, ""},
  {0xfe0f, 0x9005, 1, "1001 000d dddd 0101", OP_AVR25,   OPCODE_lpm_3, "lpm", "Rd, Z+", "Load Program Memory and Post- Increment", "Rd <-- (Z), Z <-- Z+1", "None", {"3", "3", "3", "n/a"}, ""},
  {0xffff, 0x95d8, 1, "1001 0101 1101 1000", OP_AVR_L,   OPCODE_elpm_1, "elpm", "R0, Z", "Extended Load Program Memory", "R0 <-- (RAMPZ:Z)", "None", {"3", "3", "3", "n/a"}, ""},
  {0xfe0f, 0x9006, 1, "1001 000d dddd 0110", OP_AVR_L,   OPCODE_elpm_2, "elpm", "Rd, Z", "Extended Load Program Memory", "Rd <-- (RAMPZ:Z)", "None", {"3", "3", "3", "n/a"}, ""},
  {0xfe0f, 0x9007, 1, "1001 000d dddd 0111", OP_AVR_L,   OPCODE_elpm_3, "elpm", "Rd, Z+", "Extended Load Program Memory and Post-Increment", "Rd <-- (RAMPZ:Z), Z <-- Z+1", "None", {"3", "3", "3", "n/a"}, ""},
  {0xffff, 0x95e8, 1, "1001 0101 1110 1000", OP_AVR25,   OPCODE_spm, "spm", "Z", "Store Program Memory", "(RAMPZ:Z) <-- R1:R0", "None", {"-", "-", "-", "-"}, ""},
  {0xffff, 0x95f8, 1, "1001 0101 1111 1000", OP_AVR_XTM, OPCODE_spm_zz, "spm", "Z+", "Store Program Memory and Post- Increment by 2", "(RAMPZ:Z) <-- R1:R0, Z <-- Z+2", "None", {"n/a", "-", "-", "n/a"}, ""},
  {0xf800, 0xb000, 1, "1011 0AAd dddd AAAA", OP_AVR1,    OPCODE_in, "in", "Rd, A", "In From I/O Location", "Rd <-- I/O(A)", "None", {"1", "1", "1", "1"}, ""},
  {0xf800, 0xb800, 1, "1011 1AAr rrrr AAAA", OP_AVR1,    OPCODE_out, "out", "A, Rr", "Out To I/O Location", "I/O(A) <-- Rr", "None", {"1", "1", "1", "1"}, ""},
  {0xfe0f, 0x920f, 1, "1001 001d dddd 1111", OP_AVR2,    OPCODE_push, "push", "Rr", "Push Register on Stack", "STACK <-- Rr", "None", {"2", "1", "1", "1"}, ""},
  {0xfe0f, 0x900f, 1, "1001 000d dddd 1111", OP_AVR2,    OPCODE_pop, "pop", "Rd", "Pop Register from Stack", "Rd <-- STACK", "None", {"2", "2", "2", "3"}, ""},
  {0xfe0f, 0x9204, 1, "1001 001r rrrr 0100", OP_AVR_XM,  OPCODE_xch, "xch", "Z, Rd", "Exchange", "(Z) <-- Rd, Rd <-- (Z)", "None", {"n/a", "1", "n/a", "n/a"}, ""},
  {0xfe0f, 0x9205, 1, "1001 001r rrrr 0101", OP_AVR_XM,  OPCODE_las, "las", "Z, Rd", "Load and Set", "(Z) <-- Rd|(Z), Rd <-- (Z)", "None", {"n/a", "1", "n/a", "n/a"}, ""},
  {0xfe0f, 0x9206, 1, "1001 001r rrrr 0110", OP_AVR_XM,  OPCODE_lac, "lac", "Z, Rd", "Load and Clear", "(Z) <-- ($FF - Rd) & (Z), Rd <-- (Z)", "None", {"n/a", "1", "n/a", "n/a"}, ""},
  {0xfe0f, 0x9207, 1, "1001 001r rrrr 0111", OP_AVR_XM,  OPCODE_lat, "lat", "Z, Rd", "Load and Toggle", "(Z) <-- Rd^(Z), Rd <-- (Z)", "None", {"n/a", "1", "n/a", "n/a"}, ""},

  // Bit and Bit-test Instructions
  {0xfc00, 0x0c00, 1, "0000 11dd dddd dddd", OP_AVR1,    OPCODE_lsl, "lsl", "Rd", "Logical Shift Left", "C <-- Rd(7) <-- Rd(6) ... Rd(1) <-- Rd(0) <-- 0", "Z,C,N,V,H", {"1", "1", "1", "1"}, "alias for ADD Rd, Rd"},
  {0xfe0f, 0x9406, 1, "1001 010d dddd 0110", OP_AVR1,    OPCODE_lsr, "lsr", "Rd", "Logical Shift Right", "0 --> Rd(7) --> Rd(6) ... Rd(1) --> Rd(0) --> C", "Z,C,N,V", {"1", "1", "1", "1"}, ""},
  {0xfc00, 0x1c00, 1, "0001 11dd dddd dddd", OP_AVR1,    OPCODE_rol, "rol", "Rd", "Rotate Left Through Carry", "C <-- Rd(7) <-- Rd(6) ... Rd(1) <-- Rd(0) <-- C", "Z,C,N,V,H", {"1", "1", "1", "1"}, "alias for ADC Rd, Rd"},
  {0xfe0f, 0x9407, 1, "1001 010d dddd 0111", OP_AVR1,    OPCODE_ror, "ror", "Rd", "Rotate Right Through Carry", "C --> Rd(7) --> Rd(6) ... Rd(1) --> Rd(0) --> C", "Z,C,N,V", {"1", "1", "1", "1"}, ""},
  {0xfe0f, 0x9405, 1, "1001 010d dddd 0101", OP_AVR1,    OPCODE_asr, "asr", "Rd", "Arithmetic Shift Right", "Rd(7) --> Rd(7) --> Rd(6) ... Rd(1) --> Rd(0) --> C", "Z,C,N,V", {"1", "1", "1", "1"}, ""},
  {0xfe0f, 0x9402, 1, "1001 010d dddd 0010", OP_AVR1,    OPCODE_swap, "swap", "Rd", "Swap Nibbles", "Rd(3..0) <--> Rd(7..4)", "None", {"1", "1", "1", "1"}, ""},
  {0xff8f, 0x9408, 1, "1001 0100 0sss 1000", OP_AVR1,    OPCODE_bset, "bset", "s", "Flag Set", "SREG(s) <-- 1", "SREG(s)", {"1", "1", "1", "1"}, ""},
  {0xff8f, 0x9488, 1, "1001 0100 1sss 1000", OP_AVR1,    OPCODE_bclr, "bclr", "s", "Flag Clear", "SREG(s) <-- 0", "SREG(s)", {"1", "1", "1", "1"}, "s = 0-7 = C,Z,N,V,S,H,T,I"},
  {0xff00, 0x9a00, 1, "1001 1010 AAAA Abbb", OP_AVR1,    OPCODE_sbi, "sbi", "A, b", "Set Bit in I/O Register", "I/O(A,b) <-- 1", "None", {"2", "1", "1", "1"}, ""},
  {0xff00, 0x9800, 1, "1001 1000 AAAA Abbb", OP_AVR1,    OPCODE_cbi, "cbi", "A, b", "Clear Bit in I/O Register", "I/O(A,b) <-- 0", "None", {"2", "1", "1", "1"}, ""},
  {0xfe08, 0xfa00, 1, "1111 101d dddd 0bbb", OP_AVR1,    OPCODE_bst, "bst", "Rr, b", "Bit Store from Register to T", "T <-- Rr(b)", "T", {"1", "1", "1", "1"}, ""},
  {0xfe08, 0xf800, 1, "1111 100d dddd 0bbb", OP_AVR1,    OPCODE_bld, "bld", "Rd, b", "Bit load from T to Register", "Rd(b) <-- T", "None", {"1", "1", "1", "1"}, ""},
  {0xffff, 0x9408, 1, "1001 0100 0000 1000", OP_AVR1,    OPCODE_sec, "sec", "-", "Set Carry", "C <-- 1", "C", {"1", "1", "1", "1"}, "alias for BSET 0"},
  {0xffff, 0x9488, 1, "1001 0100 1000 1000", OP_AVR1,    OPCODE_clc, "clc", "-", "Clear Carry", "C <-- 0", "C", {"1", "1", "1", "1"}, "alias for BCLR 0"},
  {0xffff, 0x9428, 1, "1001 0100 0010 1000", OP_AVR1,    OPCODE_sen, "sen", "-", "Set Negative Flag", "N <-- 1", "N", {"1", "1", "1", "1"}, "alias for BSET 2"},
  {0xffff, 0x94a8, 1, "1001 0100 1010 1000", OP_AVR1,    OPCODE_cln, "cln", "-", "Clear Negative Flag", "N <-- 0", "N", {"1", "1", "1", "1"}, "alias for BCLR 2"},
  {0xffff, 0x9418, 1, "1001 0100 0001 1000", OP_AVR1,    OPCODE_sez, "sez", "-", "Set Zero Flag", "Z <-- 1", "Z", {"1", "1", "1", "1"}, "alias for BSET 1"},
  {0xffff, 0x9498, 1, "1001 0100 1001 1000", OP_AVR1,    OPCODE_clz, "clz", "-", "Clear Zero Flag", "Z <-- 0", "Z", {"1", "1", "1", "1"}, "alias for BCLR 1"},
  {0xffff, 0x9478, 1, "1001 0100 0111 1000", OP_AVR1,    OPCODE_sei, "sei", "-", "Global Interrupt Enable", "I <-- 1", "I", {"1", "1", "1", "1"}, "alias for BSET 7"},
  {0xffff, 0x94f8, 1, "1001 0100 1111 1000", OP_AVR1,    OPCODE_cli, "cli", "-", "Global Interrupt Disable", "I <-- 0", "I", {"1", "1", "1", "1"}, "alias for BCLR 7"},
  {0xffff, 0x9448, 1, "1001 0100 0100 1000", OP_AVR1,    OPCODE_ses, "ses", "-", "Set Signed Test Flag", "S <-- 1", "S", {"1", "1", "1", "1"}, "alias for BSET 4"},
  {0xffff, 0x94c8, 1, "1001 0100 1100 1000", OP_AVR1,    OPCODE_cls, "cls", "-", "Clear Signed Test Flag", "S <-- 0", "S", {"1", "1", "1", "1"}, "alias for BCLR 4"},
  {0xffff, 0x9438, 1, "1001 0100 0011 1000", OP_AVR1,    OPCODE_sev, "sev", "-", "Set Two's Complement Overflow", "V <-- 1", "V", {"1", "1", "1", "1"}, "alias for BSET 3"},
  {0xffff, 0x94b8, 1, "1001 0100 1011 1000", OP_AVR1,    OPCODE_clv, "clv", "-", "Clear Two's Complement Overflow", "V <-- 0", "V", {"1", "1", "1", "1"}, "alias for BCLR 3"},
  {0xffff, 0x9468, 1, "1001 0100 0110 1000", OP_AVR1,    OPCODE_set, "set", "-", "Set T in SREG", "T <-- 1", "T", {"1", "1", "1", "1"}, "alias for BSET 6"},
  {0xffff, 0x94e8, 1, "1001 0100 1110 1000", OP_AVR1,    OPCODE_clt, "clt", "-", "Clear T in SREG", "T <-- 0", "T", {"1", "1", "1", "1"}, "alias for BCLR 6"},
  {0xffff, 0x9458, 1, "1001 0100 0101 1000", OP_AVR1,    OPCODE_seh, "seh", "-", "Set Half Carry Flag in SREG", "H <-- 1", "H", {"1", "1", "1", "1"}, "alias for BSET 5"},
  {0xffff, 0x94d8, 1, "1001 0100 1101 1000", OP_AVR1,    OPCODE_clh, "clh", "-", "Clear Half Carry Flag in SREG", "H <-- 0", "H", {"1", "1", "1", "1"}, "alias for BCLR 5"},

  // MCU Control Instructions
  {0xffff, 0x9598, 1, "1001 0101 1001 1000", OP_AVR1,    OPCODE_break, "break", "-", "Break", "(cf documentation)", "None", {"-", "-", "-", "-"}, "not available on all parts"},
  {0xffff, 0x0000, 1, "0000 0000 0000 0000", OP_AVR1,    OPCODE_nop, "nop", "-", "No Operation", "-", "None", {"1", "1", "1", "1"}, ""},
  {0xffff, 0x9588, 1, "1001 0101 1000 1000", OP_AVR1,    OPCODE_sleep, "sleep", "-", "Sleep", "(cf documentation)", "None", {"-", "-", "-", "-"}, ""},
  {0xffff, 0x95a8, 1, "1001 0101 1010 1000", OP_AVR1,    OPCODE_wdr, "wdr", "-", "Watchdog Reset", "(cf documentation)", "None", {"1", "1", "1", "1"}, ""},

  // Unallocated codes
  {0xff00, 0x0000, 1, "0000 0000 xxxx xxxx", OP_AVR1,    OPCODE_x_nop_1, "nop", "", "", "", "", {"", "", "", ""}, "xxxx xxxx != 0000 0000"},
  {0xfe0f, 0x9003, 1, "1001 000x xxxx 0011", OP_AVR1,    OPCODE_x_nop_2, "nop", "", "", "", "", {"", "", "", ""}, ""},
  {0xfe0f, 0x9008, 1, "1001 000x xxxx 1000", OP_AVR1,    OPCODE_x_nop_3, "nop", "", "", "", "", {"", "", "", ""}, ""},
  {0xfe0f, 0x900b, 1, "1001 000x xxxx 1011", OP_AVR1,    OPCODE_x_nop_4, "nop", "", "", "", "", {"", "", "", ""}, ""},
  {0xfe0f, 0x9203, 1, "1001 001x xxxx 0011", OP_AVR1,    OPCODE_x_nop_5, "nop", "", "", "", "", {"", "", "", ""}, ""},
  {0xfe0f, 0x9208, 1, "1001 001x xxxx 1000", OP_AVR1,    OPCODE_x_nop_6, "nop", "", "", "", "", {"", "", "", ""}, ""},
  {0xfe0f, 0x920b, 1, "1001 001x xxxx 1011", OP_AVR1,    OPCODE_x_nop_7, "nop", "", "", "", "", {"", "", "", ""}, ""},
  {0xff1f, 0x9509, 1, "1001 0101 xxx0 1001", OP_AVR2,    OPCODE_x_icall, "icall", "", "", "", "", {"", "", "", ""}, "xxx != 000"},
  {0xff1f, 0x9519, 1, "1001 0101 xxx1 1001", OP_AVR_XL,  OPCODE_x_eicall, "eicall", "", "", "", "", {"", "", "", ""}, "xxx != 000"},
  {0xff9f, 0x9508, 1, "1001 0101 0xx0 1000", OP_AVR1,    OPCODE_x_ret, "ret", "", "", "", "", {"", "", "", ""}, "xx != 00"},
  {0xff9f, 0x9518, 1, "1001 0101 0xx1 1000", OP_AVR1,    OPCODE_x_reti, "reti", "", "", "", "", {"", "", "", ""}, "xx != 00"},
  {0xffff, 0x95b8, 1, "1001 0101 1011 1000", OP_AVR1,    OPCODE_x_nop_8, "nop", "", "", "", "", {"", "", "", ""}, ""},
  {0xfe0f, 0x9404, 1, "1001 010x xxxx 0100", OP_AVR1,    OPCODE_x_nop_9, "nop", "", "", "", "", {"", "", "", ""}, ""},
  {0xff0f, 0x950b, 1, "1001 0101 xxxx 1011", OP_AVR1,    OPCODE_x_nop_a, "nop", "", "", "", "", {"", "", "", ""}, ""},
  {0xff1f, 0x9409, 1, "1001 0100 xxx0 1001", OP_AVR2,    OPCODE_x_ijmp, "ijmp", "", "", "", "", {"", "", "", ""}, "xxx != 000"},
  {0xff1f, 0x9419, 1, "1001 0100 xxx1 1001", OP_AVR_XL,  OPCODE_x_eijmp, "eijmp", "", "", "", "", {"", "", "", ""}, "xxx != 000"},
  {0xfe08, 0xf808, 1, "1111 100x xxxx 1xxx", OP_AVR1,    OPCODE_x_bld, "bld", "Rd, b", "", "", "", {"", "", "", ""}, ""},
  {0xfe08, 0xfa08, 1, "1111 101x xxxx 1xxx", OP_AVR1,    OPCODE_x_bst, "bst", "Rd, b", "", "", "", {"", "", "", ""}, ""},
  {0xfe08, 0xfc08, 1, "1111 110x xxxx 1xxx", OP_AVR1,    OPCODE_x_sbrc, "sbrc", "Rr, b", "", "", "", {"", "", "", ""}, ""},
  {0xfe08, 0xfe08, 1, "1111 111x xxxx 1xxx", OP_AVR1,    OPCODE_x_sbrs, "sbrs", "Rr, b", "", "", "", {"", "", "", ""}, ""},
};
