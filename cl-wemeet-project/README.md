# CL-WeMeet  
**WeMeet을 모티브로 한 실시간 협업 웹 플랫폼**

![Demo](media/Demo_Video.gif)

> **실시간 채팅 · 인터랙티브 퀴즈 · 웹툰 추천 · 팀 소개**  
> 한 번에 즐기는 올인원 협업 공간  


<br>

## 프로젝트 미리 보기

| 메인 화면 & 전체 UI |
|---------------------|
| ![메인 화면](images/최종.png) |

<br>

| 우리 팀의 빛나는 순간들 |
|-------------------------|
| ![팀 사진1](assets/Pt_moment_01.JPG)  ![팀 사진2](assets/Pt_moment_02.JPG)  ![팀 사진3](assets/Pt_moment_03.JPG) |

<br>

## 핵심 기능

| 기능                | 설명                                                   | 스크린샷                                      |
|---------------------|--------------------------------------------------------|-----------------------------------------------|
| 실시간 채팅       | Node.js + Express로 구현된 완전 동작 채팅 시스템       | ![채팅](img/chatbot.png)                     |
| 인터랙티브 퀴즈    | JSON 기반 랜덤 문제 출제 → 실시간 정답 확인 + 점수 표시 | ![퀴즈](img/quizboard.png)                   |
| 웹툰 추천 슬라이드  | 자동 슬라이드 + 클릭 시 상세 정보 표시                  | ![웹툰](img/webtoon.png)                     |
| 고급스러운 UI/UX   | Tencent WeMeet 디자인 완벽 재현 + 부드러운 애니메이션   | 전체 페이지 적용                             |
| 완전 반응형        | 모바일 · 태블릿 · 데스크톱 어디서나 완벽 동작           | 자동 지원                                    |

<br>

## 기술 스택

| 분야         | 사용 기술                                      |
|--------------|------------------------------------------------|
| Frontend     | HTML5 · CSS3 · Vanilla JavaScript              |
| Backend      | Node.js · Express.js                           |
| Server       | `server.js` (포트 3000)                        |
| 데이터       | JSON (`questions.json`, `quiz.json`)           |
| 디자인       | WeMeet 스타일 UI/UX 완벽 재현                  |

<br>

## 프로젝트 구조

```plaintext
cl-wemeet-project/
├── assets/           팀 순간 사진 (Pt_moment_01~03.JPG)
├── css/              style.css
├── data/             questions.json (퀴즈 데이터)
├── images/           배경, 버튼 이미지
├── img/              아이콘, 팀원 사진, 기능 심볼
├── js/               main.js, quiz.js
├── media/            Demo_Video.gif
├── node_modules/     Express 등 의존성
├── server.js         실시간 채팅 서버
├── package.json
└── *.html            홈, 채팅, 퀴즈, 웹툰, 팀 소개 등