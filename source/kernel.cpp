
#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/driver.h>
#include <drivers/vga.h>
#include <multitasking.h>
#include <memorymanagement.h>
#include <drivers/ata.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000; // ADRESS OF graphics ram

    static uint8_t x = 0, y = 0; //screen is 25*80

    for(int i=0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                y++;
                x = 0;
                break;

            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i]; 
                x++;
                break;
        }

        if(x >= 80)
        {
            y++;
            x = 0;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }

    }
}

void printfHex(uint8_t c)
{
        char* foo = "0x00";
        char* hex = "0123456789ABCDEF";
        foo[2] = hex[(c >> 4) & 0x0F];
        foo[3] = hex[c & 0x0F];
        printf(foo);

}


class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
    public:
        void OnKeyDown(char c)
        {
            char* foo = " ";
            foo[0] = c;
            printf(foo);
        }
};



class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
    public:
    MouseToConsole()
    {
        x = 40;
        y = 12;
        uint16_t* VideoMemory = (uint16_t*)0xb8000; 

        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
            | ((VideoMemory[80*y+x] & 0x0F00) << 4)
            | (VideoMemory[80*y+x] & 0x00FF);

    }
    void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000; 

        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
            | ((VideoMemory[80*y+x] & 0x0F00) << 4)
            | (VideoMemory[80*y+x] & 0x00FF);


        x += xoffset;
        if(x < 0) x = 0;
        if(x >= 80) x =79;

        y += yoffset;
        if(y <0) y = 0;
        if(y >= 25) y =24;

        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
            | ((VideoMemory[80*y+x] & 0x0F00) << 4)
            | (VideoMemory[80*y+x] & 0x00FF);


    }   
};

void taskA()
{
    while(true)
        printf("A");
}

void taskB()
{
    while(true)
        printf("B");
}


extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber)
{
    printf("Hello World!\n");


    GlobalDescriptorTable gdt;
    
    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
    size_t heap = 10*1024*1024;
    MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);

    printf("heap: 0x");
    printfHex((heap >> 24) & 0xFF);
    printfHex((heap >> 16) & 0xFF);
    printfHex((heap >> 8 ) & 0xFF);
    printfHex((heap      ) & 0xFF);

    /*
    void* allocated = memoryManager.malloc(1024);
    printf("\nallocated: 0x");
    printfHex(((size_t)allocated >> 24) & 0xFF);
    printfHex(((size_t)allocated >> 16) & 0xFF);
    printfHex(((size_t)allocated >> 8 ) & 0xFF);
    printfHex(((size_t)allocated      ) & 0xFF);
    printf("\n");
    */


    TaskManager taskManager;
    /*
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    */
    InterruptManager interrupts(0x20, &gdt, &taskManager);

    printf("Initializing Hardware, Stage 1\n");
    DriverManager drvManager;

    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);

    MouseToConsole mshandler;
    MouseDriver mouse(&interrupts, &mshandler);
    drvManager.AddDriver(&mouse);

    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);



    printf("Initializing Hardware, Stage 2\n");
    drvManager.ActivateAll();

    printf("Initializing Hardware, Stage 3\n");

    //interrupt 14
    AdvancedTechnologyAttachment ata0m(0x1F0, true);
    printf("ATA Primary Master: ");
    ata0m.Identify();

    AdvancedTechnologyAttachment ata0s(0x1F0, false);
    printf("ATA Primary Slave: ");
    ata0s.Identify();

    char* atabuffer = "Hello Hard drive";
    ata0s.Write28(0,(uint8_t*)atabuffer,16);
    ata0s.Flush();

    char* readbuffer = " ";
    ata0s.Read28(0,16);


    interrupts.Activate();



    while(1);
}
