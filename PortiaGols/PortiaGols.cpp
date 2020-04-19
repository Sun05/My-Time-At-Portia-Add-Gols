
#include "stdafx.h"
#include "proc.h"
#include "PortiaGols.h"
#include <iostream>
#include <windows.h>
#include <vector>

int main()
{
	DWORD procId = GetProcId(L"Portia.exe");

	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"mono.dll");

	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
	if (hProcess == NULL)
	{
		std::cout << "Hanndle Returned NULL: " << GetLastError() << std::endl;
		return -1;
	}
	else std::cout << "Process handle opened." << std::endl; 

	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x267B58;

	std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;


	std::vector<unsigned int> golsOffset = { 0x20, 0xA8, 0x170, 0x98, 0x58, 0x100, 0x58 };
	uintptr_t golsAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, golsOffset);

	std::cout << "golsAddr = " << "0x" << std::hex << golsAddr << std::endl;

	std::cout << "1. Your father sends you money. (999)\n";
	std::cout << "2. You hack into the Portia Bank System (99999)\n>>";
	int selectioni;
	std::cin >> selectioni;
	int newGols{};
	if (selectioni == 1)
	{
		newGols = 999;
	}
	else
	{
		newGols = 99999;
	}
	int golsValue = 0;

	ReadProcessMemory(hProcess, (BYTE*)golsAddr, &golsValue, sizeof(golsValue), nullptr);
	std::cout << "Current gols = " << std::dec << golsValue << std::endl;

	newGols = newGols + golsValue ;
	WriteProcessMemory(hProcess, (BYTE*)golsAddr, &newGols, sizeof(newGols), nullptr);
	std::cout << "Gols updated" << std::endl;
	
	ReadProcessMemory(hProcess, (BYTE*)golsAddr, &golsValue, sizeof(golsValue), nullptr);
	std::cout << "Current gols = " << std::dec << golsValue << std::endl;

	getchar();
	return 0;
}

