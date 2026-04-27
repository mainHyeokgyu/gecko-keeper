# 📌 파충류 스마트 사육장 플랫폼

---

# 1. 프로젝트 개요

## 1.1 목적

파충류 사육장을 대상으로 다음 기능을 제공하는 스마트 IoT 플랫폼을 개발한다.

- 원격 자동 분무 제어 (웹/모바일)
- 실시간 환경 모니터링 (온도, 습도)
- 영상 기반 개체 상태 관찰 (홈캠)
- 이상 상황 감지 및 알림
- 데이터 기반 사육 환경 최적화

---

## 1.2 핵심 가치

| 항목 | 설명 |
| --- | --- |
| 자동화 | 사용자가 직접 관리하지 않아도 환경(습도) 유지 |
| 원격 제어 | 외부에서도 제어 가능 |
| 데이터 기반 | 센싱 데이터 축적 및 분석 |
| 확장성 | 다수 사육장 관리 가능 |
| 사업성 | 반려 파충류 시장 타겟 |

---

# 2. 시스템 전체 구조

```
[사용자 앱 (Web / Mobile)]
            │
            ▼
     API Gateway / BFF
            │
            ▼
==============================
        Kafka Cluster
==============================
      ▲             ▲
      │             │
[Edge Gateway]   [MSA Services]
      ▲             │
      │             ▼
[Embedded Device]  DB / Cache / Storage
```

---

# 3. 디바이스 구성 (Embedded Layer)

## 3.1 하드웨어 구성

- MCU: ESP32 / Raspberry Pi
- 센서:
    - 온도 센서
    - 습도 센서
- 액추에이터:
    - 분무기 (릴레이 제어)
- 카메라:
    - 홈캠 (RTSP 기반)

---

## 3.2 기능

- 센서 데이터 수집
- 분무기 ON/OFF 제어
- 이벤트 발생 (임계값 초과 등)
- Edge Gateway로 데이터 전송 (MQTT)

---

# 4. Edge Gateway 설계

## 4.1 역할

- 디바이스 데이터 수신 (MQTT / TCP)
- 데이터 정규화
- Kafka로 이벤트 전달
- 홈캠 스트림 처리 (이벤트 추출)

---

## 4.2 핵심 포인트

- Embedded ↔ Kafka 직접 연결 ❌
- Gateway를 통한 안정적 처리 ⭕
- Burst 트래픽 흡수

---

# 5. Kafka 기반 이벤트 설계

## 5.1 Topic 구성

| Topic | 설명 | Key |
| --- | --- | --- |
| sensor.telemetry | 온습도 데이터 | device_id |
| actuator.command | 분무 제어 명령 | device_id |
| actuator.result | 제어 결과 | device_id |
| camera.event | 영상 분석 이벤트 | camera_id |
| behavior.event | 개체 행동 이벤트 | enclosure_id |
| alert.event | 경보 이벤트 | enclosure_id |

---

## 5.2 이벤트 예시

### Sensor Data

```
{
  device_id: "A01",
  temperature: 28.5,
  humidity: 62.3,
  timestamp: ...
}
```

### Spray Command

```
{
  device_id: "A01",
  action: "SPRAY_ON",
  duration: 5s
}
```

### Behavior Event

```
{
  enclosure_id: "E01",
  type: "LOW_ACTIVITY",
  confidence: 0.87
}
```

---

# 6. MSA 서비스 구성

## 6.1 서비스 목록

### 1) Device Service

- 디바이스 상태 관리
- 등록 / 인증

---

### 2) Environment Service

- 온습도 데이터 처리
- 평균 / 추세 계산

---

### 3) Spray Control Service

- 자동 분무 로직
- 사용자 명령 처리

```
조건 예:
humidity < 60% → 분무 실행
```

---

### 4) Camera Analysis Service

- 영상 기반 이벤트 생성
- 움직임 / 행동 분석

---

### 5) Behavior Analysis Service

- 데이터 기반 이상 감지

예:

- 장시간 움직임 없음
- 비정상적인 활동 패턴

---

### 6) Alert Service

- 이벤트 기반 알림 생성

```
LOW_ACTIVITY + HIGH_TEMP → 위험 알림
```

---

### 7) Notification Service

- WebSocket / Push 알림
- 사용자 전달

---

# 7. 데이터 저장 구조

| 데이터 | 저장소 |
| --- | --- |
| 센서 데이터 | TimescaleDB |
| 상태 캐시 | Redis |
| 사용자 정보 | PostgreSQL |
| 이벤트 로그 | Kafka |
| 영상 | NAS / Object Storage |

---

# 8. 웹 / 모바일 앱 기능

## 8.1 주요 기능

- 실시간 환경 모니터링
- 분무 제어 버튼
- 자동 모드 설정
- 홈캠 영상 확인
- 알림 확인

---

## 8.2 UI 요소

- 사육장 리스트
- 온습도 그래프
- 이벤트 타임라인
- 상태 표시 (정상 / 경고)

---

# 9. 홈캠 및 영상 처리 전략

## 9.1 구조

```
RTSP Stream → Edge AI → Event → Kafka
```

---

## 9.2 처리 원칙

- 영상 자체는 Kafka로 보내지 않음
- 이벤트만 Kafka 전송

---

## 9.3 분석 항목

- 움직임 감지
- 활동량 분석
- 특정 행동 패턴

---

# 10. 대용량 데이터 설계 전략

## 10.1 확장 시나리오

- 사육장 수: 1 → 1,000+
- 사용자 증가
- 카메라 증가

---

## 10.2 트래픽 발생 요소

- 센서 주기 데이터
- 영상 이벤트
- 사용자 제어 요청
- 알림 이벤트

---

## 10.3 Kafka 활용 포인트

- Partition 기반 확장
- Consumer Group 확장
- Replay 가능 구조

---

# 11. 자동 제어 로직 (핵심 기능)

## 11.1 Rule 기반

```
IF humidity < threshold
THEN spray ON
```

---

## 11.2 향후 확장

- ML 기반 제어
- 개체별 맞춤 환경
- 계절별 자동 조정

---

# 12. 사업 확장 가능성

## 12.1 타겟 시장

- 파충류 / 양서류 사육자
- 전문 브리더
- 동물원 / 연구소

---

## 12.2 확장 방향

- 구독형 SaaS
- 스마트 사육장 패키지 판매
- 데이터 기반 컨설팅

---

# 13. 핵심 기술 스택

## Embedded

- ESP32 / Raspberry Pi
- MQTT

## Backend

- Kafka
- Spring Boot / FastAPI
- Redis
- TimescaleDB

## Frontend

- React
- WebSocket

---

# 14. 핵심 설계 요약

이 프로젝트의 본질은 다음 3가지다:

1. **IoT 제어 시스템 (분무기)**
2. **이벤트 기반 데이터 플랫폼 (Kafka)**
3. **지능형 분석 시스템 (행동 + 환경)**