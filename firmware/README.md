# Firmware — Gecko Keeper

ESP32 기반 크레스티드게코 사육장 자동 분무 제어 펌웨어.

> AI 작업 컨텍스트는 [CLAUDE.md](CLAUDE.md) 참고.  
> 전체 프로젝트 컨텍스트는 루트 [CLAUDE.md](../CLAUDE.md) 및 [docs/PROJECT_CONTEXT.md](../docs/PROJECT_CONTEXT.md) 참고.

---

## 구현 상태

| 기능 | 상태 | 비고 |
|---|---|---|
| DHT22 온습도 읽기 | 코드 완성, 하드웨어 미연결 | 부품 수령 후 테스트 예정 |
| 릴레이 제어 (펌프 ON/OFF) | 동작 확인 완료 | Buck 5V 공급 시 정상 동작 |
| 시리얼 출력 (온도/습도) | 코드 완성 | 115200 baud |
| WiFi 연결 | 미구현 | Step 2 |
| 백엔드 HTTP 통신 | 미구현 | Step 2 |

---

## 핀 맵

| GPIO | 역할 | 비고 |
|---|---|---|
| 4 | DHT22 DATA | 4.7kΩ pull-up → 3.3V |
| 5 | 릴레이 IN | Active HIGH, 3.3V 신호 |

---

## 릴레이 동작 방식

- **Active HIGH**: GPIO HIGH → 펌프 ON / GPIO LOW → 펌프 OFF
- **VCC**: Buck 컨버터 5V 공급 (ESP32 USB 전원만으론 전류 부족)
- **IN 신호**: ESP32 GPIO 3.3V — Buck 5V VCC 상태에서 정상 동작 확인

---

## 빌드 & 업로드

```powershell
# 빌드
pio run -e esp32dev

# 빌드 + 업로드
pio run -e esp32dev --target upload

# 시리얼 모니터
pio device monitor
```

---

## 테스트

| 테스트 suite | 파일 | 상태 |
|---|---|---|
| `test_relay` | `test/test_relay/test_relay.cpp` | PASSED |
| `test_dht22` | 미작성 (DHT22 연결 후 예정) | - |

```powershell
# 릴레이 테스트
pio test -e esp32dev -f test_relay

# 전체 테스트
pio test -e esp32dev
```

**릴레이 테스트 동작 순서:**
1. OFF 상태 확인 (소리 없음)
2. ON 1초 유지 → 딸깍
3. OFF 1초 유지 → 딸깍

---

## 의존성 (platformio.ini)

```ini
lib_deps =
    adafruit/DHT sensor library@^1.4.6
    adafruit/Adafruit Unified Sensor@^1.1.14
```
