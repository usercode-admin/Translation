#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "esp_camera.h"

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7789  _panel_instance;
  lgfx::Bus_SPI       _bus_instance; [span_4](start_span)//[span_4](end_span)

public:
  LGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI2_HOST; [span_5](start_span)//[span_5](end_span)
      cfg.spi_mode = 0;
      cfg.freq_write = 80000000;
      cfg.freq_read  = 16000000;
      cfg.spi_3wire  = true;
      cfg.use_lock   = true; [span_6](start_span)//[span_6](end_span)
      cfg.dma_channel = SPI_DMA_CH_AUTO;

      // ĐÃ SỬA CHÂN MÀN HÌNH ĐỂ TRÁNH XUNG ĐỘT CAMERA
      cfg.pin_sclk = 14; 
      cfg.pin_mosi = 47; 
      cfg.pin_miso = -1;
      cfg.pin_dc   = 21; 

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs           = 42; 
      cfg.pin_rst          = 2;  
      cfg.pin_busy         = -1; [span_7](start_span)//[span_7](end_span)
      cfg.memory_width     = 240;
      cfg.memory_height    = 240; [span_8](start_span)//[span_8](end_span)
      cfg.panel_width      = 240;
      cfg.panel_height     = 240; [span_9](start_span)//[span_9](end_span)
      cfg.offset_x         = 0;
      cfg.offset_y         = 0; [span_10](start_span)//[span_10](end_span)
      cfg.offset_rotation  = 1;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1; [span_11](start_span)//[span_11](end_span)
      cfg.readable         = false;
      cfg.invert           = true; [span_12](start_span)//[span_12](end_span)
      cfg.rgb_order        = false; [span_13](start_span)//[span_13](end_span)
      cfg.bus_shared       = false;

      _panel_instance.config(cfg);
    }
    setPanel(&_panel_instance);
  }
};

LGFX lcd;

// CHÂN CAMERA GIỮ NGUYÊN MẶC ĐỊNH CỦA KIT S3 CAM
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     15
#define SIOD_GPIO_NUM     4
#define SIOC_GPIO_NUM     5
#define Y9_GPIO_NUM       16
#define Y8_GPIO_NUM       17
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       12
#define Y5_GPIO_NUM       10
#define Y4_GPIO_NUM       8
#define Y3_GPIO_NUM       9
#define Y2_GPIO_NUM       11
#define VSYNC_GPIO_NUM    6
#define HREF_GPIO_NUM     7
#define PCLK_GPIO_NUM     13

void setup() {
  Serial.begin(115200);
  lcd.init(); [span_14](start_span)//[span_14](end_span)
  lcd.fillScreen(0x0000); 

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM; [span_15](start_span)//[span_15](end_span)
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM; [span_16](start_span)//[span_16](end_span)
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM; [span_17](start_span)//[span_17](end_span)

  config.xclk_freq_hz = 24000000;
  config.pixel_format = PIXFORMAT_RGB565;
  config.frame_size   = FRAMESIZE_240X240;
  config.jpeg_quality = 10;
  config.fb_count     = 3; [span_18](start_span)//[span_18](end_span)
  config.fb_location  = CAMERA_FB_IN_PSRAM;
  config.grab_mode    = CAMERA_GRAB_WHEN_EMPTY;
  
  esp_err_t err = esp_camera_init(&config); [span_19](start_span)//[span_19](end_span)
  if (err != ESP_OK) {
    lcd.drawString("Camera initialization error", 10, 10);
    return;
  [span_20](start_span)} //[span_20](end_span)

  sensor_t * s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);
    s->set_gain_ctrl(s, 1); [span_21](start_span)//[span_21](end_span)
    s->set_gainceiling(s, GAINCEILING_16X);
    s->set_exposure_ctrl(s, 1);
    s->set_aec2(s, 1);
    s->set_aec_value(s, 450);
    s->set_denoise(s, 1);
  [span_22](start_span)} //[span_22](end_span)
}

void loop() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) return;

  lcd.pushImage(0, 0, 240, 240, (uint16_t *)fb->buf);

  esp_camera_fb_return(fb);
}