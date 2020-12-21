
#include "types.h"
#include "gdt.h"

void printf(char* str)
{
	static uint16_t* VideoMemory = (uint16_t*)0xb8000; // ADRESS OF graphics ram

	for(int i=0; str[i] != '\0'; ++i)
	{
		VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];  // High byte for color, low byte for character
	}
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber)
{
	printf("Hello Zirong Cai!");
	
	GlobalDescriptorTable gdt;
	while(1);

}
