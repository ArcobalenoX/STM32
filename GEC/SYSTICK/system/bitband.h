#ifndef __BITBAND_H_
#define __BITBAND_H_


#define PAout(n) (*(vu32*)(0x42000000+(0x20000+0x14)*32+n*4))
#define PBout(n) (*(vu32*)(0x42000000+(0x20400+0x14)*32+n*4))
#define PCout(n) (*(vu32*)(0x42000000+(0x20800+0x14)*32+n*4))
#define PDout(n) (*(vu32*)(0x42000000+(0x20C00+0x14)*32+n*4))
#define PEout(n) (*(vu32*)(0x42000000+(0x21000+0x14)*32+n*4))
#define PFout(n) (*(vu32*)(0x42000000+(0x21400+0x14)*32+n*4))
#define PGout(n) (*(vu32*)(0x42000000+(0x21800+0x14)*32+n*4))

#define PAin(n) (*(vu32*)(0x42000000+(0x20000+0x10)*32+n*4))
#define PBin(n) (*(vu32*)(0x42000000+(0x20400+0x10)*32+n*4))
#define PCin(n) (*(vu32*)(0x42000000+(0x20800+0x10)*32+n*4))
#define PDin(n) (*(vu32*)(0x42000000+(0x20C00+0x10)*32+n*4))
#define PEin(n) (*(vu32*)(0x42000000+(0x21000+0x10)*32+n*4))
#define PFin(n) (*(vu32*)(0x42000000+(0x21400+0x10)*32+n*4))
#define PGin(n) (*(vu32*)(0x42000000+(0x21800+0x10)*32+n*4))



#endif
