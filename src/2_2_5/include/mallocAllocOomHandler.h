#ifndef _MALLOC_ALLOC_OOM_HANDLER_H_
#define _MALLOC_ALLOC_OOM_HANDLER_H_

#include <windef.h>
#include <winbase.h>

/*在 Windows 环境下的一个简易 OOM 处理例程*/

#define BIG_MEMORY 1024 * 1024 * 100

static void ReleaseMemory();
static void myAllocHandler();

#endif // _MALLOC_ALLOC_OOM_HANDLER_H_