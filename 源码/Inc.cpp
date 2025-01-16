#include <iostream>
#include <windows.h>
using namespace std;

#pragma pack(1) //字节对齐
typedef struct _PARTITION_ENTRY//分区表结构
{
	UCHAR active; //状态（是否被激活）   重要
	UCHAR StartHead; //分区起始磁头号   
	USHORT StartSecCyli; //与63相位与得出的是开始扇区，把它右移6位就是开始柱面
	UCHAR PartitionType; // 分区类型   重要 
	UCHAR EndHead; //分区结束磁头号
	USHORT EndSecCyli; //与63相位与得出的就是结束扇区，把它右移6位就是结束柱面
	ULONG StartLBA; // 扇区起始逻辑地址（相对扇区号）   重要
	ULONG TotalSector; // 分区大小      重要
} PARTITION_ENTRY, *PPARTITION_ENTRY;

//引导区512BYTE结构
typedef struct _MBR_SECTOR
{
	UCHAR BootCode[440];//启动记录440 Byte
	ULONG DiskSignature;//磁盘签名
	USHORT NoneDisk;//二个字节
	PARTITION_ENTRY Partition[4];//分区表结构64 Byte
	USHORT Signature;//结束标志2 Byte 55 AA
} MBR_SECTOR, *PMBR_SECTOR;

#pragma pack()

int main()
{
	TCHAR szDevicename[64]={0};
	MBR_SECTOR _ReadMbr;
	wsprintf(szDevicename,L"\\\\.\\PHYSICALDRIVE0");
	HANDLE hDevice=CreateFile(szDevicename,GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
	if(hDevice==INVALID_HANDLE_VALUE)
	{
		cout<<"打开设备出错"<<endl;
		return -1;
	}
	memset(&_ReadMbr,0,sizeof(MBR_SECTOR));
	DWORD leng=512;
	DWORD count;
	DeviceIoControl(hDevice,FSCTL_LOCK_VOLUME,NULL,0,NULL,0,&count,NULL);
	UCHAR sz[512]={0};
	//ReadFile(hDevice,sz,512,&leng,NULL);
	BOOL bcheck=ReadFile(hDevice,&_ReadMbr,512,&leng,NULL);
	memcpy(sz,&_ReadMbr,512);
	for(int i=0;i<66;i++)
	{
		printf("%02X ",sz[446+i]);
		if((i+1)%16==0)
			printf("\n");
	}
	printf("\n");
	if(bcheck==FALSE && leng<512)
	{
		cout<<"读取MBR出错！"<<endl;
		DeviceIoControl(hDevice,FSCTL_UNLOCK_VOLUME,NULL,0,NULL,0,&count,NULL);
		CloseHandle(hDevice);
		return -1;
	}
	char *szTemp=new char[64];
	for(int i=0;i<4;i++)
	{
		if(_ReadMbr.Partition[i].PartitionType==0)
		{
			continue;
		}
		memset(szTemp,0,64);
		if(_ReadMbr.Partition[i].active==128)
		{
			cout<<"激活分区"<<endl;
		}else
			cout<<"非激活分区"<<endl;
		memset(szTemp,0,64);
		sprintf(szTemp,"激活分区标志位:%02X",_ReadMbr.Partition[i].active);
		cout<<szTemp<<endl;
		memset(szTemp,0,64);
		sprintf(szTemp,"分区起始磁头号:%d",_ReadMbr.Partition[i].StartHead);
		cout<<szTemp<<endl;
		memset(szTemp,0,64);
		int temp = _ReadMbr.Partition[i].StartSecCyli;
		sprintf(szTemp,"分区起始扇区号:%d",temp & 63);//63转为二进制111111，做&运算，只取低6位
		cout<<szTemp<<endl;
		memset(szTemp,0,64);
		sprintf(szTemp,"分区起始磁柱号:%d",temp>>6);//取高10位
		cout<<szTemp<<endl;
		memset(szTemp,0,64);
		sprintf(szTemp,"分区文件类型标识:%02d",_ReadMbr.Partition[i].PartitionType);
		cout<<szTemp<<endl;
		memset(szTemp,0,64);
		sprintf(szTemp,"分区结束磁头号:%d",_ReadMbr.Partition[i].EndHead);
		cout<<szTemp<<endl;
		memset(szTemp,0,64);
		temp = _ReadMbr.Partition[i].EndSecCyli;
		sprintf(szTemp,"分区结束扇区号:%d",temp & 63);
		cout<<szTemp<<endl;
		memset(szTemp,0,64);
		sprintf(szTemp,"分区结束磁柱号:%d",temp>>6);
		cout<<szTemp<<endl;
		memset(szTemp,0,64);
		sprintf(szTemp,"分区起始相对扇区号:%d",_ReadMbr.Partition[i].StartLBA);
		cout<<szTemp<<endl;
		memset(szTemp,0,64);
		sprintf(szTemp,"分区总的扇区数:%d",_ReadMbr.Partition[i].TotalSector);
		cout<<szTemp<<endl;
	}
	DeviceIoControl(hDevice, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &count, NULL);
	CloseHandle(hDevice);
	system("pause");
	return 0;
}

