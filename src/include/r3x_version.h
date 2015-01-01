#ifndef __R3X_VERSION_H
#define __R3X_VERSION_H
#ifdef LINUX_ARCH_X8664
	#define R3X_SYSTEM_VERSION "R3X Virtual Machine 0.1.7 Cripsy Pie, compiled for: Linux64"
#endif
#ifdef LINUX_ARCH_X8632
	#define R3X_SYSTEM_VERSION "R3X Virtual Machine 0.1.7 Cripsy Pie, compiled for: Linux32"
#endif
#ifdef LINUX_ARCH_ARM64
	#define R3X_SYSTEM_VERSION "R3X Virtual Machine 0.1.7 Cripsy Pie, compiled for: LinuxARM64"
#endif
 
#endif
