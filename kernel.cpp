

void printf(char* str)
{
	unsigned short* VideoMemory = (unsigned short*)0xb8000; // ADRESS OF graphics ram

	for(int i=0; str[i] != '\0'; ++i)
	{
		VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];  // High byte for color, low byte for character
	}
}

extern "C" void kernelMain(void* multiboot_structure, unsigned int magicnumber)
{
	printf("Hello World!");
	
	while(1);

}
