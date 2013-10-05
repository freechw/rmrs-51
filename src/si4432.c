#include "si4432.h"
#include "si4432spiport.h"
#include "si4432spi.h"
#include "interseral.h"
#include "stdlib.h"
#include <intrins.h>
#include <STC/STC12C5A60S2.H>

#define NOP; _nop_();



//This value should be set according to the experiment value, different batches has different value.
#define CRYSTAL_CAPACITANCE     0xB4

#define TxGPIOSetting(); SPI_Write_Reg(0x0E, 0x02);
#define RxGPIOSetting(); SPI_Write_Reg(0x0E, 0x01);
#define IdleGPIOSetting(); SPI_Write_Reg(0x0E, 0x01);

#define SI4432_SEND_TIMEOUT 4
#define SI4432_RESET_TIMEOUT 4


data unsigned char ItStatus1,ItStatus2;


void RF_init(unsigned char syncwordf, unsigned char syncwords)
{
    //Set the physical parameters
    //Frequency:       433MHz
    //Modulation type: GFSK
    //CrytalTolerance: Rx,10ppm; Tx, 10ppm
    //Data rate:       9.6Kbps/1.2Kbps
    //AFC:             Enable
    //Frequency deviation: 20KHz

    //set the center frequency to 433 MHz
    SPI_Write_Reg(0x75, 0x53);                                                           //write data to the Frequency Band Select register             
    SPI_Write_Reg(0x76, 0x4B);                                                           //write data to the Nominal Carrier Frequency1 register
    SPI_Write_Reg(0x77, 0x00);                                                           //write data to the Nominal Carrier Frequency0 register

    //set the desired TX data rate (1.2kbps)
    SPI_Write_Reg(0x6E, 0x09);                                                           //write data to the TXDataRate 1 register
    SPI_Write_Reg(0x6F, 0xD5);                                                           //write data to the TXDataRate 0 register
    SPI_Write_Reg(0x70, 0x2C);                                                           //write data to the Modulation Mode Control 1 register
    SPI_Write_Reg(0x58, 0x80);

    //set the TX power to MAX. dBm
    SPI_Write_Reg(0x6D, 0x1F);                                                           //write data to the TX Power register 

    //set the Tx deviation register (+-20kHz)
    SPI_Write_Reg(0x72, 0x20);                                                           //write data to the Frequency Deviation register 

                                /*set the modem parameters according to the exel calculator(parameters: 1.2 kbps, deviation: 20 kHz*/
    SPI_Write_Reg(0x1C, 0x2C);                                                           //write data to the IF Filter Bandwidth register        
    SPI_Write_Reg(0x20, 0x41);                                                           //write data to the Clock Recovery Oversampling Ratio register      
    SPI_Write_Reg(0x21, 0x60);                                                           //write data to the Clock Recovery Offset 2 register        
    SPI_Write_Reg(0x22, 0x27);                                                           //write data to the Clock Recovery Offset 1 register        
    SPI_Write_Reg(0x23, 0x52);                                                           //write data to the Clock Recovery Offset 0 register        
    SPI_Write_Reg(0x24, 0x00);                                                           //write data to the Clock Recovery Timing Loop Gain 1 register      
    SPI_Write_Reg(0x25, 0x04);                                                           //write data to the Clock Recovery Timing Loop Gain 0 register      
    SPI_Write_Reg(0x1D, 0x40);                                                           //write data to the AFC Loop Gearshift Override register        
    SPI_Write_Reg(0x1E, 0x0A);
    SPI_Write_Reg(0x2A, 0x0F);                                                           //write data to the AFC Limiter register        
    SPI_Write_Reg(0x1F, 0x03);
    SPI_Write_Reg(0x69, 0x60);
    
                            /*set the packet structure and the modulation type*/
    //set the preamble length to 10bytes if the antenna diversity is used and set to 5bytes if not 
    SPI_Write_Reg(0x34, 0x0C);                                                           //write data to the Preamble Length register
    //set preamble detection threshold to 20bits
    SPI_Write_Reg(0x35, 0x2A);                                                           //write data to the Preamble Detection Control  register

    //Disable header bytes; set variable packet length (the length of the payload is defined by the
    //received packet length field of the packet); set the synch word to two bytes long
    SPI_Write_Reg(0x33, 0x02);                                                           //write data to the Header Control2 register    
    
    //Set the sync word pattern to 0x2DD4
    SPI_Write_Reg(0x36, syncwordf);                                                           //write data to the Sync Word 3 register
    SPI_Write_Reg(0x37, syncwords);                                                           //write data to the Sync Word 2 register

    //enable the TX & RX packet handler and CRC-16 (IBM) check
    SPI_Write_Reg(0x30, 0x8D);                                                           //write data to the Data Access Control register
    //Disable the receive header filters
    SPI_Write_Reg(0x32, 0x00 );                                                          //write data to the Header Control1 register            

    //enable FIFO mode and GFSK modulation
    SPI_Write_Reg(0x71, 0x63);                                                           //write data to the Modulation Mode Control 2 register

                                            /*set the GPIO's according the testcard type*/
    SPI_Write_Reg(0x0B, 0xCA);                                                           //Set GPIO0 output
    SPI_Write_Reg(0x0C, 0xCA);                                                           //Set GPIO1 output
    SPI_Write_Reg(0x0D, 0xCA);                                                       //Set GPIO2 output Rx Data

    //set  Crystal Oscillator Load Capacitance register
    SPI_Write_Reg(0x09, CRYSTAL_CAPACITANCE);                                            //write data to the Crystal Oscillator Load Capacitance register
}

void RF_Set_RXMode(void)
{
    ItStatus1 = SPI_Read_Reg(0x03);
    ItStatus2 = SPI_Read_Reg(0x04);

    RxGPIOSetting();

    SPI_Write_Reg(0x07, 0x05);
}

void RF_Set_TXMode(void)
{
    ItStatus1 = SPI_Read_Reg(0x03);
    ItStatus2 = SPI_Read_Reg(0x04);

    TxGPIOSetting();

    SPI_Write_Reg(0x07, 0x09);
}

void RF_Set_IdleMode(void)
{
    SPI_Write_Reg(0x07, 0x01);

    IdleGPIOSetting();

    ItStatus1 = SPI_Read_Reg(0x03);
    ItStatus2 = SPI_Read_Reg(0x04);
}

void RF_FIFO_Send(unsigned char buf[], unsigned char length)
{
    //unsigned char i;

    RF_Set_IdleMode();

    InterSendString("Radio: Prepare to send!\r\n");

    /*********DEBUG!!!**************/
    InterSendString("Si4432: Send data is: ");
    InterHexString(buf, length);
    InterSendString("\r\n");
    /*******************************/

    SPI_Write_Reg(0x3E, length);
//    for(i = 0; i < length; i++)
//    {
//        SPI_Write_Reg(0x7F, buf[i]);
//    }
    SPI_Burst_Write(0x7F, buf, length);

    SPI_Write_Reg(0x05, 0x04);
    SPI_Write_Reg(0x06, 0x00);

    RF_Set_TXMode();

    InterSendString("Si4432: Sending......\r\n");

    TimerUpper(SI4432_SEND_TIMEOUT);
    while((NIRQ == 1) && (TimerUpper_Flag == 0));
    TimerUpperOff();
    if(1 == TimerUpper_Flag)
    {
        TimerUpper_Flag = 0;
        InterSendString("Si4432: Send Fail!\r\n");
    }

    SPI_Write_Reg(0x05, 0x03);
    SPI_Write_Reg(0x06, 0x00);
    RF_Set_RXMode();

    ItStatus1 = SPI_Read_Reg(0x03);
    ItStatus2 = SPI_Read_Reg(0x04);

    InterSendString("Si4432: Send End!\r\n");

}

void RF_Reset()
{
    ItStatus1 = SPI_Read_Reg(0x03);
    ItStatus2 = SPI_Read_Reg(0x04);
    //Reset the Radio Chip
    NIRQ = 1;
    NOP;
    SPI_Write_Reg(0x07, 0x80);

    InterSendString("Si4432: Reset Radio!\r\n");

    //wait for chip reset
    TimerUpper(SI4432_RESET_TIMEOUT);
    while((1 == NIRQ) && (TimerUpper_Flag == 0));
    TimerUpperOff();
    if (1 == TimerUpper_Flag)
    {
        InterSendString("Si4432: Reset Radio Fail!\r\n");
    }
    else
    {
        InterSendString("Si4432: Reset OK!\r\n");
    }
    ItStatus1 = SPI_Read_Reg(0x03);
    ItStatus2 = SPI_Read_Reg(0x04);
}

