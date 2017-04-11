/*
    sim_mega324.c

    Copyright 2008, 2009 Michel Pollet <buserror@gmail.com>

     This file is part of simavr.

    simavr is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simavr is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simavr.  If not, see <http://www.gnu.org/licenses/>.
 */

#define SIM_VECTOR_SIZE    4
#define SIM_MMCU        "atmega32u4"
#define SIM_CORENAME    mcu_mega32u4
#define USBRF 5 // missing in avr/iom32u4.h

#define PD7     7
#define PD6     6
#define PD5     5
#define PD4     4
#define PD3     3
#define PD2     2
#define PD1     1
#define PD0     0

#define PE6     6

#include "sim_avr.h"
#include "sim_core_declare.h"
#include "avr_eeprom.h"
#include "avr_flash.h"
#include "avr_watchdog.h"
#include "avr_extint.h"
#include "avr_ioport.h"
#include "avr_uart.h"
#include "avr_adc.h"
#include "avr_timer.h"
#include "avr_spi.h"
#include "avr_twi.h"
#include "avr_usb.h"

void m32u4_init(struct avr_t * avr);
void m32u4_reset(struct avr_t * avr);

#define _AVR_IO_H_
#define __ASSEMBLER__
#ifndef __AVR_ATmega32u4__
#define __AVR_ATmega32u4__
#endif
#include "avr/iom32u4.h"

/*
 * ATmega32u4 definitions
 */
const struct mcu_t {
  avr_t            core;
  avr_eeprom_t     eeprom;
  avr_flash_t      selfprog;
  avr_watchdog_t   watchdog;
  avr_extint_t     extint;
  avr_ioport_t     portb, portc, portd, porte, portf;
  avr_uart_t       uart1;
  avr_timer_t      timer0, timer1, timer2, timer3;
  avr_spi_t        spi;
  avr_usb_t        usb;
} mcu_mega32u4 = {
  .core = {
    .mmcu = "atmega32u4",
    DEFAULT_CORE(4),
    
    .init = m32u4_init,
    .reset = m32u4_reset,

    .rampz = RAMPZ,
  },
  AVR_EEPROM_DECLARE(EE_READY_vect),
  AVR_SELFPROG_DECLARE(SPMCSR, SPMEN, SPM_READY_vect),
  AVR_WATCHDOG_DECLARE(WDTCSR, WDT_vect),
  .extint = {
    AVR_EXTINT_MEGA_DECLARE(0, 'D', PD0, A),
    AVR_EXTINT_MEGA_DECLARE(1, 'D', PD1, A),
    AVR_EXTINT_MEGA_DECLARE(2, 'D', PD2, A),
    AVR_EXTINT_MEGA_DECLARE(3, 'D', PD3, A),
    AVR_EXTINT_MEGA_DECLARE(6, 'E', PE6, B),
  },
    .portb = {
        .name = 'B', .r_port = PORTB, .r_ddr = DDRB, .r_pin = PINB,
        .pcint = {
            .enable = AVR_IO_REGBIT(PCICR, PCIE0),
            .raised = AVR_IO_REGBIT(PCIFR, PCIF0),
            .vector = PCINT0_vect,
        },
        .r_pcint = PCMSK0,
    },
    AVR_IOPORT_DECLARE(c, 'C', C),
    AVR_IOPORT_DECLARE(d, 'D', D),
    AVR_IOPORT_DECLARE(e, 'E', E),
    AVR_IOPORT_DECLARE(f, 'F', F),
    .uart1 = {
        .disabled = AVR_IO_REGBIT(PRR1, PRUSART1),
        .name = '1',
        .r_udr = UDR1,

        .txen = AVR_IO_REGBIT(UCSR1B, TXEN1),
        .rxen = AVR_IO_REGBIT(UCSR1B, RXEN1),
        .ucsz = AVR_IO_REGBITS(UCSR1C, UCSZ10, 0x3), // 2 bits
        .ucsz2 = AVR_IO_REGBIT(UCSR1B, UCSZ12),     // 1 bits

        .r_ucsra = UCSR1A,
        .r_ucsrb = UCSR1B,
        .r_ucsrc = UCSR1C,
        .r_ubrrl = UBRR1L,
        .r_ubrrh = UBRR1H,
        .rxc = {
            .enable = AVR_IO_REGBIT(UCSR1B, RXCIE1),
            .raised = AVR_IO_REGBIT(UCSR1A, RXC1),
            .vector = USART1_RX_vect,
        },
        .txc = {
            .enable = AVR_IO_REGBIT(UCSR1B, TXCIE1),
            .raised = AVR_IO_REGBIT(UCSR1A, TXC1),
            .vector = USART1_TX_vect,
        },
        .udrc = {
            .enable = AVR_IO_REGBIT(UCSR1B, UDRIE1),
            .raised = AVR_IO_REGBIT(UCSR1A, UDRE1),
            .vector = USART1_UDRE_vect,
        },
    },
      .timer0 = {
        .name = '0',
        .disabled = AVR_IO_REGBIT(PRR0, PRTIM0),
        .wgm = { AVR_IO_REGBIT(TCCR0A, WGM00), AVR_IO_REGBIT(TCCR0A, WGM01), AVR_IO_REGBIT(TCCR0B, WGM02) },
        .wgm_op = {
            [0] = AVR_TIMER_WGM_NORMAL8(),
            [2] = AVR_TIMER_WGM_CTC(),
            [3] = AVR_TIMER_WGM_FASTPWM8(),
            [7] = AVR_TIMER_WGM_OCPWM(),
        },
        .cs = { AVR_IO_REGBIT(TCCR0B, CS00), AVR_IO_REGBIT(TCCR0B, CS01), AVR_IO_REGBIT(TCCR0B, CS02) },
        .cs_div = { 0, 0, 3 /* 8 */, 6 /* 64 */, 8 /* 256 */, 10 /* 1024 */ },

        .r_tcnt = TCNT0,

        .overflow = {
            .enable = AVR_IO_REGBIT(TIMSK0, TOIE0),
            .raised = AVR_IO_REGBIT(TIFR0, TOV0),
            .vector = TIMER0_OVF_vect,
        },
        .comp = {
            [AVR_TIMER_COMPA] = {
                .r_ocr = OCR0A,
                .com = AVR_IO_REGBITS(TCCR0A, COM0A0, 0x3),
                .com_pin = AVR_IO_REGBIT(PORTB, 7),
                .interrupt = {
                    .enable = AVR_IO_REGBIT(TIMSK0, OCIE0A),
                    .raised = AVR_IO_REGBIT(TIFR0, OCF0A),
                    .vector = TIMER0_COMPA_vect,
                },
            },
            [AVR_TIMER_COMPB] = {
                .r_ocr = OCR0B,
                .com = AVR_IO_REGBITS(TCCR0A, COM0B0, 0x3),
                .com_pin = AVR_IO_REGBIT(PORTD, 0),
                .interrupt = {
                    .enable = AVR_IO_REGBIT(TIMSK0, OCIE0B),
                    .raised = AVR_IO_REGBIT(TIFR0, OCF0B),
                    .vector = TIMER0_COMPB_vect,
                }
            }
        }
    },
    .timer1 = {
        .name = '1',
        .disabled = AVR_IO_REGBIT(PRR0, PRTIM1),
        .wgm = { AVR_IO_REGBIT(TCCR1A, WGM10), AVR_IO_REGBIT(TCCR1A, WGM11),
                    AVR_IO_REGBIT(TCCR1B, WGM12), AVR_IO_REGBIT(TCCR1B, WGM13) },
        .wgm_op = {
            [0] = AVR_TIMER_WGM_NORMAL16(),
            [1] = AVR_TIMER_WGM_FCPWM8(),
            [2] = AVR_TIMER_WGM_FCPWM9(),
            [3] = AVR_TIMER_WGM_FCPWM10(),
            [4] = AVR_TIMER_WGM_CTC(),
            [5] = AVR_TIMER_WGM_FASTPWM8(),
            [6] = AVR_TIMER_WGM_FASTPWM9(),
            [7] = AVR_TIMER_WGM_FASTPWM10(),
            [12] = AVR_TIMER_WGM_ICCTC(),
            [14] = AVR_TIMER_WGM_ICPWM(),
            [15] = AVR_TIMER_WGM_OCPWM(),
        },
        .cs = { AVR_IO_REGBIT(TCCR1B, CS10), AVR_IO_REGBIT(TCCR1B, CS11), AVR_IO_REGBIT(TCCR1B, CS12) },
        .cs_div = { 0, 0, 3 /* 8 */, 6 /* 64 */, 8 /* 256 */, 10 /* 1024 */  /* External clock T1 is not handled */},

        .r_tcnt = TCNT1L,
        .r_tcnth = TCNT1H,
        .r_icr = ICR1L,
        .r_icrh = ICR1H,

        .ices = AVR_IO_REGBIT(TCCR1B, ICES1),
        .icp = AVR_IO_REGBIT(PORTD, 4),

        .overflow = {
            .enable = AVR_IO_REGBIT(TIMSK1, TOIE1),
            .raised = AVR_IO_REGBIT(TIFR1, TOV1),
            .vector = TIMER1_OVF_vect,
        },
        .icr = {
            .enable = AVR_IO_REGBIT(TIMSK1, ICIE1),
            .raised = AVR_IO_REGBIT(TIFR1, ICF1),
            .vector = TIMER1_CAPT_vect,
        },
        .comp = {
            [AVR_TIMER_COMPA] = {
                .r_ocr = OCR1AL,
                .r_ocrh = OCR1AH,    // 16 bits timers have two bytes of it
                .com = AVR_IO_REGBITS(TCCR1A, COM1A0, 0x3),
                .com_pin = AVR_IO_REGBIT(PORTB, 5),
                .interrupt = {
                    .enable = AVR_IO_REGBIT(TIMSK1, OCIE1A),
                    .raised = AVR_IO_REGBIT(TIFR1, OCF1A),
                    .vector = TIMER1_COMPA_vect,
                }
            },
            [AVR_TIMER_COMPB] = {
                .r_ocr = OCR1BL,
                .r_ocrh = OCR1BH,
                .com = AVR_IO_REGBITS(TCCR1A, COM1B0, 0x3),
                .com_pin = AVR_IO_REGBIT(PORTB, 6),
                .interrupt = {
                    .enable = AVR_IO_REGBIT(TIMSK1, OCIE1B),
                    .raised = AVR_IO_REGBIT(TIFR1, OCF1B),
                    .vector = TIMER1_COMPB_vect,
                }
            }
        }
    },
    .timer3 = {
        .name = '3',
        .disabled = AVR_IO_REGBIT(PRR1, PRTIM3),
        .wgm = { AVR_IO_REGBIT(TCCR3A, WGM30), AVR_IO_REGBIT(TCCR3A, WGM31),
                    AVR_IO_REGBIT(TCCR3B, WGM32), AVR_IO_REGBIT(TCCR3B, WGM33) },
        .wgm_op = {
            [0] = AVR_TIMER_WGM_NORMAL16(),
            [1] = AVR_TIMER_WGM_FCPWM8(),
            [2] = AVR_TIMER_WGM_FCPWM9(),
            [3] = AVR_TIMER_WGM_FCPWM10(),
            [4] = AVR_TIMER_WGM_CTC(),
            [5] = AVR_TIMER_WGM_FASTPWM8(),
            [6] = AVR_TIMER_WGM_FASTPWM9(),
            [7] = AVR_TIMER_WGM_FASTPWM10(),
            [12] = AVR_TIMER_WGM_ICCTC(),
            [14] = AVR_TIMER_WGM_ICPWM(),
            [15] = AVR_TIMER_WGM_OCPWM(),
        },
        .cs = { AVR_IO_REGBIT(TCCR3B, CS30), AVR_IO_REGBIT(TCCR3B, CS31), AVR_IO_REGBIT(TCCR3B, CS32) },
        .cs_div = { 0, 0, 3 /* 8 */, 6 /* 64 */, 8 /* 256 */, 10 /* 1024 */  /* External clock T1 is not handled */},

        .r_tcnt = TCNT3L,
        .r_tcnth = TCNT3H,
        .r_icr = ICR3L,
        .r_icrh = ICR3H,

        .ices = AVR_IO_REGBIT(TCCR3B, ICES3),
        .icp = AVR_IO_REGBIT(PORTC, 7),

        .overflow = {
            .enable = AVR_IO_REGBIT(TIMSK3, TOIE3),
            .raised = AVR_IO_REGBIT(TIFR3, TOV3),
            .vector = TIMER3_OVF_vect,
        },
        .icr = {
            .enable = AVR_IO_REGBIT(TIMSK3, ICIE3),
            .raised = AVR_IO_REGBIT(TIFR3, ICF3),
            .vector = TIMER3_CAPT_vect,
        },
        .comp = {
            [AVR_TIMER_COMPA] = {
                .r_ocr = OCR3AL,
                .r_ocrh = OCR3AH,    // 16 bits timers have two bytes of it
                .com = AVR_IO_REGBITS(TCCR3A, COM3A0, 0x3),
                .com_pin = AVR_IO_REGBIT(PORTC, 6),
                .interrupt = {
                    .enable = AVR_IO_REGBIT(TIMSK3, OCIE3A),
                    .raised = AVR_IO_REGBIT(TIFR3, OCF3A),
                    .vector = TIMER3_COMPA_vect,
                }
            },
            [AVR_TIMER_COMPB] = {
                .r_ocr = OCR3BL,
                .r_ocrh = OCR3BH,
                .com = AVR_IO_REGBITS(TCCR3A, COM3B0, 0x3),
                .com_pin = AVR_IO_REGBIT(PORTC, 6), // WTF nothing in doc about this
                .interrupt = {
                    .enable = AVR_IO_REGBIT(TIMSK3, OCIE3B),
                    .raised = AVR_IO_REGBIT(TIFR3, OCF3B),
                    .vector = TIMER3_COMPB_vect,
                }
            }
        }
    },
  //  .timer4 = { /* TODO 10 bits realtime timer */ },
    AVR_SPI_DECLARE(0, 0),
    .usb = {
        .name='1',
        .disabled = AVR_IO_REGBIT(PRR1, PRUSB), // bit in the PRR

        .usbrf = AVR_IO_REGBIT(MCUSR, USBRF),    // bit in the MCUSR

        .r_usbcon = USBCON,
        .r_pllcsr = PLLCSR,

        .usb_com_vect = USB_COM_vect,
        .usb_gen_vect = USB_GEN_vect,
    },
};


static avr_t * make()
{
    return avr_core_allocate(&mcu_mega32u4.core, sizeof(struct mcu_t));
}

avr_kind_t mega32u4 = {
    .names = { "atmega32u4", },
    .make = make
};

void m32u4_init(struct avr_t * avr)
{
    struct mcu_t * mcu = (struct mcu_t*)avr;

    avr_eeprom_init(avr, &mcu->eeprom);
    avr_flash_init(avr, &mcu->selfprog);
    avr_extint_init(avr, &mcu->extint);
    avr_watchdog_init(avr, &mcu->watchdog);
    avr_ioport_init(avr, &mcu->portb);
    avr_ioport_init(avr, &mcu->portc);
    avr_ioport_init(avr, &mcu->portd);
    avr_ioport_init(avr, &mcu->porte);
    avr_ioport_init(avr, &mcu->portf);
    avr_uart_init(avr, &mcu->uart1);
    avr_timer_init(avr, &mcu->timer0);
    avr_timer_init(avr, &mcu->timer1);
    avr_timer_init(avr, &mcu->timer3);
    avr_spi_init(avr, &mcu->spi);
    avr_usb_init(avr, &mcu->usb);
}

void m32u4_reset(struct avr_t * avr)
{
//    struct mcu_t * mcu = (struct mcu_t*)avr;
}
