/*
2017 / 08 / 23, 20 : 30분 최종점검 끝.
(1) 08 / 11 테트리스 피하기 -> 별 피하기 수정
__이유__
1.테트리스 AI를 C언어로 구현하는데 어려움이 있음

2.또한 기존의 테트리스와 다르게 여러개의 블럭을 떨어트리며 자동완성 시킬 방법을 못 찾음.

(2) 08 / 19 게임 도중 pause를 하게 만들고 싶었으나 불가함.

(3) 별과 충돌시 충돌 사운드를 넣으려 하였으나 bgm으로 플레이하고 있는 노래와 충돌을 일으켜 못 넣음.
+SND_NOSTOP으로 해결하려 하였으나 실패.

+ 게임에 들어간 모든 음악은 오픈소스 음악임.
오픈소스 음악을 얻은 사이트) https://www.youtube.com/audiolibrary/music
_______________________________________________________________________________________________________

최종발표 이후의 잡오류 및 버그 수정

1) 음악의 절대경로 설정을 상대경로로 바꿈.

2) 게임오버시 ESC를 누르고 나갈 때 발생하는 오류 해결.

_______________________________________________________________________________________________________

중앙대학교 컴퓨터공학과 컴퓨터공학전공 1학년 방성원
*/




#include <windows.h> 
#include <stdio.h>
#include <conio.h>    
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

// 게임 환경 변수
#define MAP_XSIZE 28
#define MAP_YSIZE 22

#define RED 4 // 플레이어의 색깔
#define YELLOW 6 // 별의 색깔
#define SKYBLUE 11 // 배경 색깔
#define WHITE 15 // 문자 색깔
#define MUSIC_1 "Beat_Your_Competition.wav" // 타이틀 음악의 경로
#define MUSIC_2 "Back_on_Track_Latinesque.wav" // 게임 배경음의 경로
#define MUSIC_3 "Nevada_City.wav" // 게임오버시 나오는 노래_1의 경로
#define MUSIC_4 "Space_Craft_Passing_Pulse_Stutter.wav" // 로딩중 나오는 음향효과의 경로
#define MUSIC_5 "In_Albany_New_York.wav" // 게임오버시 나오는 노래_2의 경로
#define MUSIC_6 "Free_Dog.wav" // 게임오버시 나오는 노래_3의 경로
#define MUSIC_7 "Grassy_Hill.wav" // 게임오버시 나오는 노래_4의 경로


#define MAX_STAR 80 // 별의 최대 개수.
#define LEVEL_COUNT 3   // 레벨마다 늘어날 별의 개수
#define LEVEL_SPEED 10  // 레벨마다 빨라질 스피드 (1000 = 1초, 15 = 0.015초)

#define ENTER 13 // 엔터
#define ESC 27 // 게임종료 
#define LEFT_MOVE 75
#define RIGHT_MOVE 77

int life_point = 3; // 캐릭터의 생명
int fallen_star = 0; // 피한 별의 총 개수

					 // 별 구조체.
struct STAR
{
	int x, y, wait;
};
// 화면의 특정 위치로 이동해 주는 함수.
void gotoxy(int x, int y)
{
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
// 게임 함수.

void player_color(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}

void star_color(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}

void background_color(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}

void text_color(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}

void removeCursor(void) // 커서를 안보이게한다
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

int game(int level)
{
	PlaySound(TEXT(MUSIC_2), NULL, SND_ASYNC | SND_NOSTOP | SND_LOOP);
	struct STAR star[MAX_STAR], user;  //별들과 유저가 있는 구조체
	int i, key, check = 1, avoid = 0;
	int speed = 150 - level * LEVEL_SPEED; // 속도
	int count = 5 + level * LEVEL_COUNT; // for문에서 별이 count만큼 떨어짐.
	int delay = 40; //비프음 딜레이
					// 레벨업에 필요한 별의 개수
	int levelup = 20 + level * 20;
	// 별의 개수 체크. (최대값을 넘지 않도록 조정)
	if (count >= MAX_STAR) count = MAX_STAR - 1;
	// 스피드 체크. (최소값이 10 보다 작지 않도록 저정)
	if (speed < 10) speed = 10;
	// 유저 시작 위치.
	user.x = 14;
	user.y = 21;
	// 별 초기화.
	for (i = 0; i<count; i++)
	{
		star[i].x = rand() % 27 + 1;
		star[i].y = 3;
		star[i].wait = rand() % 20;
	}

	// 게임 루프
	while (check)
	{
		// 스피드에 따른 시연 지연. 상수는 난이도 보정용.
		Sleep(speed);

		// 화면 지우기.
		system("cls");

		// 게임 환경 출력.
		background_color(SKYBLUE);
		gotoxy(32, 2);
		printf("[☆AVOID_STAR☆]");

		//바닥 출력
		for (int block = 1; block <= MAP_XSIZE; block++)
		{
			gotoxy(block, MAP_YSIZE); printf("▦");
			block++;
		}

		// 사람 움직임 처리
		while (kbhit())
		{
			key = getch();
			if (key == 224)
			{
				do
				{
					key = getch();
				} while (key == 224);
				switch (key)
				{
				case LEFT_MOVE:
					if (user.x > 1) user.x--; break;   // 왼쪽으로 이동.
				case RIGHT_MOVE:
					if (user.x <= 26) user.x++; break; // 오른쪽으로 이동.
				}
			}
			else
			{
				switch (key)
				{
				case ESC:
					PlaySound(NULL, 0, 0);
					system("cls"); //화면을 지운다.
					background_color(SKYBLUE);
					gotoxy(6, 6); printf("플레이해주셔서 감사합니다.");
					puts("\n\n\n\n\n\n");
					Sleep(2000);
					exit(0); //게임종료 
				}
			}
		}
		// 사람 출력
		gotoxy(user.x, user.y);
		player_color(RED);
		printf("♥");
		// 현재의 게임 상황을 보여주는 창

		text_color(WHITE);
		gotoxy(32, 4); printf("당신이 피해왔던 모든    :%4d", fallen_star);
		star_color(YELLOW);
		gotoxy(53, 4); printf("★");
		text_color(WHITE);
		gotoxy(32, 6); printf("당신이 한 라운드에서 피한    :%4d", avoid);
		star_color(YELLOW);
		gotoxy(58, 6); printf("★");
		text_color(WHITE);
		gotoxy(32, 8); printf("레벨업까지 남은    :%4d", levelup - avoid);
		star_color(YELLOW);
		gotoxy(48, 8); printf("★");
		text_color(WHITE);
		gotoxy(32, 10); printf("현재 레벨 : %d", level);
		gotoxy(32, 12); printf("남은    : %d", life_point);
		player_color(RED);
		gotoxy(37, 12); printf("♥");
		// 별의 움직임 처리
		for (i = 0; i<count; i++)
		{
			if (star[i].x >= 2)
			{
				if (star[i].wait > 0)
				{
					// 대기시간 동안 아직 별을 떨어뜨리지 않는다.
					star[i].wait--;
				}
				else
				{
					// 별을 한칸 아래로 이동한다.
					star[i].y++;
					// 별이 최하단에 도착 했을 때 처리.
					if (star[i].y >= 22)
					{
						star[i].x = rand() % 27 + 1;
						star[i].y = 3;
						avoid++;    // 피한 별의 개수 카운트
						fallen_star += 1; //피한 총 별의 개수 카운트
					}
					// 별 출력
					gotoxy(star[i].x, star[i].y);
					star_color(YELLOW);
					printf("★");
					// 충돌 체크
					if (abs(star[i].x - user.x) < 2 && star[i].y == user.y)
					{
						// 비프음.
						Beep(380, delay);
						life_point--;
						if (life_point == 0)
						{
							check = 0;
							break;
						}
					}
				}
			}
		}
		// 피한 별의 개수가 levelup보다 높으면 레벨업.
		if (avoid > levelup)
		{
			fallen_star -= 1; //오차 보정
			PlaySound(NULL, 0, 0);
			Sleep(500);
			return 1;
		}
	}
	PlaySound(NULL, 0, 0);
	Sleep(500);
	return 0;
}

// 메인 함수
void main()
{
	removeCursor();
	PlaySound(TEXT(MUSIC_1), NULL, SND_ASYNC | SND_NOSTOP | SND_LOOP);
	int level = 1;
	int key, loop;
	int x = 5, y = 3;

	gotoxy(x, y + 0); printf("************************************************"); Sleep(100);
	gotoxy(x, y + 1); printf("*■□□□■■■□□■■□□■■■■□□■■■□*"); Sleep(100);
	gotoxy(x, y + 2); printf("*■■■□  ■□□  ■■□□      □□  ■□■■*"); Sleep(100);
	gotoxy(x, y + 3); printf("*□□□                          !   ■□■■■*"); Sleep(100);
	gotoxy(x, y + 4); printf("*■■□■■      ■■■■  □  ■□□□  ■□■*"); Sleep(100);
	gotoxy(x, y + 5); printf("*■■  ■□□□□■■■■□□□■■■□■■□□*"); Sleep(100);
	gotoxy(x + 11, y + 3); printf(" A V O I D ☆ S T A R"); Sleep(100);
	gotoxy(x, y + 6); printf("************************************************"); Sleep(100);
	gotoxy(x, y + 8); printf("◁  ▷ : Left / Right");
	gotoxy(x, y + 10); printf("Press Enter Key To Start..");
	gotoxy(x, y + 12); printf("Press Esc If You Want To Quit..");


	for (int cnt = 0;; cnt++) //메인화면에 넣은 별 반짝이는 애니메이션효과 
	{
		if (kbhit()) break; //(kbhit은 keyboard hit의 줄임말같다)키입력이 있으면 무한루프 종료 
		if (cnt % 200 == 0) { gotoxy(x + 7, y + 3); printf("★"); }//cnt가 200으로 나누어 떨어질때 별을 표시      
		if ((cnt % 200 - 100) == 0) { gotoxy(x + 7, y + 3); printf("  "); } //위 카운트에서 100카운트 간격으로 별을 지움 
		if ((cnt % 350) == 0) { gotoxy(x + 35, y + 3); printf("☆"); } //윗별과 같지만 시간차를 뒀음 
		if ((cnt % 350 - 100) == 0) { gotoxy(x + 35, y + 3); printf("  "); }
		Sleep(10); // 00.1초 딜레이  
	}

	while (1)
	{
		if (kbhit())
		{
			key = getch();
			if (key == ENTER)
			{
				Sleep(500);
				PlaySound(NULL, 0, 0);
				break;
			}
			else if (key == ESC)
			{
				PlaySound(NULL, 0, 0);
				system("cls");
				background_color(SKYBLUE);
				gotoxy(x + 1, y + 1); printf("플레이해주셔서 감사합니다.");
				puts("\n\n\n\n\n\n");
				Sleep(2000);
				exit(0); //게임종료 
			}
		}
	}
loop:
	while (1)
	{
		PlaySound(TEXT(MUSIC_4), NULL, SND_ASYNC | SND_NOSTOP);
		// 화면 지우기.
		system("cls");
		// 로딩창.
		star_color(YELLOW);
		int x_1, x_2, y_1 = 1, y_2 = 2;
		for (x_1 = 16; x_1 > 0; x_1--)
		{
			gotoxy(x_1, y_1); printf("★");
			y_1++;
		}
		for (x_1 = 1; x_1 < 17; x_1++)
		{
			gotoxy(x_1, y_1); printf("★");
			y_1++;
		}
		for (x_2 = 17; x_2 < 32; x_2++)
		{
			gotoxy(x_2, y_2); printf("★");
			y_2++;
		}
		for (x_2 = 31; x_2 > 16; x_2--)
		{
			gotoxy(x_2, y_2); printf("★");
			y_2++;
		}
		background_color(SKYBLUE);
		gotoxy(9, 16); printf("[☆AVOID_STAR★]");
		gotoxy(14, 18); printf("LEVEL %d", level);
		gotoxy(11, 21); printf("게임 준비중..");
		// 시간 지연 (1초 = 1000)
		Sleep(5000);

		// 게임 시작.
		if (game(level++) == 0)
		{
			system("cls");
			x = 5;
			y = 5;
			gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
			gotoxy(x, y + 1); printf("▤                              ▤");
			gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
			gotoxy(x, y + 3); printf("▤  |  G A M E  O V E R..   |   ▤");
			gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
			gotoxy(x, y + 5); printf("▤       Your Level = %2d        ▤", level - 1);
			gotoxy(x, y + 6); printf("▤   Your Best Score = %4d     ▤", fallen_star);
			gotoxy(x, y + 7); printf("▤                              ▤");
			gotoxy(x, y + 8); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

			if (fallen_star < 300)
			{
				PlaySound(TEXT(MUSIC_3), NULL, SND_ASYNC | SND_NOSTOP | SND_LOOP);
				gotoxy(x + 4, y + 10);  printf("<<Hmm.. You Are Beginner>>");
			}

			else if (fallen_star >= 300 && fallen_star < 600)
			{
				PlaySound(TEXT(MUSIC_5), NULL, SND_ASYNC | SND_NOSTOP | SND_LOOP);
				gotoxy(x + 4, y + 10);  printf("<<Not Bad. You Are Normal>>");
			}

			else if (fallen_star >= 600 && fallen_star < 900)
			{
				PlaySound(TEXT(MUSIC_6), NULL, SND_ASYNC | SND_NOSTOP | SND_LOOP);
				gotoxy(x + 4, y + 10); printf("<<Great! You Are Veteran!>>");
			}

			else
			{
				PlaySound(TEXT(MUSIC_7), NULL, SND_ASYNC | SND_NOSTOP | SND_LOOP);
				gotoxy(x + 4, y + 10); printf("<<Marvelous! You Are Expert!>>");
			}
			gotoxy(x, y + 12); printf("Press Enter Key To Restart");
			gotoxy(x, y + 14); printf("ESC == EXIT");
			while (1)
			{
				if (kbhit())
				{
					key = getch();
					if (key == ENTER)
					{
						fallen_star = 0;
						level = 1;
						life_point = 3;
						PlaySound(NULL, 0, 0);
						Sleep(1000);
						goto loop;
					}
					else if (key == ESC)
					{
						system("cls");
						background_color(SKYBLUE);
						PlaySound(NULL, 0, 0);
						gotoxy(x + 1, y + 1); printf("플레이해주셔서 감사합니다.");
						Sleep(2000);
						puts("\n\n\n\n\n\n");
						break;
					}
				}
			}
			break;
		}
	}
}