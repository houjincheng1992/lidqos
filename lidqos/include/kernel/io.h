/*
 * io.h
 * Copyright (C) Feb 10, 2014 by lidq
 *
 * 端口输入输出头文件
 */

#ifndef _IO_H_
#define _IO_H_

#include <kernel/typedef.h>

/***
 * 关中断
 */
#define cli()	\
	({__asm__ volatile("cli");})

/***
 * 打中断
 */
#define sti()	\
	({__asm__ volatile("sti");})

/*
 * hlt
 */
#define hlt()	\
	({__asm__ volatile("hlt");})

/***
 * 载入gdt
 */
#define load_gdt(gdtp)	\
	({	\
		__asm__ volatile("lgdt  %0" :: "m"(gdtp));	\
	})

/***
 * 载入idt
 */
#define load_idt(idtp)	\
	({	\
		__asm__ volatile("lidt  %0"::"m"(idtp));	\
	})

/*
 * struct gdt_s
 */
#define load_tss(tss_index)	\
	({	\
		__asm__ volatile("ltrw	%%ax"::"a"(tss_index));	\
	})

/*
 * struct idt_s
 */
#define load_ldt(ldt_index)	\
	({	\
		__asm__ volatile("lldt	%%ax"::"a"(ldt_index));	\
	})

#define call_tss()	\
	({	\
		__asm__ volatile("call $0x20, $0");	\
	})

static inline void set_cr3(u32 cr3)
{
	__asm__ volatile("movl	%%eax, %%cr3" :: "a"(cr3));
}

static inline void open_mm_page()
{
	__asm__ volatile(
			"movl	%cr0, %eax;"
			"orl	$0x80000000, %eax;"
			"movl    %eax, %cr0;"
	);
}

static inline void set_ds(u16 gdt_ind)
{
	__asm__ volatile("movw	%%ax, %%ds" :: "a"(gdt_ind));
}

static inline void set_es(u16 gdt_ind)
{
	__asm__ volatile("movw	%%ax, %%es" :: "a"(gdt_ind));
}

static inline void set_ss(u16 gdt_ind)
{
	__asm__ volatile("movw	%%ax, %%ss" :: "a"(gdt_ind));
}

static inline void set_fs(u16 gdt_ind)
{
	__asm__ volatile("movw	%%ax, %%fs" :: "a"(gdt_ind));
}

static inline void set_gs(u16 gdt_ind)
{
	__asm__ volatile("movw	%%ax, %%gs" :: "a"(gdt_ind));
}

/***
 * 向端口写一个字节
 */
static inline void outb_p(u8 val, u16 port)
{
	__asm__ volatile("outb	%0, %1" : : "a" (val), "dN" (port));
}

#define insl(buf, port, nr) 				\
		__asm__ ("cld;rep;insl\n\t"			\
		::"d"(port), "D"(buf), "c"(nr))

#define outsl(buf, port, nr ) 				\
		__asm__ ("cld;rep;outsl\n\t"		\
		::"d"(port), "S" (buf), "c" (nr))

//open FPU
#define open_fpu()	\
	({__asm__ volatile( "movl    %cr0, %eax			\n\t"	\
						"andl    $0xfffffff1, %eax	\n\t"	\
						"movl    %eax, %cr0			\n\t"	\
	);})

//close FPU
#define close_fpu()	\
	({__asm__ volatile( "movl    %cr0, %eax		\n\t"	\
						"orl     $0x6, %eax		\n\t"	\
						"movl    %eax, %cr0		\n\t"	\
	);})

//save FPU
#define save_fpu(fpu_addr)	\
	({__asm__ volatile( "fxsave	%0;"					\
						"finit;" :: "m"(fpu_addr)		\
	);})

//restore FPU
#define restore_fpu(fpu_addr)	\
	({__asm__ volatile("finit;fxrstor	%0;" ::"m"(fpu_addr)		\
	);})

/**
 * 从端口中读入一个字节
 */
static inline u8 inb_p(u16 port)
{
	u8 val;
	__asm__ volatile("inb	%%dx, %%al" :"=a"(val) : "dx"(port));
	return val;
}

static inline void outw_p(u16 val, u16 port)
{
	__asm__ volatile("outw	%0, %1" : : "a" (val), "dN" (port));
}

static inline u16 inw_p(u16 port)
{
	u16 val;
	__asm__ volatile("inw	%%dx, %%al" :"=a"(val) : "dx"(port));
	return val;
}

static inline void insl_p(u32 buf, u16 port, u32 count)
{
	__asm__ volatile("cld;rep;insl\n\t" ::"d"(port), "D"(buf), "c"(count<<9));
}

static inline void outsl_p(u32 buf, u16 port, u32 count)
{
	__asm__ volatile("cld;rep;outsl\n\t" ::"d"(port), "D" (buf), "c" (count<<9));
}

static inline u16 ds()
{
	u16 ds;
	__asm__ volatile("movw	%%ds, %0" : "=a" (ds) : );
	return ds;
}

static inline u16 cs()
{
	u16 cs;
	__asm__ volatile("movw	%%cs, %0" : "=a" (cs) : );
	return cs;
}

static inline u32 cr2()
{
	u32 cr2;
	__asm__ volatile("movl	%%cr2, %0" : "=a" (cr2) : );
	return cr2;
}

static inline u32 cr3()
{
	u32 cr3;
	__asm__ volatile("movl	%%cr3, %0" : "=a" (cr3) : );
	return cr3;
}

#endif
