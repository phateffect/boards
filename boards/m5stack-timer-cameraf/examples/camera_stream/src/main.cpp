// M5Stack Timer Camera F — OV3660 摄像头初始化 + 抓拍示例
// 按本板 DVP 接线（见板子页「摄像头 DVP 接线」表）配置 esp32-camera。
// 摄像头(SCCB/信号)细节见 modules/ov3660-camera。
//
// 说明：本示例只做"点亮+抓一帧"，验证接线；HTTP 图传请见 M5 官方 TimerCamera 例程。
#include "esp_camera.h"
#include <Arduino.h>

// ---- Timer Camera F 板级 DVP 接线（= 板子页 DVP 接线表） ----
#define CAM_PIN_SIOD   25   // SCCB SDA
#define CAM_PIN_SIOC   23   // SCCB SCL
#define CAM_PIN_XCLK   27
#define CAM_PIN_VSYNC  22
#define CAM_PIN_HREF   26
#define CAM_PIN_PCLK   21
#define CAM_PIN_D0     32
#define CAM_PIN_D1     35
#define CAM_PIN_D2     34
#define CAM_PIN_D3     5
#define CAM_PIN_D4     39
#define CAM_PIN_D5     18
#define CAM_PIN_D6     36
#define CAM_PIN_D7     19
#define CAM_PIN_RESET  15
#define CAM_PIN_PWDN   -1   // 本板未接

static camera_config_t cam_cfg() {
    camera_config_t c{};
    c.ledc_channel = LEDC_CHANNEL_0;
    c.ledc_timer   = LEDC_TIMER_0;
    c.pin_d0  = CAM_PIN_D0;  c.pin_d1 = CAM_PIN_D1;  c.pin_d2 = CAM_PIN_D2;
    c.pin_d3  = CAM_PIN_D3;  c.pin_d4 = CAM_PIN_D4;  c.pin_d5 = CAM_PIN_D5;
    c.pin_d6  = CAM_PIN_D6;  c.pin_d7 = CAM_PIN_D7;
    c.pin_xclk = CAM_PIN_XCLK;
    c.pin_pclk = CAM_PIN_PCLK;
    c.pin_vsync = CAM_PIN_VSYNC;
    c.pin_href  = CAM_PIN_HREF;
    c.pin_sccb_sda = CAM_PIN_SIOD;
    c.pin_sccb_scl = CAM_PIN_SIOC;
    c.pin_pwdn = CAM_PIN_PWDN;
    c.pin_reset = CAM_PIN_RESET;
    c.xclk_freq_hz = 20000000;          // XCLK 20MHz
    c.frame_size   = FRAMESIZE_VGA;     // 640x480（OV3660 可更高）
    c.pixel_format = PIXFORMAT_RGB565;
    c.grab_mode    = CAMERA_GRAB_LATEST;
    c.fb_location  = CAMERA_FB_IN_PSRAM; // 用 8MB PSRAM
    c.jpeg_quality = 12;
    c.fb_count     = 2;
    return c;
}

void setup() {
    Serial.begin(115200);
    Serial.println("[timer-cameraf] init OV3660 via esp32-camera");

    camera_config_t config = cam_cfg();
    esp_err_t err = esp_camera_init(&config);
    Serial.printf("esp_camera_init: %s\n", esp_err_to_name(err));
    if (err != ESP_OK) { Serial.println("camera init failed — 检查 DVP 接线/PSRAM"); return; }

    sensor_t *s = esp_camera_sensor_get();
    Serial.printf("sensor PID=0x%x (OV3660=0x%04x)\n",
                  s ? s->id.PID : 0, (int)OV3660_PID);

    // 抓一帧验证
    camera_fb_t *fb = esp_camera_fb_get();
    if (fb) {
        Serial.printf("captured frame: %dx%d, len=%u, format=%d\n",
                      fb->width, fb->height, (unsigned)fb->len, fb->format);
        esp_camera_fb_return(fb);
    } else {
        Serial.println("fb get failed");
    }
}

void loop() { delay(2000); }
