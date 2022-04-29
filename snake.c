#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#include <conio.h> //kbhit(): 사용자가 키보드의 임의의 키를 눌렀는지 확인한다.
#include<string.h>
#include<time.h>

int character[1000];
int length = 2;
int fruit;
int score;
clock_t t;
int s_e;
int vx;
int vy;
int speed = 5;

int Random(int a, int b)
{
	srand(time(NULL));
	return a + rand() % (b - a + 1);
}
//화면씩 변수 이름을 넣어 된다.
enum state {
	MENU = 0, INGAME, SETTING, INFORMATION, GAMEOVER, EXIT
} state;

void gotoxy(int x, int y)
{
	static HANDLE  h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x,y };
	SetConsoleCursorPosition(h, c);
}

void information() {
	system("cls");
	gotoxy(30, 10);
	printf("뱀 게임입니다~~");
	gotoxy(30, 11);
	printf("웬낫휘_2105026");
	getch();

	state = MENU;
}
void setting() {
	system("cls");
	gotoxy(20, 9);
	printf("왼쪽(<) 또는 오른쪽(>)을 눌러 속도 조절");
	gotoxy(20, 13);
	printf("Enter 키를 눌러 메뉴로 돌아가기~");
	gotoxy(30, 11);
	printf("현재 속도 : %d", speed);
	int key;
	while (state == SETTING) {
		//kbhit():사용자가 키보드의 임의의 키를 눌렀는지 확인한다.
		if (kbhit()) {	
			key = getch();
			switch (key) {
			//왼쪽 키 입력할 때
			case 75:
				if (speed > 1) {

					speed -= 1;
					gotoxy(30, 11);
					printf("현재 속도 : %d   ", speed);
				}
				break;

			//오른쪽 키 입력할 때
			case  77:
				if (speed < 10) {
					speed += 1;
					gotoxy(30, 11);
					printf("현재 속도 : %d   ", speed);
				}
				break;
			////Enter 입력할 때
			case 13:	
				state = MENU;
				break;
			}
		}
	}
}
void drawBox() {
	//첫 번째 행에는 # 표시가 인쇄된다.
	for (int i = 0; i < 79; i++) {
		printf("+");
	}
	printf("\n");
	
	for (int i = 0; i < 23; i++) {
		//첫 번째 열과 마지막 열에는 # 표시가 인쇄된다.
		for (int j = 0; j < 79; j++) {
			if (j == 0) {
				printf("+");
			}
			else if(j == 78) {
				printf("+\n"); 
			}
			else {
				printf(" ");
			}
		}
	}
	//마지막 행에는 # 표시가 인쇄된다.
	for (int i = 0; i < 79; i++) {
		printf("+");
	}

}

void gameover() {
	system("cls");
	gotoxy(30, 9);
	printf("....GAME OVER....");
	gotoxy(29, 11);
	printf("\t 점수: [%d]", score);
	gotoxy(29, 13);
	printf("아무 키나 눌러 메뉴로 돌아가기~");
	getch();
	state = MENU;
}

void menu() {
	int choice;
	system("cls");
	gotoxy(28, 3);
	printf("---SNAKE GAME---");
	gotoxy(30, 5);
	printf("1. 시작");
	gotoxy(30, 6);
	printf("2. 속도");
	gotoxy(30, 7);
	printf("3. 정보");
	gotoxy(30, 8);
	printf("4. 종료");
	gotoxy(30, 9);
	printf("메뉴 선택 : ");
	scanf("%d", &choice);
	//사용자의 선택에 따라 화면을 제시하다
	switch (choice) {
	case 1:
		state = INGAME;
		break;
	case 2:
		state = SETTING;
		break;
	case 3:
		state = INFORMATION;
		break;
	case 4:
		state = EXIT;
		break;
	}
	
}
	

void initgame() {
	system("cls");
	//속도 초기화
	s_e = 120 - speed*10;
	t = 0;
	vx = 1;
	vy = 0;
	score = 0;
	length = 6;
	//뱀과 먹이를 위한 초기 위치 초기화
	character[0] = 11510;
	character[1] = 11410;
	character[2] = 11310;
	character[3] = 11210;
	character[4] = 11110;
	character[5] = 11010;
	fruit = 15010;
}

void creatfruit() {
	int no_stop = 1;
	while (no_stop) {
		no_stop = 0;
		//먹이는 x = 1 ~ 76 및 y = 1 ~ 22 사이에 나타난다.
		fruit = 10000 + Random(1, 77) * 100 + Random(1, 23); 
		//실행할 때 동안 뱀이 먹이를 먹었으면 새 먹이를 재설정하고 그렇지 않으면 반복한다
		for (int i = 0; i < length; i++) {	
			if (character[i] == fruit) {
				no_stop = 1;
			}
		}
	}
}
//네 자리 숫자 중 첫 두 자리는 X이고 다음 두 자리는 Y이다.
int getx(int a) {			
	return (a - 10000) / 100;
}
int gety(int a) {
	return a % 100;
}
void drawscore() {
	gotoxy(1, 25);
	printf("SCORE : %d", score);
	gotoxy(32, 25);
	printf("EXIT : [esc]");
	gotoxy(60, 25);
	printf("github.com/bin0108");
}


void ingame() {
	int key;
	int i;
	initgame();
	drawBox();
	drawscore();
	state = INGAME;
	//먹이는 O 표시가 인쇄된다.
	gotoxy(getx(fruit), gety(fruit));
	printf("O");
	//뱀의 머리는 # 표시가 인쇄된다.
	gotoxy(getx(character[0]), gety(character[0]));
	printf("#");
	
	//뱀의 몸통은 o 표시가 인쇄된다.
	for (int i = 0; i < length; i++) {
		gotoxy(getx(character[i]), gety(character[i]));
		printf("o");
	}
	while (state == INGAME) {
		if (kbhit()) {
			key = getch();
			switch (key) {
			//위 버튼 눌렸을 때
			case 72:
				if (vy == 0) { //뱀이 반대쪽으로 뒤집히는 것을 피하기 위해
					vx = 0;
					vy = -1;
				}
				break;
			//아래 버튼 눌렸을 때
			case 80:
				if (vy == 0) {
					vx = 0;
					vy = 1;
				}
				break;
			//왼 버튼 눌렸을 때
			case 75:
				if (vx == 0) {
					vy = 0;
					vx = -1;
				}
				break;
			//오른 버튼 눌렸을 때
			case 77:
				if (vx == 0) {
					vy = 0;
					vx = 1;
				}
				break;
			//esc 버튼 눌렸을 때
			case 27:
				system("cls");
				state = MENU;
			}
		}
		
		//수행시간 측정하고 속도를 조절한다. 
		if ((clock() - t) >= s_e) {
			// 뱀의 끝자리가 공백으로 변할 것이다
			if (character[length - 1] != 0) {
				gotoxy(getx(character[length - 1]), gety(character[length - 1]));
				printf(" ");	
			}
			//뒤에 있는 위치가 이전 위치에 넣어 된다.
			i = length - 1;
			for (i; i > 0; i--) {
				character[i] = character[i - 1];
			}
			//첫 위치는 o 표시가 인쇄된다.
			gotoxy(getx(character[0]), gety(character[0]));
			printf("o");

			character[0] += vx * 100;
			character[0] += vy;
			if (character[0] == fruit) { //뱀이 먹이를 먹었을 때
				length += 1; //뱀의 길이가 1 추가된다.
				score += speed;
				drawscore();
				creatfruit();
				gotoxy(getx(fruit), gety(fruit));
				printf("O");
			}
			//벽으로 가면 반대 위치로 돌아간다
			if ((vx > 0) && (getx(character[0]) == 78)) {
				character[0] -= 7700;
			}
			if ((vx < 0) && (getx(character[0]) == 0)) {
				character[0] += 7700;
			}
			if ((vy > 0) && (gety(character[0]) == 24)) {
				character[0] -= 23;
			}
			if ((vy < 0) && (gety(character[0]) == 0)) {
				character[0] += 23;
			}
			gotoxy(getx(character[0]), gety(character[0]));
			printf("#");

			//뱀의 머리가 몸에 닿으면 게임이 끝난다
			i = 1;
			for (i; i < length; i++) {
				if (character[i] == character[0]) {
					system("cls");
					state = GAMEOVER;
				}
			}
			t = clock();
		}

	}
}

void main() {
	state = MENU;
	menu();

	while (1) {
		switch (state) {
		case MENU:
			menu();
			break;
		case INGAME:
			ingame();
			break;
		case GAMEOVER:
			gameover();
			break;
		case INFORMATION:
			information();
			break;
		case  SETTING:
			setting();
			break;
		case EXIT:
			return 0;
		}
	}
}
