#pragma once
#ifndef CM_COMPILER_H
#define CM_COMPILER_H

#ifndef __MSCVER__

#define abstract =0

#endif

#ifndef CM_ALIGNMENT
#define CM_ALIGNMENT 64
#endif

#define CM_ALIGNAS alignas(CM_ALIGNMENT)

#endif