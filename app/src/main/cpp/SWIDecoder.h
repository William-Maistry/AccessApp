#pragma once
#include <stddef.h>
#include <vector>

//
struct T_size_178;
struct T_size_24;
struct WiImageBlock;


/*
** unsigned char I/O function pointer
*/
typedef int(*ByteIOFunc)(void*, int*);

/*
** image row I/O function pointer
*/
typedef int(*ScanlineIOFunc)(void*, WiImageBlock*, int, unsigned char**);

/*
** image extension I/O function pointer
*/
typedef int(*ExtensionIOFunc)(void*, int, unsigned char**, int*);

struct WiImageBlock {/*size = 0x28*/
	int Page;//Page
	int PageLeft;//PageLeft
	int PageTop;//PageTop
	int PageWidth;//PageWidth
	int PageHeight;//PageHeight
	int Block;//Block
	int BlockLeft;//BlockLeft
	int BlockTop;//BlockTop
	int BlockWidth;//BlockWidth
	int BlockHeight;//BlockHeight
};

struct WiDecmpOptions {/*size = 0x50*/
	int Smoothing;//Smoothing
	int Fast;//Fast
	ByteIOFunc ReadNextByte; //ReadNextByte
	void* ReadParam;//ReadParam
	int Sharpening;//Sharpening
	ScanlineIOFunc WriteScanline;//WriteScanline
	void* WriteScanlineParam;//WriteScanlineParam
	WiImageBlock SubImage;//SubImage
	int Magnification;//Magnification
	ExtensionIOFunc WriteAppExtension;//WriteAppExtension
	void* WriteAppExtensionParam;//WriteAppExtensionParam
};

struct WiRawImage {/*size = 30*/
	unsigned char* Raw;//Raw
	int Height;//Height
	int Width;//Width
	int BitsPerPixel;//BitsPerPixel
	int Color;//Color
	int LevelHeight;//LevelHeight
	int LevelWidth;//LevelWidth
	void* AppData;//AppData T_size_178*
	unsigned char    *Comment; //dw_20
	int              CommentLength;//dw_24
	unsigned char* AppExtension;//AppExtension
	int AppExtensionLength;//AppExtensionLength
};
/*
** Compressed Image
*/
typedef struct
{
	unsigned char    *CmpData;
	int              Size;
}WiCmpImage;
struct T_size_31C;
struct T_size_178;
struct T_size_14 {
	int dw0x00;
	unsigned short* hg0x04;
	int dw0x08;//int
	unsigned short* hg0x0C;
	int* hg0x10;
};
struct T_size_2C {
	int dw0x00;
	int dw0x04;
	T_size_14 t_08;
	signed char** hg_1C;
	signed char** hg_20;
	int(*hg0x24)(T_size_24*, T_size_178*, int, int);
	int(*hg0x28)(T_size_24*, T_size_178*, int, int);
};
struct T_size_Above_3C {
	int dw_0;
	T_size_14 tvar_4;
	signed char** hg_18;
	signed char** hg_1C;
	int dw_20;
	int dw_24;
	int dw_28;
	int dw_2C;
	int dw_30;
	int dw_34;
	int dw_38;
};
struct T_size_Above_C {
	int dw_00;//size
	T_size_Above_C** ppt_04;
	size_t ppt_04Len = 0;
	unsigned char * pb_08;
	size_t pb_08Len = 0;
	T_size_Above_C* pt_0C;
};
struct T_size_178
{
	int ** hg0x00;
	int *** hg0x04;
	unsigned char** hg0x08;
	unsigned char*** hg0x0C;
	int dw0x10;
	int dw0x14;
	int dw0x18;
	int dw0x1C;
	int dw0x20;
	int dw0x24;
	int dw0x28;
	int dw0x2C;
	int dw0x30;
	int dw0x34;
	T_size_14 t_38;//-4c
	T_size_31C* pt_4C;//T_size_31C*?
	unsigned long dw_50;
	int dw0x54;
	int dw0x58;
	int dw0x5C;
	int dw0x60;
	int dw0x64;
	int dw0x68;
	T_size_2C t_6C;//-98
	ScanlineIOFunc pf0x98;//WriteScanline
	void * pv_9C;//WriteScanlineParam
	ExtensionIOFunc pf_A0;//WriteAppExtension
	void * pv_A4;//WriteAppExtensionParam
	T_size_24 * pt0xA8;
	unsigned long res0;
	int*** hg0xB0;
	unsigned char* hg_B4;
	unsigned char*** hg_B8;
	unsigned char*** hg0xBC;
	unsigned long dw0xC0;
	int dw_C4;
	int dw_C8;
	int dw0xCC;//BlockHeight PageHeight
	int dw0xD0;//BlockWidth PageWidth
	int dw0xD4;
	unsigned long res[5];
	int dw0xEC;
	int dw0xF0;
	int dw0xF4;
	unsigned long res1;
	int dw_FC;
	int dw0x100 = 0; // =dw0xFC
	int dwdw_104 = 0;
	int dw0x108 = 0;
	int dw0x10C = 0;
	int dw0x110 = 0;
	int dw0x114 = 0;
	int dw0x118 = 0;
	int dw0x11C = 0;
	int dw0x120 = 0;
	int dw0x124 = 0;
	int dw0x128 = 0;
	int dw0x12C = 0;//Width
	int dw_130 = 0;//Magnification
	int dw0x134 = 0;
	unsigned long res2 = 0;
	int dw0x13C = 0;//Fast
	unsigned long res3 = 0;
	int dw_144 = 0;
	int dw0x148 = 0;
	unsigned char aby_14C[4];//unsigned char
	int dw0x150 = 0;//int
	int dw0x154 = 0;//int
	int dw0x158 = 0;//WiImageBlock.Page
	int dw0x15C = 0;//WiImageBlock.Block
	T_size_178 * pt_160;
	T_size_178 * pt0x164;
	unsigned char* hgE_168;
	int dwLEN_16C = 0;
	unsigned char* AE_170;//AppExtension
	int AELen_174 = 0;//AppExtentionLength
};
struct T_size_24 {
	unsigned char* pb_00;//CmpData
	unsigned char* pb_04;
	unsigned char  by0x08;
	unsigned char res[3];
	int dw0x0C;
	int dw0x10;//Size
	int dw0x14;//% 16
	int dw0x18;
	ByteIOFunc pf0x1C;
	int dw0x20;
};

struct T_size_31C {
	T_size_Above_C** hg_000;
	unsigned short aw_004[0x102];//unknown
	unsigned char ab_208[0x40];//uk memset 0x40
	int dw_308;
	int dw_30C;
	unsigned short w_310;
	unsigned short w_312;
	unsigned short w_314;
};

//extern "C" __declspec(dllexport) int WiDecompress(WiDecmpOptions * arg_0, WiRawImage * arg_4, WiCmpImage* arg_8);
//extern "C" __declspec(dllexport) WiDecmpOptions* WiCreateDecmpOptions();
//extern "C" __declspec(dllexport) WiCmpImage* WiCreateCmpImage();
//extern "C" __declspec(dllexport) WiRawImage* WiCreateRawImage();
//extern "C" __declspec(dllexport) int WiDestroyCmpImage(WiCmpImage* a1);
//extern "C" __declspec(dllexport) void WiFreeRawImageData(WiRawImage* a1);
//extern "C" __declspec(dllexport) int WiDestroyDecmpOptions(WiDecmpOptions* a1);

int sub_10011840(int arg_0, int arg_4);

int sub_10012090(int arg_0);

void* AllocateMemorySize(size_t dwBYTEs);

void* ReallocateMemoryAndCopyContents(void* hMem, size_t dwBYTEs);

void sub_10001430(unsigned char * arg_0, int arg_4, unsigned char * arg_8, int arg_C);

void* sub_10001470(int arg_0, int arg_4, int arg_8);

void sub_100014E0(unsigned char** arg_0, int arg_4, int arg_8, unsigned char * arg_C, int arg_10);

void DeallocateMemory(void* hMem);

void sub_10001550(int a);

T_size_24 * sub_1000A730(size_t dwBYTEs);

void sub_1000A8B0(T_size_24 * arg_0, ByteIOFunc arg_4, void * arg_8);

void sub_1000A900(T_size_24 * arg_0);

void sub_1000A970(T_size_24 * arg_0, int arg_4);

unsigned char sub_1000AB10(T_size_24 * arg_0, unsigned long arg_4);

unsigned char sub_1000ABC0(T_size_24 * arg_0);

void sub_1000D930(T_size_178 * arg_0);

void sub_1000DC30(T_size_178 * arg_0);

int sub_1000DCD0(T_size_178 * arg_0);

int sub_1000DDF0(T_size_178 * arg_0);

void sub_1000DA50(T_size_178 * arg_0, unsigned char** arg_4, int * arg_8);

int sub_1000DDA0(T_size_24 * arg_0);

void sub_1000DAF0(T_size_178 * arg_0);

int *** sub_1000E100(int arg_0, int arg_4, int arg_8);

unsigned char*** sub_1000E260(int arg_0, int arg_4, int arg_8);

unsigned char*** sub_1000E340(int arg_0, int arg_4, int arg_8);

void sub_1000E390(unsigned char *** arg_0, unsigned char* arg_4, int arg_8, int arg_C, int arg_10, int arg_14);

unsigned char*** sub_1000E420(unsigned char *** arg_0, int arg_4, int arg_8, int arg_C, int arg_10, int arg_14, int arg_18);

void* sub_1000E540(unsigned char*** hMem, int arg_4, int arg_8, int arg_C);

void sub_1000E580(int*** hMem, int arg_4);

int** sub_1000E5C0(int arg_0, int arg_4);

void sub_1000E620(unsigned char*** hMem, int arg_4);

void sub_1000E660(unsigned char*** hMem, int arg_4);

void sub_1000E600(int** arg_0);

void sub_1000E690(T_size_178 * arg_0);

void sub_1000E700(T_size_178 * arg_0);

void sub_1000E780(T_size_178 * arg_0);

void sub_1000EC40(T_size_178 * arg_0);

unsigned char sub_1000ED20(T_size_178 * arg_0);

unsigned char sub_1000ED60(T_size_178 * arg_0);

unsigned char sub_1000F050(T_size_178 * arg_0);

void sub_1000F8D0(T_size_178 * arg_0);

void sub_1000F960(T_size_178 * arg_0);

void sub_1000FDB0(T_size_178 * arg_0);

void sub_10011870(int arg_0, int arg_4, int arg_8, int arg_C, int * arg_10, int * arg_14, int * arg_18, int * arg_1C);

void sub_10011910(signed char*** arg_0, signed char*** arg_4, int arg_8, int arg_C, int arg_10);

int sub_10017970(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10017AE0(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10017C80(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10017DF0(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10018030(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_100181B0(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10018360(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_100184E0(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10018760(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10018A70(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_100188D0(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10018BE0(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10018E60(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10018FE0(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10019180(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10019300(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

void sub_100143E0(int***  arg_0, int arg_4, int arg_8, int arg_C);

void sub_100144F0(int*** arg_0, unsigned char** arg_4, int arg_8, int arg_C, int arg_10, int arg_14);

void sub_100146F0(int*** hgMem, unsigned char** hgArg_4, int arg_8, int arg_C, int arg_10, int arg_14, int arg_18, unsigned short * pwArg_1C);

unsigned char** sub_10014680(int*** hgMem, unsigned char** hgArg_4, int arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C);

unsigned char** sub_10014A40(int*** hgMem, unsigned char** hgArg_4, int arg_8, int arg_C, int arg_10, int arg_14, unsigned short * pwArg_18);

void sub_10014D50(unsigned short* hgMem, int arg_4, int arg_8, int arg_C);

unsigned char sub_10014DE0(T_size_178 * arg_0);

int sub_1000DFA0(T_size_178 * arg_0, int arg_4);

void sub_10010390(T_size_178 * arg_0);

void sub_10011A10(signed char** arg_0, signed char** arg_4);

void sub_10015150(T_size_24 * arg_0, unsigned char ** arg_4, int arg_8, int arg_C, int arg_10, int arg_14);

void sub_100151D0(T_size_24 * arg_0, int*** arg_4, unsigned char*** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C, int arg_30, int arg_34, int arg_38, int arg_3C);

void sub_10015350(T_size_178 * arg_0);

int sub_10015390(T_size_178 * arg_0);

unsigned char sub_10015560(T_size_178 * arg_0);

void sub_10015A80(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24);

void sub_10015940(T_size_178 * arg_0);

void sub_10015980(T_size_2C * arg_0, int arg_4, int arg_8, int arg_C, int(*arg_10)(T_size_24*, T_size_178*, int, int), int(*arg_14)(T_size_24*, T_size_178*, int, int), int arg_18);

void sub_100159D0(T_size_2C * arg_0);

void sub_10015A00(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C* arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C);

void sub_10015FF0(T_size_2C * arg_0, int arg_4, int arg_8, int arg_C, int(*arg_10)(T_size_24*, T_size_178*, int, int), int(*arg_14)(T_size_24*, T_size_178*, int, int), int arg_18);

void sub_10016050(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C);

void sub_10016030(T_size_2C * arg_0);

void sub_100160D0(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24);

void sub_10016300(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C* arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C);

void sub_10016560(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28);

void sub_10016B70(T_size_2C * arg_0, int arg_4, int arg_8, int arg_C, int(*arg_10)(T_size_24*, T_size_178*, int, int), int(*arg_14)(T_size_24*, T_size_178*, int, int), int arg_18);

void sub_10016BC0(T_size_2C * arg_0);

void sub_10016C10(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C);

void sub_10016EA0(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24);

void sub_10016FF0(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24);

unsigned char sub_10017870(unsigned char ** arg_0, int arg_4, int arg_8, int arg_C, int arg_10);

void sub_100178D0(T_size_24 * arg_0, T_size_178 * arg_4, int *** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C);

void sub_10017FA0(T_size_24 * arg_0, T_size_178 * arg_4, int *** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C);

void sub_10018E40(T_size_24 * arg_0, T_size_178 * arg_4);

void sub_100186A0(T_size_24 * arg_0, T_size_178 * arg_4, int *** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C);

void sub_10018D90(T_size_24 * arg_0, T_size_178 * arg_4, int *** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C);

void sub_100194C0(int** arg_0, int arg_4, int arg_8, int arg_C);

void sub_10019760(int** arg_0, unsigned char** arg_4, int arg_8, int arg_C);

void sub_10019B00(int** arg_0, unsigned char** arg_4, int arg_8, int arg_C);

void sub_10019E80(int** arg_0, int** arg_4, int arg_8, int arg_C, int arg_10);

void sub_100201D0(T_size_24 * arg_0, unsigned char ** arg_4, int arg_8, int arg_C, int arg_10);

void sub_100202D0(T_size_24 * arg_0, T_size_Above_3C* arg_4, unsigned char ** arg_8);

void sub_10020440(T_size_24 * arg_0, unsigned char ** arg_4, int arg_8, int arg_C, int arg_10);

void sub_10020720(T_size_24 * arg_0, unsigned char ** arg_4, T_size_Above_3C* arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C);

void sub_10021480(T_size_24 * arg_0, unsigned char ** arg_4, int arg_8, int arg_C, int arg_10);

void sub_100215A0(T_size_24 * arg_0, unsigned char ** arg_4, T_size_Above_3C* arg_8, int arg_C, int arg_10, int arg_14, int arg_18);

void sub_10021F60(T_size_24 * arg_0, int*** arg_4, unsigned char*** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C, int arg_30, int arg_34, int arg_38);

int sub_10021420(unsigned char ** arg_0, int arg_4, int arg_8, int arg_C, int arg_10);

void sub_10022190(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20);

void sub_100223F0(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28);

void sub_100226D0(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28);

void sub_10022940(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C, int arg_30);

void sub_10022C50(T_size_24 * arg_0, int *** arg_4, unsigned char*** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C, int arg_30, int arg_34, int arg_38);

void sub_10022E80(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20);

void sub_10023100(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28);

void sub_100233F0(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28);

void sub_10023680(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C, int arg_30);

void sub_10023990(T_size_24 * arg_0, int *** arg_4, unsigned char*** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C, int arg_30, int arg_34, int arg_38);

void sub_10023BC0(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20);

void sub_10023E90(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28);

void sub_10024200(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28);

void sub_100244F0(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C, int arg_30);

void sub_10024890(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int * arg_1C, int arg_20, int arg_24);

void sub_10024E50(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int * arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C);

void sub_10025560(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C);

void sub_10025B80(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C, int arg_30, int arg_34);

void sub_10026310(T_size_24 * arg_0, int *** arg_4, unsigned char*** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C, int arg_30, int arg_34, int arg_38);

void sub_100265F0(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, int arg_C, int arg_10, int arg_14, int arg_18);

void sub_10026580(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, int arg_C, int arg_10, int arg_14, int arg_18);

void sub_10026670(T_size_2C * arg_0, int arg_4, int arg_8, int arg_C, int(*arg_10)(T_size_24*, T_size_178*, int, int), int(*arg_14)(T_size_24*, T_size_178*, int, int), int arg_18);

void sub_100266A0(T_size_2C * arg_0);

void sub_100266C0(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, int arg_C, int arg_10, int arg_14, int arg_18);

void sub_10026750(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C);

void sub_10026CB0(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, int arg_C, int arg_10, int arg_14, int arg_18);

void sub_10026D40(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, int arg_C, int arg_10, int arg_14, int arg_18);

void sub_10026C90(T_size_2C * arg_0, int arg_4, int arg_8, int arg_C, int(*arg_10)(T_size_24*, T_size_178*, int, int), int(*arg_14)(T_size_24*, T_size_178*, int, int), int arg_18);

void sub_10027230(T_size_24 * arg_0, T_size_178 * arg_4, int *** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C);

void sub_100272D0(T_size_178 * arg_0);

int sub_100272F0(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10027600(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10027460(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10027770(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10027930(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10027C30(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10027AA0(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10027DA0(T_size_24 * arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

int sub_10027F50(T_size_24 * arg_0);

int sub_10027FA0(T_size_14 * arg_0, int arg_4);

int sub_100280B0(T_size_14 * arg_0, int arg_4, int arg_8);

void sub_10028140(T_size_14 * arg_0);

int sub_100282C0(T_size_14 * arg_0, T_size_24 * arg_4, int arg_8);

int sub_10028360(T_size_14 * arg_0, T_size_24 * arg_4);

void sub_100283A0(T_size_14 * arg_0, int arg_4);

void sub_10028410(T_size_14* arg_0);

void sub_100285F0(T_size_14* arg_0, int arg_4, int arg_8);

void sub_10028700(T_size_14* arg_0, int arg_4);

void sub_100287E0(unsigned char** arg_0, int arg_4, int arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20);

void sub_1002C8F0(unsigned char ** arg_0, int arg_4, int arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20);

void sub_10030730(int** arg_0, int arg_4, int arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20);

T_size_31C* sub_10031B10();

void sub_10031C60(T_size_24 * arg_0, T_size_31C* arg_4);

void sub_10031CA0(T_size_31C* arg_0);

void sub_10031D20(T_size_Above_C* arg_0);

void sub_10031D80(int arg_0, T_size_24 * arg_4, T_size_31C* arg_8);

int sub_10031DB0(T_size_24 * arg_0, T_size_31C * arg_4);

int sub_10031E40(T_size_24 * arg_0, T_size_31C * arg_4);

T_size_Above_C* sub_10031FA0(T_size_Above_C* arg_0, int arg_4, T_size_Above_C* arg_8);

void sub_10032040(int arg_0, T_size_31C * arg_4);

void sub_100320B0(T_size_Above_C* arg_0, int arg_4, T_size_31C * arg_8);

void sub_10032270(unsigned short * arg_0, T_size_31C * arg_4);

void sub_10032330(int arg_0, T_size_31C * arg_4);

T_size_Above_C* sub_10032390(T_size_Above_C* arg_0, int arg_4, int arg_8);

int sub_10032770(unsigned short * arg_0, T_size_31C * arg_4);

int sub_100322A0(int arg_0, unsigned short * arg_4, T_size_31C * arg_8);

void sub_100323F0(T_size_Above_C* arg_0);

void sub_10032470(T_size_Above_C* arg_0, T_size_31C * arg_4);

void sub_100327B0(T_size_24 * arg_0, T_size_31C * arg_4);

void sub_10032800(T_size_24 * arg_0, unsigned short * arg_4, T_size_31C * arg_8);

void sub_100328F0(int*** arg_0, unsigned char** arg_4, int arg_8, int arg_C);


struct WiResultImage {
	int size;
	unsigned char* raw;
};
