#ifndef __AD1836A_H__
#define __AD1836A_H__

// 1割り込みあたりのサンプル数
// タスクにはこのサンプル数のデータが渡される
#define SAMPLE_PER_INTR 16

// CODECレジスタアドレス。VDSP++のサンプルからコピー
#define DAC_CONTROL_1		0x0000
#define DAC_CONTROL_2		0x1000
#define DAC_VOLUME_0		0x2000
#define DAC_VOLUME_1		0x3000
#define DAC_VOLUME_2		0x4000
#define DAC_VOLUME_3		0x5000
#define DAC_VOLUME_4		0x6000
#define DAC_VOLUME_5		0x7000
#define ADC_0_PEAK_LEVEL	0x8000
#define ADC_1_PEAK_LEVEL	0x9000
#define ADC_2_PEAK_LEVEL	0xA000
#define ADC_3_PEAK_LEVEL	0xB000
#define ADC_CONTROL_1		0xC000
#define ADC_CONTROL_2		0xD000
#define ADC_CONTROL_3		0xE000

// タスク間でやり取りされるデータ型
struct CODEC_BUFFER{
	int data[SAMPLE_PER_INTR][8];
} ;


//

#ifdef __cplusplus
  extern "C" {
#endif

extern void run_ad1836_task( VP_INT exinf );
void spi_handler(void);
void sport0_handler(void);

#ifdef __cplusplus
  };
#endif

extern const int codec_rx_dtq;
extern const int codec_tx_dtq;
extern const int spi_sem;
extern const int sport0_sem;


#endif
