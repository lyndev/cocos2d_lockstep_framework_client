#include "MemoryMonitor.h"

using namespace cocos2d;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include <mach/mach.h>
    #include <sys/sysctl.h>
#endif

#ifdef WIN32
    #include <Windows.h>
    #include <psapi.h>
    #pragma comment(lib,"psapi.lib")
#endif

CMemoryMonitor* CMemoryMonitor::s_pSharedInstance = NULL;

/***************************************************************
* 函数功能: 游戏使用内存情况
* 参数说明:
* 返回说明: MB
***************************************************************/
double CMemoryMonitor::GameUsedMemory()
{
    double result = 0.0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//TODO:这里读取android的内存情况
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    task_basic_info_data_t taskInfo;
    mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
    kern_return_t kernReturn = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&taskInfo, &infoCount);
    if(kernReturn != KERN_SUCCESS) {
        return result;
    }
    result = (taskInfo.resident_size / 1024.0) / 1024.0;
#endif

#ifdef WIN32
    HANDLE handle = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(handle,&pmc,sizeof(pmc));
    result = (pmc.WorkingSetSize / 1024.0) / 1024.0; 
#endif
    return result;
}

/***************************************************************
* 函数功能: 终端剩余可用内存
* 参数说明:
* 返回说明: MB
***************************************************************/
double CMemoryMonitor::AvailableMemory()
{
    double result = 0.0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//TODO:这里读取android的内存情况
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    vm_statistics_data_t vmStats;
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
    if(kernReturn != KERN_SUCCESS)
    {
        return result;
    }
    result = ((vm_page_size * vmStats.free_count) / 1024.0) / 1024.0;
#endif

#ifdef WIN32
    MEMORYSTATUS memstatus;
    memset(&memstatus,0,sizeof(MEMORYSTATUS));
    memstatus.dwLength =sizeof(MEMORYSTATUS);
    GlobalMemoryStatus(&memstatus);
    result = (memstatus.dwAvailPhys / 1024.0) /1024.0; 
#endif
    return result;
}

/***************************************************************
* 函数功能: 打印内存信息
* 参数说明: 无
* 返回说明: 无
***************************************************************/
void CMemoryMonitor::PrintMemory(char* msg)
{
	return;
#ifdef COCOS2D_DEBUG 
	double iCan = AvailableMemory();
	double iUse = GameUsedMemory();
	printf("==========================================C\n");
	if(msg == NULL)
	{
		msg = "";
	}
	printf("=== %s, All=%fM, Used=%fM ===\n", msg, iCan, iUse);
	printf("==========================================\n");
#endif
}