# CL-WeMeet  
**WeMeet 스타일 실시간 협업 웹 플랫폼**

![데모 영상](media/Demo_Video.gif)

> **실시간 채팅 · Azure 클라우드 퀴즈 · 웹툰 추천 · 팀 소개**  
> 귀여움 + 기능성 다 잡은 2025 종합설계 최고의 작품!

<br>

## 프로젝트 미리 보기

| 메인 화면 & 전체 분위기 |
|-------------------------|
| ![메인 화면](images/최종.png) |

<br>

| 우리 팀의 따뜻한 순간들 |
|-------------------------|
| ![팀 사진1](assets/Pt_moment_01.JPG)  ![팀 사진2](assets/Pt_moment_02.JPG)  ![팀 사진3](assets/Pt_moment_03.JPG) |

<br>

## 핵심 기능

| 기능                  | 설명                                                    | 실제 화면                                     |
|-----------------------|---------------------------------------------------------|-----------------------------------------------|
| 실시간 채팅           | Node.js + Express 기반 완전 동작 채팅 시스템            | ![채팅 화면](img/chatbot.png)                |
| Azure 클라우드 퀴즈   | 60문항 JSON 기반 랜덤 출제<br>실시간 정답 확인 + 점수 표시 | ![퀴즈 정답](https://raw.githubusercontent.com/INJAE0818/Choi_Projects/main/cl-wemeet-project/img/quiz_wrong_example.jpg)<br>![퀴즈 오답](https://raw.githubusercontent.com/INJAE0818/Choi_Projects/main/cl-wemeet-project/img/quiz_right_example.jpg) |
| 퀴즈 인사이트         | 자주 틀리는 문제 / 자주 맞는 문제 랭킹 실시간 제공      | ![퀴즈 인사이트](https://raw.githubusercontent.com/INJAE0818/Choi_Projects/main/cl-wemeet-project/img/quiz_1.png)
[퀴즈 인사이트2](https://raw.githubusercontent.com/INJAE0818/Choi_Projects/main/cl-wemeet-project/img/quiz_2.png)
[퀴즈 인사이트3](https://raw.githubusercontent.com/INJAE0818/Choi_Projects/main/cl-wemeet-project/img/quiz_3.png) |
| 언어 장벽 없는 회의   | 재미있는 4칸 만화로 Azure 서비스 설명 (웃음 보장)       | ![만화 설명](https://raw.githubusercontent.com/INJAE0818/Choi_Projects/main/cl-wemeet-project/img/webtoon_home.png) |
| 웹툰 추천 인덱스       | 귀여운 로봇이 안내하는 자동 슬라이드 웹툰 추천          | ![웹툰 인덱스](https://raw.githubusercontent.com/INJAE0818/Choi_Projects/main/cl-wemeet-project/img/webtoon_language.png) |
| 완전 반응형 디자인    | 모바일 · 태블릿 · 데스크톱 어디서나 완벽                 | 모든 페이지 적용                              |

<br>

## 기술 스택

| 분야         | 사용 기술                                      |
|--------------|------------------------------------------------|
| Frontend     | HTML5 · CSS3 · Vanilla JavaScript              |
| Backend      | Node.js · Express.js                           |
| Server       | `server.js` (포트 3000)                        |
| 데이터       | JSON (`questions.json` – 총 60문항)            |
| 디자인       | Cloud Corporation 스타일 귀여운 일러스트       |

<br>

## 프로젝트 구조

```plaintext
cl-wemeet-project/
├── assets/           팀 순간 사진
├── css/              style.css
├── data/             questions.json (Azure 퀴즈 60문제)
├── images/           메인 배경, 버튼
├── img/              퀴즈, 웹툰, 채팅, 만화 이미지
├── js/               main.js, quiz.js
├── media/            Demo_Video.gif
├── server.js         실시간 채팅 서버
└── *.html            메인, 퀴즈, 웹툰, 인사이트 등