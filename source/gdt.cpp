
#include "gdt.h"

GlobalDescriptorTable::GlobalDescriptorTable()
: nullSegmentSelector(0,0,0),
unusedSegmentSelector(0,0,0),
codeSegmentSelector(0,64*1024*1024, 0x9A),
dataSegmentSelector(0,64*1024*1024, 0x92)
{
	uint32_t i[2];
	i[1] = (uint32_t)this; //base
	i[0] = sizeof(GlobalDescriptorTable) << 16;// limit

	//load the base address and limit of gdt into gdtr(register)
	//the input of lgdt should be 6 bytes, lower 2 bytes for limits and higher 4 bytes for base. Since data is stored in Computer from low bits to high bits, (char* i + 2) = (high 2 bytes of i[0] and i[1]);
	asm volatile("lgdt (%0)": :"p" (((uint8_t *) i)+2));


}


GlobalDescriptorTable::~GlobalDescriptorTable()
{
}

uint16_t GlobalDescriptorTable::DataSegmentSelector()//offset of data segment
{
	return (uint8_t*)&dataSegmentSelector - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::CodeSegmentSelector()
{
	return (uint8_t*)&codeSegmentSelector - (uint8_t*)this;
}


GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags)
{
	uint8_t* target = (uint8_t*)this;
	if(limit <= 65536) //16 bit
	{
		target[6] = 0x40;// 4 is the flag
	}
	else
	{
		if((limit & 0xFFF) != 0xFFF) //whether the last 12 bit of limit is all 1.
		{
			limit = (limit >>12)-1;
		}
		else
		{
			limit = limit >>12;
		}

		target[6] = 0xC0;// C is a flag;

	}


	target[0] = limit & 0xFF;
	target[1] = (limit >> 8) & 0xFF;
	target[6] |= (limit >> 16) & 0xF;


	target[2] = (base) &0xFF;	
	target[3] = (base >> 8) &0xFF;	
	target[4] = (base >> 16) &0xFF;	
	target[7] = (base >> 24) &0xFF;	

	target[5] = flags;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base()
{
	uint8_t* target = (uint8_t*)this;
	uint32_t result = target[7];
	result = (result << 8) + target[4];
	result = (result << 8) + target[3];
	result = (result << 8) + target[2];
	return result;
}
uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit()
{
	uint8_t* target = (uint8_t*)this;
	uint32_t result = target[6] & 0xF;
	result = (result << 8) + target[1];
	result = (result << 8) + target[0];

	if((target[6] & 0xC0) == 0xC0)
            result = (result << 12) | 0xFFF;
	return result;
}

