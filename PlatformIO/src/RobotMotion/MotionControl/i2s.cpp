#include "Arduino.h"
#ifdef ENABLE_I2S
#include "i2s_lcl.h"
#include "driver/i2s.h"


// Bit field and masking macros
// bitnum_to_mask(n) is similar to bit(n) as defined in Arduino.h.
// We define our own version so we can apply the static_cast, thus making it work with scoped enums,
// using a different name to avoid name conflicts and include ordering issues with Arduino.h
#define bitnum_to_mask(n) (1 << static_cast<unsigned int>(n))

#define set_bits(target, mask) (target) |= (mask)
#define clear_bits(target, mask) (target) &= ~(mask)

const int I2S_OUT_PIN_BASE = 128;   
#define I2SO(n) (I2S_OUT_PIN_BASE + n)
void i2s_mask(uint8_t bitnum, uint8_t val);


//i2s configuration 
static int i2s_num = 0; // i2s port number
static i2s_config_t i2s_config = {
     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
     .sample_rate = 50000,
     .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
     .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
     .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
     .dma_buf_count = 8,
     .dma_buf_len = 64   //Interrupt level 1

};
    

static i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCK , //this is BCK pin
    .ws_io_num = I2S_WS, // this is LRCK pin
    .data_out_num = I2S_DATA, // this is DATA output pin
    .data_in_num = -1   //Not used
};

// output value
static uint32_t i2s_port_data = 0;

void
i2s_init() {
      //initialize i2s with configurations above
  i2s_driver_install((i2s_port_t)i2s_num, &i2s_config, 0, NULL);
  i2s_set_pin((i2s_port_t)i2s_num, &pin_config);
  i2s_set_sample_rates((i2s_port_t)i2s_num, 50000); 


  I2S0.conf1.tx_stop_en = 0;  // set to keep xmting clks when fifo empty
          // Use normal clock format, (WS is aligned with the last bit)
  I2S0.conf.tx_msb_shift = 0;
  I2S0.conf.rx_msb_shift = 0;
    
  I2S0.fifo_conf.tx_fifo_mod_force_en = 1;  // should always be set to 1 to enable Tx fifo.
	I2S0.conf1.tx_stop_en = 0;  // set to keep xmting clks when fifo empty
  //I2S0_fifo_wr_reg = 0; // seed the FIFO - sets all expanders o/puts low 
	I2S0.conf.tx_start = 1; // stop any XMT

  // Disconnect DMA from FIFO
  //I2S0.fifo_conf.dscr_en = 0;  // clr this bit to disable I2S DMA mode. (R/W)
  //I2S0.conf_single_data = 0xffffffff;
  I2S0.conf_chan.tx_chan_mod=3;   //When I2S_TX_MSB_RIGHT equals 0, the right-channel data are constants in the range of REG[31:0].
  I2S0.conf.tx_msb_right=1;
}
// inner lock
static portMUX_TYPE i2s_out_spinlock = portMUX_INITIALIZER_UNLOCKED;
#    define I2S_OUT_ENTER_CRITICAL()                                                                                                       \
        do {                                                                                                                               \
            if (xPortInIsrContext()) {                                                                                                     \
                portENTER_CRITICAL_ISR(&i2s_out_spinlock);                                                                                 \
            } else {                                                                                                                       \
                portENTER_CRITICAL(&i2s_out_spinlock);                                                                                     \
            }                                                                                                                              \
        } while (0)
#    define I2S_OUT_EXIT_CRITICAL()                                                                                                        \
        do {                                                                                                                               \
            if (xPortInIsrContext()) {                                                                                                     \
                portEXIT_CRITICAL_ISR(&i2s_out_spinlock);                                                                                  \
            } else {                                                                                                                       \
                portEXIT_CRITICAL(&i2s_out_spinlock);                                                                                      \
            }                                                                                                                              \
        } while (0)
#    define I2S_OUT_ENTER_CRITICAL_ISR() portENTER_CRITICAL_ISR(&i2s_out_spinlock)
#    define I2S_OUT_EXIT_CRITICAL_ISR() portEXIT_CRITICAL_ISR(&i2s_out_spinlock)


void IRAM_ATTR i2s_mask(uint8_t bitnum, uint8_t val ) {
  //Set/clear bits
  //If we're not in an interrupt, should we disable interrupts


  I2S_OUT_ENTER_CRITICAL();
  if(val) {
    set_bits(i2s_port_data, 1 << bitnum);
  } else {
    clear_bits(i2s_port_data, 1 << bitnum);
  }
  I2S_OUT_EXIT_CRITICAL();

}
void IRAM_ATTR
i2s_push_sample() {

 // i2s_write((i2s_port_t)i2s_num, &i2s_port_data, 4, &bytes_written, 100);
  //i2s_port_data |= 0x80;
  I2S_OUT_ENTER_CRITICAL();
  I2S0.conf_single_data = i2s_port_data << 16;
  I2S_OUT_EXIT_CRITICAL();

}


void IRAM_ATTR
my_digitalWrite(uint8_t pin, uint8_t val) {
        if(pin >= 128) {
            i2s_mask(pin-128, val);
        } else {
            digitalWrite(pin, val);
        }
  }

uint32_t getI2S() {
  return i2s_port_data;
}
#else
void 
i2s_init() {};
void IRAM_ATTR
my_digitalWrite(uint8_t pin, uint8_t val)  {
    digitalWrite(pin, val);
}

void IRAM_ATTR
i2s_push_sample() {}
#endif