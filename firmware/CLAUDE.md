# Firmware CLAUDE.md — Gecko Keeper

ESP32 펌웨어 작업 시 AI 컨텍스트. 전체 프로젝트 규칙은 루트 CLAUDE.md 참고.

---

## 현재 구현 상태

- `src/main.cpp`: DHT22 읽기 + 릴레이 제어 스켈레톤 완성
- WiFi 연결 / 백엔드 HTTP 통신 미구현 (다음 단계)
- 릴레이 단독 동작 확인 완료, DHT22 하드웨어 연결 미완료

## 핵심 하드웨어 제약

| 항목 | 내용 |
|---|---|
| 릴레이 방식 | Active HIGH (HIGH=ON) |
| 릴레이 VCC | 반드시 Buck 컨버터 5V — USB 전원만으론 전류 부족 |
| DHT22 VCC | ESP32 3V3 핀 (5V 사용 시 GPIO 손상 위험) |
| DHT22 pull-up | 4.7kΩ → 3.3V 라인 |
| GPIO 4 | DHT22 DATA |
| GPIO 5 | 릴레이 IN |

## 다음 구현 단계

1. WiFi 연결 (WiFiClient)
2. 백엔드로 센서 데이터 HTTP POST
3. 백엔드로부터 릴레이 제어 명령 수신
4. 자동 제어 로직 (습도 임계값)

## 참고

- 회로 설계 전체: `docs/CIRCUIT_DESIGN.md`
- 백엔드 API 엔드포인트: 루트 `CLAUDE.md` → REST API 섹션
