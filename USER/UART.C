#define "UART.H"

void talk() interrupt 4
{
	if(TI)
		TI=0;
	if(RI)
	{
		RI=0;
		rx_data=SBUF;
		flag=1;
	}
}