void EncodeIDTEntry(int, struct IDTEncode*);
void setupIDT();
extern void loadIDT();
extern void ISRReserved(); /* Reserved IRQ */

struct IDTEntry IDT[256];
struct IDTLoc IDTPtr;

void EncodeIDTEntry(int num, struct IDTEncode *entry)
{

	IDT[num].LowBase = (entry->base & 0xFFFF);
	IDT[num].segment = 0x08;
	IDT[num].zero = 0x00;
	IDT[num].flags = entry->flags;
	IDT[num].HighBase = (entry->base >> 16) & 0xFFFF;

}