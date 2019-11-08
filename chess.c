#include <stdio.h> //printf(). scanf(), fopen(), fscanf(), fprintf() 등 실행에 주가 되는 함수를 위함
#include <string.h> //체스판과 데이터를 저장하고 변경하는 과정에 있어서 필요한 strcpy(), strcat(), strcmp(), strlen()를 위함
#include <stdlib.h> //system("clear")를 위함
#include <termio.h> // getch() 함수를 정의하기 위해 필요한 tcgetattr(), ICANON, ECHO, VMIN, VTIME, tcsetattr(), TCSAFLUSH를 위함


char pan[17][34]; //실행중인 체스판
char backpan[17][34]; //되돌리기 위한 백그라운드 저장용 체스판
char checkbackpan[17][34]; //check여부 확인 시 사용할 체스판
char (*pStr)[34]; //현재 맵 변경시 사용 (pan[0][0]을 포인트한다)
char PAN[100] = "C:\\Users\\SoHyun Kim\\Desktop\\chess.txt"; // 새 게임용 체스판
char SPAN[100] = "C:\\Users\\SoHyun Kim\\Desktop\\chess_save.txt"; // 저장된 체스판
static int startplayer; //시작하는 player 확인용
static int end; // 승리한 player 확인용 (1 : player 1 승, 2 : player 2 승, 3 : 강제종료)
static int F=0; // 선택한 말이 움직일 수 있는 위치가 없을 때 F=0 -> 다시 말 선택
static int kx1=18, ky1=1; //player 1의 King 좌표 (check여부 확인시 이용)
static int kx2=18, ky2=15; //player 2의 King 좌표 (check여부 확인시 이용)
static int kx3=-1, ky3=-1; //King을 이동시킬 때 check 확인 좌표
void clear() { system("clear"); } // clear함수 (화면을 깨끗하게 함)
char die1[20]={""}, die2[20]={""}; // 죽은 말 표시


void Load(int); // 게임을 시작하기 전 필요한 과정
void Print(void); //현재 판 출력
void Backpan(void); //되돌리기용 판 저장
void Checkbackpan(void); //되돌리기용 판 저장
void Returncheckpan(void); //판 되돌리기
void Rule(void); //도움말 출력
void Save(void); //저장
void Delete_s(void); // *표시 원래(. 또는 말)로 돌리기
void Move1(void); //Player1 이동
void Move2(void); //Player2 이동
void Pawn(int, int); //P 선택시 이동 가능 위치 *로 표시
void Rook(int, int); //R 선택시 이동 가능 위치 *로 표시
void Knight(int, int); //N 선택시 이동 가능 위치 *로 표시
void Bishop(int, int); //B 선택시 이동 가능 위치 *로 표시
void Queen(int, int); //Q 선택시 이동 가능 위치 *로 표시
void King(int, int); //K 선택시 이동 가능 위치 *로 표시
int Check(int, int); //check 여부 확인
int Checkmate(int); //종료 조건
int getch() {
    int ch;
    struct termios buf, save;

    tcgetattr(0, &save); // 현재 터미널 설정 읽음
    buf = save;
    buf.c_lflag &= ~(ICANON | ECHO); // CANONICAL과 ECHO 끔
    buf.c_cc[VMIN] = 1; // 최소 입력 문자 수를 1로 설정
    buf.c_cc[VTIME] = 0; // 최소 읽기 대기 시간을 0으로 설정
    tcsetattr(0, TCSAFLUSH, &buf); // 터미널에 설정 입력
    ch = getchar(); // 키보드 입력 읽음
    tcsetattr(0, TCSAFLUSH, &save); // 원래의 설정으로 복구
    return ch;
}


int main() {
    int a=0; // 메뉴 선택용 변수

    while (1) {
        clear(); //화면정리
        if (end == 1 || end == 2) {printf("\nPlayer %d WIN!\n\n",end); end=0;} //이전 판 승리자 표시 (처음 게임 실행 시 프린트 되지 않음)
        printf("+---- MENU ----+\n|  1.New game  |\n|  2.Load game |\n|    3.Help    |\n|    4.Exit    |\n+--------------+\n");
        printf("Meun Number : ");
        scanf("%d", &a); clear(); // 메뉴 선택
        switch (a) {
            case 1: { // New game
                Load(1); Print(); //새 게임 판을 로드하여 프린트
                while (1) {
                    if (startplayer == 1) {
                        Move1(); //player 1 이동

                        if(end==1||end==2) { //만약 king이 잡히거나 checkmate상태가 되면 게임 종료
                            break;
                        }
                        else if (end==3) {Save(); break;} //사용자가 exit 입력시 자동 저장 후 게임 종료

                        clear(); Print(); //이동 후 변경된 체스판을 정리된 화면에 프린트
                        startplayer = 2;
                    }

                    if ( startplayer == 2 ){
                        Move2(); //player 2 이동
                        if (end == 1 || end == 2) { //만약 king이 잡히거나 checkmate상태가 되면 게임 종료
                            break;
                        } else if (end == 3) {
                            Save();
                            break;
                        } //사용자가 exit 입력시 자동 저장 후 게임 종료

                        clear(); Print(); //이동 후 변경된 체스판을 정리된 화면에 프린트
                        startplayer = 1;
                    }

                }break;
            }
            case 2: { //Load
                Load(2); Print(); //새 게임 판을 로드하여 프린트
                while (1) {
                    if (startplayer == 1) {
                        Move1(); //player 1 이동

                        if(end==1||end==2) { //만약 king이 잡히거나 checkmate상태가 되면 게임 종료
                            break;
                        }
                        else if (end==3) {Save(); break;} //사용자가 exit 입력시 자동 저장 후 게임 종료

                        clear(); Print(); //이동 후 변경된 체스판을 정리된 화면에 프린트
                        startplayer = 2;
                    }

                    if ( startplayer == 2 ){
                        Move2(); //player 2 이동
                        if (end == 1 || end == 2) { //만약 king이 잡히거나 checkmate상태가 되면 게임 종료
                            break;
                        } else if (end == 3) {
                            Save();
                            break;
                        } //사용자가 exit 입력시 자동 저장 후 게임 종료

                        clear(); Print(); //이동 후 변경된 체스판을 정리된 화면에 프린트
                        startplayer = 1;
                    }

                }break;
            }
            case 3: { //Help (도움말 출력)
                Rule();
                break;
            }
            default:
                break;
        }

        if (a==4) { //게임 종료
            printf("Thank you");
            break; //while문 종료
        }

    }
    return 0;
}

void Load(int k) { // 게임을 시작하기 전 필요한 과정 (새 게임 실행 시 k=1, 저장된 게임 실행 시 k=2)
    FILE *fp = NULL; // 체스판이 저장된 텍스트 파일을 저장할 공간 선언
    char input[50]; // 파일 속 문자열을 읽어서 배열에 저장하기 위한 중간 문자열
    int I=-1; // while문 사용
    if (k==1) fp = fopen(PAN, "r"); // 새 게임용 텍스트파일 열기
    else { //저장된 게임용 텍스트파일 실행
        fp = fopen(SPAN, "r");
        if (fp==NULL) { //만약 저장된 게임(체스판)이 없으면 새 게임 실행
            printf("no savefile. new game start\n");
            fp = fopen(PAN, "r");
        }
    }
    while (fscanf(fp, "%s", input) != EOF) { //파일이 끝날 때까지 한줄 씩 읽어서 input에 저장
        if (I == -1) { strcpy(die2, input); I++; }
        else if ( I>=0 && I < 17) {
            strcpy(pan[I], input); // input에 저장된 문자열 체스판 배열 (pan)에 저장
            I++; //다음 줄을 읽고 저장하기 위함
        }
        else if(I == 17) { strcpy(die1, input); I++; }
        else startplayer = input[0] - 48;
    } fclose(fp); //파일 닫기

    pStr = pan; // 포인터 pStr은 배열 pan의 시작주소를 포인트 함

 }

void Print(void) { // 체스판 출력
    int pnum = 0;
    printf("%s\n", die2); // player1이 잡은 말 출력
    printf("    A   B   C   D   E   F   G   H\n"); // 열 표시
    for (int i=0;i<17;i++) { // 체스판이 저장된 배열pan을 포인트하는 pStr을 이용하여 한줄 씩 출력
        if (i%2==0) printf("  %s\n", *(pStr+i)); // 행 표시
        else { printf("%d %s\n", pnum,*(pStr+i)); pnum++;}
    }
    printf("%s\n", die1); // player2가 잡은 말 출력
}

void Backpan(void) { // 백그라운드 체스판에 현재 판 복사
    for(int i=0;i<17;i++)
        strcpy(backpan[i], pan[i]); // 한줄 씩 pan을 backpan에 복사한다
}

void Checkbackpan(void) { // 체크여부확인 체스판에 현재 판 복사
    for(int i=0;i<17;i++)
        strcpy(checkbackpan[i], pan[i]);
}

void Returncheckpan(void) { // 현재 판에 체크여부확인 체스판 복사
    for(int i=0;i<17;i++)
        strcpy(pan[i], checkbackpan[i]);
}

void Rule(void) { //도움말 출력
    printf("\n<  > :Player 1, [  ] : Player 2\n");
    printf("\nYou can move your pieces to '*'\n");
    printf("\nK can move one square horizontally, vertically, or diagonally.\n"
           "Q can move any number of vacant squares diagonally, horizontally, or vertically.\n"
           "R can move any number of vacant squares vertically or horizontally. It also is moved while castling.\n"
           "B can move any number of vacant squares in any diagonal direction.\n"
           "N can move one square along any rank or file and then at an angle. The knight´s movement can also be viewed as an “L” or “7″ laid out at any horizontal or vertical angle.\n"
           "P can move forward one square, if that square is unoccupied. \n(If it has not yet moved, the pawn can move two squares forward provided both squares in front of the pawn are unoccupied.)\n\n");
    getch(); // 메뉴 출력 후 a입력 받을 때 개행문자 입력받음
    getch(); //사용자가 Enter 누르면 종료
}

void Delete_s(void) { // 이동 후 이동 가능 여부 체크했던 *표시 원래대로 돌려놓음
    char Back; // 돌려놓을 문자 저장용 변수

    for(int i=0;i<17;i++)
        for(int j=0;j<49;j++)
            if (*(*(pStr+i)+j)=='*') { //모든 판을 체크하여 *표시를 backpan에 저장된 원래 문자로 돌려놓음
                Back = backpan[i][j];
                *(*(pStr+i)+j) = Back;
            }
}

void Move1(void) { //player 1 이동함수
    char before[5], after[3]; //이동시킬 말, 위치 입력받을 변수 (save나 exit 입력받아야하는 경우가 있어서 크기: 5)
    int b1=0, b2=0, a1=0, a2=0; //이동시킬 말의 행, 열, 이동시킬 위치의 행, 열
    Backpan();

    //이동시킬 말 입력받아 이동할 수 있는 경로 *로 표시하기
    while(1) {

        if(Checkmate(1) == 0) { end = 2; break;} //만약 player1이 움직일 수 있는 말이 없는 경우 player2 승리, 게임 종료
        else { for(int k=0;k<17;k++) {strcpy(pan[k], backpan[k]);} } //아니면 다시 실행중인 판 복구

        printf("\n< Player 1 >\n(save: save game board, exit: end game)\nWhat? : ");
        scanf("%s", before);
        if (strcmp(before, "save") == 0) { // 사용자가 save입력 시 현재 체스판 텍스트 파일에 저장
            Save();
            printf("S A V E . . . !\n");
            continue;
        }
        else if(strcmp(before, "exit") == 0) { // 사용자가 exit입력 시 게임 종료 후 메뉴로 돌아감
            end += 3;
            break;
        }
        else if (strlen(before) == 2) {
            b1 = 2 * (before[1] - 48) + 1; //입력 받은 숫자(문자열 변수에 저장) 아스키 코드를 이용해서 행 인덱스로 변경
            b2 = 4 * (before[0] - 64) - 2; //입력 받은 대문자 알파벳 아스키 코드를 이용해서 열 인덱스로 변경
            if (*(*(pStr + b1) + (b2-1)) == '[') { printf("It is not yours\n\n"); continue; } // 상대방 말 선택시 다시 선택
            if (*(*(pStr + b1) + b2) == '.') { printf("There are nothing\n\n"); continue; } // 비어있는 위치 선택시 다시 선택
            else {
                // 선택한 말의 종류에 따라 이동 가능한 곳 *으로 표시
                switch (*(*(pStr+b1)+b2)) {
                    case 'P': { Pawn(b1,b2); break; }
                    case 'R': { Rook(b1,b2); break; }
                    case 'N': { Knight(b1,b2); break; }
                    case 'B': { Bishop(b1,b2); break; }
                    case 'Q': { Queen(b1,b2); break; }
                    case 'K': { King(b1,b2); break; }
                }

                //이동 후 check 상태가 되는 곳은 .으로 변경
                for(int i=0;i<8;i++) {
                    for(int j=0;j<8;j++) {
                        Checkbackpan(); //확인이 끝난 후 돌려놓을 판을 위해 저장해놓음 (백업용)
                        int checkx=-1, checky=-1; // 이동을 금지시킬 행과 열 인덱스 저장용
                        if(*(*(pStr+(2*i+1))+(4*j+2)) == '*') { // 이동 가능하다고 표시된 곳으로 이동시키기 (임시)
                            *(*(pStr+(2*i+1))+(4*j+1)) = '<';
                            *(*(pStr+(2*i+1))+(4*j+2)) = *(*(pStr+b1)+b2);
                            *(*(pStr+(2*i+1))+(4*j+3)) = '>';
                            *(*(pStr+b1)+(b2-1)) = *(*(pStr+b1)+b2) = *(*(pStr+b1)+(b2+1)) = '.';
                            Delete_s();
                            if ( *(*(pStr+(2*i+1))+(4*j+2)) != 'K' ) {
                                if ( Check(1, 0) == 1 ) { checkx = 4*j+2; checky = 2*i+1; } //check인 경우 좌표 저장
                            }
                            else {
                                kx3 = 4*j+2; ky3 = 2*i+1; //움직일 말이 King이면 Check함수에서 이용할 변수값도 바꿔야 함
                                if ( Check(1, 1) == 1 ) { checkx = 4*j+2; checky = 2*i+1; } //check인 경우 좌표 저장
                            }
                            Returncheckpan(); //다시 현재 판으로 돌려놓음
                            if (checkx >= 0) {*(*(pStr+(checky))+(checkx)) = '.'; }
                            // 만약 check인 경우가 있어서 checkx, checky에 인덱스 값이 저장되어있으면 * -> . (이동 못함)
                        }
                    }
                }

                // 선택된 말이 움질일 수 있는 경로가 있는지 확인
                for(int i=0;i<8;i++) {
                    for(int j=0;j<8;j++) {
                        if(*(*(pStr+(2*i+1))+(4*j+2)) == '*')
                            F += 1; // F >= 1 이면 이동 시킬 수 있다
                    }
                }
                if(F == 0) { //이동시킬 수 있는 위치가 없으면 위치 다시 입력 받기
                    printf("You can not move that!\n\n");
                    continue;
                }

                else { //이동 시킬 수 있는 위치가 있는 경우
                    F=0; //다음 이용을 위해 초기화
                    clear();
                    Print(); // 이동 가능 위치 표시된 체스판 출력
                    break;}
            }
        }
        else {printf("again\n"); clear(); Print(); continue;}
    }

    // 선택된 말 이동시킬 경로 입력받아 이동시키기
    while(1) {
        if(end == 3||end == 2) break; // 앞에서 exit을 입력받았거나 종료 조건이 만족된 경우 종료
        printf("\n< Player 1 >\nWhere? : ");
        scanf("%s", after);
        if (strlen(after) == 2) {
            a1 = 2 * (after[1] - 48) + 1; //입력 받은 숫자(문자열 변수에 저장) 아스키 코드를 이용해서 행 인덱스로 변경
            a2 = 4 * (after[0] - 64) - 2; //입력 받은 대문자 알파벳 아스키 코드를 이용해서 열 인덱스로 변경

            if( *(*(pStr+b1)+b2) =='P' && a1==15) { //특수룰 - Pawn이 상대측 체스판 끝에 도달 시 Q, B, N, R 중 하나로 변경
                char Change[2];
                clear();
                printf("change P to ? (Q or B or N or R or P) : ");
                scanf("%s", Change);
                if(Change[0] == 'Q' || Change[0] == 'B' || Change[0] == 'N' || Change[0] == 'R' || Change[0] == 'P' )
                    *(*(pStr+b1)+b2) = Change[0]; // 입력받은 문자열로 변경
                else { printf("You can not change P to %c\n", Change[0]); } // 다른 문자 입력하면 PASS!
            }

            if(*(*(pStr+a1)+a2) != '*') { printf("again\n"); continue; } // 선택한 위치가 이동 불가능 한 위치면 다시 입력
            if (*(*(pStr + a1) + (a2 - 1)) == '<') { printf("again\n"); continue; } // 본인 말이 있는 위치 선택 시 다시 입력
            else { Delete_s(); break; } // 이동할 위치 정해졌으니 *표시 모두 제거
        }
        else {printf("again\n"); clear(); Print(); continue;}
    }

    if( end!=3 && end!=2 ) { //게임이 종료되는 경우가 아니면 실행

        //이동시킬 말이 King인 경우 변경될 좌표 저장 (for check확인)
        if (*(*(pStr+b1)+b2)=='K') {
            kx1 = a1; ky1 = a2;
        }

        //왕 잡으면 end
        if (*(*(pStr+a1)+ a2)=='K') end=1;

        //자리 옮기기
        if (*(*(pStr+a1)+(a2-1)) == '[') { //상대말 말을 잡은 경우 체스판 프린트시 표시
            switch (backpan[a1][a2]) { // "DIE-" 뒤에 이어서 저장 (strcat() 이용)
                case 'P': { strcat(die2, "[P]"); break; }
                case 'R': { strcat(die2, "[R]"); break; }
                case 'B': { strcat(die2, "[B]"); break; }
                case 'N': { strcat(die2, "[N]"); break; }
                case 'Q': { strcat(die2, "[Q]"); break; }
            }
        }
        //위치 변경 후 기존 자리는 ...으로 표시
        *(*(pStr+a1)+ a2) = *(*(pStr+b1)+b2);
        *(*(pStr+a1)+ ++a2) = '>'; a2--;
        *(*(pStr+a1)+ --a2) = '<';
        *(*(pStr+b1)+b2) = '.';
        *(*(pStr+b1)+ ++b2) = '.'; b2--;
        *(*(pStr+b1)+ --b2) = '.';
    }
}

void Move2(void) { //player 2 이동함수
    char before[5], after[3];  //이동시킬 말, 위치 입력받을 변수 (save나 exit 입력받아야하는 경우가 있어서 크기: 5)
    int a1=0, a2=0, b1=0, b2=0; //이동시킬 말의 행, 열, 이동시킬 위치의 행, 열
    Backpan();

    //이동시킬 말 입력받아 이동할 수 있는 경로 *로 표시하기
    while(1) {

        if(Checkmate(2) == 0) { end = 1; break;} //만약 player2가 움직일 수 있는 말이 없는 경우 player1 승리, 게임 종료
        for(int k=0;k<17;k++) {strcpy(pan[k], backpan[k]);} //아니면 다시 실행중인 판 복구

        printf("\n[ Player 2 ]\n(save: save game board, exit: end game)\nWhat? : ");
        scanf("%s", before);
        if (strcmp(before, "save") == 0) { // 사용자가 save입력 시 현재 체스판 텍스트 파일에 저장
            Save();
            printf("S A V E . . . !\n");
            continue;
        }
        else if(strcmp(before, "exit") == 0) { // 사용자가 exit입력 시 게임 종료 후 메뉴로 돌아감
            end += 3;
            break;
        }
        else if (strlen(before) == 2) {
            b1 = 2 * (before[1] - 48) + 1; //입력 받은 숫자(문자열 변수에 저장) 아스키 코드를 이용해서 행 인덱스로 변경
            b2 = 4 * (before[0] - 64) - 2; //입력 받은 대문자 알파벳 아스키 코드를 이용해서 열 인덱스로 변경
            if (*(*(pStr + b1) + (b2-1)) == '<') { printf("It is not yours\n\n"); continue; } // 상대방 말 선택시 다시 선택
            if (*(*(pStr + b1) + b2) == '.') { printf("There are nothing\n\n"); continue; } // 비어있는 위치 선택시 다시 선택
            else {
                // 선택한 말의 종류에 따라 이동 가능한 곳 *으로 표시
                switch (*(*(pStr+b1)+b2)) {
                    case 'P': { Pawn(b1,b2); break; }
                    case 'R': { Rook(b1,b2); break; }
                    case 'N': { Knight(b1,b2); break; }
                    case 'B': { Bishop(b1,b2); break; }
                    case 'Q': { Queen(b1,b2); break; }
                    case 'K': { King(b1,b2); break; }
                }

                //이동 후 check 상태가 되는 곳은 .으로 변경
                for(int i=0;i<8;i++) {
                    for(int j=0;j<8;j++) {
                        Checkbackpan(); // 확인이 끝난 후 돌려놓을 판을 위해 저장해놓음 (백업용)
                        int checkx=-1, checky=-1; // 이동을 금지시킬 행과 열 인덱스 저장용
                        if( *(*(pStr+(2*i+1))+(4*j+2)) == '*' ) { // 이동 가능하다고 표시된 곳으로 이동시키기 (임시)
                            *(*(pStr+(2*i+1))+(4*j+1)) = '[';
                            *(*(pStr+(2*i+1))+(4*j+2)) = *(*(pStr+b1)+b2);
                            *(*(pStr+(2*i+1))+(4*j+3)) = ']';
                            *(*(pStr+b1)+(b2-1)) = *(*(pStr+b1)+b2) = *(*(pStr+b1)+(b2+1)) = '.';
                            Delete_s();

                            if ( *(*(pStr+(2*i+1))+(4*j+2)) != 'K' ) {
                                if ( Check(2,0) == 1 ) { checkx = 4*j+2; checky = 2*i+1; } //check인 경우 좌표 저장
                            }
                            else {
                                kx3 = 4*j+2; ky3 = 2*i+1; //움직일 말이 King이면 Check함수에서 이용할 변수값도 바꿔야 함
                                if ( Check(2,1) == 1 ) { checkx = kx3; checky = ky3; } //check인 경우 좌표 저장
                            }

                            Returncheckpan(); //다시 현재 판으로 돌려놓음
                            if (checkx >= 0) {*(*(pStr+checky)+checkx) = '.'; }
                            // 만약 check인 경우가 있어서 checkx, checky에 인덱스 값이 저장되어있으면 * -> . (이동 금지 시키기)
                        }
                    }
                }

                // 선택된 말이 움질일 수 있는 경로가 있는지 확인
                for(int i=0;i<8;i++) {
                    for(int j=0;j<8;j++) {
                        if(*(*(pStr+(2*i+1))+(4*j+2)) == '*')
                            F += 1; // F >= 1 이면 이동 시킬 수 있다
                    }
                }
                if(F == 0) { //이동시킬 수 있는 위치가 없으면 위치 다시 입력 받기
                    printf("You can not move that!\n");
                    continue;
                }

                else { //이동 시킬 수 있는 위치가 있는 경우
                    F=0; //다음 이용을 위해 초기화
                    clear();
                    Print(); // 이동 가능 위치 표시된 체스판 출력
                    break;}
            }
        }
        else {printf("again\n"); clear(); Print(); continue;}
    }

    // 선택된 말 이동시킬 경로 입력받아 이동시키기
    while(1) {
        if (end==3 || end==1) break; // 앞에서 exit을 입력받았거나 종료 조건이 만족된 경우 종료
        printf("\n[ Player 2 ]\nWhere? : ");
        scanf("%s", after);
        if (strlen(after) == 2) {
            a1 = 2 * (after[1] - 48) + 1; //입력 받은 숫자(문자열 변수에 저장) 아스키 코드를 이용해서 행 인덱스로 변경
            a2 = 4 * (after[0] - 64) - 2; //입력 받은 대문자 알파벳 아스키 코드를 이용해서 열 인덱스로 변경

            if( *(*(pStr+b1)+b2) == 'P' && a1==1) { //특수룰 - Pawn이 상대측 체스판 끝에 도달 시 Q, B, N, R 중 하나로 변경
                char Change[2];
                clear();
                printf("change P to ? (Q or B or N or R) : ");
                scanf("%s", Change);
                if(Change[0] == 'Q' || Change[0] == 'B' || Change[0] == 'N' || Change[0] == 'R' || Change[0] == 'P' )
                    *(*(pStr+b1)+b2) = Change[0]; // 입력받은 문자열로 변경
                else { printf("You can not change P to %c\n", Change[0]); } // 다른 문자 입력하면 PASS!
            }

            if(*(*(pStr+a1)+a2) != '*') { printf("again\n"); continue; } // 선택한 위치가 이동 불가능 한 위치면 다시 입력
            if (*(*(pStr + a1) + (a2 - 1)) == '[') { printf("again\n"); continue; } // 본인 말이 있는 위치 선택 시 다시 입력
            else { Delete_s(); break; } // 이동할 위치 정해졌으니 *표시 모두 제거
        }
        else {printf("again\n"); clear(); Print(); continue;}
    }

    if (end != 3 && end != 1) { //게임이 종료되는 경우가 아니면 실행

        //이동시킬 말이 King인 경우 변경될 좌표 저장 (for check확인)
        if (*(*(pStr+b1)+b2)=='K') {
            kx2 = a1; ky2 = a2;
        }

        // 왕 잡으면 end
        if (*(*(pStr+a1)+ a2)=='K') end=2;


        //자리 옮기기
        if (*(*(pStr+a1)+(a2-1)) == '<') { //상대말 말을 잡은 경우 체스판 프린트 시 표시
            switch (backpan[a1][a2]) { // "DIE-" 뒤에 이어서 저장 (strcat() 이용)
                case 'P': { strcat(die1, "<P>"); break; }
                case 'R': { strcat(die1, "<R>"); break; }
                case 'B': { strcat(die1, "<B>"); break; }
                case 'N': { strcat(die1, "<N>"); break; }
                case 'Q': { strcat(die1, "<Q>"); break; }
            }
        }

        //위치 변경 후 기존 자리는 ...으로 표시
        *(*(pStr+a1)+ a2) = *(*(pStr+b1)+b2);
        *(*(pStr+a1)+ ++a2) = ']'; a2--;
        *(*(pStr+a1)+ --a2) = '[';
        *(*(pStr+b1)+b2) = '.';
        *(*(pStr+b1)+ ++b2) = '.'; b2--;
        *(*(pStr+b1)+ --b2) = '.';
    }
}

void Pawn(int b1, int b2) { // Pawn 선택 시 이동 가능 위치 *로 표시

    if(*(*(pStr+b1)+(b2-1))=='<') { //player1의 경우
        if(*(*(pStr+(b1+2))+b2) == '.' ) { // Pawn은 앞 칸이 이어있는 경우 앞으로 한 칸 전진(행 인덱스 2 증가) 가능 하다
            *(*(pStr+(b1+2))+b2) = '*';
            if( b1==3 ) {*(*(pStr+(b1+4))+b2) = '*';} // 처음 이동 시키는 경우 두 칸 전진도 가능하다
        }
        if( *(*(pStr+(b1+2))+(b2-4))!='.' && *(*(pStr+(b1+2))+(b2-5))=='[' ) {
            // 한 칸 대각선에 상대말 말이 있는 경우 이동하여 잡는 것이 가능하다
            *(*(pStr+(b1+2))+(b2-4)) = '*';
        }
        if( *(*(pStr+(b1+2))+(b2+4)) != '.' && *(*(pStr+(b1+2))+(b2+5))==']' ) {
            // 한 칸 대각선에 상대말 말이 있는 경우 이동하여 잡는 것이 가능하다

            *(*(pStr+(b1+2))+(b2+4)) = '*'; }
    }
    else { //player2의 경우
        if(*(*(pStr+(b1-2))+b2) == '.') { // Pawn은 앞 칸이 이어있는 경우 앞으로 한 칸 전진(행 인덱스 2 감소) 가능 하다
            *(*(pStr+(b1-2))+b2) = '*';
            if( b1==13 ) {*(*(pStr+(b1-4))+b2) = '*';} // 처음 이동 시키는 경우 두 칸 전진도 가능하다
        }
        if( *(*(pStr+(b1-2))+(b2-4))!='.' && *(*(pStr+(b1-2))+(b2-5))=='<') {
            // 한 칸 대각선에 상대말 말이 있는 경우 이동하여 잡는 것이 가능하다
            *(*(pStr+(b1-2))+(b2-4)) = '*';
        }
        if( *(*(pStr+(b1-2))+(b2+4)) != '.' && *(*(pStr+(b1-2))+(b2+3))=='<') {
            // 한 칸 대각선에 상대말 말이 있는 경우 이동하여 잡는 것이 가능하다
            *(*(pStr+(b1-2))+(b2+4)) = '*';
        }
    }
}

void Rook(int b1, int b2) { // Rook 선택 시 이동 가능 위치 *로 표시
    int B1 = b1, B2 = b2; // 변동이 있을 때 초기화를 위함

    if (*(*(pStr + B1) + (B2 - 1)) == '<') { // Player 1의 경우
        while (1) { // 앞으로 이동 가능
            B1 += 2;
            if(B1 > 16) break; //체스판 끝까지 확인
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //비어있는 칸이면 계속 이동 가능
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
        B1 = b1; // 다시 원래 자리로
        while (1) { // 뒤로 이동 가능
            B1 -= 2;
            if (B1 < 0) break; //체스판 끝까지 확인
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //비어있는 칸이면 계속 이동 가능
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
        B1 = b1; // 다시 원래 자리로
        while (1) { // 오른쪽으로 이동 가능
            B2 += 4;
            if (B2 > 33) break; //체스판 끝까지 확인
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //비어있는 칸이면 계속 이동 가능
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
        B2 = b2; // 다시 원래 자리로
        while (1) { // 왼쪽으로 이동 가능
            B2 -= 4;
            if (B2<0) break; //체스판 끝까지 확인
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //비어있는 칸이면 계속 이동 가능
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
    }

    else { // Player 2의 경우
        while (1) { //뒤로 이동 가능
            B1 += 2;
            if(B1 > 16) break; //체스판 끝까지 확인
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //비어있는 칸이면 계속 이동 가능
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
        B1 = b1; // 다시 원래 자리로
        while (1) { //앞으로 이동 가능
            B1 -= 2;
            if(B1<0) break; //체스판 끝까지 확인
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //비어있는 칸이면 계속 이동 가능
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
        B1 = b1; // 다시 원래 자리로
        while (1) { //오른쪽으로 이동 가능
            B2 += 4;
            if (B2 > 33) break; //체스판 끝까지 확인
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //비어있는 칸이면 계속 이동 가능
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
        B2 = b2; // 다시 원래 자리로
        while (1) { //왼쪽으로 이동 가능
            B2 -= 4;
            if (B2 < 0) break; //체스판 끝까지 확인
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //비어있는 칸이면 계속 이동 가능
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
    }
}

void Bishop(int b1, int b2) { // Bishop 선택 시 이동 가능 위치 *로 표시
    int B1 = b1, B2 = b2; // 변동이 있을 때 초기화를 위함

    if(*(*(pStr+b1)+(b2-1))=='<') { //player1의 경우
        while (1) { // ↘
            B1 += 2;
            B2 += 4;
            if (B1 > 16 || B2 > 33) break; //체스판 끝까지 확인
            else if(*(*(pStr+B1)+B2) == '.') { //비어있는 칸이면 계속 이동 가능
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '[') { //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
        B1=b1; B2=b2; // 다시 원래 자리로

        while (1) { // ↙
            B1 += 2;
            B2 -= 4;
            if (B1 > 16 || B2 < 0) break; //체스판 끝까지 확인
            else if(*(*(pStr+B1)+B2) == '.') { //비어있는 칸이면 계속 이동 가능
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '[') { //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
        B1=b1; B2=b2; // 다시 원래 자리로

        while (1) { // ↗
            B1 -= 2;
            B2 += 4;
            if (B1 < 0 || B2 > 33) break; //체스판 끝까지 확인
            else if(*(*(pStr+B1)+B2) == '.') { //비어있는 칸이면 계속 이동 가능
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '[') { //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
        B1=b1; B2=b2; // 다시 원래 자리로

        while (1) { // ↖
            B1 -= 2;
            B2 -= 4;
            if (B1 < 0 || B2 < 0) break; //체스판 끝까지 확인
            else if(*(*(pStr+B1)+B2) == '.') { //비어있는 칸이면 계속 이동 가능
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '[') { //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
    }

    else { //player2의 경우
        while (1) { // ↘
            B1 += 2;
            B2 += 4;
            if (B1 > 16 || B2 > 33) break;
            else if(*(*(pStr+B1)+B2) == '.') { //비어있는 칸이면 계속 이동 가능
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '<') { //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
        B1=b1; B2=b2; // 다시 원래 자리로

        while (1) { // ↙
            B1 += 2;
            B2 -= 4;
            if (B1 > 16 || B2 < 0) break;
            else if(*(*(pStr+B1)+B2) == '.') { //비어있는 칸이면 계속 이동 가능
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '<') { //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
        B1=b1; B2=b2; // 다시 원래 자리로

        while (1) { // ↗
            B1 -= 2;
            B2 += 4;
            if (B1 < 0 || B2 > 33) break;
            else if(*(*(pStr+B1)+B2) == '.') { //비어있는 칸이면 계속 이동 가능
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '<') { //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
        B1=b1; B2=b2; // 다시 원래 자리로

        while (1) { // ↖
            B1 -= 2;
            B2 -= 4;
            if (B1 < 0 || B2 < 0) break;
            else if(*(*(pStr+B1)+B2) == '.') { //비어있는 칸이면 계속 이동 가능
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '<') { //상대방 말이 있으면 그 칸까지 이동 가능
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //본인 말이 있으면 그 칸부터 이동 불가능
        }
    }
}

void Knight(int b1, int b2) { // Knight 선택 시 이동 가능 위치 *로 표시
    //비어있는 칸이거나 상대방 말이 있는 경우 이동 가능

    if(*(*(pStr+b1)+(b2-1)) == '<') { //player1의 경우 (상대방의 말이 있거나 비어있으면 이동가능)
        if( *(*(pStr+(b1+4))+(b2+3)) == '[' || *(*(pStr+(b1+4))+(b2+3)) == '.' ) *(*(pStr+(b1+4))+(b2+4)) = '*'; // ↓ ↓ -→
        if( *(*(pStr+(b1+4))+(b2-5)) == '[' || *(*(pStr+(b1+4))+(b2-5)) == '.' ) *(*(pStr+(b1+4))+(b2-4)) = '*'; // ←- ↓ ↓
        if( *(*(pStr+(b1+2))+(b2+7)) == '[' || *(*(pStr+(b1+2))+(b2+7)) == '.' ) *(*(pStr+(b1+2))+(b2+8)) = '*'; // ↓ -→ -→
        if( *(*(pStr+(b1+2))+(b2-9)) == '[' || *(*(pStr+(b1+2))+(b2-9)) == '.' ) *(*(pStr+(b1+2))+(b2-8)) = '*'; // ←- ←- ↓
        if( *(*(pStr+(b1-4))+(b2+3)) == '[' || *(*(pStr+(b1-4))+(b2+3)) == '.' ) *(*(pStr+(b1-4))+(b2+4)) = '*'; // ↑ ↑ -→
        if( *(*(pStr+(b1-4))+(b2-5)) == '[' || *(*(pStr+(b1-4))+(b2-5)) == '.' ) *(*(pStr+(b1-4))+(b2-4)) = '*'; // ←- ↑ ↑
        if( *(*(pStr+(b1-2))+(b2+7)) == '[' || *(*(pStr+(b1-2))+(b2+7)) == '.' ) *(*(pStr+(b1-2))+(b2+8)) = '*'; // ↑ -→ -→
        if( *(*(pStr+(b1-2))+(b2-9)) == '[' || *(*(pStr+(b1-2))+(b2-9)) == '.' ) *(*(pStr+(b1-2))+(b2-8)) = '*'; // ←- ←- ↑
    }
    else { //player2의 경우 (상대방의 말이 있거나 비어있으면 이동가능)
        if( *(*(pStr+(b1+4))+(b2+3)) == '<' || *(*(pStr+(b1+4))+(b2+3)) == '.' ) *(*(pStr+(b1+4))+(b2+4)) = '*'; // ↓ ↓ -→
        if( *(*(pStr+(b1+4))+(b2-5)) == '<' || *(*(pStr+(b1+4))+(b2-5)) == '.' ) *(*(pStr+(b1+4))+(b2-4)) = '*'; // ←- ↓ ↓
        if( *(*(pStr+(b1+2))+(b2+7)) == '<' || *(*(pStr+(b1+2))+(b2+7)) == '.' ) *(*(pStr+(b1+2))+(b2+8)) = '*'; // ↓ -→ -→
        if( *(*(pStr+(b1+2))+(b2-9)) == '<' || *(*(pStr+(b1+2))+(b2-9)) == '.' ) *(*(pStr+(b1+2))+(b2-8)) = '*'; // ←- ←- ↓
        if( *(*(pStr+(b1-4))+(b2+3)) == '<' || *(*(pStr+(b1-4))+(b2+3)) == '.' ) *(*(pStr+(b1-4))+(b2+4)) = '*'; // ↑ ↑ -→
        if( *(*(pStr+(b1-4))+(b2-5)) == '<' || *(*(pStr+(b1-4))+(b2-5)) == '.' ) *(*(pStr+(b1-4))+(b2-4)) = '*'; // ←- ↑ ↑
        if( *(*(pStr+(b1-2))+(b2+7)) == '<' || *(*(pStr+(b1-2))+(b2+7)) == '.' ) *(*(pStr+(b1-2))+(b2+8)) = '*'; // ↑ -→ -→
        if( *(*(pStr+(b1-2))+(b2-9)) == '<' || *(*(pStr+(b1-2))+(b2-9)) == '.' ) *(*(pStr+(b1-2))+(b2-8)) = '*'; // ←- ←- ↑
    }

}

void Queen(int b1, int b2) { // Queen 선택 시 이동 가능 위치 *로 표시
    Rook(b1,b2); // 사방으로 이동 가능 = Rook
    Bishop(b1,b2); // 대각선으로 이동 가능 = Bishop
}

void King(int b1, int b2) { // King 선택 시 이동 가능 위치 *로 표시
    if(*(*(pStr+b1)+(b2-1)) == '<') { //player1의 경우
        if( *(*(pStr+(b1+2))+(b2-1)) != '<' ) *(*(pStr+(b1+2))+b2) = '*'; // ↓
        if( *(*(pStr+(b1-2))+(b2-1)) != '<' ) *(*(pStr+(b1-2))+b2) = '*'; // ↑
        if( *(*(pStr+b1)+(b2+3)) != '<' ) *(*(pStr+b1)+(b2+4)) = '*'; // -→
        if( *(*(pStr+b1)+(b2-5)) != '<' ) *(*(pStr+b1)+(b2-4)) = '*'; // ←-
        if( *(*(pStr+(b1+2))+(b2+3)) != '<' ) *(*(pStr+(b1+2))+(b2+4)) = '*'; // ↓ -→
        if( *(*(pStr+(b1+2))+(b2-5)) != '<' ) *(*(pStr+(b1+2))+(b2-4)) = '*'; // ←- ↓
        if( *(*(pStr+(b1-2))+(b2+3)) != '<' ) *(*(pStr+(b1-2))+(b2+4)) = '*'; // ↑ -→
        if( *(*(pStr+(b1-2))+(b2-5)) != '<' ) *(*(pStr+(b1-2))+(b2-4)) = '*'; // ←- ↑
    }
    else { //player2의 경우
        if( *(*(pStr+(b1+2))+(b2-1)) != '[' ) *(*(pStr+(b1+2))+b2) = '*'; // ↓
        if( *(*(pStr+(b1-2))+(b2-1)) != '[' ) *(*(pStr+(b1-2))+b2) = '*'; // ↑
        if( *(*(pStr+b1)+(b2+3)) != '[' ) *(*(pStr+b1)+(b2+4)) = '*'; // -→
        if( *(*(pStr+b1)+(b2-5)) != '[' ) *(*(pStr+b1)+(b2-4)) = '*'; // ←-
        if( *(*(pStr+(b1+2))+(b2+3)) != '[' ) *(*(pStr+(b1+2))+(b2+4)) = '*'; // ↓ -→
        if( *(*(pStr+(b1+2))+(b2-5)) != '[' ) *(*(pStr+(b1+2))+(b2-4)) = '*'; // ←- ↓
        if( *(*(pStr+(b1-2))+(b2+3)) != '[' ) *(*(pStr+(b1-2))+(b2+4)) = '*'; // ↑ -→
        if( *(*(pStr+(b1-2))+(b2-5)) != '[' ) *(*(pStr+(b1-2))+(b2-4)) = '*'; // ←- ↑
    }
}

void Save(void) { //현재 실행중인 체스판 저장
    FILE *fp = NULL; // 저장용 텍스트 파일 저장할 공간
    fp = fopen(SPAN, "w+"); //chess_save.txt파일 열기 (없으면 만들어서 열기)

    for(int i=-1;i<19;i++) { // 한줄씩 파일에 입력하기
        if (i == -1) fprintf(fp, "%s \n", die2);
        else if (i == 17) fprintf(fp, "%s \n", die1);
        else if (i == 18) fprintf(fp, "%d", startplayer);
        else fprintf(fp, "%s \n", *(pStr+i));
    }
    fclose(fp); // 파일 닫기
}

int Check(int player, int king) { // 모든 말이 움직일 수 있는 경우의 수를 계산하고 그 중 check상태가 될 수 있는 경우가 있는지 확인하기
    int c=0; // check가 될 수 있는 경우의 수
    int x,y;

    if(player==1) { //player1의 경우
        if ( king == 1 ) { x=kx3, y=ky3; } //현재 king의 좌표 저장
        else { x=kx1, y=ky1; } //현재 king의 좌표 저장

        //오른쪽
        for(int i=(x+4);i<34;i+=4) { // 체스판 끝까지 확인
            if ( *(*(pStr+i)+(x-1)) == '<') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if( *(*(pStr+y)+(i-1))  == '[') { // 상대방 R이나 Q가 있으면 check
                if( *(*(pStr+y)+i) == 'R' || *(*(pStr+y)+i) == 'Q' ) {
                    c++; break;
                }
            }
        }

        //왼쪽
        for(int i=(x-4);i>=0;i-=4) { // 체스판 끝까지 확인
            if ( *(*(pStr+i)+(x-1)) == '<') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if( *(*(pStr+y)+(i-1))  == '[') { // 상대방 R이나 Q가 있으면 check
                if( *(*(pStr+y)+i) == 'R' || *(*(pStr+y)+i) == 'Q' ) {
                    c++; break;
                }
            }
        }

        //위
        for(int i=(y+2);i<17;i+=2) { // 체스판 끝까지 확인
            if ( *(*(pStr+i)+(x-1)) == '<') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if( *(*(pStr+i)+(x-1))  == '[') { // 상대방 R이나 Q가 있으면 check
                if( *(*(pStr+i)+x) == 'R' || *(*(pStr+i)+x) == 'Q' ) {
                    c++; break;
                }
            }
        }

        //아래
        for(int i=(y-2);i>=0;i-=2) { // 체스판 끝까지 확인
            if ( *(*(pStr+i)+(x-1)) == '<') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if( *(*(pStr+i)+(x-1))  == '[') { // 상대방 R이나 Q가 있으면 check
                if( *(*(pStr+i)+x) == 'R' || *(*(pStr+i)+x) == 'Q' ) {
                    c++; break;
                }
            }
        }

        //대각선1
        int pp=0; //pawn 확인용 (대각선 기준 몇 칸 차이인지)
        while(1) {
            x += 4; y += 2; pp++;
            if( *(*(pStr+y)+(x-1)) == '<') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if( *(*(pStr+y)+(x-1)) == '[') {
                if ( *(*(pStr+y)+x) == 'P' && pp == 1) { // 한 칸 대각선에 상대방 P가 있으면 check
                    c++; break;
                }
                else if( *(*(pStr+y)+(x-1)) == 'Q' || *(*(pStr+y)+(x-1)) == 'B' ) { // 상대방 B나 Q가 있으면 check
                    c++; break;
                }
            }
            if(x>33||y>16) {break;} // 체스판 끝까지 확인
        }

        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx1, y=ky1; } // 좌표 원래대로 돌려놓기

        //대각선2
        pp=0; //pawn 확인용 (대각선 기준 몇 칸 차이인지)
        while(1) {
            x -= 4; y += 2; pp++;
            if( *(*(pStr+y)+(x-1)) == '<') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if( *(*(pStr+y)+(x-1)) == '[') {
                if ( *(*(pStr+y)+x) == 'P' && pp == 1) { // 한 칸 대각선에 상대방 P가 있으면 check
                    c++; break;
                }
                else if( *(*(pStr+y)+(x-1)) == 'Q' || *(*(pStr+y)+(x-1)) == 'B' ) { // 상대방 B나 Q가 있으면 check
                    c++; break;
                }
            }
            if(x<0||y>16) break; // 체스판 끝까지 확인
        }
        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx1, y=ky1; } // 좌표 원래대로 돌려놓기

        //대각선3
        while(1) {
            x -= 4; y -= 2;
            if( *(*(pStr+y)+(x-1)) == '<') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if( *(*(pStr+y)+(x-1)) == '[') { // 상대방 B나 Q가 있으면 check
                if( *(*(pStr+y)+(x-1)) == 'Q' || *(*(pStr+y)+(x-1)) == 'B' ) {
                    c++; break;
                }
            }
            if(x<0||y<0) break; // 체스판 끝까지 확인
        }
        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx1, y=ky1; } // 좌표 원래대로 돌려놓기

        //대각선4
        while(1) {
            x += 4; y -= 2;
            if( *(*(pStr+y)+(x-1)) == '<') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if( *(*(pStr+y)+(x-1)) == '[') { // 상대방 B나 Q가 있으면 check
                if( *(*(pStr+y)+(x-1)) == 'Q' || *(*(pStr+y)+(x-1)) == 'B' ) {
                    c++; break;
                }
            }
            if(x>33||y<0) break; // 체스판 끝까지 확인
        }

        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx1, y=ky1; } // 좌표 원래대로 돌려놓기

        //knight
        if( *(*(pStr+(y+4))+(x+3)) == '[' && *(*(pStr+(y+4))+(x+4)) == 'N' ) {
            if( (x+3)<33 && (y+4)<17) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y+4))+(x-5)) == '[' && *(*(pStr+(y+4))+(x-4)) == 'N' ) {
            if ((x-5) >= 0 && (y+4) < 17) c++; // ←- ↓ ↓ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y-2))+(x-9)) == '[' && *(*(pStr+(y-2))+(x-8)) == 'N' ) {
            if((x-9)>=0 && (y-2)>=0) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y+2))+(x+7)) == '[' && *(*(pStr+(y+2))+(x+8)) == 'N' ) {
            if((x+7)<33 && (y+2)<17) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y+2))+(x-9)) == '[' && *(*(pStr+(y+2))+(x-8)) == 'N' ) {
            if((x-9)>=0 && (y+2)<17) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y-4))+(x+3)) == '[' && *(*(pStr+(y-4))+(x+4)) == 'N' ) {
            if((x+3)<33 && (y-4)>=0) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y-4))+(x-5)) == '[' && *(*(pStr+(y-4))+(x-4)) == 'N' ) {
            if((x-5)>=0 && (y-4)>=0) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y-2))+(x+7)) == '[' && *(*(pStr+(y-2))+(x+8)) == 'N' ) {
            if((x+7)<33 && (y-2)>=0) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
    }


    else { //player2의 경우
        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx2, y=ky2; } //현재 king의 좌표 저장

        //오른쪽
        for (int i = (x+4); i < 34; i += 4) { // 체스판 끝까지 확인
            if ( *(*(pStr+i)+(x-1)) == '[') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if (*(*(pStr + y) + (i - 1)) == '<') { // 상대방 R이나 Q가 있으면 check
                if (*(*(pStr + y) + i) == 'R' || *(*(pStr + y) + i) == 'Q') {
                    c++; break;
                }
            }
        }

        //왼쪽
        for (int i = (x-4); i >= 0; i -= 4) { // 체스판 끝까지 확인
            if ( *(*(pStr+i)+(x-1)) == '[') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if (*(*(pStr + y) + (i - 1)) == '<') { // 상대방 R이나 Q가 있으면 check
                if (*(*(pStr + y) + i) == 'R' || *(*(pStr + y) + i) == 'Q') {
                    c++;  break;
                }
            }
        }

        //위
        for (int i = (y+2); i < 17; i += 2) { // 체스판 끝까지 확인
            if ( *(*(pStr+i)+(x-1)) == '[') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if (*(*(pStr + i) + (x - 1)) == '<') { // 상대방 R이나 Q가 있으면 check
                if (*(*(pStr + i) + x) == 'R' || *(*(pStr + i) + x) == 'Q') {
                    c++;  break;
                }
            }
        }

        //아래
        for (int i = (y-2); i >= 0; i -= 2) { // 체스판 끝까지 확인
            if ( *(*(pStr+i)+(x-1)) == '[') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if (*(*(pStr + i) + (x - 1)) == '<') { // 상대방 R이나 Q가 있으면 check
                if (*(*(pStr + i) + x) == 'R' || *(*(pStr + i) + x) == 'Q') {
                    c++;  break;
                }
            }
        }

        //대각선1
        while (1) {
            x += 4; y += 2;
            if( *(*(pStr+y)+(x-1)) == '[') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if (*(*(pStr + y) + (x - 1)) == '<') { // 상대방 B나 Q가 있으면 check
                if (*(*(pStr + y) + (x - 1)) == 'Q' || *(*(pStr + y) + (x - 1)) == 'B') {
                    c++;  break;
                }
            }
            if (x > 33 || y > 16) break; // 체스판 끝까지 확인
        }
        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx2, y=ky2; } // 좌표 원래대로 돌려놓기

        //대각선2
        while (1) {
            x -= 4; y += 2;
            if( *(*(pStr+y)+(x-1)) == '[') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if (*(*(pStr + y) + (x - 1)) == '<') { // 상대방 B나 Q가 있으면 check
                if (*(*(pStr + y) + (x - 1)) == 'Q' || *(*(pStr + y) + (x - 1)) == 'B') {
                    c++;  break;
                }
            }
            if (x < 0 || y > 16) break; // 체스판 끝까지 확인
        }
        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx2, y=ky2; } // 좌표 원래대로 돌려놓기

        //대각선3
        int pp = 0; //pawn 확인용 (대각선 기준 몇 칸 차이인지)
        while (1) {
            x -= 4; y -= 2; pp++;
            if( *(*(pStr+y)+(x-1)) == '[') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if (*(*(pStr + y) + (x - 1)) == '<') {
                if (*(*(pStr + y) + x) == 'P' && pp == 1) { // 한 칸 대각선에 상대방 P가 있으면 check
                    c++;  break;
                }
                else if (*(*(pStr + y) + (x - 1)) == 'Q' || *(*(pStr + y) + (x - 1)) == 'B') { // 상대방 B나 Q가 있으면 check
                    c++; break; // 체스판 끝까지 확인
                }
            }
            if (x < 0 || y < 0) break; // 체스판 끝까지 확인
        }
        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx2, y=ky2; } // 좌표 원래대로 돌려놓기

        //대각선4
        pp = 0; //pawn 확인용 (대각선 기준 몇 칸 차이인지)
        while (1) {
            x += 4; y -= 2; pp++;
            if( *(*(pStr+y)+(x-1)) == '[') break; // 본인 말이 가장 가까이에 있으면 check 아님
            else if (*(*(pStr + y) + (x - 1)) == '<') {
                if (*(*(pStr + y) + x) == 'P' && pp == 1) { // 한 칸 대각선에 상대방 P가 있으면 check
                    c++; break;
                }
                else if (*(*(pStr + y) + (x - 1)) == 'Q' || *(*(pStr + y) + (x - 1)) == 'B') { // 상대방 B나 Q가 있으면 check
                    c++;  break;
                }
            }
            if (x > 33 || y < 0) break; // 체스판 끝까지 확인
        }

        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx2, y=ky2; } // 좌표 원래대로 돌려놓기

        //knight
        if( *(*(pStr+(y+4))+(x+3)) == '<' && *(*(pStr+(y+4))+(x+4)) == 'N' ) {
            if( (x+3)<33 && (y+4)<17) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y+4))+(x-5)) == '<' && *(*(pStr+(y+4))+(x-4)) == 'N' ) {
            if ((x-5) >= 0 && (y+4) < 17) c++; // ←- ↓ ↓ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y-2))+(x-9)) == '<' && *(*(pStr+(y-2))+(x-8)) == 'N' ) {
            if((x-9)>=0 && (y-2)>=0) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y+2))+(x+7)) == '<' && *(*(pStr+(y+2))+(x+8)) == 'N' ) {
            if((x+7)<33 && (y+2)<17) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y+2))+(x-9)) == '<' && *(*(pStr+(y+2))+(x-8)) == 'N' ) {
            if((x-9)>=0 && (y+2)<17) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y-4))+(x+3)) == '<' && *(*(pStr+(y-4))+(x+4)) == 'N' ) {
            if((x+3)<33 && (y-4)>=0) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y-4))+(x-5)) == '<' && *(*(pStr+(y-4))+(x-4)) == 'N' ) {
            if((x-5)>=0 && (y-4)>=0) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if( *(*(pStr+(y-2))+(x+7)) == '<' && *(*(pStr+(y-2))+(x+8)) == 'N' ) {
            if((x+7)<33 && (y-2)>=0) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
    }

    if(c != 0) return 1; // check인 경우가 있다
    else return 0; // check인 경우가 없다
}

int Checkmate(int player) {
    if(player == 1) {
        Backpan(); // 확인 후 체스판을 처음 상태로 돌려놓기 위해 저장 (백업)

        for(int I=1;I<17;I+=2) {
            for(int J=2;J<34;J+=4) {
                if ( *(*(pStr+I)+(J-1)) == '<' ) { // 판에 있는 모든 player1의 말을 대상으로 확인
                    switch (*(*(pStr+I)+J)) { // 말의 종류에 따라 이동 가능한 위치 표시
                        case 'P': { Pawn(I,J); break;}
                        case 'R': { Rook(I,J); break;}
                        case 'N': { Knight(I,J); break;}
                        case 'B': { Bishop(I,J); break;}
                        case 'Q': { Queen(I,J); break;}
                        case 'K': { King(I,J); break;}
                    }

                    //이동 후 check 상태가 되는 곳은 .으로 변경
                    for(int i=0;i<8;i++) {
                        for(int j=0;j<8;j++) {
                            Checkbackpan(); //확인이 끝난 후 돌려놓을 판을 위해 저장해놓음 (백업용)
                            int checkx=-1, checky=-1; // 이동을 금지시킬 행과 열 인덱스 저장용
                            if(*(*(pStr+(2*i+1))+(4*j+2)) == '*') {  // 이동 가능하다고 표시된 곳으로 이동시키기 (임시)
                                *(*(pStr+(2*i+1))+(4*j+1)) = '<';
                                *(*(pStr+(2*i+1))+(4*j+2)) = *(*(pStr+I)+J);
                                *(*(pStr+(2*i+1))+(4*j+3)) = '>';
                                *(*(pStr+I)+(J-1)) = *(*(pStr+I)+J) = *(*(pStr+I)+(J+1)) = '.';
                                Delete_s();
                                if ( *(*(pStr+(2*i+1))+(4*j+2)) != 'K' ) {
                                    if ( Check(1, 0) == 1 ) { checkx = j; checky = i; } //check인 경우 좌표 저장
                                }
                                else {
                                    kx3 = 4*j+2; ky3 = 2*i+1; //움직일 말이 King이면 Check함수에서 이용할 변수값도 바꿔야 함
                                    if ( Check(1, 1) == 1 ) { checkx = j; checky = i; } //check인 경우 좌표 저장
                                }
                                Returncheckpan(); //다시 현재 판으로 돌려놓음
                                if (checkx >= 0) {*(*(pStr+(2*checky+1))+(4*checkx+2)) = '.';}
                                // 만약 check인 경우가 있어서 checkx, checky에 인덱스 값이 저장되어있으면 * -> . (이동 못함)
                            }
                        }
                    }

                    // 선택된 말이 움질일 수 있는 경로가 있는지 확인
                    for(int i=0;i<8;i++) {
                        for(int j=0;j<8;j++) {
                            if(*(*(pStr+(2*i+1))+(4*j+2)) == '*')
                                F += 1; // F >= 1이면 말이 움질일 수 있다는 것을 의미
                        }
                    }

                    if(F!=0) {F=0; return 1;} // 이동할 수 있는 곳이 적어도 하나 있으면 checkmate아님
                    else {
                        for(int k=0;k<17;k++)
                            strcpy(pan[k], backpan[k]);
                    }
                }
            }
        }
        if (F==0) return F;
    }

    else {
        Backpan(); // 확인 후 체스판을 처음 상태로 돌려놓기 위해 저장 (백업)

        for(int I=1;I<17;I+=2) {
            for(int J=2;J<34;J+=4) {
                if ( *(*(pStr+I)+(J-1)) == '[' ) { // 판에 있는 모든 player2의 말을 대상으로 확인
                    switch (*(*(pStr+I)+J)) { // 말의 종류에 따라 이동 가능한 위치 표시
                        case 'P': { Pawn(I,J); break;}
                        case 'R': { Rook(I,J); break;}
                        case 'N': { Knight(I,J); break;}
                        case 'B': { Bishop(I,J); break;}
                        case 'Q': { Queen(I,J); break;}
                        case 'K': { King(I,J); break;}
                    }

                    //이동 후 check 상태가 되는 곳은 .으로 변경
                    for(int i=0;i<8;i++) {
                        for(int j=0;j<8;j++) {
                            Checkbackpan(); //확인이 끝난 후 돌려놓을 판을 위해 저장해놓음 (백업용)
                            int checkx=-1, checky=-1; // 이동을 금지시킬 행과 열 인덱스 저장용
                            if(*(*(pStr+(2*i+1))+(4*j+2)) == '*') { // 이동 가능하다고 표시된 곳으로 이동시키기 (임시)
                                *(*(pStr+(2*i+1))+(4*j+1)) = '[';
                                *(*(pStr+(2*i+1))+(4*j+2)) = *(*(pStr+I)+J);
                                *(*(pStr+(2*i+1))+(4*j+3)) = ']';
                                *(*(pStr+I)+(J-1)) = *(*(pStr+I)+J) = *(*(pStr+I)+(J+1)) = '.';
                                Delete_s();
                                if ( *(*(pStr+(2*i+1))+(4*j+2)) != 'K' ) {
                                    if ( Check(2, 0) == 1 ) { checkx = j; checky = i;} //check인 경우 좌표 저장
                                }
                                else {
                                    kx3 = 4*j+2; ky3 = 2*i+2; //움직일 말이 King이면 Check함수에서 이용할 변수값도 바꿔야 함
                                    if ( Check(2, 1) == 1 ) { checkx = j; checky = i; } //check인 경우 좌표 저장
                                }
                                Returncheckpan(); //다시 현재 판으로 돌려놓음
                                if (checkx >= 0) {*(*(pStr+(2*checky+1))+(4*checkx+2)) = '.'; }
                                //  checkx, checky에 인덱스 값이 저장되어있으면 * -> . (이동 못함)
                            }
                        }
                    }

                    // 선택된 말이 움질일 수 있는 경로가 있는지 확인
                    for(int i=0;i<8;i++) {
                        for(int j=0;j<8;j++) {
                            if(*(*(pStr+(2*i+1))+(4*j+2)) == '*')
                                F += 1; // F >= 1이면 말이 움질일 수 있다는 것을 의미
                        }
                    }

                    if(F!=0) {F=0; return 1;} // 이동할 수 있는 곳이 적어도 하나 있으면 checkmate아님
                    else {
                        for(int k=0;k<17;k++)
                            strcpy(pan[k], backpan[k]);
                    }
                }
            }
        }
        if (F==0) return F; // checkmate인 경우 return 0;
    }
}
