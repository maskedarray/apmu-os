	.file	"main.c"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.section	.rodata
	.align	2
.LC0:
	.string	"Hello, RISC-V!\n"
	.text
	.align	2
	.globl	main
	.hidden	main
	.type	main, @function
main:
.LFB0:
	.file 1 "main.c"
	.loc 1 2 14
	.cfi_startproc
	addi	sp,sp,-32
	.cfi_def_cfa_offset 32
	sw	s0,28(sp)
	.cfi_offset 8, -4
	addi	s0,sp,32
	.cfi_def_cfa 8, 0
	.loc 1 3 19
	li	a5,268435456
	sw	a5,-24(s0)
	.loc 1 4 16
	lla	a5,.LC0
	sw	a5,-20(s0)
	.loc 1 5 10
	j	.L2
.L3:
	.loc 1 6 20
	lw	a5,-20(s0)
	addi	a4,a5,1
	sw	a4,-20(s0)
	.loc 1 6 16
	lbu	a4,0(a5)
	.loc 1 6 14
	lw	a5,-24(s0)
	sb	a4,0(a5)
.L2:
	.loc 1 5 11
	lw	a5,-20(s0)
	lbu	a5,0(a5)
	.loc 1 5 10
	bne	a5,zero,.L3
.L4:
	.loc 1 8 10 discriminator 1
	j	.L4
	.cfi_endproc
.LFE0:
	.size	main, .-main
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0x78
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.byte	0x1
	.4byte	.LASF296
	.byte	0xc
	.4byte	.LASF297
	.4byte	.LASF298
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.4byte	.Ldebug_macro0
	.byte	0x2
	.4byte	.LASF299
	.byte	0x1
	.byte	0x2
	.byte	0x7
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.byte	0x1
	.byte	0x9c
	.4byte	0x5e
	.byte	0x3
	.4byte	.LASF295
	.byte	0x1
	.byte	0x3
	.byte	0x13
	.4byte	0x5e
	.byte	0x2
	.byte	0x91
	.byte	0x68
	.byte	0x4
	.string	"msg"
	.byte	0x1
	.byte	0x4
	.byte	0x10
	.4byte	0x75
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0
	.byte	0x5
	.byte	0x4
	.4byte	0x6b
	.byte	0x6
	.byte	0x1
	.byte	0x8
	.4byte	.LASF300
	.byte	0x7
	.4byte	0x64
	.byte	0x8
	.4byte	0x64
	.byte	0x5
	.byte	0x4
	.4byte	0x70
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.byte	0x1
	.byte	0x11
	.byte	0x1
	.byte	0x25
	.byte	0xe
	.byte	0x13
	.byte	0xb
	.byte	0x3
	.byte	0xe
	.byte	0x1b
	.byte	0xe
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x6
	.byte	0x10
	.byte	0x17
	.byte	0x99,0x42
	.byte	0x17
	.byte	0
	.byte	0
	.byte	0x2
	.byte	0x2e
	.byte	0x1
	.byte	0x3f
	.byte	0x19
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x6
	.byte	0x40
	.byte	0x18
	.byte	0x97,0x42
	.byte	0x19
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x3
	.byte	0x34
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x2
	.byte	0x18
	.byte	0
	.byte	0
	.byte	0x4
	.byte	0x34
	.byte	0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x2
	.byte	0x18
	.byte	0
	.byte	0
	.byte	0x5
	.byte	0xf
	.byte	0
	.byte	0xb
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x6
	.byte	0x24
	.byte	0
	.byte	0xb
	.byte	0xb
	.byte	0x3e
	.byte	0xb
	.byte	0x3
	.byte	0xe
	.byte	0
	.byte	0
	.byte	0x7
	.byte	0x35
	.byte	0
	.byte	0x49
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x8
	.byte	0x26
	.byte	0
	.byte	0x49
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	0
	.4byte	0
	.section	.debug_macro,"",@progbits
.Ldebug_macro0:
	.2byte	0x4
	.byte	0x2
	.4byte	.Ldebug_line0
	.byte	0x7
	.4byte	.Ldebug_macro2
	.byte	0x3
	.byte	0
	.byte	0x1
	.byte	0x4
	.byte	0
	.section	.debug_macro,"G",@progbits,wm4.0.bc96be2ddbb82b92b55ee5cd8a5a9373,comdat
.Ldebug_macro2:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.byte	0
	.4byte	.LASF0
	.byte	0x5
	.byte	0
	.4byte	.LASF1
	.byte	0x5
	.byte	0
	.4byte	.LASF2
	.byte	0x5
	.byte	0
	.4byte	.LASF3
	.byte	0x5
	.byte	0
	.4byte	.LASF4
	.byte	0x5
	.byte	0
	.4byte	.LASF5
	.byte	0x5
	.byte	0
	.4byte	.LASF6
	.byte	0x5
	.byte	0
	.4byte	.LASF7
	.byte	0x5
	.byte	0
	.4byte	.LASF8
	.byte	0x5
	.byte	0
	.4byte	.LASF9
	.byte	0x5
	.byte	0
	.4byte	.LASF10
	.byte	0x5
	.byte	0
	.4byte	.LASF11
	.byte	0x5
	.byte	0
	.4byte	.LASF12
	.byte	0x5
	.byte	0
	.4byte	.LASF13
	.byte	0x5
	.byte	0
	.4byte	.LASF14
	.byte	0x5
	.byte	0
	.4byte	.LASF15
	.byte	0x5
	.byte	0
	.4byte	.LASF16
	.byte	0x5
	.byte	0
	.4byte	.LASF17
	.byte	0x5
	.byte	0
	.4byte	.LASF18
	.byte	0x5
	.byte	0
	.4byte	.LASF19
	.byte	0x5
	.byte	0
	.4byte	.LASF20
	.byte	0x5
	.byte	0
	.4byte	.LASF21
	.byte	0x5
	.byte	0
	.4byte	.LASF22
	.byte	0x5
	.byte	0
	.4byte	.LASF23
	.byte	0x5
	.byte	0
	.4byte	.LASF24
	.byte	0x5
	.byte	0
	.4byte	.LASF25
	.byte	0x5
	.byte	0
	.4byte	.LASF26
	.byte	0x5
	.byte	0
	.4byte	.LASF27
	.byte	0x5
	.byte	0
	.4byte	.LASF28
	.byte	0x5
	.byte	0
	.4byte	.LASF29
	.byte	0x5
	.byte	0
	.4byte	.LASF30
	.byte	0x5
	.byte	0
	.4byte	.LASF31
	.byte	0x5
	.byte	0
	.4byte	.LASF32
	.byte	0x5
	.byte	0
	.4byte	.LASF33
	.byte	0x5
	.byte	0
	.4byte	.LASF34
	.byte	0x5
	.byte	0
	.4byte	.LASF35
	.byte	0x5
	.byte	0
	.4byte	.LASF36
	.byte	0x5
	.byte	0
	.4byte	.LASF37
	.byte	0x5
	.byte	0
	.4byte	.LASF38
	.byte	0x5
	.byte	0
	.4byte	.LASF39
	.byte	0x5
	.byte	0
	.4byte	.LASF40
	.byte	0x5
	.byte	0
	.4byte	.LASF41
	.byte	0x5
	.byte	0
	.4byte	.LASF42
	.byte	0x5
	.byte	0
	.4byte	.LASF43
	.byte	0x5
	.byte	0
	.4byte	.LASF44
	.byte	0x5
	.byte	0
	.4byte	.LASF45
	.byte	0x5
	.byte	0
	.4byte	.LASF46
	.byte	0x5
	.byte	0
	.4byte	.LASF47
	.byte	0x5
	.byte	0
	.4byte	.LASF48
	.byte	0x5
	.byte	0
	.4byte	.LASF49
	.byte	0x5
	.byte	0
	.4byte	.LASF50
	.byte	0x5
	.byte	0
	.4byte	.LASF51
	.byte	0x5
	.byte	0
	.4byte	.LASF52
	.byte	0x5
	.byte	0
	.4byte	.LASF53
	.byte	0x5
	.byte	0
	.4byte	.LASF54
	.byte	0x5
	.byte	0
	.4byte	.LASF55
	.byte	0x5
	.byte	0
	.4byte	.LASF56
	.byte	0x5
	.byte	0
	.4byte	.LASF57
	.byte	0x5
	.byte	0
	.4byte	.LASF58
	.byte	0x5
	.byte	0
	.4byte	.LASF59
	.byte	0x5
	.byte	0
	.4byte	.LASF60
	.byte	0x5
	.byte	0
	.4byte	.LASF61
	.byte	0x5
	.byte	0
	.4byte	.LASF62
	.byte	0x5
	.byte	0
	.4byte	.LASF63
	.byte	0x5
	.byte	0
	.4byte	.LASF64
	.byte	0x5
	.byte	0
	.4byte	.LASF65
	.byte	0x5
	.byte	0
	.4byte	.LASF66
	.byte	0x5
	.byte	0
	.4byte	.LASF67
	.byte	0x5
	.byte	0
	.4byte	.LASF68
	.byte	0x5
	.byte	0
	.4byte	.LASF69
	.byte	0x5
	.byte	0
	.4byte	.LASF70
	.byte	0x5
	.byte	0
	.4byte	.LASF71
	.byte	0x5
	.byte	0
	.4byte	.LASF72
	.byte	0x5
	.byte	0
	.4byte	.LASF73
	.byte	0x5
	.byte	0
	.4byte	.LASF74
	.byte	0x5
	.byte	0
	.4byte	.LASF75
	.byte	0x5
	.byte	0
	.4byte	.LASF76
	.byte	0x5
	.byte	0
	.4byte	.LASF77
	.byte	0x5
	.byte	0
	.4byte	.LASF78
	.byte	0x5
	.byte	0
	.4byte	.LASF79
	.byte	0x5
	.byte	0
	.4byte	.LASF80
	.byte	0x5
	.byte	0
	.4byte	.LASF81
	.byte	0x5
	.byte	0
	.4byte	.LASF82
	.byte	0x5
	.byte	0
	.4byte	.LASF83
	.byte	0x5
	.byte	0
	.4byte	.LASF84
	.byte	0x5
	.byte	0
	.4byte	.LASF85
	.byte	0x5
	.byte	0
	.4byte	.LASF86
	.byte	0x5
	.byte	0
	.4byte	.LASF87
	.byte	0x5
	.byte	0
	.4byte	.LASF88
	.byte	0x5
	.byte	0
	.4byte	.LASF89
	.byte	0x5
	.byte	0
	.4byte	.LASF90
	.byte	0x5
	.byte	0
	.4byte	.LASF91
	.byte	0x5
	.byte	0
	.4byte	.LASF92
	.byte	0x5
	.byte	0
	.4byte	.LASF93
	.byte	0x5
	.byte	0
	.4byte	.LASF94
	.byte	0x5
	.byte	0
	.4byte	.LASF95
	.byte	0x5
	.byte	0
	.4byte	.LASF96
	.byte	0x5
	.byte	0
	.4byte	.LASF97
	.byte	0x5
	.byte	0
	.4byte	.LASF98
	.byte	0x5
	.byte	0
	.4byte	.LASF99
	.byte	0x5
	.byte	0
	.4byte	.LASF100
	.byte	0x5
	.byte	0
	.4byte	.LASF101
	.byte	0x5
	.byte	0
	.4byte	.LASF102
	.byte	0x5
	.byte	0
	.4byte	.LASF103
	.byte	0x5
	.byte	0
	.4byte	.LASF104
	.byte	0x5
	.byte	0
	.4byte	.LASF105
	.byte	0x5
	.byte	0
	.4byte	.LASF106
	.byte	0x5
	.byte	0
	.4byte	.LASF107
	.byte	0x5
	.byte	0
	.4byte	.LASF108
	.byte	0x5
	.byte	0
	.4byte	.LASF109
	.byte	0x5
	.byte	0
	.4byte	.LASF110
	.byte	0x5
	.byte	0
	.4byte	.LASF111
	.byte	0x5
	.byte	0
	.4byte	.LASF112
	.byte	0x5
	.byte	0
	.4byte	.LASF113
	.byte	0x5
	.byte	0
	.4byte	.LASF114
	.byte	0x5
	.byte	0
	.4byte	.LASF115
	.byte	0x5
	.byte	0
	.4byte	.LASF116
	.byte	0x5
	.byte	0
	.4byte	.LASF117
	.byte	0x5
	.byte	0
	.4byte	.LASF118
	.byte	0x5
	.byte	0
	.4byte	.LASF119
	.byte	0x5
	.byte	0
	.4byte	.LASF120
	.byte	0x5
	.byte	0
	.4byte	.LASF121
	.byte	0x5
	.byte	0
	.4byte	.LASF122
	.byte	0x5
	.byte	0
	.4byte	.LASF123
	.byte	0x5
	.byte	0
	.4byte	.LASF124
	.byte	0x5
	.byte	0
	.4byte	.LASF125
	.byte	0x5
	.byte	0
	.4byte	.LASF126
	.byte	0x5
	.byte	0
	.4byte	.LASF127
	.byte	0x5
	.byte	0
	.4byte	.LASF128
	.byte	0x5
	.byte	0
	.4byte	.LASF129
	.byte	0x5
	.byte	0
	.4byte	.LASF130
	.byte	0x5
	.byte	0
	.4byte	.LASF131
	.byte	0x5
	.byte	0
	.4byte	.LASF132
	.byte	0x5
	.byte	0
	.4byte	.LASF133
	.byte	0x5
	.byte	0
	.4byte	.LASF134
	.byte	0x5
	.byte	0
	.4byte	.LASF135
	.byte	0x5
	.byte	0
	.4byte	.LASF136
	.byte	0x5
	.byte	0
	.4byte	.LASF137
	.byte	0x5
	.byte	0
	.4byte	.LASF138
	.byte	0x5
	.byte	0
	.4byte	.LASF139
	.byte	0x5
	.byte	0
	.4byte	.LASF140
	.byte	0x5
	.byte	0
	.4byte	.LASF141
	.byte	0x5
	.byte	0
	.4byte	.LASF142
	.byte	0x5
	.byte	0
	.4byte	.LASF143
	.byte	0x5
	.byte	0
	.4byte	.LASF144
	.byte	0x5
	.byte	0
	.4byte	.LASF145
	.byte	0x5
	.byte	0
	.4byte	.LASF146
	.byte	0x5
	.byte	0
	.4byte	.LASF147
	.byte	0x5
	.byte	0
	.4byte	.LASF148
	.byte	0x5
	.byte	0
	.4byte	.LASF149
	.byte	0x5
	.byte	0
	.4byte	.LASF150
	.byte	0x5
	.byte	0
	.4byte	.LASF151
	.byte	0x5
	.byte	0
	.4byte	.LASF152
	.byte	0x5
	.byte	0
	.4byte	.LASF153
	.byte	0x5
	.byte	0
	.4byte	.LASF154
	.byte	0x5
	.byte	0
	.4byte	.LASF155
	.byte	0x5
	.byte	0
	.4byte	.LASF156
	.byte	0x5
	.byte	0
	.4byte	.LASF157
	.byte	0x5
	.byte	0
	.4byte	.LASF158
	.byte	0x5
	.byte	0
	.4byte	.LASF159
	.byte	0x5
	.byte	0
	.4byte	.LASF160
	.byte	0x5
	.byte	0
	.4byte	.LASF161
	.byte	0x5
	.byte	0
	.4byte	.LASF162
	.byte	0x5
	.byte	0
	.4byte	.LASF163
	.byte	0x5
	.byte	0
	.4byte	.LASF164
	.byte	0x5
	.byte	0
	.4byte	.LASF165
	.byte	0x5
	.byte	0
	.4byte	.LASF166
	.byte	0x5
	.byte	0
	.4byte	.LASF167
	.byte	0x5
	.byte	0
	.4byte	.LASF168
	.byte	0x5
	.byte	0
	.4byte	.LASF169
	.byte	0x5
	.byte	0
	.4byte	.LASF170
	.byte	0x5
	.byte	0
	.4byte	.LASF171
	.byte	0x5
	.byte	0
	.4byte	.LASF172
	.byte	0x5
	.byte	0
	.4byte	.LASF173
	.byte	0x5
	.byte	0
	.4byte	.LASF174
	.byte	0x5
	.byte	0
	.4byte	.LASF175
	.byte	0x5
	.byte	0
	.4byte	.LASF176
	.byte	0x5
	.byte	0
	.4byte	.LASF177
	.byte	0x5
	.byte	0
	.4byte	.LASF178
	.byte	0x5
	.byte	0
	.4byte	.LASF179
	.byte	0x5
	.byte	0
	.4byte	.LASF180
	.byte	0x5
	.byte	0
	.4byte	.LASF181
	.byte	0x5
	.byte	0
	.4byte	.LASF182
	.byte	0x5
	.byte	0
	.4byte	.LASF183
	.byte	0x5
	.byte	0
	.4byte	.LASF184
	.byte	0x5
	.byte	0
	.4byte	.LASF185
	.byte	0x5
	.byte	0
	.4byte	.LASF186
	.byte	0x5
	.byte	0
	.4byte	.LASF187
	.byte	0x5
	.byte	0
	.4byte	.LASF188
	.byte	0x5
	.byte	0
	.4byte	.LASF189
	.byte	0x5
	.byte	0
	.4byte	.LASF190
	.byte	0x5
	.byte	0
	.4byte	.LASF191
	.byte	0x5
	.byte	0
	.4byte	.LASF192
	.byte	0x5
	.byte	0
	.4byte	.LASF193
	.byte	0x5
	.byte	0
	.4byte	.LASF194
	.byte	0x5
	.byte	0
	.4byte	.LASF195
	.byte	0x5
	.byte	0
	.4byte	.LASF196
	.byte	0x5
	.byte	0
	.4byte	.LASF197
	.byte	0x5
	.byte	0
	.4byte	.LASF198
	.byte	0x5
	.byte	0
	.4byte	.LASF199
	.byte	0x5
	.byte	0
	.4byte	.LASF200
	.byte	0x5
	.byte	0
	.4byte	.LASF201
	.byte	0x5
	.byte	0
	.4byte	.LASF202
	.byte	0x5
	.byte	0
	.4byte	.LASF203
	.byte	0x5
	.byte	0
	.4byte	.LASF204
	.byte	0x5
	.byte	0
	.4byte	.LASF205
	.byte	0x5
	.byte	0
	.4byte	.LASF206
	.byte	0x5
	.byte	0
	.4byte	.LASF207
	.byte	0x5
	.byte	0
	.4byte	.LASF208
	.byte	0x5
	.byte	0
	.4byte	.LASF209
	.byte	0x5
	.byte	0
	.4byte	.LASF210
	.byte	0x5
	.byte	0
	.4byte	.LASF211
	.byte	0x5
	.byte	0
	.4byte	.LASF212
	.byte	0x5
	.byte	0
	.4byte	.LASF213
	.byte	0x5
	.byte	0
	.4byte	.LASF214
	.byte	0x5
	.byte	0
	.4byte	.LASF215
	.byte	0x5
	.byte	0
	.4byte	.LASF216
	.byte	0x5
	.byte	0
	.4byte	.LASF217
	.byte	0x5
	.byte	0
	.4byte	.LASF218
	.byte	0x5
	.byte	0
	.4byte	.LASF219
	.byte	0x5
	.byte	0
	.4byte	.LASF220
	.byte	0x5
	.byte	0
	.4byte	.LASF221
	.byte	0x5
	.byte	0
	.4byte	.LASF222
	.byte	0x5
	.byte	0
	.4byte	.LASF223
	.byte	0x5
	.byte	0
	.4byte	.LASF224
	.byte	0x5
	.byte	0
	.4byte	.LASF225
	.byte	0x5
	.byte	0
	.4byte	.LASF226
	.byte	0x5
	.byte	0
	.4byte	.LASF227
	.byte	0x5
	.byte	0
	.4byte	.LASF228
	.byte	0x5
	.byte	0
	.4byte	.LASF229
	.byte	0x5
	.byte	0
	.4byte	.LASF230
	.byte	0x5
	.byte	0
	.4byte	.LASF231
	.byte	0x5
	.byte	0
	.4byte	.LASF232
	.byte	0x5
	.byte	0
	.4byte	.LASF233
	.byte	0x5
	.byte	0
	.4byte	.LASF234
	.byte	0x5
	.byte	0
	.4byte	.LASF235
	.byte	0x5
	.byte	0
	.4byte	.LASF236
	.byte	0x5
	.byte	0
	.4byte	.LASF237
	.byte	0x5
	.byte	0
	.4byte	.LASF238
	.byte	0x5
	.byte	0
	.4byte	.LASF239
	.byte	0x5
	.byte	0
	.4byte	.LASF240
	.byte	0x5
	.byte	0
	.4byte	.LASF241
	.byte	0x5
	.byte	0
	.4byte	.LASF242
	.byte	0x5
	.byte	0
	.4byte	.LASF243
	.byte	0x5
	.byte	0
	.4byte	.LASF244
	.byte	0x5
	.byte	0
	.4byte	.LASF245
	.byte	0x5
	.byte	0
	.4byte	.LASF246
	.byte	0x5
	.byte	0
	.4byte	.LASF247
	.byte	0x5
	.byte	0
	.4byte	.LASF248
	.byte	0x5
	.byte	0
	.4byte	.LASF249
	.byte	0x5
	.byte	0
	.4byte	.LASF250
	.byte	0x5
	.byte	0
	.4byte	.LASF251
	.byte	0x5
	.byte	0
	.4byte	.LASF252
	.byte	0x5
	.byte	0
	.4byte	.LASF253
	.byte	0x5
	.byte	0
	.4byte	.LASF254
	.byte	0x5
	.byte	0
	.4byte	.LASF255
	.byte	0x5
	.byte	0
	.4byte	.LASF256
	.byte	0x5
	.byte	0
	.4byte	.LASF257
	.byte	0x5
	.byte	0
	.4byte	.LASF258
	.byte	0x5
	.byte	0
	.4byte	.LASF259
	.byte	0x5
	.byte	0
	.4byte	.LASF260
	.byte	0x5
	.byte	0
	.4byte	.LASF261
	.byte	0x5
	.byte	0
	.4byte	.LASF262
	.byte	0x5
	.byte	0
	.4byte	.LASF263
	.byte	0x5
	.byte	0
	.4byte	.LASF264
	.byte	0x5
	.byte	0
	.4byte	.LASF265
	.byte	0x5
	.byte	0
	.4byte	.LASF266
	.byte	0x5
	.byte	0
	.4byte	.LASF267
	.byte	0x5
	.byte	0
	.4byte	.LASF268
	.byte	0x5
	.byte	0
	.4byte	.LASF269
	.byte	0x5
	.byte	0
	.4byte	.LASF270
	.byte	0x5
	.byte	0
	.4byte	.LASF271
	.byte	0x5
	.byte	0
	.4byte	.LASF272
	.byte	0x5
	.byte	0
	.4byte	.LASF273
	.byte	0x5
	.byte	0
	.4byte	.LASF274
	.byte	0x5
	.byte	0
	.4byte	.LASF275
	.byte	0x5
	.byte	0
	.4byte	.LASF276
	.byte	0x5
	.byte	0
	.4byte	.LASF277
	.byte	0x5
	.byte	0
	.4byte	.LASF278
	.byte	0x5
	.byte	0
	.4byte	.LASF279
	.byte	0x5
	.byte	0
	.4byte	.LASF280
	.byte	0x5
	.byte	0
	.4byte	.LASF281
	.byte	0x5
	.byte	0
	.4byte	.LASF282
	.byte	0x5
	.byte	0
	.4byte	.LASF283
	.byte	0x5
	.byte	0
	.4byte	.LASF284
	.byte	0x5
	.byte	0
	.4byte	.LASF285
	.byte	0x5
	.byte	0
	.4byte	.LASF286
	.byte	0x5
	.byte	0
	.4byte	.LASF287
	.byte	0x5
	.byte	0
	.4byte	.LASF288
	.byte	0x5
	.byte	0
	.4byte	.LASF289
	.byte	0x5
	.byte	0
	.4byte	.LASF290
	.byte	0x5
	.byte	0
	.4byte	.LASF291
	.byte	0x5
	.byte	0
	.4byte	.LASF292
	.byte	0x5
	.byte	0
	.4byte	.LASF293
	.byte	0x5
	.byte	0
	.4byte	.LASF294
	.byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF132:
	.string	"__UINT_FAST8_MAX__ 0xffffffffU"
.LASF218:
	.string	"__FLT64_HAS_DENORM__ 1"
.LASF129:
	.string	"__INT_FAST32_WIDTH__ 32"
.LASF192:
	.string	"__FLT32_DIG__ 6"
.LASF23:
	.string	"__SIZEOF_SIZE_T__ 4"
.LASF44:
	.string	"__INT64_TYPE__ long long int"
.LASF231:
	.string	"__FLT128_EPSILON__ 1.92592994438723585305597794258492732e-34F128"
.LASF155:
	.string	"__FLT_EPSILON__ 1.19209289550781250000000000000000000e-7F"
.LASF114:
	.string	"__INT64_C(c) c ## LL"
.LASF31:
	.string	"__SIZEOF_POINTER__ 4"
.LASF55:
	.string	"__UINT_LEAST32_TYPE__ long unsigned int"
.LASF290:
	.string	"__riscv_muldiv 1"
.LASF84:
	.string	"__WCHAR_WIDTH__ 32"
.LASF144:
	.string	"__FLT_RADIX__ 2"
.LASF91:
	.string	"__UINTMAX_C(c) c ## ULL"
.LASF124:
	.string	"__INT_FAST8_MAX__ 0x7fffffff"
.LASF48:
	.string	"__UINT64_TYPE__ long long unsigned int"
.LASF240:
	.string	"__FLT32X_MAX_EXP__ 1024"
.LASF180:
	.string	"__LDBL_MAX_10_EXP__ 4932"
.LASF274:
	.string	"__GCC_ATOMIC_CHAR32_T_LOCK_FREE 1"
.LASF10:
	.string	"__ATOMIC_SEQ_CST 5"
.LASF213:
	.string	"__FLT64_MAX__ 1.79769313486231570814527423731704357e+308F64"
.LASF285:
	.string	"__SIZEOF_WINT_T__ 4"
.LASF165:
	.string	"__DBL_MAX_10_EXP__ 308"
.LASF215:
	.string	"__FLT64_MIN__ 2.22507385850720138309023271733240406e-308F64"
.LASF185:
	.string	"__LDBL_MIN__ 3.36210314311209350626267781732175260e-4932L"
.LASF112:
	.string	"__INT_LEAST32_WIDTH__ 32"
.LASF39:
	.string	"__CHAR32_TYPE__ long unsigned int"
.LASF175:
	.string	"__LDBL_MANT_DIG__ 113"
.LASF259:
	.string	"__FLT64X_NORM_MAX__ 1.18973149535723176508575932662800702e+4932F64x"
.LASF24:
	.string	"__CHAR_BIT__ 8"
.LASF173:
	.string	"__DBL_HAS_INFINITY__ 1"
.LASF1:
	.string	"__STDC_VERSION__ 201710L"
.LASF244:
	.string	"__FLT32X_NORM_MAX__ 1.79769313486231570814527423731704357e+308F32x"
.LASF239:
	.string	"__FLT32X_MIN_10_EXP__ (-307)"
.LASF246:
	.string	"__FLT32X_EPSILON__ 2.22044604925031308084726333618164062e-16F32x"
.LASF263:
	.string	"__FLT64X_HAS_DENORM__ 1"
.LASF142:
	.string	"__FLT_EVAL_METHOD_TS_18661_3__ 0"
.LASF216:
	.string	"__FLT64_EPSILON__ 2.22044604925031308084726333618164062e-16F64"
.LASF170:
	.string	"__DBL_EPSILON__ ((double)2.22044604925031308084726333618164062e-16L)"
.LASF254:
	.string	"__FLT64X_MIN_10_EXP__ (-4931)"
.LASF103:
	.string	"__UINT64_MAX__ 0xffffffffffffffffULL"
.LASF257:
	.string	"__FLT64X_DECIMAL_DIG__ 36"
.LASF75:
	.string	"__WINT_MAX__ 0xffffffffU"
.LASF32:
	.string	"__SIZE_TYPE__ unsigned int"
.LASF220:
	.string	"__FLT64_HAS_QUIET_NAN__ 1"
.LASF203:
	.string	"__FLT32_HAS_DENORM__ 1"
.LASF79:
	.string	"__SCHAR_WIDTH__ 8"
.LASF160:
	.string	"__DBL_MANT_DIG__ 53"
.LASF56:
	.string	"__UINT_LEAST64_TYPE__ long long unsigned int"
.LASF169:
	.string	"__DBL_MIN__ ((double)2.22507385850720138309023271733240406e-308L)"
.LASF65:
	.string	"__INTPTR_TYPE__ int"
.LASF113:
	.string	"__INT_LEAST64_MAX__ 0x7fffffffffffffffLL"
.LASF80:
	.string	"__SHRT_WIDTH__ 16"
.LASF45:
	.string	"__UINT8_TYPE__ unsigned char"
.LASF40:
	.string	"__SIG_ATOMIC_TYPE__ int"
.LASF199:
	.string	"__FLT32_NORM_MAX__ 3.40282346638528859811704183484516925e+38F32"
.LASF198:
	.string	"__FLT32_MAX__ 3.40282346638528859811704183484516925e+38F32"
.LASF275:
	.string	"__GCC_ATOMIC_WCHAR_T_LOCK_FREE 1"
.LASF283:
	.string	"__PRAGMA_REDEFINE_EXTNAME 1"
.LASF109:
	.string	"__INT_LEAST16_WIDTH__ 16"
.LASF280:
	.string	"__GCC_ATOMIC_TEST_AND_SET_TRUEVAL 1"
.LASF183:
	.string	"__LDBL_MAX__ 1.18973149535723176508575932662800702e+4932L"
.LASF204:
	.string	"__FLT32_HAS_INFINITY__ 1"
.LASF189:
	.string	"__LDBL_HAS_INFINITY__ 1"
.LASF157:
	.string	"__FLT_HAS_DENORM__ 1"
.LASF223:
	.string	"__FLT128_MIN_EXP__ (-16381)"
.LASF133:
	.string	"__UINT_FAST16_MAX__ 0xffffffffU"
.LASF136:
	.string	"__INTPTR_MAX__ 0x7fffffff"
.LASF101:
	.string	"__UINT16_MAX__ 0xffff"
.LASF41:
	.string	"__INT8_TYPE__ signed char"
.LASF29:
	.string	"__BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__"
.LASF99:
	.string	"__INT64_MAX__ 0x7fffffffffffffffLL"
.LASF168:
	.string	"__DBL_NORM_MAX__ ((double)1.79769313486231570814527423731704357e+308L)"
.LASF30:
	.string	"__FLOAT_WORD_ORDER__ __ORDER_LITTLE_ENDIAN__"
.LASF118:
	.string	"__UINT_LEAST16_MAX__ 0xffff"
.LASF43:
	.string	"__INT32_TYPE__ long int"
.LASF50:
	.string	"__INT_LEAST16_TYPE__ short int"
.LASF58:
	.string	"__INT_FAST16_TYPE__ int"
.LASF88:
	.string	"__INTMAX_MAX__ 0x7fffffffffffffffLL"
.LASF156:
	.string	"__FLT_DENORM_MIN__ 1.40129846432481707092372958328991613e-45F"
.LASF187:
	.string	"__LDBL_DENORM_MIN__ 6.47517511943802511092443895822764655e-4966L"
.LASF21:
	.string	"__SIZEOF_DOUBLE__ 8"
.LASF294:
	.string	"__ELF__ 1"
.LASF100:
	.string	"__UINT8_MAX__ 0xff"
.LASF78:
	.string	"__SIZE_MAX__ 0xffffffffU"
.LASF282:
	.string	"__GCC_HAVE_DWARF2_CFI_ASM 1"
.LASF195:
	.string	"__FLT32_MAX_EXP__ 128"
.LASF111:
	.string	"__INT32_C(c) c ## L"
.LASF210:
	.string	"__FLT64_MAX_EXP__ 1024"
.LASF137:
	.string	"__INTPTR_WIDTH__ 32"
.LASF87:
	.string	"__SIZE_WIDTH__ 32"
.LASF201:
	.string	"__FLT32_EPSILON__ 1.19209289550781250000000000000000000e-7F32"
.LASF37:
	.string	"__UINTMAX_TYPE__ long long unsigned int"
.LASF95:
	.string	"__SIG_ATOMIC_WIDTH__ 32"
.LASF135:
	.string	"__UINT_FAST64_MAX__ 0xffffffffffffffffULL"
.LASF177:
	.string	"__LDBL_MIN_EXP__ (-16381)"
.LASF18:
	.string	"__SIZEOF_LONG_LONG__ 8"
.LASF146:
	.string	"__FLT_DIG__ 6"
.LASF82:
	.string	"__LONG_WIDTH__ 32"
.LASF171:
	.string	"__DBL_DENORM_MIN__ ((double)4.94065645841246544176568792868221372e-324L)"
.LASF12:
	.string	"__ATOMIC_RELEASE 3"
.LASF150:
	.string	"__FLT_MAX_10_EXP__ 38"
.LASF269:
	.string	"__NO_INLINE__ 1"
.LASF76:
	.string	"__WINT_MIN__ 0U"
.LASF22:
	.string	"__SIZEOF_LONG_DOUBLE__ 16"
.LASF105:
	.string	"__INT8_C(c) c"
.LASF200:
	.string	"__FLT32_MIN__ 1.17549435082228750796873653722224568e-38F32"
.LASF6:
	.string	"__GNUC_MINOR__ 2"
.LASF298:
	.string	"/home/a26rahma/work/alsaqr/final_testing_mohamed/pmu-pic"
.LASF260:
	.string	"__FLT64X_MIN__ 3.36210314311209350626267781732175260e-4932F64x"
.LASF143:
	.string	"__DEC_EVAL_METHOD__ 2"
.LASF72:
	.string	"__LONG_LONG_MAX__ 0x7fffffffffffffffLL"
.LASF5:
	.string	"__GNUC__ 10"
.LASF234:
	.string	"__FLT128_HAS_INFINITY__ 1"
.LASF127:
	.string	"__INT_FAST16_WIDTH__ 32"
.LASF191:
	.string	"__FLT32_MANT_DIG__ 24"
.LASF159:
	.string	"__FLT_HAS_QUIET_NAN__ 1"
.LASF4:
	.string	"__STDC_HOSTED__ 0"
.LASF147:
	.string	"__FLT_MIN_EXP__ (-125)"
.LASF245:
	.string	"__FLT32X_MIN__ 2.22507385850720138309023271733240406e-308F32x"
.LASF250:
	.string	"__FLT32X_HAS_QUIET_NAN__ 1"
.LASF181:
	.string	"__DECIMAL_DIG__ 36"
.LASF292:
	.string	"__riscv_float_abi_soft 1"
.LASF27:
	.string	"__ORDER_BIG_ENDIAN__ 4321"
.LASF38:
	.string	"__CHAR16_TYPE__ short unsigned int"
.LASF178:
	.string	"__LDBL_MIN_10_EXP__ (-4931)"
.LASF36:
	.string	"__INTMAX_TYPE__ long long int"
.LASF277:
	.string	"__GCC_ATOMIC_INT_LOCK_FREE 1"
.LASF19:
	.string	"__SIZEOF_SHORT__ 2"
.LASF154:
	.string	"__FLT_MIN__ 1.17549435082228750796873653722224568e-38F"
.LASF219:
	.string	"__FLT64_HAS_INFINITY__ 1"
.LASF279:
	.string	"__GCC_ATOMIC_LLONG_LOCK_FREE 1"
.LASF108:
	.string	"__INT16_C(c) c"
.LASF264:
	.string	"__FLT64X_HAS_INFINITY__ 1"
.LASF149:
	.string	"__FLT_MAX_EXP__ 128"
.LASF172:
	.string	"__DBL_HAS_DENORM__ 1"
.LASF262:
	.string	"__FLT64X_DENORM_MIN__ 6.47517511943802511092443895822764655e-4966F64x"
.LASF224:
	.string	"__FLT128_MIN_10_EXP__ (-4931)"
.LASF70:
	.string	"__INT_MAX__ 0x7fffffff"
.LASF233:
	.string	"__FLT128_HAS_DENORM__ 1"
.LASF68:
	.string	"__SCHAR_MAX__ 0x7f"
.LASF7:
	.string	"__GNUC_PATCHLEVEL__ 0"
.LASF194:
	.string	"__FLT32_MIN_10_EXP__ (-37)"
.LASF166:
	.string	"__DBL_DECIMAL_DIG__ 17"
.LASF161:
	.string	"__DBL_DIG__ 15"
.LASF151:
	.string	"__FLT_DECIMAL_DIG__ 9"
.LASF60:
	.string	"__INT_FAST64_TYPE__ long long int"
.LASF25:
	.string	"__BIGGEST_ALIGNMENT__ 16"
.LASF16:
	.string	"__SIZEOF_INT__ 4"
.LASF116:
	.string	"__UINT_LEAST8_MAX__ 0xff"
.LASF241:
	.string	"__FLT32X_MAX_10_EXP__ 308"
.LASF97:
	.string	"__INT16_MAX__ 0x7fff"
.LASF225:
	.string	"__FLT128_MAX_EXP__ 16384"
.LASF258:
	.string	"__FLT64X_MAX__ 1.18973149535723176508575932662800702e+4932F64x"
.LASF81:
	.string	"__INT_WIDTH__ 32"
.LASF145:
	.string	"__FLT_MANT_DIG__ 24"
.LASF92:
	.string	"__INTMAX_WIDTH__ 64"
.LASF270:
	.string	"__CHAR_UNSIGNED__ 1"
.LASF57:
	.string	"__INT_FAST8_TYPE__ int"
.LASF196:
	.string	"__FLT32_MAX_10_EXP__ 38"
.LASF276:
	.string	"__GCC_ATOMIC_SHORT_LOCK_FREE 1"
.LASF102:
	.string	"__UINT32_MAX__ 0xffffffffUL"
.LASF130:
	.string	"__INT_FAST64_MAX__ 0x7fffffffffffffffLL"
.LASF236:
	.string	"__FLT32X_MANT_DIG__ 53"
.LASF106:
	.string	"__INT_LEAST8_WIDTH__ 8"
.LASF54:
	.string	"__UINT_LEAST16_TYPE__ short unsigned int"
.LASF164:
	.string	"__DBL_MAX_EXP__ 1024"
.LASF138:
	.string	"__UINTPTR_MAX__ 0xffffffffU"
.LASF140:
	.string	"__GCC_IEC_559_COMPLEX 0"
.LASF243:
	.string	"__FLT32X_MAX__ 1.79769313486231570814527423731704357e+308F32x"
.LASF11:
	.string	"__ATOMIC_ACQUIRE 2"
.LASF197:
	.string	"__FLT32_DECIMAL_DIG__ 9"
.LASF90:
	.string	"__UINTMAX_MAX__ 0xffffffffffffffffULL"
.LASF297:
	.string	"main.c"
.LASF152:
	.string	"__FLT_MAX__ 3.40282346638528859811704183484516925e+38F"
.LASF284:
	.string	"__SIZEOF_WCHAR_T__ 4"
.LASF242:
	.string	"__FLT32X_DECIMAL_DIG__ 17"
.LASF272:
	.string	"__GCC_ATOMIC_CHAR_LOCK_FREE 1"
.LASF63:
	.string	"__UINT_FAST32_TYPE__ unsigned int"
.LASF295:
	.string	"uart"
.LASF289:
	.string	"__riscv_div 1"
.LASF209:
	.string	"__FLT64_MIN_10_EXP__ (-307)"
.LASF207:
	.string	"__FLT64_DIG__ 15"
.LASF47:
	.string	"__UINT32_TYPE__ long unsigned int"
.LASF121:
	.string	"__UINT32_C(c) c ## UL"
.LASF51:
	.string	"__INT_LEAST32_TYPE__ long int"
.LASF107:
	.string	"__INT_LEAST16_MAX__ 0x7fff"
.LASF235:
	.string	"__FLT128_HAS_QUIET_NAN__ 1"
.LASF85:
	.string	"__WINT_WIDTH__ 32"
.LASF227:
	.string	"__FLT128_DECIMAL_DIG__ 36"
.LASF74:
	.string	"__WCHAR_MIN__ (-__WCHAR_MAX__ - 1)"
.LASF273:
	.string	"__GCC_ATOMIC_CHAR16_T_LOCK_FREE 1"
.LASF293:
	.string	"__riscv_cmodel_medany 1"
.LASF179:
	.string	"__LDBL_MAX_EXP__ 16384"
.LASF237:
	.string	"__FLT32X_DIG__ 15"
.LASF248:
	.string	"__FLT32X_HAS_DENORM__ 1"
.LASF89:
	.string	"__INTMAX_C(c) c ## LL"
.LASF153:
	.string	"__FLT_NORM_MAX__ 3.40282346638528859811704183484516925e+38F"
.LASF77:
	.string	"__PTRDIFF_MAX__ 0x7fffffff"
.LASF141:
	.string	"__FLT_EVAL_METHOD__ 0"
.LASF49:
	.string	"__INT_LEAST8_TYPE__ signed char"
.LASF119:
	.string	"__UINT16_C(c) c"
.LASF69:
	.string	"__SHRT_MAX__ 0x7fff"
.LASF20:
	.string	"__SIZEOF_FLOAT__ 4"
.LASF73:
	.string	"__WCHAR_MAX__ 0x7fffffff"
.LASF232:
	.string	"__FLT128_DENORM_MIN__ 6.47517511943802511092443895822764655e-4966F128"
.LASF0:
	.string	"__STDC__ 1"
.LASF28:
	.string	"__ORDER_PDP_ENDIAN__ 3412"
.LASF214:
	.string	"__FLT64_NORM_MAX__ 1.79769313486231570814527423731704357e+308F64"
.LASF26:
	.string	"__ORDER_LITTLE_ENDIAN__ 1234"
.LASF35:
	.string	"__WINT_TYPE__ unsigned int"
.LASF52:
	.string	"__INT_LEAST64_TYPE__ long long int"
.LASF9:
	.string	"__ATOMIC_RELAXED 0"
.LASF110:
	.string	"__INT_LEAST32_MAX__ 0x7fffffffL"
.LASF139:
	.string	"__GCC_IEC_559 0"
.LASF247:
	.string	"__FLT32X_DENORM_MIN__ 4.94065645841246544176568792868221372e-324F32x"
.LASF251:
	.string	"__FLT64X_MANT_DIG__ 113"
.LASF288:
	.string	"__riscv_mul 1"
.LASF265:
	.string	"__FLT64X_HAS_QUIET_NAN__ 1"
.LASF190:
	.string	"__LDBL_HAS_QUIET_NAN__ 1"
.LASF93:
	.string	"__SIG_ATOMIC_MAX__ 0x7fffffff"
.LASF162:
	.string	"__DBL_MIN_EXP__ (-1021)"
.LASF300:
	.string	"char"
.LASF249:
	.string	"__FLT32X_HAS_INFINITY__ 1"
.LASF221:
	.string	"__FLT128_MANT_DIG__ 113"
.LASF66:
	.string	"__UINTPTR_TYPE__ unsigned int"
.LASF184:
	.string	"__LDBL_NORM_MAX__ 1.18973149535723176508575932662800702e+4932L"
.LASF123:
	.string	"__UINT64_C(c) c ## ULL"
.LASF206:
	.string	"__FLT64_MANT_DIG__ 53"
.LASF158:
	.string	"__FLT_HAS_INFINITY__ 1"
.LASF212:
	.string	"__FLT64_DECIMAL_DIG__ 17"
.LASF83:
	.string	"__LONG_LONG_WIDTH__ 64"
.LASF217:
	.string	"__FLT64_DENORM_MIN__ 4.94065645841246544176568792868221372e-324F64"
.LASF42:
	.string	"__INT16_TYPE__ short int"
.LASF17:
	.string	"__SIZEOF_LONG__ 4"
.LASF134:
	.string	"__UINT_FAST32_MAX__ 0xffffffffU"
.LASF2:
	.string	"__STDC_UTF_16__ 1"
.LASF186:
	.string	"__LDBL_EPSILON__ 1.92592994438723585305597794258492732e-34L"
.LASF61:
	.string	"__UINT_FAST8_TYPE__ unsigned int"
.LASF33:
	.string	"__PTRDIFF_TYPE__ int"
.LASF188:
	.string	"__LDBL_HAS_DENORM__ 1"
.LASF120:
	.string	"__UINT_LEAST32_MAX__ 0xffffffffUL"
.LASF261:
	.string	"__FLT64X_EPSILON__ 1.92592994438723585305597794258492732e-34F64x"
.LASF128:
	.string	"__INT_FAST32_MAX__ 0x7fffffff"
.LASF266:
	.string	"__REGISTER_PREFIX__ "
.LASF14:
	.string	"__ATOMIC_CONSUME 1"
.LASF182:
	.string	"__LDBL_DECIMAL_DIG__ 36"
.LASF3:
	.string	"__STDC_UTF_32__ 1"
.LASF46:
	.string	"__UINT16_TYPE__ short unsigned int"
.LASF256:
	.string	"__FLT64X_MAX_10_EXP__ 4932"
.LASF176:
	.string	"__LDBL_DIG__ 33"
.LASF253:
	.string	"__FLT64X_MIN_EXP__ (-16381)"
.LASF205:
	.string	"__FLT32_HAS_QUIET_NAN__ 1"
.LASF286:
	.string	"__SIZEOF_PTRDIFF_T__ 4"
.LASF228:
	.string	"__FLT128_MAX__ 1.18973149535723176508575932662800702e+4932F128"
.LASF67:
	.string	"__GXX_ABI_VERSION 1014"
.LASF126:
	.string	"__INT_FAST16_MAX__ 0x7fffffff"
.LASF94:
	.string	"__SIG_ATOMIC_MIN__ (-__SIG_ATOMIC_MAX__ - 1)"
.LASF131:
	.string	"__INT_FAST64_WIDTH__ 64"
.LASF230:
	.string	"__FLT128_MIN__ 3.36210314311209350626267781732175260e-4932F128"
.LASF59:
	.string	"__INT_FAST32_TYPE__ int"
.LASF202:
	.string	"__FLT32_DENORM_MIN__ 1.40129846432481707092372958328991613e-45F32"
.LASF62:
	.string	"__UINT_FAST16_TYPE__ unsigned int"
.LASF296:
	.string	"GNU C17 10.2.0 -march=rv32im -mabi=ilp32 -mcmodel=medany -march=rv32im -ggdb3 -O0 -fvisibility=hidden -ffreestanding"
.LASF287:
	.string	"__riscv 1"
.LASF278:
	.string	"__GCC_ATOMIC_LONG_LOCK_FREE 1"
.LASF167:
	.string	"__DBL_MAX__ ((double)1.79769313486231570814527423731704357e+308L)"
.LASF252:
	.string	"__FLT64X_DIG__ 33"
.LASF96:
	.string	"__INT8_MAX__ 0x7f"
.LASF174:
	.string	"__DBL_HAS_QUIET_NAN__ 1"
.LASF148:
	.string	"__FLT_MIN_10_EXP__ (-37)"
.LASF125:
	.string	"__INT_FAST8_WIDTH__ 32"
.LASF13:
	.string	"__ATOMIC_ACQ_REL 4"
.LASF34:
	.string	"__WCHAR_TYPE__ int"
.LASF229:
	.string	"__FLT128_NORM_MAX__ 1.18973149535723176508575932662800702e+4932F128"
.LASF291:
	.string	"__riscv_xlen 32"
.LASF15:
	.string	"__FINITE_MATH_ONLY__ 0"
.LASF71:
	.string	"__LONG_MAX__ 0x7fffffffL"
.LASF86:
	.string	"__PTRDIFF_WIDTH__ 32"
.LASF267:
	.string	"__USER_LABEL_PREFIX__ "
.LASF255:
	.string	"__FLT64X_MAX_EXP__ 16384"
.LASF193:
	.string	"__FLT32_MIN_EXP__ (-125)"
.LASF226:
	.string	"__FLT128_MAX_10_EXP__ 4932"
.LASF64:
	.string	"__UINT_FAST64_TYPE__ long long unsigned int"
.LASF117:
	.string	"__UINT8_C(c) c"
.LASF222:
	.string	"__FLT128_DIG__ 33"
.LASF53:
	.string	"__UINT_LEAST8_TYPE__ unsigned char"
.LASF115:
	.string	"__INT_LEAST64_WIDTH__ 64"
.LASF98:
	.string	"__INT32_MAX__ 0x7fffffffL"
.LASF299:
	.string	"main"
.LASF238:
	.string	"__FLT32X_MIN_EXP__ (-1021)"
.LASF281:
	.string	"__GCC_ATOMIC_POINTER_LOCK_FREE 1"
.LASF104:
	.string	"__INT_LEAST8_MAX__ 0x7f"
.LASF271:
	.string	"__GCC_ATOMIC_BOOL_LOCK_FREE 1"
.LASF122:
	.string	"__UINT_LEAST64_MAX__ 0xffffffffffffffffULL"
.LASF163:
	.string	"__DBL_MIN_10_EXP__ (-307)"
.LASF8:
	.string	"__VERSION__ \"10.2.0\""
.LASF268:
	.string	"__GNUC_STDC_INLINE__ 1"
.LASF211:
	.string	"__FLT64_MAX_10_EXP__ 308"
.LASF208:
	.string	"__FLT64_MIN_EXP__ (-1021)"
	.ident	"GCC: (crosstool-NG 1.24.0.498_5075e1f) 10.2.0"
