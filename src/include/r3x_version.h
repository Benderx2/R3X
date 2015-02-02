/*
Copyright (c) 2015 Benderx2, 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of R3X nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef R3X_VERSION_H
#define R3X_VERSION_H
#ifdef LINUX_ARCH_X8664
	#define R3X_SYSTEM_VERSION "R3X Virtual Machine 0.4.0a BASICPie, compiled for: Linux64"
#endif
#ifdef LINUX_ARCH_X8632
	#define R3X_SYSTEM_VERSION "R3X Virtual Machine 0.4.0a BASICPie, compiled for: Linux32"
#endif
#ifdef LINUX_ARCH_ARM64
	#define R3X_SYSTEM_VERSION "R3X Virtual Machine 0.4.6a BASICPie, compiled for: LinuxARM64"
#endif
#ifdef LINUX_ARCH_PPC 
	#define R3X_SYSTEM_VERSION "R3X Virtual Machine 0.4.6a BASICPie, compiled for: LinuxPPC"
#endif
#ifdef WIN_ARCH_X8632
	#define R3X_SYSTEM_VERSION "R3X Virtual Machine 0.4.6a BASICPie, compiled for: Windows32"
#endif
#ifdef WIN_ARCH_X8664
	#define R3X_SYSTEM_VERSION "R3X Virtual Machine 0.4.6a BASICPie, compiled for: Windows64"
#endif
#endif
