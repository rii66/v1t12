#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID       "ICA ELEK"
#define WIFI_PASS       "koyok tekek"

#define AP_SSID         "SolderStation"
#define AP_PASS         "88889999"

#define OTA_HOSTNAME    "SolderStation"

/* ===== PINS ===== */
#define TEMP_PIN        0
#define PWM_PIN         2
#define BUZZER_PIN      8
#define RGB_RED         9
#define RGB_BLUE        10
#define ENC_SW          3
#define ENC_A           4
#define ENC_B           5
#define OLED_SDA        6
#define OLED_SCL        7
#define MOTION_PIN      1

/* ===== TIP TYPE ===== */
#define TIP_T12         0
#define TIP_C210        1

/* ===== PID T12 ===== */
#define PID_KP_T12      3.2f
#define PID_KI_T12      0.12f
#define PID_KD_T12      1.8f

/* ===== PID C210 ===== */
#define PID_KP_C210     2.8f
#define PID_KI_C210     0.10f
#define PID_KD_C210     1.4f

/* ===== TEMP LIMIT ===== */
#define TEMP_MIN        100
#define TEMP_MAX_T12    450
#define TEMP_MAX_C210   380

/* ===== DEFAULT TEMP ===== */
#define DEFAULT_TEMP    320
#define BOOST_TEMP      380
#define SLEEP_TEMP      180

/* ===== BOOST ===== */
#define BOOST_TIME      10000

#endif