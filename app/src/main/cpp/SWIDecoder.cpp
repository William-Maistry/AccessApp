#include "SWIDecoder.h"
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <jni.h>
#include <jni.h>

int factor64 = sizeof(void*) == 4 ? 1 : 2;
int sifeof8 = sizeof(int);
long sizeofprt = INTPTR_MAX;
int sizeof64 = INT64_MAX;
int sizeof32 = INT32_MAX;

int memAllocateCnt = 0;
int memDeallocateCnt = 0;


int int_1003A8E8;

//p
int sub_10011840(int arg_0, int arg_4) {
	int eax; // eax@1
	int v3; // eax@3

	eax = arg_4 - 2;
	if ((arg_0 - 2) < (arg_4 - 2))
		eax = arg_0 - 2;
	v3 = sub_10012090(eax);
	return (v3 - 2) < 0 ? 0 : v3 - 2;
}
int sub_10012090(int arg_0) {
	int v1 = arg_0;
	int result = 0;
	if (arg_0 > 0)
	{
		for (result = 0; v1 > 1; v1 /= 2, result++);
	}
	else
	{
		result = -1;
	}
	return result;
}
//OK
void* AllocateMemorySize(size_t dwBYTEs)
{
	void* result; // eax@2
	if (dwBYTEs)
	{
		result = malloc(dwBYTEs); // GlobalAlloc(0x40u, dwBYTEs);
		if (result)
		{
			//printf("Allocate %p\n", result);
			memAllocateCnt++;
			memset(result, 0, dwBYTEs);
			return result;
		}
		sub_10001550(8);
	}
	return 0;
}

//pp
void* ReallocateMemoryAndCopyContents(void* hMem, size_t dwBYTEs, size_t hMemLen) {
	//size_t v3;
	void* pebx;
	unsigned int iecx;
	if (dwBYTEs)
	{
		if (!hMem)
			return AllocateMemorySize(dwBYTEs);
		//v3 = GlobalSize(hMem);
		pebx = malloc(dwBYTEs); // GlobalAlloc(0, dwBYTEs);
		if (pebx)
		{
			iecx = hMemLen;
			if (hMemLen >= dwBYTEs)
				iecx = dwBYTEs;
			memcpy(pebx, hMem, iecx);
			//printf("Reallocate %p -> %p\n", hMem, pebx);
			free(hMem);	//		GlobalFree(hMem);
			return pebx;
		}
		sub_10001550(8);
	}
	return 0;
}

//k
void sub_10001430(unsigned char* arg_0, int arg_4, unsigned char* arg_8, int arg_C) {
	for (int i = arg_4; i > 0; i--) {
		memcpy(&arg_0[(arg_4 - i) * arg_C], arg_8, arg_C);
	}
}
//ok unsigned char** int * arg_8 = size(unsigned char) or size(int)
void* sub_10001470(int arg_0, int arg_4, int arg_8) {
	unsigned char** hedi = (unsigned char**)AllocateMemorySize(arg_0 * 4 * factor64);
	if (!hedi)return 0;
	hedi[0] = (unsigned char*)AllocateMemorySize(arg_0 * (arg_8 * factor64) * arg_4);
	if (!hedi[0]) {
		DeallocateMemory(hedi);
		return 0;
	}
	for (int i = arg_0 - 1; i > 0; i--) {//edx
		hedi[arg_0 - 1 - i + 1] = &hedi[0][arg_4 * arg_8 * (arg_0 - 1 - i + 1)];
	}
	return (unsigned char**)hedi;
}
//k
void sub_100014E0(unsigned char** arg_0, int arg_4, int arg_8, unsigned char * arg_C, int arg_10) {
	for (int i = arg_4; i > 0; i--) {
		sub_10001430(arg_0[arg_4 - i], arg_8, arg_C, arg_10);
	}
}

//p GlobalFree
void DeallocateMemory(void* hMem)
{
	//try
	//{
	memDeallocateCnt++;
	//printf("Deallocate %p\n", hMem);
	free(hMem);
	//}
	//catch (int ex)
	//{
	//	return;
	//}
}

//k
void sub_10001550(int a)
{
	int_1003A8E8 = a;
}
//p OK
T_size_24 * sub_1000A730(size_t dwBYTEs)
{
	T_size_24 * esi = (T_size_24 *)AllocateMemorySize(sizeof(T_size_24));
	//int edi = 0;
	if (esi == 0)
	{
		return 0;
	}
	if (dwBYTEs > 0)
	{
		unsigned char* eax = (unsigned char*)AllocateMemorySize(dwBYTEs);
		esi->pb_00 = eax;
		if (eax == 0) {
			DeallocateMemory(esi);
			return 0;
		}
	}
	else
		esi->pb_00 = 0;
	esi->dw0x14 = 0;
	esi->pb_04 = esi->pb_00;
	esi->dw0x0C = 0;
	esi->dw0x10 = 0;
	esi->dw0x18 = 0;
	esi->dw0x20 = 0;
	esi->by0x08 = 0x80;
	return esi;
}
//OK
void sub_1000A8B0(T_size_24 * arg_0, ByteIOFunc arg_4, void* arg_8)
{
	arg_0->pf0x1C = arg_4;
	if (arg_4) {
		arg_0->pb_00 = (unsigned char*)arg_8;
	}
}
//OK
void sub_1000A900(T_size_24 * arg_0) {
	arg_0->dw0x14 = 0;
	arg_0->pb_04 = arg_0->pb_00;
	arg_0->by0x08 = 0x80;
	arg_0->dw0x0C = 0;
	arg_0->dw0x10 = 0;
	arg_0->dw0x18 = 0;
}
//k
void sub_1000A970(T_size_24 * arg_0, int arg_4) {
	T_size_24 * pesi = arg_0;
	int i = arg_4;
	pesi->by0x08 = 0x80;
	if (pesi->pf0x1C) {
		for (; i > 0; i--) {//edi
			if (pesi->dw0x14)
				break;
			if (pesi->pf0x1C(pesi->pb_00, &pesi->dw0x0C))
			{
				pesi->dw0x14 = 3;
				break;
			}
		}

	}
	pesi->pb_04 += i;
}
//OK
unsigned char sub_1000AB10(T_size_24 * arg_0, unsigned long arg_4) {
	int edi = 1 << (arg_4 - 1);
	int ebp = 0;
	if (!edi)
		return 0;
	T_size_24 * esi = arg_0;
	unsigned char bl = 0x80;
	do {
		if (esi->by0x08 == bl)
		{
			ByteIOFunc pfeax = esi->pf0x1C;
			if (pfeax)
			{
				if (!esi->dw0x14)
				{
					if (!pfeax(esi->pb_00, &esi->dw0x0C))
					{
						//eax = esi->hg0x04;
						esi->pb_04++;
					}
					else
					{
						esi->dw0x14 = 3;
						return 0;
					}
				}
				//loc_1000AB75
			}
			//loc_1000AB5A
			else {
				if ((esi->pb_04 - esi->pb_00) >= esi->dw0x10)
				{
					esi->dw0x14 = 1;
					return 0;
				}//?
				esi->dw0x0C = esi->pb_04[0];
				esi->pb_04++;
			}
		}
		//loc_1000AB75
		if (esi->by0x08 & esi->dw0x0C)
		{
			ebp |= edi;
		}
		edi /= 2;
		esi->by0x08 /= 2;
		if (!esi->by0x08)
		{
			esi->by0x08 = bl;
		}

	} while (edi);
	return ebp;

}
//k
unsigned char sub_1000ABC0(T_size_24 * arg_0) {
	T_size_24 *pesi = arg_0;
	if (pesi->by0x08 == 0x80) {
		if (pesi->pf0x1C) {
			if (!pesi->dw0x14) {
				if (pesi->pf0x1C(pesi->pb_00, &pesi->dw0x0C)) {
					pesi->dw0x14 = 3;
					return 0;
				}
				else {
					pesi->pb_04++;
				}
			}
		}
		else {
			if ((pesi->pb_04 - pesi->pb_00) >= pesi->dw0x10) {
				pesi->dw0x14 = 1;
				return 0;
			}
			pesi->dw0x0C = *pesi->pb_04;
			pesi->pb_04++;
		}
	}
	//loc_1000AC1F
	unsigned char cl = pesi->by0x08;
	pesi->by0x08 = pesi->by0x08 / 2;
	if (!pesi->by0x08) {
		pesi->by0x08 = 0x80;
	}
	return (pesi->dw0x0C & (cl & 0xFF)) != 0;
}
//OK
void sub_1000D930(T_size_178 * arg_0) {
	T_size_178 *esi = arg_0;
	T_size_24 * edi = esi->pt0xA8;
	esi->dw0xC0 = sub_1000AB10(edi, 8);//3 good
	int eax;
	eax = (sub_1000AB10(edi, 8) << 8) + sub_1000AB10(edi, 8);
	esi->dw0xCC = eax;
	esi->dw0x128 = eax;
	eax = (sub_1000AB10(edi, 8) << 8) + sub_1000AB10(edi, 8);//good
	esi->dw0xD0 = eax;
	esi->dw0x12C = eax;
	sub_1000DAF0(esi);//b
	if (esi->dw0xC0 < 3) {
		esi->hgE_168 = 0;
		esi->dwLEN_16C = 0;
		esi->AE_170 = 0;
		esi->AELen_174 = 0;
		return;
	}
	sub_1000DC30(esi);//b,b good
	if (esi->dw0xC0 >= 4)
	{
		sub_1000AB10(edi, 8);
	}

	sub_1000DA50(esi, &esi->hgE_168, &esi->dwLEN_16C);//good
	if (esi->dw0xC0 >= 4) {
		sub_1000DA50(esi, &esi->AE_170, &esi->AELen_174);
		sub_1000A970(edi, 0);
		return;
	}
	esi->AE_170 = 0;
	esi->AELen_174 = 0;
}
//k
void sub_1000DC30(T_size_178 * arg_0) {
	T_size_178 *esi = arg_0;
	esi->dw0x148 = sub_1000AB10(esi->pt0xA8, 1);
	if (!esi->dw0x148)
	{
		int i;
		if (esi->dw0xF4 == 1)
		{
			for (i = 0; i < esi->dw0xF0 / 8; i += 1)//edi
			{
				esi->aby_14C[i] = sub_1000AB10(esi->pt0xA8, 8);
			}
		}
		else {
			for (i = 0; i < 3; i += 1)
			{
				esi->aby_14C[i] = sub_1000AB10(esi->pt0xA8, 8);
			}
		}
	}
}
//k
int sub_1000DCD0(T_size_178 * arg_0) {
	T_size_178 * pesi = arg_0;
	sub_1000A970(pesi->pt0xA8, 0);
	pesi->hgE_168 = 0;
	pesi->dwLEN_16C = 0;
	pesi->AE_170 = 0;
	pesi->AELen_174 = 0;
	unsigned char * pebp = pesi->pt0xA8->pb_04;
	pesi->dw0xEC = sub_1000AB10(pesi->pt0xA8, 3);
	if (!pesi->dw0xEC) {
		sub_1000A970(pesi->pt0xA8, pesi->dw0xEC);
		return pesi->pt0xA8->pb_04 - pebp;
	}
	sub_1000DDF0(pesi);
	sub_1000DC30(pesi);
	if (pesi->dw0xC0 >= 4) {
		sub_1000DA50(pesi, &pesi->AE_170, &pesi->AELen_174);
	}
	else {
		pesi->AE_170 = 0;
		pesi->AELen_174 = 0;
	}
	sub_1000A970(pesi->pt0xA8, 0);
	return pesi->pt0xA8->pb_04 - pebp;
}
//k
int sub_1000DDF0(T_size_178* arg_0) {
	T_size_178 * pesi = arg_0;
	sub_1000A970(pesi->pt0xA8, 0);
	unsigned char * pebp = pesi->pt0xA8->pb_04;
	pesi->dw_144 = sub_1000AB10(pesi->pt0xA8, 1);
	int iedi = sub_10012090(pesi->pt_160->dw0x12C);
	iedi++;
	int iebx = sub_10012090(pesi->pt_160->dw0x128);
	iebx++;
	pesi->dw_C8 = sub_1000AB10(pesi->pt0xA8, iebx);
	pesi->dw_C4 = sub_1000AB10(pesi->pt0xA8, iedi);
	pesi->dw0xCC = sub_1000AB10(pesi->pt0xA8, iebx);
	pesi->dw0x128 = pesi->dw0xCC;
	pesi->dw0xD0 = sub_1000AB10(pesi->pt0xA8, iedi);
	pesi->dw0x12C = pesi->dw0xD0;
	if (pesi->dw_144) {
		pesi->pf0x98 = pesi->pt_160->pf0x98;
		pesi->pv_9C = pesi->pt_160->pv_9C;
		pesi->dw0xF0 = pesi->pt_160->dw0xF0;
		pesi->dw0xF4 = pesi->pt_160->dw0xF4;
		pesi->dwdw_104 = pesi->pt_160->dwdw_104;
		pesi->dw0x108 = pesi->pt_160->dw0x108;
		pesi->dw0x10C = pesi->pt_160->dw0x10C;
		pesi->dw0x134 = pesi->pt_160->dw0x134;
		pesi->dw0x114 = pesi->pt_160->dw0x114;
		pesi->dw0x118 = pesi->pt_160->dw0x118;
		pesi->dw0x110 = pesi->pt_160->dw0x110;
		pesi->dw0x13C = pesi->pt_160->dw0x13C;
		pesi->dw0x120 = pesi->pt_160->dw0x120;
		pesi->pt0x164 = 0;
	}
	else
		sub_1000DAF0(pesi);
	sub_1000A970(pesi->pt0xA8, 0);
	return pesi->pt0xA8->pb_04 - pebp;
}
//k
void sub_1000DA50(T_size_178 * arg_0, unsigned char** arg_4, int * arg_8) {
	T_size_178 * ebx = arg_0;
	if (sub_1000AB10(ebx->pt0xA8, 1) && ebx->dw0xC0 >= 4)
	{
		arg_8[0] = sub_1000AB10(ebx->pt0xA8, 0x10);
		arg_4[0] = (unsigned char*)AllocateMemorySize(arg_8[0]);
		for (int i = 0; i < arg_8[0]; i++) {//esi
			arg_4[0][i] = sub_1000AB10(ebx->pt0xA8, 8);
		}
	}
	else {
		arg_4[0] = 0;
		arg_8[0] = 0;
	}
}
//k
int sub_1000DDA0(T_size_24 * arg_0) {
	int v1;
	sub_1000A970(arg_0, 0);
	if (sub_1000AB10(arg_0, 1))
		v1 = sub_1000AB10(arg_0, 0x17);
	else
		v1 = sub_1000AB10(arg_0, 0xF);
	sub_1000A970(arg_0, 0);
	return v1;
}
//k
void sub_1000DAF0(T_size_178 * arg_0) {
	T_size_178 * esi = arg_0;
	T_size_24 * edi = esi->pt0xA8;
	esi->dw0xF0 = sub_1000AB10(edi, 5);//8 4
	esi->dw0xF4 = (sub_1000AB10(edi, 1) != 0 ? 3 : 1); // ?neg sbb 
	if (sub_1000AB10(edi, 1))
	{
		esi->dw0x10C = sub_1000AB10(edi, 8);//9 good
	}
	else
		esi->dw0x10C = sub_1000AB10(edi, 0x10);
	esi->dw0x134 = sub_1000AB10(edi, 2);
	esi->dw0x108 = sub_1000AB10(edi, 2);
	esi->dwdw_104 = sub_1000AB10(edi, 2);
	for (esi->dw0x114 = 0; sub_1000AB10(edi, 1); esi->dw0x114++);
	for (esi->dw0x110 = 0; sub_1000AB10(edi, 1); esi->dw0x110++);
	for (esi->dw0x118 = 0; sub_1000AB10(edi, 1); esi->dw0x118++);
	esi->dw0xD4 = sub_1000AB10(edi, 1);
}
//Ok
int *** sub_1000E100(int arg_0, int arg_4, int arg_8) {
	int *** hMem = (int ***)AllocateMemorySize(sizeof(int) * arg_8 * factor64);
	int i = 0;
	for (i = 0; i < arg_8; i++) {//edi,esi
		hMem[i] = (int**)sub_1000E5C0(arg_0, arg_4);
		if (!hMem[i]) {
			for (int j = i; j > 0; j--) {//edi, esi
				sub_1000E600(hMem[i - j]);
			}
			DeallocateMemory(hMem);
			return 0;
		}
	}
	return hMem;
}

//K 
unsigned char*** sub_1000E260(int arg_0, int arg_4, int arg_8) {
	unsigned char*** hebp = (unsigned char***)AllocateMemorySize(arg_8 * 4 * factor64);
	hebp[0] = (unsigned char**)sub_10001470(arg_0, arg_4 * arg_8, 1);
	if (!hebp[0])
	{
		DeallocateMemory(hebp);
		return 0;
	}
	for (int i = 1; i < arg_8; i++) {
		hebp[i] = (unsigned char**)AllocateMemorySize(arg_0 * 4 * factor64);
		if (hebp[i]) {
			for (int j = 0; j < arg_0; j++) {//eax
				hebp[i][j] = &hebp[0][j][i];
			}
		}
		else {
			for (int j = i - 1; j > 0; j--) {//esi
				DeallocateMemory(hebp[j - i]);
			}
			DeallocateMemory(hebp[0]);
			DeallocateMemory(hebp);
			return 0;
		}
	}
	return hebp;
}
//k
unsigned char*** sub_1000E340(int arg_0, int arg_4, int arg_8) {
	unsigned char*** ebp = (unsigned char***)AllocateMemorySize(arg_8 * 4 * factor64);
	for (int i = arg_8; i > 0; i--) {//edi, esi
		ebp[arg_8 - i] = (unsigned char**)AllocateMemorySize(arg_0 * 4 * factor64);
	}
	return ebp;
}
//k width, top 
void sub_1000E390(unsigned char *** arg_0, unsigned char* arg_4, int arg_8, int arg_C, int arg_10, int arg_14) {
	if (arg_14 == 1 && arg_10 > 8) {
		for (int i = arg_14; i > 0; i--) {//iarg_0,arg0+=4
			for (int j = 0; j < arg_8; j++) {//ieax
				arg_0[arg_14 - i][j] = &arg_4[arg_C * 2 * j];
			}
		}
	}
	else {
		for (int i = 0; i < arg_14; i++) {//iebx
			for (int j = 0; j < arg_8; j++) {//ieax
				arg_0[i][j] = &arg_4[i + arg_C * arg_14 * j];
			}
		}
	}
}
//k
unsigned char*** sub_1000E420(unsigned char *** arg_0, int arg_4, int arg_8, int arg_C, int arg_10, int arg_14, int arg_18) {
	unsigned char *** var_4 = sub_1000E340(arg_C, arg_10, arg_18);
	if (!var_4) return 0;
	if (arg_18 == 1 && arg_14 > 8) {
		for (int i = 0; i < arg_C; i++) {//arg_10 ecx arg_4 += 4
			for (int j = arg_18; j > 0; j--) {//esi
				var_4[1 - j][i] = &arg_0[1 - j][arg_4 + i][arg_8 * 2];
			}
		}
	}
	else {
		for (int i = 0; i < arg_C; i++) {//arg_10, ecx arg_4 += 4
			for (int j = arg_18; j > 0; j--) {//edi
				var_4[1 - j][i] = &arg_0[1 - j][arg_4 + i][arg_8 * arg_18];
			}
		}
	}
	return var_4;

}
//k	free []
void* sub_1000E540(unsigned char*** hMem, int arg_4, int arg_8, int arg_C) {
	int edi = arg_C;
	unsigned char* ebp = hMem[0][0];
	if (arg_C > 0)
	{
		for (int i = arg_C; i > 0; i--) {//edi
			DeallocateMemory(hMem[arg_C - i]);
		}
	}
	DeallocateMemory(hMem);
	return ebp;
}

//k	free hMem[arg_4] free at hMem[0](void**) - 4 -4
void sub_1000E580(int*** hMem, int arg_4) {
	if (hMem)
	{
		for (int i = arg_4; i > 0; i--) {//edi
			sub_1000E600(hMem[arg_4 - i]);
		}
		DeallocateMemory(hMem);
	}
}
//ok
int** sub_1000E5C0(int arg_0, int arg_4) {
	int ** hMem = (int**)sub_10001470(arg_0 + 1, arg_4 + 1, 4);
	if (!hMem) return 0;
	int **hecx = hMem;
	for (int i = arg_0 + 1; i > 0; i--)//esi
	{
		hMem[arg_0 + 1 - i]++;
	}
	return &hMem[1];
}
//k	free void * hMem[arg_4] free **hMem[0]
void sub_1000E620(unsigned char*** hMem, int arg_4) {
	DeallocateMemory(hMem[0][0]);
	for (int i = arg_4; i > 0; i--) {//edi
		DeallocateMemory(hMem[arg_4 - i]);
	}
	DeallocateMemory(hMem);
}
//k	free void * hMem[arg_4];
void sub_1000E660(unsigned char*** hMem, int arg_4) {
	for (int i = arg_4; i > 0; i--) {//edi
		DeallocateMemory(hMem[arg_4 - i]);
	}
	DeallocateMemory(hMem);
}
//k free void ** free at -4 -4
void sub_1000E600(int** arg_0) {
	if (arg_0)
	{
		int** v = arg_0;
		arg_0[-1]--;
		arg_0[-1][0]--;
		DeallocateMemory(&arg_0[-1][0]);
		DeallocateMemory(&arg_0[-1]);
	}
}
//Ok
void sub_1000E690(T_size_178 * arg_0) {
	T_size_178 *esi = arg_0;
	esi->dw0x154 = 0;
	esi->dw0x150 = 0;
	int eax = sub_1000AB10(esi->pt0xA8, 8);
	if ((eax == 0x53 || eax == 0x57) && sub_1000AB10(esi->pt0xA8, 8) == 0x49) {
		sub_1000D930(esi);//good
						  //get b0 b8
		sub_1000E700(esi);
		return;
	}
	sub_10001550(1);
	esi->dw0x154 = 1;
}

//OK
void sub_1000E700(T_size_178 * arg_0) {
	T_size_178 *esi = arg_0;
	if (esi->dw0xC0 == 2 || (esi->dw0xC0 == 4 && esi->dw0xD4 == 0))
	{
		sub_1000E780(esi);
		esi->dw0x150 = 3;
		return;
	}
	if (esi->dw0xC0 == 3 || (esi->dw0xC0 == 4 && esi->dw0xD4 == 1))
	{
		sub_1000F960(esi);
		esi->dw0x150 = 3;
		return;
	}
	sub_10001550(2);
	esi->dw0x154 = 2;
}
//k1
void sub_1000E780(T_size_178 * arg_0) {
	T_size_178 *pesi = arg_0;
	pesi->dw0x154 = 0;
	pesi->hg_B8 = 0;
	if (pesi->dw0xC0 != 2 && (pesi->dw0xC0 != 4 || pesi->dw0xD4))
	{
		sub_10001550(2);
		pesi->dw0x154 = 2;
		return;
	}
	sub_1000EC40(pesi);
	if (pesi->dw0x154) return;
	pesi->dw_FC = sub_10011840(pesi->dw0xCC, pesi->dw0xD0);
	if (pesi->dw_130 >= 0) {
		pesi->dw_130 = 0;
	}
	else {
		if ((-pesi->dw_130) > pesi->dw_FC)
		{
			pesi->dw_130 = -pesi->dw_FC;
		}
		pesi->dw_FC += pesi->dw_130;
		pesi->dw0x13C = 0;
		int edx = 1 << (unsigned char)(-pesi->dw_130 + 1);
		pesi->dw0xCC = (pesi->dw0xCC + edx - 2) >> (unsigned char)(-pesi->dw_130);
		pesi->dw0xD0 = (pesi->dw0xD0 + edx - 2) >> (unsigned char)(-pesi->dw_130);
	}
	if (pesi->dw0x13C && pesi->dw_FC > 0)
	{
		pesi->hg0xB0 = sub_1000E100(pesi->dw0xCC / 2 + 1, pesi->dw0xD0 / 2 + 1/*cdq*/, pesi->dw0xF4);
	}
	else {
		//alloc pesi->hg0xB0
		pesi->hg0xB0 = sub_1000E100(pesi->dw0xCC, pesi->dw0xD0, pesi->dw0xF4);
	}

	if (!pesi->hg0xB0) {
		pesi->dw0x154 = 8;
		return;
	}
	if (pesi->hg_B4) {
		int flag = 2;//ecx
		if (pesi->dw0xF4 != 1 || pesi->dw0xF0 <= 8)
			flag = 1;
		pesi->hg_B8 = sub_1000E340(pesi->dw0xCC, pesi->dw0xD0 * flag, pesi->dw0xF4);
		if (!pesi->hg_B8)
		{
			pesi->dw0x154 = 8;
			sub_1000E580(pesi->hg0xB0, pesi->dw0xF4);
			return;
		}
		sub_1000E390(pesi->hg_B8, pesi->hg_B4, pesi->dw0xCC, pesi->dw0xD0, pesi->dw0xF0, pesi->dw0xF4);
	}
	else {
		int flag = 2;//eax
		if (pesi->dw0xF4 != 1 || pesi->dw0xF0 <= 8)
			flag = 1;

		//pesi->hg_B8 = CreateItem(pesi->dw0xCC, pesi->dw0xD0 * flag, pesi->dw0xF4);
		pesi->hg_B8 = sub_1000E260(pesi->dw0xCC, pesi->dw0xD0 * flag, pesi->dw0xF4);
		if (!pesi->hg_B8)
		{
			pesi->dw0x154 = 8;
			sub_1000E580(pesi->hg0xB0, pesi->dw0xF4);
			return;
		}
	}
	//loc_1000E95F
	if (pesi->dw0x134 == 1)
	{
		sub_1000ED20(pesi);
		if (pesi->dw0x154 == 0xC)
		{
			pesi->dw0x154 = 0;
		}

	}
	else if (pesi->dw0x134 == 2) {
		sub_10015350(pesi);
		if (pesi->dw0x154 == 0xC)
		{
			pesi->dw0x154 = 0;
		}
	}
	else {
		pesi->dw0x150 = 5;
		//get pesi->hg0xB0
		sub_10014DE0(pesi);
		if (pesi->dw0x154 == 0xB)
		{
			pesi->dw0x154 = 0;
		}
	}
	pesi->dw0x150 = 3;
	//loc_1000E9B1
	if (((unsigned char)pesi->pt0xA8->dw0x14) & 1 && !pesi->dw0x154)
	{
		pesi->dw0x154 = 10;
	}
	if (pesi->dw0x154 && pesi->dw0x154 != 0xA)
	{
		if (pesi->hg_B4)
		{
			sub_1000E660(pesi->hg_B8, pesi->dw0xF4);
			pesi->hg_B8 = 0;
		}
		else {
			sub_1000E620(pesi->hg_B8, pesi->dw0xF4);
			pesi->hg_B8 = 0;
		}
	}
	else {
		if (!pesi->dw0x13C)
		{
			if (pesi->dw_130 < 0)
			{
				unsigned short* edi = (unsigned short*)AllocateMemorySize((1 << (unsigned char)pesi->dw0xF0) * 2);
				sub_10014D50(edi, pesi->dw0xF0, (1 << (unsigned char)(pesi->dw0xF0 - 1)), pesi->dw0x110);
				sub_100146F0(pesi->hg0xB0, pesi->hg_B8[0], pesi->dw0x128, pesi->dw0x12C, pesi->dw0xF0, pesi->dw0xF4, -pesi->dw_130, edi);
				DeallocateMemory(edi);
			}
			else {
				//get pesi->hg_B8[0] 
				sub_10014680(pesi->hg0xB0, pesi->hg_B8[0], pesi->dw0xCC, pesi->dw0xD0, pesi->dw0xF0, pesi->dw0xF4, 1 >> (unsigned char)(pesi->dw0xF0 - 1), pesi->dw0x110);
			}
		}
		//loc_1000EAEF
		if (pesi->pf0x98)
		{
			WiImageBlock a;//var_28
			a.Page = 1;//var_28
			a.Block = 1;//var_14
			a.BlockLeft = 0;//var_10
			a.BlockTop = 0;//var_C
			a.PageLeft = 0;//var_24
			a.PageTop = 0;//var_20
			a.BlockHeight = pesi->dw0xCC;//var_4
			a.PageHeight = pesi->dw0xCC;//var_18
			a.BlockWidth = pesi->dw0xD0;//var_8
			a.PageWidth = pesi->dw0xD0;//var_1C
			for (int i = 0; i < pesi->dw0xCC; i++)//edi
			{
				if (pesi->pf0x98(pesi->pv_9C, &a, i, &pesi->hg_B8[0][i]))
					break;
			}
			if (pesi->hg_B4) {
				sub_1000E660(pesi->hg_B8, pesi->dw0xF4);
				pesi->hg_B8 = 0;
			}
			else {
				sub_1000E620(pesi->hg_B8, pesi->dw0xF4);
				pesi->hg_B8 = 0;
			}

		}
	}
	//loc_1000EBA7
	sub_1000E580(pesi->hg0xB0, pesi->dw0xF4);

}
//k1
void sub_1000EC40(T_size_178 * arg_0) {
	int edi = 3;
	T_size_178 * pesi = arg_0;
	if (pesi->dw0xF4 == edi && (pesi->dw0xF0 != 8 || (pesi->dw0xF4 == 1 && (pesi->dw0xF0 < 8 || pesi->dw0xF0 > 0x10)))) {
		sub_10001550(0x10);
		pesi->dw0x154 = 0x10;
		return;
	}
	if (pesi->dw0x134 <= 2)
	{
		if (pesi->dw0x134 == 1)
		{
			if (!pesi->dwdw_104) {
				if (pesi->dw0x108 != 2)
				{
					if (pesi->dwdw_104)
					{
						//loc_1000ECB4
						if (pesi->dw0x108 == 1)
						{
							sub_10001550(edi);
							pesi->dw0x154 = edi;
							return;
						}
						//loc_1000ECE2
					}
					//loc_1000ECE2
				}
				//loc_1000ED08
				else {
					sub_10001550(edi);
					pesi->dw0x154 = edi;
					return;
				}
			}
			//loc_1000ECB4
			else {
				if (pesi->dw0x108 == 1)
				{
					sub_10001550(edi);
					pesi->dw0x154 = edi;
					return;
				}
				//loc_1000ECE2
			}
		}
		else {
			//loc_1000ECCF
			if (pesi->dw0x134 == 2)
			{
				if (pesi->dwdw_104)
				{
					if (pesi->dwdw_104 == edi)
					{
						//loc_1000ED08
						sub_10001550(edi);
						pesi->dw0x154 = edi;
						return;
					}
					//loc_1000ECE2
				}
				else {
					//loc_1000ED08
					sub_10001550(edi);
					pesi->dw0x154 = edi;
					return;
				}
			}
			//loc_1000ECE2
		}
		//loc_1000ECE2
		if (pesi->dw0xF0 > 8 || pesi->dw0x110 > 0)
		{
			pesi->dw0x13C = 0;
		}
		if (pesi->dw_130 <= 0) {
			return;
		}
	}
	//loc_1000ED08
	sub_10001550(edi);
	pesi->dw0x154 = edi;
	return;
}
//k
unsigned char sub_1000ED20(T_size_178 * arg_0) {
	sub_1000ED60(arg_0);
	while (sub_1000F050(arg_0));
	sub_1000F8D0(arg_0);
	return 1;
}
//k
unsigned char sub_1000ED60(T_size_178 * arg_0) {
	T_size_178 * pesi = arg_0;
	pesi->hg0xBC = (unsigned char***)AllocateMemorySize(pesi->dw0xF4 * 4 * factor64);
	for (int i = 0; i < pesi->dw0xF4; i++) {//ebx
		pesi->hg0xBC[i] = (unsigned char**)sub_10001470(pesi->dw0xCC, pesi->dw0xD0, 1);
		if (!pesi->hg0xBC[i]) {
			for (int j = 0; j < i; j++) {//edi
				DeallocateMemory(pesi->hg0xBC[j]);
			}
			DeallocateMemory(pesi->hg0xBC);
			pesi->dw0x154 = 8;
			return 0;
		}
		memset(pesi->hg0xBC[0], 1, pesi->dw0xCC * pesi->dw0xD0);
	}
	pesi->dw0x11C = sub_1000DFA0(pesi, 0);
	for (int i = 1; i < pesi->dw0xF4; i++) {//edi
		sub_1000DFA0(pesi, i);
	}
	pesi->dw0x150 = 1;
	if (pesi->dw0xF4 == 3)
		sub_100143E0(pesi->hg0xB0, pesi->dw0xCC, pesi->dw0xD0, pesi->dw_FC);
	if (!pesi->dw_FC) {
		sub_100144F0(pesi->hg0xB0, pesi->hg_B8[0], pesi->dw0xCC, pesi->dw0xD0, pesi->dw0xF4, -pesi->dw_130);
	}
	int(*pfeax)(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C);
	int(*pfecx)(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C);

	if (pesi->dw0x114 <= 0 && pesi->dw0x118 <= 0)
	{
		switch (pesi->dwdw_104)
		{
		case 0:
			pfeax = sub_10019180;
			pfecx = sub_10018E60;
			break;
		case 1:
			pfeax = sub_10018A70;
			pfecx = sub_10018760;
			break;
		case 2:
			pfeax = sub_10018360;
			pfecx = sub_10018030;
			break;
		case 3:
			pfeax = sub_10017C80;
			pfecx = sub_10017970;
			break;
		default:
			pfeax = (int(*)(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C))arg_0;
			pfecx = (int(*)(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C))arg_0;
			break;
		}
	}
	else {
		switch (pesi->dwdw_104)
		{
		case 0:
			pfeax = sub_10019300;
			pfecx = sub_10018FE0;
			break;
		case 1:
			pfeax = sub_10018BE0;
			pfecx = sub_100188D0;
			break;
		case 2:
			pfeax = sub_100184E0;
			pfecx = sub_100181B0;
			break;
		case 3:
			pfeax = sub_10017DF0;
			pfecx = sub_10017AE0;
			break;
		default:
			pfeax = (int(*)(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C))arg_0;
			pfecx = (int(*)(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C))arg_0;
			break;
		}
	}
	//loc_1000EF36
	if (!(pesi->dw0x108)) {
		if (!pesi->dwdw_104) {
			sub_10015FF0(&pesi->t_6C, pesi->dw0xCC, pesi->dw0xD0, pesi->dw_FC, pfeax, pfecx, pesi->dw0x13C);
		}
		else
			sub_10015980(&pesi->t_6C, pesi->dw0xCC, pesi->dw0xD0, pesi->dw_FC, pfeax, pfecx, pesi->dw0x13C);
	}
	else if (!(pesi->dw0x108 - 1)) {
		sub_10015FF0(&pesi->t_6C, pesi->dw0xCC, pesi->dw0xD0, pesi->dw_FC, pfeax, pfecx, pesi->dw0x13C);
	}
	else if (!(pesi->dw0x108 - 2)) {
		sub_10016B70(&pesi->t_6C, pesi->dw0xCC, pesi->dw0xD0, pesi->dw_FC, pfeax, pfecx, pesi->dw0x13C);
	}
	pesi->dw0x100 = pesi->dw_FC;
	return 1;
}
//k1
unsigned char sub_1000F050(T_size_178 * arg_0) {
	T_size_178 * pesi = arg_0;
	pesi->dw0x100--;
	if (pesi->dw0x100 < 0)return 0;
	pesi->dw0x150 = 0;
	int** pebp;
	if (!pesi->dw0x100 && pesi->dw0x120 > 0) {
		if (pesi->dw0x13C) {
			pebp = (int**)sub_1000E5C0(pesi->dw0xCC / 2 + 1, pesi->dw0xD0 / 2 + 1);
		}
		else
			pebp = (int**)sub_1000E5C0(pesi->dw0xCC, pesi->dw0xD0);
	}
	else
		pebp = (int**)arg_0;//?
	sub_10011870(pesi->dw0xCC, pesi->dw0xD0, pesi->dw0x100, 2, &pesi->dw0x20, &pesi->dw0x28, &pesi->dw0x24, &pesi->dw0x2C);
	pesi->dw0x18 = pesi->dw0x100;
	for (int i = 0; i < pesi->dw0xF4; i++) {//edi
		if (!pesi->dw0x100 && pesi->dw0x120 > 0) {
			sub_10019E80(pesi->hg0xB0[i], pebp, pesi->dw0xCC / 2 + 1, pesi->dw0xD0 / 2 + 1, pesi->dw0x120);
		}
		//loc_1000F165
		if (pesi->dw0x13C && !pesi->dw0x100) {
			if (pesi->dw0xF4 == 1)
			{
				sub_10019B00(pesi->hg0xB0[i], pesi->hg_B8[i], pesi->dw0xCC, pesi->dw0xD0);
			}
			else
				sub_10019760(pesi->hg0xB0[i], pesi->hg_B8[i], pesi->dw0xCC, pesi->dw0xD0);

		}
		else
			sub_100194C0(pesi->hg0xB0[i], pesi->dw0xCC, pesi->dw0xD0, pesi->dw0x100);
	}
	if (!pesi->dw0x100 && pesi->dw0x120 > 0) {
		sub_1000E600(pebp);
	}
	if (pesi->dw0x13C && !pesi->dw0x100)
		pesi->dw0x150 = 2;
	else
		pesi->dw0x150 = 1;
	if (sub_1000AB10(pesi->pt0xA8, 1)) {
		for (int i = 0; i < pesi->dw0xF4; i++) {//ebp
			if (pesi->dw0x13C && !pesi->dw0x100 && i)
			{
				return 1;
			}
			switch (pesi->dwdw_104)
			{
			case 0:
				sub_10018D90(pesi->pt0xA8, pesi, pesi->hg0xB0, pesi->hg_B8, pesi->dw0xCC, pesi->dw0xD0, i, pesi->dw0xF4, pesi->dw0x10C, pesi->dw0x118, pesi->dw0x114, pesi->dw0x124);
				break;
			case 1:
				sub_100186A0(pesi->pt0xA8, pesi, pesi->hg0xB0, pesi->hg_B8, pesi->dw0xCC, pesi->dw0xD0, i, pesi->dw0xF4, pesi->dw0x10C, pesi->dw0x118, pesi->dw0x114, pesi->dw0x124);
				break;
			case 2:
				sub_10017FA0(pesi->pt0xA8, pesi, pesi->hg0xB0, pesi->hg_B8, pesi->dw0xCC, pesi->dw0xD0, i, pesi->dw0xF4, pesi->dw0x10C, pesi->dw0x118, pesi->dw0x114, pesi->dw0x124);
				break;
			case 3:
				sub_100178D0(pesi->pt0xA8, pesi, pesi->hg0xB0, pesi->hg_B8, pesi->dw0xCC, pesi->dw0xD0, i, pesi->dw0xF4, pesi->dw0x10C, pesi->dw0x118, pesi->dw0x114, pesi->dw0x124);
				break;
			default:
				break;
			}
			for (int j = 0; j < 3; j++) {//edi
				pesi->dw0x1C = j;
				if (!(pesi->dw0x108))
				{
					//loc_1000F43F
					if (!pesi->dwdw_104)
					{
						sub_10016050(pesi->pt0xA8, pesi, &pesi->t_6C, pesi->hg0xBC[j], pesi->dw0xCC, pesi->dw0xD0, pesi->dw0x100, j);
					}
					else
						sub_10015A00(pesi->pt0xA8, pesi, &pesi->t_6C, pesi->hg0xBC[j], pesi->dw0xCC, pesi->dw0xD0, pesi->dw0x100, j);
				}
				else if (!(pesi->dw0x108 - 1)) {
					sub_10016300(pesi->pt0xA8, pesi, &pesi->t_6C, pesi->hg0xBC[j], pesi->dw0xCC, pesi->dw0xD0, pesi->dw0x100, j);
				}
				else if (!(pesi->dw0x108 - 2)) {
					sub_10016C10(pesi->pt0xA8, pesi, &pesi->t_6C, pesi->hg0xBC[j], pesi->dw0xCC, pesi->dw0xD0, pesi->dw0x100, j);
				}
				if (((unsigned char)pesi->pt0xA8->dw0x14) & 1) {
					pesi->dw0x154 = 10;
					break;
				}
			}
			//loc_1000F4CD
			switch (pesi->dwdw_104) {
			case 0:
				sub_10018E40(pesi->pt0xA8, pesi);
				break;
			case 1:
				sub_100272D0(pesi);
				break;
			case 2:
				//nullsub_1();
				break;
			case 3:
				//nullsub_1();
				break;
			default:
				break;
			}

		}
	}
	return 1;
}
//k
void sub_1000F8D0(T_size_178 * arg_0) {
	T_size_178 * pesi = arg_0;
	if (!(pesi->dw0x108)) {
		if (!pesi->dwdw_104) {
			sub_10016030(&pesi->t_6C);
		}
		else
			sub_100159D0(&pesi->t_6C);
	}
	else if (!(pesi->dw0x108 - 1)) {
		sub_10016030(&pesi->t_6C);
	}
	else if (!(pesi->dw0x108 - 2))
		sub_10016BC0(&pesi->t_6C);
	for (int i = 0; i < pesi->dw0xF4; i++) {//edi
		DeallocateMemory(pesi->hg0xBC[i]);
	}
	DeallocateMemory(pesi->hg0xBC);
}
//k
void sub_1000F960(T_size_178 * arg_0) {
	T_size_178 * esi = arg_0;
	esi->dw0x154 = 0;
	if (esi->dw0xC0 != 3 && (esi->dw0xC0 != 4 || esi->dw0xD4 != 1)) {
		sub_10001550(2);
		esi->dw0x154 = 2;
		return;
	}
	sub_1000EC40(esi);
	if (esi->dw0x154) return;
	if (esi->dw0x148 != 0)
	{
		esi->aby_14C[0] = 0;
		esi->aby_14C[1] = 0;
		esi->aby_14C[2] = 0;
	}
	esi->dw0xF0 = sub_10011840(esi->dw0xCC, esi->dw0xD0);
	if (esi->dw_130 < 0) {
		esi->dw_FC = sub_10011840(esi->dw0xCC, esi->dw0xD0);
		if ((-esi->dw_130) > esi->dw_FC)
		{
			esi->dw_130 = -esi->dw_FC;
		}
		esi->dw_FC += esi->dw_130;
		int edx = 1 << (unsigned char)(-esi->dw_130 + 1);
		esi->dw0xCC = (edx + esi->dw0xCC - 2) >> (unsigned char)(-esi->dw_130);
		esi->dw0xD0 = (edx + esi->dw0xD0 - 2) >> (unsigned char)(-esi->dw_130);
	}
	else {
		esi->dw_130 = 0;
		//loc_1000FA84
	}
	//loc_1000FA84
	if (!esi->pf0x98)
	{
		if (esi->hg_B4)
		{
			int flag = 2;
			if (esi->dw0xF4 != 1 || esi->dw0xF0 <= 8)
				flag = 1;
			esi->hg_B8 = sub_1000E340(esi->dw0xCC, esi->dw0xD0 * flag, esi->dw0xF4);
			if (!esi->hg_B8)
			{
				esi->dw0x154 = 8;
				return;
			}
			sub_1000E390(esi->hg_B8, esi->hg_B4, esi->dw0xCC, esi->dw0xD0, esi->dw0xF0, esi->dw0xF4);
			//loc_1000FB20
		}
		else {
			int flag = 2;
			if (esi->dw0xF4 != 1 || esi->dw0xF0 <= 8)
				flag = 1;
			esi->hg_B8 = sub_1000E260(esi->dw0xCC, esi->dw0xD0 * flag, esi->dw0xF4);
			if (!esi->hg_B8)
			{
				esi->dw0x154 = 8;
				return;
			}
			//loc_1000FB20
		}
		//loc_1000FB20
		sub_100014E0(esi->hg_B8[0], esi->dw0xCC, esi->dw0xD0, esi->aby_14C, esi->dw0xF4 * ((esi->dw0xF0 + 7) / 8));
		//loc_1000FCB0
	}
	//loc_1000FBB9
	else {
		unsigned char* hg_1A4;//1A4
		WiImageBlock var_1A0;
		if (esi->dw0x148 == 0) {
			hg_1A4 = (unsigned char*)AllocateMemorySize(esi->dw0xD0 * esi->dw0xF4 * ((esi->dw0xF0 + 7) / 8));
			if (!hg_1A4) {
				esi->dw0x154 = 8;
				sub_10001550(8);
				return;
			}
			sub_100014E0(&hg_1A4, 1, esi->dw0xD0, esi->aby_14C, esi->dw0xF4 * ((esi->dw0xF0 + 7) / 8));
			var_1A0.Block = 0;
			var_1A0.Page = 0;
			var_1A0.BlockTop = 0;
			var_1A0.BlockLeft = 0;
			var_1A0.PageTop = 0;
			var_1A0.PageLeft = 0;
			var_1A0.BlockHeight = esi->dw0xCC;
			var_1A0.PageHeight = esi->dw0xCC;
			var_1A0.BlockWidth = esi->dw0xD0;
			var_1A0.PageWidth = esi->dw0xD0;
			for (int i = 0; i < esi->dw0xCC; i++)//edi
			{
				if (esi->pf0x98(esi->pv_9C, &var_1A0, i, &hg_1A4))
				{
					break;
				}
			}
			//loc_1000FCA3
			DeallocateMemory(hg_1A4);
			//loc_1000FCB0
		}
		//loc_1000FCB0
	}
	T_size_178 var_178;
	var_178.dw0xC0 = esi->dw0xC0;
	esi->pt0x164 = &var_178;
	var_178.pf0x98 = esi->pf0x98;
	var_178.dw_130 = esi->dw_130;
	var_178.pt0xA8 = esi->pt0xA8;
	var_178.pt_160 = esi;
	var_178.dw0x154 = 0;
	var_178.pv_9C = esi->pv_9C;
	var_178.dw0x158 = 1;
	var_178.dw0x150 = 0;
	var_178.hg_B4 = esi->hg_B4;
	esi->dw0x150 = 1;
	sub_1000FDB0(&var_178);
	if (var_178.dw0x154 != 0)
	{
		esi->dw0x154 = var_178.dw0x154;
	}
	if (esi->dw0x158 == -2) {
		do {
			var_178.dw0x158++;
			var_178.dw0x154 = 0;
			sub_1000FDB0(&var_178);
			if (var_178.dw0xEC == 0)break;
			esi->dw0x154 = var_178.dw0x154;
		} while (var_178.dw0x154 != 0x13);
	}
	esi->dw0x150 = 3;
	//loc_1000FDA3
}
//k
void sub_1000FDB0(T_size_178 * arg_0) {
	T_size_178 * pebp = arg_0;
	sub_1000A970(pebp->pt0xA8, 0);
	sub_1000DCD0(pebp);
	if (!pebp->dw0xEC) return;
	sub_1000EC40(pebp);
	if (pebp->dw0x154) return;
	if (pebp->pt_160->AE_170) {
		DeallocateMemory(pebp->pt_160->AE_170);
	}
	pebp->pt_160->AE_170 = pebp->AE_170;
	pebp->pt_160->AELen_174 = pebp->AELen_174;
	T_size_178 * pedx = pebp->pt_160;
	if (pedx->pf_A0 != NULL) {
		if (pebp->pt_160->AE_170 && pebp->pt_160->AELen_174 > 0)
			pedx->pf_A0(pedx->pv_A4, pebp->dw0x158, &pebp->pt_160->AE_170, &pebp->pt_160->AELen_174);
	}
	if ((pebp->dw0xCC + pebp->dw_C8) <= pedx->dw0x128 && (pebp->dw_C4 + pebp->dw0xD0) <= pedx->dw0x12C) {
		if (!pebp->dw0xEC) return;
		if (pebp->dw0xEC > 3) {
			sub_10001550(3);
			pebp->dw0x154 = 3;
			return;
		}
		pebp->dw_FC = sub_10011840(pebp->dw0xCC, pebp->dw0xD0);
		if (pebp->dw_130 < 0) {
			if (-pebp->dw_130 > pebp->dw_FC) {
				pebp->dw_130 = -pebp->dw_FC;
			}
			//loc_1000FF17
			pebp->dw_FC += pebp->dw_130;
			int iedx = 1 << (unsigned char)(-pebp->dw_130 + 1);
			pebp->dw0xCC = (pebp->dw0xCC + iedx - 2) >> (unsigned char)(-pebp->dw_130);
			pebp->dw0xD0 = (pebp->dw0xD0 + iedx - 2) >> (unsigned char)(-pebp->dw_130);
			if ((pebp->dw_C4 >> (unsigned char)(-pebp->dw_130)) >= (pebp->pt_160->dw0xD0 - pebp->dw0xD0)) {
				pebp->dw_C4 = pebp->pt_160->dw0xD0 - pebp->dw0xD0;
			}
			else
				pebp->dw_C4 >>= (unsigned char)(-pebp->dw_130);
			if ((pebp->dw_C8 >> (unsigned char)(-pebp->dw_130)) >= (pebp->pt_160->dw0xCC - pebp->dw0xCC)) {
				pebp->dw_C8 = pebp->pt_160->dw0xCC - pebp->dw0xCC;
			}
			else
				pebp->dw_C8 >>= (unsigned char)(-pebp->dw_130);
		}
		else {
			pebp->dw_130 = 0;
		}
		//loc_1000FFA3
		if (!pebp->pf0x98) {
			pebp->hg_B8 = sub_1000E420(pebp->pt_160->hg_B8, pebp->dw_C8, pebp->dw_C4, pebp->dw0xCC, pebp->dw0xD0, pebp->dw0xF0, pebp->dw0xF4);
			if (!pebp->dw0x148) {
				sub_100014E0(pebp->hg_B8[0], pebp->dw0xCC, pebp->dw0xD0, pebp->aby_14C, pebp->dw0xF4 * ((pebp->dw0xF0 + 7) / 8));
			}
			//loc_10010145
		}
		else if (!pebp->dw0x148) {
			unsigned char* var_1C8 = (unsigned char*)AllocateMemorySize(pebp->dw0xD0 * pebp->dw0xF4 * ((pebp->dw0xF0 + 7) / 8));
			if (!var_1C8)
			{
				pebp->dw0x154 = 8; sub_10001550(8);
				return;
			}
			else {
				sub_100014E0(&var_1C8, 1, pebp->dw0xD0, pebp->aby_14C, pebp->dw0xF4 * (pebp->dw0xF0 + 7) / 8);
				WiImageBlock var_1A0;//178
				var_1A0.Page = pebp->dw0x158;
				var_1A0.BlockLeft = pebp->dw_C4;
				var_1A0.PageLeft = pebp->dw_C4;
				var_1A0.BlockTop = pebp->dw_C8;
				var_1A0.PageTop = pebp->dw_C8;
				var_1A0.Block = 0;
				var_1A0.BlockHeight = pebp->dw0xCC;
				var_1A0.PageHeight = pebp->dw0xCC;
				var_1A0.BlockWidth = pebp->dw0xD0;
				var_1A0.PageWidth = pebp->dw0xD0;
				for (int i = 0; i < pebp->dw0xCC; i++) {//esi
					if (pebp->pf0x98(pebp->pv_9C, &var_1A0, i, &var_1C8)) {
						break;
					}
				}
				DeallocateMemory(var_1C8);
			}
		}
		//loc_10010145
		sub_1000A970(pebp->pt0xA8, 0);
		T_size_178 var_178;
		var_178.dw0xC0 = pebp->dw0xC0;
		var_178.pf0x98 = pebp->pf0x98;
		var_178.dw0x158 = pebp->dw0x158;
		var_178.pt_160 = pebp;
		var_178.pv_9C = pebp->pv_9C;
		var_178.dw0x15C = 1;
		var_178.hg_B4 = pebp->hg_B4;
		pebp->dw0x150 = 1;
		while (1) {
			//loc_100101AF
			var_178.dw0x150 = 0;
			int iebx = sub_1000DDA0(pebp->pt0xA8);
			if (iebx) {
				sub_1000A970(pebp->pt0xA8, 0);
				T_size_24 var_1C4;
				memcpy(&var_1C4, pebp->pt0xA8, 0x24);
				if (var_1C4.pf0x1C) {
					var_1C4.pb_04 = var_1C4.pb_00;
				}
				else
					var_1C4.pb_00 = var_1C4.pb_04;
				pebp->pt0x164 = &var_178;
				var_1C4.dw0x10 = iebx;
				var_178.dw0x154 = 0;
				var_178.pt0xA8 = &var_1C4;
				var_178.dw_130 = pebp->dw_130;
				sub_10010390(&var_178);
				if (var_178.dw0x154) {
					pebp->dw0x154 = var_178.dw0x154;
				}
				sub_1000A970(&var_1C4, 0);
				int ieax = var_1C4.pb_00 - var_1C4.pb_04 + iebx;
				if (!(((unsigned char)var_1C4.dw0x14) & 2)) {
					if (ieax < 0 || var_178.dw0x154 == 0x13) {
						var_178.dw0x154 = 0x13;
						if (!pebp->pf0x98)
							sub_1000E660(pebp->hg_B8, pebp->dw0xF4);
						pebp->hg_B8 = 0;
						return;
					}
					pebp->pt0xA8->pb_04 = var_1C4.pb_04;
					sub_1000A970(pebp->pt0xA8, ieax);
					var_178.dw0x15C++;
					continue;
				}
				else
				{
					var_178.dw0x154 = 10;
					if (!pebp->pf0x98) {
						sub_1000E660(pebp->hg_B8, pebp->dw0xF4);
					}
					pebp->hg_B8 = 0;
					return;
				}
			}
			pebp->dw0x150 = 3;
			if (!pebp->pf0x98) {
				sub_1000E660(pebp->hg_B8, pebp->dw0xF4);
			}
			pebp->hg_B8 = 0;
			return;
		}
	}
	else {
		sub_10001550(0x13);
		pebp->dw0x154 = 0x13;
	}
}
int sub_1000DFA0(T_size_178 * arg_0, int arg_4) {
	T_size_178 * esi = arg_0;
	T_size_24 * var_1C = esi->pt0xA8;
	//int** pedi = esi->hg0xB0[arg_4];
	//int iebx = esi->dw0x10C;
	//int var_4 = esi->dw_FC;
	int a = 1 << (unsigned char)(esi->dw_FC + 1);
	int var_C = (esi->dw0xCC + a - 2) >> (unsigned char)esi->dw_FC;
	int iebp = (esi->dw0xD0 + a - 2) >> (unsigned char)esi->dw_FC;
	unsigned char bvar_8 = sub_1000AB10(var_1C, 5);
	unsigned char var_14;
	if (bvar_8 > 0)
		var_14 = sub_1000AB10(var_1C, bvar_8);
	else
		var_14 = 0;
	int sum = 0;//var_18
	int iebx = esi->dw0x10C * 32;
	for (int i = var_C; i > 0; i--) {//var_10
		for (int j = 0; j < iebp; j++) {//esi
			int ieax;
			if (bvar_8 > 0) //6 ,6
				ieax = sub_1000AB10(var_1C, bvar_8);
			else
				ieax = 0;
			ieax -= var_14;//2d good
			if (ieax > 0)
				esi->hg0xB0[arg_4][var_C - i][j] = (ieax * 2 + 1) * iebx;
			else if (!ieax)
				esi->hg0xB0[arg_4][var_C - i][j] = (arg_4 == 0 ? iebx : 0);
			else
				esi->hg0xB0[arg_4][var_C - i][j] = (ieax * 2 - 1) * iebx;
			//loc_1000E0A1
			sum += esi->hg0xB0[arg_4][var_C - i][j];
		}
	}
	//loc_1000E0CB
	iebp *= var_C;
	return (sum / iebp + (1 << (unsigned char)(esi->dw_FC - 1))) >> (unsigned char)(esi->dw_FC - esi->dw_130 + 5);
}
//k
void sub_10010390(T_size_178 * arg_0) {
	T_size_178 * pesi = arg_0;
	pesi->dw0x154 = 0;
	sub_1000DDF0(arg_0);
	if ((pesi->dw_C8 + pesi->dw0xCC) > pesi->pt_160->dw0x128 || (pesi->dw_C4 + pesi->dw0xD0) > pesi->pt_160->dw0x12C || pesi->dw0xF4 * pesi->dw0xD0 * pesi->dw0xCC < pesi->pt0xA8->dw0x10 * 7 / 8) {
		sub_10001550(0x13);
		pesi->dw0x154 = 0x13;
		return;
	}
	sub_1000EC40(pesi);
	if (pesi->dw0x154)
		return;
	pesi->dw_FC = sub_10011840(pesi->dw0xCC, pesi->dw0xD0);//ebp;var_2C
	int var_2C = pesi->dw_FC;//ebp
	int iarg_0 = pesi->dw_130;//edi
	if (pesi->dw_130 < 0) {
		pesi->dw_FC = pesi->dw_130 + pesi->dw_FC;
		int a = 1 << (unsigned char)(-pesi->dw_130 + 1);//edx
		pesi->dw0xCC = (pesi->dw0xCC + a - 2) >> (unsigned char)(-pesi->dw_130);
		pesi->dw0xD0 = (pesi->dw0xD0 + a - 2) >> (unsigned char)(-pesi->dw_130);
		a = 1 << (unsigned char)(-pesi->dw_130 - 1);//ebp
		int iedx = (pesi->dw_C4 + a) >> (unsigned char)(-pesi->dw_130);
		if (iedx < (pesi->pt_160->dw0xD0 - pesi->dw0xD0))
			pesi->dw_C4 = iedx;
		else
			pesi->dw_C4 = pesi->pt_160->dw0xD0 - pesi->dw0xD0;
		iedx = (pesi->dw_C8 + a) >> (unsigned char)(-pesi->dw_130);
		if (iedx < (pesi->pt_160->dw0xCC - pesi->dw0xCC))
			pesi->dw_C8 = iedx;
		else
			pesi->dw_C8 = pesi->pt_160->dw0xCC - pesi->dw0xCC;
		pesi->dw0x13C = 0;
	}
	else
		pesi->dw_130 = 0;
	//loc_10010511
	int iarg_0_1 = pesi->dw0xCC;
	int var_2C_1 = pesi->dw0xD0;
	if (pesi->dw0x13C && pesi->dw_FC > 0) {
		pesi->hg0xB0 = sub_1000E100(pesi->dw0xCC / 2 + 1, pesi->dw0xD0 / 2 + 1, pesi->dw0xF4);
	}
	else if (pesi->dw_FC < 0) {
		int a = 1 << (unsigned char)(var_2C + 1);
		pesi->hg0xB0 = sub_1000E100((pesi->dw0x128 + a - 2) >> (unsigned char)var_2C, (pesi->dw0x12C + a - 2) >> (unsigned char)var_2C, pesi->dw0xF4);
	}
	else {
		pesi->hg0xB0 = sub_1000E100(pesi->dw0xCC, pesi->dw0xD0, pesi->dw0xF4);
	}
	if (!pesi->hg0xB0) {
		pesi->dw0x154 = 8;
		return;
	}
	if (pesi->pf0x98) {
		int flag = 2;
		if (pesi->dw0xF4 != 1 || pesi->dw0xF0 <= 8)
			flag = 1;
		pesi->hg_B8 = sub_1000E260(pesi->dw0xCC, pesi->dw0xD0 * flag, pesi->dw0xF4);
		if (!pesi->hg_B8) {
			sub_1000E580(pesi->hg0xB0, pesi->dw0xF4);
			pesi->dw0x154 = 8;
		}
	}
	else {
		pesi->hg_B8 = (unsigned char***)sub_1000E420(pesi->pt_160->hg_B8, pesi->dw_C8, pesi->dw_C4, pesi->dw0xCC, pesi->dw0xD0, pesi->dw0xF0, pesi->dw0xF4);
	}
	if (pesi->dw_FC < 0) {
		int a = 1 << (unsigned char)(var_2C + 1);
		pesi->dw_FC = 0;
		pesi->dw0xCC = (pesi->dw0x128 + a - 2) >> (unsigned char)var_2C;
		pesi->dw0xD0 = (pesi->dw0x12C + a - 2) >> (unsigned char)var_2C;
	}
	if (!pesi->dw0x134) {
		pesi->dw0x150 = 5;
		sub_10014DE0(pesi);
		if (pesi->dw0x154 == 0xB) {
			pesi->dw0x154 = 0;
		}
	}
	else if (!(pesi->dw0x134 - 1)) {
		pesi->dw0x150 = 0xC;
		sub_1000ED20(pesi);
		if (pesi->dw0x154 == 0xC) {
			pesi->dw0x154 = 0;
		}
	}
	else if (!(pesi->dw0x134 - 1)) {
		pesi->dw0x150 = 0xC;
		sub_10015350(pesi);
		if (pesi->dw0x154 == 0xC) {
			pesi->dw0x154 = 0;
		}
	}
	//loc_10010724
	pesi->dw0x150 = 3;
	if (!pesi->dw0x154 || pesi->dw0x154 == 0xA) {
		if (!pesi->dw0x13C) {
			if (iarg_0 < 0) {
				unsigned short* hebx = (unsigned short*)AllocateMemorySize((1 << (unsigned char)(pesi->dw0xF0)) * 2);
				sub_10014D50(hebx, pesi->dw0xF0, 1 << (unsigned char)(pesi->dw0xF0 - 1), pesi->dw0x110);
				if (iarg_0_1 == pesi->dw0xCC) {
					sub_100146F0(pesi->hg0xB0, *pesi->hg_B8, pesi->dw0x128, pesi->dw0x12C, pesi->dw0xF0, pesi->dw0xF4, -iarg_0, hebx);
					DeallocateMemory(hebx);
				}
				else {
					int a = 1 << (unsigned char)(1 - iarg_0 - pesi->dw_FC);
					int iecx = -(iarg_0 + pesi->dw_FC);
					sub_100146F0(pesi->hg0xB0, *pesi->hg_B8, (pesi->dw0x128 + a - 2) >> (unsigned char)iecx, (pesi->dw0x12C + a - 2) >> (unsigned char)iecx, pesi->dw0xF0, pesi->dw0xF4, var_2C, hebx);
					pesi->dw0xCC = iarg_0_1;
					pesi->dw0xD0 = var_2C_1;
					DeallocateMemory(hebx);
				}
			}
			else {
				sub_10014680(pesi->hg0xB0, *pesi->hg_B8, pesi->dw0xCC, pesi->dw0xD0, pesi->dw0xF0, pesi->dw0xF4, 1 << (unsigned char)(pesi->dw0xF0 - 1), pesi->dw0x110);
			}
		}
		//loc_100108A9
		if (pesi->pf0x98) {
			WiImageBlock var_28;
			var_28.Page = pesi->dw0x158;
			var_28.Block = pesi->dw0x15C;
			var_28.PageLeft = pesi->pt_160->dw_C4;
			var_28.PageTop = pesi->pt_160->dw_C8;
			var_28.PageHeight = pesi->pt_160->dw0xCC;
			var_28.BlockLeft = pesi->dw_C4;
			var_28.PageWidth = pesi->pt_160->dw0xD0;
			var_28.BlockTop = pesi->dw_C8;
			var_28.BlockHeight = pesi->dw0xCC;
			var_28.BlockWidth = pesi->dw0xD0;
			for (int i = 0; i < pesi->dw0xCC; i++) {//edi
				if (pesi->pf0x98(pesi->pv_9C, &var_28, i, &pesi->hg_B8[0][i]))
					break;
			}
			sub_1000E620(pesi->hg_B8, pesi->dw0xF4);
			pesi->hg_B8 = 0;
		}
		else {
			sub_1000E660(pesi->hg_B8, pesi->dw0xF4);
			pesi->hg_B8 = 0;
		}
	}
	sub_1000E580(pesi->hg0xB0, pesi->dw0xF4);
}
//k
void sub_10011870(int arg_0, int arg_4, int arg_8, int arg_C, int* arg_10, int* arg_14, int* arg_18, int* arg_1C) {
	int a = 1 << (unsigned char)(arg_8 + 1);
	int ieax = (arg_0 + a - 2) >> (unsigned char)arg_8;
	int iedx = (arg_4 + a - 2) >> (unsigned char)arg_8;
	int iecx = (ieax + 2) / 2;
	int iesi = (iedx + 2) / 2;
	if (!arg_C) {
		*arg_10 = 0;
		*arg_14 = iecx;
		*arg_18 = iesi;
		*arg_1C = iedx;
	}
	else if (!(arg_C - 1)) {
		*arg_10 = iecx;
		*arg_14 = ieax;
		*arg_18 = 0;
		*arg_1C = iesi;
	}
	else if (!(arg_C - 2)) {
		*arg_10 = iecx;
		*arg_14 = ieax;
		*arg_18 = iesi;
		*arg_1C = iedx;
	}
}
//OK
void sub_10011910(signed char*** arg_0, signed char*** arg_4, int arg_8, int arg_C, int arg_10) {
	signed char **pedi = (signed char**)sub_10001470(3, arg_10 + 1, 1);
	signed char** pebx = (signed char**)sub_10001470(3, arg_10 + 1, 1);
	int var1, var2, var3, var4;
	for (int i = 0; i < arg_10; i++) {//esi
		sub_10011870(arg_8, arg_C, i, 0, &var1, &var2, &var3, &var4);
		pedi[0][i] = (var2 & 1) + 4;
		pebx[0][i] = var4 & 3;
		pebx[2][i] = pebx[0][i];
		sub_10011870(arg_8, arg_C, i, 1, &var1, &var2, &var3, &var4);
		pedi[1][i] = var2 & 3;
		pedi[2][i] = pedi[1][i];
		pebx[1][i] = (var4 & 1) + 4;
	}
	arg_0[0] = pedi;
	arg_4[0] = pebx;
}
//k
void sub_10011A10(signed char** arg_0, signed char** arg_4) {
	DeallocateMemory(arg_0[0]);
	DeallocateMemory(arg_0);
	DeallocateMemory(arg_4);
}
//k
void sub_100143E0(int***  arg_0, int arg_4, int arg_8, int arg_C) {
	int a = 1 << (unsigned char)(arg_C + 1);
	int var_4 = (arg_4 + a - 2) >> (unsigned char)arg_C;
	if (var_4 > 0) {
		int iarg4 = (arg_8 + a - 2) >> (unsigned char)arg_C;
		for (int i = 0; i < var_4; i++) {//var_8 edx
			int ***pesi = arg_0;
			for (int j = 0; j < iarg4; j++) {//arg_C eax
				pesi[0][i][j] = pesi[2][i][j] * 5 / 8 + pesi[1][i][j] + pesi[0][i][j];
				pesi[1][i][j] = (-pesi[2][i][j] * 5) / 8 - pesi[1][i][j] / 4 + pesi[0][i][j];
				pesi[2][i][j] = pesi[2][i][j] * 7 / 4 - pesi[1][i][j] + pesi[0][i][j];
			}
		}
	}
	//loc_100144E2
}
//k
void sub_100144F0(int*** arg_0, unsigned char** arg_4, int arg_8, int arg_C, int arg_10, int arg_14) {
	if (arg_10 == 1) {
		for (int i = arg_8; i > 0; i--) {//arg_8 eax
			int r;
			for (int j = 0; j < arg_C; arg_4[arg_8 - i][j] = (unsigned char)r, j++) {//edx
				if (arg_0[0][arg_8 - i][j] >= (0x100 << (unsigned char)(arg_14 + 5)))
					r = -1;
				else if (arg_0[0][arg_8 - i][j] < 0)
					r = 0;
				else
					r = arg_0[0][arg_8 - i][j] >> (unsigned char)(arg_14 + 5);
			}
		}
		return;
	}
	//loc_10014571
	for (int i = arg_8; i > 0; i--) {//arg_10 edx eax = arg_8 - i
		for (int j = 0; j < arg_C; j++) {//arg_8 ebp esi = j * 3
			int iedi = 0x100 << (unsigned char)(arg_14 + 5);
			int r;
			if (arg_0[0][arg_8 - i][j] >= iedi)
				r = -1;
			else if (arg_0[0][arg_8 - i][j] < 0)
				r = 0;
			else
				r = arg_0[0][arg_8 - i][j] >> (unsigned char)(arg_14 + 5);
			arg_4[arg_8 - i][j * 3 + 0] = (unsigned char)r;

			if (arg_0[1][arg_8 - i][j] >= iedi)
				r = -1;
			else if (arg_0[1][arg_8 - i][j] < 0)
				r = 0;
			else
				r = arg_0[1][arg_8 - i][j] >> (unsigned char)(arg_14 + 5);
			arg_4[arg_8 - i][j * 3 + 1] = (unsigned char)r;

			if (arg_0[2][arg_8 - i][j] >= iedi)
				r = -1;
			else if (arg_0[2][arg_8 - i][j] < 0)
				r = 0;
			else
				r = arg_0[2][arg_8 - i][j] >> (unsigned char)(arg_14 + 5);
			arg_4[arg_8 - i][j * 3 + 2] = (unsigned char)r;
		}
	}
}
//k
void sub_100146F0(int*** hgArg_0, unsigned char** hgArg_4, int arg_8, int arg_C, int arg_10, int arg_14, int arg_18, unsigned short* pwArg_1C /*unsigned short**/) {
	int var_201C = arg_18 + 5;
	int a = 1 << (unsigned char)(arg_18 + 1);
	int var_200C = (arg_8 + a - 2) >> (unsigned char)arg_18;
	int var_2014 = (arg_C + a - 2) >> (unsigned char)arg_18;
	unsigned char*** hvar_2004 = 0;
	unsigned char** hvar_2018;
	unsigned short var_2000[0x1000];
	if (!hgArg_4) {
		int flag = 2;
		if (arg_14 != 1 || arg_10 <= 8)
			flag = 1;
		hvar_2004 = sub_1000E260(var_200C, var_2014 * flag, arg_14);
		if (!hvar_2004) {
			return;
		}
		hvar_2018 = hvar_2004[0];
	}
	else
		hvar_2018 = hgArg_4;
	if (!pwArg_1C) {
		pwArg_1C = var_2000;
		sub_10014D50(var_2000, arg_10, 1 << (unsigned char)(arg_10 - 1), 0);
	}
	if (arg_14 == 1) {
		if (arg_10 <= 8) {
			for (int i = 0; i < var_200C; i++) {//var_2020 eax
				int * pesi = hgArg_0[0][i];
				unsigned char * pbedi = hvar_2018[i];
				//loc_100147F4
				int var = 1 << (unsigned char)arg_10;
				for (int j = var_2014; j > 0; j--) {//ebx esi = var_2014 - j edi = var_2014 - j
					int ieax = pesi[var_2014 - j] >> (unsigned char)var_201C;
					int r;
					if (ieax >= var)
						r = var - 1;
					else if (ieax < 0)
						r = 0;
					else
						r = pwArg_1C[ieax];
					pbedi[var_2014 - j] = (unsigned char)r;
				}
			}
			//loc_100149F6
		}
		else {
			for (int i = 0; i < var_200C; i++) {//var_2020
				int * pebx = hgArg_0[0][i];
				unsigned char * pbesi = hvar_2018[i];
				int var = 1 << (unsigned char)arg_10;
				for (int j = var_2014; j > 0; j--) {//ebp
					int iedx = pebx[var_2014 - j] >> (unsigned char)var_201C;
					int r;
					if (iedx >= var)
						r = var - 1;
					else if (iedx < 0)
						r = 0;
					else
						r = pwArg_1C[iedx];
					pbesi[(var_2014 - j) * 2 + 0] = (unsigned char)r;
					pbesi[(var_2014 - j) * 2 + 1] = (unsigned char)(r >> 8);
				}
			}
			//loc_100149F6
		}
	}
	else {
		//loc_100148E9
		for (int i = 0; i < var_200C; i++) {//var_2020 ecx
			int *var_2010 = hgArg_0[0][i];
			int *pebx = hgArg_0[1][i];
			int *pedi = hgArg_0[2][i];
			unsigned char *pbedx = hvar_2018[i];
			int var = 1 << (unsigned char)arg_10;
			for (int j = var_2014; j > 0; j--) {//var_2008 eax
				int ieax = var_2010[var_2014 - j] >> (unsigned char)var_201C;
				int r;
				if (ieax >= var)
					r = var - 1;
				else if (ieax < 0)
					r = 0;
				else
					r = pwArg_1C[ieax];
				pbedx[(var_2014 - j) * 3 + 0] = (unsigned char)r;

				ieax = pebx[var_2014 - j] >> (unsigned char)var_201C;
				if (ieax >= var)
					r = var - 1;
				else if (ieax < 0)
					r = 0;
				else
					r = pwArg_1C[ieax];
				pbedx[(var_2014 - j) * 3 + 1] = (unsigned char)r;

				ieax = pedi[var_2014 - j] >> (unsigned char)var_201C;
				if (ieax >= var)
					r = var - 1;
				else if (ieax < 0)
					r = 0;
				else
					r = pwArg_1C[ieax];
				pbedx[(var_2014 - j) * 3 + 2] = (unsigned char)r;
			}
		}
	}
	if (!hgArg_4) {
		sub_1000E540(hvar_2004, arg_8, arg_C, arg_14);
	}
}
//k get pesi->hg_B8[0] 
unsigned char** sub_10014680(int*** hgMem, unsigned char** hgArg_4, int arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C) {
	unsigned short* hesi = (unsigned short*)AllocateMemorySize((1 << (unsigned char)arg_10) * 2);
	if (!hesi) return 0;
	sub_10014D50(hesi, arg_10, arg_18, arg_1C);
	unsigned char** hedi = sub_10014A40(hgMem, hgArg_4, arg_8, arg_C, arg_10, arg_14, hesi);
	DeallocateMemory(hesi);
	return hedi;
}
//k
unsigned char** sub_10014A40(int*** hgMem, unsigned char** hgArg_4, int arg_8, int arg_C, int arg_10, int arg_14, unsigned short* pwArg_18)
{
	unsigned char*** hvar_2004 = 0;
	unsigned char **var_200C;
	unsigned short var_2000[0x1000];
	if (!hgArg_4) {
		int flag = 2;
		if (arg_14 != 1 || arg_10 <= 8) {
			flag = 1;
		}
		hvar_2004 = sub_1000E260(arg_8, arg_C * flag, arg_14);
		if (!hvar_2004) return 0;
		var_200C = hvar_2004[0];
	}
	else
		var_200C = hgArg_4;
	if (!pwArg_18) {
		pwArg_18 = var_2000;
		sub_10014D50(var_2000, arg_10, 1 << (unsigned char)(arg_10 - 1), 0);
	}
	if (arg_14 == 1) {
		if (arg_10 <= 8) {
			for (int i = 0; i < arg_8; i++) {//iedi
				unsigned char * pbesi = var_200C[i];
				int * piedx = hgMem[0][i];
				for (int j = arg_C; j > 0; j--) {
					int ieax = piedx[arg_C - j] + 0x10;
					int iecx;
					int r;
					if (ieax >= 0x2000)
						r = -1;
					else if (ieax < 0)
						r = 0;
					else
						r = pwArg_18[ieax / 32];
					pbesi[arg_C - j] = (unsigned char)r;
				}
			}
			//loc_10014C06
		}
		else {
			for (int i = 0; i < arg_8; i++) {//var_2010
				int * piesi = hgMem[0][i];
				unsigned char * pbedx = var_200C[i];
				int var = 1 << (unsigned char)(arg_10 + 5);
				for (int j = arg_C; j > 0; j--) {//ebx
					int iecx = piesi[arg_C - j];
					int r;
					if (iecx >= var)
						r = (1 << (unsigned char)arg_10) - 1;
					else if (iecx < 0)
						r = 0;
					else
						r = pwArg_18[iecx / 32];
					pbedx[(arg_C - j) * 2 + 0] = (unsigned char)r;
					pbedx[(arg_C - j) * 2 + 1] = (unsigned char)(r >> 8);

				}
				//loc_10014C06
			}
		}
	}
	else {
		//loc_10014C3A
		for (int i = 0; i < arg_8; i++) {//var_2010 ecx
			int *pebp = hgMem[0][i];
			int *pedi = hgMem[1][i];
			int *pesi = hgMem[2][i];
			unsigned char *pecx = var_200C[i];
			for (int j = arg_C; j > 0; j--) {//var_2008
				int ieax = pebp[arg_C - j] + 0x10;
				int r;
				if (ieax >= 0x2000)
					r = -1;
				else if (ieax < 0)
					r = 0;
				else
					r = pwArg_18[ieax / 32];
				pecx[(arg_C - j) * 3 + 0] = (unsigned char)r;

				ieax = pedi[arg_C - j] + 0x10;
				if (ieax >= 0x2000)
					r = -1;
				else if (ieax < 0)
					r = 0;
				else
					r = pwArg_18[ieax / 32];
				pecx[(arg_C - j) * 3 + 1] = (unsigned char)r;

				ieax = pesi[arg_C - j] + 0x10;
				if (ieax >= 0x2000)
					r = -1;
				else if (ieax < 0)
					r = 0;
				else
					r = pwArg_18[ieax / 32];
				pecx[(arg_C - j) * 3 + 2] = (unsigned char)r;
			}
			//loc_10014C06
		}
	}
	//loc_10014C06
	if (!hgArg_4)
		return (unsigned char**)sub_1000E540(hvar_2004, arg_8, arg_C, arg_14);
	return hgArg_4;
}
//k
void sub_10014D50(unsigned short* hgMem, int arg_4, int arg_8, int arg_C) {
	int iesi = 1 << ((unsigned char)arg_4);
	if (!arg_C) {
		for (int i = 0; i < iesi; i++) {//eax
			hgMem[i] = i;
		}
		return;
	}
	int ieax = iesi / 2;
	int iarg_4 = arg_C + 1;
	int iebp = arg_8 * iarg_4;
	unsigned short * pwedi = hgMem;
	int iedx = 0;
	for (int i = iesi; i > 0; i--)//arg_0 ecx
	{
		int iecx = (iedx - iebp + ieax);
		if (iecx >= iesi) {
			hgMem[iesi - i] = (unsigned short)(iesi - 1);//edi
		}
		else {
			hgMem[iesi - i] = iecx < 0 ? 0 : iecx;
		}
		iedx += iarg_4;
	}
}
//k get pesi->hg0xB0
unsigned char sub_10014DE0(T_size_178 * arg_0) {
	T_size_178 * pesi = arg_0;
	int var_18[3];
	unsigned char ** var_C[3]/*unknown*/;
	int var_1C = sub_10011840(pesi->dw0x128, pesi->dw0x12C);//ebx
	for (int i = 0; i < pesi->dw0xF4; i++) {//ebp
		var_C[i] = (unsigned char**)sub_10001470(pesi->dw0x128, pesi->dw0x12C, 1);//pebx
		if (var_C[i]) {
			memset(var_C[i][0], 1, pesi->dw0x128 * pesi->dw0x12C);
		}
		else {
			unsigned char*** pedi = &var_C[i]/*&var_C[iebp]*/;
			for (; i > 0; i--) {//ebp
				DeallocateMemory(*pedi);
			}
			pesi->dw0x154 = 8;
			return 0;
		}
	}
	//loc_10014E71
	pesi->dw0x11C = sub_1000DFA0(pesi, 0);//e1 good
	for (int i = 1; i < pesi->dw0xF4; i++) {//ebp
		sub_1000DFA0(pesi, i);
	}
	if (pesi->dw0xF4 == 3)
	{
		if (var_1C == 0 || !pesi->dw0x13C)
		{
			sub_100143E0(pesi->hg0xB0, pesi->dw0xCC, pesi->dw0xD0, pesi->dw_FC);
		}
	}
	//loc_10014EE1
	if (!pesi->dw_FC) {
		if (pesi->dw0x13C) {
			sub_100144F0(pesi->hg0xB0, pesi->hg_B8[0], pesi->dw0xCC, pesi->dw0xD0, pesi->dw0xF4, -pesi->dw_130);
		}
		for (int i = 0; i < pesi->dw0xF4; i++)//edi
			DeallocateMemory(var_C[i]);
		return 1;
	}
	//loc_10014F65
	for (int i = 0; i < pesi->dw0xF4; i++) {//iebp
		for (var_18[i] = 0; sub_1000AB10(pesi->pt0xA8, 1); var_18[i]++);// + 1
		int ieax = var_18[i];
		var_18[i] += pesi->dw_130;
		if (ieax <= var_1C) {//ebx
			int a = 1 << (unsigned char)(ieax + 1);
			sub_10015150(pesi->pt0xA8, var_C[i], (pesi->dw0x128 + a - 2) >> (unsigned char)ieax, (pesi->dw0x12C + a - 2) >> (unsigned char)ieax, var_1C - ieax, pesi->dw0x108);
			if ((unsigned char)pesi->pt0xA8->dw0x14 & 1) {
				//loc_10015046
				pesi->dw0x154 = 10;
				break;
			}
		}
		//loc_1001502D
		else {
			pesi->dw0x154 = 10;
			pesi->pt0xA8->dw0x14 = 1;
			break;
		}
	}
	//loc_10015050
	if (pesi->dw0x154 == 0xA) {
		if (pesi->dw0xF4 > 0) {
			memset(var_18, pesi->dw_FC, pesi->dw0xF4);
		}
	}
	sub_100151D0(pesi->pt0xA8, pesi->hg0xB0, pesi->hg_B8, var_C, pesi->dw0xCC, pesi->dw0xD0, pesi->dw0xF4, pesi->dw_FC, var_18, pesi->dw0x10C, pesi->dwdw_104, pesi->dw0x114, pesi->dw0x118, pesi->dw0x120, pesi->dw0x124, pesi->dw0x13C);
	for (int i = 0; i < pesi->dw0xF4; i++) {//edi
		DeallocateMemory(*var_C[i]);
		DeallocateMemory(var_C[i]);
	}
	return 1;
}
//k
void sub_10015150(T_size_24 * arg_0, unsigned char** arg_4, int arg_8, int arg_C, int arg_10, int arg_14) {
	if (!arg_14) {
		sub_100201D0(arg_0, arg_4, arg_8, arg_C, arg_10);
		return;
	}
	if (!(arg_14 - 1)) {
		sub_10020440(arg_0, arg_4, arg_8, arg_C, arg_10);
		return;
	}
	if (!(arg_14 - 2)) {
		sub_10021480(arg_0, arg_4, arg_8, arg_C, arg_10);
		return;
	}
}
//k
void sub_100151D0(T_size_24 * arg_0, int*** arg_4, unsigned char*** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C, int arg_30, int arg_34, int arg_38, int arg_3C) {
	switch (arg_28)
	{
	case 0:
		sub_10026310(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, arg_1C, arg_20, arg_24, arg_2C, arg_30, arg_34, arg_38, arg_3C);
		break;
	case 1:
		sub_10023990(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, arg_1C, arg_20, arg_24, arg_2C, arg_30, arg_34, arg_38, arg_3C);
		break;
	case 2:
		sub_10022C50(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, arg_1C, arg_20, arg_24, arg_2C, arg_30, arg_34, arg_38, arg_3C);
		break;
	case 3:
		sub_10021F60(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, arg_1C, arg_20, arg_24, arg_2C, arg_30, arg_34, arg_38, arg_3C);
		break;
	default:
		break;
	}
}
//k
void sub_10015350(T_size_178 * arg_0) {
	sub_10015390(arg_0);
	while (sub_10015560(arg_0));
	sub_10015940(arg_0);
}
//k
int sub_10015390(T_size_178 * arg_0) {
	T_size_178 * pesi = arg_0;
	pesi->dw0x11C = sub_1000DFA0(pesi, 0);
	for (int i = 1; i < pesi->dw0xF4; i++) {//edi
		sub_1000DFA0(pesi, i);
	}
	pesi->dw0x150 = 1;
	if (pesi->dw0xF4 == 3) {
		sub_100143E0(pesi->hg0xB0, pesi->dw0xCC, pesi->dw0xD0, pesi->dw_FC);
	}
	if (!pesi->dw_FC && pesi->dw0x13C) {
		sub_100144F0(pesi->hg0xB0, pesi->hg_B8[0], pesi->dw0xCC, pesi->dw0xD0, pesi->dw0xF4, -pesi->dw_130);
	}
	int(*pfeax)(T_size_24*, T_size_178 *, int, int);
	int(*pfecx)(T_size_24*, T_size_178 *, int, int);
	if (pesi->dw0x114 <= 0 && pesi->dw0x118 <= 0) {
		if (!(pesi->dw0x114 - 1)) {
			pfeax = sub_10027600;
			pfecx = sub_100272F0;
		}
		else if (!(pesi->dw0x114 - 2)) {
			pfeax = sub_10027C30;
			pfecx = sub_10027930;
		}
		else {
			pfeax = (int(*)(T_size_24*, T_size_178 *, int, int))arg_0;
			pfecx = (int(*)(T_size_24*, T_size_178 *, int, int))arg_0;
		}

	}
	else {
		//loc_10015486
		if (!(pesi->dw0x114 - 1)) {
			pfeax = sub_10027770;
			pfecx = sub_10027460;
		}
		else if (!(pesi->dw0x114 - 2))
		{
			pfeax = sub_10027DA0;
			pfecx = sub_10027AA0;
		}
		else
		{
			pfeax = (int(*)(T_size_24*, T_size_178 *, int, int))arg_0;//?
			pfecx = (int(*)(T_size_24*, T_size_178 *, int, int))arg_0;
		}

	}
	//loc_100154B2
	if (!pesi->dw0x108)
		sub_10026670(&pesi->t_6C, pesi->dw0xCC, pesi->dw0xD0, pesi->dw_FC, pfeax, pfecx, pesi->dw0x13C);
	else if (!(pesi->dw0x108 - 1))
		sub_10026670(&pesi->t_6C, pesi->dw0xCC, pesi->dw0xD0, pesi->dw_FC, pfeax, pfecx, pesi->dw0x13C);
	else if (!(pesi->dw0x108 - 2))
		sub_10026C90(&pesi->t_6C, pesi->dw0xCC, pesi->dw0xD0, pesi->dw_FC, pfeax, pfecx, pesi->dw0x13C);
	pesi->dw0x100 = pesi->dw_FC;
	return 1;
}
//k
unsigned char sub_10015560(T_size_178 * arg_0) {
	T_size_178 * pesi = arg_0;
	int var_4 = 0;
	pesi->dw0x100--;
	if (pesi->dw0x100 < 0)
		return 0;
	pesi->dw0x150 = 0;
	int** pebp;
	if (!pesi->dw0x100 && pesi->dw0x120 > 0) {
		if (pesi->dw0x13C)
			pebp = (int**)sub_1000E5C0(pesi->dw0xCC / 2 + 1, pesi->dw0xD0 / 2 + 1);
		else
			pebp = (int**)sub_1000E5C0(pesi->dw0xCC, pesi->dw0xD0);
	}
	else
		pebp = (int**)arg_0;//?
	sub_10011870(pesi->dw0xCC, pesi->dw0xD0, pesi->dw0x100, 2, &pesi->dw0x20, &pesi->dw0x28, &pesi->dw0x24, &pesi->dw0x2C);
	pesi->dw0x18 = pesi->dw0x100;
	for (int i = 0; i < pesi->dw0xF4; i++) {//edi
		if (!pesi->dw0x100 && pesi->dw0x120 > 0)
		{
			sub_10019E80(pesi->hg0xB0[i], pebp, pesi->dw0xCC / 2 + 1, pesi->dw0xD0 / 2 + 1, pesi->dw0x120);
			int **temp = pesi->hg0xB0[i];
			pesi->hg0xB0[i] = pebp;
			pebp = temp;
		}
		if (pesi->dw0x13C && !pesi->dw0x100) {
			if (pesi->dw0xF4 == 1)
				sub_10019B00(pesi->hg0xB0[i], pesi->hg_B8[i], pesi->dw0xCC, pesi->dw0xD0);
			else
				sub_10019760(pesi->hg0xB0[i], pesi->hg_B8[i], pesi->dw0xCC, pesi->dw0xD0);
		}
		else
			sub_100194C0(pesi->hg0xB0[i], pesi->dw0xCC, pesi->dw0xD0, pesi->dw0x100);
		//loc_100156E7
	}
	//loc_100156F9
	if (!pesi->dw0x100 && pesi->dw0x120 > 0)
		sub_1000E600(pebp);
	if (pesi->dw0x13C && !pesi->dw0x100)
		pesi->dw0x150 = 2;
	else
		pesi->dw0x150 = 1;
	if ((unsigned char)pesi->pt0xA8->dw0x14 & 1)
		return 1;
	if (!sub_1000AB10(pesi->pt0xA8, 1))
	{
		if (!sub_1000AB10(pesi->pt0xA8, 1))
			return 1;
		var_4 = 1;
	}
	//loc_10015778
	for (int i = 0; i < pesi->dw0xF4; i++) {//ebp
		if ((!(pesi->dw0x13C == 0 || pesi->dw0x100) || var_4) && i)
			return 1;
		if (!(pesi->dwdw_104 - 1))
			sub_10027230(pesi->pt0xA8, pesi, pesi->hg0xB0, pesi->hg_B8, pesi->dw0xCC, pesi->dw0xD0, i, pesi->dw0xF4, pesi->dw0x10C, pesi->dw0x118, pesi->dw0x114, pesi->dw0x124);
		else if (!(pesi->dwdw_104 - 2))
			sub_10017FA0(pesi->pt0xA8, pesi, pesi->hg0xB0, pesi->hg_B8, pesi->dw0xCC, pesi->dw0xD0, i, pesi->dw0xF4, pesi->dw0x10C, pesi->dw0x118, pesi->dw0x114, pesi->dw0x124);
		for (int j = 0; j < 3; j++) {//edi
			pesi->dw0x1C = j;
			if (!pesi->dw0x108)
				sub_10026580(pesi->pt0xA8, pesi, &pesi->t_6C, pesi->dw0xCC, pesi->dw0xD0, pesi->dw0x100, j);
			else if (!(pesi->dw0x108 - 1))
				sub_100266C0(pesi->pt0xA8, pesi, &pesi->t_6C, pesi->dw0xCC, pesi->dw0xD0, pesi->dw0x100, j);
			else if (!(pesi->dw0x108 - 2))
				sub_10026CB0(pesi->pt0xA8, pesi, &pesi->t_6C, pesi->dw0xCC, pesi->dw0xD0, pesi->dw0x100, j);

			if ((unsigned char)pesi->pt0xA8->dw0x14 & 1) {
				pesi->dw0x154 = 10;
				break;
			}
		}
		if (!(pesi->dwdw_104 - 1)) {
			sub_100272D0(pesi);
		}
		else if (!(pesi->dwdw_104 - 2)) {
			;
		}
	}
	return 1;
}
void sub_10015A80(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24) {
	T_size_2C * pedi = arg_8;
	int var_18 = sub_10028360(&pedi->t_08, arg_0);
	unsigned char** var_4 = (unsigned char**)pedi->hg_1C;//ebp
	unsigned char** var_10 = (unsigned char**)pedi->hg_20;
	unsigned char** pedx = (unsigned char**)arg_C;
	int var_14 = 1;
	for (int i = arg_10; i < arg_14; i++) {// arg_10 esi
		for (int j = arg_18; j < arg_1C; j++) {//var_C ebx
			if (pedx[i][j] != 4) {
				if (!var_18) {
					var_18 = sub_10028360(&pedi->t_08, arg_0);
					if (var_14 == 2) {
						var_14 = 1;
						if (arg_20 > 0) {
							int iecx = 2;
							int iesi = i * 2;
							int iedx = j * 2;
							int iedi = 2;
							switch (var_4[arg_24][arg_20 - 1]) {
							case 0:
								iesi--;
								if (i == (arg_14 - 1)) {
									iecx = 1;
								}
								break;
							case 1:
								iesi--;
								break;
							case 2:
								iesi -= 2;
								break;
							case 3:
								iesi -= 2;
								if (i == (arg_14 - 1)) {
									iecx = 3;
								}
								break;
							case 4:
								if (i == (arg_14 - 1)) {
									iecx = 0;
								}
								break;
							case 5:
								if (i == (arg_14 - 1))
									iecx = 1;
								break;
							}
							switch (var_10[arg_24][arg_20 - 1]) {
							case 0:
								iedx--;
								if (j == (arg_1C - 1)) {
									iedi = 1;
								}
								break;
							case 1:
								iedx--;
								break;
							case 2:
								iedx -= 2;
								break;
							case 3:
								iedx -= 2;
								if (j == (arg_1C - 1)) {
									iedi = 3;
								}
								break;
							case 4:
								if (j == (arg_1C - 1)) {
									iedi = 0;
								}
								break;
							case 5:
								if (j == (arg_1C - 1))
									iedi = 1;
								break;
							}
							unsigned char** pecx = (unsigned char**)arg_C;
							for (int k = iecx + iesi; k > iesi; k--) {//ebx
								for (unsigned char* l = &pecx[iecx + iesi - k + iesi][iedx]; l <= &pecx[iecx + iesi - k + iesi][iedi + iedx - 1]; l++) {//eax
									*l = 4;
								}
							}
						}
						var_18--;
					}
					else {
						var_14 = 2;
						pedi->hg0x24(arg_0, arg_4, i, j);
						var_18--;
					}
				}
				else {
					if (var_14 != 2) {
						if (arg_20 > 0) {
							int iecx = 2;
							int iesi = i * 2;
							int iedx = j * 2;
							int iedi = 2;
							switch (var_4[arg_24][arg_20 - 1]) {
							case 0:
								iesi--;
								if (i == (arg_14 - 1)) {
									iecx = 1;
								}
								break;
							case 1:
								iesi--;
								break;
							case 2:
								iesi -= 2;
								break;
							case 3:
								iesi -= 2;
								if (i == (arg_14 - 1)) {
									iecx = 3;
								}
								break;
							case 4:
								if (i == (arg_14 - 1)) {
									iecx = 0;
								}
								break;
							case 5:
								if (i == (arg_14 - 1))
									iecx = 1;
								break;
							}
							switch (var_10[arg_24][arg_20 - 1]) {
							case 0:
								iedx--;
								if (j == (arg_1C - 1)) {
									iedi = 1;
								}
								break;
							case 1:
								iedx--;
								break;
							case 2:
								iedx -= 2;
								break;
							case 3:
								iedx -= 2;
								if (j == (arg_1C - 1)) {
									iedi = 3;
								}
								break;
							case 4:
								if (j == (arg_1C - 1)) {
									iedi = 0;
								}
								break;
							case 5:
								if (j == (arg_1C - 1))
									iedi = 1;
								break;
							}
							unsigned char** pecx = (unsigned char**)arg_C;
							for (int k = iecx + iesi; k > iesi; k--) {//ebx
								for (unsigned char* l = &pecx[iecx + iesi - k + iesi][iedx]; l <= &pecx[iecx + iesi - k + iesi][iedi + iedx - 1]; l++) {//eax
									*l = 4;
								}
							}
						}
						var_18--;
					}
					else {
						pedi->hg0x24(arg_0, arg_4, i, j);
						var_18--;
					}
				}
			}
			else if (arg_20 > 0) {
				int iecx = 2;
				int iesi = i * 2;
				int iedx = j * 2;
				int iedi = 2;
				switch (var_4[arg_24][arg_20 - 1]) {
				case 0:
					iesi--;
					if (i == (arg_14 - 1)) {
						iecx = 1;
					}
					break;
				case 1:
					iesi--;
					break;
				case 2:
					iesi -= 2;
					break;
				case 3:
					iesi -= 2;
					if (i == (arg_14 - 1)) {
						iecx = 3;
					}
					break;
				case 4:
					if (i == (arg_14 - 1)) {
						iecx = 0;
					}
					break;
				case 5:
					if (i == (arg_14 - 1))
						iecx = 1;
					break;
				}
				switch (var_10[arg_24][arg_20 - 1]) {
				case 0:
					iedx--;
					if (j == (arg_1C - 1)) {
						iedi = 1;
					}
					break;
				case 1:
					iedx--;
					break;
				case 2:
					iedx -= 2;
					break;
				case 3:
					iedx -= 2;
					if (j == (arg_1C - 1)) {
						iedi = 3;
					}
					break;
				case 4:
					if (j == (arg_1C - 1)) {
						iedi = 0;
					}
					break;
				case 5:
					if (j == (arg_1C - 1))
						iedi = 1;
					break;
				}
				unsigned char** pecx = (unsigned char**)arg_C;
				for (int k = iecx + iesi; k > iesi; k--) {//ebx
					for (unsigned char* l = &pecx[iecx + iesi - k + iesi][iedx]; l <= &pecx[iecx + iesi - k + iesi][iedi + iedx - 1]; l++) {//eax
						*l = 4;
					}
				}
			}
		}
	}
	if (arg_20 > 0 && var_18)
		arg_0->dw0x14 = 1;
}
//k
void sub_10015940(T_size_178 *arg_0) {
	T_size_178 * pesi = arg_0;
	if (!(pesi->dw0x108)) {
		sub_100266A0(&pesi->t_6C);
	}
	else if (!(pesi->dw0x108 - 1)) {
		sub_100266A0(&pesi->t_6C);
	}
	else if (!(pesi->dw0x108 - 2)) {
		;
	}

}
//k
void sub_10015980(T_size_2C * arg_0, int arg_4, int arg_8, int arg_C, int(*arg_10)(T_size_24*, T_size_178*, int, int), int(*arg_14)(T_size_24*, T_size_178*, int, int), int arg_18) {
	T_size_2C * pesi = (T_size_2C *)arg_0;
	sub_10011910(&pesi->hg_1C, &pesi->hg_20, arg_4, arg_8, arg_C);
	sub_100280B0(&pesi->t_08, 0x100, 0);
	pesi->hg0x24 = arg_10;
	pesi->hg0x28 = arg_14;
	pesi->dw0x00 = arg_18;
}
//k
void sub_100159D0(T_size_2C * arg_0) {
	T_size_2C * pesi = (T_size_2C *)arg_0;
	sub_10028140(&pesi->t_08);
	sub_10011A10(pesi->hg_1C, pesi->hg_20);
}
//k
void sub_10015A00(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C* arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C) {
	T_size_2C * pedi = (T_size_2C *)arg_8;
	if (!arg_18 && pedi->dw0x00) {
		pedi->hg0x24 = pedi->hg0x28;
	}
	int iarg_14, iarg_1C, iarg_8, iarg_18;
	sub_10011870(arg_10, arg_14, arg_18, arg_1C, &iarg_14, &iarg_1C, &iarg_8, &iarg_18);
	sub_10015A80(arg_0, arg_4, pedi, arg_C, iarg_14, iarg_1C, iarg_8, iarg_18, arg_18, arg_1C);
}
//k
void sub_10015FF0(T_size_2C * arg_0, int arg_4, int arg_8, int arg_C, int(*arg_10)(T_size_24*, T_size_178*, int, int), int(*arg_14)(T_size_24*, T_size_178*, int, int), int arg_18) {
	T_size_2C * pesi = (T_size_2C*)arg_0;
	sub_10011910(&pesi->hg_1C, &pesi->hg_20, arg_4, arg_8, arg_C);
	pesi->hg0x24 = arg_10;
	pesi->hg0x28 = arg_14;
	pesi->dw0x00 = arg_18;
}
//k
void sub_10016050(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C) {
	T_size_2C * pedi = (T_size_2C *)arg_8;
	if (!arg_18 && pedi->dw0x00) {
		pedi->hg0x24 = pedi->hg0x28;
	}
	int iarg_14, iarg_1C, iarg_8, iarg_18;
	sub_10011870(arg_10, arg_14, arg_18, arg_1C, &iarg_14, &iarg_1C, &iarg_8, &iarg_18);
	sub_100160D0(arg_0, arg_4, pedi, arg_C, iarg_14, iarg_1C, iarg_8, iarg_18, arg_18, arg_1C);
}
//k
void sub_10016030(T_size_2C * arg_0) {
	T_size_2C * peax = (T_size_2C *)arg_0;
	sub_10011A10(peax->hg_1C, peax->hg_20);
}
void sub_100160D0(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24) {
	T_size_2C * peax = arg_8;
	unsigned char** var_C = (unsigned char**)peax->hg_1C;
	unsigned char** var_8 = (unsigned char**)peax->hg_20;
	unsigned char** pecx = (unsigned char**)arg_C;
	for (int i = arg_10; i < arg_14; i++) {// arg_10 esi
		for (int j = arg_18; j < arg_1C; j++) {//var_14 ebx
			if (pecx[i][j] != 4) {
				if (arg_8->hg0x24(arg_0, arg_4, i, j)) {
					continue;
				}
			}
			if (arg_20 > 0) {
				int iecx = 2;
				int iesi = i * 2;
				int iedx = j * 2;
				int iedi = 2;
				switch (var_C[arg_24][arg_20 - 1]) {
				case 0:
					iesi--;
					if (i == (arg_14 - 1)) {
						iecx = 1;
					}
					break;
				case 1:
					iesi--;
					break;
				case 2:
					iesi -= 2;
					break;
				case 3:
					iesi -= 2;
					if (i == (arg_14 - 1)) {
						iecx = 3;
					}
					break;
				case 4:
					if (i == (arg_14 - 1)) {
						iecx = 0;
					}
					break;
				case 5:
					if (i == (arg_14 - 1))
						iecx = 1;
					break;
				}
				//loc_100161E1
				switch (var_8[arg_24][arg_20 - 1]) {
				case 0:
					iedx--;
					if (j == (arg_1C - 1)) {
						iedi = 1;
					}
					break;
				case 1:
					iedx--;
					break;
				case 2:
					iedx -= 2;
					break;
				case 3:
					iedx -= 2;
					if (j == (arg_1C - 1)) {
						iedi = 3;
					}
					break;
				case 4:
					if (j == (arg_1C - 1)) {
						iedi = 0;
					}
					break;
				case 5:
					if (j == (arg_1C - 1))
						iedi = 1;
					break;
				}
				unsigned char** pecx = (unsigned char**)arg_C;
				for (int k = iecx + iesi; k > iesi; k--) {//ebx
					for (unsigned char* l = &pecx[iecx + iesi - k + iesi][iedx]; l <= &pecx[iecx + iesi - k + iesi][iedi + iedx - 1]; l++) {//eax
						*l = 4;
					}
				}
			}

		}
	}
}
void sub_10016300(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C* arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C) {
	T_size_2C * pesi = (T_size_2C *)arg_8;
	unsigned char** var_8 = (unsigned char**)pesi->hg_1C;
	unsigned char** var_4 = (unsigned char**)pesi->hg_20;
	unsigned char** pedi = (unsigned char**)arg_C;
	if (!arg_18 && pesi->dw0x00) {
		pesi->hg0x24 = pesi->hg0x28;
	}
	int var_10, iarg_10, var_C, iarg_8;
	sub_10011870(arg_10, arg_14, arg_18, arg_1C, &var_10, &iarg_10, &var_C, &iarg_8);
	sub_10016560(arg_0, arg_4, pesi, arg_C, 0, var_10, iarg_10, var_C, iarg_8, arg_18, arg_1C);
	for (int i = var_10; i < iarg_10; i++) {// arg_14 eax
		for (int j = var_C; j < iarg_8; j++) {//arg_0 ebx
			if (pedi[i][j] != 2) {
				if (arg_18 > 0) {
					int iecx = 2;
					int iesi = i * 2;
					int iedx = j * 2;
					int iedi = 2;
					switch (var_8[arg_1C][arg_18 - 1]) {
					case 0:
						iesi--;
						if (i == (arg_10 - 1)) {
							iecx = 1;
						}
						break;
					case 1:
						iesi--;
						break;
					case 2:
						iesi -= 2;
						break;
					case 3:
						iesi -= 2;
						if (i == (arg_10 - 1)) {
							iecx = 3;
						}
						break;
					case 4:
						if (i == (arg_10 - 1)) {
							iecx = 0;
						}
						break;
					case 5:
						if (i == (arg_10 - 1))
							iecx = 1;
						break;
					}
					switch (var_4[arg_1C][arg_18 - 1]) {
					case 0:
						iedx--;
						if (j == (iarg_8 - 1)) {
							iedi = 1;
						}
						break;
					case 1:
						iedx--;
						break;
					case 2:
						iedx -= 2;
						break;
					case 3:
						iedx -= 2;
						if (j == (iarg_8 - 1)) {
							iedi = 3;
						}
						break;
					case 4:
						if (j == (iarg_8 - 1)) {
							iedi = 0;
						}
						break;
					case 5:
						if (j == (iarg_8 - 1))
							iedi = 1;
						break;
					}
					unsigned char** pecx = (unsigned char**)arg_C;
					for (int k = iecx + iesi; k > iesi; k--) {//ebx
						for (unsigned char* l = &pecx[iecx + iesi - k + iesi][iedx]; l <= &pecx[iecx + iesi - k + iesi][iedi + iedx - 1]; l++) {//eax
							*l = 4;
						}
					}
				}
			}
		}
	}
}
void sub_10016560(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C* arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28) {
	int iecx = arg_18 - arg_14;
	int iedx = arg_20 - arg_1C;//ieax
	int iesi = iecx / 2 + arg_14;
	int iedi = iedx / 2 + arg_1C;
	int ieax_0 = arg_10;
	unsigned char** pedx = arg_C;
	for (; iedx > 2; iedx = arg_20 - arg_1C, iecx = arg_18 - arg_14, iesi = iecx / 2 + arg_14, iedi = iedx / 2 + arg_1C) {
		if (iecx > 2) {
			switch (ieax_0) {
			case 0:
				sub_10016560(arg_0, arg_4, arg_8, arg_C, 3, arg_14, iesi, iedi, arg_20, arg_24, arg_28);
				sub_10016560(arg_0, arg_4, arg_8, arg_C, 0, iesi, arg_18, iedi, arg_20, arg_24, arg_28);
				sub_10016560(arg_0, arg_4, arg_8, arg_C, 0, iesi, arg_18, arg_1C, iedi, arg_24, arg_28);
				arg_20 = iedi;
				arg_18 = iesi;
				ieax_0 = 2;
				break;
			case 1:
				sub_10016560(arg_0, arg_4, arg_8, arg_C, 2, iesi, arg_18, arg_1C, iedi, arg_24, arg_28);
				sub_10016560(arg_0, arg_4, arg_8, arg_C, 1, arg_14, iesi, arg_1C, iedi, arg_24, arg_28);
				sub_10016560(arg_0, arg_4, arg_8, arg_C, 1, arg_14, iesi, iedi, arg_20, arg_24, arg_28);
				arg_1C = iedi;
				arg_14 = iesi;
				ieax_0 = 3;
				break;
			case 2:
				sub_10016560(arg_0, arg_4, arg_8, arg_C, 1, iesi, arg_18, arg_1C, iedi, arg_24, arg_28);
				sub_10016560(arg_0, arg_4, arg_8, arg_C, 2, iesi, arg_18, iedi, arg_20, arg_24, arg_28);
				sub_10016560(arg_0, arg_4, arg_8, arg_C, 2, arg_14, iesi, iedi, arg_20, arg_24, arg_28);
				arg_20 = iedi;
				arg_18 = iesi;
				ieax_0 = 0;
				break;
			case 3:
				sub_10016560(arg_0, arg_4, arg_8, arg_C, 0, arg_14, iesi, iedi, arg_20, arg_24, arg_28);
				sub_10016560(arg_0, arg_4, arg_8, arg_C, 3, arg_14, iesi, arg_1C, iedi, arg_24, arg_28);
				sub_10016560(arg_0, arg_4, arg_8, arg_C, 3, iesi, arg_18, arg_1C, iedi, arg_24, arg_28);
				arg_1C = iedi;
				arg_14 = iesi;
				ieax_0 = 2;
				break;
			}
		}
		else break;
	}
	//loc_10016833
	int ieax, i;
	switch (arg_10) {
	case 0:
		ieax = 1;
		for (i = arg_20 - 1; i >= arg_1C; i--) {//iesi
			if (ieax == 1) {
				for (int j = arg_14; j < arg_18; j++) {//iedi
					if (pedx[j][i] == 1) {
						if (arg_8->hg0x24(arg_0, arg_4, j, i)) {
							pedx[j][i] = 2;
						}
					}
				}
				ieax = 0;
			}
			else {
				for (int j = arg_18 - 1; j >= arg_14; j--) {//edi
					if (pedx[j][i] == 1) {
						if (arg_8->hg0x24(arg_0, arg_4, j, i)) {
							pedx[j][i] = 2;
						}
					}
				}
				ieax = 1;
			}
		}
		return;
	case 1:
		ieax = 0;
		for (i = arg_1C; i < arg_20; i++) {//iesi
			if (ieax == 1) {
				for (int j = arg_14; j < arg_18; j++) {//iedi
					if (pedx[j][i] == 1) {
						if (arg_8->hg0x24(arg_0, arg_4, j, i)) {
							pedx[j][i] = 2;
						}
					}
				}
				ieax = 0;
			}
			else {
				for (int j = arg_18 - 1; j >= arg_14; j--) {//edi
					if (pedx[j][i] == 1) {
						if (arg_8->hg0x24(arg_0, arg_4, j, i)) {
							pedx[j][i] = 2;
						}
					}
				}
				ieax = 1;
			}
		}
		return;
	case 2:
		ieax = 0;
		for (i = arg_18 - 1; i >= arg_14; i--) {//iedi
			if (ieax == 1) {
				for (int j = arg_1C; j < arg_20; j++) {//iesi
					if (pedx[i][j] == 1) {
						if (arg_8->hg0x24(arg_0, arg_4, i, j)) {
							pedx[i][j] = 2;
						}
					}
				}
				ieax = 0;
			}
			else {
				for (int j = arg_20 - 1; j >= arg_1C; j--) {//edi
					if (pedx[i][j] == 1) {
						if (arg_8->hg0x24(arg_0, arg_4, i, j)) {
							pedx[i][j] = 2;
						}
					}
				}
				ieax = 1;
			}
		}
		return;
	case 3:
		ieax = 0;
		for (i = arg_14; i < arg_18; i++) {//iebx
			if (ieax == 1) {
				for (int j = arg_1C; j < arg_20; j++) {//iesi
					if (pedx[i][j] == 1) {
						if (arg_8->hg0x24(arg_0, arg_4, i, j)) {
							pedx[i][j] = 2;
						}
					}
				}
				ieax = 0;
			}
			else {
				for (int j = arg_20 - 1; j >= arg_1C; j--) {//edi
					if (pedx[i][j] == 1) {
						if (arg_8->hg0x24(arg_0, arg_4, i, j)) {
							pedx[i][j] = 2;
						}
					}
				}
				ieax = 1;
			}
		}
		return;
	}

}
//k
void sub_10016B70(T_size_2C * arg_0, int arg_4, int arg_8, int arg_C, int(*arg_10)(T_size_24*, T_size_178*, int, int), int(*arg_14)(T_size_24*, T_size_178*, int, int), int arg_18) {
	T_size_2C * pesi = (T_size_2C *)arg_0;
	sub_10011910(&pesi->hg_1C, &pesi->hg_20, arg_4, arg_8, arg_C);
	sub_100280B0(&pesi->t_08, 0x40, 0);
	pesi->hg0x24 = arg_10;
	pesi->hg0x28 = arg_14;
	pesi->dw0x00 = arg_18;
}
//k
void sub_10016BC0(T_size_2C * arg_0) {
	T_size_2C * pesi = (T_size_2C *)arg_0;
	sub_10011A10(pesi->hg_1C, pesi->hg_20);
	sub_10028140(&pesi->t_08);
}
//k
int sub_10016BF0(T_size_24 * arg_0, T_size_2C * arg_4) {
	return sub_100282C0(&arg_4->t_08, arg_0, 4);
}
void sub_10016C10(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C) {
	T_size_2C * pesi = (T_size_2C *)arg_8;
	unsigned char** var_8 = (unsigned char**)pesi->hg_1C;
	unsigned char** var_4 = (unsigned char**)pesi->hg_20;
	unsigned char** pedi = (unsigned char**)arg_C;
	if (!arg_18 && pesi->dw0x00) {
		pesi->hg0x24 = pesi->hg0x28;
	}
	int iarg_10, iarg_14, var_10, iarg_8;
	sub_10011870(arg_10, arg_14, arg_18, arg_1C, &iarg_10, &iarg_14, &var_10, &iarg_8);
	if (sub_1000AB10(arg_0, 1)) {
		if (sub_1000AB10(arg_0, 1))
			sub_10016EA0(arg_0, arg_4, pesi, arg_C, arg_18, arg_1C, iarg_10, iarg_14, var_10, iarg_8);
		else
			sub_10016FF0(arg_0, arg_4, pesi, arg_C, arg_18, arg_1C, iarg_10, iarg_14, var_10, iarg_8);
	}
	for (int i = iarg_10; i < iarg_14; i++) {// arg_4 eax
		for (int j = var_10; j < iarg_8; j++) {//var_C ebx
			if (pedi[i][j] != 5) {
				if (arg_18 > 0) {
					int iecx = 2;
					int iesi = i * 2;
					int iedx = j * 2;
					int iedi = 2;
					switch (var_8[arg_1C][arg_18 - 1]) {
					case 0:
						iesi--;
						if (i == (arg_14 - 1)) {
							iecx = 1;
						}
						break;
					case 1:
						iesi--;
						break;
					case 2:
						iesi -= 2;
						break;
					case 3:
						iesi -= 2;
						if (i == (arg_14 - 1)) {
							iecx = 3;
						}
						break;
					case 4:
						if (i == (arg_14 - 1)) {
							iecx = 0;
						}
						break;
					case 5:
						if (i == (arg_14 - 1))
							iecx = 1;
						break;
					}
					switch (var_4[arg_1C][arg_18 - 1]) {
					case 0:
						iedx--;
						if (j == (iarg_8 - 1)) {
							iedi = 1;
						}
						break;
					case 1:
						iedx--;
						break;
					case 2:
						iedx -= 2;
						break;
					case 3:
						iedx -= 2;
						if (j == (iarg_8 - 1)) {
							iedi = 3;
						}
						break;
					case 4:
						if (j == (iarg_8 - 1)) {
							iedi = 0;
						}
						break;
					case 5:
						if (j == (iarg_8 - 1))
							iedi = 1;
						break;
					}
					unsigned char** pecx = arg_C;
					for (int k = iecx + iesi; k > iesi; k--) {//ebx
						for (unsigned char* l = &pecx[iecx + iesi - k + iesi][iedx]; l <= &pecx[iecx + iesi - k + iesi][iedi + iedx - 1]; l++) {//eax
							*l = 4;
						}
					}
				}
			}
		}
	}
}
//k
void sub_10016EA0(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C* arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24) {
	unsigned char** pecx = arg_C;
	for (int i = arg_18; i < arg_1C; i += 2) {//var_10 iedi var_14
		for (int j = arg_20; j < arg_24; j += 2) {//var_1C iesi
			int var_18 = i + 2;//ebx
			if (var_18 > arg_1C)
				var_18 = arg_1C;
			int iarg_18 = j + 2; //ebp
			if (iarg_18 > arg_24)
				iarg_18 = arg_24;
			int var_8;
			if (!sub_10017870(arg_C, i, var_18, j, iarg_18)) {
				var_8 = sub_10016BF0(arg_0, arg_8);
				int iebp = 1 << (unsigned char)((iarg_18 - j) * (var_18 - i));
				for (int k = i; k < var_18; k++) {//iedi
					for (int l = j; l < iarg_18; l++) {//iesi
						iebp /= 2;
						if (var_8 != iebp) {
							pecx[k][l] = 2;
							arg_8->hg0x24(arg_0, arg_4, k, l);
						}
					}
				}
			}

		}
	}
}
//k
void sub_10016FF0(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C* arg_8, unsigned char** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24) {
	int ieax = arg_1C - arg_18;
	int iedi = arg_24 - arg_20;
	unsigned char barg_18;
	for (; iedi > 2; ieax = arg_1C - arg_18, iedi = arg_24 - arg_20) {//arg_18 ebx
		if (ieax > 2) {
			barg_18 = 0;
			int iesi = ieax / 2 + arg_18, iedi1 = iedi / 2 + arg_20;
			unsigned char iarg_0 = sub_10017870(arg_C, arg_18, iesi, arg_20, iedi1);
			if (iarg_0)
				barg_18 = 1;
			unsigned char var_C = sub_10017870(arg_C, arg_18, iesi, iedi1, arg_24);
			if (var_C) {
				barg_18++;
			}
			unsigned char var_8 = sub_10017870(arg_C, iesi, arg_1C, arg_20, iedi1);
			if (var_8) {
				barg_18++;
			}
			unsigned char var_4 = sub_10017870(arg_C, iesi, arg_1C, iedi1, arg_24);
			if (var_4) {
				barg_18++;
			}
			if (barg_18 > 0) {
				if (!iarg_0) {
					if (barg_18 == 3 || sub_1000AB10(arg_0, 1)) {
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, iesi, arg_20, iedi1);
					}
					else
						barg_18++;
				}
				//loc_1001711B
				if (!var_C) {
					if (barg_18 == 3 || sub_1000AB10(arg_0, 1)) {
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, iesi, iedi1, arg_24);
					}
					else
						barg_18++;
				}
				if (!var_8) {
					if (barg_18 == 3)
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, iesi, arg_1C, arg_20, iedi1);
					else if (sub_1000AB10(arg_0, 1))
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, iesi, arg_1C, arg_20, iedi1);
					else
						barg_18++;
				}
				if (!var_4) {
					if (barg_18 == 3 || sub_1000AB10(arg_0, 1))
					{
						arg_20 = iedi1;//case 3
						arg_18 = iesi;
					}
					else return;
				}
				else return;
			}
			else {
				if (sub_1000AB10(arg_0, 1)) {
					unsigned char iarg_18 = sub_1000AB10(arg_0, 1) * 2;
					unsigned char al = iarg_18 | sub_1000AB10(arg_0, 1);
					switch (al) {
					case 0:
						arg_24 = iedi1;
						arg_1C = iesi;
						break;
					case 1:
						arg_20 = iedi1;
						arg_1C = iesi;
						break;
					case 2:
						arg_24 = iedi1;
						arg_18 = iesi;
						break;
					case 3:
						arg_20 = iedi1;//case 3
						arg_18 = iesi;
						break;
					default:
						return;
					}
				}
				else if (sub_1000AB10(arg_0, 1)) {
					if (sub_1000AB10(arg_0, 1)) {
						unsigned char iarg_18 = sub_1000AB10(arg_0, 1) * 2;
						unsigned char al = iarg_18 | sub_1000AB10(arg_0, 1);
						switch (al) {
						case 0:
							sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, iesi, arg_20, iedi1);
							arg_20 = iedi1;
							arg_1C = iesi;
							break;
						case 1:
							sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, iesi, arg_1C, arg_20, iedi1);
							arg_24 = iedi1;
							arg_18 = iesi;
							break;
						case 2:
							sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, iesi, arg_20, iedi1);
							arg_24 = iedi1;
							arg_18 = iesi;
							break;
						case 3:
							sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, iesi, iedi1, arg_24);
							arg_20 = iedi1;
							arg_18 = iesi;
							break;
						default:
							return;
						}
					}
					else if (sub_1000AB10(arg_0, 1)) {
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, iesi, iedi1, arg_24);
						arg_24 = iedi1;
						arg_18 = iesi;
					}
					else {
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, iesi, arg_20, iedi1);
						arg_20 = iedi1;
						arg_18 = iesi;
					}
				}
				else if (sub_1000AB10(arg_0, 1)) {
					unsigned char iarg_18 = sub_1000AB10(arg_0, 1) * 2;
					unsigned char al = iarg_18 | sub_1000AB10(arg_0, 1);
					switch (al) {
					case 0:
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, iesi, arg_20, iedi1);
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, iesi, iedi1, arg_24);
						arg_20 = iedi1;//case 3
						arg_18 = iesi;
						break;
					case 1:
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, iesi, iedi1, arg_24);
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, iesi, arg_1C, iedi1, arg_24);
						arg_24 = iedi1;
						arg_18 = iesi;
						break;
					case 2:
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, iesi, arg_1C, iedi1, arg_24);
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, iesi, arg_1C, arg_20, iedi1);
						arg_24 = iedi1;
						arg_1C = iesi;
						break;
					case 3:
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, iesi, arg_1C, arg_20, iedi1);
						sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, iesi, arg_20, iedi1);
						arg_20 = iedi1;
						arg_1C = iesi;
						break;
					default:
						return;
					}
				}
				else {
					sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, iesi, arg_20, iedi1);
					sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, iesi, iedi1, arg_24);
					sub_10016FF0(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, iesi, arg_1C, arg_20, iedi1);
					arg_20 = iedi1;//case 3
					arg_18 = iesi;
				}
			}
		}
		else
			break;
	}
	//loc_10017594
	int iarg_14 = ieax * iedi - 1;
	unsigned char ** pecx = arg_C;
	int cnt = 0, i; //iesi; arg_18
	for (i = arg_1C - arg_18; i > 0; i--) {//arg_10 ecx arg_18 = 0;
		for (int j = arg_20; j < arg_24; j++) {//eax
			if (pecx[arg_1C - arg_18 - i + arg_18][j] == 4) {
				cnt++;
			}
		}
	}
	//loc_100175EB
	if (!cnt && arg_14 == 3) {
		if (sub_1000AB10(arg_0, 1)) {
			if (sub_1000AB10(arg_0, 1)) {
				barg_18 = 2 - (sub_1000AB10(arg_0, 1) != 0);
			}
			else
				barg_18 = sub_1000AB10(arg_0, 1) != 0 ? 4 : 8;
		}
		else if (sub_1000AB10(arg_0, 1))
		{
			if (sub_1000AB10(arg_0, 1))
			{
				if (sub_1000AB10(arg_0, 1))
					barg_18 = sub_1000AB10(arg_0, 1) != 0 ? 9 : 3;
				else
					barg_18 = sub_1000AB10(arg_0, 1) != 0 ? 6 : 12;
			}
			else
			{
				barg_18 = sub_1000AB10(arg_0, 1) != 0 ? 5 : 10;
			}
		}
		else if (sub_1000AB10(arg_0, 1))
		{
			if (sub_1000AB10(arg_0, 1))
				barg_18 = sub_1000AB10(arg_0, 1) != 0 ? 13 : 11;
			else
				barg_18 = sub_1000AB10(arg_0, 1) != 0 ? 7 : 14;
		}
		else
		{
			barg_18 = 15;
		}
		//loc_1001770E
		unsigned char al = 0x10;
		for (int i = arg_18; i < arg_1C; i++) {//ebx
			for (int j = arg_20; j < arg_24; j++) {//iedi
				al = al / 2;
				if (barg_18 & al) {
					pecx[i][j] = 2;
					arg_8->hg0x24(arg_0, arg_4, i, j);
				}
			}
		}
		return;
	}
	//loc_10017776
	for (i = arg_1C - arg_18; i > 0; i--) {//arg_10 eax
		for (int j = arg_20; j < arg_24; j++) {//iedi
			if (pecx[arg_1C - arg_18 - i + arg_18][j] != 4) {
				if (arg_18 != arg_14 && !sub_1000AB10(arg_0, 1))
					barg_18++;
				else
					pecx[arg_1C - arg_18 - i + arg_18][j] = 2;
			}
		}
	}
	//loc_100177DD
	for (i = arg_18; i < arg_1C; i++) {//iebx
		for (int j = arg_20; j < arg_24; j++) {//iedi
			if (pecx[i][j] == 2)
			{
				arg_8->hg0x24(arg_0, arg_4, i, j);
			}
		}
	}
}
//k
unsigned char sub_10017870(unsigned char ** arg_0, int arg_4, int arg_8, int arg_C, int arg_10) {
	for (int i = arg_4; i < arg_8; i++) {//iesi
		for (int j = arg_C; j < arg_10; j++) {//ieax
			if (arg_0[i][j] != 4) return 0;
		}
	}
	return 1;
}
//k
void sub_100178D0(T_size_24 * arg_0, T_size_178 * arg_4, int *** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C) {
	T_size_178 * pecx = arg_4;
	pecx->dw_50 = sub_1000AB10(arg_0, 5);
	pecx->hg0x04 = arg_8;
	pecx->hg0x00 = *arg_8;
	if (arg_C) {
		pecx->hg0x0C = arg_C;
		pecx->hg0x08 = *arg_C;
	}
	pecx->dw0x10 = arg_10;
	pecx->dw0x14 = arg_14;
	pecx->dw0x34 = arg_1C;
	pecx->dw0x30 = arg_18;
	pecx->dw0x60 = arg_24;
	pecx->dw0x68 = arg_2C;
	pecx->dw0x54 = arg_20;
	pecx->dw0x64 = arg_28;
	pecx->dw0x58 = arg_20 / (arg_28 + 1);
	if (pecx->dw0x58 < 1)
		pecx->dw0x58 = 1;
	if (arg_28 > 0) {
		pecx->dw0x5C = arg_20 / pecx->dw0x58;
	}
	else
		pecx->dw0x5C = 0;

}
//k
int sub_10017970(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	unsigned char bebx = sub_1000AB10(arg_0, 1);
	unsigned char beax = sub_1000AB10(arg_0, pesi->dw_50);
	if (pesi->dw0x68 && beax == 1) {
		beax = pesi->dw0x54;
	}
	else {
		beax = beax * pesi->dw0x54 + pesi->dw0x54 / 2;
	}
	if (bebx) {
		beax = -beax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_1002C8F0(pesi->hg0x08, beax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = beax;
		b = beax;
		c = beax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = beax * 2;
		c = -a;
		b = -beax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = beax / 4 + beax;
		b = -a;
		c = beax * 4 - beax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_100287E0(pesi->hg0x0C[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10017AE0(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	unsigned char iebx = sub_1000AB10(arg_0, 1);
	unsigned char ieax = sub_1000AB10(arg_0, pesi->dw_50);
	if (ieax < pesi->dw0x5C) {
		if (pesi->dw0x68 && ieax == 1) {
			ieax = pesi->dw0x58;
		}
		else {
			ieax = ieax * pesi->dw0x58 + pesi->dw0x58 / 2;
		}
	}
	else {
		if (pesi->dw0x68 && (ieax - pesi->dw0x5C) == 1) {
			ieax = pesi->dw0x54;
		}
		else {
			ieax = (ieax - pesi->dw0x5C) * pesi->dw0x54 + pesi->dw0x54 / 2;
		}
	}
	if (!pesi->dw0x68)
		ieax = ieax * pesi->dw0x60 / 8 + ieax;
	if (iebx) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_1002C8F0(pesi->hg0x08, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		c = -a;
		b = -ieax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_100287E0(pesi->hg0x0C[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10017C80(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	unsigned char iebx = sub_1000AB10(arg_0, 1);
	unsigned char ieax = sub_1000AB10(arg_0, pesi->dw_50);
	if (pesi->dw0x68 && pesi->dw0x18 < 2 && ieax == 1) {
		ieax = pesi->dw0x54;
	}
	else {
		ieax = ieax * pesi->dw0x54 + pesi->dw0x54 / 2;
	}
	if (iebx) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_10030730(pesi->hg0x00, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		c = -a;
		b = -ieax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_10030730(pesi->hg0x04[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10017DF0(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	unsigned char iebx = sub_1000AB10(arg_0, 1);//arg_0
	unsigned char ieax = sub_1000AB10(arg_0, pesi->dw_50);
	if (ieax < pesi->dw0x5C) {
		if (pesi->dw0x68 && pesi->dw0x18 < 2 && ieax == 1) {
			ieax = pesi->dw0x58;
		}
		else {
			ieax = ieax * pesi->dw0x58 + pesi->dw0x58 / 2;
		}
	}
	else {
		if (pesi->dw0x68 && pesi->dw0x18 < 2 && (ieax - pesi->dw0x5C) == 1) {
			ieax = pesi->dw0x54;
		}
		else {
			ieax = (ieax - pesi->dw0x5C) * pesi->dw0x54 + pesi->dw0x54 / 2;
		}
	}
	if (pesi->dw0x18 < 2 && !pesi->dw0x68)
		ieax = ieax * pesi->dw0x60 / 8 + ieax;
	if (iebx) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_10030730(pesi->hg0x00, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		c = -a;
		b = -ieax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_10030730(pesi->hg0x04[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
void sub_10017FA0(T_size_24 * arg_0, T_size_178 * arg_4, int *** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C) {
	T_size_178 * pecx = arg_4;
	pecx->hg0x04 = arg_8;
	pecx->hg0x00 = *arg_8;
	if (arg_C) {
		pecx->hg0x0C = arg_C;
		pecx->hg0x08 = *arg_C;
	}
	pecx->dw0x10 = arg_10;
	pecx->dw0x14 = arg_14;
	pecx->dw0x34 = arg_1C;
	pecx->dw0x30 = arg_18;
	pecx->dw0x60 = arg_24;
	pecx->dw0x68 = arg_2C;
	pecx->dw0x54 = arg_20;
	pecx->dw0x64 = arg_28;
	pecx->dw0x58 = arg_20 / (arg_28 + 1);
	if (pecx->dw0x58 < 0)
		pecx->dw0x58 = 1;
	if (arg_28 > 0) {
		pecx->dw0x5C = arg_20 / pecx->dw0x58;
	}
	else
		pecx->dw0x5C = 0;

}
//k
int sub_10018030(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	unsigned char iebp = sub_1000AB10(arg_0, 1);
	int i, ieax;
	for (i = 0; sub_1000AB10(arg_0, 1); ++i);//edi
	if (pesi->dw0x68 && i == 1) {
		ieax = pesi->dw0x54;
	}
	else {
		ieax = i * pesi->dw0x54 + pesi->dw0x54 / 2;
	}
	if (iebp) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_1002C8F0(pesi->hg0x08, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		c = -a;
		b = -ieax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_100287E0(pesi->hg0x0C[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_100181B0(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	unsigned char iebp = sub_1000AB10(arg_0, 1);
	int i, ieax;
	for (i = 0; sub_1000AB10(arg_0, 1); ++i);
	if (i < pesi->dw0x5C) {
		if (pesi->dw0x68 && i == 1) {
			ieax = pesi->dw0x58;
		}
		else {
			ieax = i * pesi->dw0x58 + pesi->dw0x58 / 2;
		}
	}
	else {
		if (pesi->dw0x68 && (i - pesi->dw0x5C) == 1) {
			ieax = pesi->dw0x54;
		}
		else {
			ieax = (i - pesi->dw0x5C) * pesi->dw0x54 + pesi->dw0x54 / 2;
		}
	}
	if (!pesi->dw0x68)
		ieax = ieax * pesi->dw0x60 / 8 + ieax;
	if (iebp) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_1002C8F0(pesi->hg0x08, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		c = -a;
		b = -ieax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_100287E0(pesi->hg0x0C[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10018360(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	unsigned char iebp = sub_1000AB10(arg_0, 1);
	int i, ieax;
	for (i = 0; sub_1000AB10(arg_0, 1); ++i);
	if (pesi->dw0x68 && pesi->dw0x18 < 2 && i == 1) {
		ieax = pesi->dw0x54;
	}
	else {
		ieax = i * pesi->dw0x54 + pesi->dw0x54 / 2;
	}
	if (iebp) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_10030730(pesi->hg0x00, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		c = -a;
		b = -ieax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_10030730(pesi->hg0x04[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_100184E0(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	unsigned char iebp = sub_1000AB10(arg_0, 1);//arg0
	int i, ieax;
	for (i = 0; sub_1000AB10(arg_0, 1); ++i);
	if (i < pesi->dw0x5C) {
		if (pesi->dw0x68 && pesi->dw0x18 < 2 && i == 1) {
			ieax = pesi->dw0x58;
		}
		else {
			ieax = i * pesi->dw0x58 + pesi->dw0x58 / 2;
		}
	}
	else {
		if (pesi->dw0x68 && pesi->dw0x18 < 2 && (i - pesi->dw0x5C) == 1) {
			ieax = pesi->dw0x54;
		}
		else {
			ieax = (i - pesi->dw0x5C) * pesi->dw0x54 + pesi->dw0x54 / 2;
		}
	}
	if (pesi->dw0x18 < 2 && !pesi->dw0x68)
		ieax = ieax * pesi->dw0x60 / 8 + ieax;
	if (iebp) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_10030730(pesi->hg0x00, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		c = -a;
		b = -ieax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_10030730(pesi->hg0x04[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
void sub_100186A0(T_size_24 * arg_0, T_size_178 * arg_4, int *** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C) {
	T_size_178 * pesi = arg_4;
	sub_100280B0(&pesi->t_38, 0x800, 1);
	pesi->dw_50 = sub_1000AB10(arg_0, 5);
	pesi->hg0x04 = arg_8;
	pesi->hg0x00 = *arg_8;
	if (arg_C) {
		pesi->hg0x0C = arg_C;
		pesi->hg0x08 = *arg_C;
	}
	pesi->dw0x10 = arg_10;
	pesi->dw0x14 = arg_14;
	pesi->dw0x34 = arg_1C;
	pesi->dw0x30 = arg_18;
	pesi->dw0x60 = arg_24;
	pesi->dw0x68 = arg_2C;
	pesi->dw0x54 = arg_20;
	pesi->dw0x64 = arg_28;
	pesi->dw0x58 = arg_20 / (arg_28 + 1);
	if (pesi->dw0x58 < 1)
		pesi->dw0x58 = 1;
	if (arg_28 > 0) {
		pesi->dw0x5C = arg_20 / pesi->dw0x58;
	}
	else
		pesi->dw0x5C = 0;
}
//k
int sub_10018760(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	unsigned char iebx = sub_1000AB10(arg_0, 1);
	int ieax = sub_100282C0(&pesi->t_38, arg_0, pesi->dw_50);
	if (pesi->dw0x68 && ieax == 1) {
		ieax = pesi->dw0x54;
	}
	else {
		ieax = ieax * pesi->dw0x54 + pesi->dw0x54 / 2;
	}
	if (iebx) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_1002C8F0(pesi->hg0x08, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		c = -a;
		b = -ieax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_100287E0(pesi->hg0x0C[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_100188D0(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	unsigned char iebx = sub_1000AB10(arg_0, 1);
	int ieax = sub_100282C0(&pesi->t_38, arg_0, pesi->dw_50);
	if (ieax < pesi->dw0x5C) {
		if (pesi->dw0x68 && ieax == 1) {
			ieax = pesi->dw0x58;
		}
		else {
			ieax = ieax * pesi->dw0x58 + pesi->dw0x58 / 2;
		}
	}
	else {
		if (pesi->dw0x68 && (ieax - pesi->dw0x5C) == 1) {
			ieax = pesi->dw0x54;
		}
		else {
			ieax = (ieax - pesi->dw0x5C) * pesi->dw0x54 + pesi->dw0x54 / 2;
		}
	}
	if (!pesi->dw0x68)
		ieax = ieax * pesi->dw0x60 / 8 + ieax;
	if (iebx) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_1002C8F0(pesi->hg0x08, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		c = -a;
		b = -ieax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_100287E0(pesi->hg0x0C[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10018A70(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	unsigned char iebx = sub_1000AB10(arg_0, 1);
	int ieax = sub_100282C0(&pesi->t_38, arg_0, pesi->dw_50);
	if (pesi->dw0x68 && pesi->dw0x18 < 2 && ieax == 1) {
		ieax = pesi->dw0x54;
	}
	else {
		ieax = ieax * pesi->dw0x54 + pesi->dw0x54 / 2;
	}

	if (iebx) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_10030730(pesi->hg0x00, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		c = -a;
		b = -ieax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_10030730(pesi->hg0x04[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10018BE0(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	unsigned char iebx = sub_1000AB10(arg_0, 1);
	int ieax = sub_100282C0(&pesi->t_38, arg_0, pesi->dw_50);
	if (ieax < pesi->dw0x5C) {
		if (pesi->dw0x68 && pesi->dw0x18 < 2 && ieax == 1) {
			ieax = pesi->dw0x58;
		}
		else {
			ieax = ieax * pesi->dw0x58 + pesi->dw0x58 / 2;
		}
	}
	else {
		if (pesi->dw0x68 && pesi->dw0x18 < 2 && (ieax - pesi->dw0x5C) == 1) {
			ieax = pesi->dw0x54;
		}
		else {
			ieax = (ieax - pesi->dw0x5C) * pesi->dw0x54 + pesi->dw0x54 / 2;
		}
	}
	if (pesi->dw0x18 < 2 && !pesi->dw0x68)
		ieax = ieax * pesi->dw0x60 / 8 + ieax;
	if (iebx) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_10030730(pesi->hg0x00, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		c = -a;
		b = -ieax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_10030730(pesi->hg0x04[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
void sub_10018D90(T_size_24 * arg_0, T_size_178 * arg_4, int *** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C) {
	T_size_178 * pesi = arg_4;
	pesi->pt_4C = sub_10031B10();
	sub_10031D80(1, arg_0, pesi->pt_4C);
	pesi->hg0x04 = arg_8;
	pesi->hg0x00 = *arg_8;
	if (arg_C) {
		pesi->hg0x0C = arg_C;
		pesi->hg0x08 = *arg_C;
	}
	pesi->dw0x10 = arg_10;
	pesi->dw0x14 = arg_14;
	pesi->dw0x34 = arg_1C;
	pesi->dw0x30 = arg_18;
	pesi->dw0x60 = arg_24;
	pesi->dw0x68 = arg_2C;
	pesi->dw0x54 = arg_20;
	pesi->dw0x64 = arg_28;
	pesi->dw0x58 = arg_20 / (arg_28 + 1);
	if (pesi->dw0x58 < 1)
		pesi->dw0x58 = 1;
	if (arg_28 > 0) {
		pesi->dw0x5C = arg_20 / pesi->dw0x58;
	}
	else
		pesi->dw0x5C = 0;

}
//k
void sub_10018E40(T_size_24 * arg_0, T_size_178 * arg_4) {
	sub_10031C60(arg_0, arg_4->pt_4C);
}
//k
int sub_10018E60(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	int ieax = sub_10031E40(arg_0, pesi->pt_4C);
	if (!ieax) return 0;
	int iedx;
	if ((unsigned char)ieax & 1)
	{
		ieax /= 2;
		iedx = 1;
	}
	else {
		ieax /= 2;
		--ieax;
		iedx = 0;
	}
	if (pesi->dw0x68 && pesi->dw0x18 < 2 && ieax == 1) {
		ieax = pesi->dw0x54;
	}
	else {
		ieax = ieax * pesi->dw0x54 + pesi->dw0x54 / 2;
	}

	if (iedx) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_1002C8F0(pesi->hg0x08, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;//iecx
		c = -a;//iedi
		b = -ieax / 2;//iebp
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_100287E0(pesi->hg0x0C[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10018FE0(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	int ieax = sub_10031E40(arg_0, pesi->pt_4C);
	if (!ieax) return 0;
	int iebp;
	if ((unsigned char)ieax & 1)
	{
		ieax /= 2;
		iebp = 1;
	}
	else {
		ieax /= 2;
		--ieax;
		iebp = 0;
	}
	if (ieax < pesi->dw0x5C) {
		if (pesi->dw0x68 && ieax == 1) {
			ieax = pesi->dw0x58;
		}
		else {
			ieax = ieax * pesi->dw0x58 + pesi->dw0x58 / 2;
		}
	}
	else {
		if (pesi->dw0x68 && (ieax - pesi->dw0x5C) == 1) {
			ieax = pesi->dw0x54;
		}
		else {
			ieax = (ieax - pesi->dw0x5C) * pesi->dw0x54 + pesi->dw0x54 / 2;
		}
	}
	if (!pesi->dw0x68)
		ieax = ieax * pesi->dw0x60 / 8 + ieax;
	if (iebp) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_1002C8F0(pesi->hg0x08, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		c = -a;
		b = -ieax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_100287E0(pesi->hg0x0C[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10019180(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	int ieax = sub_10031E40(arg_0, pesi->pt_4C);
	if (!ieax) return 0;
	int iedx;
	if ((unsigned char)ieax & 1)
	{
		ieax /= 2;
		iedx = 1;
	}
	else {
		ieax /= 2;
		--ieax;
		iedx = 0;
	}
	if (pesi->dw0x68 && pesi->dw0x18 < 2 && ieax == 1) {
		ieax = pesi->dw0x54;
	}
	else {
		ieax = ieax * pesi->dw0x54 + pesi->dw0x54 / 2;
	}

	if (iedx) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_10030730(pesi->hg0x00, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;//iecx
		c = -a;//iedi
		b = -ieax / 2;//iebp
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_10030730(pesi->hg0x04[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10019300(T_size_24 *arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_178 * pesi = arg_4;
	T_size_24 * pedi = arg_0;
	int ieax = sub_10031E40(arg_0, pesi->pt_4C);
	if (!ieax) return 0;
	int iebp;
	if ((unsigned char)ieax & 1)
	{
		ieax /= 2;
		iebp = 1;
	}
	else {
		ieax /= 2;
		--ieax;
		iebp = 0;
	}
	if (ieax < pesi->dw0x5C) {
		if (pesi->dw0x68 && pesi->dw0x18 < 2 && ieax == 1) {
			ieax = pesi->dw0x58;
		}
		else {
			ieax = ieax * pesi->dw0x58 + pesi->dw0x58 / 2;
		}
	}
	else {
		if (pesi->dw0x68 && pesi->dw0x18 < 2 && (ieax - pesi->dw0x5C) == 1) {
			ieax = pesi->dw0x54;
		}
		else {
			ieax = (ieax - pesi->dw0x5C) * pesi->dw0x54 + pesi->dw0x54 / 2;
		}
	}
	if (pesi->dw0x18 < 2 && !pesi->dw0x68)
		ieax = ieax * pesi->dw0x60 / 8 + ieax;
	if (iebp) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_10030730(pesi->hg0x00, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int c, b, a;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		c = -a;
		b = -ieax / 2;
	}
	else if (!(pesi->dw0x30 - 2)) {
		a = ieax / 4 + ieax;
		b = -a;
		c = ieax * 4 - ieax / 2;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}
	sub_10030730(pesi->hg0x04[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
void sub_100194C0(int** arg_0, int arg_4, int arg_8, int arg_C) {
	unsigned long varA = 1 << ((arg_C + 1) % 256);
	unsigned long iedx = (arg_4 + varA - 2) >> (arg_C % 256);
	unsigned long iebx = (arg_8 + varA - 2) >> (arg_C % 256);
	int c = (iedx - 1) % 2;//var_14
	int a = (iedx + 2) / 2;//vararg_8
	int b = (iebx + 2) / 2;//iebp
	int d = (iebx - 1) % 2;//vararg_c
	if (c) {
		int * pedi = &arg_0[a - 1][b - 1];
		int * pecx = &arg_0[iedx - 1][iebx - 1];
		if (d) {
			int r = *pedi + 1;
			*pecx = r / 2;
			pedi--;
			pecx--;
		}
		int rPrev = (*pedi + 1);//esi
		*pecx = rPrev / 2;
		pecx--;
		pedi--;
		for (int i = b - d - 1; i > 0; i--) {//arg_4
			int r = *pedi + 1;
			*pecx = (rPrev + r) / 4;
			pecx--;
			*pecx = (r) / 2;
			pecx--;
			pedi--;
			rPrev = r;
		}
	}

	//loc_1001958C
	//var_C = iebx * 4;
	int * pedx = &arg_0[a - c - 1][b - 1];
	int * pecx = &arg_0[iedx - c - 1][iebx - 1];
	//int vararg_4 = vararg_8 - var_14;
	//var_8 = iebp * 4;
	if (d) {
		*pecx = (*pedx + 1) / 2;
		pecx--;
		pedx--;
	}
	//loc_100195D2
	int rPrev = (*pedx + 1);//esi
	*pecx = rPrev >> 1;
	pecx--;
	pedx--;

	//vararg_8 = iebp - vararg_c;
	for (int i = b - d - 1; i > 0; i--) {//ebp
		int r = *pedx + 1;
		*pecx = (rPrev + r) >> 2;
		pecx--;
		*pecx = r >> 1;
		pecx--;
		pedx--;
		rPrev = r;
	}
	//int var_4 = /*vararg_8*/b - d - 2;
	for (int i = a - c - 1; i > 0; i--) {//var_10
										 //var_14 = &((int**)arg_0)[var_10 - 1];-4
										 //arg_8 = &((int**)arg_0)[(var_10 - 1) * 2];-8
		int * peax = &arg_0[(i - 1) * 2][iebx - 1];
		int * pecx = &arg_0[(i - 1) * 2 + 1][iebx - 1];
		int * pedi = &arg_0[i - 1][b - 1];
		int * pebx = &arg_0[i - 1 + 1][b - 1];
		if (d) {
			int  r = *pedi + 1;
			*pecx = (*pebx + 1 + r) >> 2;
			pecx--;
			*peax = (r) >> 1;
			peax--;
			pebx--;
			pedi--;
		}
		int rPrev = *pedi + 1;//esi
		*peax = rPrev >> 1;
		peax--;
		int vararg_0 = (*pebx + rPrev + 1);
		*pecx = vararg_0 >> 2;
		pecx--;
		pedi--;
		pebx--;
		for (int j = b - d - 2; j > 0; j--) {//arg_4 ebp
			int r = *pedi + 1;
			*peax = (rPrev + r) >> 2;
			peax--;
			*peax = r >> 1;
			peax--;
			*pecx = (vararg_0 + r + *pebx + 1) >> 3;
			pecx--;
			*pecx = (r + *pebx + 1) >> 2;
			vararg_0 = r + *pebx + 1;
			pecx--;
			rPrev = r;
			pebx--;
			pedi--;
		}
		int r = *pedi + 1;
		*peax = (rPrev + r) >> 2;
		peax--;
		*pecx = (vararg_0 + r + *pebx + 1) >> 3;
		pecx--;
		*pecx = (r + *pebx + 1) >> 2;
		*peax = r >> 1;
		pedi--;
		pebx--;
	}
}
//k
void sub_10019760(int** arg_0, unsigned char** arg_4, int arg_8, int arg_C) {
	int a = (arg_8 + 2) / 2;//var_C
	int c = (arg_8 - 1) & 1;//iebp
	int b = (arg_C + 2) / 2;//var_arg_8
	int d = (arg_C - 1) & 1;//var_14
	if (c) {
		int *pedi;
		unsigned char *pecx;
		pedi = &arg_0[a - 1][b - 1];
		pecx = &arg_4[arg_8 - 1][(arg_C - 1) * 3];
		if (d) {
			int r;
			if ((*pedi + 0x20) >= 0x4000)
				r = -1;
			else if ((*pedi + 0x20) < 0)
				r = 0;
			else
				r = (*pedi + 0x20) >> 6;
			*pecx = (unsigned char)r;
			pecx -= 3;
			pedi--;
		}
		int r1;//edx
		if (*pedi >= 0x4000)
			r1 = -1;
		else if (*pedi < 0)
			r1 = 0;
		else
			r1 = *pedi >> 6;
		*pecx = (unsigned char)r1;
		pecx -= 3;
		pedi--;
		//loc_10019801
		for (int i = b - d - 1; i > 0; i--) {//var_10
			int r;//eax
			if ((*pedi + 0x20) >= 0x4000)
				r = -1;
			else if ((*pedi + 0x20) < 0)
				r = 0;
			else
				r = (*pedi + 0x20) >> 6;
			*pecx = (unsigned char)(r1 + r) / 2;
			pecx -= 3;
			*pecx = (unsigned char)r;
			pecx -= 3;
			pedi--;
			r1 = r;
		}
	}
	//loc_10019865
	int * pebp = &arg_0[a - c - 1][b - 1];
	unsigned char * pesi = &arg_4[arg_8 - c - 1][(arg_C - 1) * 3];
	//var_8 = arg_C * 3;
	//var_10 = var_arg8 * 4;
	//int varC = a - c;
	unsigned char ieax;
	if (d) {
		if ((*pebp + 0x20) >= 0x4000)
			ieax = -1;
		else if ((*pebp + 0x20) < 0)
			ieax = 0;
		else
			ieax = (*pebp + 0x20) >> 6;
		*pesi = (unsigned char)ieax;
		pesi -= 3;
		pebp--;
	}
	int rPrev;
	if ((*pebp + 0x20) >= 0x4000)
		rPrev = -1;
	else if ((*pebp + 0x20) < 0)
		rPrev = 0;
	else
		rPrev = (*pebp + 0x20) >> 6;
	//loc_100198E8
	*pesi = (unsigned char)rPrev;
	pesi -= 3;
	pebp--;
	//var_arg8_1 = b - d;
	for (int i = b - d - 1; i > 0; i--) {//arg_C
		int r;
		if ((*pebp + 0x20) >= 0x4000)
			r = -1;
		else if ((*pebp + 0x20) < 0)
			r = 0;
		else
			r = (*pebp + 0x20) >> 6;
		*pesi = (unsigned char)(rPrev + r) / 2;
		pesi -= 3;
		*pesi = (unsigned char)r;
		pesi -= 3;
		pebp--;
		rPrev = r;
	}
	//loc_10019941
	int var_4 = b - d - 2;
	for (int i = a - c - 1; i > 0; i--) {//varC
										 //int ** parg_C = &((int**)arg_0)[i - 1];-4
										 //unsigned char ** parg_0 = &((unsigned char**)arg_4)[i * 2];-8
		unsigned char * pesi = &arg_4[i * 2 - 1][(arg_C - 1) * 3];
		unsigned char * pecx = &arg_4[i * 2 - 2][(arg_C - 1) * 3];
		unsigned char * pebp = &arg_4[i * 2][(arg_C - 1) * 3];
		int * pebx = &arg_0[i - 1][b - 1];
		if (d) {
			unsigned char iedi = *pebp;
			pebp -= 3;
			int r;
			if ((*pebx + 0x20) >= 0x4000)
				r = -1;
			else if ((*pebx + 0x20) < 0)
				r = 0;
			else
				r = (*pebx + 0x20) >> 6;
			*pesi = ((unsigned char)r + iedi) / 2;
			pesi -= 3;
			*pecx = (unsigned char)r;
			pecx -= 3;
			pebx--;
		}
		int iedx;
		//loc_100199E3
		if ((*pebx + 0x20) >= 0x4000)
			iedx = -1;
		else if ((*pebx + 0x20) < 0)
			iedx = 0;
		else
			iedx = (*pebx + 0x20) >> 6;
		*pecx = (unsigned char)iedx;
		pecx -= 3;
		*pesi = ((unsigned char)iedx + *pebp) / 2;
		pesi -= 3;
		int iedi = (unsigned char)iedx + *pebp;
		pebp -= 6;
		pebx--;
		for (int j = var_4; j > 0; j--) {//iarg_4
			unsigned char ieax;
			if ((*pebx + 0x20) >= 0x4000)
				ieax = 255;
			else if ((*pebx + 0x20) < 0)
				ieax = 0;
			else
				ieax = (*pebx + 0x20) >> 6;
			*pecx = ((unsigned char)ieax + (unsigned char)iedx) / 2;
			pecx -= 3;
			*pecx = (unsigned char)ieax;
			pecx -= 3;
			*pesi = (iedi + (unsigned char)ieax + *pebp) / 4;
			*pesi -= 3;
			*pesi = ((unsigned char)ieax + *pebp) / 2;
			iedi = ieax + *pebp;
			pesi -= 3;
			pebp -= 6;
			pebx--;
		}
		//loc_10019A8C
		if ((*pebx + 0x20) >= 0x4000)
			ieax = 255;
		else if ((*pebx + 0x20) < 0)
			ieax = 0;
		else
			ieax = (*pebx + 0x20) >> 6;
		*pecx = ((unsigned char)iedx + (unsigned char)ieax) / 2;
		pecx -= 3;
		*pesi = ((unsigned char)ieax + *pebp + iedi) / 4;
		pesi -= 3;
		*pesi = ((unsigned char)ieax + *pebp) / 2;
		pesi -= 3;
		*pecx = (unsigned char)ieax / 2;
		pecx -= 3;
	}
}
//k
void sub_10019B00(int** arg_0, unsigned char** arg_4, int arg_8, int arg_C) {
	int a = (arg_8 + 2) / 2;//var_8
	int c = (arg_8 - 1) & 1;//iedx
	int b = (arg_C + 2) / 2;//var_C
	int d = (arg_C - 1) & 1;//var_14
	int *pesi;
	unsigned char *pecx;
	if (c) {
		pesi = &arg_0[a - 1][b - 1];
		pecx = &arg_4[arg_8 - 1][arg_C - 1];
		unsigned char ieax;
		if (d) {
			if ((*pesi + 0x20) >= 0x4000)
				ieax = 255;
			else if ((*pesi + 0x20) < 0)
				ieax = 0;
			else
				ieax = (*pesi + 0x20) >> 6;
			*pecx = ieax;
			pecx--;
			pesi--;

		}
		//loc_10019B7D
		unsigned char iarg_8;
		if ((*pesi + 0x20) >= 0x4000)
			iarg_8 = 255;
		else if ((*pesi + 0x20) < 0)
			iarg_8 = 0;
		else
			iarg_8 = (*pesi + 0x20) >> 6;
		*pecx = iarg_8;
		pecx--;
		pesi--;
		for (int i = b - d - 1; i > 0; i--) {//var_10
			unsigned char ieax;
			if ((*pesi + 0x20) >= 0x4000)
				ieax = 255;
			else if ((*pesi + 0x20) < 0)
				ieax = 0;
			else
				ieax = (*pesi + 0x20) >> 6;
			*pecx = (iarg_8 + ieax) / 2;
			pecx--;
			*pecx = ieax;
			pecx--;
			pesi--;
			iarg_8 = ieax;
		}
	}
	//loc_10019C0A
	//int iedi = arg_8 - iedx;
	pesi = &arg_0[a - c - 1][b - 1];
	pecx = &arg_4[arg_8 - c - 1][arg_C - 1];
	//var_10 = var_C * 4;
	//int varC = a - c;
	int ieax;
	if (d) {
		if ((*pesi + 0x20) >= 0x4000)
			ieax = -1;
		else if ((*pesi + 0x20) < 0)
			ieax = 0;
		else
			ieax = (*pesi + 0x20) >> 6;
		*pecx = (unsigned char)ieax;
		pecx--;
		pesi--;
	}
	//loc_10019C61
	int iedx_1;
	if ((*pesi + 0x20) >= 0x4000)
		iedx_1 = -1;
	else if ((*pesi + 0x20) < 0)
		iedx_1 = 0;
	else
		iedx_1 = (*pesi + 0x20) >> 6;
	*pecx = (unsigned char)iedx_1;
	pecx--;
	pesi--;
	a = b - d;
	int i;
	for (i = b - d - 1; i > 0; i--) {//arg_8
		int ieax;
		if ((*pesi + 0x20) >= 0x4000)
			ieax = -1;
		else if ((*pesi + 0x20) < 0)
			ieax = 0;
		else
			ieax = (*pesi + 0x20) >> 6;
		*pecx = (unsigned char)(iedx_1 + ieax) / 2;
		pecx--;
		*pecx = (unsigned char)ieax;
		pecx--;
		pesi--;
		iedx_1 = ieax;
	}
	//loc_10019CD7
	//int var_4 = b - d - 2;
	for (i = a - c - 1; i > 0; i--)//varC
	{
		//int ** parg_8 = &((int**)arg_0)[varC - 1];
		//unsigned char ** parg_0 = &((unsigned char**)arg_4)[varC * 2];

		unsigned char * pesi = &arg_4[i * 2 - 1][arg_C - 1];
		unsigned char * pecx = &arg_4[i * 2 - 2][arg_C - 1];
		unsigned char * pebp = &arg_4[i * 2][arg_C - 1];
		int * pebx = &arg_0[i - 1][b - 1];

		if (d) {
			unsigned char iedi = *pebp;
			pebp--;
			if ((*pebx + 0x20) >= 0x4000)
				ieax = -1;
			else if ((*pebx + 0x20) < 0)
				ieax = 0;
			else
				ieax = (*pebx + 0x20) >> 6;
			*pesi = ((unsigned char)ieax + iedi) / 2;
			pesi--;
			*pecx = (unsigned char)ieax;
			pecx--;
			pebx--;
		}
		int iedx_2;
		//loc_10019D72
		if ((*pebx + 0x20) >= 0x4000)
			iedx_2 = -1;
		else if ((*pebx + 0x20) < 0)
			iedx_2 = 0;
		else
			iedx_2 = (*pebx + 0x20) >> 6;
		*pecx = (unsigned char)iedx_2;
		pecx--;
		*pesi = ((unsigned char)iedx_2 + *pebp) / 2;
		pesi--;
		int iedi = *pebp + iedx_2;
		pebp -= 2;
		pebx--;
		for (int j = b - d - 2; j > 0; j--) {//arg_4
			int ieax;
			if ((*pebx + 0x20) >= 0x4000)
				ieax = -1;
			else if ((*pebx + 0x20) < 0)
				ieax = 0;
			else
				ieax = (*pebx + 0x20) >> 6;
			*pecx = ((unsigned char)ieax + iedx_2) / 2;
			pecx--;
			*pecx = (unsigned char)ieax;
			pecx--;
			*pesi = (iedi + ieax + *pebp) / 4;
			*pesi--;
			*pesi = (ieax + *pebp) / 2;
			//iedx_2 = ieax;?????????????????
			iedi = ieax + *pebp;
			pebp -= 2;
			pesi--;
			pebx--;

		}
		//loc_10019E12
		if ((*pebx + 0x20) >= 0x4000)
			ieax = -1;
		else if ((*pebx + 0x20) < 0)
			ieax = 0;
		else
			ieax = (*pebx + 0x20) >> 6;
		*pecx = ((unsigned char)iedx_2 + ieax) / 2;
		pecx--;
		*pecx = ieax;
		*pesi = ((unsigned char)ieax + *pebp + iedi) / 4;
		pesi--;
		*pesi = ((unsigned char)ieax + *pebp) / 2;
		pesi--;
		*pecx = ((unsigned char)ieax) / 2;
		pecx--;
		pebp--;
	}
}
//k
void sub_10019E80(int** arg_0, int** arg_4, int arg_8, int arg_C, int arg_10) {
	int **pesi = arg_0;
	int **pebx = arg_4;
	int var_C = 0;
	//int var_4 = 0;
	for (int i = arg_C; i > 0; i--) //iedx 65
		arg_4[0][arg_C - i] = arg_0[0][arg_C - i];
	for (int i = arg_C; i > 0; i--)
		arg_4[arg_8 - 1][arg_C - i] = arg_0[arg_8 - 1][arg_C - i];
	int i, offset;
	//int var_4 = arg_C - 1;
	offset = pebx - pesi;
	for (i = arg_8 - 2; i > 0; i--) {//arg_0 7e - 2
									 //loc_10019F06
									 //int *pecx = arg_0[i];
									 //int *pedx = arg_0[i + offset];
		int iprevEbx = arg_0[i][0];
		arg_0[i + offset][0] = arg_0[i][0];
		//int * pedi = &arg_0[i - 1][1];
		//int * pesi = &arg_0[i + 1][1];
		int iNextEax = arg_0[i][1];
		int var_8, j;
		int k = 0;
		for (j = arg_C - 2; j > 0; j--, k++) {// arg_8 65 -3
											  //var_C = pecx[1];
											  //pecx++;
			var_C = arg_0[i][2 + k];
			var_8 = iNextEax * 3 * 4 - iprevEbx;
			arg_0[i + offset][1 + k] = (var_8 - var_C - arg_0[i + 1][1 + k] - arg_0[i - 1][1 + k]) >> 3;
			iprevEbx = iNextEax;
			iNextEax = var_C;
		}
		arg_0[i + offset][1 + k] = var_C; // 64 3fa8
	}
}
//k
void sub_100201D0(T_size_24 * arg_0, unsigned char** arg_4, int arg_8, int arg_C, int arg_10) {
	T_size_24 * pesi = arg_0;
	T_size_Above_3C var_3C;
	sub_100280B0(&var_3C.tvar_4, 0x100, 0);
	var_3C.dw_0 = sub_10028360(&var_3C.tvar_4, arg_0);
	var_3C.dw_20 = arg_10;
	sub_10011910(&var_3C.hg_18, &var_3C.hg_1C, arg_8, arg_C, arg_10);
	var_3C.dw_24 = 0;
	for (int i = 0; i < arg_10; i++, var_3C.dw_24 = i) { //var_18
		var_3C.dw_28 = 0;
		for (int j = 0; j < 3; j++, var_3C.dw_28 = j) {//var_14
			if (!((unsigned char)pesi->dw0x14 & 1)) {
				sub_10011870(arg_8, arg_C, var_3C.dw_24, var_3C.dw_28, &var_3C.dw_2C, &var_3C.dw_30, &var_3C.dw_34, &var_3C.dw_38);
				sub_100202D0(pesi, &var_3C, arg_4);
			}
		}
	}
	sub_10028140(&var_3C.tvar_4);
	sub_10011A10(var_3C.hg_18, var_3C.hg_1C);
	if (var_3C.dw_0) {
		pesi->dw0x14 = 1;
	}
}
//k?
void sub_100202D0(T_size_24 * arg_0, T_size_Above_3C* arg_4, unsigned char** arg_8) {
	T_size_Above_3C *pesi = arg_4;
	for (int i = pesi->dw_2C; i < pesi->dw_30; i++) {//var_4 edi
		int iedi = i;
		for (int j = pesi->dw_34; j < pesi->dw_38; j++) {//var_C ebx
			int iebx = j;
			if (arg_8[i][j] == 1) {
				if (!pesi->dw_0)
				{
					arg_8[i][j] = 2;
					int iarg_4 = pesi->dw_38;
					int iebp = pesi->dw_30;
					for (int k = pesi->dw_24 + 1; k < pesi->dw_20; k++, iebp = iebp / 2 + 1, iarg_4 = iarg_4 / 2 + 1) {
						switch (pesi->hg_18[pesi->dw_28][k - 1]) {
						case 3:
							if (iedi == (iebp - 1)) {
								iedi--;
							}
						case 2:
							iedi++;
						case 0:case 1:
							iedi++;
						default:
							switch (pesi->hg_1C[pesi->dw_28][k - 1]) {
							case 3:
								if (iebx == (iarg_4 - 1)) {
									iebx--;
								}
							case 2:
								iebx++;
							case 0:case 1:
								iebx++;
							}
						}
						iedi = iedi / 2;
						iebx = iebx / 2;
						if (arg_8[iedi][iebx] == 4)
							break;
						arg_8[iedi][iebx] = 4;
					}
					pesi->dw_0 = sub_10028360(&pesi->tvar_4, arg_0);
				}
				else
					pesi->dw_0--;
			}
		}
	}

}
//k
void sub_10020440(T_size_24 * arg_0, unsigned char** arg_4, int arg_8, int arg_C, int arg_10) {
	T_size_Above_3C var_3C;
	sub_100280B0(&var_3C.tvar_4, 0x100, 0);
	var_3C.dw_0 = sub_10028360(&var_3C.tvar_4, arg_0); // 1bf
	sub_10011910(&var_3C.hg_18, &var_3C.hg_1C, arg_8, arg_C, arg_10);
	T_size_24 * pesi = (T_size_24*)arg_0;
	int iecx = 1;
	var_3C.dw_24 = 0;
	var_3C.dw_20 = arg_10;
	for (int i = 0; i < arg_10; i++, var_3C.dw_24 = i) {
		if (!((unsigned char)pesi->dw0x14 & 1)) {
			int var_40, var_4C, var_44, var_48;
			if (iecx) {
				sub_10011870(arg_8, arg_C, var_3C.dw_24, 0, &var_40, &var_4C, &var_44, &var_48);
				var_3C.dw_38 = var_48;
				var_3C.dw_30 = var_4C;
				var_3C.dw_28 = 0;
				sub_10020720(pesi, arg_4, &var_3C, 3, var_40, var_4C, var_44, var_48);//620d 81 1     10 66   04 50
				sub_10011870(arg_8, arg_C, var_3C.dw_24, 2, &var_40, &var_4C, &var_44, &var_48);
				var_3C.dw_38 = var_48;
				var_3C.dw_30 = var_4C;
				var_3C.dw_28 = 2;
				sub_10020720(pesi, arg_4, &var_3C, 0, var_40, var_4C, var_44, var_48);//3219 0 9				02 d6
				sub_10011870(arg_8, arg_C, var_3C.dw_24, 1, &var_40, &var_4C, &var_44, &var_48);
				var_3C.dw_38 = var_48;
				var_3C.dw_30 = var_4C;
				var_3C.dw_28 = 1;
				//100205C1
				sub_10020720(pesi, arg_4, &var_3C, 0, var_40, var_4C, var_44, var_48);//12d 1a 1 error 1b		10 10
				iecx = 0;
			}
			else {
				sub_10011870(arg_8, arg_C, var_3C.dw_24, 1, &var_40, &var_4C, &var_44, &var_48);
				var_3C.dw_38 = var_48;
				var_3C.dw_30 = var_4C;
				var_3C.dw_28 = 1;
				sub_10020720(pesi, arg_4, &var_3C, 1, var_40, var_4C, var_44, var_48);//535 18     40 e
				sub_10011870(arg_8, arg_C, var_3C.dw_24, 2, &var_40, &var_4C, &var_44, &var_48);
				var_3C.dw_38 = var_48;
				var_3C.dw_30 = var_4C;
				var_3C.dw_28 = 2;
				sub_10020720(pesi, arg_4, &var_3C, 2, var_40, var_4C, var_44, var_48);//185 1    02 d0
				sub_10011870(arg_8, arg_C, var_3C.dw_24, 0, &var_40, &var_4C, &var_44, &var_48);
				var_3C.dw_38 = var_48;
				var_3C.dw_30 = var_4C;
				var_3C.dw_28 = 0;
				sub_10020720(pesi, arg_4, &var_3C, 2, var_40, var_4C, var_44, var_48);//1 1      04 63
				iecx = 1;
			}
		}
	}
	sub_10028140(&var_3C.tvar_4);
	sub_10011A10(var_3C.hg_18, var_3C.hg_1C);
	if (var_3C.dw_0) {// 0 , 1
		pesi->dw0x14 = 1;
	}
}
//k
void sub_10020720(T_size_24 * arg_0, unsigned char** arg_4, T_size_Above_3C* arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C) {
#define macro1(){\
		int a = arg_8->dw_30, b = arg_8->dw_38;\
		for (int k = arg_8->dw_24 + 1; k < arg_8->dw_20; k++) {\
			switch (arg_8->hg_18[arg_8->dw_28][k - 1]) {\
			case 3:\
				if (newI == (a - 1)) newI--;\
			case 2:\
				newI++;\
			case 0:case 1:\
				newI++;\
			default:\
				break;\
			}\
			switch (arg_8->hg_1C[arg_8->dw_28][k - 1]) {\
			case 3:\
				if (newJ == (b - 1)) newJ--;\
			case 2:\
				newJ++;\
			case 0:case 1:\
				newJ++;\
			default:\
				break;\
			}\
			newJ = newJ / 2;\
			newI = newI / 2;\
			if (arg_4[newI][newJ] == 4)\
				break;\
			a = a / 2 + 1;\
			b = b / 2 + 1;\
			arg_4[newI][newJ] = 4;\
		}\
		arg_8->dw_0 = sub_10028360(&arg_8->tvar_4, arg_0);\
}\

#define macro2(){\
	if (arg_4[i][j] == 1) {\
		if (!arg_8->dw_0) {\
			newI = i;\
			newJ = j;\
			arg_4[i][j] = 2;\
			macro1()\
		}\
		else\
			arg_8->dw_0--;\
	}\
}
	int iebp = arg_1C - arg_18;
	int var_4 = arg_14 - arg_10;
	int iebx = iebp / 2 + arg_18;
	int iedi = var_4 / 2 + arg_10;
	int retval = sub_10021420(arg_4, arg_10, arg_14, arg_18, arg_1C);
	for (; arg_8->dw_0 < retval; retval = sub_10021420(arg_4, arg_10, arg_14, arg_18, arg_1C))
	{
		if (iebp > 2 && var_4 > 2) {
			switch (arg_C) {
			case 0:
				sub_10020720(arg_0, arg_4, arg_8, 3, arg_10, iedi, iebx, arg_1C);
				sub_10020720(arg_0, arg_4, arg_8, 0, iedi, arg_14, iebx, arg_1C);
				sub_10020720(arg_0, arg_4, arg_8, 0, iedi, arg_14, arg_18, iebx);
				arg_1C = iebx;
				arg_14 = iedi;
				arg_C = 2;
				break;
			case 1:
				sub_10020720(arg_0, arg_4, arg_8, 2, iedi, arg_14, arg_18, iebx);
				sub_10020720(arg_0, arg_4, arg_8, 1, arg_10, iedi, arg_18, iebx);
				sub_10020720(arg_0, arg_4, arg_8, 1, arg_10, iedi, iebx, arg_1C);
				arg_18 = iebx;
				arg_10 = iedi;
				arg_C = 3;
				break;
			case 2:
				sub_10020720(arg_0, arg_4, arg_8, 1, iedi, arg_14, arg_18, iebx);
				sub_10020720(arg_0, arg_4, arg_8, 2, iedi, arg_14, iebx, arg_1C);
				sub_10020720(arg_0, arg_4, arg_8, 2, arg_10, iedi, iebx, arg_1C);
				arg_1C = iebx;
				arg_14 = iedi;
				arg_C = 0;
				break;
			case 3:
				sub_10020720(arg_0, arg_4, arg_8, 0, arg_10, iedi, iebx, arg_1C);
				sub_10020720(arg_0, arg_4, arg_8, 3, arg_10, iedi, arg_18, iebx);
				sub_10020720(arg_0, arg_4, arg_8, 3, iedi, arg_14, arg_18, iebx);
				arg_18 = iebx;
				arg_10 = iedi;
				arg_C = 1;
				break;
			default:
				return;
			}
			iebp = arg_1C - arg_18;
			var_4 = arg_14 - arg_10;
			iedi = var_4 / 2 + arg_10;
			iebx = iebp / 2 + arg_18;
			continue;
		}
		switch (arg_C) {
		case 0:
		{
			int ieax = 1;
			int newI, newJ;
			for (int j = arg_1C - 1; j >= arg_18; j--) {//arg_8 ebx
				if (ieax == 1) {
					for (int i = arg_10; i < arg_14; i++) {//var_4 edi arg_1C = arg_4[arg_10] + 4
						macro2()
					}
					ieax = 0;
				}
				else {
					for (int i = arg_14 - 1; i >= arg_10; i--) {
						macro2()
					}
					ieax = 1;
				}

			}
			return;
		}
		case 1:
		{
			int ieax = 0;
			int newI, newJ;
			for (int j = arg_18; j < arg_1C; j++) {
				if (ieax == 1) {
					for (int i = arg_10; i < arg_14; i++) {
						macro2()
					}
					ieax = 0;
				}
				else {
					for (int i = arg_14 - 1; i >= arg_10; i--) {
						macro2()
					}
					ieax = 1;
				}
			}
			return;
		}
		case 2:
		{
			int ieax = 1;
			int newI, newJ;
			for (int i = arg_14 - 1; i >= arg_10; i--) {
				if (ieax == 1) {
					for (int j = arg_18; j < arg_1C; j++) {
						macro2()
					}
					ieax = 0;
				}
				else {
					for (int j = arg_1C - 1; j >= arg_18; j--) {
						macro2()
					}
					ieax = 1;
				}

			}
			return;
		}
		case 3:
		{
			int ieax = 0;
			int newI, newJ;
			for (int i = arg_10; i < arg_14; i++) {
				if (ieax == 1) {
					for (int j = arg_18; j < arg_1C; j++) {
						macro2()
					}
					ieax = 0;
				}
				else {
					for (int j = arg_1C - 1; j >= arg_18; j--) {
						macro2()
					}
					ieax = 1;
				}
			}
			return;
		}
		default:
			return;
		}
	};
	arg_8->dw_0 -= retval;
}
//k
void sub_10021480(T_size_24 * arg_0, unsigned char** arg_4, int arg_8, int arg_C, int arg_10) {
	T_size_24 * pesi = arg_0;
	T_size_Above_3C var_3C;
	sub_10011910(&var_3C.hg_18, &var_3C.hg_1C, arg_8, arg_C, arg_10);
	var_3C.dw_24 = 0;
	var_3C.dw_20 = arg_10;
	for (int i = 0; i < arg_10; i++, var_3C.dw_24 = i) {
		var_3C.dw_28 = 0;
		for (int j = 0; j < 3; j++, var_3C.dw_28 = j) {
			if (!((unsigned char)pesi->dw0x14 & 1)) {
				int var1, var2, var3, var4;
				sub_10011870(arg_8, arg_C, var_3C.dw_24, var_3C.dw_28, &var1, &var2, &var3, &var4);
				if (sub_1000AB10(pesi, 1)) {
					sub_100215A0(pesi, arg_4, &var_3C, var1, var2, var3, var4);
				}
			}
		}
	}
	sub_10011A10(var_3C.hg_18, var_3C.hg_1C);
}
void sub_100215A0(T_size_24 * arg_0, unsigned char ** arg_4, T_size_Above_3C* arg_8, int arg_C, int arg_10, int arg_14, int arg_18) {
	T_size_Above_3C * pesi = arg_8;
	int iebx = arg_C;
	int ieax, iebp;
	for (ieax = arg_10 - iebx, iebp = arg_18 - arg_14; iebp > 2 && ieax > 2; iebp = arg_18 - arg_14, ieax = arg_10 - iebx) {
		int ivaredi = ieax / 2 + arg_C;
		int ivarebp = iebp / 2 + arg_14;
		unsigned char barg_8 = sub_10017870(arg_4, iebx, ivaredi, arg_14, ivarebp);
		int iarg_C = 0;
		if (barg_8) {
			iarg_C = 1;
		}
		unsigned char bvar_4 = sub_10017870(arg_4, iebx, ivaredi, ivarebp, arg_18);
		if (bvar_4) {
			iarg_C++;
		}
		unsigned char bvar_8 = sub_10017870(arg_4, ivaredi, arg_10, arg_14, ivarebp);
		if (bvar_8) {
			iarg_C++;
		}
		unsigned char bvar_C = sub_10017870(arg_4, ivaredi, arg_10, ivarebp, arg_18);
		if (bvar_C) {
			iarg_C++;
		}
		if (iarg_C > 0) {
			if (!barg_8 && iarg_C == 3)
				sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, arg_14, ivarebp);
			else if (sub_1000AB10(arg_0, 1))
				sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, arg_14, ivarebp);
			else
				iarg_C++;
			//loc_100216DD
			if (!bvar_4 && iarg_C == 3) {
				sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, ivarebp, arg_18);
			}
			else if (sub_1000AB10(arg_0, 1))
				sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, ivarebp, arg_18);
			else
				iarg_C++;
			if (!bvar_8 && iarg_C == 3) {
				sub_100215A0(arg_0, arg_4, pesi, ivaredi, arg_10, arg_14, ivarebp);
			}
			else if (sub_1000AB10(arg_0, 1))
				sub_100215A0(arg_0, arg_4, pesi, ivaredi, arg_10, arg_14, ivarebp);
			else
				iarg_C++;
			if (!bvar_C) {
				if (iarg_C == 3) {
					arg_14 = ivarebp;
					iebx = ivaredi;
				}
				else if (sub_1000AB10(arg_0, 1))
				{
					arg_14 = ivarebp;
					iebx = ivaredi;
				}
				else
					return;
			}
			else
				return;
		}
		else {
			unsigned char barg_C;
			if (sub_1000AB10(arg_0, 1)) {
				barg_C = sub_1000AB10(arg_0, 1) * 2;
				barg_C |= sub_1000AB10(arg_0, 1);
				switch (barg_C)
				{
				case 0:
					arg_18 = ivarebp;
					arg_10 = ivaredi;
					break;
				case 1:
					arg_14 = ivarebp;
					arg_10 = ivaredi;
					break;
				case 2:
					arg_18 = ivarebp;
					iebx = ivaredi;
					break;
				case 3:
					arg_14 = ivarebp;
					iebx = ivaredi;
					break;
				default:
					return;
				}
			}
			else if (sub_1000AB10(arg_0, 1))
			{
				if (sub_1000AB10(arg_0, 1)) {
					barg_C = sub_1000AB10(arg_0, 1) * 2;
					barg_C |= sub_1000AB10(arg_0, 1);
					switch (barg_C)
					{
					case 0:
						sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, arg_14, ivarebp);
						arg_14 = ivarebp;
						arg_10 = ivaredi;
						break;
					case 1:
						sub_100215A0(arg_0, arg_4, pesi, ivaredi, arg_10, arg_14, ivarebp);
						arg_14 = ivarebp;
						iebx = ivaredi;
						break;
					case 2:
						sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, arg_14, ivarebp);
						arg_18 = ivarebp;
						iebx = ivaredi;
						break;
					case 3:
						sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, ivarebp, arg_18);
						arg_14 = ivarebp;
						iebx = ivaredi;
						break;
					default:
						return;
					}
				}
				else if (sub_1000AB10(arg_0, 1)) {
					sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, ivarebp, arg_18);
					arg_18 = ivarebp;
					iebx = ivaredi;
				}
				else {
					sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, arg_14, ivarebp);
					arg_14 = ivarebp;
					iebx = ivaredi;
				}
			}
			//loc_10021928
			else if (sub_1000AB10(arg_0, 1)) {
				barg_C = sub_1000AB10(arg_0, 1) * 2;
				barg_C |= sub_1000AB10(arg_0, 1);
				switch (barg_C)
				{
				case 0:
					sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, arg_14, ivarebp);
					sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, ivarebp, arg_18);
					arg_14 = ivarebp;
					iebx = ivaredi;
					break;
				case 1:
					sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, ivarebp, arg_18);
					sub_100215A0(arg_0, arg_4, pesi, ivaredi, arg_10, ivarebp, arg_18);
					arg_18 = ivarebp;
					iebx = ivaredi;
					break;
				case 2:
					sub_100215A0(arg_0, arg_4, pesi, ivaredi, arg_10, ivarebp, arg_18);
					sub_100215A0(arg_0, arg_4, pesi, ivaredi, arg_10, arg_14, ivarebp);
					arg_18 = ivarebp;
					arg_10 = ivaredi;
					break;
				case 3:
					sub_100215A0(arg_0, arg_4, pesi, ivaredi, arg_10, arg_14, ivarebp);
					sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, arg_14, ivarebp);
					arg_14 = ivarebp;
					arg_10 = ivaredi;
					break;
				default:
					return;
				}
			}
			else {
				//loc_10021A7A
				sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, arg_14, ivarebp);
				sub_100215A0(arg_0, arg_4, pesi, iebx, ivaredi, ivarebp, arg_18);
				sub_100215A0(arg_0, arg_4, pesi, ivaredi, arg_10, arg_14, ivarebp);
				arg_14 = ivarebp;
				iebx = ivaredi;
			}
		}


	}
	int ivar4 = ieax * iebp - 1;
	int cnt = 0;//arg_C
	for (int i = arg_10 - iebx; i > 0; i--) {
		for (int j = arg_14; j < arg_18; j++) {
			if (arg_4[arg_10 - iebx - i + iebx][j] == 4) {
				cnt++;
			}
		}
	}
	unsigned char barg_C;
	if (!cnt && ivar4 == 3) {
		if (sub_1000AB10(arg_0, 1)) {
			if (sub_1000AB10(arg_0, 1)) {
				barg_C = 2 - (sub_1000AB10(arg_0, 1) != 0);
			}
			else
				barg_C = sub_1000AB10(arg_0, 1) != 0 ? 4 : 8;
		}
		else if (sub_1000AB10(arg_0, 1)) {
			if (sub_1000AB10(arg_0, 1)) {
				if (sub_1000AB10(arg_0, 1))
					barg_C = sub_1000AB10(arg_0, 1) != 0 ? 9 : 3;
				else
					barg_C = sub_1000AB10(arg_0, 1) != 0 ? 6 : 12;
			}
			else
				barg_C = sub_1000AB10(arg_0, 1) != 0 ? 5 : 10;
		}
		else {
			if (sub_1000AB10(arg_0, 1)) {
				if (sub_1000AB10(arg_0, 1))
					barg_C = sub_1000AB10(arg_0, 1) != 0 ? 13 : 11;
				else
					barg_C = sub_1000AB10(arg_0, 1) != 0 ? 7 : 14;
			}
			else
				barg_C = 15;
		}
		//loc_10021C7A
		unsigned char dl = 16;
		for (int i = iebx; i < arg_10; i++) {
			for (int j = arg_14; j < arg_18; j++) {
				dl = dl / 2;
				if (barg_C != dl) {
					arg_4[i][j] = 2;
					int var10 = pesi->dw_38;
					int iedi = pesi->dw_30;
					int tempi = i, tempj = j;
					for (int k = pesi->dw_24 + 1; k < pesi->dw_20; k++, iedi = iedi / 2 + 1, var10 = var10 / 2 + 1) {
						switch (pesi->hg_18[pesi->dw_28][k - 1]) {
						case 3:
							if (tempi == (iedi - 1)) {
								tempi--;
							}
						case 2:
							tempi++;
						case 0:case 1:
							tempi++;
						default:
							switch (pesi->hg_1C[pesi->dw_28][k - 1]) {
							case 3:
								if (tempj == (var10 - 1)) {
									tempj--;
								}
							case 2:
								tempj++;
							case 0:case 1:
								tempj++;
							}
						}
						tempi /= 2;
						tempj /= 2;
						if (arg_4[tempi][tempj] != 1)
							break;
						arg_4[tempi][tempj] = 4;
					}
					//loc_10021D55
				}
			}
		}
	}
	else {
		//loc_10021D9A
		for (int i = iebx; i < arg_10; i++) {
			for (int j = arg_14; j < arg_18; j++) {
				if (arg_4[i][j] != 4 && (ivar4 == cnt || !sub_1000AB10(arg_0, 1))) {
					arg_4[i][j] = 2;
					int var10 = pesi->dw_38;
					int iedi = pesi->dw_30;
					int tempi = i, tempj = j;
					for (int k = pesi->dw_24 + 1; k < pesi->dw_20; k++, iedi = iedi / 2 + 1, var10 = var10 / 2 + 1) {
						switch (pesi->hg_18[pesi->dw_28][k - 1]) {
						case 3:
							if (tempi == (iedi - 1)) {
								tempi--;
							}
						case 2:
							tempi++;
						case 0:case 1:
							tempi++;
						default:
							switch (pesi->hg_1C[pesi->dw_28][k - 1]) {
							case 3:
								if (tempj == (var10 - 1)) {
									tempj--;
								}
							case 2:
								tempj++;
							case 0:case 1:
								tempj++;
							}
						}
						tempi /= 2;
						tempj /= 2;
						if (arg_4[tempi][tempj] != 1)
							break;
						arg_4[tempi][tempj] = 4;
					}
				}
			}
		}
	}
}
//k
void sub_10021F60(T_size_24 * arg_0, int*** arg_4, unsigned char*** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C, int arg_30, int arg_34, int arg_38) {
	for (int i = arg_1C - 1; i >= 0; i--) {//arg_1C
		int *** pedi = arg_4;
		if (!i && arg_30 > 0) {
			int** hesi;
			if (arg_38) {
				hesi = sub_1000E5C0(arg_10 / 2 + 1, arg_14 / 2 + 1);
			}
			else
				hesi = sub_1000E5C0(arg_10, arg_14);
			if (hesi) {
				sub_10019E80(*pedi, hesi, arg_10 / 2 + 1, arg_14 / 2 + 1, arg_30);
				sub_1000E600(*pedi);
				*pedi = hesi;
			}
		}
		if (!i && arg_38) {
			if (arg_18 == 1) {
				sub_10019B00(*pedi, *arg_8, arg_10, arg_14);
			}
			else
				sub_100328F0(pedi, *arg_8, arg_10, arg_14);
		}
		else {
			for (int j = arg_18; j > 0; j--, pedi++) {//esi
				sub_100194C0(*pedi, arg_10, arg_14, i);
			}
		}
		for (int j = 0; j < arg_18; j++) {//edi
										  //int iarg_14 = (arg_20 - arg_C);
										  //pesi = var_C; + 4
			if (!((unsigned char)arg_0->dw0x14 & 1)) {
				if (!i && arg_38 != i && arg_20[j] == i) {
					if (arg_28 <= 0 && arg_2C <= 0) {
						sub_10022190(arg_0, arg_8, arg_C[j], arg_10, arg_14, j, arg_18, arg_24, arg_34);
					}
					else {
						sub_100223F0(arg_0, arg_8, arg_C[j], arg_10, arg_14, j, arg_18, arg_24, arg_34, arg_2C, arg_28);
					}
				}
				else if (i >= arg_20[j]) {
					if (arg_28 <= 0 && arg_2C <= 0) {
						sub_100226D0(arg_0, arg_4, arg_C[j], arg_10, arg_14, j, arg_18, i, arg_24, arg_34, arg_38);
					}
					else {
						sub_10022940(arg_0, arg_4, arg_C[j], arg_10, arg_14, j, arg_18, i, arg_24, arg_34, arg_2C, arg_28, arg_38);
					}
				}

			}
		}
	}
}
int sub_10021420(unsigned char ** arg_0, int arg_4, int arg_8, int arg_C, int arg_10) {
	int cnt = 0;
	for (int iebx = arg_8 - arg_4; iebx > 0; iebx--) {
		for (int iedx = arg_10 - arg_C; iedx > 0; iedx--) {
			if (arg_0[(arg_8 - arg_4) - iebx + arg_4][(arg_10 - arg_C) - iedx + arg_C] == 1) {
				cnt++;
			}
		}

	}
	return cnt;
}
//1
void sub_10022190(T_size_24 * arg_0, unsigned char*** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20) {
	unsigned char** var_4 = ((unsigned char***)arg_4)[arg_14];
	unsigned char bvar_C = sub_1000AB10(arg_0, 5);
	int var_30, var_34, var_38, var_3C;
	sub_10011870(arg_C, arg_10, 0, 2, &var_38, &var_30, &var_3C, &var_34);
	for (int k = 0; k < 3; k++) {
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, 0, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] != 1) {
					unsigned char bvar_8 = sub_1000AB10(arg_0, 1);
					unsigned char ieax = sub_1000AB10(arg_0, bvar_C);
					if (arg_20 && ieax == 1) {
						ieax = arg_1C;
					}
					else {
						ieax = ieax * arg_1C + arg_1C / 2;
					}
					if (bvar_8) {
						ieax = -ieax;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1) {
							sub_1002C8F0(var_4, ieax, var_30, var_34, var_38, var_3C, k, i, j);
						}
						else {
							int c, b, a;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									a = ieax * 2;
									c = -a;
									b = -ieax / 2;
								}
								else if (!(arg_14 - 2)) {
									a = ieax / 4 + ieax;
									b = -a;
									c = ieax * 4 - ieax / 2;
								}

							}
							else {
								a = ieax;
								b = ieax;
								c = ieax;
							}
							sub_100287E0(((unsigned char***)arg_4)[0], a, var_30, var_34, var_38, var_3C, k, i, j);
							sub_100287E0(((unsigned char***)arg_4)[1], b, var_30, var_34, var_38, var_3C, k, i, j);
							sub_100287E0(((unsigned char***)arg_4)[2], c, var_30, var_34, var_38, var_3C, k, i, j);
						}
					}

				}
			}
		}
	}
}
//2
void sub_100223F0(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28) {
	unsigned char** var_4 = ((unsigned char***)arg_4)[arg_14];
	int iarg_28 = arg_1C / (arg_28 + 1);
	if (iarg_28 < 1) {
		iarg_28 = 1;
	}
	int var_24;
	if (arg_28 > 0) {
		var_24 = arg_1C / iarg_28;
	}
	else
		var_24 = 0;
	unsigned char bvar_C = sub_1000AB10(arg_0, 5);
	int var1, var2, var3, var4;
	sub_10011870(arg_C, arg_10, 0, 2, &var3, &var1, &var4, &var2);
	for (int k = 0; k < 3; k++) {
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, 0, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] != 1) {
					unsigned char bvar_8 = sub_1000AB10(arg_0, 1);
					unsigned char ieax = sub_1000AB10(arg_0, bvar_C);
					int iecx;
					if (ieax < var_24)
					{
						if (arg_20 && ieax == 1) {
							iecx = arg_28;
						}
						else {
							iecx = iecx * arg_28 + arg_28 / 2;
						}
					}
					else {
						if (arg_20 && (ieax - var_24) == 1) {
							iecx = arg_1C;
						}
						else {
							iecx = iecx * arg_1C + arg_1C / 2;
						}
					}
					if (!arg_20) {
						iecx = iecx * arg_24 / 8 + iecx;
					}
					if (bvar_8) {
						iecx = -iecx;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1) {
							sub_1002C8F0(var_4, iecx, var1, var2, var3, var4, k, i, j);
						}
						else {
							int c, b, a;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									a = iecx * 2;
									c = -a;
									b = -iecx / 2;
								}
								else if (!(arg_14 - 2)) {
									a = iecx / 4 + iecx;
									b = -a;
									c = iecx * 4 - iecx / 2;
								}

							}
							else {
								a = iecx;
								b = iecx;
								c = iecx;
							}
							sub_100287E0(((unsigned char***)arg_4)[0], a, var1, var2, var3, var4, k, i, j);
							sub_100287E0(((unsigned char***)arg_4)[1], b, var1, var2, var3, var4, k, i, j);
							sub_100287E0(((unsigned char***)arg_4)[2], c, var1, var2, var3, var4, k, i, j);
						}
					}
				}
			}
		}
	}

}
//3 arg1c
void sub_100226D0(T_size_24 * arg_0, int*** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28) {
	unsigned char bvar_8 = sub_1000AB10(arg_0, 5);
	int var1, var2, var3, var4;
	sub_10011870(arg_C, arg_10, arg_1C, 2, &var3, &var1, &var4, &var2);
	for (int k = 0; k < 3; k++) {
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, arg_1C, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] != 1) {
					unsigned char bvar_4 = sub_1000AB10(arg_0, 1);
					unsigned char ieax = sub_1000AB10(arg_0, bvar_8);
					if (arg_24 && arg_1C < 2 && ieax == 1) {
						ieax = arg_20;
					}
					else {
						ieax = ieax * arg_20 + arg_20 / 2;
					}
					if (bvar_4) {
						ieax = -ieax;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1 || arg_28) {
							if (arg_14)ieax = ieax * 2;
							sub_10030730(((int ***)arg_4)[arg_14], ieax, var1, var2, var3, var4, k, i, j);
						}
						else {
							int c, b, a;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									a = ieax * 2;
									c = -a;
									b = -ieax / 2;
								}
								else if (!(arg_14 - 2)) {
									a = ieax / 4 + ieax;
									b = -a;
									c = ieax * 4 - ieax / 2;
								}

							}
							else {
								a = ieax;
								b = ieax;
								c = ieax;
							}
							sub_10030730(((int ***)arg_4)[0], a, var1, var2, var3, var4, k, i, j);
							sub_10030730(((int ***)arg_4)[1], b, var1, var2, var3, var4, k, i, j);
							sub_10030730(((int ***)arg_4)[2], c, var1, var2, var3, var4, k, i, j);
						}
					}

				}
			}
		}
	}
}
//4 arg1c
void sub_10022940(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C, int arg_30) {
	int iarg_2C = arg_1C / (arg_28 + 1);
	if (iarg_2C < 1) {
		iarg_2C = 1;
	}
	int var_20;
	if (arg_28 > 0) {
		var_20 = arg_1C / iarg_2C;
	}
	else
		var_20 = 0;
	unsigned char bvar_8 = sub_1000AB10(arg_0, 5);
	int var1, var2, var3, var4;
	sub_10011870(arg_C, arg_10, arg_1C, 2, &var3, &var1, &var4, &var2);
	for (int k = 0; k < 3; k++) {
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, arg_1C, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] != 1) {
					unsigned char bvar_4 = sub_1000AB10(arg_0, 1);
					unsigned char ieax = sub_1000AB10(arg_0, bvar_8);
					if (ieax < var_20)
					{
						if (arg_24 && arg_1C < 2 && ieax == 1) {
							ieax = arg_20;
						}
						else {
							ieax = ieax * arg_20 + arg_20 / 2;
						}
					}
					else {
						if (arg_24 && arg_1C < 2 && (ieax - var_20) == 1) {
							ieax = arg_20;
						}
						else {
							ieax = ieax * arg_20 + arg_20 / 2;
						}
					}
					if (arg_1C < 2 && !arg_24) {
						ieax = ieax * arg_28 / 8 + ieax;
					}
					if (bvar_4) {
						ieax = -ieax;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1 || arg_30) {
							if (arg_14)ieax = ieax * 2;
							sub_10030730(((int ***)arg_4)[arg_14], ieax, var1, var2, var3, var4, k, i, j);
						}
						else {
							int c, b, a;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									a = ieax * 2;
									c = -a;
									b = -ieax / 2;
								}
								else if (!(arg_14 - 2)) {
									a = ieax / 4 + ieax;
									b = -a;
									c = ieax * 4 - ieax / 2;
								}

							}
							else {
								a = ieax;
								b = ieax;
								c = ieax;
							}
							sub_10030730(arg_4[0], a, var1, var2, var3, var4, k, i, j);
							sub_10030730(arg_4[1], b, var1, var2, var3, var4, k, i, j);
							sub_10030730(arg_4[2], c, var1, var2, var3, var4, k, i, j);
						}
					}
				}
			}
		}
	}
}
//k
void sub_10022C50(T_size_24 * arg_0, int *** arg_4, unsigned char*** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C, int arg_30, int arg_34, int arg_38) {
	for (int i = arg_1C - 1; i >= 0; i--) {//arg_1C
		int *** pedi = arg_4;
		if (!i && arg_30 > 0) {
			int ** hesi;
			if (arg_38) {
				hesi = sub_1000E5C0(arg_10 / 2 + 1, arg_14 / 2 + 1);
			}
			else
				hesi = sub_1000E5C0(arg_10, arg_14);
			if (hesi) {
				sub_10019E80(*pedi, hesi, arg_10 / 2 + 1, arg_14 / 2 + 1, arg_30);
				sub_1000E600(*pedi);
				*pedi = hesi;
			}
		}
		if (!i && arg_38) {
			if (arg_18 == 1) {
				sub_10019B00(*pedi, arg_8[0], arg_10, arg_14);
			}
			else
				sub_100328F0(pedi, arg_8[0], arg_10, arg_14);
		}
		else {
			for (int j = arg_18; j > 0; j--, pedi++) {//esi
				sub_100194C0(*pedi, arg_10, arg_14, i);
			}
		}
		for (int j = 0; j < arg_18; j++) {//edi
										  //pesi = var_C; + 4
			if (!((unsigned char)arg_0->dw0x14 & 1)) {
				if (!i && arg_38 != i && arg_20[j] == i) {
					if (arg_28 <= 0 && arg_2C <= 0) {
						sub_10022E80(arg_0, arg_8, arg_C[j], arg_10, arg_14, j, arg_18, arg_24, arg_34);
					}
					else {
						sub_10023100(arg_0, arg_8, arg_C[j], arg_10, arg_14, j, arg_18, arg_24, arg_34, arg_2C, arg_28);
					}
				}
				else {
					if (i >= arg_20[j]) {
						if (arg_28 <= 0 && arg_2C <= 0) {
							sub_100233F0(arg_0, arg_4, arg_C[j], arg_10, arg_14, j, arg_18, i, arg_24, arg_34, arg_38);
						}
						else {
							sub_10023680(arg_0, arg_4, arg_C[j], arg_10, arg_14, j, arg_18, i, arg_24, arg_34, arg_2C, arg_28, arg_38);
						}
					}
				}
			}
		}
	}
}
//1
void sub_10022E80(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20) {
	unsigned char** var_4 = arg_4[arg_14];
	int var1, var2, var3, var4;
	sub_10011870(arg_C, arg_10, 0, 2, &var3, &var1, &var4, &var2);
	for (int k = 0; k < 3; k++) {
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, 0, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] != 1) {
					unsigned char bvar_8 = sub_1000AB10(arg_0, 1);
					int iedi, ieax;
					for (iedi = (arg_8[i][j] == 2); sub_1000ABC0(arg_0); iedi++);
					if (arg_20 && iedi == 1) {
						ieax = arg_1C;
					}
					else {
						ieax = ieax * arg_1C + arg_1C / 2;
					}
					if (bvar_8) {
						ieax = -ieax;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1) {
							sub_1002C8F0(var_4, ieax, var1, var2, var3, var4, k, i, j);
						}
						else {
							int c, b, a;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									a = ieax * 2;
									c = -a;
									b = -ieax / 2;
								}
								else if (!(arg_14 - 2)) {
									a = ieax / 4 + ieax;
									b = -a;
									c = ieax * 4 - ieax / 2;
								}

							}
							else {
								a = ieax;
								b = ieax;
								c = ieax;
							}
							sub_100287E0(arg_4[0], a, var1, var2, var3, var4, k, i, j);
							sub_100287E0(arg_4[1], b, var1, var2, var3, var4, k, i, j);
							sub_100287E0(arg_4[2], c, var1, var2, var3, var4, k, i, j);
						}
					}
				}
			}
		}
	}
}
//2
void sub_10023100(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28) {
	unsigned char** var_4 = arg_4[arg_14];
	int iarg_28 = arg_1C / (arg_28 + 1);
	if (iarg_28 < 1) {
		iarg_28 = 1;
	}
	int var_C;
	var_C = arg_1C / iarg_28;
	int var1, var2, var3, var4;
	sub_10011870(arg_C, arg_10, 0, 2, &var3, &var1, &var4, &var2);
	for (int k = 0; k < 3; k++) {
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, 0, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] != 1) {
					unsigned char bvar_8 = sub_1000AB10(arg_0, 1);
					int iesi;
					for (iesi = (arg_8[i][j] == 2); sub_1000ABC0(arg_0); iesi++);
					int ieax;
					if (iesi < var_C)
					{
						if (arg_20 && iesi == 1) {
							ieax = arg_1C;
						}
						else {
							ieax = iesi * arg_1C + arg_1C / 2;
						}
					}
					else {
						if (arg_20 && (iesi - var_C) == 1) {
							ieax = arg_1C;
						}
						else {
							ieax = iesi * arg_1C + arg_1C / 2;
						}
					}
					if (!arg_20) {
						ieax = ieax * arg_24 / 8 + ieax;
					}
					if (bvar_8) {
						ieax = -ieax;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1) {
							sub_1002C8F0(var_4, ieax, var1, var2, var3, var4, k, i, j);
						}
						else {
							int c, b, a;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									a = ieax * 2;
									c = -a;
									b = -ieax / 2;
								}
								else if (!(arg_14 - 2)) {
									a = ieax / 4 + ieax;
									b = -a;
									c = ieax * 4 - ieax / 2;
								}
							}
							else {
								a = ieax;
								b = ieax;
								c = ieax;
							}
							sub_100287E0(arg_4[0], a, var1, var2, var3, var4, k, i, j);
							sub_100287E0(arg_4[1], b, var1, var2, var3, var4, k, i, j);
							sub_100287E0(arg_4[2], c, var1, var2, var3, var4, k, i, j);
						}
					}
				}
			}
		}
	}
}
//3
void sub_100233F0(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28) {
	int var1, var2, var3, var4;
	sub_10011870(arg_C, arg_10, arg_1C, 2, &var3, &var1, &var4, &var2);
	for (int k = 0; k < 3; k++) {
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, arg_1C, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] != 1) {
					unsigned char bvar_4 = sub_1000AB10(arg_0, 1);
					int iedi, ieax;
					for (iedi = (arg_8[i][j] == 2); sub_1000ABC0(arg_0); iedi++);
					if (arg_24 && arg_1C < 2 && iedi == 1) {
						ieax = arg_20;
					}
					else {
						ieax = ieax * arg_20 + arg_20 / 2;
					}
					if (bvar_4) {
						ieax = -ieax;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1 || arg_28) {
							if (arg_14)ieax = ieax * 2;
							sub_10030730(arg_4[arg_14], ieax, var1, var2, var3, var4, k, i, j);
						}
						else {
							int c, b, a;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									a = ieax * 2;
									c = -a;
									b = -ieax / 2;
								}
								else if (!(arg_14 - 2)) {
									a = ieax / 4 + ieax;
									b = -a;
									c = ieax * 4 - ieax / 2;
								}

							}
							else {
								a = ieax;
								b = ieax;
								c = ieax;
							}
							sub_10030730(arg_4[0], a, var1, var2, var3, var4, k, i, j);
							sub_10030730(arg_4[1], b, var1, var2, var3, var4, k, i, j);
							sub_10030730(arg_4[2], c, var1, var2, var3, var4, k, i, j);
						}
					}
				}
			}
		}
	}
}
//4
void sub_10023680(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C, int arg_30) {
	int iarg_2C = arg_20 / (arg_2C + 1);
	if (iarg_2C < 1) {
		iarg_2C = 1;
	}
	int var_8;
	if (arg_28 > 0) {
		var_8 = arg_20 / iarg_2C;
	}
	else
		var_8 = 0;
	int var1, var2, var3, var4;
	sub_10011870(arg_C, arg_10, arg_1C, 2, &var3, &var1, &var4, &var2);
	for (int k = 0; k < 3; k++) {
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, arg_1C, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] != 1) {
					unsigned char bvar_4 = sub_1000AB10(arg_0, 1);
					int iesi, ieax;
					for (iesi = (arg_8[i][j] == 2); sub_1000ABC0(arg_0); iesi++);
					if (iesi < var_8)
					{
						if (arg_24 && arg_1C < 2 && iesi == 1) {
							ieax = arg_20;
						}
						else {
							ieax = iesi * arg_20 + arg_20 / 2;
						}
					}
					else {
						if (arg_24 && arg_1C < 2 && (iesi - var_8) == 1) {
							ieax = arg_20;
						}
						else {
							ieax = iesi * arg_20 + arg_20 / 2;
						}
					}
					if (arg_1C < 2 && !arg_24) {
						ieax = ieax * arg_28 / 8 + ieax;
					}
					if (bvar_4) {
						ieax = -ieax;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1 || arg_30) {
							if (arg_14)ieax = ieax * 2;
							sub_10030730(((int ***)arg_4)[arg_14], ieax, var1, var2, var3, var4, k, i, j);
						}
						else {
							int c, b, a;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									a = ieax * 2;
									c = -a;
									b = -ieax / 2;
								}
								else if (!(arg_14 - 2)) {
									a = ieax / 4 + ieax;
									b = -a;
									c = ieax * 4 - ieax / 2;
								}

							}
							else {
								a = ieax;
								b = ieax;
								c = ieax;
							}
							sub_10030730(arg_4[0], a, var1, var2, var3, var4, k, i, j);
							sub_10030730(arg_4[1], b, var1, var2, var3, var4, k, i, j);
							sub_10030730(arg_4[2], c, var1, var2, var3, var4, k, i, j);
						}
					}
				}
			}
		}
	}
}
//k
void sub_10023990(T_size_24 * arg_0, int *** arg_4, unsigned char*** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C, int arg_30, int arg_34, int arg_38) {
	for (int i = arg_1C - 1; i >= 0; i--) {//arg_1C
		int *** pedi = arg_4;
		if (!i && arg_30 > 0) {
			int ** hesi;
			if (arg_38) {
				hesi = sub_1000E5C0(arg_10 / 2 + 1, arg_14 / 2 + 1);
			}
			else
				hesi = sub_1000E5C0(arg_10, arg_14);
			if (hesi) {
				sub_10019E80(*pedi, hesi, arg_10 / 2 + 1, arg_14 / 2 + 1, arg_30);
				sub_1000E600(*pedi);
				*pedi = hesi;
			}
		}
		if (!i && arg_38) {
			if (arg_18 == 1) {
				sub_10019B00(*pedi, arg_8[0], arg_10, arg_14);
			}
			else
				sub_100328F0(pedi, arg_8[0], arg_10, arg_14);
		}
		else {
			for (int j = arg_18; j > 0; j--, pedi++) {
				sub_100194C0(*pedi, arg_10, arg_14, i);
			}
		}
		for (int j = 0; j < arg_18; j++) {
			//pesi = var_C; + 4
			if (!((unsigned char)arg_0->dw0x14 & 1)) {
				if (!i && arg_38 != i && arg_20[j] == i) {
					if (arg_28 <= 0 && arg_2C <= 0) {
						sub_10023BC0(arg_0, arg_8, arg_C[j], arg_10, arg_14, j, arg_18, arg_24, arg_34);
					}
					else {
						sub_10023E90(arg_0, arg_8, arg_C[j], arg_10, arg_14, j, arg_18, arg_24, arg_34, arg_2C, arg_28);
					}
				}
				else {
					if (i >= arg_20[j]) {
						if (arg_28 <= 0 && arg_2C <= 0) {
							sub_10024200(arg_0, arg_4, arg_C[j], arg_10, arg_14, j, arg_18, i, arg_24, arg_34, arg_38);
						}
						else {
							sub_100244F0(arg_0, arg_4, arg_C[j], arg_10, arg_14, j, arg_18, i, arg_24, arg_34, arg_2C, arg_28, arg_38);
						}
					}
				}
			}
		}
	}
}
//1
void sub_10023BC0(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20) {
	unsigned char** var_18 = arg_4[arg_14];
	int var1, var2, var3, var4;
	unsigned char bvar_20 = sub_1000AB10(arg_0, 5);
	if (bvar_20 > 0xA) {
		arg_0->dw0x14 = 1;
		return;
	}
	T_size_14 var_14;
	sub_100280B0(&var_14, 0x800, 1);
	sub_10011870(arg_C, arg_10, 0, 2, &var3, &var1, &var4, &var2);
	for (int k = 0; k < 3; k++) {
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, 0, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] != 1) {
					unsigned char bvar_1C = sub_1000AB10(arg_0, 1);
					int ieax = sub_100282C0(&var_14, arg_0, bvar_20);
					if (arg_8[i][j] == 2)
					{
						ieax++;
					}
					if (arg_20 && ieax == 1) {
						ieax = arg_1C;
					}
					else {
						ieax = ieax * arg_1C + arg_1C / 2;
					}
					if (bvar_1C) {
						ieax = -ieax;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1) {
							sub_1002C8F0(var_18, ieax, var1, var2, var3, var4, k, i, j);
						}
						else {
							int c, b, a;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									a = ieax * 2;
									c = -a;
									b = -ieax / 2;
								}
								else if (!(arg_14 - 2)) {
									a = ieax / 4 + ieax;
									b = -a;
									c = ieax * 4 - ieax / 2;
								}

							}
							else {
								a = ieax;
								b = ieax;
								c = ieax;
							}
							sub_100287E0(arg_4[0], a, var1, var2, var3, var4, k, i, j);
							sub_100287E0(arg_4[1], b, var1, var2, var3, var4, k, i, j);
							sub_100287E0(arg_4[2], c, var1, var2, var3, var4, k, i, j);
						}
					}
				}
			}
		}
	}
	sub_10028140(&var_14);
}
//2
void sub_10023E90(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28) {
	unsigned char** var_18 = arg_4[arg_14];
	int iarg_28 = arg_1C / (arg_28 + 1);
	if (iarg_28 < 1) {
		iarg_28 = 1;
	}
	int var_38;
	if (arg_28 > 0) {
		var_38 = arg_1C / iarg_28;
	}
	else
		var_38 = 0;
	unsigned char bvar_20 = sub_1000AB10(arg_0, 5);
	if (bvar_20 > 0xA) {
		arg_0->dw0x14 = 1;
		return;
	}
	T_size_14 var_14;
	sub_100280B0(&var_14, 0x800, 1);
	int var1, var2, var3, var4;
	sub_10011870(arg_C, arg_10, 0, 2, &var3, &var1, &var4, &var2);
	for (int k = 0; k < 3; k++) {
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, 0, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] != 1) {
					unsigned char bvar_1C = sub_1000AB10(arg_0, 1);
					int ieax = sub_100282C0(&var_14, arg_0, bvar_20);
					if (arg_8[i][j] == 2)
					{
						ieax++;
					}
					int iecx;
					if (ieax < var_38) {
						if (arg_20 && ieax == 1) {
							iecx = arg_28;
						}
						else {
							iecx = ieax * arg_28 + arg_28 / 2;
						}
					}
					else {
						if (arg_20 && (ieax - var_38) == 1) {
							iecx = arg_1C;
						}
						else
							iecx = ieax * arg_1C + arg_1C / 2;
					}
					if (!arg_20) {
						iecx = iecx * arg_28 / 8 + iecx;
					}
					if (bvar_1C) {
						iecx = -iecx;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1) {
							sub_1002C8F0(var_18, ieax, var1, var2, var3, var4, k, i, j);
						}
						else {
							int c, b, a;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									a = ieax * 2;
									c = -a;
									b = -ieax / 2;
								}
								else if (!(arg_14 - 2)) {
									a = ieax / 4 + ieax;
									b = -a;
									c = ieax * 4 - ieax / 2;
								}
							}
							else {
								a = ieax;
								b = ieax;
								c = ieax;
							}
							sub_100287E0(arg_4[0], a, var1, var2, var3, var4, k, i, j);
							sub_100287E0(arg_4[1], b, var1, var2, var3, var4, k, i, j);
							sub_100287E0(arg_4[2], c, var1, var2, var3, var4, k, i, j);
						}
					}
				}
			}
		}
	}
	sub_10028140(&var_14);
}
//3
void sub_10024200(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28) {
	int var1, var2, var3, var4;
	unsigned char bvar_1C = sub_1000AB10(arg_0, 5);
	if (bvar_1C > 0xA) {
		arg_0->dw0x14 = 1;
		return;
	}
	T_size_14 var_14;
	sub_100280B0(&var_14, 0x800, 1);
	sub_10011870(arg_C, arg_10, arg_1C, 2, &var3, &var1, &var4, &var2);
	for (int k = 0; k < 3; k++) {
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, arg_1C, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] != 1) {
					unsigned char bvar_18 = sub_1000AB10(arg_0, 1);
					int ieax = sub_100282C0(&var_14, arg_0, bvar_1C);
					if (arg_8[i][j] == 2)
					{
						ieax++;
					}
					if (arg_24 && arg_1C < 2 && ieax == 1) {
						ieax = arg_20;
					}
					else {
						ieax = ieax * arg_20 + arg_20 / 2;
					}
					if (bvar_18) {
						ieax = -ieax;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1 || arg_28) {
							if (arg_14)ieax = ieax * 2;
							sub_10030730(arg_4[arg_14], ieax, var1, var2, var3, var4, k, i, j);
						}
						else {
							int c, b, a;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									a = ieax * 2;
									c = -a;
									b = -ieax / 2;
								}
								else if (!(arg_14 - 2)) {
									a = ieax / 4 + ieax;
									b = -a;
									c = ieax * 4 - ieax / 2;
								}

							}
							else {
								a = ieax;
								b = ieax;
								c = ieax;
							}
							sub_10030730(arg_4[0], a, var1, var2, var3, var4, k, i, j);
							sub_10030730(arg_4[1], b, var1, var2, var3, var4, k, i, j);
							sub_10030730(arg_4[2], c, var1, var2, var3, var4, k, i, j);
						}
					}
				}
			}
		}
	}
	sub_10028140(&var_14);
}
//4
void sub_100244F0(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C, int arg_30) {
	int iarg_2C = arg_20 / (arg_2C + 1);
	if (iarg_2C < 1) {
		iarg_2C = 1;
	}
	int var_34;
	if (arg_28 > 0) {
		var_34 = arg_20 / iarg_2C;
	}
	else
		var_34 = 0;
	unsigned char bvar_1C = sub_1000AB10(arg_0, 5);
	if (bvar_1C >= 0xF) {
		arg_0->dw0x14 = 1;
		return;
	}
	T_size_14 var_14;
	sub_100280B0(&var_14, 0x800, 1);
	int var1, var2, var3, var4;
	sub_10011870(arg_C, arg_10, arg_1C, 2, &var3, &var1, &var4, &var2);
	for (int k = 0; k < 3; k++) {
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, arg_1C, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] != 1) {
					unsigned char bvar_18 = sub_1000AB10(arg_0, 1);
					int ieax = sub_100282C0(&var_14, arg_0, bvar_1C);
					if (arg_8[i][j] == 2)
					{
						ieax++;
					}
					if (ieax < var_34)
					{
						if (arg_24 && arg_1C < 2 && ieax == 1) {
							ieax = arg_2C;
						}
						else {
							ieax = ieax * arg_2C + arg_2C / 2;
						}
					}
					else {
						if (arg_24 && arg_1C < 2 && (ieax - var_34) == 1) {
							ieax = arg_20;
						}
						else {
							ieax = ieax * arg_20 + arg_20 / 2;
						}
					}
					if (arg_1C < 2 && !arg_24) {
						ieax = ieax * arg_28 / 8 + ieax;
					}
					if (bvar_18) {
						ieax = -ieax;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1 || arg_30) {
							if (arg_14)ieax = ieax * 2;
							sub_10030730(arg_4[arg_14], ieax, var1, var2, var3, var4, k, i, j);
						}
						else {
							int c, b, a;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									a = ieax * 2;
									c = -a;
									b = -ieax / 2;
								}
								else if (!(arg_14 - 2)) {
									a = ieax / 4 + ieax;
									b = -a;
									c = ieax * 4 - ieax / 2;
								}

							}
							else {
								a = ieax;
								b = ieax;
								c = ieax;
							}
							sub_10030730(arg_4[0], a, var1, var2, var3, var4, k, i, j);
							sub_10030730(arg_4[1], b, var1, var2, var3, var4, k, i, j);
							sub_10030730(arg_4[2], c, var1, var2, var3, var4, k, i, j);
						}
					}
				}
			}
		}
	}
	sub_10028140(&var_14);
}
//1
void sub_10024890(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int * arg_1C, int arg_20, int arg_24) {
	int var_4C, var_44, var_48, var_50;
	unsigned char** var_20 = arg_4[arg_14];
	sub_10011870(arg_C, arg_10, 0, 2, &var_4C, &var_44, &var_50, &var_48);
	if (arg_1C[0] <= 0)
	{
		unsigned char var_1C = sub_1000AB10(arg_0, 5);//hmem?
		T_size_14 var_14;
		if (!var_1C)
		{
			arg_1C[0] = var_1C;
			if (var_1C < 0) return;
			//loc_10024901
		}
		else {
			sub_100280B0(&var_14, 0x800, 1);
			for (int iebx = 0; iebx < 3; iebx++) {
				int var_2C, var_3C, var_30, var_40;
				sub_10011870(arg_C, arg_10, 0, iebx, &var_2C, &var_3C, &var_30, &var_40);
				for (int iedi = var_2C; iedi < var_3C; iedi++) {
					for (int iesi = var_30; iesi < var_40; iesi++) {
						if (arg_8[iedi][iesi] & 6) {
							unsigned char bvar_18 = sub_1000AB10(arg_0, 1);
							int ieax = sub_100282C0(&var_14, arg_0, var_1C);
							if (arg_8[iedi][iesi] & 2)
							{
								ieax++;
							}
							if (arg_24 && ieax == 1) {
								ieax = arg_20;
							}
							else {
								ieax = ieax * arg_20 + arg_20 / 2;
							}
							//loc_10024CF1
							if (bvar_18) {
								ieax = -ieax;
							}
							if (!((unsigned char)arg_0->dw0x14 & 1)) {
								if (arg_18 == 1) {
									sub_1002C8F0(var_20, ieax, var_44, var_48, var_4C, var_50, iebx, iedi, iesi);
								}
								else {
									int var_34, var_38, iebp;
									if (arg_14) {
										if (!(arg_14 - 1)) {
											var_34 = -ieax * 2;
											var_38 = -ieax / 2;
											iebp = ieax * 2;
										}
										else if (!(arg_14 - 2)) {
											iebp = ieax / 4 + ieax;
											var_38 = -iebp;
											var_34 = ieax * 4 - ieax / 2;
										}

									}
									else {
										iebp = ieax;
										var_38 = ieax;
										var_34 = ieax;
									}
									sub_100287E0(arg_4[0], iebp, var_44, var_48, var_4C, var_50, iebx, iedi, iesi);
									sub_100287E0(arg_4[1], var_38, var_44, var_48, var_4C, var_50, iebx, iedi, iesi);
									sub_100287E0(arg_4[2], var_34, var_44, var_48, var_4C, var_50, iebx, iedi, iesi);
								}
							}
						}
					}
				}
			}
			sub_10028140(&var_14);
			return;
		}
	}
	int i;
	//loc_10024901
	for (i = 0; i < 3; i++) {//arg_1C
		int var_2C, var_3C, var_30, var_40;
		sub_10011870(arg_C, arg_10, 0, i, &var_2C, &var_3C, &var_30, &var_40);
		for (int iebp = var_2C; iebp < var_3C; iebp++) {
			//&arg_8[iebp] + 4
			for (int iedi = var_30; iedi < var_40; iedi++) {
				//esi = &arg_8[var_2C][iedi] + 1
				if (arg_8[iebp][iedi] & 6) {
					unsigned char al = sub_1000AB10(arg_0, 1);
					arg_8[iebp][iedi] += al;
				}
			}
		}
		//loc_10024997
	}
	T_size_31C* var_1C = sub_10031B10();
	sub_10031D80(0, arg_0, var_1C);
	for (i = 0; i < 3; i++) {//arg_1C
		int var_2C, var_3C, var_30, var_40;
		sub_10011870(arg_C, arg_10, 0, i, &var_2C, &var_3C, &var_30, &var_40);
		for (int iedi = var_2C; iedi < var_3C; iedi++) {
			for (int iesi = var_30; iesi < var_40; iesi++) {
				if (arg_8[iedi][iesi] != 1) {
					int ieax = sub_10031DB0(arg_0, var_1C);
					if (arg_8[iedi][iesi] == 3 || arg_8[iedi][iesi] == 2) {
						ieax++;
					}
					if (arg_24 && ieax == 1) {
						ieax = arg_20;
					}
					else {
						ieax = ieax * arg_20 + arg_20 / 2;
					}
					if (arg_8[iedi][iesi] == 5 || arg_8[iedi][iesi] == 3) {
						ieax = -ieax;
					}
					//loc_10024A8E
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1) {
							sub_1002C8F0(var_20, ieax, var_44, var_48, var_4C, var_50, i, iedi, iesi);
						}
						else {
							int var_34, var_38, iebp;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									var_34 = -ieax * 2;
									var_38 = -ieax / 2;
									iebp = ieax * 2;
								}
								else if (!(arg_14 - 2)) {
									iebp = ieax / 4 + ieax;
									var_38 = -iebp;
									var_34 = ieax * 4 - ieax / 2;
								}

							}
							else {
								iebp = ieax;
								var_38 = ieax;
								var_34 = ieax;
							}
							sub_100287E0(arg_4[0], iebp, var_44, var_48, var_4C, var_50, i, iedi, iesi);
							sub_100287E0(arg_4[1], var_38, var_44, var_48, var_4C, var_50, i, iedi, iesi);
							sub_100287E0(arg_4[2], var_34, var_44, var_48, var_4C, var_50, i, iedi, iesi);
						}
					}
					//loc_10024BBA
				}
			}
		}
	}
	sub_10031C60(arg_0, var_1C);
}
//2
void sub_10024E50(T_size_24 * arg_0, unsigned char *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int * arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C) {
	unsigned char** var_1C = arg_4[arg_14];
	int var_2C = arg_20 / (arg_2C + 1);
	if (var_2C < 1) {
		var_2C = 1;
	}
	int var_20;
	if (arg_2C > 0) {
		var_20 = arg_20 / var_2C;
	}
	else
		var_20 = 0;
	int var_50, var_48, var_54, var_4C;
	sub_10011870(arg_C, arg_10, 0, 2, &var_50, &var_48, &var_54, &var_4C);
	if (arg_1C[0] <= 0) {
		unsigned char bvar_18 = sub_1000AB10(arg_0, 5);
		if (!bvar_18) {
			arg_1C[0] = bvar_18;
		}
		else {
			//loc_10025286
			T_size_14 var_14;
			sub_100280B0(&var_14, 0x800, 1);
			for (int iarg_2C = 0; iarg_2C < 3; iarg_2C++) {
				int var_34, var_38, var_40, var_44;
				sub_10011870(arg_C, arg_10, 0, iarg_2C, &var_34, &var_40, &var_38, &var_44);
				for (int iedi = var_34; iedi < var_40; iedi++) {
					for (int iesi = var_38; iesi < var_44; iesi++) {
						if (arg_8[iedi][iesi] & 6) {
							unsigned char bvar_28 = sub_1000AB10(arg_0, 1);
							int ieax = sub_100282C0(&var_14, arg_0, bvar_18);
							if (arg_8[iedi][iesi] & 2)
							{
								ieax++;
							}
							int iecx;
							if (ieax < var_20) {
								if (arg_24 && ieax == 1) {
									iecx = var_2C;
								}
								else {
									iecx = ieax * var_2C + var_2C / 2;
								}
							}
							else {
								if (arg_24 && (ieax - var_20) == 1) {
									iecx = arg_20;
								}
								else
									iecx = ieax * arg_20 + arg_20 / 2;
							}
							//loc_100253A6
							if (!arg_24) {
								iecx = iecx * arg_28 / 8 + iecx;
							}
							if (bvar_28) {
								iecx = -iecx;
							}
							//loc_100253C3
							if (!((unsigned char)arg_0->dw0x14 & 1)) {
								if (arg_18 == 1) {
									sub_1002C8F0(var_1C, iecx, var_48, var_4C, var_50, var_54, iarg_2C, iedi, iesi);
								}
								else {
									int var_3C, iarg_1C, iebp;
									if (arg_14) {
										if (!(arg_14 - 1)) {
											var_3C = -iecx * 2;
											iarg_1C = -iecx / 2;
											iebp = iecx * 2;
										}
										else if (!(arg_14 - 2)) {
											iebp = iecx / 4 + iecx;
											iarg_1C = -iebp;
											var_3C = iecx * 4 - iecx / 2;
										}

									}
									else {
										iebp = iecx;
										var_3C = iecx;
										iarg_1C = iecx;
									}
									sub_100287E0(arg_4[0], iebp, var_48, var_4C, var_50, var_54, iarg_2C, iedi, iesi);
									sub_100287E0(arg_4[1], iarg_1C, var_48, var_4C, var_50, var_54, iarg_2C, iedi, iesi);
									sub_100287E0(arg_4[2], var_3C, var_48, var_4C, var_50, var_54, iarg_2C, iedi, iesi);
								}
							}
						}
					}
				}
			}
			sub_10028140(&var_14);
			return;
		}
	}
	if (arg_1C[0] < 0)return;
	for (int iarg_2C = 0; iarg_2C < 3; iarg_2C++) {
		int var_34, var_38, var_40, var_44;
		sub_10011870(arg_C, arg_10, 0, iarg_2C, &var_34, &var_40, &var_38, &var_44);
		for (int iarg_1C = var_34; iarg_1C < var_40; iarg_1C++) {
			for (int iedi = var_38; iedi < var_44; iedi++) {
				if (arg_8[iarg_1C][iedi] & 6) {
					unsigned char al = sub_1000AB10(arg_0, 1);
					arg_8[iarg_1C][iedi] += al;
				}
			}
		}
	}
	T_size_31C*  var_24 = sub_10031B10();
	sub_10031D80(0, arg_0, var_24);
	for (int iebx = 0; iebx < 3; iebx++) {
		int var_34, var_38, var_40, var_44;
		sub_10011870(arg_C, arg_10, 0, iebx, &var_34, &var_40, &var_38, &var_44);
		for (int iedi = var_34; iedi < var_40; iedi++) {
			for (int iesi = var_38; iesi < var_44; iesi++) {
				if (arg_8[iedi][iesi] != 1) {
					int ieax = sub_10031DB0(arg_0, var_24);
					if (arg_8[iedi][iesi] == 3 || arg_8[iedi][iesi] == 2) {
						ieax++;
					}
					int iecx;
					if (ieax < var_20) {
						if (arg_24 && ieax == 1) {
							iecx = var_2C;
						}
						else {
							iecx = ieax * var_2C + var_2C / 2;
						}
					}
					else {
						if (arg_24 && (ieax - var_20) == 1) {
							iecx = arg_20;
						}
						else
							iecx = ieax * arg_20 + arg_20 / 2;
					}
					//loc_100250EA
					if (!arg_24) {
						iecx = iecx * arg_28 / 8 + iecx;
					}
					if (arg_8[iedi][iesi] == 5 || arg_8[iedi][iesi] == 3) {
						iecx = -iecx;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1) {
							sub_1002C8F0(var_1C, iecx, var_48, var_4C, var_50, var_54, iebx, iedi, iesi);
						}
						else {
							int var_3C, iarg_1C, iebp;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									var_3C = -iecx * 2;
									iarg_1C = -iecx / 2;
									iebp = iecx * 2;
								}
								else if (!(arg_14 - 2)) {
									iebp = iecx / 4 + iecx;
									iarg_1C = -iebp;
									var_3C = iecx * 4 - iecx / 2;
								}

							}
							else {
								iebp = iecx;
								var_3C = iecx;
								iarg_1C = iecx;
							}
							sub_100287E0(arg_4[0], iebp, var_48, var_4C, var_50, var_54, iebx, iedi, iesi);
							sub_100287E0(arg_4[1], iarg_1C, var_48, var_4C, var_50, var_54, iebx, iedi, iesi);
							sub_100287E0(arg_4[2], var_3C, var_48, var_4C, var_50, var_54, iebx, iedi, iesi);
						}
					}
					//loc_10024BBA
				}
			}
		}
	}
	sub_10031C60(arg_0, var_24);
}
//3
void sub_10025560(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C) {
	int var1, var2, var3, var4;
	sub_10011870(arg_C, arg_10, arg_1C, 2, &var3, &var1, &var4, &var2);
	if (arg_1C >= *arg_20) {
		unsigned char bvar_1C = sub_1000AB10(arg_0, 5);
		if (!bvar_1C) {
			arg_20[0] = arg_1C;
			//loc_100255C6
		}
		else {
			T_size_14 var_14;
			sub_100280B0(&var_14, 0x800, 1);
			for (int k = 0; k < 3; k++) {//arg_20
				int s1, e1, s2, e2;
				sub_10011870(arg_C, arg_10, arg_1C, k, &s1, &e1, &s2, &e2);
				for (int i = s1; i < e1; i++) {
					for (int j = s2; j < e2; j++) {
						if (arg_8[i][j] & 6) {
							unsigned char bvar_18 = sub_1000AB10(arg_0, 1);
							int ieax = sub_100282C0(&var_14, arg_0, bvar_1C);
							if (arg_8[i][j] & 2)
							{
								ieax++;
							}
							if (arg_28 && arg_1C < 2 && ieax == 1) {
								ieax = arg_24;
							}
							else {
								ieax = ieax * arg_24 + arg_24 / 2;
							}
							//loc_100259E0
							if (bvar_18) {
								ieax = -ieax;
							}
							if (!((unsigned char)arg_0->dw0x14 & 1)) {
								if (arg_18 == 1 || arg_2C) {
									if (arg_14)ieax = ieax * 2;
									sub_10030730(arg_4[arg_14], ieax, var1, var2, var3, var4, k, i, j);
								}
								else {
									int c, b, a;
									if (arg_14) {
										if (!(arg_14 - 1)) {
											a = ieax * 2;
											c = -a;
											b = -ieax / 2;
										}
										else if (!(arg_14 - 2)) {
											a = ieax / 4 + ieax;
											b = -a;
											c = ieax * 4 - ieax / 2;
										}
									}
									else {
										a = ieax;
										b = ieax;
										c = ieax;
									}
									sub_10030730(arg_4[0], a, var1, var2, var3, var4, k, i, j);
									sub_10030730(arg_4[1], b, var1, var2, var3, var4, k, i, j);
									sub_10030730(arg_4[2], c, var1, var2, var3, var4, k, i, j);
								}
							}
						}
					}
				}
			}
			sub_10028140(&var_14);
			return;
		}

	}
	//loc_100255C6
	int k;
	for (k = 0; k < 3; k++) {//arg_20
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, arg_1C, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] & 6) {
					unsigned char al = sub_1000AB10(arg_0, 1);
					arg_8[i][j] += al;
				}
			}
		}
	}
	T_size_31C*  var_1C = sub_10031B10();
	sub_10031D80(0, arg_0, var_1C);
	for (k = 0; k < 3; k++) {//arg_20
		int s1, e1, s2, e2;
		sub_10011870(arg_C, arg_10, 0, k, &s1, &e1, &s2, &e2);
		for (int i = s1; i < e1; i++) {
			for (int j = s2; j < e2; j++) {
				if (arg_8[i][j] != 1) {
					int ieax = sub_10031DB0(arg_0, var_1C);
					if (arg_8[i][j] == 3 || arg_8[i][j] == 2) {
						ieax++;
					}
					if (arg_28 && arg_1C < 2 && ieax == 1) {
						ieax = arg_24;
					}
					else {
						ieax = ieax * arg_24 + arg_24 / 2;
					}

					if (arg_8[i][j] == 5 || arg_8[i][j] == 3) {
						ieax = -ieax;
					}
					//loc_10025767
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1 || arg_2C) {
							if (arg_14)ieax = ieax * 2;
							sub_10030730(arg_4[arg_14], ieax, var1, var2, var3, var4, k, i, j);
						}
						else {
							int c, b, a;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									a = ieax * 2;
									c = -a;
									b = -ieax / 2;
								}
								else if (!(arg_14 - 2)) {
									a = ieax / 4 + ieax;
									b = -a;
									c = ieax * 4 - ieax / 2;
								}

							}
							else {
								a = ieax;
								b = ieax;
								c = ieax;
							}
							sub_10030730(arg_4[0], a, var1, var2, var3, var4, k, i, j);
							sub_10030730(arg_4[1], b, var1, var2, var3, var4, k, i, j);
							sub_10030730(arg_4[2], c, var1, var2, var3, var4, k, i, j);
						}
					}
				}
			}
		}
	}
	sub_10031C60(arg_0, var_1C);
}
//4
void sub_10025B80(T_size_24 * arg_0, int *** arg_4, unsigned char ** arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C, int arg_30, int arg_34) {
	int var_28 = arg_24 / (arg_30 + 1);
	if (var_28 < 1) {
		var_28 = 1;
	}
	int var_20;
	if (arg_30 > 0) {
		var_20 = arg_24 / var_28;
	}
	else
		var_20 = 0;
	int var_4C, var_44, var_50, var_48;
	sub_10011870(arg_C, arg_10, arg_1C, 2, &var_4C, &var_44, &var_50, &var_48);
	if (arg_1C >= arg_20[0]) {
		unsigned char bvar_1C = sub_1000AB10(arg_0, 5);
		if (!bvar_1C) {
			arg_20[0] = arg_1C;
		}
		else {
			//loc_10026000
			T_size_14 var_14;
			sub_100280B0(&var_14, 0x800, 1);
			for (int iarg_30 = 0; iarg_30 < 3; iarg_30++) {
				int var_30, var_34, var_3C, var_40;
				sub_10011870(arg_C, arg_10, arg_1C, iarg_30, &var_30, &var_3C, &var_34, &var_40);
				for (int iedi = var_30; iedi < var_3C; iedi++) {
					for (int iesi = var_34; iesi < var_40; iesi++) {
						if (arg_8[iedi][iesi] & 6) {
							unsigned char bvar_18 = sub_1000AB10(arg_0, 1);
							int ieax = sub_100282C0(&var_14, arg_0, bvar_1C);
							if (arg_8[iedi][iesi] & 2)
							{
								ieax++;
							}
							int iecx;
							if (ieax < var_20) {
								if (arg_28 && arg_1C < 2 && ieax == 1) {
									iecx = var_28;
								}
								else {
									iecx = ieax * var_28 + var_28 / 2;
								}
							}
							else {
								if (arg_28 && arg_1C < 2 && (ieax - var_20) == 1) {
									iecx = arg_24;
								}
								else
									iecx = ieax * arg_24 + arg_24 / 2;
							}
							//loc_1002613F
							if (arg_1C < 2 && !arg_28) {
								iecx = iecx * arg_2C / 8 + iecx;
							}
							if (bvar_18) {
								iecx = -iecx;
							}
							//loc_10026166
							if (!((unsigned char)arg_0->dw0x14 & 1)) {
								if (arg_18 == 1 || arg_34) {
									if (arg_14)iecx = iecx * 2;
									sub_10030730(arg_4[arg_14], iecx, var_44, var_48, var_4C, var_50, iarg_30, iedi, iesi);
								}
								else {
									int var_38, iarg_20, iebp;
									if (arg_14) {
										if (!(arg_14 - 1)) {
											var_38 = -iecx * 2;
											iarg_20 = -iecx / 2;
											iebp = iecx * 2;
										}
										else if (!(arg_14 - 2)) {
											iebp = iecx / 4 + iecx;
											iarg_20 = -iebp;
											var_38 = iecx * 4 - iecx / 2;
										}

									}
									else {
										iebp = iecx;
										var_38 = iecx;
										iarg_20 = iecx;
									}
									sub_10030730(arg_4[0], iebp, var_40, var_48, var_4C, var_50, iarg_30, iedi, iesi);
									sub_10030730(arg_4[1], iarg_20, var_40, var_48, var_4C, var_50, iarg_30, iedi, iesi);
									sub_10030730(arg_4[2], var_38, var_40, var_48, var_4C, var_50, iarg_30, iedi, iesi);
								}
							}
						}
					}
				}
			}
			sub_10028140(&var_14);
			return;
		}
	}
	if (arg_20[0] < arg_1C)return;
	int k;
	for (k = 0; k < 3; k++) {//arg_30
		int var_30, var_34, var_3C, var_40;
		sub_10011870(arg_C, arg_10, arg_1C, k, &var_30, &var_3C, &var_34, &var_40);
		for (int i = var_30; i < var_3C; i++) {
			for (int j = var_34; j < var_40; j++) {
				if (arg_8[i][j] & 6) {
					unsigned char al = sub_1000AB10(arg_0, 1);
					arg_8[i][j] += al;
				}
			}
		}
	}
	T_size_31C* var_1C = sub_10031B10();
	sub_10031D80(0, arg_0, var_1C);
	for (k = 0; k < 3; k++) {//arg_30
		int var_34, var_38, var_40, var_44;
		sub_10011870(arg_C, arg_10, arg_1C, k, &var_34, &var_40, &var_38, &var_44);
		for (int i = var_34; i < var_40; i++) {
			for (int j = var_38; j < var_44; j++) {
				if (arg_8[i][j] != 1) {
					int ieax = sub_10031DB0(arg_0, var_1C);
					if (arg_8[i][j] == 3 || arg_8[i][j] == 2) {
						ieax++;
					}
					int iecx;
					if (ieax < var_20) {
						if (arg_28 && arg_1C < 2 && ieax == 1) {
							iecx = var_28;
						}
						else {
							iecx = ieax * var_28 + var_28 / 2;
						}
					}
					else {
						if (arg_28 && arg_1C < 2 && (ieax - var_20) == 1) {
							iecx = arg_24;
						}
						else
							iecx = ieax * arg_24 + arg_24 / 2;
					}
					//loc_10025E2C
					if (arg_1C < 2 && !arg_28) {
						iecx = iecx * arg_2C / 8 + iecx;
					}
					if (arg_8[i][j] == 5 || arg_8[i][j] == 3) {
						iecx = -iecx;
					}
					if (!((unsigned char)arg_0->dw0x14 & 1)) {
						if (arg_18 == 1 || arg_34) {
							if (arg_14)iecx = iecx * 2;
							sub_10030730(arg_4[arg_14], iecx, var_44, var_48, var_4C, var_50, k, i, j);
						}
						else {
							int var_38, iarg_20, iebp;
							if (arg_14) {
								if (!(arg_14 - 1)) {
									var_38 = -iecx * 2;
									iarg_20 = -iecx / 2;
									iebp = iecx * 2;
								}
								else if (!(arg_14 - 2)) {
									iebp = iecx / 4 + iecx;
									iarg_20 = -iebp;
									var_38 = iecx * 4 - iecx / 2;
								}

							}
							else {
								iebp = iecx;
								var_38 = iecx;
								iarg_20 = iecx;
							}
							sub_10030730(arg_4[0], iebp, var_40, var_48, var_4C, var_50, k, i, j);
							sub_10030730(arg_4[1], iarg_20, var_40, var_48, var_4C, var_50, k, i, j);
							sub_10030730(arg_4[2], var_38, var_40, var_48, var_4C, var_50, k, i, j);
						}
					}
				}
			}
		}
	}
	sub_10031C60(arg_0, var_1C);
}
//k
void sub_10026310(T_size_24 * arg_0, int *** arg_4, unsigned char*** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int * arg_20, int arg_24, int arg_28, int arg_2C, int arg_30, int arg_34, int arg_38) {
	int var_C[3] = { 0 };
	for (int i = arg_1C - 1; i >= 0; i--) {//arg_1C
		int *** pedi = arg_4;
		if (!i && arg_30 > 0) {
			int** hesi;
			if (arg_38) {
				hesi = sub_1000E5C0(arg_10 / 2 + 1, arg_14 / 2 + 1);
			}
			else
				hesi = sub_1000E5C0(arg_10, arg_14);
			if (hesi) {
				sub_10019E80(*pedi, hesi, arg_10 / 2 + 1, arg_14 / 2 + 1, arg_30);
				sub_1000E600(*pedi);
				*pedi = hesi;
			}
		}
		if (!i && arg_38) {
			if (arg_18 == 1) {
				sub_10019B00(*pedi, *arg_8, arg_10, arg_14);
			}
			else
				sub_100328F0(pedi, *arg_8, arg_10, arg_14);
			//loc_10026408
		}
		else {
			for (int j = arg_18; j > 0; j--, pedi++) {
				sub_100194C0(*pedi, arg_10, arg_14, i);
			}
		}
		//loc_10026408
		for (int j = 0; j < arg_18; j++) {//edi
										  //pesi = var_C; + 4
			if (!((unsigned char)arg_0->dw0x14 & 1)) {
				if (!i && arg_38 != i && arg_20[j] == i) {
					if (arg_28 <= 0 && arg_2C <= 0) {
						sub_10024890(arg_0, arg_8, arg_C[j], arg_10, arg_14, j, arg_18, &var_C[j], arg_24, arg_34);
					}
					else {
						sub_10024E50(arg_0, arg_8, arg_C[j], arg_10, arg_14, j, arg_18, &var_C[j], arg_24, arg_34, arg_2C, arg_28);
					}
				}
				//loc_100264CC
				else {
					if (i >= arg_20[j]) {
						if (arg_28 <= 0 && arg_2C <= 0) {
							sub_10025560(arg_0, arg_4, arg_C[j], arg_10, arg_14, j, arg_18, i, &var_C[j], arg_24, arg_34, arg_38);
						}
						else {
							sub_10025B80(arg_0, arg_4, arg_C[j], arg_10, arg_14, j, arg_18, i, &var_C[j], arg_24, arg_34, arg_2C, arg_28, arg_38);
						}
					}
				}
			}
		}
	}
}
//k
void sub_100265F0(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, int arg_C, int arg_10, int arg_14, int arg_18) {
	T_size_24 * pesi = arg_0;
	T_size_2C * peax = arg_8;
	int ieax = sub_10028360(&peax->t_08, arg_0);
	for (int i = arg_C; i < arg_10; i++) {//ebx
		for (int j = arg_14; j < arg_18; j++) {//edx
			if (!ieax) {
				peax->hg0x24(arg_0, arg_4, arg_C, arg_14);
				sub_10028360(&peax->t_08, arg_0);
			}
			else
				ieax--;
		}
	}
	if (ieax) {
		pesi->dw0x14 = 1;
	}
}
//k
void sub_10026580(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, int arg_C, int arg_10, int arg_14, int arg_18) {
	T_size_2C * pesi = arg_8;
	if (!arg_14) {
		if (pesi->dw0x00)
		{
			pesi->hg0x24 = pesi->hg0x28;
		}
	}
	int iarg_8, iarg_18, iarg_14, iarg_10;
	sub_10011870(arg_C, arg_10, arg_14, arg_18, &iarg_10, &iarg_18, &iarg_8, &iarg_14);
	sub_100265F0(arg_0, arg_4, pesi, iarg_10, iarg_18, iarg_8, iarg_14);
}
//k
void sub_10026670(T_size_2C * arg_0, int arg_4, int arg_8, int arg_C, int(*arg_10)(T_size_24*, T_size_178*, int, int), int(*arg_14)(T_size_24*, T_size_178*, int, int), int arg_18) {
	T_size_2C * pesi = arg_0;
	sub_100280B0(&pesi->t_08, 0x100, 0);
	pesi->hg0x24 = arg_10;
	pesi->hg0x28 = arg_14;
	pesi->dw0x00 = arg_18;
}
//k
void sub_100266A0(T_size_2C * arg_0) {
	T_size_2C * peax = arg_0;
	sub_10028140(&peax->t_08);
}
//k
void sub_100266C0(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, int arg_C, int arg_10, int arg_14, int arg_18) {
	T_size_2C * pesi = arg_8;
	if (!arg_14 && pesi->dw0x00) {
		pesi->hg0x24 = pesi->hg0x28;
	}
	int iarg_10, iarg_18, iarg_8, iarg_14;
	sub_10011870(arg_C, arg_10, arg_14, arg_18, &iarg_10, &iarg_18, &iarg_8, &iarg_14);
	pesi->dw0x04 = sub_10028360(&pesi->t_08, arg_0);
	sub_10026750(arg_0, arg_4, pesi, 0, iarg_10, iarg_18, iarg_8, iarg_14);
	if (pesi->dw0x04) {
		arg_0->dw0x14 = 1;//14 15 16 17
	}

}
void sub_10026750(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C) {
	T_size_2C * pesi = arg_8;
	int iedx = (arg_14 - arg_10) * (arg_1C - arg_18);
	int iedi = arg_10 + (arg_14 - arg_10) / 2;
	int iebx = arg_18 + (arg_1C - arg_18) / 2;
	int ieax = arg_1C - arg_18;
	int iecx = arg_14 - arg_10;
	T_size_24 * pebp = arg_0;
	for (; pesi->dw0x04 < iedx;) {
		if (ieax > 2 && iecx > 2)
		{
			switch (iedx) {
			case 0:
				sub_10026750(pebp, arg_4, pesi, 3, arg_10, iedi, iebx, arg_1C);
				sub_10026750(pebp, arg_4, pesi, 0, iedi, arg_14, iebx, arg_1C);
				sub_10026750(pebp, arg_4, pesi, 0, iedi, arg_14, arg_18, iebx);
				arg_1C = iebx;
				arg_14 = iedi;
				arg_C = 2;
				//loc_10026958
				break;
			case 1:
				sub_10026750(pebp, arg_4, pesi, 2, iedi, arg_14, arg_18, iebx);
				sub_10026750(pebp, arg_4, pesi, 1, arg_10, iedi, arg_18, iebx);
				sub_10026750(pebp, arg_4, pesi, 1, arg_10, iedi, iebx, arg_1C);
				arg_18 = iebx;
				arg_10 = iedi;
				arg_C = 3;
				break;
			case 2:
				sub_10026750(pebp, arg_4, pesi, 1, iedi, arg_14, arg_18, iebx);
				sub_10026750(pebp, arg_4, pesi, 2, iedi, arg_14, iebx, arg_1C);
				sub_10026750(pebp, arg_4, pesi, 2, arg_10, iedi, iebx, arg_1C);
				arg_1C = iebx;
				arg_14 = iedi;
				arg_C = 0;
				break;
			case 3:
				sub_10026750(pebp, arg_4, pesi, 0, arg_10, iedi, iebx, arg_1C);
				sub_10026750(pebp, arg_4, pesi, 3, arg_10, iedi, arg_18, iebx);
				sub_10026750(pebp, arg_4, pesi, 3, iedi, arg_14, arg_18, iebx);
				arg_18 = iebx;
				arg_10 = iedi;
				arg_C = 1;
				break;
			default:
				return;
			}
			//loc_10026958
			iecx = arg_14 - arg_10;
			ieax = arg_1C - arg_18;
			iedx = iecx * ieax;
			iebx = arg_18 + ieax / 2;
			iedi = arg_10 + iecx / 2;
			continue;
		}
		//loc_10026992
		else {
			switch (iedx) {
				int ieax, i, j;
			case 0:
				ieax = 1;
				for (i = arg_1C - 1; i >= arg_18; i--) {
					if (ieax == 1) {
						for (j = arg_10; j < arg_14; j++) {
							if (!pesi->dw0x04) {
								pesi->hg0x24(pebp, arg_4, j, i);
								ieax = sub_10028360(&pesi->t_08, pebp);
							}
							else
								ieax--;
							pesi->dw0x04 = ieax;
						}
						ieax = 0;
					}
					else {
						for (j = arg_14 - 1; j >= arg_10; j--) {
							if (!pesi->dw0x04) {
								pesi->hg0x24(pebp, arg_4, j, i);
								ieax = sub_10028360(&pesi->t_08, pebp);
							}
							else
								ieax--;
							pesi->dw0x04 = ieax;
						}
						ieax = 1;
					}
					//loc_10026A45
				}
				return;
			case 1:
				ieax = 0;
				for (i = arg_18; i < arg_1C; i++) {
					if (ieax == 1) {
						for (j = arg_10; j < arg_14; j++) {
							if (!pesi->dw0x04) {
								pesi->hg0x24(pebp, arg_4, j, i);
								ieax = sub_10028360(&pesi->t_08, pebp);
							}
							else
								ieax--;
							pesi->dw0x04 = ieax;
						}
						ieax = 0;
					}
					else {
						for (j = arg_14 - 1; j >= arg_10; j--) {
							if (!pesi->dw0x04) {
								pesi->hg0x24(pebp, arg_4, j, i);
								ieax = sub_10028360(&pesi->t_08, pebp);
							}
							else
								ieax--;
							pesi->dw0x04 = ieax;
						}
						ieax = 1;
					}
					//loc_10026AF4
				}
				return;
			case 2:
				ieax = 1;
				for (i = arg_14 - 1; i >= arg_10; i--) {
					if (ieax == 1) {
						for (int j = arg_18; j < arg_1C; j++) {
							if (!pesi->dw0x04) {
								pesi->hg0x24(pebp, arg_4, i, j);
								ieax = sub_10028360(&pesi->t_08, pebp);
							}
							else
								ieax--;
							pesi->dw0x04 = ieax;
						}
						ieax = 0;
					}
					else {
						for (j = arg_1C - 1; j >= arg_18; j--) {
							if (!pesi->dw0x04) {
								pesi->hg0x24(pebp, arg_4, i, j);
								ieax = sub_10028360(&pesi->t_08, pebp);
							}
							else
								ieax--;
							pesi->dw0x04 = ieax;
						}
						ieax = 1;
					}
					//loc_10026BA9
				}
				return;
			case 3:
				ieax = 0;
				for (i = arg_10; i < arg_14; i++) {
					if (ieax == 1) {
						for (j = arg_18; j < arg_1C; j++) {
							if (!pesi->dw0x04) {
								pesi->hg0x24(pebp, arg_4, i, j);
								ieax = sub_10028360(&pesi->t_08, pebp);
							}
							else
								ieax--;
							pesi->dw0x04 = ieax;
						}
						ieax = 0;
					}
					else {
						for (j = arg_1C - 1; j >= arg_18; j--) {
							if (!pesi->dw0x04) {
								pesi->hg0x24(pebp, arg_4, i, j);
								ieax = sub_10028360(&pesi->t_08, pebp);
							}
							else
								ieax--;
							pesi->dw0x04 = ieax;
						}
						ieax = 1;
					}
					//loc_10026C58
				}
				return;
			default:
				return;
			}
		}
	}
	pesi->dw0x04 -= iedx;


}
//k
void sub_10026CB0(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, int arg_C, int arg_10, int arg_14, int arg_18) {
	T_size_24 * pedi = arg_0;
	T_size_2C * pesi = arg_8;
	if ((!(unsigned char)pedi->dw0x14 & 1) && !arg_14 && pesi->dw0x00)
	{
		pesi->hg0x24 = pesi->hg0x28;
	}
	int iarg_18, iarg_8, iarg_14, iarg_0;
	sub_10011870(arg_C, arg_10, arg_14, arg_18, &iarg_18, &iarg_8, &iarg_14, &iarg_0);
	if (sub_1000AB10(pedi, 1)) {
		sub_10026D40(pedi, arg_4, pesi, iarg_18, iarg_8, iarg_14, iarg_0);
	}

}
void sub_10026D40(T_size_24 * arg_0, T_size_178 * arg_4, T_size_2C * arg_8, int arg_C, int arg_10, int arg_14, int arg_18) {
	int iesi = arg_10 - arg_C;
	int ieax = arg_18 - arg_14;
	T_size_24 * pebp = arg_0;
	int iebx = arg_C;
	int iarg_18, iarg_14, iarg_10;
	for (; ieax > 2; ieax = iarg_18 - iarg_14, iesi = iarg_10 - iebx) {
		if (iesi > 2)
			break;
		ieax = (ieax / 2) + arg_14;//iedi
		iesi = (iesi / 2) + iebx;
		sub_1000AB10(pebp, 1);
		if (ieax)
		{
			unsigned char barg_C = sub_1000AB10(pebp, 1);
			barg_C *= 2;
			barg_C |= sub_1000AB10(pebp, 1);
			switch (barg_C) {
			case 0:
				//loc_10026F6E
				iarg_18 = ieax;
				iarg_10 = iesi;
				break;
			case 1:
				iarg_14 = ieax;
				iarg_10 = iesi;
				break;
			case 2:
				iarg_18 = ieax;
				iebx = iesi;
				break;
			case 3:
				iarg_14 = ieax;
				iebx = iesi;
				break;
			default:return;
			}
			//loc_10027016
		}
		else {
			if (sub_1000AB10(pebp, 1)) {
				if (sub_1000AB10(pebp, 1))
				{
					unsigned char barg_C = sub_1000AB10(pebp, 1);
					barg_C *= 2;
					barg_C |= sub_1000AB10(pebp, 1);
					switch (barg_C) {
					case 0:
						//loc_10026F9A
						sub_10026D40(pebp, arg_4, arg_8, iebx, iesi, iarg_14, ieax);
						iarg_14 = ieax;
						iarg_10 = iesi;
						break;
					case 1:
						sub_10026D40(pebp, arg_4, arg_8, iesi, iarg_10, iarg_14, ieax);
						iarg_14 = ieax;
						iebx = iesi;
						break;
					case 2:
						sub_10026D40(pebp, arg_4, arg_8, iebx, iesi, iarg_14, ieax);
						iarg_18 = ieax;
						iebx = iesi;
						break;
					case 3:
						sub_10026D40(pebp, arg_4, arg_8, iebx, iesi, ieax, iarg_18);
						iarg_14 = ieax;
						iebx = iesi;
						break;
					default:return;
					}
				}
				else {
					//loc_10026E33
					if (sub_1000AB10(pebp, 1)) {
						sub_10026D40(pebp, arg_4, arg_8, iebx, iesi, ieax, iarg_18);
						iarg_18 = ieax;
						iebx = iesi;
					}
					else {
						//loc_10026E63
						sub_10026D40(pebp, arg_4, arg_8, iebx, iesi, iarg_14, ieax);
						iarg_14 = ieax;
						iebx = iesi;
					}
				}
			}
			else {
				if (sub_1000AB10(pebp, 1))
				{
					unsigned char barg_C = sub_1000AB10(pebp, 1);
					barg_C *= 2;
					barg_C |= sub_1000AB10(pebp, 1);
					switch (barg_C) {
					case 0:
						sub_10026D40(pebp, arg_4, arg_8, iebx, iesi, iarg_14, ieax);
						sub_10026D40(pebp, arg_4, arg_8, iebx, iesi, ieax, iarg_18);
						iarg_14 = ieax;
						iarg_10 = iesi;
						break;
					case 1:
						sub_10026D40(pebp, arg_4, arg_8, iebx, iesi, ieax, iarg_18);
						sub_10026D40(pebp, arg_4, arg_8, iesi, iarg_10, ieax, iarg_18);
						iarg_18 = ieax;
						iebx = iesi;
						break;
					case 2:
						sub_10026D40(pebp, arg_4, arg_8, iesi, iarg_10, ieax, iarg_18);
						sub_10026D40(pebp, arg_4, arg_8, iesi, iarg_10, iarg_14, ieax);
						iarg_18 = ieax;
						iarg_10 = iesi;
						break;
					case 3:
						sub_10026D40(pebp, arg_4, arg_8, iesi, iarg_10, iarg_14, ieax);
						sub_10026D40(pebp, arg_4, arg_8, iebx, iesi, iarg_14, ieax);
						iarg_14 = ieax;
						iarg_10 = iesi;
						break;
					default:return;
					}
				}
				else {
					//loc_10026FBF
					sub_10026D40(pebp, arg_4, arg_8, iebx, iesi, iarg_14, ieax);
					sub_10026D40(pebp, arg_4, arg_8, iebx, iesi, ieax, iarg_18);
					sub_10026D40(pebp, arg_4, arg_8, iesi, iarg_10, iarg_14, ieax);
					iarg_14 = ieax;
					iebx = iesi;
				}
			}
		}
		//loc_10027016
	}
	//loc_10027033
	iesi = iesi * ieax - 1;
	if (iesi == 3) {
		unsigned char barg_C;
		if (sub_1000AB10(pebp, 1))
		{
			if (sub_1000AB10(pebp, 1))
				barg_C = 2 - (sub_1000AB10(pebp, 1) != 0);
			else
				barg_C = sub_1000AB10(pebp, 1) != 0 ? 4 : 8;
		}
		else if (sub_1000AB10(pebp, 1))
		{
			if (sub_1000AB10(pebp, 1))
			{
				if (sub_1000AB10(pebp, 1))
					barg_C = sub_1000AB10(pebp, 1) != 0 ? 9 : 3;
				else
					barg_C = sub_1000AB10(pebp, 1) != 0 ? 6 : 12;
			}
			else
			{
				barg_C = sub_1000AB10(pebp, 1) != 0 ? 5 : 10;
			}
		}
		else if (sub_1000AB10(pebp, 1))
		{
			if (sub_1000AB10(pebp, 1))
				barg_C = sub_1000AB10(pebp, 1) != 0 ? 13 : 11;
			else
				barg_C = sub_1000AB10(pebp, 1) != 0 ? 7 : 14;
		}
		else
		{
			barg_C = 15;
		}

		unsigned char al = 0x10;
		for (int i = iebx; i < arg_10; i++) {//ebx
			for (int j = arg_14; j < arg_18; j++) {//iesi
				al = al / 2;
				if (barg_C & al) {
					arg_8->hg0x24(pebp, arg_4, i, j);
				}
			}
		}
		return;
	}
	int cnt = 0;//edi
	for (int i = iebx; i < arg_10; i++) {//ebx
		for (int j = arg_14; j < arg_18; j++) {//esi
			if (cnt != iesi && !sub_1000AB10(pebp, 1)) {//arg_C
				cnt++;
			}
			else {
				arg_8->hg0x24(pebp, arg_4, i, j);
			}
		}
	}

}
//k
void sub_10026C90(T_size_2C * arg_0, int arg_4, int arg_8, int arg_C, int(*arg_10)(T_size_24*, T_size_178*, int, int), int(*arg_14)(T_size_24*, T_size_178*, int, int), int arg_18) {
	T_size_2C * peax = (T_size_2C *)arg_0;
	peax->hg0x24 = arg_10;
	peax->hg0x28 = arg_14;
	peax->dw0x00 = arg_18;
}
//k
void sub_10027230(T_size_24 * arg_0, T_size_178 * pesi, int *** arg_8, unsigned char*** arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20, int arg_24, int arg_28, int arg_2C) {
	sub_100280B0(&pesi->t_38, 0x100, 1);
	pesi->hg0x04 = arg_8;
	pesi->hg0x00 = *arg_8;
	if (arg_C) {
		pesi->hg0x0C = arg_C;
		pesi->hg0x08 = *arg_C;
	}
	pesi->dw0x10 = arg_10;
	pesi->dw0x14 = arg_14;
	pesi->dw0x34 = arg_1C;
	pesi->dw0x30 = arg_18;
	pesi->dw0x60 = arg_24;
	pesi->dw0x68 = arg_2C;
	pesi->dw0x54 = arg_20;
	pesi->dw0x64 = arg_28;
	pesi->dw0x58 = arg_20 / (arg_28 + 1);
	if (pesi->dw0x58 < 1)
		pesi->dw0x58 = 1;
	if (arg_28 > 0) {
		pesi->dw0x5C = arg_20 / pesi->dw0x58;
	}
	else
		pesi->dw0x5C = 0;
}
//OK
void sub_100272D0(T_size_178 * arg_0) {
	sub_10028140(&arg_0->t_38);
}
//k
int sub_100272F0(T_size_24* arg_0, T_size_178 * arg_4, int arg_8, int arg_C)
{
	T_size_24 * pedi = arg_0;
	T_size_178 * pesi = (T_size_178 *)arg_4;
	unsigned char bebx = sub_1000AB10(pedi, 1);
	int ieax = sub_10028360(&pesi->t_38, pedi);
	ieax++;
	if (pesi->dw0x68 && ieax == 1) {
		ieax = pesi->dw0x54;
	}
	else {
		ieax = pesi->dw0x54 * ieax + pesi->dw0x54 / 2;
	}
	if (bebx) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_1002C8F0(pesi->hg0x08, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int b, a, c;
	if (!pesi->dw0x30) {
		a = ieax;
		b = ieax;
		c = ieax;
	}
	else if (!(pesi->dw0x30 - 1)) {
		a = ieax * 2;
		b = -(ieax / 2);
		c = -(2 * ieax);
	}
	else if (!(pesi->dw0x30 - 2)) {
		b = -(ieax / 4 + ieax);
		c = ieax * 4 - (ieax / 2);
		a = ieax / 4 + ieax;
	}
	else {
		a = arg_8;
		b = arg_8;
		c = arg_8;
	}

	sub_100287E0(pesi->hg0x0C[0], a, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[1], b, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[2], c, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10027600(T_size_24* arg_0, T_size_178 * arg_4, int arg_8, int arg_C)
{
	T_size_24 * pedi = arg_0;
	T_size_178 * pesi = (T_size_178 *)arg_4;
	unsigned char bebx = sub_1000AB10(pedi, 1);
	int ieax = sub_10028360(&pesi->t_38, pedi);
	ieax++;
	if (pesi->dw0x68 && pesi->dw0x18 < 2 && ieax == 1) {
		ieax = pesi->dw0x54;
	}
	else {
		ieax = pesi->dw0x54 * ieax + pesi->dw0x54 / 2;
	}
	if (bebx) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_10030730(pesi->hg0x00, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int iebp, iecx, iedi;
	if (pesi->dw0x30) {
		if (!(pesi->dw0x30 - 1)) {
			iecx = ieax * 2;
			iebp = -(ieax / 2);
			iedi = -(2 * ieax);
		}
		else if (!(pesi->dw0x30 - 2)) {
			iebp = -(ieax / 4 + ieax);
			iedi = ieax * 4 - (ieax / 2);
			iecx = ieax / 4 + ieax;
		}
		else {
			iecx = arg_8;
			iebp = arg_8;
			iedi = arg_8;
		}
	}
	else {
		iecx = ieax;
		iebp = ieax;
		iedi = ieax;
	}
	//loc_100276E0
	sub_10030730(pesi->hg0x04[0], iecx, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[1], iebp, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[2], iedi, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10027460(T_size_24* arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_24 * pebx = arg_0;
	T_size_178 * pesi = (T_size_178 *)arg_4;
	unsigned char barg_0 = sub_1000AB10(pebx, 1);
	int ieax = sub_10028360(&pesi->t_38, pebx);
	ieax++;
	if (ieax < pesi->dw0x5C) {
		if (pesi->dw0x68 && ieax == 1) {
			ieax = pesi->dw0x58;
		}
		else {
			ieax = pesi->dw0x58 * ieax + pesi->dw0x58 / 2;
		}
	}
	else {
		if (pesi->dw0x68 && (ieax - pesi->dw0x5C) == 1) {
			ieax = pesi->dw0x54;
		}
		else {
			ieax = pesi->dw0x54 * (ieax - pesi->dw0x5C) + pesi->dw0x54 / 2;
		}
	}
	if (!pesi->dw0x68) {
		ieax += pesi->dw0x60 * ieax / 8;
	}
	if (barg_0) {
		ieax = -ieax;
	}
	if ((unsigned char)pebx->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_1002C8F0(pesi->hg0x08, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int iebp, iecx, iedi;
	if (pesi->dw0x30) {
		if (!(pesi->dw0x30 - 1)) {
			iecx = ieax * 2;
			iebp = -((2 * ieax) / 2);
			iedi = -(2 * ieax);
		}
		else if (!(pesi->dw0x30 - 2)) {
			iecx = (ieax / 4 + ieax);
			iebp = -(ieax / 4 + ieax);
			iedi = ieax * 4 - (ieax / 2);
		}
		else {
			iecx = arg_8;
			iebp = arg_8;
			iedi = arg_8;
		}
	}
	else {
		iecx = ieax;
		iebp = ieax;
		iedi = ieax;
	}
	sub_100287E0(((unsigned char***)pesi->hg0x0C)[0], iecx, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(((unsigned char***)pesi->hg0x0C)[1], iebp, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(((unsigned char***)pesi->hg0x0C)[2], iedi, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10027770(T_size_24* arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_24 * pebx = arg_0;
	T_size_178 * pesi = (T_size_178 *)arg_4;
	unsigned char barg_0 = sub_1000AB10(pebx, 1);
	int ieax = sub_10028360(&pesi->t_38, pebx);
	ieax++;
	if (ieax < pesi->dw0x5C) {
		if (pesi->dw0x68 && pesi->dw0x18 < 2 && ieax == 1) {
			ieax = pesi->dw0x58;
		}
		else {
			ieax = pesi->dw0x58 * ieax + pesi->dw0x58 / 2;
		}
	}
	else {
		if (pesi->dw0x68 && pesi->dw0x18 < 2 && (ieax - pesi->dw0x5C) == 1) {
			ieax = pesi->dw0x54;
		}
		else {
			ieax = pesi->dw0x54 * (ieax - pesi->dw0x5C) + pesi->dw0x54 / 2;
		}
	}
	if (pesi->dw0x18 < 2 && !pesi->dw0x68) {
		ieax += pesi->dw0x60 * ieax / 8;
	}
	if (barg_0) {
		ieax = -ieax;
	}
	if ((unsigned char)pebx->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_10030730(pesi->hg0x00, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int iebp, iecx, iedi;
	if (pesi->dw0x30) {
		if (!(pesi->dw0x30 - 1)) {
			iecx = ieax * 2;
			iebp = -((2 * ieax) / 2);
			iedi = -(2 * ieax);
		}
		else if (!(pesi->dw0x30 - 2)) {
			iecx = (ieax / 4 + ieax);
			iebp = -(ieax / 4 + ieax);
			iedi = ieax * 4 - (ieax / 2);
		}
		else {
			iecx = arg_8;
			iebp = arg_8;
			iedi = arg_8;
		}
	}
	else {
		iecx = ieax;
		iebp = ieax;
		iedi = ieax;
	}
	sub_10030730(pesi->hg0x04[0], iecx, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[1], iebp, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[2], iedi, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10027930(T_size_24* arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_24 * pedi = arg_0;
	T_size_178 * pesi = (T_size_178 *)arg_4;
	unsigned char bebx = sub_1000AB10(pedi, 1);
	int ieax = sub_10027F50(pedi);
	ieax++;
	if (pesi->dw0x68 && ieax == 1) {
		ieax = pesi->dw0x54;
	}
	else {
		ieax = pesi->dw0x54 * ieax + pesi->dw0x54 / 2;
	}
	if (bebx) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_1002C8F0(pesi->hg0x08, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int iebp, iecx, iedi;
	if (pesi->dw0x30) {
		if (!(pesi->dw0x30 - 1)) {
			iecx = ieax * 2;
			iebp = -(ieax / 2);
			iedi = -(2 * ieax);
		}
		else if (!(pesi->dw0x30 - 2)) {
			iebp = -(ieax / 4 + ieax);
			iedi = ieax * 4 - (ieax / 2);
			iecx = ieax / 4 + ieax;
		}
		else {
			iecx = arg_8;
			iebp = arg_8;
			iedi = arg_8;
		}
	}
	else {
		iecx = ieax;
		iebp = ieax;
		iedi = ieax;
	}
	//loc_10027A07
	sub_100287E0(pesi->hg0x0C[0], iecx, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[1], iebp, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[2], iedi, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10027C30(T_size_24* arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_24 * pedi = arg_0;
	T_size_178 * pesi = arg_4;
	unsigned char bebx = sub_1000AB10(pedi, 1);
	int ieax = sub_10027F50(pedi);
	ieax++;
	if (pesi->dw0x68 && pesi->dw0x18 < 2 && ieax == 1) {
		ieax = pesi->dw0x54;
	}
	else {
		ieax = pesi->dw0x54 * ieax + pesi->dw0x54 / 2;
	}
	if (bebx) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	//loc_10027C87
	if (pesi->dw0x34 == 1) {
		sub_10030730(pesi->hg0x00, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int iebp, iecx, iedi;
	if (pesi->dw0x30) {
		if (!(pesi->dw0x30 - 1)) {
			iecx = ieax * 2;
			iebp = -((2 * ieax) / 2);
			iedi = -(2 * ieax);
		}
		else if (!(pesi->dw0x30 - 2)) {
			iecx = (ieax / 4 + ieax);
			iebp = -(ieax / 4 + ieax);
			iedi = ieax * 4 - (ieax / 2);
		}
		else {
			iecx = arg_8;
			iebp = arg_8;
			iedi = arg_8;
		}
	}
	else {
		iecx = ieax;
		iebp = ieax;
		iedi = ieax;
	}
	sub_10030730(pesi->hg0x04[0], iecx, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[1], iebp, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[2], iedi, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10027AA0(T_size_24* arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_24 * pebx = arg_0;
	T_size_178 * pesi = arg_4;
	unsigned char barg_0 = sub_1000AB10(pebx, 1);
	int ieax = sub_10027F50(pebx);
	ieax++;
	if (ieax < pesi->dw0x5C) {
		if (pesi->dw0x68 && ieax == 1) {
			ieax = pesi->dw0x58;
		}
		else {
			ieax = pesi->dw0x58 * ieax + pesi->dw0x58 / 2;
		}
	}
	else {
		if (pesi->dw0x68 && (ieax - pesi->dw0x5C) == 1) {
			ieax = pesi->dw0x54;
		}
		else {
			ieax = pesi->dw0x54 * (ieax - pesi->dw0x5C) + pesi->dw0x54 / 2;
		}
	}
	if (!pesi->dw0x68) {
		ieax += pesi->dw0x60 * ieax / 8;
	}
	if (barg_0) {
		ieax = -ieax;
	}
	if ((unsigned char)pebx->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_1002C8F0(pesi->hg0x08, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int iebp, iecx, iedi;
	if (pesi->dw0x30) {
		if (!(pesi->dw0x30 - 1)) {
			iecx = ieax * 2;
			iebp = -((2 * ieax) / 2);
			iedi = -(2 * ieax);
		}
		else if (!(pesi->dw0x30 - 2)) {
			iecx = (ieax / 4 + ieax);
			iebp = -(ieax / 4 + ieax);
			iedi = ieax * 4 - (ieax / 2);
		}
		else {
			iecx = arg_8;
			iebp = arg_8;
			iedi = arg_8;
		}
	}
	else {
		iecx = ieax;
		iebp = ieax;
		iedi = ieax;
	}
	sub_100287E0(pesi->hg0x0C[0], iecx, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[1], iebp, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_100287E0(pesi->hg0x0C[2], iedi, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10027DA0(T_size_24* arg_0, T_size_178 * arg_4, int arg_8, int arg_C) {
	T_size_24 * pedi = arg_0;
	T_size_178 * pesi = (T_size_178 *)arg_4;
	unsigned char barg_0 = sub_1000AB10(pedi, 1);
	int ieax = sub_10027F50(pedi);
	ieax++;
	if (ieax < pesi->dw0x5C) {
		if (pesi->dw0x68 && pesi->dw0x18 < 2 && ieax == 1) {
			ieax = pesi->dw0x58;
		}
		else {
			ieax = pesi->dw0x58 * ieax + pesi->dw0x58 / 2;
		}
	}
	else {
		if (pesi->dw0x68 && pesi->dw0x18 < 2 && (ieax - pesi->dw0x5C) == 1) {
			ieax = pesi->dw0x54;
		}
		else {
			ieax = pesi->dw0x54 * (ieax - pesi->dw0x5C) + pesi->dw0x54 / 2;
		}
	}
	if (pesi->dw0x18 < 2 && !pesi->dw0x68) {
		ieax += pesi->dw0x60 * ieax / 8;
	}
	if (barg_0) {
		ieax = -ieax;
	}
	if ((unsigned char)pedi->dw0x14 & 1)
		return 0;
	if (pesi->dw0x34 == 1) {
		sub_10030730(pesi->hg0x00, ieax, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
		return 1;
	}
	int iebp, iecx, iedi;
	if (pesi->dw0x30) {
		if (!(pesi->dw0x30 - 1)) {
			iecx = ieax * 2;
			iebp = -((2 * ieax) / 2);
			iedi = -(2 * ieax);
		}
		else if (!(pesi->dw0x30 - 2)) {
			iecx = (ieax / 4 + ieax);
			iebp = -(ieax / 4 + ieax);
			iedi = ieax * 4 - (ieax / 2);
		}
		else {
			iecx = arg_8;
			iebp = arg_8;
			iedi = arg_8;
		}
	}
	else {
		iecx = ieax;
		iebp = ieax;
		iedi = ieax;
	}
	sub_10030730(pesi->hg0x04[0], iecx, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[1], iebp, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	sub_10030730(pesi->hg0x04[2], iedi, pesi->dw0x28, pesi->dw0x2C, pesi->dw0x20, pesi->dw0x24, pesi->dw0x1C, arg_8, arg_C);
	return 1;
}
//k
int sub_10027F50(T_size_24* arg_0) {
	T_size_24 * pedi = arg_0;
	if (!sub_1000ABC0(arg_0)) return 0;
	int i;
	for (i = 1; sub_1000ABC0(pedi); i++);//esi
	i *= 2;
	if (sub_1000ABC0(pedi)) {
		i--;
	}
	return i;
}
//k
int sub_10027FA0(T_size_14 * arg_0, int arg_4) {
	T_size_14 * pesi = (T_size_14 *)arg_0;
	pesi->dw0x00 = arg_4;
	pesi->hg0x0C = (unsigned short*)AllocateMemorySize(arg_4 * 3 * 4 + 18);
	pesi->hg0x10 = (int*)AllocateMemorySize(arg_4 * 8 + 12);
	pesi->hg0x04 = (unsigned short*)AllocateMemorySize(arg_4 * 2 + 4);
	if (pesi->hg0x0C && pesi->hg0x10 && pesi->hg0x04) {
		pesi->hg0x0C[2] = 1;
		pesi->hg0x0C[1] = 0;
		pesi->hg0x10[0] = 2;
		pesi->hg0x0C[0] = 0xFFFF;
		pesi->hg0x0C[5] = pesi->dw0x00;//di?unsigned short
		pesi->hg0x0C[4] = 1;
		pesi->hg0x10[1] = 1;
		pesi->hg0x0C[3] = 0;
		unsigned short* pwedx_04 = (unsigned short*)pesi->hg0x04;
		pesi->hg0x04[pesi->dw0x00] = 1;
		pesi->hg0x0C[8] = (arg_4 + 1);//?unsigned short
		pesi->hg0x0C[7] = 1;
		pesi->hg0x10[2] = 1;
		pesi->hg0x0C[6] = 0;
		pesi->hg0x04[arg_4 + 1] = 2;
		pesi->dw0x08 = 3;
		for (int i = 0; i < pesi->dw0x00; i++) {//ecx
			pwedx_04[i] = 0xFFFF;
		}
		return 1;
	}
	else {
		sub_10028140(arg_0);
		return 0;
	}

}
//k
int sub_100280B0(T_size_14 * arg_0, int arg_4, int arg_8) {
	if (!sub_10027FA0(arg_0, arg_4))
		return 0;
	if (arg_8 == 1)
	{
		sub_10028700(arg_0, 0);
		sub_100283A0(arg_0, 0);
		sub_10028700(arg_0, 1);
		sub_100283A0(arg_0, 1);
		sub_10028700(arg_0, 2);
		sub_100283A0(arg_0, 2);
		return 1;
	}
	for (int i = 0; i < 0x40; i++) {//esi
		sub_10028700(arg_0, i);
		sub_100283A0(arg_0, i);
	}
	return 1;
}
//k
void sub_10028140(T_size_14 * arg_0) {

	T_size_14 * pesi = (T_size_14 *)arg_0;
	if (pesi->hg0x0C)
		DeallocateMemory(pesi->hg0x0C);
	if (pesi->hg0x10)
		DeallocateMemory(pesi->hg0x10);
	if (pesi->hg0x04)
		DeallocateMemory(pesi->hg0x04);
}
//k
int sub_100282C0(T_size_14 * arg_0, T_size_24 *arg_4, int arg_8) {
	T_size_14 * pedi = arg_0;
	T_size_24 * pebp = arg_4;
	int iebx = pedi->dw0x00 + 1;
	int i = 0, k;
	for (i = 0; pedi->hg0x0C[i * 3 + 1] % 256 == 0;) {//esi
		i = pedi->hg0x0C[i * 3 + 2] + sub_1000ABC0(pebp);
		if ((unsigned char)pebp->dw0x14 & 1) {
			return 0;
		}
	}
	i = pedi->hg0x0C[i * 3 + 2];
	if (i == iebx) {
		i = sub_1000AB10(pebp, arg_8);
		if ((unsigned char)pebp->dw0x14 & 1)
			return 0;
		if (i >= pedi->dw0x00) {
			sub_100283A0(pedi, iebx);
			return i;
		}
		sub_10028700(pedi, i);
	}
	sub_100283A0(pedi, i);
	return i;
}
//k
int sub_10028360(T_size_14 * arg_0, T_size_24 *arg_4) {
	int result = sub_100282C0(arg_0, arg_4, 8);
	if (result >= 255)
	{
		int iesi = sub_10028360(arg_0, arg_4);
		return (iesi << 8) + sub_100282C0(arg_0, arg_4, 8);
	}
	return result;
}
//k
void sub_100283A0(T_size_14 * arg_0, int arg_4) {
	T_size_14 * pebx = arg_0;
	if (((unsigned long)pebx->hg0x10[0]) == 0x80000000) {
		sub_10028410(pebx);
	}
	for (int i = (signed short)pebx->hg0x04[arg_4]; i != -1; i = (signed short)pebx->hg0x0C[i * 3]) {//eax
		pebx->hg0x10[i]++;
		int j;
		for (j = i - 1; j >= 0; j--) {//esi
			if (pebx->hg0x10[j] >= pebx->hg0x10[i]) {
				break;
			}
		}
		j++;
		if (i != j) {
			sub_100285F0(pebx, i, j);
			i = j;
		}
	}
}
void sub_10028410(T_size_14* arg_0) {
	T_size_14 * pebx = arg_0;
	int * pdw10 = pebx->hg0x10;
	unsigned short * pw0C = pebx->hg0x0C;
	unsigned short * pw04 = pebx->hg0x04;
	/*iarg_0 = pdw10[var_18 - 1];
	var_14 = (var_18 - 1) * 3 * 2;*/
	int iedi = pebx->dw0x08 - 1;
	for (int var_18 = pebx->dw0x08; var_18 > 0; var_18--) {
		if ((unsigned char)pw0C[(var_18 - 1) * 3 + 1]) {
			pw0C[iedi * 3] = pw0C[(var_18 - 1) * 3];
			pw0C[iedi * 3 + 1] = pw0C[(var_18 - 1) * 3 + 1];
			pw0C[iedi * 3 + 2] = pw0C[(var_18 - 1) * 3 + 2];
			pdw10[iedi] = (pdw10[var_18 - 1] + 1) / 2;
			iedi--;
		}
	}
	//loc_1002849A
	int var_C = pebx->dw0x08 - 2;//-2
								 //int *var_18 = &pdw10[var_C];//-8
	for (int iarg_0 = iedi; iarg_0 >= 0; iarg_0--) {
		//var_14 = iarg_0 << 0x1E - iarg_0; -0x3FFFFFFF
		//var_18 = &pdw10[pebx->dw0x08 - 2] -8
		//var_1C = &pdw10[iarg_0 + 1];-4
		//pebp = iarg_0 * 3 * 2 -6
		pdw10[iarg_0] = pdw10[var_C + 1] + pdw10[var_C];//dst = var_4 eax
		iedi++;
		pw0C[iarg_0 * 3 + 1] = (unsigned char)0;
		for (int iecx = 0; (pdw10[var_C + 1] + pdw10[var_C]) < pdw10[iarg_0 + 1 + iecx]; iecx++, iedi++);
		memcpy(&pw0C[iarg_0 * 3], &pw0C[(iarg_0 + 1) * 3], (iedi * 3 - iarg_0 * 3) * 2);
		memcpy(&pdw10[iarg_0], &pdw10[iarg_0 + 1], iedi + ((iarg_0 << 0x1E) - iarg_0));
		pdw10[iedi] = pdw10[var_C + 1] + pdw10[var_C];
		pw0C[iedi * 3 + 2] = var_C;
		pw0C[iedi * 3 + 1] = (unsigned char)0;
		var_C -= 2;
	}
	//loc_100285A0
	for (int ieax = pebx->dw0x08 - 1; ieax >= 0; ieax--) {
		int iecx = pw0C[ieax * 3 + 2];
		if ((unsigned char)pw0C[ieax * 3 + 1])
		{
			pw04[iecx] = ieax;
		}
		else {
			pw0C[(iecx + 1) * 3] = ieax;
			pw0C[iecx] = ieax;
		}
	}

}
void sub_100285F0(T_size_14* arg_0, int arg_4, int arg_8) {
	T_size_14 * pebx = arg_0;
	int *pdwarg0 = pebx->hg0x10;
	unsigned short * pw0C = pebx->hg0x0C;
	unsigned short * pw04 = pebx->hg0x04;
	//edx = arg_4 * 3 * 2;
	if (pw0C[arg_4 * 3 + 1] % 256)
	{
		pw04[pw0C[arg_4 * 3 + 2]] = arg_8;
	}
	else
	{
		pw0C[pw0C[arg_4 * 3 + 2] * 3] = arg_8;
		pw0C[(pw0C[arg_4 * 3 + 2] + 1) * 3] = arg_8;
	}
	if (pw0C[arg_8 * 3 + 1] % 256)
	{
		pw04[pw0C[arg_8 * 3 + 2]] = arg_4;
	}
	else {
		pw0C[pw0C[arg_8 * 3 + 2] * 3] = arg_4;
		pw0C[(pw0C[arg_8 * 3 + 2] + 1) * 3] = arg_4;
	}
	unsigned short var_8_lo = pw0C[arg_4 * 3];
	unsigned short var_8_hi = pw0C[arg_4 * 3 + 1];
	unsigned short var_4 = pw0C[arg_4 * 3 + 2];
	int dwarg_8 = pdwarg0[arg_4];
	pw0C[arg_4 * 3] = pw0C[arg_8 * 3];
	pw0C[arg_4 * 3 + 1] = pw0C[arg_8 * 3 + 1];
	pw0C[arg_4 * 3 + 2] = pw0C[arg_8 * 3 + 2];
	pdwarg0[arg_4] = pdwarg0[arg_8];
	pw0C[arg_4 * 3] = var_8_lo;
	var_8_lo = pw0C[arg_8 * 3];
	pw0C[arg_8 * 3] = var_8_lo;
	pw0C[arg_8 * 3 + 1] = var_8_hi;
	pw0C[arg_8 * 3 + 2] = var_4;
	pdwarg0[arg_8] = dwarg_8;

}
//k
void sub_10028700(T_size_14* arg_0, int arg_4) {
	T_size_14 * peax = arg_0;
	//var_4 = 2 * arg_4;
	unsigned short * pw04 = peax->hg0x04;
	int *parg0_10 = peax->hg0x10;
	unsigned short * pw0C = peax->hg0x0C;
	if (pw04[arg_4] == 0xFFFF) {
		//var_8 = peax->dw0x08 + 1;
		//iesi = (peax->dw0x08 - 1)* 3 * 2;
		//var_C = peax->dw0x08 * 3/* *2*/;
		pw0C[peax->dw0x08 * 3] = pw0C[(peax->dw0x08 - 1) * 3];//3 = 0
		pw0C[peax->dw0x08 * 3 + 1] = pw0C[(peax->dw0x08 - 1) * 3 + 1];
		pw0C[peax->dw0x08 * 3 + 2] = pw0C[(peax->dw0x08 - 1) * 3 + 2];//5 = 2
		parg0_10[peax->dw0x08] = parg0_10[peax->dw0x08 - 1];
		pw0C[peax->dw0x08 * 3] = (peax->dw0x08 - 1);//3
		pw04[pw0C[peax->dw0x08 * 3 + 2]] = peax->dw0x08;
		pw0C[(peax->dw0x08 - 1) * 3 + 2] = peax->dw0x08; //2
		pw0C[(peax->dw0x08 - 1) * 3 + 1] = pw0C[(peax->dw0x08 - 1) * 3 + 1] / 256 * 256;//unsigned char ptr ? 
		pw0C[(peax->dw0x08 + 1) * 3 + 2] = arg_4;//8
		pw0C[(peax->dw0x08 + 1) * 3 + 1] = pw0C[(peax->dw0x08 + 1) * 3 + 1] / 256 * 256 + 1;//unsigned char ptr ?
		parg0_10[peax->dw0x08 + 1] = 0;
		pw0C[(peax->dw0x08 + 1) * 3] = peax->dw0x08 - 1;//6
		pw04[arg_4] = peax->dw0x08 + 1;
		peax->dw0x08 += 2;
	}
}
void sub_100287E0(unsigned char** arg_0, int arg_4, int arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20) {
#define f(t) t <= 255 ? (t < 0 ? 0 : t) : 255

	unsigned char ** pesi = arg_0;
	if (!arg_18) {
		if (!arg_1C) {
			if (arg_20 == arg_14) {
				int a = (arg_4 + 2) / 4;
				int b = (arg_4 + 2) / 8;
				int c = a + b;
				int j = 0, i = 0;
				int t;
				t = pesi[i + 0][(j + 0) * 3] - a;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] + c * 2;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 0][(j + 2) * 3] - a;
				pesi[i + 0][(j + 2) * 3] = f(t);
				t = pesi[i + 0][(j + 3) * 3] - b;
				pesi[i + 0][(j + 3) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] - b;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 1) * 3] + c;
				pesi[i + 1][(j + 1) * 3] = f(t);
				t = pesi[i + 1][(j + 2) * 3] - b;
				pesi[i + 1][(j + 2) * 3] = f(t);
				t = pesi[i + 1][(j + 3) * 3] - b / 2;
				pesi[i + 1][(j + 3) * 3] = f(t);
				return;
			}
			else if (arg_20 == (arg_C - 1)) {
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//edi
				int c = a + b;//ebx
				int j = (arg_20 - arg_14) * 2 - 1, i = 0;//ebp
				int t;
				t = pesi[i + 0][(j + 0) * 3] - b;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] - a;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 0][(j + 2) * 3] + c * 2;
				pesi[i + 0][(j + 2) * 3] = f(t);
				t = pesi[i + 0][(j + 3) * 3] - a;
				pesi[i + 0][(j + 3) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] - b / 2;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 1) * 3] - b;
				pesi[i + 1][(j + 1) * 3] = f(t);
				t = pesi[i + 1][(j + 2) * 3] + c;
				pesi[i + 1][(j + 2) * 3] = f(t);
				t = pesi[i + 1][(j + 3) * 3] - b;
				pesi[i + 1][(j + 3) * 3] = f(t);
				return;
			}
			else {
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//edi
				int c = a + b;//arg_4
				int j = (arg_20 - arg_14) * 2 - 1, i = 0;//ebp
				int t;
				t = pesi[i + 0][(j + 0) * 3] - b;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] - a;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 0][(j + 2) * 3] + c * 2;
				pesi[i + 0][(j + 2) * 3] = f(t);
				t = pesi[i + 0][(j + 3) * 3] - a;
				pesi[i + 0][(j + 3) * 3] = f(t);
				t = pesi[i + 0][(j + 4) * 3] - b;
				pesi[i + 0][(j + 4) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] - b / 2;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 1) * 3] - b;
				pesi[i + 1][(j + 1) * 3] = f(t);
				t = pesi[i + 1][(j + 2) * 3] + c;
				pesi[i + 1][(j + 2) * 3] = f(t);
				t = pesi[i + 1][(j + 3) * 3] - b;
				pesi[i + 1][(j + 3) * 3] = f(t);
				t = pesi[i + 1][(j + 4) * 3] - b / 2;
				pesi[i + 1][(j + 4) * 3] = f(t);
				return;

			}
		}
		if (arg_1C == (arg_8 - 1) / 2) {
			if (arg_20 == arg_14) {
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//edi
				int c = a + b;
				int i = arg_1C * 2 - 1;
				int j = 0;//ebp
				int t;
				t = pesi[i + 0][(j + 0) * 3] - b;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] + c;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 0][(j + 2) * 3] - b;
				pesi[i + 0][(j + 2) * 3] = f(t);
				t = pesi[i + 0][(j + 3) * 3] - b / 2;
				pesi[i + 0][(j + 3) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] - a;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 1) * 3] + a * 3;
				pesi[i + 1][(j + 1) * 3] = f(t);
				t = pesi[i + 1][(j + 2) * 3] - a;
				pesi[i + 1][(j + 2) * 3] = f(t);
				t = pesi[i + 1][(j + 3) * 3] - b;
				pesi[i + 1][(j + 3) * 3] = f(t);
				return;
			}
			else if (arg_20 == (arg_C - 1)) {
				int a = (arg_4 + 2) / 4;//edi
				int b = (arg_4 + 2) / 8;//ecx
				int c = a + b;
				int i = arg_1C * 2 - 1;
				int j = (arg_20 - arg_14) * 2 - 1;//ebp
				int t;
				t = pesi[i + 0][(j + 0) * 3] - b / 2;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] - b;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 0][(j + 2) * 3] + c;
				pesi[i + 0][(j + 2) * 3] = f(t);
				t = pesi[i + 0][(j + 3) * 3] - b;
				pesi[i + 0][(j + 3) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] - b;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 1) * 3] - a;
				pesi[i + 1][(j + 1) * 3] = f(t);
				t = pesi[i + 1][(j + 2) * 3] + a * 3;
				pesi[i + 1][(j + 2) * 3] = f(t);
				t = pesi[i + 1][(j + 3) * 3] - a;
				pesi[i + 1][(j + 3) * 3] = f(t);
				return;
			}
			else {
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//iesi
				int c = a + b;
				int i = arg_1C * 2 - 1;
				int j = (arg_20 - arg_14) * 2 - 1;//ebp
				int t;
				t = pesi[i + 0][(j + 0) * 3] - b / 2;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] - b;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 0][(j + 2) * 3] + c;
				pesi[i + 0][(j + 2) * 3] = f(t);
				t = pesi[i + 0][(j + 3) * 3] - b;
				pesi[i + 0][(j + 3) * 3] = f(t);
				t = pesi[i + 0][(j + 4) * 3] - b / 2;
				pesi[i + 0][(j + 4) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] - b;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 1) * 3] - a;
				pesi[i + 1][(j + 1) * 3] = f(t);
				t = pesi[i + 1][(j + 2) * 3] + c * 2;
				pesi[i + 1][(j + 2) * 3] = f(t);
				t = pesi[i + 1][(j + 3) * 3] - a;
				pesi[i + 1][(j + 3) * 3] = f(t);
				t = pesi[i + 1][(j + 4) * 3] - b;
				pesi[i + 1][(j + 4) * 3] = f(t);
				return;
			}
		}
		else if (arg_1C == (arg_10 - 1)) {
			if (arg_20 == arg_14) {
				int i = arg_8 - 1;
				int j = 0;//ebp
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//edi
				int c = a + b;
				int t;
				t = pesi[i + 0][(j + 0) * 3] - a;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] + 2 * c;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 0][(j + 2) * 3] - a;
				pesi[i + 0][(j + 2) * 3] = f(t);
				t = pesi[i + 0][(j + 3) * 3] - b;
				pesi[i + 0][(j + 3) * 3] = f(t);
				return;
			}
			else if (arg_20 == (arg_C - 1)) {
				int i = arg_8 - 1;
				int j = (arg_20 - arg_14) * 2 - 1;//ebp
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//edi
				int c = a + b;
				int t;
				t = pesi[i + 0][(j + 0) * 3] - b;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] - a;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 0][(j + 2) * 3] + c * 2;
				pesi[i + 0][(j + 2) * 3] = f(t);
				t = pesi[i + 0][(j + 3) * 3] - a;
				pesi[i + 0][(j + 3) * 3] = f(t);
				return;
			}
			else {
				int i = arg_8 - 1;
				int j = (arg_20 - arg_14) * 2 - 1;//ebp
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//edi
				int c = a + b;
				int t;
				t = pesi[i + 0][(j + 0) * 3] - b;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] - a;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 0][(j + 2) * 3] + c * 2;
				pesi[i + 0][(j + 2) * 3] = f(t);
				t = pesi[i + 0][(j + 3) * 3] - a;
				pesi[i + 0][(j + 3) * 3] = f(t);
				t = pesi[i + 0][(j + 4) * 3] - b;
				pesi[i + 0][(j + 4) * 3] = f(t);
				return;
			}
		}
		if (arg_20 == arg_14) {
			int a = (arg_4 + 2) / 4;//edi
			int b = (arg_4 + 2) / 8;//iesi
			int c = a + b;
			int i = arg_1C * 2 - 1;
			int j = 0;//ebp
			int t;
			t = pesi[i + 0][(j + 0) * 3] - b;
			pesi[i + 0][(j + 0) * 3] = f(t);
			t = pesi[i + 0][(j + 1) * 3] + c;
			pesi[i + 0][(j + 1) * 3] = f(t);
			t = pesi[i + 0][(j + 2) * 3] - b;
			pesi[i + 0][(j + 2) * 3] = f(t);
			t = pesi[i + 0][(j + 3) * 3] - b / 2;
			pesi[i + 0][(j + 3) * 3] = f(t);
			t = pesi[i + 1][(j + 0) * 3] - a;
			pesi[i + 1][(j + 0) * 3] = f(t);
			t = pesi[i + 1][(j + 1) * 3] - a;
			pesi[i + 1][(j + 1) * 3] = f(t);
			t = pesi[i + 1][(j + 2) * 3] + c * 2;
			pesi[i + 1][(j + 2) * 3] = f(t);
			t = pesi[i + 1][(j + 3) * 3] - a;
			pesi[i + 1][(j + 3) * 3] = f(t);
			t = pesi[i + 2][(j + 0) * 3] - b;
			pesi[i + 2][(j + 0) * 3] = f(t);
			t = pesi[i + 2][(j + 1) * 3] + c;
			pesi[i + 2][(j + 1) * 3] = f(t);
			t = pesi[i + 2][(j + 2) * 3] - b;
			pesi[i + 2][(j + 2) * 3] = f(t);
			t = pesi[i + 2][(j + 3) * 3] - b / 2;
			pesi[i + 2][(j + 3) * 3] = f(t);
			return;
		}
		else if (arg_20 == (arg_C - 1)) {
			int a = (arg_4 + 2) / 4;//edi
			int b = (arg_4 + 2) / 8;//iesi
			int c = a + b;
			int i = arg_1C * 2 - 1;
			int j = (arg_20 - arg_14) * 2 - 1;//ebp
			int t;
			t = pesi[i + 0][(j + 0) * 3] - b / 2;
			pesi[i + 0][(j + 0) * 3] = f(t);
			t = pesi[i + 0][(j + 1) * 3] - b;
			pesi[i + 0][(j + 1) * 3] = f(t);
			t = pesi[i + 0][(j + 2) * 3] + c;
			pesi[i + 0][(j + 2) * 3] = f(t);
			t = pesi[i + 0][(j + 3) * 3] - b;
			pesi[i + 0][(j + 3) * 3] = f(t);
			t = pesi[i + 1][(j + 0) * 3] - b;
			pesi[i + 1][(j + 0) * 3] = f(t);
			t = pesi[i + 1][(j + 1) * 3] - a;
			pesi[i + 1][(j + 1) * 3] = f(t);
			t = pesi[i + 1][(j + 2) * 3] + c * 2;
			pesi[i + 1][(j + 2) * 3] = f(t);
			t = pesi[i + 1][(j + 3) * 3] - a;
			pesi[i + 1][(j + 3) * 3] = f(t);
			t = pesi[i + 2][(j + 0) * 3] - b / 2;
			pesi[i + 2][(j + 0) * 3] = f(t);
			t = pesi[i + 2][(j + 1) * 3] - b;
			pesi[i + 2][(j + 1) * 3] = f(t);
			t = pesi[i + 2][(j + 2) * 3] + c;
			pesi[i + 2][(j + 2) * 3] = f(t);
			t = pesi[i + 2][(j + 3) * 3] - b;
			pesi[i + 2][(j + 3) * 3] = f(t);
			return;
		}
		else {
			int a = (arg_4 + 2) / 4;//edi
			int b = (arg_4 + 2) / 8;//iesi
			int c = a + b;
			int i = arg_1C * 2 - 1;
			int j = (arg_20 - arg_14) * 2 - 1;//ebp
			int t;
			t = pesi[i + 0][(j + 0) * 3] - b / 2;
			pesi[i + 0][(j + 0) * 3] = f(t);
			t = pesi[i + 0][(j + 1) * 3] - b;
			pesi[i + 0][(j + 1) * 3] = f(t);
			t = pesi[i + 0][(j + 2) * 3] + c;
			pesi[i + 0][(j + 2) * 3] = f(t);
			t = pesi[i + 0][(j + 3) * 3] - b;
			pesi[i + 0][(j + 3) * 3] = f(t);
			t = pesi[i + 0][(j + 4) * 3] - b / 2;
			pesi[i + 0][(j + 4) * 3] = f(t);
			t = pesi[i + 1][(j + 0) * 3] - b;
			pesi[i + 1][(j + 0) * 3] = f(t);
			t = pesi[i + 1][(j + 1) * 3] - a;
			pesi[i + 1][(j + 1) * 3] = f(t);
			t = pesi[i + 1][(j + 2) * 3] + c * 2;
			pesi[i + 1][(j + 2) * 3] = f(t);
			t = pesi[i + 1][(j + 3) * 3] - a;
			pesi[i + 1][(j + 3) * 3] = f(t);
			t = pesi[i + 1][(j + 4) * 3] - b;
			pesi[i + 1][(j + 4) * 3] = f(t);
			t = pesi[i + 2][(j + 0) * 3] - b / 2;
			pesi[i + 2][(j + 0) * 3] = f(t);
			t = pesi[i + 2][(j + 1) * 3] - b;
			pesi[i + 2][(j + 1) * 3] = f(t);
			t = pesi[i + 2][(j + 2) * 3] + c;
			pesi[i + 2][(j + 2) * 3] = f(t);
			t = pesi[i + 2][(j + 3) * 3] - b;
			pesi[i + 2][(j + 3) * 3] = f(t);
			t = pesi[i + 2][(j + 4) * 3] - b / 2;
			pesi[i + 2][(j + 4) * 3] = f(t);
			return;
		}
	}
	if (arg_18 == 1) {
		if (arg_1C == arg_10) {
			if (!arg_20) {
				int a = (arg_4 + 2) / 4;//iesi
				int b = (arg_4 + 2) / 8;//iecx
				int c = a + b;//ebp
				int i = 0;
				int j = 0;
				int t;
				t = pesi[i + 0][(j + 0) * 3] - a;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] - b;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] + c * 2;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 1) * 3] + c;
				pesi[i + 1][(j + 1) * 3] = f(t);
				t = pesi[i + 2][(j + 0) * 3] - a;
				pesi[i + 2][(j + 0) * 3] = f(t);
				t = pesi[i + 2][(j + 1) * 3] - b;
				pesi[i + 2][(j + 1) * 3] = f(t);
				t = pesi[i + 3][(j + 0) * 3] - b;
				pesi[i + 3][(j + 0) * 3] = f(t);
				t = pesi[i + 3][(j + 1) * 3] - b / 2;
				pesi[i + 3][(j + 1) * 3] = f(t);
				return;
			}
			if (arg_20 == (arg_C - 1) / 2) {
				int a = (arg_4 + 2) / 4;//iedi
				int b = (arg_4 + 2) / 8;//iesi
				int c = a + b;//ebp
				int i = 0;
				int j = arg_20 * 2 - 1;
				int t;
				t = pesi[i + 0][(j + 0) * 3] - b;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] - a;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] + c;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 1) * 3] + c * 2;
				pesi[i + 1][(j + 1) * 3] = f(t);
				t = pesi[i + 2][(j + 0) * 3] - b;
				pesi[i + 2][(j + 0) * 3] = f(t);
				t = pesi[i + 2][(j + 1) * 3] - a;
				pesi[i + 2][(j + 1) * 3] = f(t);
				t = pesi[i + 3][(j + 0) * 3] - b / 2;
				pesi[i + 3][(j + 0) * 3] = f(t);
				t = pesi[i + 3][(j + 1) * 3] - b;
				pesi[i + 3][(j + 1) * 3] = f(t);
				return;
			}
			else if (arg_20 == (arg_14 - 1)) {
				int a = (arg_4 + 2) / 4;//ieax
				int b = (arg_4 + 2) / 8;//iedi
				int c = a + b;//ebp
				int i = 0;
				int j = arg_20 * 2 - 1;
				int t;
				t = pesi[i + 0][(j + 0) * 3] - a;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] + c * 2;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 2][(j + 0) * 3] - a;
				pesi[i + 2][(j + 0) * 3] = f(t);
				t = pesi[i + 3][(j + 0) * 3] - b;
				pesi[i + 3][(j + 0) * 3] = f(t);
				return;
			}
			else {
				int a = (arg_4 + 2) / 4;//iedi
				int b = (arg_4 + 2) / 8;//iesi
				int c = a + b;//ebp
				int i = 0;
				int j = arg_20 * 2 - 1;
				int t;
				t = pesi[i + 0][(j + 0) * 3] - b;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] - a;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 0][(j + 2) * 3] - b;
				pesi[i + 0][(j + 2) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] + c;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 1) * 3] + c * 2;
				pesi[i + 1][(j + 1) * 3] = f(t);
				t = pesi[i + 1][(j + 2) * 3] + c;
				pesi[i + 1][(j + 2) * 3] = f(t);
				t = pesi[i + 2][(j + 0) * 3] - b;
				pesi[i + 2][(j + 0) * 3] = f(t);
				t = pesi[i + 2][(j + 1) * 3] - a;
				pesi[i + 2][(j + 1) * 3] = f(t);
				t = pesi[i + 2][(j + 2) * 3] - b;
				pesi[i + 2][(j + 2) * 3] = f(t);
				t = pesi[i + 3][(j + 0) * 3] - b / 2;
				pesi[i + 3][(j + 0) * 3] = f(t);
				t = pesi[i + 3][(j + 1) * 3] - b;
				pesi[i + 3][(j + 1) * 3] = f(t);
				t = pesi[i + 3][(j + 2) * 3] - b / 2;
				pesi[i + 3][(j + 2) * 3] = f(t);
				return;
			}
		}
		else if (arg_1C == (arg_8 - 1)) {
			if (!arg_20) {
				int a = (arg_4 + 2) / 4;//iedx
				int b = (arg_4 + 2) / 8;//iedi
				int c = a + b;//ebp
				int i = (arg_1C - arg_10) * 2 - 1;
				int j = 0;
				int t;
				t = pesi[i + 0][(j + 0) * 3] - b;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] - b / 2;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] - a;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 1) * 3] - b;
				pesi[i + 1][(j + 1) * 3] = f(t);
				t = pesi[i + 2][(j + 0) * 3] + c * 2;
				pesi[i + 2][(j + 0) * 3] = f(t);
				t = pesi[i + 2][(j + 1) * 3] + c;
				pesi[i + 2][(j + 1) * 3] = f(t);
				t = pesi[i + 3][(j + 0) * 3] - a;
				pesi[i + 3][(j + 0) * 3] = f(t);
				t = pesi[i + 3][(j + 1) * 3] - b;
				pesi[i + 3][(j + 1) * 3] = f(t);
				return;
			}
			else if (arg_20 == (arg_C - 1) / 2) {
				int a = (arg_4 + 2) / 4;//edi
				int b = (arg_4 + 2) / 8;//edx
				int c = a + b;//ebp
				int i = (arg_1C - arg_10) * 2 - 1;
				int j = arg_20 * 2 - 1;
				int t;
				t = pesi[i + 0][(j + 0) * 3] - b / 2;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] - b;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] - b;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 1) * 3] - a;
				pesi[i + 1][(j + 1) * 3] = f(t);
				t = pesi[i + 2][(j + 0) * 3] + c;
				pesi[i + 2][(j + 0) * 3] = f(t);
				t = pesi[i + 2][(j + 1) * 3] + c * 2;
				pesi[i + 2][(j + 1) * 3] = f(t);
				t = pesi[i + 3][(j + 0) * 3] - b;
				pesi[i + 3][(j + 0) * 3] = f(t);
				t = pesi[i + 3][(j + 1) * 3] - a;
				pesi[i + 3][(j + 1) * 3] = f(t);
				return;
			}
			else if (arg_20 == (arg_14 - 1)) {
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//esi
				int c = a + b;//ebp
				int i = (arg_1C - arg_10) * 2 - 1;
				int j = arg_C - 1;
				int t;
				t = pesi[i + 0][(j + 0) * 3] - b;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] - a;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 2][(j + 0) * 3] + c * 2;
				pesi[i + 2][(j + 0) * 3] = f(t);
				t = pesi[i + 3][(j + 0) * 3] - a;
				pesi[i + 3][(j + 0) * 3] = f(t);
				return;
			}
			else {
				int a = (arg_4 + 2) / 4;//ebp
				int b = (arg_4 + 2) / 8;//esi
				int c = a + b;//ebp
				int i = (arg_1C - arg_10) * 2 - 1;
				int j = arg_20 * 2 - 1;
				int t;
				t = pesi[i + 0][(j + 0) * 3] - b / 2;
				pesi[i + 0][(j + 0) * 3] = f(t);
				t = pesi[i + 0][(j + 1) * 3] - b;
				pesi[i + 0][(j + 1) * 3] = f(t);
				t = pesi[i + 0][(j + 2) * 3] - b / 2;
				pesi[i + 0][(j + 2) * 3] = f(t);
				t = pesi[i + 1][(j + 0) * 3] - b;
				pesi[i + 1][(j + 0) * 3] = f(t);
				t = pesi[i + 1][(j + 1) * 3] - a;
				pesi[i + 1][(j + 1) * 3] = f(t);
				t = pesi[i + 1][(j + 2) * 3] - b;
				pesi[i + 1][(j + 2) * 3] = f(t);
				t = pesi[i + 2][(j + 0) * 3] + c;
				pesi[i + 2][(j + 0) * 3] = f(t);
				t = pesi[i + 2][(j + 1) * 3] + c * 2;
				pesi[i + 2][(j + 1) * 3] = f(t);
				t = pesi[i + 2][(j + 2) * 3] + c;
				pesi[i + 2][(j + 2) * 3] = f(t);
				t = pesi[i + 3][(j + 0) * 3] - b;
				pesi[i + 3][(j + 0) * 3] = f(t);
				t = pesi[i + 3][(j + 1) * 3] - a;
				pesi[i + 3][(j + 1) * 3] = f(t);
				t = pesi[i + 3][(j + 1) * 3] - b;
				pesi[i + 3][(j + 1) * 3] = f(t);
				return;
			}
		}
		else if (!arg_20) {
			int a = (arg_4 + 2) / 4;//esi
			int b = (arg_4 + 2) / 8;//edx
			int c = a + b;//ebp
			int i = (arg_1C - arg_10) * 2 - 1;
			int j = 0;
			int t;
			t = pesi[i + 0][(j + 0) * 3] - b;
			pesi[i + 0][(j + 0) * 3] = f(t);
			t = pesi[i + 0][(j + 1) * 3] - b / 2;
			pesi[i + 0][(j + 1) * 3] = f(t);
			t = pesi[i + 1][(j + 0) * 3] - a;
			pesi[i + 1][(j + 0) * 3] = f(t);
			t = pesi[i + 1][(j + 1) * 3] - b;
			pesi[i + 1][(j + 1) * 3] = f(t);
			t = pesi[i + 2][(j + 0) * 3] + c * 2;
			pesi[i + 2][(j + 0) * 3] = f(t);
			t = pesi[i + 2][(j + 1) * 3] + c;
			pesi[i + 2][(j + 1) * 3] = f(t);
			t = pesi[i + 3][(j + 0) * 3] - a;
			pesi[i + 3][(j + 0) * 3] = f(t);
			t = pesi[i + 3][(j + 1) * 3] - b;
			pesi[i + 3][(j + 1) * 3] = f(t);
			t = pesi[i + 4][(j + 0) * 3] - b;
			pesi[i + 4][(j + 0) * 3] = f(t);
			t = pesi[i + 4][(j + 1) * 3] - b / 2;
			pesi[i + 4][(j + 1) * 3] = f(t);
			return;
		}
		else if (arg_20 == (arg_C - 1) / 2) {
			int a = (arg_4 + 2) / 4;//edi
			int b = (arg_4 + 2) / 8;//esi
			int c = a + b;//ebp
			int i = (arg_1C - arg_10) * 2 - 1;
			int j = 2 * arg_20 - 1;
			int t;
			t = pesi[i + 0][(j + 0) * 3] - b / 2;
			pesi[i + 0][(j + 0) * 3] = f(t);
			t = pesi[i + 0][(j + 1) * 3] - b;
			pesi[i + 0][(j + 1) * 3] = f(t);
			t = pesi[i + 1][(j + 0) * 3] - b;
			pesi[i + 1][(j + 0) * 3] = f(t);
			t = pesi[i + 1][(j + 1) * 3] - a;
			pesi[i + 1][(j + 1) * 3] = f(t);
			t = pesi[i + 2][(j + 0) * 3] + c;
			pesi[i + 2][(j + 0) * 3] = f(t);
			t = pesi[i + 2][(j + 1) * 3] + c * 2;
			pesi[i + 2][(j + 1) * 3] = f(t);
			t = pesi[i + 3][(j + 0) * 3] - b;
			pesi[i + 3][(j + 0) * 3] = f(t);
			t = pesi[i + 3][(j + 1) * 3] - a;
			pesi[i + 3][(j + 1) * 3] = f(t);
			t = pesi[i + 4][(j + 0) * 3] - b / 2;
			pesi[i + 4][(j + 0) * 3] = f(t);
			t = pesi[i + 4][(j + 1) * 3] - b;
			pesi[i + 4][(j + 1) * 3] = f(t);
			return;
		}
		else if (arg_20 == (arg_14 - 1)) {
			int a = (arg_4 + 2) / 4;//ecx
			int b = (arg_4 + 2) / 8;//edi
			int c = a + b;//ebp
			int i = (arg_1C - arg_10) * 2 - 1;
			int j = arg_C - 1;
			int t;
			t = pesi[i + 0][(j + 0) * 3] - b;
			pesi[i + 0][(j + 0) * 3] = f(t);
			t = pesi[i + 1][(j + 0) * 3] - a;
			pesi[i + 1][(j + 0) * 3] = f(t);
			t = pesi[i + 2][(j + 0) * 3] + c * 2;
			pesi[i + 2][(j + 0) * 3] = f(t);
			t = pesi[i + 3][(j + 0) * 3] - a;
			pesi[i + 3][(j + 0) * 3] = f(t);
			t = pesi[i + 4][(j + 0) * 3] - b;
			pesi[i + 4][(j + 0) * 3] = f(t);
			return;
		}
		else {
			int a = (arg_4 + 2) / 4;//edi
			int b = (arg_4 + 2) / 8;//esi
			int c = a + b;//ebp
			int i = (arg_1C - arg_10) * 2 - 1;
			int j = 2 * arg_20 - 1;
			int t;
			t = pesi[i + 0][(j + 0) * 3] - b / 2;
			pesi[i + 0][(j + 0) * 3] = f(t);
			t = pesi[i + 0][(j + 1) * 3] - b;
			pesi[i + 0][(j + 1) * 3] = f(t);
			t = pesi[i + 0][(j + 2) * 3] - b / 2;
			pesi[i + 0][(j + 2) * 3] = f(t);
			t = pesi[i + 1][(j + 0) * 3] - b;
			pesi[i + 1][(j + 0) * 3] = f(t);
			t = pesi[i + 1][(j + 1) * 3] - a;
			pesi[i + 1][(j + 1) * 3] = f(t);
			t = pesi[i + 1][(j + 2) * 3] - b;
			pesi[i + 1][(j + 2) * 3] = f(t);
			t = pesi[i + 2][(j + 0) * 3] + c;
			pesi[i + 2][(j + 0) * 3] = f(t);
			t = pesi[i + 2][(j + 1) * 3] + c * 2;
			pesi[i + 2][(j + 1) * 3] = f(t);
			t = pesi[i + 2][(j + 2) * 3] + c;
			pesi[i + 2][(j + 2) * 3] = f(t);
			t = pesi[i + 3][(j + 0) * 3] - b;
			pesi[i + 3][(j + 0) * 3] = f(t);
			t = pesi[i + 3][(j + 1) * 3] - a;
			pesi[i + 3][(j + 1) * 3] = f(t);
			t = pesi[i + 3][(j + 2) * 3] - b;
			pesi[i + 3][(j + 2) * 3] = f(t);
			t = pesi[i + 4][(j + 0) * 3] - b / 2;
			pesi[i + 4][(j + 0) * 3] = f(t);
			t = pesi[i + 4][(j + 1) * 3] - b;
			pesi[i + 4][(j + 1) * 3] = f(t);
			t = pesi[i + 4][(j + 2) * 3] - b / 2;
			pesi[i + 4][(j + 2) * 3] = f(t);
			return;
		}
	}
	if (arg_1C == (arg_8 - 1)) {
		if (arg_20 == arg_14) {
			int a = (arg_4 + 2) / 8;//esi
			int b = (arg_4 + 2) / 16;//edi
			int c = a + b;
			int d = c * 2;//ebp
			int i = (arg_1C - arg_10) * 2 - 1, j = 0;
			int t;
			t = pesi[i + 0][(j + 0) * 3] + b;
			pesi[i + 0][(j + 0) * 3] = f(t);
			t = pesi[i + 0][(j + 1) * 3] - c;
			pesi[i + 0][(j + 1) * 3] = f(t);
			t = pesi[i + 0][(j + 2) * 3] + b;
			pesi[i + 0][(j + 2) * 3] = f(t);
			t = pesi[i + 0][(j + 3) * 3] + b / 2;
			pesi[i + 0][(j + 3) * 3] = f(t);
			t = pesi[i + 1][(j + 0) * 3] + a;
			pesi[i + 1][(j + 0) * 3] = f(t);
			t = pesi[i + 1][(j + 1) * 3] - d;
			pesi[i + 1][(j + 1) * 3] = f(t);
			t = pesi[i + 1][(j + 2) * 3] + a;
			pesi[i + 1][(j + 2) * 3] = f(t);
			t = pesi[i + 1][(j + 3) * 3] + b;
			pesi[i + 1][(j + 3) * 3] = f(t);
			t = pesi[i + 2][(j + 0) * 3] - d;
			pesi[i + 2][(j + 0) * 3] = f(t);
			t = pesi[i + 2][(j + 1) * 3] + d * 3;
			pesi[i + 2][(j + 1) * 3] = f(t);
			t = pesi[i + 2][(j + 2) * 3] - d;
			pesi[i + 2][(j + 2) * 3] = f(t);
			t = pesi[i + 2][(j + 3) * 3] - c;
			pesi[i + 2][(j + 3) * 3] = f(t);
			t = pesi[i + 3][(j + 0) * 3] + a;
			pesi[i + 3][(j + 0) * 3] = f(t);
			t = pesi[i + 3][(j + 1) * 3] - d;
			pesi[i + 3][(j + 1) * 3] = f(t);
			t = pesi[i + 3][(j + 2) * 3] + a;
			pesi[i + 3][(j + 2) * 3] = f(t);
			t = pesi[i + 3][(j + 3) * 3] + b;
			pesi[i + 3][(j + 3) * 3] = f(t);
			return;
		}
		else if (arg_20 == (arg_C - 1)) {
			int a = (arg_4 + 2) / 8;//edi
			int b = (arg_4 + 2) / 16;//esi
			int c = a + b;
			int d = c * 2;//ebp
			int i = (arg_1C - arg_10) * 2 - 1, j = 2 * (arg_20 - arg_14) - 1;
			int t;
			t = pesi[i + 0][(j + 0) * 3] + b / 2;
			pesi[i + 0][(j + 0) * 3] = f(t);
			t = pesi[i + 0][(j + 1) * 3] + b;
			pesi[i + 0][(j + 1) * 3] = f(t);
			t = pesi[i + 0][(j + 2) * 3] - c;
			pesi[i + 0][(j + 2) * 3] = f(t);
			t = pesi[i + 0][(j + 3) * 3] + b;
			pesi[i + 0][(j + 3) * 3] = f(t);
			t = pesi[i + 1][(j + 0) * 3] + b;
			pesi[i + 1][(j + 0) * 3] = f(t);
			t = pesi[i + 1][(j + 1) * 3] + a;
			pesi[i + 1][(j + 1) * 3] = f(t);
			t = pesi[i + 1][(j + 2) * 3] - d;
			pesi[i + 1][(j + 2) * 3] = f(t);
			t = pesi[i + 1][(j + 3) * 3] + a;
			pesi[i + 1][(j + 3) * 3] = f(t);
			t = pesi[i + 2][(j + 0) * 3] - c;
			pesi[i + 2][(j + 0) * 3] = f(t);
			t = pesi[i + 2][(j + 1) * 3] - d;
			pesi[i + 2][(j + 1) * 3] = f(t);
			t = pesi[i + 2][(j + 2) * 3] + d * 3;
			pesi[i + 2][(j + 2) * 3] = f(t);
			t = pesi[i + 2][(j + 3) * 3] - d;
			pesi[i + 2][(j + 3) * 3] = f(t);
			t = pesi[i + 3][(j + 0) * 3] + b;
			pesi[i + 3][(j + 0) * 3] = f(t);
			t = pesi[i + 3][(j + 1) * 3] + a;
			pesi[i + 3][(j + 1) * 3] = f(t);
			t = pesi[i + 3][(j + 2) * 3] - d;
			pesi[i + 3][(j + 2) * 3] = f(t);
			t = pesi[i + 3][(j + 3) * 3] + a;
			pesi[i + 3][(j + 3) * 3] = f(t);
			return;
		}
		else {
			int a = (arg_4 + 2) / 8;//919
			int b = (arg_4 + 2) / 16;//920
			int c = a + b;
			int d = c * 2;//v1189
			int i = (arg_1C - arg_10) * 2 - 1, j = 2 * (arg_20 - arg_14) - 1;
			int t;
			t = pesi[i + 0][(j + 0) * 3] + b / 2;
			pesi[i + 0][(j + 0) * 3] = f(t);
			t = pesi[i + 0][(j + 1) * 3] + b;
			pesi[i + 0][(j + 1) * 3] = f(t);
			t = pesi[i + 0][(j + 2) * 3] - c;
			pesi[i + 0][(j + 2) * 3] = f(t);
			t = pesi[i + 0][(j + 3) * 3] + b;
			pesi[i + 0][(j + 3) * 3] = f(t);
			t = pesi[i + 0][(j + 4) * 3] + b / 2;
			pesi[i + 0][(j + 4) * 3] = f(t);
			t = pesi[i + 1][(j + 0) * 3] + b;
			pesi[i + 1][(j + 0) * 3] = f(t);
			t = pesi[i + 1][(j + 1) * 3] + a;
			pesi[i + 1][(j + 1) * 3] = f(t);
			t = pesi[i + 1][(j + 2) * 3] - d;
			pesi[i + 1][(j + 2) * 3] = f(t);
			t = pesi[i + 1][(j + 3) * 3] + a;
			pesi[i + 1][(j + 3) * 3] = f(t);
			t = pesi[i + 1][(j + 4) * 3] + b;
			pesi[i + 1][(j + 4) * 3] = f(t);
			t = pesi[i + 2][(j + 0) * 3] - c;
			pesi[i + 2][(j + 0) * 3] = f(t);
			t = pesi[i + 2][(j + 1) * 3] - d;
			pesi[i + 2][(j + 1) * 3] = f(t);
			t = pesi[i + 2][(j + 2) * 3] + d * 3;
			pesi[i + 2][(j + 2) * 3] = f(t);
			t = pesi[i + 2][(j + 3) * 3] - d;
			pesi[i + 2][(j + 3) * 3] = f(t);
			t = pesi[i + 2][(j + 4) * 3] - c;
			pesi[i + 2][(j + 4) * 3] = f(t);
			t = pesi[i + 3][(j + 0) * 3] + b;
			pesi[i + 3][(j + 0) * 3] = f(t);
			t = pesi[i + 3][(j + 1) * 3] + a;
			pesi[i + 3][(j + 1) * 3] = f(t);
			t = pesi[i + 3][(j + 2) * 3] - d;
			pesi[i + 3][(j + 2) * 3] = f(t);
			t = pesi[i + 3][(j + 3) * 3] + a;
			pesi[i + 3][(j + 3) * 3] = f(t);
			t = pesi[i + 3][(j + 4) * 3] + b;
			pesi[i + 3][(j + 4) * 3] = f(t);
			return;
		}
	}
	if (arg_20 == arg_14) {
		int a = (arg_4 + 2) / 8;//v980
		int b = (arg_4 + 2) / 16;//v981
		int c = a + b;
		int d = c * 2;//v982
		int i = (arg_1C - arg_10) * 2 - 1, j = 0;
		int t;
		t = pesi[i + 0][(j + 0) * 3] + b;
		pesi[i + 0][(j + 0) * 3] = f(t);
		t = pesi[i + 0][(j + 1) * 3] - c;
		pesi[i + 0][(j + 1) * 3] = f(t);
		t = pesi[i + 0][(j + 2) * 3] + b;
		pesi[i + 0][(j + 2) * 3] = f(t);
		t = pesi[i + 0][(j + 3) * 3] + b / 2;
		pesi[i + 0][(j + 3) * 3] = f(t);
		t = pesi[i + 1][(j + 0) * 3] + a;
		pesi[i + 1][(j + 0) * 3] = f(t);
		t = pesi[i + 1][(j + 1) * 3] - d;
		pesi[i + 1][(j + 1) * 3] = f(t);
		t = pesi[i + 1][(j + 2) * 3] + a;
		pesi[i + 1][(j + 2) * 3] = f(t);
		t = pesi[i + 1][(j + 3) * 3] + b;
		pesi[i + 1][(j + 3) * 3] = f(t);
		t = pesi[i + 2][(j + 0) * 3] - d;
		pesi[i + 2][(j + 0) * 3] = f(t);
		t = pesi[i + 2][(j + 1) * 3] + d * 3;
		pesi[i + 2][(j + 1) * 3] = f(t);
		t = pesi[i + 2][(j + 2) * 3] - d;
		pesi[i + 2][(j + 2) * 3] = f(t);
		t = pesi[i + 2][(j + 3) * 3] - c;
		pesi[i + 2][(j + 3) * 3] = f(t);
		t = pesi[i + 3][(j + 0) * 3] + a;
		pesi[i + 3][(j + 0) * 3] = f(t);
		t = pesi[i + 3][(j + 1) * 3] - d;
		pesi[i + 3][(j + 1) * 3] = f(t);
		t = pesi[i + 3][(j + 2) * 3] + a;
		pesi[i + 3][(j + 2) * 3] = f(t);
		t = pesi[i + 3][(j + 3) * 3] + b;
		pesi[i + 3][(j + 3) * 3] = f(t);
		t = pesi[i + 4][(j + 0) * 3] + b;
		pesi[i + 4][(j + 0) * 3] = f(t);
		t = pesi[i + 3][(j + 1) * 3] - c;
		pesi[i + 4][(j + 1) * 3] = f(t);
		t = pesi[i + 4][(j + 2) * 3] + b;
		pesi[i + 4][(j + 2) * 3] = f(t);
		t = pesi[i + 4][(j + 3) * 3] + b / 2;
		pesi[i + 4][(j + 3) * 3] = f(t);
		return;
	}
	if (arg_20 == (arg_C - 1)) {
		int a = (arg_4 + 2) / 8;//v1041
		int b = (arg_4 + 2) / 16;//v1042
		int c = a + b;
		int d = c * 2;//v1190
		int i = (arg_1C - arg_10) * 2 - 1, j = (arg_20 - arg_14) * 2 - 1;
		int t;
		t = pesi[i + 0][(j + 0) * 3] + b / 2;
		pesi[i + 0][(j + 0) * 3] = f(t);
		t = pesi[i + 0][(j + 1) * 3] + b;
		pesi[i + 0][(j + 1) * 3] = f(t);
		t = pesi[i + 0][(j + 2) * 3] - c;
		pesi[i + 0][(j + 2) * 3] = f(t);
		t = pesi[i + 0][(j + 3) * 3] + b;
		pesi[i + 0][(j + 3) * 3] = f(t);
		t = pesi[i + 1][(j + 0) * 3] + b;
		pesi[i + 1][(j + 0) * 3] = f(t);
		t = pesi[i + 1][(j + 1) * 3] + a;
		pesi[i + 1][(j + 1) * 3] = f(t);
		t = pesi[i + 1][(j + 2) * 3] - d;
		pesi[i + 1][(j + 2) * 3] = f(t);
		t = pesi[i + 1][(j + 3) * 3] + a;
		pesi[i + 1][(j + 3) * 3] = f(t);
		t = pesi[i + 2][(j + 0) * 3] - c;
		pesi[i + 2][(j + 0) * 3] = f(t);
		t = pesi[i + 2][(j + 1) * 3] - d;
		pesi[i + 2][(j + 1) * 3] = f(t);
		t = pesi[i + 2][(j + 2) * 3] + d * 3;
		pesi[i + 2][(j + 2) * 3] = f(t);
		t = pesi[i + 2][(j + 3) * 3] - d;
		pesi[i + 2][(j + 3) * 3] = f(t);
		t = pesi[i + 3][(j + 0) * 3] + b;
		pesi[i + 3][(j + 0) * 3] = f(t);
		t = pesi[i + 3][(j + 1) * 3] + a;
		pesi[i + 3][(j + 1) * 3] = f(t);
		t = pesi[i + 3][(j + 2) * 3] - d;
		pesi[i + 3][(j + 2) * 3] = f(t);
		t = pesi[i + 3][(j + 3) * 3] + a;
		pesi[i + 3][(j + 3) * 3] = f(t);
		t = pesi[i + 4][(j + 0) * 3] + b / 2;
		pesi[i + 4][(j + 0) * 3] = f(t);
		t = pesi[i + 3][(j + 1) * 3] + b;
		pesi[i + 4][(j + 1) * 3] = f(t);
		t = pesi[i + 4][(j + 2) * 3] - c;
		pesi[i + 4][(j + 2) * 3] = f(t);
		t = pesi[i + 4][(j + 3) * 3] + b;
		pesi[i + 4][(j + 3) * 3] = f(t);
		return;
	}
	int a = (arg_4 + 2) / 8;//v1102
	int b = (arg_4 + 2) / 16;//v1103
	int c = a + b;
	int d = c * 2;//v1191
	int i = (arg_1C - arg_10) * 2 - 1, j = (arg_20 - arg_14) * 2 - 1;
	int t;
	t = pesi[i + 0][(j + 0) * 3] + b / 2;
	pesi[i + 0][(j + 0) * 3] = f(t);
	t = pesi[i + 0][(j + 1) * 3] + b;
	pesi[i + 0][(j + 1) * 3] = f(t);
	t = pesi[i + 0][(j + 2) * 3] - c;
	pesi[i + 0][(j + 2) * 3] = f(t);
	t = pesi[i + 0][(j + 3) * 3] + b;
	pesi[i + 0][(j + 3) * 3] = f(t);
	t = pesi[i + 0][(j + 4) * 3] + b / 2;
	pesi[i + 0][(j + 4) * 3] = f(t);
	t = pesi[i + 1][(j + 0) * 3] + b;
	pesi[i + 1][(j + 0) * 3] = f(t);
	t = pesi[i + 1][(j + 1) * 3] + a;
	pesi[i + 1][(j + 1) * 3] = f(t);
	t = pesi[i + 1][(j + 2) * 3] - d;
	pesi[i + 1][(j + 2) * 3] = f(t);
	t = pesi[i + 1][(j + 3) * 3] + a;
	pesi[i + 1][(j + 3) * 3] = f(t);
	t = pesi[i + 1][(j + 4) * 3] + b;
	pesi[i + 1][(j + 4) * 3] = f(t);
	t = pesi[i + 2][(j + 0) * 3] - c;
	pesi[i + 2][(j + 0) * 3] = f(t);
	t = pesi[i + 2][(j + 1) * 3] - d;
	pesi[i + 2][(j + 1) * 3] = f(t);
	t = pesi[i + 2][(j + 2) * 3] + d * 3;
	pesi[i + 2][(j + 2) * 3] = f(t);
	t = pesi[i + 2][(j + 3) * 3] - d;
	pesi[i + 2][(j + 3) * 3] = f(t);
	t = pesi[i + 2][(j + 4) * 3] - c;
	pesi[i + 2][(j + 4) * 3] = f(t);
	t = pesi[i + 3][(j + 0) * 3] + b;
	pesi[i + 3][(j + 0) * 3] = f(t);
	t = pesi[i + 3][(j + 1) * 3] + a;
	pesi[i + 3][(j + 1) * 3] = f(t);
	t = pesi[i + 3][(j + 2) * 3] - d;
	pesi[i + 3][(j + 2) * 3] = f(t);
	t = pesi[i + 3][(j + 3) * 3] + a;
	pesi[i + 3][(j + 3) * 3] = f(t);
	t = pesi[i + 3][(j + 4) * 3] + b;
	pesi[i + 3][(j + 4) * 3] = f(t);
	t = pesi[i + 4][(j + 0) * 3] + b / 2;
	pesi[i + 4][(j + 0) * 3] = f(t);
	t = pesi[i + 3][(j + 1) * 3] + b;
	pesi[i + 4][(j + 1) * 3] = f(t);
	t = pesi[i + 4][(j + 2) * 3] - c;
	pesi[i + 4][(j + 2) * 3] = f(t);
	t = pesi[i + 4][(j + 3) * 3] + b;
	pesi[i + 4][j + 3] = f(t);
	t = pesi[i + 4][(j + 4) * 3] + b / 2;
	pesi[i + 4][(j + 4) * 3] = f(t);
	return;
}
void sub_1002C8F0(unsigned char ** arg_0, int arg_4, int arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20) {
#define f(t) t <= 255 ? (t < 0 ? 0 : t) : 255

	unsigned char ** pesi = arg_0;
	if (!arg_18) {
		if (!arg_1C) {
			if (arg_20 == arg_14) {
				int a = (arg_4 + 2) / 4;
				int b = (arg_4 + 2) / 8;
				int c = a + b;
				int j = 0, i = 0;
				int t;
				t = pesi[i + 0][j + 0] - a;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] + c * 2;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 0][j + 2] - a;
				pesi[i + 0][j + 2] = f(t);
				t = pesi[i + 0][j + 3] - b;
				pesi[i + 0][j + 3] = f(t);
				t = pesi[i + 1][j + 0] - b;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 1][j + 1] + c;
				pesi[i + 1][j + 1] = f(t);
				t = pesi[i + 1][j + 2] - b;
				pesi[i + 1][j + 2] = f(t);
				t = pesi[i + 1][j + 3] - b / 2;
				pesi[i + 1][j + 3] = f(t);
				return;
			}
			//loc_1002CA48
			else if (arg_20 == (arg_C - 1)) {
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//edi
				int c = a + b;//ebx
				int j = (arg_20 - arg_14) * 2 - 1, i = 0;//ebp
				int t;
				t = pesi[i + 0][j + 0] - b;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] - a;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 0][j + 2] + c * 2;
				pesi[i + 0][j + 2] = f(t);
				t = pesi[i + 0][j + 3] - a;
				pesi[i + 0][j + 3] = f(t);
				t = pesi[i + 1][j + 0] - b / 2;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 1][j + 1] - b;
				pesi[i + 1][j + 1] = f(t);
				t = pesi[i + 1][j + 2] + c;
				pesi[i + 1][j + 2] = f(t);
				t = pesi[i + 1][j + 3] - b;
				pesi[i + 1][j + 3] = f(t);
				return;
			}
			else {
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//edi
				int c = a + b;//arg_4
				int j = (arg_20 - arg_14) * 2 - 1, i = 0;//ebp
				int t;
				t = pesi[i + 0][j + 0] - b;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] - a;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 0][j + 2] + c * 2;
				pesi[i + 0][j + 2] = f(t);
				t = pesi[i + 0][j + 3] - a;
				pesi[i + 0][j + 3] = f(t);
				t = pesi[i + 0][j + 4] - b;
				pesi[i + 0][j + 4] = f(t);
				t = pesi[i + 1][j + 0] - b / 2;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 1][j + 1] - b;
				pesi[i + 1][j + 1] = f(t);
				t = pesi[i + 1][j + 2] + c;
				pesi[i + 1][j + 2] = f(t);
				t = pesi[i + 1][j + 3] - b;
				pesi[i + 1][j + 3] = f(t);
				t = pesi[i + 1][j + 4] - b / 2;
				pesi[i + 1][j + 4] = f(t);
				return;

			}
		}
		//loc_1002CD1F
		if (arg_1C == (arg_8 - 1) / 2) {
			if (arg_20 == arg_14) {
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//edi
				int c = a + b;
				int i = arg_1C * 2 - 1;
				int j = 0;//ebp
				int t;
				t = pesi[i + 0][j + 0] - b;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] + c;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 0][j + 2] - b;
				pesi[i + 0][j + 2] = f(t);
				t = pesi[i + 0][j + 3] - b / 2;
				pesi[i + 0][j + 3] = f(t);
				t = pesi[i + 1][j + 0] - a;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 1][j + 1] + a * 3;
				pesi[i + 1][j + 1] = f(t);
				t = pesi[i + 1][j + 2] - a;
				pesi[i + 1][j + 2] = f(t);
				t = pesi[i + 1][j + 3] - b;
				pesi[i + 1][j + 3] = f(t);
				return;
			}
			//loc_1002CE80
			else if (arg_20 == (arg_C - 1)) {
				int a = (arg_4 + 2) / 4;//edi
				int b = (arg_4 + 2) / 8;//ecx
				int c = a + b;
				int i = arg_1C * 2 - 1;
				int j = (arg_20 - arg_14) * 2 - 1;//ebp
				int t;
				t = pesi[i + 0][j + 0] - b / 2;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] - b;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 0][j + 2] + c;
				pesi[i + 0][j + 2] = f(t);
				t = pesi[i + 0][j + 3] - b;
				pesi[i + 0][j + 3] = f(t);
				t = pesi[i + 1][j + 0] - b;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 1][j + 1] - a;
				pesi[i + 1][j + 1] = f(t);
				t = pesi[i + 1][j + 2] + a * 3;
				pesi[i + 1][j + 2] = f(t);
				t = pesi[i + 1][j + 3] - a;
				pesi[i + 1][j + 3] = f(t);
				return;
			}
			else {
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//iesi
				int c = a + b;
				int i = arg_1C * 2 - 1;
				int j = (arg_20 - arg_14) * 2 - 1;//ebp
				int t;
				t = pesi[i + 0][j + 0] - b / 2;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] - b;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 0][j + 2] + c;
				pesi[i + 0][j + 2] = f(t);
				t = pesi[i + 0][j + 3] - b;
				pesi[i + 0][j + 3] = f(t);
				t = pesi[i + 0][j + 4] - b / 2;
				pesi[i + 0][j + 4] = f(t);
				t = pesi[i + 1][j + 0] - b;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 1][j + 1] - a;
				pesi[i + 1][j + 1] = f(t);
				t = pesi[i + 1][j + 2] + c * 2;
				pesi[i + 1][j + 2] = f(t);
				t = pesi[i + 1][j + 3] - a;
				pesi[i + 1][j + 3] = f(t);
				t = pesi[i + 1][j + 4] - b;
				pesi[i + 1][j + 4] = f(t);
				return;
			}
		}
		//loc_1002D16A
		else if (arg_1C == (arg_10 - 1)) {
			if (arg_20 == arg_14) {
				int i = arg_8 - 1;
				int j = 0;//ebp
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//edi
				int c = a + b;
				int t;
				t = pesi[i + 0][j + 0] - a;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] + 2 * c;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 0][j + 2] - a;
				pesi[i + 0][j + 2] = f(t);
				t = pesi[i + 0][j + 3] - b;
				pesi[i + 0][j + 3] = f(t);
				return;
			}
			//loc_1002D227
			else if (arg_20 == (arg_C - 1)) {
				int i = arg_8 - 1;
				int j = (arg_20 - arg_14) * 2 - 1;//ebp
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//edi
				int c = a + b;
				int t;
				t = pesi[i + 0][j + 0] - b;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] - a;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 0][j + 2] + c * 2;
				pesi[i + 0][j + 2] = f(t);
				t = pesi[i + 0][j + 3] - a;
				pesi[i + 0][j + 3] = f(t);
				return;
			}
			else {
				int i = arg_8 - 1;
				int j = (arg_20 - arg_14) * 2 - 1;//ebp
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//edi
				int c = a + b;
				int t;
				t = pesi[i + 0][j + 0] - b;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] - a;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 0][j + 2] + c * 2;
				pesi[i + 0][j + 2] = f(t);
				t = pesi[i + 0][j + 3] - a;
				pesi[i + 0][j + 3] = f(t);
				t = pesi[i + 0][j + 4] - b;
				pesi[i + 0][j + 4] = f(t);
				return;
			}
		}
		//loc_1002D3A2
		if (arg_20 == arg_14) {
			int a = (arg_4 + 2) / 4;//edi
			int b = (arg_4 + 2) / 8;//iesi
			int c = a + b;
			int i = arg_1C * 2 - 1;
			int j = 0;//ebp
			int t;
			t = pesi[i + 0][j + 0] - b;
			pesi[i + 0][j + 0] = f(t);
			t = pesi[i + 0][j + 1] + c;
			pesi[i + 0][j + 1] = f(t);
			t = pesi[i + 0][j + 2] - b;
			pesi[i + 0][j + 2] = f(t);
			t = pesi[i + 0][j + 3] - b / 2;
			pesi[i + 0][j + 3] = f(t);

			t = pesi[i + 1][j + 0] - b;//////////////?
			pesi[i + 1][j + 0] = f(t);
			t = pesi[i + 1][j + 1] - a;
			pesi[i + 1][j + 1] = f(t);
			t = pesi[i + 1][j + 2] + c * 2;
			pesi[i + 1][j + 2] = f(t);
			t = pesi[i + 1][j + 3] - a;
			pesi[i + 1][j + 3] = f(t);

			t = pesi[i + 2][j + 0] - b;
			pesi[i + 2][j + 0] = f(t);
			t = pesi[i + 2][j + 1] + c;
			pesi[i + 2][j + 1] = f(t);
			t = pesi[i + 2][j + 2] - b;
			pesi[i + 2][j + 2] = f(t);
			t = pesi[i + 2][j + 3] - b / 2;
			pesi[i + 2][j + 3] = f(t);
			return;
		}
		else if (arg_20 == (arg_C - 1)) {
			int a = (arg_4 + 2) / 4;//edi
			int b = (arg_4 + 2) / 8;//iesi
			int c = a + b;
			int i = arg_1C * 2 - 1;
			int j = (arg_20 - arg_14) * 2 - 1;//ebp
			int t;
			t = pesi[i + 0][j + 0] - b / 2;
			pesi[i + 0][j + 0] = f(t);
			t = pesi[i + 0][j + 1] - b;
			pesi[i + 0][j + 1] = f(t);
			t = pesi[i + 0][j + 2] + c;
			pesi[i + 0][j + 2] = f(t);
			t = pesi[i + 0][j + 3] - b;
			pesi[i + 0][j + 3] = f(t);
			t = pesi[i + 1][j + 0] - b;
			pesi[i + 1][j + 0] = f(t);
			t = pesi[i + 1][j + 1] - a;
			pesi[i + 1][j + 1] = f(t);
			t = pesi[i + 1][j + 2] + c * 2;
			pesi[i + 1][j + 2] = f(t);
			t = pesi[i + 1][j + 3] - a;
			pesi[i + 1][j + 3] = f(t);
			t = pesi[i + 2][j + 0] - b / 2;
			pesi[i + 2][j + 0] = f(t);
			t = pesi[i + 2][j + 1] - b;
			pesi[i + 2][j + 1] = f(t);
			t = pesi[i + 2][j + 2] + c;
			pesi[i + 2][j + 2] = f(t);
			t = pesi[i + 2][j + 3] - b;
			pesi[i + 2][j + 3] = f(t);
			return;
		}
		else {
			int a = (arg_4 + 2) / 4;//edi
			int b = (arg_4 + 2) / 8;//iesi
			int c = a + b;
			int i = arg_1C * 2 - 1;
			int j = (arg_20 - arg_14) * 2 - 1;//ebp
			int t;
			t = pesi[i + 0][j + 0] - b / 2;
			pesi[i + 0][j + 0] = f(t);
			t = pesi[i + 0][j + 1] - b;
			pesi[i + 0][j + 1] = f(t);
			t = pesi[i + 0][j + 2] + c;
			pesi[i + 0][j + 2] = f(t);
			t = pesi[i + 0][j + 3] - b;
			pesi[i + 0][j + 3] = f(t);
			t = pesi[i + 0][j + 4] - b / 2;
			pesi[i + 0][j + 4] = f(t);
			t = pesi[i + 1][j + 0] - b;
			pesi[i + 1][j + 0] = f(t);
			t = pesi[i + 1][j + 1] - a;
			pesi[i + 1][j + 1] = f(t);
			t = pesi[i + 1][j + 2] + c * 2;
			pesi[i + 1][j + 2] = f(t);
			t = pesi[i + 1][j + 3] - a;
			pesi[i + 1][j + 3] = f(t);
			t = pesi[i + 1][j + 4] - b;
			pesi[i + 1][j + 4] = f(t);
			t = pesi[i + 2][j + 0] - b / 2;
			pesi[i + 2][j + 0] = f(t);
			t = pesi[i + 2][j + 1] - b;
			pesi[i + 2][j + 1] = f(t);
			t = pesi[i + 2][j + 2] + c;
			pesi[i + 2][j + 2] = f(t);
			t = pesi[i + 2][j + 3] - b;
			pesi[i + 2][j + 3] = f(t);
			t = pesi[i + 2][j + 4] - b / 2;
			pesi[i + 2][j + 4] = f(t);
			return;
		}
	}
	//loc_1002D9C8
	if (arg_18 == 1) {
		if (arg_1C == arg_10) {
			if (!arg_20) {
				int a = (arg_4 + 2) / 4;//iesi
				int b = (arg_4 + 2) / 8;//iecx
				int c = a + b;//ebp
				int i = 0;
				int j = 0;
				int t;
				t = pesi[i + 0][j + 0] - a;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] - b;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 1][j + 0] + c * 2;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 1][j + 1] + c;
				pesi[i + 1][j + 1] = f(t);
				t = pesi[i + 2][j + 0] - a;
				pesi[i + 2][j + 0] = f(t);
				t = pesi[i + 2][j + 1] - b;
				pesi[i + 2][j + 1] = f(t);
				t = pesi[i + 3][j + 0] - b;
				pesi[i + 3][j + 0] = f(t);
				t = pesi[i + 3][j + 1] - b / 2;
				pesi[i + 3][j + 1] = f(t);
				return;
			}
			//loc_1002DB0F
			if (arg_20 == (arg_C - 1) / 2) {
				int a = (arg_4 + 2) / 4;//iedi
				int b = (arg_4 + 2) / 8;//iesi
				int c = a + b;//ebp
				int i = 0;
				int j = arg_20 * 2 - 1;
				int t;
				t = pesi[i + 0][j + 0] - b;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] - a;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 1][j + 0] + c;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 1][j + 1] + c * 2;
				pesi[i + 1][j + 1] = f(t);
				t = pesi[i + 2][j + 0] - b;
				pesi[i + 2][j + 0] = f(t);
				t = pesi[i + 2][j + 1] - a;
				pesi[i + 2][j + 1] = f(t);
				t = pesi[i + 3][j + 0] - b / 2;
				pesi[i + 3][j + 0] = f(t);
				t = pesi[i + 3][j + 1] - b;
				pesi[i + 3][j + 1] = f(t);
				return;
			}
			//loc_1002DC68
			else if (arg_20 == (arg_14 - 1)) {
				int a = (arg_4 + 2) / 4;//ieax
				int b = (arg_4 + 2) / 8;//iedi
				int c = a + b;//ebp
				int i = 0;
				int j = arg_20 * 2 - 1;
				int t;
				t = pesi[i + 0][j + 0] - a;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 1][j + 0] + c * 2;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 2][j + 0] - a;
				pesi[i + 2][j + 0] = f(t);
				t = pesi[i + 3][j + 0] - b;
				pesi[i + 3][j + 0] = f(t);
				return;
			}
			//loc_1002DD34
			else {
				int a = (arg_4 + 2) / 4;//iedi
				int b = (arg_4 + 2) / 8;//iesi
				int c = a + b;//ebp
				int i = 0;
				int j = arg_20 * 2 - 1;
				int t;
				t = pesi[i + 0][j + 0] - b;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] - a;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 0][j + 2] - b;
				pesi[i + 0][j + 2] = f(t);
				t = pesi[i + 1][j + 0] + c;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 1][j + 1] + c * 2;
				pesi[i + 1][j + 1] = f(t);
				t = pesi[i + 1][j + 2] + c;
				pesi[i + 1][j + 2] = f(t);
				t = pesi[i + 2][j + 0] - b;
				pesi[i + 2][j + 0] = f(t);
				t = pesi[i + 2][j + 1] - a;
				pesi[i + 2][j + 1] = f(t);
				t = pesi[i + 2][j + 2] - b;
				pesi[i + 2][j + 2] = f(t);
				t = pesi[i + 3][j + 0] - b / 2;
				pesi[i + 3][j + 0] = f(t);
				t = pesi[i + 3][j + 1] - b;
				pesi[i + 3][j + 1] = f(t);
				t = pesi[i + 3][j + 2] - b / 2;
				pesi[i + 3][j + 2] = f(t);
				return;
			}
		}
		//loc_1002DF0F
		else if (arg_1C == (arg_8 - 1)) {
			if (!arg_20) {
				int a = (arg_4 + 2) / 4;//iedx
				int b = (arg_4 + 2) / 8;//iedi
				int c = a + b;//ebp
				int i = (arg_1C - arg_10) * 2 - 1;
				int j = 0;
				int t;
				t = pesi[i + 0][j + 0] - b;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] - b / 2;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 1][j + 0] - a;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 1][j + 1] - b;
				pesi[i + 1][j + 1] = f(t);
				t = pesi[i + 2][j + 0] + c * 2;
				pesi[i + 2][j + 0] = f(t);
				t = pesi[i + 2][j + 1] + c;
				pesi[i + 2][j + 1] = f(t);
				t = pesi[i + 3][j + 0] - a;
				pesi[i + 3][j + 0] = f(t);
				t = pesi[i + 3][j + 1] - b;
				pesi[i + 3][j + 1] = f(t);
				return;
			}
			//loc_1002E068
			else if (arg_20 == (arg_C - 1) / 2) {
				int a = (arg_4 + 2) / 4;//edi
				int b = (arg_4 + 2) / 8;//edx
				int c = a + b;//ebp
				int i = (arg_1C - arg_10) * 2 - 1;
				int j = arg_20 * 2 - 1;
				int t;
				t = pesi[i + 0][j + 0] - b / 2;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] - b;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 1][j + 0] - b;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 1][j + 1] - a;
				pesi[i + 1][j + 1] = f(t);
				t = pesi[i + 2][j + 0] + c;
				pesi[i + 2][j + 0] = f(t);
				t = pesi[i + 2][j + 1] + c * 2;
				pesi[i + 2][j + 1] = f(t);
				t = pesi[i + 3][j + 0] - b;
				pesi[i + 3][j + 0] = f(t);
				t = pesi[i + 3][j + 1] - a;
				pesi[i + 3][j + 1] = f(t);
				return;
			}
			//loc_1002E1F9
			else if (arg_20 == (arg_14 - 1)) {
				int a = (arg_4 + 2) / 4;//ecx
				int b = (arg_4 + 2) / 8;//esi
				int c = a + b;//ebp
				int i = (arg_1C - arg_10) * 2 - 1;
				int j = arg_C - 1;
				int t;
				t = pesi[i + 0][j + 0] - b;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 1][j + 0] - a;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 2][j + 0] + c * 2;
				pesi[i + 2][j + 0] = f(t);
				t = pesi[i + 3][j + 0] - a;
				pesi[i + 3][j + 0] = f(t);
				return;
			}
			//loc_1002E2DC
			else {
				int a = (arg_4 + 2) / 4;//ebp
				int b = (arg_4 + 2) / 8;//esi
				int c = a + b;//ebp
				int i = (arg_1C - arg_10) * 2 - 1;
				int j = arg_20 * 2 - 1;
				int t;
				t = pesi[i + 0][j + 0] - b / 2;
				pesi[i + 0][j + 0] = f(t);
				t = pesi[i + 0][j + 1] - b;
				pesi[i + 0][j + 1] = f(t);
				t = pesi[i + 0][j + 2] - b / 2;
				pesi[i + 0][j + 2] = f(t);
				t = pesi[i + 1][j + 0] - b;
				pesi[i + 1][j + 0] = f(t);
				t = pesi[i + 1][j + 1] - a;
				pesi[i + 1][j + 1] = f(t);
				t = pesi[i + 1][j + 2] - b;
				pesi[i + 1][j + 2] = f(t);
				t = pesi[i + 2][j + 0] + c;
				pesi[i + 2][j + 0] = f(t);
				t = pesi[i + 2][j + 1] + c * 2;
				pesi[i + 2][j + 1] = f(t);
				t = pesi[i + 2][j + 2] + c;
				pesi[i + 2][j + 2] = f(t);
				t = pesi[i + 3][j + 0] - b;
				pesi[i + 3][j + 0] = f(t);
				t = pesi[i + 3][j + 1] - a;
				pesi[i + 3][j + 1] = f(t);
				t = pesi[i + 3][j + 1] - b;
				pesi[i + 3][j + 1] = f(t);
				return;
			}
		}
		//loc_1002E4D2
		else if (!arg_20) {
			int a = (arg_4 + 2) / 4;//esi
			int b = (arg_4 + 2) / 8;//edx
			int c = a + b;//ebp
			int i = (arg_1C - arg_10) * 2 - 1;
			int j = 0;
			int t;
			t = pesi[i + 0][j + 0] - b;
			pesi[i + 0][j + 0] = f(t);
			t = pesi[i + 0][j + 1] - b / 2;
			pesi[i + 0][j + 1] = f(t);
			t = pesi[i + 1][j + 0] - a;
			pesi[i + 1][j + 0] = f(t);
			t = pesi[i + 1][j + 1] - b;
			pesi[i + 1][j + 1] = f(t);
			t = pesi[i + 2][j + 0] + c * 2;
			pesi[i + 2][j + 0] = f(t);
			t = pesi[i + 2][j + 1] + c;
			pesi[i + 2][j + 1] = f(t);
			t = pesi[i + 3][j + 0] - a;
			pesi[i + 3][j + 0] = f(t);
			t = pesi[i + 3][j + 1] - b;
			pesi[i + 3][j + 1] = f(t);
			t = pesi[i + 4][j + 0] - b;
			pesi[i + 4][j + 0] = f(t);
			t = pesi[i + 4][j + 1] - b / 2;
			pesi[i + 4][j + 1] = f(t);
			return;
		}
		//loc_1002E678
		else if (arg_20 == (arg_C - 1) / 2) {
			int a = (arg_4 + 2) / 4;//edi
			int b = (arg_4 + 2) / 8;//esi
			int c = a + b;//ebp
			int i = (arg_1C - arg_10) * 2 - 1;
			int j = 2 * arg_20 - 1;
			int t;
			t = pesi[i + 0][j + 0] - b / 2;
			pesi[i + 0][j + 0] = f(t);
			t = pesi[i + 0][j + 1] - b;
			pesi[i + 0][j + 1] = f(t);
			t = pesi[i + 1][j + 0] - b;
			pesi[i + 1][j + 0] = f(t);
			t = pesi[i + 1][j + 1] - a;
			pesi[i + 1][j + 1] = f(t);
			t = pesi[i + 2][j + 0] + c;
			pesi[i + 2][j + 0] = f(t);
			t = pesi[i + 2][j + 1] + c * 2;
			pesi[i + 2][j + 1] = f(t);
			t = pesi[i + 3][j + 0] - b;
			pesi[i + 3][j + 0] = f(t);
			t = pesi[i + 3][j + 1] - a;
			pesi[i + 3][j + 1] = f(t);
			t = pesi[i + 4][j + 0] - b / 2;
			pesi[i + 4][j + 0] = f(t);
			t = pesi[i + 4][j + 1] - b;
			pesi[i + 4][j + 1] = f(t);
			return;
		}
		//loc_1002E831
		else if (arg_20 == (arg_14 - 1)) {
			int a = (arg_4 + 2) / 4;//ecx
			int b = (arg_4 + 2) / 8;//edi
			int c = a + b;//ebp
			int i = (arg_1C - arg_10) * 2 - 1;
			int j = arg_C - 1;
			int t;
			t = pesi[i + 0][j + 0] - b;
			pesi[i + 0][j + 0] = f(t);
			t = pesi[i + 1][j + 0] - a;
			pesi[i + 1][j + 0] = f(t);
			t = pesi[i + 2][j + 0] + c * 2;
			pesi[i + 2][j + 0] = f(t);
			t = pesi[i + 3][j + 0] - a;
			pesi[i + 3][j + 0] = f(t);
			t = pesi[i + 4][j + 0] - b;
			pesi[i + 4][j + 0] = f(t);
			return;
		}
		//loc_1002E938
		else {
			int a = (arg_4 + 2) / 4;//edi
			int b = (arg_4 + 2) / 8;//esi
			int c = a + b;//ebp
			int i = (arg_1C - arg_10) * 2 - 1;
			int j = 2 * arg_20 - 1;
			int t;
			t = pesi[i + 0][j + 0] - b / 2;
			pesi[i + 0][j + 0] = f(t);
			t = pesi[i + 0][j + 1] - b;
			pesi[i + 0][j + 1] = f(t);
			t = pesi[i + 0][j + 2] - b / 2;
			pesi[i + 0][j + 2] = f(t);
			t = pesi[i + 1][j + 0] - b;
			pesi[i + 1][j + 0] = f(t);
			t = pesi[i + 1][j + 1] - a;
			pesi[i + 1][j + 1] = f(t);
			t = pesi[i + 1][j + 2] - b;
			pesi[i + 1][j + 2] = f(t);
			t = pesi[i + 2][j + 0] + c;
			pesi[i + 2][j + 0] = f(t);
			t = pesi[i + 2][j + 1] + c * 2;
			pesi[i + 2][j + 1] = f(t);
			t = pesi[i + 2][j + 2] + c;
			pesi[i + 2][j + 2] = f(t);
			t = pesi[i + 3][j + 0] - b;
			pesi[i + 3][j + 0] = f(t);
			t = pesi[i + 3][j + 1] - a;
			pesi[i + 3][j + 1] = f(t);
			t = pesi[i + 3][j + 2] - b;
			pesi[i + 3][j + 2] = f(t);
			t = pesi[i + 4][j + 0] - b / 2;
			pesi[i + 4][j + 0] = f(t);
			t = pesi[i + 4][j + 1] - b;
			pesi[i + 4][j + 1] = f(t);
			t = pesi[i + 4][j + 2] - b / 2;
			pesi[i + 4][j + 2] = f(t);
			return;
		}
	}
	//loc_1002EBAC
	if (arg_1C == arg_10) {
		if (arg_20 == arg_14) {
			int a = (arg_4 + 2) / 8;//esi
			int b = (arg_4 + 2) / 16;//edi
			int c = a + b;
			int d = c * 2;//ebp
			int i = 0, j = 0;
			int t;
			t = pesi[i + 0][j + 0] + a;
			pesi[i + 0][j + 0] = f(t);
			t = pesi[i + 0][j + 1] - d;
			pesi[i + 0][j + 1] = f(t);
			t = pesi[i + 0][j + 2] + a;
			pesi[i + 0][j + 2] = f(t);
			t = pesi[i + 0][j + 3] + b;

			pesi[i + 0][j + 3] = f(t);
			t = pesi[i + 1][j + 0] - d;
			pesi[i + 1][j + 0] = f(t);
			t = pesi[i + 1][j + 1] + d * 3;
			pesi[i + 1][j + 1] = f(t);
			t = pesi[i + 1][j + 2] - d;
			pesi[i + 1][j + 2] = f(t);
			t = pesi[i + 1][j + 3] - c;
			pesi[i + 1][j + 3] = f(t);

			t = pesi[i + 2][j + 0] + a;
			pesi[i + 2][j + 0] = f(t);
			t = pesi[i + 2][j + 1] - d;
			pesi[i + 2][j + 1] = f(t);
			t = pesi[i + 2][j + 2] + a;
			pesi[i + 2][j + 2] = f(t);
			t = pesi[i + 2][j + 3] + b;
			pesi[i + 2][j + 3] = f(t);

			t = pesi[i + 3][j + 0] + b;
			pesi[i + 3][j + 0] = f(t);
			t = pesi[i + 3][j + 1] - c;
			pesi[i + 3][j + 1] = f(t);
			t = pesi[i + 3][j + 2] + b;
			pesi[i + 3][j + 2] = f(t);
			t = pesi[i + 3][j + 3] + b / 2;///////////////////?
			pesi[i + 3][j + 3] = f(t);
			return;
		}
		//loc_1002EE32
		else if (arg_20 == (arg_C - 1)) {
			int a = (arg_4 + 2) / 8;//esi
			int b = (arg_4 + 2) / 16;//edi
			int c = a + b;
			int d = c * 2;//ebp
			int i = 0, j = 2 * (arg_20 - arg_14) - 1;
			int t;
			t = pesi[i + 0][j + 0] + b;
			pesi[i + 0][j + 0] = f(t);
			t = pesi[i + 0][j + 1] + a;
			pesi[i + 0][j + 1] = f(t);
			t = pesi[i + 0][j + 2] - d;
			pesi[i + 0][j + 2] = f(t);
			t = pesi[i + 0][j + 3] + a;
			pesi[i + 0][j + 3] = f(t);
			t = pesi[i + 1][j + 0] - c;
			pesi[i + 1][j + 0] = f(t);
			t = pesi[i + 1][j + 1] - d;
			pesi[i + 1][j + 1] = f(t);
			t = pesi[i + 1][j + 2] + d * 3;
			pesi[i + 1][j + 2] = f(t);
			t = pesi[i + 1][j + 3] - d;
			pesi[i + 1][j + 3] = f(t);
			t = pesi[i + 2][j + 0] + b;
			pesi[i + 2][j + 0] = f(t);
			t = pesi[i + 2][j + 1] + a;
			pesi[i + 2][j + 1] = f(t);
			t = pesi[i + 2][j + 2] - d;
			pesi[i + 2][j + 2] = f(t);
			t = pesi[i + 2][j + 3] + a;
			pesi[i + 2][j + 3] = f(t);
			t = pesi[i + 3][j + 0] + b / 2;
			pesi[i + 3][j + 0] = f(t);
			t = pesi[i + 3][j + 1] + b;
			pesi[i + 3][j + 1] = f(t);
			t = pesi[i + 3][j + 2] - c;
			pesi[i + 3][j + 2] = f(t);
			t = pesi[i + 3][j + 3] + b;
			pesi[i + 3][j + 3] = f(t);
			return;
		}
		//loc_1002F0C0
		else {
			int a = (arg_4 + 2) / 8;//esi
			int b = (arg_4 + 2) / 16;//edi
			int c = a + b;
			int d = c * 2;//ebp
			int i = 0, j = 2 * (arg_20 - arg_14) - 1;
			int t;
			t = pesi[i + 0][j + 0] + b;
			pesi[i + 0][j + 0] = f(t);
			t = pesi[i + 0][j + 1] + a;
			pesi[i + 0][j + 1] = f(t);
			t = pesi[i + 0][j + 2] - d;
			pesi[i + 0][j + 2] = f(t);
			t = pesi[i + 0][j + 3] + a;
			pesi[i + 0][j + 3] = f(t);
			t = pesi[i + 0][j + 4] + b;
			pesi[i + 0][j + 4] = f(t);
			t = pesi[i + 1][j + 0] - c;
			pesi[i + 1][j + 0] = f(t);
			t = pesi[i + 1][j + 1] - d;
			pesi[i + 1][j + 1] = f(t);
			t = pesi[i + 1][j + 2] + d * 3;
			pesi[i + 1][j + 2] = f(t);
			t = pesi[i + 1][j + 3] - d;
			pesi[i + 1][j + 3] = f(t);
			t = pesi[i + 1][j + 4] - c;
			pesi[i + 1][j + 4] = f(t);
			t = pesi[i + 2][j + 0] + b;
			pesi[i + 2][j + 0] = f(t);
			t = pesi[i + 2][j + 1] + a;
			pesi[i + 2][j + 1] = f(t);
			t = pesi[i + 2][j + 2] - d;
			pesi[i + 2][j + 2] = f(t);
			t = pesi[i + 2][j + 3] + a;
			pesi[i + 2][j + 3] = f(t);
			t = pesi[i + 2][j + 4] + b;
			pesi[i + 2][j + 4] = f(t);
			t = pesi[i + 3][j + 0] + b / 2;
			pesi[i + 3][j + 0] = f(t);
			t = pesi[i + 3][j + 1] + b;
			pesi[i + 3][j + 1] = f(t);
			t = pesi[i + 3][j + 2] - c;
			pesi[i + 3][j + 2] = f(t);
			t = pesi[i + 3][j + 3] + b;
			pesi[i + 3][j + 3] = f(t);
			t = pesi[i + 3][j + 4] + b / 2;
			pesi[i + 3][j + 4] = f(t);
			return;
		}
	}
	//loc_1002F3DA
	if (arg_1C == (arg_8 - 1)) {
		if (arg_20 == arg_14) {
			int a = (arg_4 + 2) / 8;//esi
			int b = (arg_4 + 2) / 16;//edi
			int c = a + b;
			int d = c * 2;//ebp
			int i = (arg_1C - arg_10) * 2 - 1, j = 0;
			int t;
			t = pesi[i + 0][j + 0] + b;
			pesi[i + 0][j + 0] = f(t);
			t = pesi[i + 0][j + 1] - c;
			pesi[i + 0][j + 1] = f(t);
			t = pesi[i + 0][j + 2] + b;
			pesi[i + 0][j + 2] = f(t);
			t = pesi[i + 0][j + 3] + b / 2;
			pesi[i + 0][j + 3] = f(t);
			t = pesi[i + 1][j + 0] + a;
			pesi[i + 1][j + 0] = f(t);
			t = pesi[i + 1][j + 1] - d;
			pesi[i + 1][j + 1] = f(t);
			t = pesi[i + 1][j + 2] + a;
			pesi[i + 1][j + 2] = f(t);
			t = pesi[i + 1][j + 3] + b;
			pesi[i + 1][j + 3] = f(t);
			t = pesi[i + 2][j + 0] - d;
			pesi[i + 2][j + 0] = f(t);
			t = pesi[i + 2][j + 1] + d * 3;
			pesi[i + 2][j + 1] = f(t);
			t = pesi[i + 2][j + 2] - d;
			pesi[i + 2][j + 2] = f(t);
			t = pesi[i + 2][j + 3] - c;
			pesi[i + 2][j + 3] = f(t);
			t = pesi[i + 3][j + 0] + a;
			pesi[i + 3][j + 0] = f(t);
			t = pesi[i + 3][j + 1] - d;
			pesi[i + 3][j + 1] = f(t);
			t = pesi[i + 3][j + 2] + a;
			pesi[i + 3][j + 2] = f(t);
			t = pesi[i + 3][j + 3] + b;
			pesi[i + 3][j + 3] = f(t);
			return;
		}
		//loc_1002F674
		else if (arg_20 == (arg_C - 1)) {
			int a = (arg_4 + 2) / 8;//edi
			int b = (arg_4 + 2) / 16;//esi
			int c = a + b;
			int d = c * 2;//ebp
			int i = (arg_1C - arg_10) * 2 - 1, j = 2 * (arg_20 - arg_14) - 1;
			int t;
			t = pesi[i + 0][j + 0] + b / 2;
			pesi[i + 0][j + 0] = f(t);
			t = pesi[i + 0][j + 1] + b;
			pesi[i + 0][j + 1] = f(t);
			t = pesi[i + 0][j + 2] - c;
			pesi[i + 0][j + 2] = f(t);
			t = pesi[i + 0][j + 3] + b;
			pesi[i + 0][j + 3] = f(t);
			t = pesi[i + 1][j + 0] + b;
			pesi[i + 1][j + 0] = f(t);
			t = pesi[i + 1][j + 1] + a;
			pesi[i + 1][j + 1] = f(t);
			t = pesi[i + 1][j + 2] - d;
			pesi[i + 1][j + 2] = f(t);
			t = pesi[i + 1][j + 3] + a;
			pesi[i + 1][j + 3] = f(t);
			t = pesi[i + 2][j + 0] - c;
			pesi[i + 2][j + 0] = f(t);
			t = pesi[i + 2][j + 1] - d;
			pesi[i + 2][j + 1] = f(t);
			t = pesi[i + 2][j + 2] + d * 3;
			pesi[i + 2][j + 2] = f(t);
			t = pesi[i + 2][j + 3] - d;
			pesi[i + 2][j + 3] = f(t);
			t = pesi[i + 3][j + 0] + b;
			pesi[i + 3][j + 0] = f(t);
			t = pesi[i + 3][j + 1] + a;
			pesi[i + 3][j + 1] = f(t);
			t = pesi[i + 3][j + 2] - d;
			pesi[i + 3][j + 2] = f(t);
			t = pesi[i + 3][j + 3] + a;
			pesi[i + 3][j + 3] = f(t);
			return;
		}
		//loc_1002F935
		else {
			int a = (arg_4 + 2) / 8;//919
			int b = (arg_4 + 2) / 16;//920
			int c = a + b;
			int d = c * 2;//v1189
			int i = (arg_1C - arg_10) * 2 - 1, j = 2 * (arg_20 - arg_14) - 1;
			int t;
			t = pesi[i + 0][j + 0] + b / 2;
			pesi[i + 0][j + 0] = f(t);
			t = pesi[i + 0][j + 1] + b;
			pesi[i + 0][j + 1] = f(t);
			t = pesi[i + 0][j + 2] - c;
			pesi[i + 0][j + 2] = f(t);
			t = pesi[i + 0][j + 3] + b;
			pesi[i + 0][j + 3] = f(t);
			t = pesi[i + 0][j + 4] + b / 2;
			pesi[i + 0][j + 4] = f(t);
			t = pesi[i + 1][j + 0] + b;
			pesi[i + 1][j + 0] = f(t);
			t = pesi[i + 1][j + 1] + a;
			pesi[i + 1][j + 1] = f(t);
			t = pesi[i + 1][j + 2] - d;
			pesi[i + 1][j + 2] = f(t);
			t = pesi[i + 1][j + 3] + a;
			pesi[i + 1][j + 3] = f(t);
			t = pesi[i + 1][j + 4] + b;
			pesi[i + 1][j + 4] = f(t);
			t = pesi[i + 2][j + 0] - c;
			pesi[i + 2][j + 0] = f(t);
			t = pesi[i + 2][j + 1] - d;
			pesi[i + 2][j + 1] = f(t);
			t = pesi[i + 2][j + 2] + d * 3;
			pesi[i + 2][j + 2] = f(t);
			t = pesi[i + 2][j + 3] - d;
			pesi[i + 2][j + 3] = f(t);
			t = pesi[i + 2][j + 4] - c;
			pesi[i + 2][j + 4] = f(t);
			t = pesi[i + 3][j + 0] + b;
			pesi[i + 3][j + 0] = f(t);
			t = pesi[i + 3][j + 1] + a;
			pesi[i + 3][j + 1] = f(t);
			t = pesi[i + 3][j + 2] - d;
			pesi[i + 3][j + 2] = f(t);
			t = pesi[i + 3][j + 3] + a;
			pesi[i + 3][j + 3] = f(t);
			t = pesi[i + 3][j + 4] + b;
			pesi[i + 3][j + 4] = f(t);
			return;
		}
	}
	//loc_1002FC76
	if (arg_20 == arg_14) {
		int a = (arg_4 + 2) / 8;//v980
		int b = (arg_4 + 2) / 16;//v981
		int c = a + b;
		int d = c * 2;//v982
		int i = (arg_1C - arg_10) * 2 - 1, j = 0;
		int t;
		t = pesi[i + 0][j + 0] + b;
		pesi[i + 0][j + 0] = f(t);
		t = pesi[i + 0][j + 1] - c;
		pesi[i + 0][j + 1] = f(t);
		t = pesi[i + 0][j + 2] + b;
		pesi[i + 0][j + 2] = f(t);
		t = pesi[i + 0][j + 3] + b / 2;
		pesi[i + 0][j + 3] = f(t);
		t = pesi[i + 1][j + 0] + a;
		pesi[i + 1][j + 0] = f(t);
		t = pesi[i + 1][j + 1] - d;
		pesi[i + 1][j + 1] = f(t);
		t = pesi[i + 1][j + 2] + a;
		pesi[i + 1][j + 2] = f(t);
		t = pesi[i + 1][j + 3] + b;
		pesi[i + 1][j + 3] = f(t);
		t = pesi[i + 2][j + 0] - d;
		pesi[i + 2][j + 0] = f(t);
		t = pesi[i + 2][j + 1] + d * 3;
		pesi[i + 2][j + 1] = f(t);
		t = pesi[i + 2][j + 2] - d;
		pesi[i + 2][j + 2] = f(t);
		t = pesi[i + 2][j + 3] - c;
		pesi[i + 2][j + 3] = f(t);
		t = pesi[i + 3][j + 0] + a;
		pesi[i + 3][j + 0] = f(t);
		t = pesi[i + 3][j + 1] - d;
		pesi[i + 3][j + 1] = f(t);
		t = pesi[i + 3][j + 2] + a;
		pesi[i + 3][j + 2] = f(t);
		t = pesi[i + 3][j + 3] + b;
		pesi[i + 3][j + 3] = f(t);
		t = pesi[i + 4][j + 0] + b;
		pesi[i + 4][j + 0] = f(t);
		t = pesi[i + 3][j + 1] - c;
		pesi[i + 4][j + 1] = f(t);
		t = pesi[i + 4][j + 2] + b;
		pesi[i + 4][j + 2] = f(t);
		t = pesi[i + 4][j + 3] + b / 2;
		pesi[i + 4][j + 3] = f(t);
		return;
	}
	//loc_1002FF9D
	if (arg_20 == (arg_C - 1)) {
		int a = (arg_4 + 2) / 8;//v1041
		int b = (arg_4 + 2) / 16;//v1042
		int c = a + b;
		int d = c * 2;//v1190
		int i = (arg_1C - arg_10) * 2 - 1, j = (arg_20 - arg_14) * 2 - 1;
		int t;
		t = pesi[i + 0][j + 0] + b / 2;
		pesi[i + 0][j + 0] = f(t);
		t = pesi[i + 0][j + 1] + b;
		pesi[i + 0][j + 1] = f(t);
		t = pesi[i + 0][j + 2] - c;
		pesi[i + 0][j + 2] = f(t);
		t = pesi[i + 0][j + 3] + b;
		pesi[i + 0][j + 3] = f(t);
		t = pesi[i + 1][j + 0] + b;
		pesi[i + 1][j + 0] = f(t);
		t = pesi[i + 1][j + 1] + a;
		pesi[i + 1][j + 1] = f(t);
		t = pesi[i + 1][j + 2] - d;
		pesi[i + 1][j + 2] = f(t);
		t = pesi[i + 1][j + 3] + a;
		pesi[i + 1][j + 3] = f(t);
		t = pesi[i + 2][j + 0] - c;
		pesi[i + 2][j + 0] = f(t);
		t = pesi[i + 2][j + 1] - d;
		pesi[i + 2][j + 1] = f(t);
		t = pesi[i + 2][j + 2] + d * 3;
		pesi[i + 2][j + 2] = f(t);
		t = pesi[i + 2][j + 3] - d;
		pesi[i + 2][j + 3] = f(t);
		t = pesi[i + 3][j + 0] + b;
		pesi[i + 3][j + 0] = f(t);
		t = pesi[i + 3][j + 1] + a;
		pesi[i + 3][j + 1] = f(t);
		t = pesi[i + 3][j + 2] - d;
		pesi[i + 3][j + 2] = f(t);
		t = pesi[i + 3][j + 3] + a;
		pesi[i + 3][j + 3] = f(t);
		t = pesi[i + 4][j + 0] + b / 2;
		pesi[i + 4][j + 0] = f(t);
		t = pesi[i + 3][j + 1] + b;
		pesi[i + 4][j + 1] = f(t);
		t = pesi[i + 4][j + 2] - c;
		pesi[i + 4][j + 2] = f(t);
		t = pesi[i + 4][j + 3] + b;
		pesi[i + 4][j + 3] = f(t);
		return;
	}
	//loc_10030308
	int a = (arg_4 + 2) / 8;//v1102
	int b = (arg_4 + 2) / 16;//v1103
	int c = a + b;
	int d = c * 2;//v1191
	int i = (arg_1C - arg_10) * 2 - 1, j = (arg_20 - arg_14) * 2 - 1;
	int t;
	t = pesi[i + 0][j + 0] + b / 2;
	pesi[i + 0][j + 0] = f(t);
	t = pesi[i + 0][j + 1] + b;
	pesi[i + 0][j + 1] = f(t);
	t = pesi[i + 0][j + 2] - c;
	pesi[i + 0][j + 2] = f(t);
	t = pesi[i + 0][j + 3] + b;
	pesi[i + 0][j + 3] = f(t);
	t = pesi[i + 0][j + 4] + b / 2;
	pesi[i + 0][j + 4] = f(t);
	t = pesi[i + 1][j + 0] + b;
	pesi[i + 1][j + 0] = f(t);
	t = pesi[i + 1][j + 1] + a;
	pesi[i + 1][j + 1] = f(t);
	t = pesi[i + 1][j + 2] - d;
	pesi[i + 1][j + 2] = f(t);
	t = pesi[i + 1][j + 3] + a;
	pesi[i + 1][j + 3] = f(t);
	t = pesi[i + 1][j + 4] + b;
	pesi[i + 1][j + 4] = f(t);
	t = pesi[i + 2][j + 0] - c;
	pesi[i + 2][j + 0] = f(t);
	t = pesi[i + 2][j + 1] - d;
	pesi[i + 2][j + 1] = f(t);
	t = pesi[i + 2][j + 2] + d * 3;
	pesi[i + 2][j + 2] = f(t);
	t = pesi[i + 2][j + 3] - d;
	pesi[i + 2][j + 3] = f(t);
	t = pesi[i + 2][j + 4] - c;
	pesi[i + 2][j + 4] = f(t);
	t = pesi[i + 3][j + 0] + b;
	pesi[i + 3][j + 0] = f(t);
	t = pesi[i + 3][j + 1] + a;
	pesi[i + 3][j + 1] = f(t);
	t = pesi[i + 3][j + 2] - d;
	pesi[i + 3][j + 2] = f(t);
	t = pesi[i + 3][j + 3] + a;
	pesi[i + 3][j + 3] = f(t);
	t = pesi[i + 3][j + 4] + b;
	pesi[i + 3][j + 4] = f(t);
	t = pesi[i + 4][j + 0] + b / 2;
	pesi[i + 4][j + 0] = f(t);
	t = pesi[i + 3][j + 1] + b;
	pesi[i + 4][j + 1] = f(t);
	t = pesi[i + 4][j + 2] - c;
	pesi[i + 4][j + 2] = f(t);
	t = pesi[i + 4][j + 3] + b;
	pesi[i + 4][j + 3] = f(t);
	t = pesi[i + 4][j + 4] + b / 2;
	pesi[i + 4][j + 4] = f(t);
	return;
}
void sub_10030730(int ** arg_0, int arg_4, int arg_8, int arg_C, int arg_10, int arg_14, int arg_18, int arg_1C, int arg_20) {
	int ** pesi = arg_0;
	if (!arg_18) {
		if (!arg_1C) {//k
			if (arg_20 == arg_14) {//k
				int b = arg_4 * 4;//ecx
				int a = arg_4 * 8;//edx
				pesi[0][0] -= a;//1e990
				pesi[0][1] += a * 3;//1a550
				pesi[0][2] -= a;//1bf90
				pesi[0][3] -= b;//18528
				pesi[1][0] -= b;//1f368
				pesi[1][1] += (a + b);
				pesi[1][2] -= b;
				pesi[1][3] -= b / 2;
				return;
			}
			if (arg_20 == (arg_C - 1)) {//k
				int b = arg_4 * 4;//ecx
				int a = arg_4 * 8;//edx
				int c = a + b;//esi
				int j = ((arg_20 - arg_14) * 2 - 1)/**4*/;
				pesi[0][j + 0] -= b;
				pesi[0][j + 1] -= a;
				pesi[0][j + 2] += c * 2;
				pesi[0][j + 3] -= a;
				pesi[1][j + 0] -= b / 2;
				pesi[1][j + 1] -= b;
				pesi[1][j + 2] += c;
				pesi[1][j + 3] -= b;
				return;
			}
			int d = arg_4 * 2;//edx
			int j = (arg_20 - arg_14) * 2 - 1;//ebx
			int b = 4 * arg_4;//ecx
			int a = b * 2;//esi
			int c = a + b;
			pesi[0][j + 0] -= b;
			pesi[0][j + 1] -= a;
			pesi[0][j + 2] += c * 2;
			pesi[0][j + 3] -= a;
			pesi[0][j + 4] -= b;
			pesi[1][j + 0] -= b / 2;
			pesi[1][j + 1] -= b;
			pesi[1][j + 2] += c;
			pesi[1][j + 3] -= b;
			pesi[1][j + 4] -= b / 2;
			return;
		}
		if (arg_1C == (arg_8 - 1) / 2) {
			if (arg_20 == arg_14) {//k
				int b = arg_4 * 4;//edx
				int a = b * 2;//esi
				int c = a + b;//ebx
				pesi[arg_1C * 2 - 1][0] -= b;
				pesi[arg_1C * 2 - 1][1] += c;
				pesi[arg_1C * 2 - 1][2] -= b;
				pesi[arg_1C * 2 - 1][3] -= b / 2;
				//int iecx = a * 3;
				pesi[arg_1C * 2][0] -= a;
				pesi[arg_1C * 2][1] += a * 3;
				pesi[arg_1C * 2][2] -= a;
				pesi[arg_1C * 2][3] -= b;
				return;
			}
			if (arg_20 == (arg_C - 1))//k
			{
				int iecx = (arg_20 - arg_14) * 2 - 1;
				int b = arg_4 * 4;//edx
				int a = b * 2;//esi
				int c = a + b;
				pesi[arg_1C * 2 - 1][iecx + 0] -= b / 2;
				pesi[arg_1C * 2 - 1][iecx + 1] -= b;
				pesi[arg_1C * 2 - 1][iecx + 2] += c;
				pesi[arg_1C * 2 - 1][iecx + 3] -= b;
				//iecx = a * 3;
				pesi[arg_1C * 2][iecx + 0] -= b;
				pesi[arg_1C * 2][iecx + 1] -= a;
				pesi[arg_1C * 2][iecx + 2] += a * 3;
				pesi[arg_1C * 2][iecx + 3] -= a;
				return;
			}
			//k
			int iecx = (arg_20 - arg_14) * 2 - 1;
			//int iesi = arg_4 * 2;
			int b = arg_4 * 4;//edx
			int a = b * 2;//edi
			int c = a + b;//ebx
			pesi[arg_1C * 2 - 1][iecx + 0] -= b / 2;
			pesi[arg_1C * 2 - 1][iecx + 1] -= b;
			pesi[arg_1C * 2 - 1][iecx + 2] += c;
			pesi[arg_1C * 2 - 1][iecx + 3] -= b;
			pesi[arg_1C * 2 - 1][iecx + 4] -= b / 2;
			pesi[arg_1C * 2][iecx + 0] -= b;
			pesi[arg_1C * 2][iecx + 1] -= a;
			pesi[arg_1C * 2][iecx + 2] += c * 2;
			pesi[arg_1C * 2][iecx + 3] -= a;
			pesi[arg_1C * 2][iecx + 4] -= b;
			return;
		}
		//loc_10030A5D
		if (arg_1C == (arg_10 - 1))
		{
			if (arg_20 == arg_14)//k
			{
				int b = arg_4 * 4;//ecx
				int a = b * 2;//edx
				int c = a + b;//esi
				pesi[arg_8 - 1][0] -= a;
				//c *= 2;
				pesi[arg_8 - 1][1] += c * 2;
				pesi[arg_8 - 1][2] -= a;
				pesi[arg_8 - 1][3] -= b;
				return;
			}
			int b = arg_4 * 4;//ecx
			int a = b * 2;//edx
			if (arg_20 == (arg_C - 1)) {//k
				int c = a + b;
				int ieax = (arg_20 - arg_14) * 2 - 1;
				pesi[arg_8 - 1][ieax + 0] -= b;
				//b += a;
				pesi[arg_8 - 1][ieax + 1] -= a;
				//b *= 2;
				pesi[arg_8 - 1][ieax + 2] += c * 2;
				pesi[arg_8 - 1][ieax + 3] -= a;
				return;
			}
			//k
			int ieax = (arg_20 - arg_14) * 2 - 1;
			int c = (a + b);//esi * 2
			pesi[arg_8 - 1][ieax + 0] -= b;
			pesi[arg_8 - 1][ieax + 1] -= a;
			pesi[arg_8 - 1][ieax + 2] += c * 2;
			pesi[arg_8 - 1][ieax + 3] -= a;
			pesi[arg_8 - 1][ieax + 4] -= b;
			return;
		}
		if (arg_20 == arg_14)//k
		{
			int iedi = arg_4 * 2;
			int b = iedi * 2;//edx
			int a = b * 2;//esi
			int c = a + b;
			pesi[arg_1C * 2 - 1][0] -= b;
			pesi[arg_1C * 2 - 1][1] += c;
			pesi[arg_1C * 2 - 1][2] -= b;
			pesi[arg_1C * 2 - 1][3] -= b / 2;
			pesi[arg_1C * 2][0] -= a;
			pesi[arg_1C * 2][1] += (a * 3);
			pesi[arg_1C * 2][2] -= a;
			//a += b;
			pesi[arg_1C * 2][3] -= b;
			pesi[arg_1C * 2 + 1][0] -= b;
			pesi[arg_1C * 2 + 1][1] += c;
			pesi[arg_1C * 2 + 1][2] -= b;
			pesi[arg_1C * 2 + 1][3] -= b / 2;
			return;
		}
		//loc_10030BF5
		if (arg_20 == (arg_C - 1))//k
		{
			int iecx = (arg_20 - arg_14) * 2 - 1;
			int iedi = arg_4 * 2;
			int b = iedi * 2;//edx
			int a = b * 2;//esi
			int c = a + b;
			pesi[arg_1C * 2 - 1][iecx + 0] -= b / 2;
			pesi[arg_1C * 2 - 1][iecx + 1] -= b;
			pesi[arg_1C * 2 - 1][iecx + 2] += c;
			pesi[arg_1C * 2 - 1][iecx + 3] -= b;
			pesi[arg_1C * 2][iecx + 0] -= b;
			pesi[arg_1C * 2][iecx + 1] -= a;
			pesi[arg_1C * 2][iecx + 2] += (a * 3);
			pesi[arg_1C * 2][iecx + 3] -= a;
			//a += b;
			pesi[arg_1C * 2 + 1][iecx + 0] -= b / 2;
			pesi[arg_1C * 2 + 1][iecx + 1] -= b;
			pesi[arg_1C * 2 + 1][iecx + 2] += c;
			pesi[arg_1C * 2 + 1][iecx + 3] -= b;
			return;
		}
		//k
		int iecx = (arg_20 - arg_14) * 2 - 1;
		int iesi = arg_4 * 2;
		int b = iesi * 2;//edx
		pesi[arg_1C * 2 - 1][iecx + 0] -= b / 2;
		pesi[arg_1C * 2 - 1][iecx + 1] -= b;
		int a = b * 2;//edi
		pesi[arg_1C * 2 - 1][iecx + 2] += (a + b);
		pesi[arg_1C * 2 - 1][iecx + 3] -= b;
		pesi[arg_1C * 2 - 1][iecx + 4] -= b / 2;
		pesi[arg_1C * 2][iecx + 0] -= b;
		pesi[arg_1C * 2][iecx + 1] -= a;
		pesi[arg_1C * 2][iecx + 2] += (a + b) * 2;
		pesi[arg_1C * 2][iecx + 3] -= a;
		pesi[arg_1C * 2][iecx + 4] -= b;
		pesi[arg_1C * 2 + 1][iecx + 0] -= b / 2;
		pesi[arg_1C * 2 + 1][iecx + 1] -= b;
		pesi[arg_1C * 2 + 1][iecx + 2] += (a + b);
		pesi[arg_1C * 2 + 1][iecx + 3] -= b;
		pesi[arg_1C * 2 + 1][iecx + 4] -= b / 2;
		return;
	}
	//loc_10030D50
	if (arg_18 == 1) {
		if (arg_1C == arg_10) {
			if (!arg_20) {//k
				int b = arg_4 * 4;//eax
				int a = arg_4 * 8;//edx
				pesi[0][0] -= a;
				pesi[0][1] -= b;
				pesi[1][0] += (a + b) * 2;
				pesi[1][1] += (a + b);
				pesi[2][0] -= a;
				pesi[2][1] -= b;
				pesi[3][0] -= b;
				pesi[3][1] -= b / 2;
				return;
			}
			if (arg_20 == (arg_C - 1) / 2) {//k
				int iecx = arg_20 * 2 - 1;
				int b = arg_4 * 4;//eax
				int a = b * 2;//edx
				pesi[0][iecx + 0] -= b;
				pesi[0][iecx + 1] -= a;
				pesi[1][iecx + 0] += (a + b);
				pesi[1][iecx + 1] += (a + b) * 2;
				pesi[2][iecx + 0] -= b;
				pesi[2][iecx + 1] -= a;
				//a = b / 2;
				pesi[3][iecx + 0] -= b / 2;
				pesi[3][iecx + 1] -= b;
				return;
			}
			//loc_10030E70
			if (arg_20 == (arg_14 - 1)) {//k
				int b = arg_4 * 4;//ecx
				int a = b * 2;//edx
				pesi[0][arg_C - 1] -= a;
				pesi[1][arg_C - 1] += (a + b) * 2;
				pesi[2][arg_C - 1] -= a;
				pesi[3][arg_C - 1] -= b;
				return;
			}
			//loc_10030EC3 //k
			int ieax = arg_20 * 2 - 1;
			int b = arg_4 * 4;//ecx
			int a = b * 2;//esi
			pesi[0][ieax + 0] -= b;
			pesi[0][ieax + 1] -= a;
			pesi[0][ieax + 2] -= b;
			int c = a + b;//edi
			pesi[1][ieax + 0] += c;
			pesi[1][ieax + 1] += c * 2;
			pesi[1][ieax + 2] += c;
			pesi[2][ieax + 0] -= b;
			pesi[2][ieax + 1] -= a;
			pesi[2][ieax + 2] -= b;
			//int iedx = b / 2;
			pesi[3][ieax + 0] -= b / 2;
			pesi[3][ieax + 1] -= b;
			pesi[3][ieax + 2] -= b / 2;
			return;
		}
		//loc_10030F64
		if (arg_1C == (arg_8 - 1))
		{
			if (!arg_20)//k
			{
				int b = arg_4 * 4;//ecx
				int a = b * 2;//esi
				int ieax = (arg_1C - arg_10) * 2;
				int iebx = arg_4 * 2;
				pesi[ieax - 1][0] -= b;
				pesi[ieax - 1][1] -= b / 2;
				pesi[ieax][0] -= a;
				pesi[ieax][1] -= b;
				pesi[ieax + 1][0] += (a + b) * 2;
				pesi[ieax + 1][1] += (a + b);
				pesi[ieax + 2][0] -= a;
				pesi[ieax + 2][1] -= b;
				return;
			}
			//loc_10030FEB
			if (arg_20 == (arg_C - 1) / 2)//k
			{
				int ieax = (arg_1C - arg_10) * 2;
				int iecx = arg_20 * 2 - 1;
				int b = arg_4 * 4;//edx
				int a = b * 2;//edi
				int c = a + b;//ebp
				pesi[ieax - 1][iecx + 0] -= b / 2;
				pesi[ieax - 1][iecx + 1] -= b;
				pesi[ieax][iecx + 0] -= b;
				pesi[ieax][iecx + 1] -= a;
				pesi[ieax + 1][iecx + 0] += c;
				pesi[ieax + 1][iecx + 1] += c * 2;
				pesi[ieax + 2][iecx + 0] -= b;
				pesi[ieax + 2][iecx + 1] -= a;//?
				return;
			}
			if (arg_20 == (arg_14 - 1)) {//k
				int ieax = (arg_1C - arg_10) * 2;
				int iecx = arg_C - 1;
				int b = arg_4 * 4;//edx
				int a = b * 2;
				int c = a + b;
				pesi[ieax - 1][iecx + 0] -= b;
				//b = (b + a) * 2;
				pesi[ieax][iecx + 0] -= a;
				pesi[ieax + 1][iecx + 0] += c * 2;
				pesi[ieax + 2][iecx + 0] -= a;
				return;
			}
			//k
			int ieax = (arg_1C - arg_10) * 2;
			int iecx = arg_20 * 2 - 1;
			int iedi = arg_4 * 2;
			int b = iedi * 2;//edx
			pesi[ieax - 1][iecx + 0] -= b / 2;
			pesi[ieax - 1][iecx + 1] -= b;
			pesi[ieax - 1][iecx + 2] -= b / 2;
			int a = b * 2;//ebx
			pesi[ieax][iecx + 0] -= b;
			pesi[ieax][iecx + 1] -= a;
			pesi[ieax][iecx + 2] -= b;
			int c = a + b;//ebp
			pesi[ieax + 1][iecx + 0] += c;
			pesi[ieax + 1][iecx + 1] += c * 2;
			pesi[ieax + 1][iecx + 2] += c;
			pesi[ieax + 2][iecx + 0] -= b;
			pesi[ieax + 2][iecx + 1] -= a;
			pesi[ieax + 2][iecx + 2] -= b;
			return;
		}
		//loc_10031161
		if (!arg_20) {//k
			int b = arg_4 * 4;//ecx
			int a = b * 2;//edi
			int c = a + b;
			int ieax = (arg_1C - arg_10) * 2;
			pesi[ieax - 1][0] -= b;
			pesi[ieax - 1][1] -= b / 2;
			pesi[ieax][0] -= a;
			//int iebx = b / 2;
			pesi[ieax][1] -= b;
			pesi[ieax + 1][0] += c * 2;
			pesi[ieax + 1][1] += c;
			pesi[ieax + 2][0] -= a;
			pesi[ieax + 2][1] -= b;
			pesi[ieax + 3][0] -= b;
			pesi[ieax + 3][1] -= b / 2;
			return;
		}
		//loc_100311DA
		if (arg_20 == (arg_C - 1) / 2)//k
		{
			int ieax = (arg_1C - arg_10) * 2;
			int iecx = arg_20 * 2 - 1;
			int iebx = arg_4 * 2;
			int b = iebx * 2;//edx
			int a = b * 2;//ebp
			int c = a + b;//esi
			pesi[ieax - 1][iecx + 0] -= b / 2;
			pesi[ieax - 1][iecx + 1] -= b;
			pesi[ieax][iecx + 0] -= b;
			pesi[ieax][iecx + 1] -= a;
			pesi[ieax + 1][iecx + 0] += c;
			pesi[ieax + 1][iecx + 1] += c * 2;
			pesi[ieax + 2][iecx + 0] -= b;
			pesi[ieax + 2][iecx + 1] -= a;
			pesi[ieax + 3][iecx + 0] -= b / 2;
			pesi[ieax + 3][iecx + 1] -= b;
			return;
		}
		//loc_10031274
		if (arg_20 == (arg_14 - 1)) {//k
			int ieax = (arg_1C - arg_10) * 2;
			int iecx = (arg_C - 1);
			int b = arg_4 * 4;//edx
			int a = b * 2;//edi
			int c = a + b;
			pesi[ieax - 1][iecx + 0] -= b;
			pesi[ieax][iecx + 0] -= a;
			//int iebp = (b + a) * 2;
			pesi[ieax + 1][iecx + 0] += c * 2;
			pesi[ieax + 2][iecx + 0] -= a;
			pesi[ieax + 3][iecx + 0] -= b;
			return;
		}
		//k
		int ieax = (arg_1C - arg_10) * 2;
		int iesi = arg_4 * 2;
		int b = iesi * 2;
		int a = b * 2;//ebp
		int c = a + b;//edi
		int iedx = arg_20 * 2 - 1;
		pesi[ieax - 1][iedx + 0] -= b / 2;
		pesi[ieax - 1][iedx + 1] -= b;
		pesi[ieax - 1][iedx + 2] -= b / 2;
		pesi[ieax][iedx + 0] -= b;
		pesi[ieax][iedx + 1] -= a;
		pesi[ieax][iedx + 2] -= b;
		pesi[ieax + 1][iedx + 0] += c;
		pesi[ieax + 1][iedx + 1] += c * 2;
		pesi[ieax + 1][iedx + 2] += c;
		pesi[ieax + 2][iedx + 0] -= b;
		pesi[ieax + 2][iedx + 1] -= a;
		pesi[ieax + 2][iedx + 2] -= b;
		pesi[ieax + 3][iedx + 0] -= b / 2;
		pesi[ieax + 3][iedx + 1] -= b;
		pesi[ieax + 3][iedx + 2] -= b / 2;
		return;
	}
	//loc_10031376
	if (arg_1C == arg_10) {
		if (arg_20 == arg_14) {//k
			int b = arg_4 * 2;//ecx
			int a = b * 2;//esi
			int c = a + b;//edi
			int d = c * 2;//edx
			pesi[0][0] += a;
			pesi[0][1] -= d;
			pesi[0][2] += a;
			pesi[0][3] += b;

			pesi[1][0] -= d;
			pesi[1][1] += d * 3;
			pesi[1][2] -= d;
			pesi[1][3] -= c;

			pesi[2][0] += a;
			pesi[2][1] -= d;
			pesi[2][2] += a;
			pesi[2][3] += b;

			pesi[3][0] += b;
			pesi[3][1] -= c;
			pesi[3][2] += b;
			pesi[3][3] += b / 2;
			return;
		}
		//loc_1003144A
		if (arg_20 == (arg_C - 1)) {//k
			int j = (arg_20 - arg_14) * 2 - 1;
			int b = arg_4 * 2;//ecx
			int a = b * 2;//esi
			int c = a + b;//ebx
			int d = c * 2;//edx
			pesi[0][j + 0] += b;
			pesi[0][j + 1] += a;
			pesi[0][j + 2] -= d;
			pesi[0][j + 3] += a;
			pesi[1][j + 0] -= c;
			pesi[1][j + 1] -= d;
			pesi[1][j + 2] += d * 3;
			pesi[1][j + 3] -= d;
			pesi[2][j + 0] += b;
			pesi[2][j + 1] += a;
			pesi[2][j + 2] -= d;
			pesi[2][j + 3] += a;
			pesi[3][j + 0] += b / 2;
			pesi[3][j + 1] += b;
			pesi[3][j + 2] -= c;
			pesi[3][j + 3] += b;
			return;
		}
		//loc_10031509 //k
		int ieax = (arg_20 - arg_14) * 2 - 1;
		int b = arg_4 * 2;//ecx
		int a = b * 2;
		int c = a + b;//edi
		int d = c * 2;//edx
		pesi[0][ieax + 0] += b;
		pesi[0][ieax + 1] += a;
		pesi[0][ieax + 2] -= d;
		pesi[0][ieax + 3] += a;
		pesi[0][ieax + 4] += b;
		pesi[1][ieax + 0] -= c;
		pesi[1][ieax + 1] -= d;
		pesi[1][ieax + 2] += d * 3;
		pesi[1][ieax + 3] -= d;
		pesi[1][ieax + 4] -= c;
		pesi[2][ieax + 0] += b;
		pesi[2][ieax + 1] += a;
		pesi[2][ieax + 2] -= d;
		pesi[2][ieax + 3] += a;
		pesi[2][ieax + 4] += b;
		pesi[3][ieax + 0] += b / 2;
		pesi[3][ieax + 1] += b;
		pesi[3][ieax + 2] -= c;
		pesi[3][ieax + 3] += b;
		pesi[3][ieax + 4] += b / 2;
		return;
	}
	//loc_100315DE
	if (arg_1C == (arg_8 - 1)) {
		if (arg_20 == arg_14)//k
		{
			int iecx = (arg_1C - arg_10) * 2;
			int b = arg_4 * 2;//edx
			int a = b * 2;//edi
			int c = a + b;//ebx
			int d = c * 2;//esi
			pesi[iecx - 1][0] += b;
			pesi[iecx - 1][1] -= c;
			pesi[iecx - 1][2] += b;
			pesi[iecx - 1][3] += b / 2;
			pesi[iecx][0] += a;
			pesi[iecx][1] -= d;
			pesi[iecx][2] += a;
			pesi[iecx][3] += b;
			pesi[iecx + 1][0] -= d;
			pesi[iecx + 1][1] += d * 3;
			pesi[iecx + 1][2] -= d;
			pesi[iecx + 1][3] -= c;
			pesi[iecx + 2][0] += a;
			pesi[iecx + 2][1] -= d;
			pesi[iecx + 2][2] += a;
			pesi[iecx + 2][3] += b;
			return;
		}
		//loc_1003169D
		if (arg_20 == (arg_C - 1))//k
		{
			int ieax = (arg_1C - arg_10) * 2;
			int iecx = (arg_20 - arg_14) * 2 - 1;
			int b = arg_4 * 2;//edx
			int a = b * 2;//iedi
			int d = (a + b) * 2;//esi
			int c = (a + b);
			pesi[ieax - 1][iecx + 0] += b / 2;
			pesi[ieax - 1][iecx + 1] += b;
			pesi[ieax - 1][iecx + 2] -= c;
			pesi[ieax - 1][iecx + 3] += b;
			pesi[ieax][iecx + 0] += b;
			pesi[ieax][iecx + 1] += a;
			pesi[ieax][iecx + 2] -= d;
			pesi[ieax][iecx + 3] += a;
			pesi[ieax + 1][iecx + 0] -= c;
			pesi[ieax + 1][iecx + 1] -= d;
			pesi[ieax + 1][iecx + 2] += d * 3;
			pesi[ieax + 1][iecx + 3] -= d;
			pesi[ieax + 2][iecx + 0] += b;
			pesi[ieax + 2][iecx + 1] += a;
			pesi[ieax + 2][iecx + 2] -= d;
			pesi[ieax + 2][iecx + 3] += a;
			return;
		}
		//loc_10031768
		//k
		int ieax = (arg_1C - arg_10) * 2;
		int iecx = (arg_20 - arg_14) * 2 - 1;
		int b = arg_4 * 2;//edx
		int a = b * 2;//edi
		int c = a + b;//ebx
		int d = c * 2;//esi
		pesi[ieax - 1][iecx + 0] += b / 2;
		pesi[ieax - 1][iecx + 1] += b;
		pesi[ieax - 1][iecx + 2] -= c;
		pesi[ieax - 1][iecx + 3] += b;
		pesi[ieax - 1][iecx + 4] += b / 2;
		pesi[ieax][iecx + 0] += b;
		pesi[ieax][iecx + 1] += a;
		pesi[ieax][iecx + 2] -= d;
		pesi[ieax][iecx + 3] += a;
		pesi[ieax][iecx + 4] += b;
		pesi[ieax + 1][iecx + 0] -= c;
		pesi[ieax + 1][iecx + 1] -= d;
		pesi[ieax + 1][iecx + 2] += d * 3;
		pesi[ieax + 1][iecx + 3] -= d;
		pesi[ieax + 1][iecx + 4] -= c;
		pesi[ieax + 2][iecx + 0] += b;
		pesi[ieax + 2][iecx + 1] += a;
		pesi[ieax + 2][iecx + 2] -= d;
		pesi[ieax + 2][iecx + 3] += a;
		pesi[ieax + 2][iecx + 4] += b;
		return;
	}
	//loc_1003184F
	if (arg_20 == arg_14) {//k
		int ieax = (arg_1C - arg_10) * 2;
		int b = arg_4 * 2;//edx
		int a = b * 2;//edi
		int c = a + b;//ebx
		int d = c * 2;//esi
		pesi[ieax - 1][0] += b;
		pesi[ieax - 1][1] -= c;
		pesi[ieax - 1][2] += b;
		pesi[ieax - 1][3] += b / 2;
		pesi[ieax][0] += a;
		pesi[ieax][1] -= d;
		pesi[ieax][2] += a;
		pesi[ieax][3] += b;
		pesi[ieax + 1][0] -= d;
		pesi[ieax + 1][1] += d * 3;
		pesi[ieax + 1][2] -= d;
		pesi[ieax + 1][3] -= c;
		pesi[ieax + 2][0] += a;
		pesi[ieax + 2][1] -= d;
		pesi[ieax + 2][2] += a;
		pesi[ieax + 2][3] += b;
		pesi[ieax + 3][0] += b;
		pesi[ieax + 3][1] -= c;
		pesi[ieax + 3][2] += b;
		pesi[ieax + 3][3] += b / 2;
		return;
	}
	//loc_1003191C
	if (arg_20 == (arg_C - 1))//k
	{
		int ieax = (arg_1C - arg_10) * 2;
		int iecx = (arg_20 - arg_14) * 2 - 1;
		int b = arg_4 * 2;//edx
		int a = b * 2;//edi
		int c = (a + b);//arg20
		int d = c * 2;//esi
		pesi[ieax - 1][iecx + 0] += b / 2;
		pesi[ieax - 1][iecx + 1] += b;
		pesi[ieax - 1][iecx + 2] -= c;
		pesi[ieax - 1][iecx + 3] += b;
		pesi[ieax][iecx + 0] += b;
		pesi[ieax][iecx + 1] += a;
		pesi[ieax][iecx + 2] -= d;
		pesi[ieax][iecx + 3] += a;
		pesi[ieax + 1][iecx + 0] -= c;
		pesi[ieax + 1][iecx + 1] -= d;
		pesi[ieax + 1][iecx + 2] += d * 3;
		pesi[ieax + 1][iecx + 3] -= d;
		pesi[ieax + 2][iecx + 0] += b;
		pesi[ieax + 2][iecx + 1] += a;
		pesi[ieax + 2][iecx + 2] -= d;
		pesi[ieax + 2][iecx + 3] += a;
		pesi[ieax + 3][iecx + 0] += b / 2;
		pesi[ieax + 3][iecx + 1] += b;
		pesi[ieax + 3][iecx + 2] -= c;
		pesi[ieax + 3][iecx + 3] += b;
		return;
	}
	int ieax = (arg_1C - arg_10) * 2;
	int iecx = (arg_20 - arg_14) * 2 - 1;
	int b = arg_4 * 2;//edx
	int a = b * 2;//edi
	int c = a + b;//ebx
	int d = c * 2;//esi
	pesi[ieax - 1][iecx + 0] += b / 2;
	pesi[ieax - 1][iecx + 1] += b;
	pesi[ieax - 1][iecx + 2] -= c;
	pesi[ieax - 1][iecx + 3] += b;
	pesi[ieax - 1][iecx + 4] += b / 2;
	pesi[ieax][iecx + 0] += b;
	pesi[ieax][iecx + 1] += a;
	pesi[ieax][iecx + 2] -= d;
	pesi[ieax][iecx + 3] += a;
	pesi[ieax][iecx + 4] += b;
	pesi[ieax + 1][iecx + 0] -= c;
	pesi[ieax + 1][iecx + 1] -= d;
	pesi[ieax + 1][iecx + 2] += d * 3;
	pesi[ieax + 1][iecx + 3] -= d;
	pesi[ieax + 1][iecx + 4] -= c;
	pesi[ieax + 2][iecx + 0] += b;
	pesi[ieax + 2][iecx + 1] += a;
	pesi[ieax + 2][iecx + 2] -= d;
	pesi[ieax + 2][iecx + 3] += a;
	pesi[ieax + 2][iecx + 4] += b;
	pesi[ieax + 3][iecx + 0] += b / 2;
	pesi[ieax + 3][iecx + 1] += b;
	pesi[ieax + 3][iecx + 2] -= c;
	pesi[ieax + 3][iecx + 3] += b;
	pesi[ieax + 3][iecx + 4] += b / 2;
}
//k
T_size_31C* sub_10031B10() {
	return (T_size_31C*)AllocateMemorySize(sizeof(T_size_31C));
}
//k
void sub_10031C60(T_size_24 * arg_0, T_size_31C* arg_4) {
	T_size_24 * pesi = arg_0;
	for (; !sub_1000AB10(pesi, 1);) {
		if ((unsigned char)pesi->dw0x14 & 1)
			break;
	}
	sub_10031CA0(arg_4);
}
//k
void sub_10031CA0(T_size_31C* arg_0) {
	T_size_31C* pedi = arg_0;
	sub_10031D20(pedi->hg_000[0]);
	for (int i = -2; i <= -1; i++) {//esi
		if (pedi->hg_000[i])
		{
			unsigned char* heax = pedi->hg_000[i]->pb_08;
			if (heax) {
				DeallocateMemory(heax);
			}
			T_size_Above_C ** hheax = pedi->hg_000[i]->ppt_04;
			if (hheax)
				DeallocateMemory(hheax);
			T_size_Above_C * heax1 = pedi->hg_000[i];
			DeallocateMemory(heax1);
		}
	}
	pedi->hg_000 = &pedi->hg_000[-2];
	DeallocateMemory(pedi->hg_000);
	DeallocateMemory(pedi);
}
//k
void sub_10031D20(T_size_Above_C* arg_0) {
	T_size_Above_C* pedi = arg_0;
	if (pedi->ppt_04) {
		for (int i = 0; i <= pedi->dw_00; i++) {//esi
			if (pedi->ppt_04[i]) {
				sub_10031D20(pedi->ppt_04[i]);
			}
		}
		DeallocateMemory(pedi->ppt_04);
	}
	if (pedi->pb_08) {
		DeallocateMemory(pedi->pb_08);
	}
	DeallocateMemory(pedi);
}
//k
void sub_10031D80(int arg_0, T_size_24 * arg_4, T_size_31C* arg_8) {
	T_size_31C * pesi = arg_8;
	pesi->dw_30C = arg_0;
	sub_100327B0(arg_4, arg_8);
}
//k
int sub_10031DB0(T_size_24 * arg_0, T_size_31C * arg_4) {
	T_size_31C * pesi = arg_4;
	if ((unsigned char)arg_0->dw0x14 & 1) {
		return 0;
	}
	int iedi;
	unsigned short var_8[4];
	do {
		sub_10032270(var_8, pesi);
		iedi = sub_100322A0(sub_10032770(var_8, pesi), var_8, pesi);
		sub_10032800(arg_0, var_8, pesi);
	} while (iedi == 0x100);
	if (iedi == -1) {
		return -1;
	}
	sub_10032040(iedi, pesi);
	sub_10032330(iedi, pesi);
	return iedi;
}
//k
int sub_10031E40(T_size_24 * arg_0, T_size_31C* arg_4) {
	int result;
	result = sub_10031DB0(arg_0, arg_4);
	if (result >= 253)
		result = 253 * sub_10031DB0(arg_0, arg_4) + sub_10031DB0(arg_0, arg_4);
	return result;
}
//k
T_size_Above_C* sub_10031FA0(T_size_Above_C* arg_0 /*out*/, int arg_4, T_size_Above_C* arg_8) {
	T_size_Above_C* pesi = arg_0;
	int i;
	for (i = 0; i <= pesi->dw_00; i++) {//iedi
		if (pesi->pb_08[i * 2] == arg_4) break;
	}
	if (i > pesi->dw_00) {
		pesi->dw_00++;
		if (!pesi->ppt_04) {
			pesi->ppt_04 = (T_size_Above_C**)AllocateMemorySize((pesi->dw_00 + 1) * 4 * factor64);
		}
		else
		{
			size_t vlen = (pesi->dw_00 + 1) * 4 * factor64;
			pesi->ppt_04 = (T_size_Above_C**)ReallocateMemoryAndCopyContents(pesi->ppt_04, vlen, pesi->ppt_04Len);
			pesi->ppt_04Len = vlen;
		}
		if (!pesi->pb_08) {
			pesi->pb_08 = (unsigned char*)AllocateMemorySize((pesi->dw_00 + 1) * 2);
		}
		else
		{
			size_t vlen = (pesi->dw_00 + 1) * 2;
			pesi->pb_08 = (unsigned char*)ReallocateMemoryAndCopyContents(pesi->pb_08, vlen, pesi->pb_08Len);
			pesi->pb_08Len = vlen;
		}
		pesi->pb_08[i * 2] = arg_4;
		pesi->pb_08[i * 2 + 1] = 0;
	}
	T_size_Above_C* peax = (T_size_Above_C*)AllocateMemorySize(sizeof(T_size_Above_C)); // 17
	peax->dw_00 = -1;
	peax->ppt_04[i] = peax;
	peax->pt_0C = arg_8;
	return peax;
}
//k
void sub_10032040(int arg_0, T_size_31C * arg_4) {
	T_size_31C * pesi = arg_4;
	int i = pesi->dw_308 < 0 ? 0 : pesi->dw_308;//edi
	if (arg_0 >= 0) {
		for (; i <= pesi->dw_30C; i++) {
			sub_100320B0(pesi->hg_000[i], arg_0, pesi);
		}
	}
	pesi->dw_308 = pesi->dw_30C;
	memset(pesi->ab_208, 0, 0x40);
}
//k
void sub_100320B0(T_size_Above_C* arg_0 /*out*/, int arg_4, T_size_31C * arg_8) {
	T_size_Above_C * pedi = arg_0;
	int i;
	for (i = 0; i < pedi->dw_00; i++) {//esi
		if (pedi->pb_08[i * 2] == (unsigned char)arg_4) {
			break;
		}
	}
	if (i > pedi->dw_00) {
		pedi->dw_00++;
		if (arg_8->dw_308 < arg_8->dw_30C) {
			if (!pedi->dw_00) {
				pedi->ppt_04 = (T_size_Above_C**)AllocateMemorySize((pedi->dw_00 + 1) * 4 * factor64);
			}
			else {
				size_t vlen = (pedi->dw_00 + 1) * 4 * factor64;
				pedi->ppt_04 = (T_size_Above_C**)ReallocateMemoryAndCopyContents(pedi->ppt_04, vlen, pedi->ppt_04Len);
				pedi->ppt_04Len = vlen;

			}
			pedi->ppt_04[i] = 0;
		}
		if (!pedi->dw_00) {
			pedi->pb_08 = (unsigned char*)AllocateMemorySize((pedi->dw_00 + 1) * 2);
		}
		else {
			size_t vlen = (pedi->dw_00 + 1) * 2;
			pedi->pb_08 = (unsigned char*)ReallocateMemoryAndCopyContents(pedi->pb_08, vlen, pedi->pb_08Len);
			pedi->pb_08Len = vlen;
		}
		pedi->pb_08[i * 2] = (unsigned char)arg_4;
		pedi->pb_08[i * 2 + 1] = 0;
	}
	int ieax;
	for (ieax = i; ieax > 0; ieax--) {
		if (pedi->pb_08[i * 2 + 1] != pedi->pb_08[(ieax - 1) * 2 + 1]) {
			break;
		}
	}
	if (ieax != i) {
		unsigned char temp = pedi->pb_08[i * 2];
		pedi->pb_08[i * 2] = pedi->pb_08[ieax * 2];
		pedi->pb_08[ieax * 2] = temp;
		if (pedi->ppt_04) {
			T_size_Above_C* temp = pedi->ppt_04[i];
			pedi->ppt_04[i] = pedi->ppt_04[ieax];
			pedi->ppt_04[ieax] = temp;
		}
		i = ieax;
	}
	pedi->pb_08[i * 2 + 1] ++;
	if (pedi->pb_08[i * 2 + 1] == 0x255) {
		sub_100323F0(pedi);
	}
}
//k
void sub_10032270(unsigned short * arg_0 /*out*/, T_size_31C * arg_4) {
	T_size_31C * pesi = arg_4;
	sub_10032470(pesi->hg_000[pesi->dw_308], pesi);
	arg_0[2] = pesi->aw_004[0];
}
//k
void sub_10032330(int arg_0, T_size_31C * arg_4 /*out*/) {
	T_size_31C * pesi = arg_4;
	if (pesi->dw_30C >= 0) {
		if (arg_0 >= 0) {
			pesi->hg_000[pesi->dw_30C] = sub_10032390(pesi->hg_000[pesi->dw_30C], arg_0, pesi->dw_30C);
			for (int i = pesi->dw_30C - 1; i > 0; i--) {//eax
				pesi->hg_000[i] = pesi->hg_000[i + 1]->pt_0C;
			}
		}
	}
}
//k
T_size_Above_C* sub_10032390(T_size_Above_C* arg_0, int arg_4, int arg_8) {
	T_size_Above_C *peax = arg_0;
	T_size_Above_C* pedi = peax->pt_0C;
	if (!arg_8) {
		return pedi->ppt_04[0];
	}
	for (int i = 0; i <= pedi->dw_00; i++) {
		if (pedi->pb_08[i * 2] == (unsigned char)arg_4) {
			if (!pedi->ppt_04[i])
				break;
			else return pedi->ppt_04[i];
		}
	}
	return sub_10031FA0(pedi, arg_4, sub_10032390(pedi, arg_4, arg_8 - 1));
}
//k
int sub_10032770(unsigned short * arg_0, T_size_31C * arg_4) {
	T_size_31C * pedx = arg_4;
	return ((pedx->w_310 - pedx->w_312 + 1) * arg_0[2] - 1) / (pedx->w_314 - pedx->w_312 + 1);
}
//k
int sub_100322A0(int arg_0, unsigned short * arg_4, T_size_31C * arg_8) {
	T_size_31C * pesi = arg_8;
	T_size_Above_C* pedi = pesi->hg_000[pesi->dw_308];
	int i;
	for (i = 0; arg_0 < pesi->aw_004[i]; i++);//ecx
	arg_4[1] = pesi->aw_004[i + 1];
	arg_4[0] = pesi->aw_004[i + 2];
	if (i == 1) {
		pesi->dw_308--;
		return 0x100;
	}
	if (pesi->dw_308 < -1)
	{
		return -pedi->pb_08[i * 2 - 2];
	}
	return pedi->pb_08[i * 2 - 2];
}
//k
void sub_100323F0(T_size_Above_C* arg_0) {
	T_size_Above_C *pesi = arg_0;
	if (pesi->dw_00 == -1)return;
	for (int i = 0; i <= pesi->dw_00; i++) {//ieax
		pesi->pb_08[i * 2 + 1] /= 2;
	}
	if (pesi->pb_08[pesi->dw_00 * 2 + 1]) return;
	if (pesi->ppt_04)return;
	do {
		if (pesi->dw_00 < 0) break;
		pesi->dw_00--;
	} while (!pesi->pb_08[pesi->dw_00 * 2 + 1]);
	if (pesi->dw_00 == -1)
	{
		DeallocateMemory(pesi->pb_08);
		pesi->pb_08 = 0;
		return;
	}
	size_t vlen = (pesi->dw_00 + 1) * 2;
	pesi->pb_08 = (unsigned char*)ReallocateMemoryAndCopyContents(pesi->pb_08, vlen, pesi->pb_08Len);
	pesi->pb_08Len = vlen;
}

//k
void sub_10032470(T_size_Above_C* arg_0, T_size_31C * arg_4) {
	T_size_Above_C *pebp = arg_0;
	T_size_31C* pebx = arg_4;
	do {
		int ieax = pebp->dw_00 + 2;
		pebx->aw_004[ieax] = 0;
		unsigned char barg_0 = 0;
		for (int i = ieax - 1; i > 0; i--) {//var_4
											//iecx = (i + 1) * 2; &pebx->aw_004[i + 1] esi - 2
			pebx->aw_004[i + 1] = pebx->aw_004[i + 2];
			if (pebp->pb_08[(i + 1) * 2 - 3])
			{
				if (pebx->dw_308 == -2 || pebx->ab_208[pebp->pb_08[(i + 1) * 2 - 4]] == 0) {
					pebx->aw_004[i + 1] += pebp->pb_08[(i + 1) * 2 - 3];
				}
			}
			if (pebp->pb_08[(i + 1) * 2 - 3] > barg_0) {
				barg_0 = pebp->pb_08[(i + 1) * 2 - 3];
			}
		}
		//loc_100324FE
		if (!barg_0) {
			pebx->aw_004[0] = 1;
		}
		else
		{
			pebx->aw_004[0] = 0x100 - pebp->dw_00;
			pebx->aw_004[0] = (pebp->dw_00 * pebx->aw_004[0]) / 256 / barg_0 + pebx->aw_004[1] + 1;
		}
		if (pebx->aw_004[0] < 0x3FFF)
			break;
		sub_100323F0(pebp);
	} while (1);
	//loc_10032560
	for (int i = 0; i < pebp->dw_00; i++) {//ecx
		if (pebp->pb_08[i * 2 + 1]) {
			pebx->ab_208[pebp->pb_08[i * 2]] = 1;
		}
	}
}
//k
void sub_100327B0(T_size_24 * arg_0, T_size_31C* arg_4) {
	T_size_31C * pesi = arg_4;
	pesi->w_310 = 0;
	for (int i = 0x10; i > 0; i--) {//edi
		pesi->w_310 *= 2;
		pesi->w_310 += sub_1000ABC0(arg_0);
	}
	pesi->w_312 = 0;
	pesi->w_314 = 0x0FFFF;
}
//hiunsigned char?
void sub_10032800(T_size_24 * arg_0, unsigned short * arg_4, T_size_31C * arg_8) {
	T_size_31C * pesi = arg_8;
	pesi->w_314 = arg_4[1] * (pesi->w_314 - pesi->w_312 + 1) / arg_4[2] + pesi->w_312 - 1;
	pesi->w_312 += arg_4[0] * (pesi->w_314 - pesi->w_312 + 1) / arg_4[2];
	int iedi = 0x4000;
	do {
		if (((pesi->w_312 ^ pesi->w_314) / 256) & 0x80) {//?
			if ((pesi->w_312 & iedi) == iedi && iedi & pesi->w_314) {
				pesi->w_310 ^= iedi;
				pesi->w_312 &= 0x3FFF;
				pesi->w_314 |= iedi;
			}
			else
				return;
		}
		pesi->w_312 *= 2;
		pesi->w_310 *= 2;
		pesi->w_314 = pesi->w_314 * 2 | 1;
		pesi->w_310 += sub_1000ABC0(arg_0);
	} while (1);
}
void sub_100328F0(int*** arg_0, unsigned char** arg_4, int arg_8, int arg_C) {
	int a = (arg_8 + 2) / 2;
	int c = (arg_8 - 1) & 1;
	int b = (arg_C + 2) / 2;//var_18
	int d = (arg_C - 1) & 1;//var_1C

	if (c) {
		int * peax = &arg_0[0][a - 1][b - 1];
		int * pedi = &arg_0[1][a - 1][b - 1];
		int * pebp = &arg_0[2][a - 1][b - 1];
		unsigned char * pesi = &arg_4[arg_8 - 1][arg_C * 3 - 1];
		if (d) {
			int var_38 = *peax + 0x20;
			int var_3C = *pedi;
			int var_20 = *pebp / 8 + *pebp / 2;
			int var_4 = var_20 * 2 - *pedi;
			int iecx = (*pebp / 2 + var_4 + var_38);
			if (iecx >= 0x4000)
				iecx = -1;
			else if (iecx < 0)
				iecx = 0;
			else
				iecx = iecx / 6;
			int var_40;
			if ((var_38 - var_3C / 4 - var_20) >= 0x4000)
				var_40 = -1;
			else if ((var_38 - var_3C / 4 - var_20) < 0)
				var_40 = 0;
			else
				var_40 = (var_38 - var_3C / 4 - var_20) / 6;
			int iedx;
			if ((var_20 + var_38 + var_3C) >= 0x4000)
				iedx = -1;
			else if ((var_20 + var_38 + var_3C) < 0)
				iedx = 0;
			else
				iedx = (var_20 + var_38 + var_3C) / 6;
			pedi--;
			pebp--;
			peax--;
			*pesi = (unsigned char)iecx;
			pesi--;
			*pesi = (unsigned char)var_40;
			pesi--;
			*pesi = (unsigned char)iedx;
			pesi--;
		}
		//loc_10032A3A
		int var_38 = *peax + 0x20;
		int var_3C = *pedi;
		int var_34 = *pebp / 8 + *pebp / 2;
		int var_4 = var_34 * 2 - *pedi;
		int var_30;
		if ((*pebp / 2 + var_4 + var_38) >= 0x4000)
			var_30 = -1;
		else if ((*pebp / 2 + var_4 + var_38) < 0)
			var_30 = 0;
		else
			var_30 = (*pebp / 2 + var_4 + var_38) / 6;
		int var_2C;
		if ((var_38 - var_3C / 4 - var_34) >= 0x4000)
			var_2C = -1;
		else if ((var_38 - var_3C / 4 - var_34) < 0)
			var_2C = 0;
		else
			var_2C = (var_38 - var_3C / 4 - var_34) / 6;
		int iedx;
		if ((var_34 + var_38 + var_3C) >= 0x4000)
			iedx = -1;
		else if ((var_34 + var_38 + var_3C) < 0)
			iedx = 0;
		else
			iedx = (var_34 + var_38 + var_3C) / 6;
		pedi--;
		pebp--;
		peax--;
		*pesi = (unsigned char)var_30;
		pesi--;
		*pesi = (unsigned char)var_2C;
		pesi--;
		*pesi = (unsigned char)iedx;
		pesi--;
		for (int var_10 = b - d - 1; var_10 > 0; var_10--) {
			int iedx = *peax + 0x20;
			int iebx = *pedi;
			int iecx1 = *pebp / 8 + *pebp / 2;
			int iedi = iecx1 * 2 - *pedi;//novalue
			int ieax;
			if ((*pebp / 2 + iedi + iedx) >= 0x4000)
				ieax = -1;
			else if ((*pebp / 2 + iedi + iedx) < 0)
				ieax = 0;
			else
				ieax = (*pebp / 2 + iedi + iedx) / 6;
			int var_40;
			if ((iedx - iebx / 4 - iecx1) >= 0x4000)
				var_40 = -1;
			else if ((iedx - iebx / 4 - iecx1) < 0)
				var_40 = 0;
			else
				var_40 = (iedx - iebx / 4 - iecx1) / 6;
			int iecx;
			if ((iebx + iedx + iecx1) >= 0x4000)
				iecx = -1;
			else if ((iebx + iedx + iecx1) < 0)
				iecx = 0;
			else
				iecx = (iebx + iedx + iecx1) / 6;
			pedi--;
			pebp--;
			peax--;
			*pesi = (unsigned char)(var_30 + ieax) / 2;
			pesi--;
			*pesi = (unsigned char)(var_2C + var_40) / 2;
			pesi--;
			*pesi = (unsigned char)(var_34 + iecx) / 2;
			pesi--;
			*pesi = (unsigned char)ieax;
			pesi--;
			*pesi = (unsigned char)var_40;
			pesi--;
			*pesi = (unsigned char)iecx;
			pesi--;
		}
	}
	//loc_10032C54
	int * pedi = &arg_0[0][a - c - 1][b - 1];
	int * pecx = &arg_0[1][a - c - 1][b - 1];
	int * pebx = &arg_0[2][a - c - 1][b - 1];
	unsigned char * pesi = &arg_4[arg_8 - c - 1][arg_C * 3 - 1];//var_C delete
	if (d) {
		int var_38 = *pedi + 0x20;
		int var_3C = *pecx;
		int var_20 = *pebx / 8 + *pebx / 2;
		int iebx = var_20 * 2 - var_3C;
		int iarg_8;
		if ((*pebx / 2 + iebx + var_38) >= 0x4000)
			iarg_8 = -1;
		else if ((*pebx / 2 + iebx + var_38) < 0)
			iarg_8 = 0;
		else
			iarg_8 = (*pebx / 2 + iebx + var_38) / 6;
		int var_40;
		if ((var_38 - var_3C / 4 - var_20) >= 0x4000)
			var_40 = -1;
		else if ((var_38 - var_3C / 4 - var_20) < 0)
			var_40 = 0;
		else
			var_40 = (var_38 - var_3C / 4 - var_20) / 6;
		int ieax;
		if ((var_20 + var_38 + var_3C) >= 0x4000)
			ieax = -1;
		else if ((var_20 + var_38 + var_3C) < 0)
			ieax = 0;
		else
			ieax = (var_20 + var_38 + var_3C) / 6;
		pedi--;
		pecx--;
		pebx--;
		*pesi = (unsigned char)iarg_8;
		pesi--;
		*pesi = (unsigned char)var_40;
		pesi--;
		*pesi = (unsigned char)ieax;
		pesi--;
	}
	//loc_10032D8D
	//int var_8 = var_C - var_14;
	int var_38 = *pedi + 0x20;
	int var_3C = *pecx;
	int var_34 = *pebx / 8 + *pebx / 2;
	int viebx = var_34 * 2 - var_3C;
	int var_30;
	if ((*pebx / 2 + viebx + var_38) >= 0x4000)
		var_30 = -1;
	else if ((*pebx / 2 + viebx + var_38) < 0)
		var_30 = 0;
	else
		var_30 = (*pebx / 2 + viebx + var_38) / 6;
	int var_2C;
	if ((var_38 - var_3C / 4 - var_34) >= 0x4000)
		var_2C = -1;
	else if ((var_38 - var_3C / 4 - var_34) < 0)
		var_2C = 0;
	else
		var_2C = (var_38 - var_3C / 4 - var_34) / 6;
	int ieax;
	if ((var_34 + var_38 + var_3C) >= 0x4000)
		ieax = -1;
	else if ((var_34 + var_38 + var_3C) < 0)
		ieax = 0;
	else
		ieax = (var_34 + var_38 + var_3C) / 6;
	pedi--;
	pecx--;
	pebx--;
	*pesi = (unsigned char)var_30;
	pesi--;
	*pesi = (unsigned char)var_2C;
	pesi--;
	*pesi = (unsigned char)ieax;
	pesi--;
	for (int iarg_8 = b - d - 1; iarg_8 > 0; iarg_8--) {
		int iedx = *pedi + 0x20;
		int iebx = *pecx;
		int iecx1 = *pebx / 8 + *pebx / 2;
		int iedi = iecx1 * 2 - iebx;//novalue
		int ieax;
		if ((*pebx / 2 + iedi + iedx) >= 0x4000)
			ieax = -1;
		else if ((*pebx / 2 + iedi + iedx) < 0)
			ieax = 0;
		else
			ieax = (*pebx / 2 + iedi + iedx) / 6;
		int var_40;
		if ((iedx - iebx / 4 - iecx1) >= 0x4000)
			var_40 = -1;
		else if ((iedx - iebx / 4 - iecx1) < 0)
			var_40 = 0;
		else
			var_40 = (iedx - iebx / 4 - iecx1) / 6;
		int iecx;
		if ((iebx + iedx + iecx1) >= 0x4000)
			iecx = -1;
		else if ((iebx + iedx + iecx1) < 0)
			iecx = 0;
		else
			iecx = (iebx + iedx + iecx1) / 6;
		pedi--;
		pecx--;
		pebx--;
		*pesi = (unsigned char)(var_30 + ieax) / 2;
		pesi--;
		*pesi = (unsigned char)(var_2C + var_40) / 2;
		pesi--;
		*pesi = (unsigned char)(var_34 + iecx) / 2;
		pesi--;
		*pesi = (unsigned char)ieax;
		pesi--;
		*pesi = (unsigned char)var_40;
		pesi--;
		*pesi = (unsigned char)iecx;
		pesi--;
	}
	//loc_10032F99
	for (int ivar_14 = a - c - 2; ivar_14 >= 0; ivar_14--) {
		//var_10 = ((unsigned char**)arg_4)[(ivar_14 + 1) * 2] - 8
		//int var_8 = var_18 - var_1C - 2;
		int * pecx = &arg_0[0][ivar_14][arg_C - 1];
		int * pedx = &arg_0[1][ivar_14][arg_C - 1];
		int * peax = &arg_0[2][ivar_14][arg_C - 1];
		unsigned char * pebp = &arg_4[(ivar_14 + 1) * 2][arg_C * 3 - 1];
		unsigned char * pedi = &arg_4[(ivar_14 + 1) * 2 - 1][arg_C * 3 - 1];
		unsigned char * pesi = &arg_4[(ivar_14 + 1) * 2 - 2][arg_C * 3 - 1];
		if (d) {
			int var_38 = *pecx + 0x20;
			int var_3C = *pedx;
			int iecx = *peax / 8 + *peax / 2;
			int iarg_8 = iecx * 2 - var_3C;
			int ieax;
			if ((*peax / 2 + iarg_8 + var_38) >= 0x4000)
				ieax = -1;
			else if ((*peax / 2 + iarg_8 + var_38) < 0)
				ieax = 0;
			else
				ieax = (*peax / 2 + iarg_8 + var_38) / 6;
			int var_40;
			if ((var_38 - var_3C / 4 - iecx) >= 0x4000)
				var_40 = -1;
			else if ((var_38 - var_3C / 4 - iecx) < 0)
				var_40 = 0;
			else
				var_40 = (var_38 - var_3C / 4 - iecx) / 6;
			int i;
			if ((iecx + var_38 + var_3C) >= 0x4000)
				i = -1;
			else if ((iecx + var_38 + var_3C) < 0)
				i = 0;
			else
				i = (iecx + var_38 + var_3C) / 6;
			pecx--;
			peax--;
			pedx--;
			*pedi = (unsigned char)(ieax + *pebp) / 2;
			pebp--;
			pedi--;
			*pedi = (unsigned char)(var_40 + *pebp) / 2;
			pebp--;
			pedi--;
			*pedi = (unsigned char)(i + *pebp) / 2;
			pebp--;
			pedi--;
			*pesi = (unsigned char)ieax;
			pesi--;
			*pesi = (unsigned char)var_40;
			pesi--;
			*pesi = (unsigned char)i;
			pesi--;
		}
		int var_38 = *pecx + 0x20;
		int var_3C = *pedx;
		int iecx_1 = *peax / 8 + *peax / 2;
		int iarg_8 = iecx_1 * 2 - var_3C;
		int var_30;
		if ((*peax / 2 + iarg_8 + var_38) >= 0x4000)
			var_30 = -1;
		else if ((*peax / 2 + iarg_8 + var_38) < 0)
			var_30 = 0;
		else
			var_30 = (*peax / 2 + iarg_8 + var_38) / 6;
		int var_2C;
		if ((var_38 - var_3C / 4 - iecx_1) >= 0x4000)
			var_2C = -1;
		else if ((var_38 - var_3C / 4 - iecx_1) < 0)
			var_2C = 0;
		else
			var_2C = (var_38 - var_3C / 4 - iecx_1) / 6;
		int ivar_34;
		if ((iecx_1 + var_38 + var_3C) >= 0x4000)
			ivar_34 = -1;
		else if ((iecx_1 + var_38 + var_3C) < 0)
			ivar_34 = 0;
		else
			ivar_34 = (iecx_1 + var_38 + var_3C) / 6;
		*pesi = (unsigned char)var_30;
		pesi--;
		*pesi = (unsigned char)var_2C;
		pesi--;
		*pesi = (unsigned char)ivar_34;
		pesi--;
		*pedi = (var_30 + *pebp) / 2;
		iarg_8 = var_30 + *pebp;
		pedi--;
		pebp--;
		*pedi = (var_2C + *pebp) / 2;
		int iarg_4 = var_2C + *pebp;
		pedi--;
		pebp--;
		*pedi = (ivar_34 + *pebp) / 2;
		int ivar_18 = ivar_34 + *pebp;
		pedi--;
		pebp--;
		pebp -= 3;
		pecx--;
		pedx--;
		peax--;
		for (int var_4 = b - d - 2; var_4 > 0; var_4--) {
			int var_38 = *pecx + 0x20;
			int var_3C = *pedx;
			int iecx = *peax / 8 + *peax / 2;
			int viebx = iecx * 2 - var_3C;
			int ieax;
			if ((*peax / 2 + viebx + var_38) >= 0x4000)
				ieax = -1;
			else if ((*peax / 2 + viebx + var_38) < 0)
				ieax = 0;
			else
				ieax = (*peax / 2 + viebx + var_38) / 6;
			int var_40;
			if ((var_38 - var_3C / 4 - iecx) >= 0x4000)
				var_40 = -1;
			else if ((var_38 - var_3C / 4 - iecx) < 0)
				var_40 = 0;
			else
				var_40 = (var_38 - var_3C / 4 - iecx) / 6;
			int i;
			if ((iecx + var_38 + var_3C) >= 0x4000)
				i = -1;
			else if ((iecx + var_38 + var_3C) < 0)
				i = 0;
			else
				i = (iecx + var_38 + var_3C) / 6;
			*pesi = (unsigned char)(var_30 + ieax + *pebp) / 2;
			iarg_8 = ieax + *pebp;
			pebp--;
			pesi--;
			*pesi = (unsigned char)(var_2C + var_40 + *pebp) / 2;
			iarg_4 = var_40 + *pebp;
			pebp--;
			pesi--;
			*pesi = (unsigned char)(ivar_34 + i + *pebp) / 2;
			ivar_18 = i + *pebp;
			pebp--;
			pesi--;
			*pesi = (unsigned char)ieax;
			var_30 = ieax;
			pesi--;
			*pesi = (unsigned char)var_40;
			var_2C = var_40;
			pesi--;
			*pesi = (unsigned char)i;
			ivar_34 = i;
			pesi--;
			*pedi = (iarg_8 + var_30 + *pebp) / 4;
			int t1 = var_30 + *pebp;
			pedi--;
			pebp--;
			*pedi = (iarg_4 + var_2C + *pebp) / 4;
			int t2 = var_2C + *pebp;
			pedi--;
			pebp--;
			*pedi = (ivar_18 + ivar_34 + *pebp) / 4;
			int t3 = ivar_34 + *pebp;
			pedi--;
			pebp--;
			*pedi = t1 / 2;
			pedi--;
			pebp--;
			*pedi = t2 / 2;
			pedi--;
			pebp--;
			*pedi = t3 / 2;
			pedi--;
			pebp--;
			pedx--;
			peax--;
			pecx--;
		}
		var_38 = *pecx + 0x20;
		var_3C = *pedx;
		iecx_1 = *peax / 8 + *peax / 2;
		int var = iecx_1 * 2 - var_3C;
		int ieax;
		if ((*peax / 2 + var + var_38) >= 0x4000)
			ieax = -1;
		else if ((*peax / 2 + var + var_38) < 0)
			ieax = 0;
		else
			ieax = (*peax / 2 + var + var_38) / 6;
		int var_40;
		if ((var_38 - var_3C / 4 - iecx_1) >= 0x4000)
			var_40 = -1;
		else if ((var_38 - var_3C / 4 - iecx_1) < 0)
			var_40 = 0;
		else
			var_40 = (var_38 - var_3C / 4 - iecx_1) / 6;
		int iecx;
		if ((iecx_1 + var_38 + var_3C) >= 0x4000)
			iecx = -1;
		else if ((iecx_1 + var_38 + var_3C) < 0)
			iecx = 0;
		else
			iecx = (iecx_1 + var_38 + var_3C) / 6;
		*pesi = (var_30 + ieax) / 2;
		pesi--;
		*pesi = (var_2C + var_40) / 2;
		pesi--;
		*pesi = (var_34 + iecx) / 2;
		pesi--;
		*pesi = (unsigned char)ieax;
		pesi--;
		*pesi = (unsigned char)var_40;
		pesi--;
		*pesi = (unsigned char)iecx_1;
		pesi--;
		*pedi = (iarg_8 + ieax + *pebp) / 4;
		int t1 = ieax + *pebp;
		pedi--;
		pebp--;
		*pedi = (iarg_4 + var_40 + *pebp) / 4;
		int t2 = var_40 + *pebp;
		pedi--;
		pebp--;
		*pedi = (ivar_18 + iecx_1 + *pebp) / 4;
		int t3 = iecx_1 + *pebp;
		pedi--;
		pebp--;
		*pedi = t1 / 2;
		pedi--;
		pebp--;
		*pedi = t2 / 2;
		pedi--;
		pebp--;
		*pedi = t3 / 2;
		pedi--;
		pebp--;
	}

}

WiDecmpOptions* WiCreateDecmpOptions(void) {
	WiDecmpOptions* result = (WiDecmpOptions*)AllocateMemorySize(80 * factor64); // 0x10009d72
	if (result == 0) {
		return 0;
	}
	result->Smoothing = 0;
	result->Fast = 1;
	result->ReadNextByte = 0;
	result->ReadParam = 0;
	result->Sharpening = 0;
	result->WriteScanline = 0;
	result->WriteScanlineParam = 0;
	result->SubImage.Page = 1;
	result->SubImage.Block = -2;
	result->Magnification = 0;
	result->WriteAppExtension = 0;
	result->WriteAppExtensionParam = 0;

	return result;
}

WiCmpImage* WiCreateCmpImage()
{
	WiCmpImage* result = (WiCmpImage*)AllocateMemorySize(8 * factor64); // 0x1000a5e2
	if (result == 0)
	{
		return 0;
	}
	result->CmpData = 0;
	result->Size = 0;
	return result;
}

// Address range: 0x1000a600 - 0x1000a61f
int WiDestroyCmpImage(WiCmpImage* a1)
{
	int result;
	if (a1 != 0) {
		DeallocateMemory(a1);
		a1->CmpData = 0;
		a1->Size = 0;
		result = 1;
	}
	else {
		result = 0;
	}
	return result;
}

int WiDestroyDecmpOptions(WiDecmpOptions* a1)
{
	int result; // 0x1000a609
	if (a1 != 0) {
		DeallocateMemory(a1);
		result = 1;
	}
	else {
		result = 0;
	}
	return result;
}

WiRawImage* WiCreateRawImage()
{
	WiRawImage* result = (WiRawImage*)AllocateMemorySize(48 * factor64); // 0x1000a5b2
																		 //g2 = result;
																		 //g5 = 0;
																		 //g1 = false;
																		 //g206 = result < 0;
	if (result == 0) {
		// 0x1000a5c0
		//g1 = false;
		//g206 = false;
		//g2 = 0;
		return 0;
	}
	// 0x1000a5c3
	result->Raw = 0;
	result->AppData = 0;
	result->AppExtension = 0;
	result->AppExtensionLength = 0;
	result->Comment = 0;
	result->CommentLength = 0;
	return result;
}

void WiFreeRawImageData(WiRawImage* a1) {
	if (a1 == NULL) {
		return;
	}
	if (a1->AppData != NULL || a1->AppData != 0)
	{
		DeallocateMemory(a1->AppData);
		a1->AppData = 0;
	}
	if (a1->CommentLength != 0)
		DeallocateMemory(a1->Comment);
	if (a1->AppExtensionLength != 0)
		DeallocateMemory(a1->AppExtension);
	DeallocateMemory(a1->Raw);
	a1->Raw = 0;
	DeallocateMemory(a1);
}

//Found
int WiDecompress(WiDecmpOptions * arg_0, WiRawImage * arg_4, WiCmpImage* arg_8) {
	T_size_178 *pesi = (T_size_178 *)AllocateMemorySize(sizeof(T_size_178)); // 0x178);;

	WiRawImage *ebx = arg_4;
	ebx->Raw = 0;
	pesi->hg_B4 = 0;
	T_size_24 * pebp = sub_1000A730(0);
	WiDecmpOptions *pedi = arg_0;
	if (pedi->ReadNextByte)
	{
		pebp->pb_00 = 0;
	}
	else
	{
		pebp->pb_00 = arg_8->CmpData;
	}

	sub_1000A900(pebp);
	pebp->dw0x10 = arg_8->Size;
	sub_1000A8B0(pebp, pedi->ReadNextByte, pedi->ReadParam);
	pesi->pt0xA8 = pebp;
	pesi->dw0x120 = pedi->Sharpening;
	if (pedi->Sharpening != 1)
	{
		pesi->dw0x120 = 0;
	}
	pesi->dw_130 = pedi->Magnification;
	pesi->dw0x124 = pedi->Smoothing;
	pesi->dw0x13C = pedi->Fast;
	pesi->pf0x98 = pedi->WriteScanline;
	pesi->pv_9C = pedi->WriteScanlineParam;
	pesi->dw0x158 = pedi->SubImage.Page;
	pesi->dw0x15C = pedi->SubImage.Block;
	pesi->pf_A0 = pedi->WriteAppExtension;
	pesi->pv_A4 = pedi->WriteAppExtensionParam;
	pesi->hg_B8 = 0;
	pesi->AE_170 = 0;
	pesi->AELen_174 = 0;
	ebx->AppData = pesi;
	sub_1000E690(pesi);
	if (pesi->hg_B8)
	{
		ebx->Raw = (unsigned char*)sub_1000E540(pesi->hg_B8, pesi->dw0xCC, pesi->dw0xD0, pesi->dw0xF4);
	}
	ebx->Width = pesi->dw0x12C;
	ebx->Height = pesi->dw0x128;
	ebx->LevelWidth = pesi->dw0xD0;
	ebx->LevelHeight = pesi->dw0xCC;
	int ieax = 0x18;
	if (pesi->dw0xF4 != 3)
	{
		ieax = pesi->dw0xF0;
	}
	ebx->BitsPerPixel = ieax;
	//edx = 0;
	//dl = esi->dw0xF4 == 3;
	ebx->Color = pesi->dw0xF4 == 3/*dl*/;
	ebx->AppExtension = pesi->AE_170;
	ebx->AppExtensionLength = pesi->AELen_174;
	DeallocateMemory(pebp);
	//printf("Alocated : %i, Deallocated %i, Diff %i\n", memAllocateCnt, memDeallocateCnt, memAllocateCnt - memDeallocateCnt);
	return pesi->dw0x154;
}


#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "SWIDecoder", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "SWIDecoder", __VA_ARGS__))

extern "C" {

	WiRawImage* CreateRawImage() {
		return WiCreateRawImage();
	}

    WiDecmpOptions* CreateDecmpOptions(){
		return WiCreateDecmpOptions();
	}

	WiCmpImage* CreateCmpImage() {
		return WiCreateCmpImage();
	}

	int Decompress(WiDecmpOptions decmpOpts, WiRawImage image, WiCmpImage cmpImage)
	{
		decmpOpts.Smoothing = 0;       /* do not use wavelet smoothing          */
		decmpOpts.Fast = 0;            /* do not use fast decompression method  */
		decmpOpts.ReadNextByte = NULL; /* do not use user defined bitstream I/0 */
		decmpOpts.ReadParam = NULL;    /* do not use free variable              */
		decmpOpts.Sharpening = 1;      /* wavelet sharpen image                 */
	   return WiDecompress(&decmpOpts, &image, &cmpImage);
	}

	int DestroyDecmpOptions(WiDecmpOptions * decmpOpts)
	{
		return WiDestroyDecmpOptions(decmpOpts);
	}

	int FreeRawImageData(WiRawImage * image)
	{
		WiFreeRawImageData(image);
		return 1;
	}

	int DestroyCmpImage(WiCmpImage* cmpImage)
	{
		return WiDestroyCmpImage(cmpImage);
	}


	WiResultImage DecodeImage(unsigned char * imageData, int size)
	{
		WiResultImage result;
		WiRawImage *image;
		WiCmpImage *cmpImage;
		WiDecmpOptions *decmpOpts;
		int errorCode;


		image = WiCreateRawImage();
		cmpImage = WiCreateCmpImage();
		decmpOpts = WiCreateDecmpOptions();

		cmpImage->CmpData = imageData;
		cmpImage->Size = size;

		decmpOpts->Smoothing = 0;       /* do not use wavelet smoothing          */
		decmpOpts->Fast = 0;            /* do not use fast decompression method  */
		decmpOpts->ReadNextByte = NULL; /* do not use user defined bitstream I/0 */
		decmpOpts->ReadParam = NULL;    /* do not use free variable              */
		decmpOpts->Sharpening = 1;      /* wavelet sharpen image                 */

		errorCode = WiDecompress(decmpOpts, image, cmpImage);


		result.size = image->Width * image->Height;
		result.raw = image->Raw;

		int data_size = image->Width * image->Height;
		for (int i = 0; i < data_size; i++)
		{
			result.raw[i] = image->Raw[i];
		}

		WiDestroyCmpImage(cmpImage);
		WiDestroyDecmpOptions(decmpOpts);
		WiFreeRawImageData(image);

		return result;
	}
}


extern "C" JNIEXPORT jbyteArray JNICALL
Java_com_peachss_sadldecoder_Decoder_getDecodedPhoto(JNIEnv *env, jclass clazz,
                                                     jbyteArray photo_data, jint size) {

    // Check if photo_data is valid
    if (photo_data == nullptr) {
        return nullptr; // or handle the error appropriately
    }

    // Step 1: Get the length of the byte array
    jsize length = env->GetArrayLength(photo_data);
    if (size > length) {
        return nullptr; // or handle the error appropriately
    }

    // Step 2: Get a pointer to the byte array
    jbyte *input = env->GetByteArrayElements(photo_data, nullptr);
    if (input == nullptr) {
        return nullptr; // Handle memory allocation failure
    }

    // Step 3: Decode the image
    WiResultImage decode = DecodeImage((unsigned char *) input, size + 1);

    // Step 4: Release the byte array elements
    env->ReleaseByteArrayElements(photo_data, input, JNI_COMMIT);

    // Check if the decoding was successful
    if (decode.size <= 0 || decode.raw == nullptr) {
        return nullptr; // or handle the error appropriately
    }

    // Step 5: Create a new byte array for the decoded data
    jbyteArray output = env->NewByteArray(decode.size);
    if (output == nullptr) {
         return nullptr; // Handle memory allocation failure
    }

    // Step 6: Set the byte array region with the decoded data
    env->SetByteArrayRegion(output, 0, decode.size, (jbyte *) decode.raw);

    // Step 7: Return the output byte array
    return output;
}

