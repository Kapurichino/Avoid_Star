/*
2017 / 08 / 23, 20 : 30�� �������� ��.
(1) 08 / 11 ��Ʈ���� ���ϱ� -> �� ���ϱ� ����
__����__
1.��Ʈ���� AI�� C���� �����ϴµ� ������� ����

2.���� ������ ��Ʈ������ �ٸ��� �������� ���� ����Ʈ���� �ڵ��ϼ� ��ų ����� �� ã��.

(2) 08 / 19 ���� ���� pause�� �ϰ� ����� �;����� �Ұ���.

(3) ���� �浹�� �浹 ���带 ������ �Ͽ����� bgm���� �÷����ϰ� �ִ� �뷡�� �浹�� ������ �� ����.
+SND_NOSTOP���� �ذ��Ϸ� �Ͽ����� ����.

+ ���ӿ� �� ��� ������ ���¼ҽ� ������.
���¼ҽ� ������ ���� ����Ʈ) https://www.youtube.com/audiolibrary/music
_______________________________________________________________________________________________________

������ǥ ������ ����� �� ���� ����

1) ������ ������ ������ ����η� �ٲ�.

2) ���ӿ����� ESC�� ������ ���� �� �߻��ϴ� ���� �ذ�.

_______________________________________________________________________________________________________

�߾Ӵ��б� ��ǻ�Ͱ��а� ��ǻ�Ͱ������� 1�г� �漺��
*/




#include <windows.h> 
#include <stdio.h>
#include <conio.h>    
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

// ���� ȯ�� ����
#define MAP_XSIZE 28
#define MAP_YSIZE 22

#define RED 4 // �÷��̾��� ����
#define YELLOW 6 // ���� ����
#define SKYBLUE 11 // ��� ����
#define WHITE 15 // ���� ����
#define MUSIC_1 "Beat_Your_Competition.wav" // Ÿ��Ʋ ������ ���
#define MUSIC_2 "Back_on_Track_Latinesque.wav" // ���� ������� ���
#define MUSIC_3 "Nevada_City.wav" // ���ӿ����� ������ �뷡_1�� ���
#define MUSIC_4 "Space_Craft_Passing_Pulse_Stutter.wav" // �ε��� ������ ����ȿ���� ���
#define MUSIC_5 "In_Albany_New_York.wav" // ���ӿ����� ������ �뷡_2�� ���
#define MUSIC_6 "Free_Dog.wav" // ���ӿ����� ������ �뷡_3�� ���
#define MUSIC_7 "Grassy_Hill.wav" // ���ӿ����� ������ �뷡_4�� ���


#define MAX_STAR 80 // ���� �ִ� ����.
#define LEVEL_COUNT 3   // �������� �þ ���� ����
#define LEVEL_SPEED 10  // �������� ������ ���ǵ� (1000 = 1��, 15 = 0.015��)

#define ENTER 13 // ����
#define ESC 27 // �������� 
#define LEFT_MOVE 75
#define RIGHT_MOVE 77

int life_point = 3; // ĳ������ ����
int fallen_star = 0; // ���� ���� �� ����

					 // �� ����ü.
struct STAR
{
	int x, y, wait;
};
// ȭ���� Ư�� ��ġ�� �̵��� �ִ� �Լ�.
void gotoxy(int x, int y)
{
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
// ���� �Լ�.

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

void removeCursor(void) // Ŀ���� �Ⱥ��̰��Ѵ�
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

int game(int level)
{
	PlaySound(TEXT(MUSIC_2), NULL, SND_ASYNC | SND_NOSTOP | SND_LOOP);
	struct STAR star[MAX_STAR], user;  //����� ������ �ִ� ����ü
	int i, key, check = 1, avoid = 0;
	int speed = 150 - level * LEVEL_SPEED; // �ӵ�
	int count = 5 + level * LEVEL_COUNT; // for������ ���� count��ŭ ������.
	int delay = 40; //������ ������
					// �������� �ʿ��� ���� ����
	int levelup = 20 + level * 20;
	// ���� ���� üũ. (�ִ밪�� ���� �ʵ��� ����)
	if (count >= MAX_STAR) count = MAX_STAR - 1;
	// ���ǵ� üũ. (�ּҰ��� 10 ���� ���� �ʵ��� ����)
	if (speed < 10) speed = 10;
	// ���� ���� ��ġ.
	user.x = 14;
	user.y = 21;
	// �� �ʱ�ȭ.
	for (i = 0; i<count; i++)
	{
		star[i].x = rand() % 27 + 1;
		star[i].y = 3;
		star[i].wait = rand() % 20;
	}

	// ���� ����
	while (check)
	{
		// ���ǵ忡 ���� �ÿ� ����. ����� ���̵� ������.
		Sleep(speed);

		// ȭ�� �����.
		system("cls");

		// ���� ȯ�� ���.
		background_color(SKYBLUE);
		gotoxy(32, 2);
		printf("[��AVOID_STAR��]");

		//�ٴ� ���
		for (int block = 1; block <= MAP_XSIZE; block++)
		{
			gotoxy(block, MAP_YSIZE); printf("��");
			block++;
		}

		// ��� ������ ó��
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
					if (user.x > 1) user.x--; break;   // �������� �̵�.
				case RIGHT_MOVE:
					if (user.x <= 26) user.x++; break; // ���������� �̵�.
				}
			}
			else
			{
				switch (key)
				{
				case ESC:
					PlaySound(NULL, 0, 0);
					system("cls"); //ȭ���� �����.
					background_color(SKYBLUE);
					gotoxy(6, 6); printf("�÷������ּż� �����մϴ�.");
					puts("\n\n\n\n\n\n");
					Sleep(2000);
					exit(0); //�������� 
				}
			}
		}
		// ��� ���
		gotoxy(user.x, user.y);
		player_color(RED);
		printf("��");
		// ������ ���� ��Ȳ�� �����ִ� â

		text_color(WHITE);
		gotoxy(32, 4); printf("����� ���ؿԴ� ���    :%4d", fallen_star);
		star_color(YELLOW);
		gotoxy(53, 4); printf("��");
		text_color(WHITE);
		gotoxy(32, 6); printf("����� �� ���忡�� ����    :%4d", avoid);
		star_color(YELLOW);
		gotoxy(58, 6); printf("��");
		text_color(WHITE);
		gotoxy(32, 8); printf("���������� ����    :%4d", levelup - avoid);
		star_color(YELLOW);
		gotoxy(48, 8); printf("��");
		text_color(WHITE);
		gotoxy(32, 10); printf("���� ���� : %d", level);
		gotoxy(32, 12); printf("����    : %d", life_point);
		player_color(RED);
		gotoxy(37, 12); printf("��");
		// ���� ������ ó��
		for (i = 0; i<count; i++)
		{
			if (star[i].x >= 2)
			{
				if (star[i].wait > 0)
				{
					// ���ð� ���� ���� ���� ����߸��� �ʴ´�.
					star[i].wait--;
				}
				else
				{
					// ���� ��ĭ �Ʒ��� �̵��Ѵ�.
					star[i].y++;
					// ���� ���ϴܿ� ���� ���� �� ó��.
					if (star[i].y >= 22)
					{
						star[i].x = rand() % 27 + 1;
						star[i].y = 3;
						avoid++;    // ���� ���� ���� ī��Ʈ
						fallen_star += 1; //���� �� ���� ���� ī��Ʈ
					}
					// �� ���
					gotoxy(star[i].x, star[i].y);
					star_color(YELLOW);
					printf("��");
					// �浹 üũ
					if (abs(star[i].x - user.x) < 2 && star[i].y == user.y)
					{
						// ������.
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
		// ���� ���� ������ levelup���� ������ ������.
		if (avoid > levelup)
		{
			fallen_star -= 1; //���� ����
			PlaySound(NULL, 0, 0);
			Sleep(500);
			return 1;
		}
	}
	PlaySound(NULL, 0, 0);
	Sleep(500);
	return 0;
}

// ���� �Լ�
void main()
{
	removeCursor();
	PlaySound(TEXT(MUSIC_1), NULL, SND_ASYNC | SND_NOSTOP | SND_LOOP);
	int level = 1;
	int key, loop;
	int x = 5, y = 3;

	gotoxy(x, y + 0); printf("************************************************"); Sleep(100);
	gotoxy(x, y + 1); printf("*������������������������*"); Sleep(100);
	gotoxy(x, y + 2); printf("*�����  ����  �����      ���  �����*"); Sleep(100);
	gotoxy(x, y + 3); printf("*����                          !   ������*"); Sleep(100);
	gotoxy(x, y + 4); printf("*������      �����  ��  �����  ����*"); Sleep(100);
	gotoxy(x, y + 5); printf("*���  ���������������������*"); Sleep(100);
	gotoxy(x + 11, y + 3); printf(" A V O I D �� S T A R"); Sleep(100);
	gotoxy(x, y + 6); printf("************************************************"); Sleep(100);
	gotoxy(x, y + 8); printf("��  �� : Left / Right");
	gotoxy(x, y + 10); printf("Press Enter Key To Start..");
	gotoxy(x, y + 12); printf("Press Esc If You Want To Quit..");


	for (int cnt = 0;; cnt++) //����ȭ�鿡 ���� �� ��¦�̴� �ִϸ��̼�ȿ�� 
	{
		if (kbhit()) break; //(kbhit�� keyboard hit�� ���Ӹ�����)Ű�Է��� ������ ���ѷ��� ���� 
		if (cnt % 200 == 0) { gotoxy(x + 7, y + 3); printf("��"); }//cnt�� 200���� ������ �������� ���� ǥ��      
		if ((cnt % 200 - 100) == 0) { gotoxy(x + 7, y + 3); printf("  "); } //�� ī��Ʈ���� 100ī��Ʈ �������� ���� ���� 
		if ((cnt % 350) == 0) { gotoxy(x + 35, y + 3); printf("��"); } //������ ������ �ð����� ���� 
		if ((cnt % 350 - 100) == 0) { gotoxy(x + 35, y + 3); printf("  "); }
		Sleep(10); // 00.1�� ������  
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
				gotoxy(x + 1, y + 1); printf("�÷������ּż� �����մϴ�.");
				puts("\n\n\n\n\n\n");
				Sleep(2000);
				exit(0); //�������� 
			}
		}
	}
loop:
	while (1)
	{
		PlaySound(TEXT(MUSIC_4), NULL, SND_ASYNC | SND_NOSTOP);
		// ȭ�� �����.
		system("cls");
		// �ε�â.
		star_color(YELLOW);
		int x_1, x_2, y_1 = 1, y_2 = 2;
		for (x_1 = 16; x_1 > 0; x_1--)
		{
			gotoxy(x_1, y_1); printf("��");
			y_1++;
		}
		for (x_1 = 1; x_1 < 17; x_1++)
		{
			gotoxy(x_1, y_1); printf("��");
			y_1++;
		}
		for (x_2 = 17; x_2 < 32; x_2++)
		{
			gotoxy(x_2, y_2); printf("��");
			y_2++;
		}
		for (x_2 = 31; x_2 > 16; x_2--)
		{
			gotoxy(x_2, y_2); printf("��");
			y_2++;
		}
		background_color(SKYBLUE);
		gotoxy(9, 16); printf("[��AVOID_STAR��]");
		gotoxy(14, 18); printf("LEVEL %d", level);
		gotoxy(11, 21); printf("���� �غ���..");
		// �ð� ���� (1�� = 1000)
		Sleep(5000);

		// ���� ����.
		if (game(level++) == 0)
		{
			system("cls");
			x = 5;
			y = 5;
			gotoxy(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
			gotoxy(x, y + 1); printf("��                              ��");
			gotoxy(x, y + 2); printf("��  +-----------------------+   ��");
			gotoxy(x, y + 3); printf("��  |  G A M E  O V E R..   |   ��");
			gotoxy(x, y + 4); printf("��  +-----------------------+   ��");
			gotoxy(x, y + 5); printf("��       Your Level = %2d        ��", level - 1);
			gotoxy(x, y + 6); printf("��   Your Best Score = %4d     ��", fallen_star);
			gotoxy(x, y + 7); printf("��                              ��");
			gotoxy(x, y + 8); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");

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
						gotoxy(x + 1, y + 1); printf("�÷������ּż� �����մϴ�.");
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