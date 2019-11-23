#include <stdio.h> //printf(). scanf(), fopen(), fscanf(), fprintf() 등 실행에 주가 되는 함수를 위함
#include <string.h> //체스판과 데이터를 저장하고 변경하는 과정에 있어서 필요한 strcpy(), strcat(), strcmp(), strlen()를 위함
#include <stdlib.h> //system("clear")를 위함
#include <termio.h> // getch() 함수를 정의하기 위해 필요한 tcgetattr(), ICANON, ECHO, VMIN, VTIME, tcsetattr(), TCSAFLUSH를 위함


char board[17][34]; //실행중인 체스판
char backupBoard[17][34]; //되돌리기 위한 백그라운드 저장용 체스판
char backupCheckBoard[17][34]; //check여부 확인 시 사용할 체스판
char boardFile[100] = "C:\\Users\\SoHyun Kim\\Desktop\\chess.txt"; // 새 게임용 체스판
char boardSaveFile[100] = "C:\\Users\\SoHyun Kim\\Desktop\\chess_save.txt"; // 저장된 체스판
char deadPiece1[20] = {""}, deadPiece2[20] = {""}; // 죽은 말을 저장할 배열
static int startPlayer; // 시작하는 player 확인용
static int winner; // 승리한 player 확인용 (1 : player 1 승, 2 : player 2 승)
static int countStar = 0; // 선택한 말이 움직일 수 있는 위치의 수, countStar == 0 -> 다시 말 선택
static int kingX1 = 14, kingY1 = 1; // player 1의 King 좌표 (check여부 확인시 이용)
static int kingX2 = 14, kingY2 = 15; // player 2의 King 좌표 (check여부 확인시 이용)
static int kingX3 = -1, kingY3 = -1; // King을 이동시킬 때 check 확인 좌표
static int longCastlingCheck[2] = {0,0}, shortCastlingCheck[2] = {0,0}; // 캐슬링 가능 여부 확인용
static int enPassant1[4]= {-1, -1, -1, -1}, enPassant2[4] = {-1, -1, -1, -1}; // 앙파상 가능 위치 확인용 좌표


void LoadChessGame(int); // 게임을 시작하기 전 필요한 과정
void PrintRule(void); // 도움말 출력
void PrintBoard(void); // 현재 판 출력
void BackupBoard(void); // 되돌리기용 판 저장
void BackupCheckBoard(void); // 되돌리기용 판 저장
void ReturnCheckBoard(void); // 판 되돌리기
void SaveGame(void); // 저장
void DeleteStar(void); // *표시 원래(. 또는 말)로 돌리기
void MovePiece1(void); // Player1 이동
void MovePiece2(void); // Player2 이동
void Pawn(int, int); // P 선택시 이동 가능 위치 *로 표시
void Rook(int, int); // R 선택시 이동 가능 위치 *로 표시
void Knight(int, int); // N 선택시 이동 가능 위치 *로 표시
void Bishop(int, int); // B 선택시 이동 가능 위치 *로 표시
void Queen(int, int); // Q 선택시 이동 가능 위치 *로 표시
void King(int, int); // K 선택시 이동 가능 위치 *로 표시
int Check(int, int); // check 여부 확인
int Checkmate(int); // 종료 조건
void Promotion(int, int, int, int); // 프로모션 (특수룰)
void Castling(int); // 캐슬링 (특수룰)
void Clear(void);
int getch(void);
void ChangePiece(int, int, char, char, char);


int main() {
    int menu = 0; // 메뉴 선택용 변수

    while (1) {
        Clear(); // 화면정리

        if (winner == 1 || winner == 2) {
            printf("\nPlayer %d WIN!\n\n", winner);
            winner = 0;
        } // 이전 판 승리자 표시 (처음 게임 실행 시 프린트 되지 않음)
        printf("+---- MENU ----+\n|  1. 새 게임  |\n|  2. 이어하기 |\n|  3. 도움말   |\n|   4. 종료    |\n+--------------+\n");
        printf("Meun Number : ");
        scanf("%d", &menu);
        Clear(); // 메뉴 선택

        if (menu == 1 || menu == 2)
        {
            if (menu == 1) { LoadChessGame(1); }
            else { LoadChessGame(2); }

            PrintRule();
            PrintBoard(); // 새 게임 판을 로드하여 프린트

            while (1)
            {
                if (startPlayer == 1)
                {
                    MovePiece1(); // player 1 이동

                    if (winner == 1 || winner == 2)
                    { // 만약 king이 잡히거나 checkmate 상태가 되거나 기권을 하면 게임 종료
                        break;
                    }
                    else
                    {
                        PrintBoard(); // 이동 후 변경된 체스판을 정리된 화면에 프린트
                        startPlayer = 2;
                    }
                }

                if (startPlayer == 2)
                {
                    MovePiece2(); // player 2 이동

                    if (winner == 1 || winner == 2)
                    { // 만약 king이 잡히거나 checkmate 상태가 되거나 기권을 하면 게임 종료
                        break;
                    }
                    else
                    {
                        PrintBoard(); // 이동 후 변경된 체스판을 정리된 화면에 프린트
                        startPlayer = 1;
                    }
                }
            }
        }
        else if (menu == 3) { PrintRule(); }
        else if (menu == 4) {
            printf("게임을 종료합니다. 감사합니다.");
            break; // while문 종료
        }
    }
    return 0;
}


/**
    함수 이름 : Clear
    함수 설명 : 화면을 깨끗하게 정리해준다.
    참조 함수들 : system("clear")
**/
void Clear()
{ system("clear"); }


/**
    함수 이름 : getch
    함수 설명 : 도움말 출력 시 사용자가 페이지를 넘기기 위해 누르는 엔터를 입력받기 위함 (getch 구현)
    파라미터 이름 :
    파라미터 설명
        파라미터 이름 :
    참조 함수들 :
**/
int getch()
{
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


/**
    함수 이름 : LoadChessGame
    함수 설명 : 게임을 시작하기 위해 필요한 과정을 수행한다. (텍스트 파일에서 체스판을 읽어 지정된 배열에 정보를 저장한다)
    파라미터 이름 : mode
    파라미터 설명
        mode : 1이면 새 게임, 2면 저장된 게임
    참조 함수들 : fopen(), fscanf(), strcpy(), fclose()
**/
void LoadChessGame(int  mode )
{
    FILE *fp = NULL; // 체스판이 저장된 텍스트 파일을 저장할 공간 선언
    char input[50]; // 파일 속 문자열을 읽어서 배열에 저장하기 위한 중간 문자열
    int I = -1; // while문 사용

    if (mode == 1) { fp = fopen(boardFile, "r"); } // 새 게임
    else
    { // 저장된 게임용 텍스트파일 실행
        fp = fopen(boardSaveFile, "r");

        if (fp == NULL)
        { // 만약 저장된 게임(체스판)이 없으면 새 게임 실행
            fp = fopen(boardFile, "r");
        }
    }

    while (fscanf(fp, "%s", input) != EOF)
    { // 파일이 끝날 때까지 한줄 씩 읽어서 input에 저장

        if (I == -1)
        {
            strcpy(deadPiece2, input);
            I++;
        }
        else if (I >= 0 && I < 17)
        {
            strcpy(board[I], input); // input에 저장된 문자열 체스판 배열 (board)에 저장
            I++; // 다음 줄을 읽고 저장하기 위함
        }
        else if (I == 17)
        {
            strcpy(deadPiece1, input);
            I++;
        }
        else if (I == 18)
        {
            startPlayer = input[0] - 48;
            longCastlingCheck[0] = input[2] - 48;
            longCastlingCheck[1] = input[4] - 48;
            shortCastlingCheck[0] = input[6] - 48;
            shortCastlingCheck[1] = input[8] - 48;
        }
    }
    fclose(fp); // 파일 닫기
}


/**
    함수 이름 : PrintRule
    함수 설명 : 게임진행 방법과 규칙을 출력한다.
    참조 함수들 : getch(), printf(), Clear();
**/
void PrintRule()
{
    getch();

    printf("\n게임 진행 방법\n");
    printf("\n<  > :Player 1, [  ] : Player 2\n");
    printf("\n1. 이동 시키길 원하는 말의 좌표를 입력한다.");
    printf("\n2. * 표시(이동 가능 공간) 중 원하는 곳의 좌표를 입력한다.\n\n\n(엔터를 눌러주세요)");

    getch(); // 사용자가 Enter 누르면 종료
    Clear();
}


/**
    함수 이름 : PrintBoard
    함수 설명 : 배열에 저장된
    참조 함수들 : printf()
**/
void PrintBoard(void)
{ // 체스판 출력
    int pnum = 0;
    Clear();
    printf("%s\n", deadPiece2); // player1이 잡은 말 출력
    printf("    A   B   C   D   E   F   G   H\n"); // 열 표시

    for (int i = 0; i < 17; i++)
    { // 체스판이 저장된 배열(board)을 한줄 씩 출력
        if (i % 2 == 0)
        {
            printf("  %s\n", board[i]);
        } // 행 표시
        else
        {
            printf("%d %s\n", pnum, board[i]);
            pnum++;
        }
    }
    printf("%s\n", deadPiece1); // player2가 잡은 말 출력
}


/**
    함수 이름 : BackupBoard
    함수 설명 : 현재 플레이 중인 게임판을 백업용 게임판에 저장한다.
    참조 함수들 : strcpy()
**/
void BackupBoard(void)
{ // 백그라운드 체스판에 현재 판 복사
    for (int i = 0; i < 17; i++)
    { strcpy(backupBoard[i], board[i]); } // 한줄 씩 board를 backupBoard에 복사한다
}


/**
    함수 이름 : BackupCheckBoard
    함수 설명 : 체크여부 확인시 임의로 이동시킨 말을 원래 위치로 되돌려놓기 위해 게임판을 백업한다.
    참조 함수들 : strcpy()
**/
void BackupCheckBoard(void)
{ // 체크여부확인 체스판에 현재 판 복사
    for (int i = 0; i < 17; i++)
    { strcpy(backupCheckBoard[i], board[i]); }
}


/**
    함수 이름 : ReturnCheckBoard
    함수 설명 : 체크여부 확인 후 게임판을 원상복귀 시킨다.
    참조 함수들 : strcpy()
**/
void ReturnCheckBoard(void)
{ // 현재 판에 체크여부확인 체스판 복사
    for (int i = 0; i < 17; i++)
    { strcpy(board[i], backupCheckBoard[i]); }
}


/**
    함수 이름 : DeleteStar
    함수 설명 : 이동 가능 위치를 표시를 위해 사용한 '*'을 말 이동 이후 원상복귀 시킨다.
    @ exception 예외처리
    //
**/
void DeleteStar(void)
{ // 이동 후 이동 가능 여부 체크했던 *표시 원래대로 돌려놓음
    char Back; // 돌려놓을 문자 저장용 변수

    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 49; j++)
        {
            if (board[i][j] == '*')
            { board[i][j] = backupBoard[i][j]; } // 모든 판을 체크하여 *표시를 backupBoard에 저장된 원래 문자로 돌려놓음
        }
    }
}


/**
    함수 이름 : ChangePiece
    함수 설명 : 말을 이동시킨다.
    파라미터 이름 : y, x, one, two, three
    파라미터 설명
        y, x : 이동시킬 말의 y, x좌표
        one, two, three : x좌표를 중심으로 바꿀 3개의 문자
**/
void ChangePiece(int y, int x, char one, char two, char three) {
    board[y][x-1] = one;
    board[y][x] = two;
    board[y][x+1] = three;
}


/**
    함수 이름 : MovePiece1
    함수 설명 : A - player1이 말을 움직이기 전 체크메이트 여부를 확인하여 체크메이트인 경우 패배로 인지하여 게임을 종료한다.
               B - player1이 이동을 원하는 말의 좌표를 입력받는다.
                  (SAVE 입력시 현재 게임 상황을 저장, GG 입력시 기권패 처리하여 게임을 종료한다.)
               C - 입력 받은 문자열을 각각 열과 행의 인덱스 번호로 변환한다.
                  (만약 입력받은 위치에 상대방의 말이 있거나 빈 공간인 경우 이동시킬 말의 좌표를 다시 입력받는다.)
               D - 선택한 말의 종류에 따라 정의된 함수를 이용하여 이동 가능한 위치를 표시한다.
               E - 이동이 가능한 위치 중 이동 후 체크 상태가 되는 곳은 이동을 제한한다.
               F - 이동 가능한 위치가 없다면 이동시킬 말의 좌표를 다시 입력받는다.
               G - 이동 시킬 위치의 좌표를 입력받아 이동가능 여부를 확인한 후 이동시키거나 좌표를 다시 입력받는다.
                  (상대방의 말을 잡은 경우 해당 배열에 정보를 저장하여 이후 출력시킬 수 있도록 한다.)
    참조 함수들 : printf(), scanf(), strcmp(), strlen(),
**/
void MovePiece1(void)
{ // player 1 이동함수
    char before[5], after[3]; // 이동시킬 말, 위치 입력받을 변수 ( save나 exit 입력받아야하는 경우가 있어서 크기 5 )
    int nowY = 0, nowX = 0, afterY = 0, afterX = 0; // 이동시킬 말의 행, 열, 이동시킬 위치의 행, 열
    int didCastling = 0, didEnPassant = 0; // 캐슬링 할 경우 1
    BackupBoard();

    // 이동시킬 말 입력받아 이동할 수 있는 경로 *로 표시하기
    while (1)
    {
        /** A **/
        if (Checkmate(1) == 0)
        { // 만약 player1이 움직일 수 있는 말이 없는 경우 player2 승리, 게임 종료
            winner = 2;
            break;
        }
        else
        {
            for (int k = 0; k < 17; k++)
            {
                strcpy(board[k], backupBoard[k]);
            }
        } // 아니면 다시 실행중인 판 복구

        /** B **/
        printf("\n< Player 1 >\n(SAVE: save game, GG: give up)\nWhat? : ");
        scanf("%s", before);

        if (strcmp(before, "SAVE") == 0)
        { // 사용자가 save입력 시 현재 체스판 텍스트 파일에 저장
            SaveGame();
            printf("S A V E . . . !\n");
            continue;
        }
        else if (strcmp(before, "GG") == 0)
        { // 사용자가 GG입력 시 기권패
            winner += 2;
            break;
        }
        else if (strlen(before) == 2)
        {   /** C **/
            nowY = 2 * (before[1] - 48) + 1; // 입력 받은 숫자(문자열 변수에 저장) 아스키 코드를 이용해서 행 인덱스로 변경
            nowX = 4 * (before[0] - 64) - 2; // 입력 받은 대문자 알파벳 아스키 코드를 이용해서 열 인덱스로 변경

            if ( (nowX < 0 || nowX > 32) || (nowY < 0 || nowY > 16)  )
            {
                printf("again!\n");
                continue;
            }

            if (board[nowY][(nowX - 1)] == '[')
            {
                printf("It is not yours\n\n");
                continue;
            } // 상대방 말 선택시 다시 선택

            if (board[nowY][nowX] == '.')
            {
                printf("There are nothing\n\n");
                continue;
            } // 비어있는 위치 선택시 다시 선택
            else
            { // 선택한 말의 종류에 따라 이동 가능한 곳 *으로 표시
                switch (board[nowY][nowX])
                {   /** D **/
                    case 'P':
                    { Pawn(nowY, nowX); break; }
                    case 'R':
                    { Rook(nowY, nowX); break; }
                    case 'N':
                    { Knight(nowY, nowX); break;}
                    case 'B':
                    { Bishop(nowY, nowX); break; }
                    case 'Q':
                    { Queen(nowY, nowX); break; }
                    case 'K':
                    { King(nowY, nowX); Castling(1);
                        break;
                    }
                }

                /** E **/
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        BackupCheckBoard(); // 확인이 끝난 후 돌려놓을 판을 위해 저장해놓음 (백업용)
                        int checkx = -1, checky = -1; // 이동을 금지시킬 행과 열 인덱스 저장용

                        if (board[2 * i + 1][4 * j + 2] == '*')
                        { // 이동 가능하다고 표시된 곳으로 이동시키기 (임시)
                            ChangePiece( 2*i+1, 4*j+2, '<', board[nowY][nowX], '>');
                            ChangePiece( nowY, nowX, '.', '.', '.');
                            DeleteStar();

                            if (board[2 * i + 1][4 * j + 2] != 'K')
                            {
                                if (Check(1, 0) == 1)
                                {
                                    checkx = 4 * j + 2;
                                    checky = 2 * i + 1;
                                } // check인 경우 좌표 저장
                            }
                            else
                            {
                                kingX3 = 4 * j + 2;
                                kingY3 = 2 * i + 1; // 움직일 말이 King이면 Check함수에서 이용할 변수값도 바꿔야 함

                                if (Check(1, 1) == 1)
                                {
                                    checkx = 4 * j + 2;
                                    checky = 2 * i + 1;
                                } // check인 경우 좌표 저장
                            }

                            ReturnCheckBoard(); //다시 현재 판으로 돌려놓음

                            if (checkx >= 0)
                            { // 만약 check인 경우가 있어서 checkx, checky에 인덱스 값이 저장되어있으면 * -> . (이동 못함)
                                board[checky][checkx] = '.';
                            }
                        }
                    }
                }

                /** F **/
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        if (board[2 * i + 1][4 * j + 2] == '*')
                        { countStar += 1; } // countStar >= 1 이면 이동 시킬 수 있다
                    }
                }
                if (countStar == 0)
                { // 이동시킬 수 있는 위치가 없으면 위치 다시 입력 받기
                    printf("You can not move that!\n\n");
                    continue;
                }
                else
                { // 이동 시킬 수 있는 위치가 있는 경우
                    countStar = 0; // 다음 이용을 위해 초기화
                    PrintBoard(); // 이동 가능 위치 표시된 체스판 출력
                    break;
                }
            }
        }
        else
        {
            printf("again\n");
            PrintBoard();
            continue;
        }
    }

    /** G **/
    while (1)
    {
        if (winner == 2) {break;} // 앞에서 exit을 입력받았거나 종료 조건이 만족된 경우 종료

        printf("\n< Player 1 >\nWhere? : ");
        scanf("%s", after);

        if (strlen(after) == 2)
        {
            afterY = 2 * (after[1] - 48) + 1; // 입력 받은 숫자(문자열 변수에 저장) 아스키 코드를 이용해서 행 인덱스로 변경
            afterX = 4 * (after[0] - 64) - 2; // 입력 받은 대문자 알파벳 아스키 코드를 이용해서 열 인덱스로 변경

            Promotion(nowX, nowY, afterY, 1); // 특수룰

            /**캐슬링 할 수 있는데 안하는 경우**/
            if (board[afterY][afterX] != 'a' ) {
                if (board[kingY1][kingX1 + 8] == 'a') {
                    ChangePiece(kingY1, kingX1 + 8, '.', '.', '.');
                }
                if (board[kingY1][kingX1 - 8] == 'a') {
                    ChangePiece(kingY1, kingX1 - 8, '.', '.', '.');
                }
            }

            /**앙파상 할 수 있는데 안하는 경우**/
            if (board[afterY][afterX] != 'n') {
                if ( board[enPassant1[0]][enPassant1[1]] == 'n') {
                    ChangePiece(enPassant1[0], enPassant1[1], '.', '.', '.');
                    enPassant1[0] = enPassant1[2] = -1;
                }
                if ( board[enPassant1[2]][enPassant1[3]] == 'n') {
                    ChangePiece(enPassant1[2], enPassant1[3], '.', '.', '.');
                    enPassant1[2] = enPassant1[3] = -1;
                }
            }

            /**캐슬링 하는 경우**/
            if (board[afterY][afterX] == 'a') {
                didCastling = 1;
                ChangePiece(afterY, afterX, '.', '.', '.');
                break;
            }
                /**앙파상 하는 경우**/
            else if (board[afterY][afterX] == 'n') {
                didEnPassant = 1;
                DeleteStar();
                break;
            }

            if (board[afterY][afterX] != '*')
            {
                printf("again\n");
                continue;
            } // 선택한 위치가 이동 불가능 한 위치면 다시 입력

            if (board[afterY][(afterX - 1)] == '<')
            {
                printf("again\n");
                continue;
            } // 본인 말이 있는 위치 선택 시 다시 입력
            else
            {
                DeleteStar();
                break;
            } // 이동할 위치 정해졌으니 *표시 모두 제거
        }
        else
        {
            printf("again\n");
            PrintBoard();
            continue;
        }
    }

    if (winner != 2)
    { // 게임이 종료되는 경우가 아니면 실행

        /**캐슬링 실행**/
        if (didCastling == 1) { //longcastling

            if (afterX == 22) {
                ChangePiece(1, 18, '<', 'R', '>');
                ChangePiece(1, 30, '.', '.', '.');
            }
            else if (afterX == 6) {
                ChangePiece(1, 10, '<', 'R', '>');
                ChangePiece(1, 2, '.', '.', '.');
            }
            DeleteStar();
        }

        /**앙파상 실행 시**/
        if (didEnPassant == 1) {
            strcat(deadPiece2, "[P]");
            ChangePiece( afterY - 2, afterX, '.', '.', '.');
        }

        /**앙파상으로 먹을 수 있는 좌표 저장**/
        if (board[nowY][nowX] == 'P' && afterY == nowY + 4) {
            if (board[afterY][afterX+3] == '[' && board[afterY][afterX+4] == 'P') {
                enPassant2[0] = afterY-2;
                enPassant2[1] = afterX;
            }
            if (board[afterY][afterX-5] == '[' && board[afterY][afterX-4] == 'P') {
                enPassant2[2] = afterY-2;
                enPassant2[3] = afterX;
            }
        }

        // 이동시킬 말이 King인 경우 변경될 좌표 저장 (for check확인)
        if (board[nowY][nowX] == 'K')
        {
            kingX1 = afterY;
            kingY1 = afterX;
            longCastlingCheck[0] = shortCastlingCheck[0] = 1;
        }

        // 왕 잡으면 end
        if (board[afterY][afterX] == 'K') {winner = 1;}

        // 상대말 말을 잡은 경우 체스판 프린트시 표시
        if (board[afterY][(afterX - 1)] == '[')
        {
            switch (backupBoard[afterY][afterX])
            { // "DIE-" 뒤에 이어서 저장 (strcat() 이용)
                case 'P':
                { strcat(deadPiece2, "[P]"); break; }
                case 'R':
                { strcat(deadPiece2, "[R]"); break; }
                case 'B':
                { strcat(deadPiece2, "[B]"); break; }
                case 'N':
                { strcat(deadPiece2, "[N]"); break; }
                case 'Q':
                { strcat(deadPiece2, "[Q]"); break; }
            }
        }

        // R 움직이면 그 이후 캐슬링 불가
        if (board[nowY][nowX] == 'R' && nowX == 2) {
            shortCastlingCheck[0] = 1;
        }
        else if (board[nowY][nowX] == 'R' && nowX == 30) {
            longCastlingCheck[0] = 1;
        }

        // 자리 옮기기
        ChangePiece(afterY, afterX, '<', board[nowY][nowX], '>');
        ChangePiece(nowY, nowX, '.', '.', '.');
    }

    if ( board[enPassant1[0]][enPassant1[1]] == 'n' ) {
        ChangePiece(enPassant1[0], enPassant1[1], '.', '.', '.');
        enPassant1[0] = enPassant1[1] =-1;
    }
    if ( board[enPassant1[2]][enPassant1[3]] == 'n' ) {
        ChangePiece(enPassant1[2], enPassant1[3], '.', '.', '.');
        enPassant1[2] = enPassant1[3] = -1;
    }

}


/**
    함수 이름 : MovePiece2
    함수 설명 : player2 차례에 실행시키며 MovePiece1과 실행방식이 동일하다.
    참조 함수들 : printf(), scanf(), strcmp(), strlen(),
**/
void MovePiece2(void)
{ // player 2 이동함수
    char before[5], after[3];  //이동시킬 말, 위치 입력받을 변수 (save나 exit 입력받아야하는 경우가 있어서 크기: 5)
    int afterY = 0, afterX = 0, nowY = 0, nowX = 0; //이동시킬 말의 행, 열, 이동시킬 위치의 행, 열
    int didCastling = 0, didEnPassant = 0;
    BackupBoard();

    // 이동시킬 말 입력받아 이동할 수 있는 경로 *로 표시하기
    while (1)
    {

        if (Checkmate(2) == 0)
        {
            winner = 1;
            break;
        } // 만약 player2가 움직일 수 있는 말이 없는 경우 player1 승리, 게임 종료

        for (int k = 0; k < 17; k++) { strcpy(board[k], backupBoard[k]); } // 아니면 다시 실행중인 판 복구

        printf("\n[ Player 2 ]\n(SAVE: save game, GG: give up)\nWhat? : ");
        scanf("%s", before);

        if (strcmp(before, "SAVE") == 0)
        { // 사용자가 save입력 시 현재 체스판 텍스트 파일에 저장
            SaveGame();
            printf("S A V E . . . !\n");
            continue;
        }
        else if (strcmp(before, "GG") == 0)
        { // 사용자가 exit입력 시 게임 종료 후 메뉴로 돌아감
            winner += 1;
            break;
        }
        else if (strlen(before) == 2)
        {
            nowY = 2 * (before[1] - 48) + 1; // 입력 받은 숫자(문자열 변수에 저장) 아스키 코드를 이용해서 행 인덱스로 변경
            nowX = 4 * (before[0] - 64) - 2; // 입력 받은 대문자 알파벳 아스키 코드를 이용해서 열 인덱스로 변경

            if ( (nowX < 0 || nowX > 32) || (nowY < 0 || nowY > 16)  )
            {
                printf("again!\n");
                continue;
            }

            if (board[nowY][(nowX - 1)] == '<')
            {
                printf("It is not yours\n\n");
                continue;
            } // 상대방 말 선택시 다시 선택

            if (board[nowY][nowX] == '.')
            {
                printf("There are nothing\n\n");
                continue;
            } // 비어있는 위치 선택시 다시 선택
            else
            {
                // 선택한 말의 종류에 따라 이동 가능한 곳 *으로 표시
                switch (board[nowY][nowX])
                {
                    case 'P':
                    { Pawn(nowY, nowX); break; }
                    case 'R':
                    { Rook(nowY, nowX); break; }
                    case 'N':
                    { Knight(nowY, nowX); break; }
                    case 'B':
                    { Bishop(nowY, nowX); break; }
                    case 'Q':
                    { Queen(nowY, nowX); break; }
                    case 'K':
                    { King(nowY, nowX); Castling(2);
                        shortCastlingCheck[1] = longCastlingCheck[1] = 1;
                        break;
                    }
                }

                // 이동 후 check 상태가 되는 곳은 .으로 변경
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        BackupCheckBoard(); // 확인이 끝난 후 돌려놓을 판을 위해 저장해놓음 (백업용)
                        int checkx = -1, checky = -1; // 이동을 금지시킬 행과 열 인덱스 저장용

                        if (board[2 * i + 1][4 * j + 2] == '*')
                        { // 이동 가능하다고 표시된 곳으로 이동시키기 (임시)
                            ChangePiece( 2*i+1, 4*j+2, '[', board[nowY][nowX], ']');
                            ChangePiece( nowY, nowX, '.', '.', '.');
                            DeleteStar();

                            if (board[2 * i + 1][4 * j + 2] != 'K')
                            {
                                if (Check(2, 0) == 1)
                                {
                                    checkx = 4 * j + 2;
                                    checky = 2 * i + 1;
                                } // check인 경우 좌표 저장
                            }
                            else
                            {
                                kingX3 = 4 * j + 2;
                                kingY3 = 2 * i + 1; // 움직일 말이 King이면 Check함수에서 이용할 변수값도 바꿔야 함

                                if (Check(2, 1) == 1)
                                {
                                    checkx = kingX3;
                                    checky = kingY3;
                                } // check인 경우 좌표 저장
                            }

                            ReturnCheckBoard(); // 다시 현재 판으로 돌려놓음

                            if (checkx >= 0) { board[checky][checkx] = '.'; }
                            // 만약 check인 경우가 있어서 checkx, checky에 인덱스 값이 저장되어있으면 * -> . (이동 금지 시키기)
                        }
                    }
                }

                // 선택된 말이 움질일 수 있는 경로가 있는지 확인
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        if (board[2 * i + 1][4 * j + 2] == '*')
                        {countStar += 1;} // countStar >= 1 이면 이동 시킬 수 있다
                    }
                }

                if (countStar == 0)
                { // 이동시킬 수 있는 위치가 없으면 위치 다시 입력 받기
                    printf("You can not move that!\n");
                    continue;
                }
                else
                { // 이동 시킬 수 있는 위치가 있는 경우
                    countStar = 0; //다음 이용을 위해 초기화
                    PrintBoard(); // 이동 가능 위치 표시된 체스판 출력
                    break;
                }
            }
        }
        else
        {
            printf("again\n");
            PrintBoard();
            continue;
        }
    }

    // 선택된 말 이동시킬 경로 입력받아 이동시키기
    while (1)
    {
        if (winner == 1) {break;}// 앞에서 exit을 입력받았거나 종료 조건이 만족된 경우 종료

        printf("\n[ Player 2 ]\nWhere? : ");
        scanf("%s", after);

        if (strlen(after) == 2)
        {
            afterY = 2 * (after[1] - 48) + 1; // 입력 받은 숫자(문자열 변수에 저장) 아스키 코드를 이용해서 행 인덱스로 변경
            afterX = 4 * (after[0] - 64) - 2; // 입력 받은 대문자 알파벳 아스키 코드를 이용해서 열 인덱스로 변경

            Promotion(nowX, nowY, afterY, 2);

            /**캐슬링 할 수 있는데 안하는 경우**/
            if (board[afterY][afterX] != 'a' ) {
                if (board[kingY2][kingX2 + 8] == 'a') {
                    ChangePiece(kingY2, kingX2 + 8, '.', '.', '.');
                }
                if (board[kingY2][kingX2 - 8] == 'a') {
                    ChangePiece(kingY2, kingX2 - 8, '.', '.', '.');
                }
            }

            /**앙파상 할 수 있는데 안하는 경우**/
            if (board[afterY][afterX] != 'n') {
                if ( board[enPassant2[0]][enPassant2[1]] == 'n') {
                    ChangePiece(enPassant2[0], enPassant2[1], '.', '.', '.');
                    enPassant2[0] = enPassant2[1] = -1;
                }
                if ( board[enPassant2[2]][enPassant2[3]] == 'n') {
                    ChangePiece(enPassant2[2], enPassant2[3], '.', '.', '.');
                    enPassant2[2] = enPassant2[3] = -1;
                }
            }

            /**캐슬링 하는 경우**/
            if (board[afterY][afterX] == 'a') {
                didCastling = 1;
                DeleteStar();
                break;
            }
                /**앙파상 하는 경우**/
            else if (board[afterY][afterX] == 'n') {
                didEnPassant = 1;
                DeleteStar();
                break;
            }

            if (board[afterY][afterX] != '*')
            {
                printf("again\n");
                continue;
            } // 선택한 위치가 이동 불가능 한 위치면 다시 입력

            if (board[afterY][(afterX - 1)] == '[')
            {
                printf("again\n");
                continue;
            } // 본인 말이 있는 위치 선택 시 다시 입력
            else
            {
                DeleteStar();
                break;
            } // 이동할 위치 정해졌으니 *표시 모두 제거
        }
        else
        {
            printf("again\n");
            PrintBoard();
            continue;
        }
    }

    if (winner != 1)
    { // 게임이 종료되는 경우가 아니면 실행

        /**캐슬링 실행 시**/
        if (didCastling == 1) {

            if (afterX == 22) {
                ChangePiece(15, 18, '[', 'R', ']');
                ChangePiece( 15, 30, '.', '.', '.');
            }
            else if (afterX == 6) {
                ChangePiece(15, 10, '[', 'R', ']');
                ChangePiece( 15, 2, '.', '.', '.');
            }
            DeleteStar();
        }

        /**앙파상 실행 시**/
        if (didEnPassant == 1) {
            strcat(deadPiece1, "<P>");
            ChangePiece( afterY + 2, afterX, '.', '.', '.');
        }

        /**앙파상으로 먹을 수 있는 좌표 저장**/
        if (board[nowY][nowX] == 'P' && afterY == nowY - 4) {
            if (board[afterY][afterX+3] == '<' && board[afterY][afterX+4] == 'P') {
                enPassant1[0] = afterY + 2;
                enPassant1[1] = afterX;
            }
            if (board[afterY][afterX-5] == '<' && board[afterY][afterX-4] == 'P') {
                enPassant1[2] = afterY + 2;
                enPassant1[3] = afterX;
            }
        }

        // 이동시킬 말이 King인 경우 변경될 좌표 저장 (for check확인)
        if (board[nowY][nowX] == 'K')
        {
            kingX2 = afterY;
            kingY2 = afterX;
            longCastlingCheck[1] = shortCastlingCheck[1] = 1;
        }

        // 왕 잡으면 end
        if (board[afterY][afterX] == 'K') winner = 2;

        // 자리 옮기기
        if (board[afterY][(afterX - 1)] == '<')
        { // 상대말 말을 잡은 경우 체스판 프린트 시 표시

            switch (backupBoard[afterY][afterX])
            { // "DIE-" 뒤에 이어서 저장 (strcat() 이용)
                case 'P':
                { strcat(deadPiece1, "<P>"); break; }
                case 'R':
                { strcat(deadPiece1, "<R>"); break; }
                case 'B':
                { strcat(deadPiece1, "<B>"); break; }
                case 'N':
                { strcat(deadPiece1, "<N>"); break; }
                case 'Q':
                { strcat(deadPiece1, "<Q>"); break; }
            }
        }

        /** R 움직이면 그 이후 캐슬링 불가 **/
        if (board[nowY][nowX] == 'R' && nowX == 2) {
            shortCastlingCheck[1] = 1;
        }
        else if (board[nowY][nowX] == 'R' && nowX == 30) {
            longCastlingCheck[1] = 1;
        }

        // 위치 변경
        ChangePiece(afterY, afterX, '[', board[nowY][nowX], ']');
        ChangePiece(nowY, nowX, '.', '.', '.');
    }

    if ( board[enPassant2[0]][enPassant2[1]] == 'n' ) {
        ChangePiece(enPassant2[0], enPassant2[1], '.', '.', '.');
        enPassant2[0] = enPassant2[1] = -1;
    }
    if ( board[enPassant2[2]][enPassant2[3]] == 'n' ) {
        ChangePiece(enPassant2[2], enPassant2[3], '.', '.', '.');
        enPassant2[2] = enPassant2[3] = -1;
    }

}


/**
    함수 이름 : Pawn
    함수 설명 : 이동시킬 말이 Pawn인 경우 이동 가능 위치에 '*'로 표시한다.
               Pawn은 이동하려는 위치에 말이 존재하지 않을 경우 한 칸(처음 이동시키는 경우 두 칸)전진 가능하다.
               대각선(한 칸 차이)에 상대방의 말이 있을 경우 그 위치로 이동하여 잡는 것이 가능하다.
    파라미터 이름 : nowY, nowX
    파라미터 설명
        nowY : 이동시킬 pawn의 현재 y좌표
        nowX : 이동시킬 pawn의 현재 x좌표
    참조 함수들 : fopen(), fscanf(), strcpy(), fclose()
    @ exception 예외처리
    //
**/
void Pawn(int nowY, int nowX)
{ // Pawn 선택 시 이동 가능 위치 *로 표시

    if (board[nowY][(nowX - 1)] == '<')
    { // player1의 경우

        if (board[nowY + 2][nowX] == '.')
        { // Pawn은 앞 칸이 이어있는 경우 앞으로 한 칸 전진(행 인덱스 2 증가) 가능 하다
            board[nowY + 2][nowX] = '*';

            if (nowY == 3)
            { // 처음 이동 시키는 경우 두 칸 전진도 가능하다
                board[nowY + 4][nowX] = '*';
            }
        }

        if (board[nowY + 2][nowX - 4] != '.' && board[nowY + 2][nowX - 5] == '[')
        { // 한 칸 대각선에 상대말 말이 있는 경우 이동하여 잡는 것이 가능하다
            board[nowY + 2][nowX - 4] = '*';
        }

        if (board[nowY + 2][nowX + 4] != '.' && board[nowY + 2][nowX + 5] == ']')
        { // 한 칸 대각선에 상대말 말이 있는 경우 이동하여 잡는 것이 가능하다
            board[nowY + 2][nowX + 4] = '*';
        }

        /** 앙파상 **/
        if (enPassant1[0] >= 0 && nowX == enPassant1[1] + 4 && nowY == enPassant1[0] + 2) {
            ChangePiece(enPassant1[0], enPassant1[1], 'E', 'n', 'P');
        }

        if (enPassant1[2] >= 0 && nowX == enPassant1[3] - 4 && nowY == enPassant1[2] + 2)  {
            ChangePiece(enPassant1[2], enPassant1[3], 'E', 'n', 'P');
        }
    }

    else
    { // player2의 경우

        if (board[nowY - 2][nowX] == '.')
        { // Pawn은 앞 칸이 이어있는 경우 앞으로 한 칸 전진(행 인덱스 2 감소) 가능 하다
            board[nowY - 2][nowX] = '*';

            if (nowY == 13)
            {
                board[nowY - 4][nowX] = '*';
            } // 처음 이동 시키는 경우 두 칸 전진도 가능하다
        }

        if (board[nowY - 2][nowX - 4] != '.' && board[nowY - 2][nowX - 5] == '<')
        { // 한 칸 대각선에 상대말 말이 있는 경우 이동하여 잡는 것이 가능하다
            board[nowY - 2][nowX - 4] = '*';
        }

        if (board[nowY - 2][nowX + 4] != '.' && board[nowY - 2][nowX + 3] == '<')
        { // 한 칸 대각선에 상대말 말이 있는 경우 이동하여 잡는 것이 가능하다
            board[nowY - 2][nowX + 4] = '*';
        }

        /** 앙파상 **/
        if (enPassant2[0] >= 0 && nowX == enPassant2[1] + 4 && nowY == enPassant2[0] - 2) {
            ChangePiece(enPassant2[0], enPassant2[1], 'E', 'n', 'P');
        }

        if (enPassant2[2] >= 0 && nowX == enPassant2[3] - 4 && nowY == enPassant2[2] - 2) {
            ChangePiece(enPassant2[2], enPassant2[3], 'E', 'n', 'P');
        }
    }
}


/**
    함수 이름 : Rook
    함수 설명 : 이동시킬 말이 Rook인 경우 이동 가능 위치에 '*'로 표시한다.
               Rook은 이동하려는 경로에 말이 존재하지 않을 경우 사방으로 이동이 가능하다.
    파라미터 이름 : nowY, nowX
    파라미터 설명
        nowY : 이동시킬 Rook의 현재 y좌표
        nowX : 이동시킬 Rook의 현재 x좌표
    참조 함수들 : fopen(), fscanf(), strcpy(), fclose()
    @ exception 예외처리
    //
**/
void Rook(int nowY, int nowX)
{ // Rook 선택 시 이동 가능 위치 *로 표시
    int copyNowY = nowY, copyNowX = nowX; // 변동이 있을 때 초기화를 위함

    if (board[copyNowY][copyNowX - 1] == '<')
    { // Player 1의 경우

        while (1)
        { // 앞으로 이동 가능
            copyNowY += 2;

            if (copyNowY > 16) {break;} // 체스판 끝까지 확인
            else if (board[copyNowY][copyNowX] == '.') {board[copyNowY][copyNowX] = '*';} // 비어있는 칸이면 계속 이동 가능
            else if (board[copyNowY][copyNowX - 1] == '[')
            {
                // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;}// 본인 말이 있으면 그 칸부터 이동 불가능
        }

        copyNowY = nowY; // 다시 원래 자리로

        while (1)
        { // 뒤로 이동 가능

            copyNowY -= 2;

            if (copyNowY < 0) {break;} // 체스판 끝까지 확인
            else if (board[copyNowY][copyNowX] == '.') {board[copyNowY][copyNowX] = '*';} // 비어있는 칸이면 계속 이동 가능
            else if (board[copyNowY][copyNowX - 1] == '[')
            {
                // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }

        copyNowY = nowY; // 다시 원래 자리로

        while (1)
        { // 오른쪽으로 이동 가능
            copyNowX += 4;

            if (copyNowX > 33) {break;} // 체스판 끝까지 확인
            else if (board[copyNowY][copyNowX] == '.') {board[copyNowY][copyNowX] = '*';} // 비어있는 칸이면 계속 이동 가능
            else if (board[copyNowY][copyNowX - 1] == '[')
            {
                // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }

        copyNowX = nowX; // 다시 원래 자리로

        while (1)
        { // 왼쪽으로 이동 가능
            copyNowX -= 4;

            if (copyNowX < 0) {break;} // 체스판 끝까지 확인
            else if (board[copyNowY][copyNowX] == '.') {board[copyNowY][copyNowX] = '*';} // 비어있는 칸이면 계속 이동 가능
            else if (board[copyNowY][copyNowX - 1] == '[')
            {
                // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }
    }

    else
    { // Player 2의 경우

        while (1)
        { // 뒤로 이동 가능
            copyNowY += 2;

            if (copyNowY > 16) {break;} // 체스판 끝까지 확인
            else if (board[copyNowY][copyNowX] == '.') {board[copyNowY][copyNowX] = '*';} // 비어있는 칸이면 계속 이동 가능
            else if (board[copyNowY][copyNowX - 1] == '<')
            {
                // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }

        copyNowY = nowY; // 다시 원래 자리로

        while (1)
        { // 앞으로 이동 가능
            copyNowY -= 2;

            if (copyNowY < 0) {break;} // 체스판 끝까지 확인
            else if (board[copyNowY][copyNowX] == '.') {board[copyNowY][copyNowX] = '*';} // 비어있는 칸이면 계속 이동 가능
            else if (board[copyNowY][copyNowX - 1] == '<')
            {
                // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }

        copyNowY = nowY; // 다시 원래 자리로

        while (1)
        { // 오른쪽으로 이동 가능
            copyNowX += 4;

            if (copyNowX > 33) {break;} // 체스판 끝까지 확인
            else if (board[copyNowY][copyNowX] == '.') {board[copyNowY][copyNowX] = '*';} // 비어있는 칸이면 계속 이동 가능
            else if (board[copyNowY][copyNowX - 1] == '<')
            {
                // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }

        copyNowX = nowX; // 다시 원래 자리로

        while (1)
        { // 왼쪽으로 이동 가능
            copyNowX -= 4;

            if (copyNowX < 0) {break;} // 체스판 끝까지 확인
            else if (board[copyNowY][copyNowX] == '.') {board[copyNowY][copyNowX] = '*';} // 비어있는 칸이면 계속 이동 가능
            else if (board[copyNowY][copyNowX - 1] == '<')
            {
                // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }
    }
}


/**
    함수 이름 : Bishop
    함수 설명 : 이동시킬 말이 Bishop인 경우 이동 가능 위치에 '*'로 표시한다.
               Bishop은 이동하려는 경로에 말이 존재하지 않을 경우 대각선으로 이동이 가능하다.
    파라미터 이름 : nowY, nowX
    파라미터 설명
        nowY : 이동시킬 Bishop의 현재 y좌표
        nowX : 이동시킬 Bishop의 현재 x좌표
    참조 함수들 : fopen(), fscanf(), strcpy(), fclose()
    @ exception 예외처리
    //
**/
void Bishop(int nowY, int nowX)
{ // Bishop 선택 시 이동 가능 위치 *로 표시
    int copyNowY = nowY, copyNowX = nowX; // 변동이 있을 때 초기화를 위함

    if (board[copyNowY][copyNowX - 1] == '<')
    { // player1의 경우

        while (1)
        { // ↘
            copyNowY += 2;
            copyNowX += 4;

            if (copyNowY > 16 || copyNowX > 33) {break;} // 체스판 끝까지 확인
            else if (board[copyNowY][copyNowX] == '.')
            { // 비어있는 칸이면 계속 이동 가능
                board[copyNowY][copyNowX] = '*';
                continue;
            }
            else if (board[copyNowY][copyNowX - 1] == '[')
            { // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }
        copyNowY = nowY;
        copyNowX = nowX; // 다시 원래 자리로

        while (1)
        { // ↙
            copyNowY += 2;
            copyNowX -= 4;

            if (copyNowY > 16 || copyNowX < 0) {break;} // 체스판 끝까지 확인
            else if (board[copyNowY][copyNowX] == '.')
            { // 비어있는 칸이면 계속 이동 가능
                board[copyNowY][copyNowX] = '*';
                continue;
            }
            else if (board[copyNowY][copyNowX - 1] == '[')
            { // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }
        copyNowY = nowY;
        copyNowX = nowX; // 다시 원래 자리로

        while (1)
        { // ↗
            copyNowY -= 2;
            copyNowX += 4;

            if (copyNowY < 0 || copyNowX > 33) {break;} // 체스판 끝까지 확인
            else if (board[copyNowY][copyNowX] == '.')
            { // 비어있는 칸이면 계속 이동 가능
                board[copyNowY][copyNowX] = '*';
                continue;
            }
            else if (board[copyNowY][copyNowX - 1] == '[')
            { // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }
        copyNowY = nowY;
        copyNowX = nowX; // 다시 원래 자리로

        while (1)
        { // ↖
            copyNowY -= 2;
            copyNowX -= 4;

            if (copyNowY < 0 || copyNowX < 0) {break;} // 체스판 끝까지 확인
            else if (board[copyNowY][copyNowX] == '.')
            { // 비어있는 칸이면 계속 이동 가능
                board[copyNowY][copyNowX] = '*';
                continue;
            }
            else if (board[copyNowY][copyNowX - 1] == '[')
            { // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }
    }

    else
    { //player2의 경우

        while (1)
        { // ↘
            copyNowY += 2;
            copyNowX += 4;

            if (copyNowY > 16 || copyNowX > 33) {break;}
            else if (board[copyNowY][copyNowX] == '.')
            { // 비어있는 칸이면 계속 이동 가능
                board[copyNowY][copyNowX] = '*';
                continue;
            }
            else if (board[copyNowY][copyNowX - 1] == '<')
            { // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }
        copyNowY = nowY;
        copyNowX = nowX; // 다시 원래 자리로

        while (1)
        { // ↙
            copyNowY += 2;
            copyNowX -= 4;

            if (copyNowY > 16 || copyNowX < 0) {break;}
            else if (board[copyNowY][copyNowX] == '.')
            { // 비어있는 칸이면 계속 이동 가능
                board[copyNowY][copyNowX] = '*';
                continue;
            }
            else if (board[copyNowY][copyNowX - 1] == '<')
            { // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }
        copyNowY = nowY;
        copyNowX = nowX; // 다시 원래 자리로

        while (1)
        { // ↗
            copyNowY -= 2;
            copyNowX += 4;

            if (copyNowY < 0 || copyNowX > 33) {break;}
            else if (board[copyNowY][copyNowX] == '.')
            { // 비어있는 칸이면 계속 이동 가능
                board[copyNowY][copyNowX] = '*';
                continue;
            }
            else if (board[copyNowY][copyNowX - 1] == '<')
            { // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }
        copyNowY = nowY;
        copyNowX = nowX; // 다시 원래 자리로

        while (1)
        { // ↖
            copyNowY -= 2;
            copyNowX -= 4;

            if (copyNowY < 0 || copyNowX < 0) {break;}
            else if (board[copyNowY][copyNowX] == '.')
            { // 비어있는 칸이면 계속 이동 가능
                board[copyNowY][copyNowX] = '*';
                continue;
            }
            else if (board[copyNowY][copyNowX - 1] == '<')
            { // 상대방 말이 있으면 그 칸까지 이동 가능
                board[copyNowY][copyNowX] = '*';
                break;
            }
            else {break;} // 본인 말이 있으면 그 칸부터 이동 불가능
        }
    }
}


/**
    함수 이름 : Knight
    함수 설명 : 이동시킬 말이 Knight인 경우 이동 가능 위치에 '*'로 표시한다.
               Knight은 직선한칸, 대각선 한칸으로 한번에 이동이 가능하다.
               이동 경로에 말이 있어도 상관없이 넘어서 이동할 수 있다.
    파라미터 이름 : nowY, nowX
    파라미터 설명
        nowY : 이동시킬 Knight의 현재 y좌표
        nowX : 이동시킬 Knight의 현재 x좌표
    참조 함수들 : fopen(), fscanf(), strcpy(), fclose()
    @ exception 예외처리
    //
**/
void Knight(int nowY, int nowX)
{ // Knight 선택 시 이동 가능 위치 *로 표시
    // 비어있는 칸이거나 상대방 말이 있는 경우 이동 가능

    if (board[nowY][nowX - 1] == '<')
    { // player1의 경우 (상대방의 말이 있거나 비어있으면 이동가능)
        if (board[nowY + 4][nowX + 3] == '[' || board[nowY + 4][nowX + 3] == '.')
        { board[nowY + 4][nowX + 4] = '*'; } // ↓ ↓ -→

        if (board[nowY + 4][nowX - 5] == '[' || board[nowY + 4][nowX - 5] == '.')
        { board[nowY + 4][nowX - 4] = '*'; } // ←- ↓ ↓

        if (board[nowY + 2][nowX + 7] == '[' || board[nowY + 2][nowX + 7] == '.')
        { board[nowY + 2][nowX + 8] = '*'; } // ↓ -→ -→

        if (board[nowY + 2][nowX - 9] == '[' || board[nowY + 2][nowX - 9] == '.')
        { board[nowY + 2][nowX - 8] = '*'; } // ←- ←- ↓

        if (board[nowY - 4][nowX + 3] == '[' || board[nowY - 4][nowX + 3] == '.')
        { board[nowY - 4][nowX + 4] = '*'; } // ↑ ↑ -→

        if (board[nowY - 4][nowX - 5] == '[' || board[nowY - 4][nowX - 5] == '.')
        { board[nowY - 4][nowX - 4] = '*'; } // ←- ↑ ↑

        if (board[nowY - 2][nowX + 7] == '[' || board[nowY - 2][nowX + 7] == '.')
        { board[nowY - 2][nowX + 8] = '*'; } // ↑ -→ -→

        if (board[nowY - 2][nowX - 9] == '[' || board[nowY - 2][nowX - 9] == '.')
        { board[nowY - 2][nowX - 8] = '*'; } // ←- ←- ↑
    }

    else
    { // player2의 경우 (상대방의 말이 있거나 비어있으면 이동가능)

        if (board[nowY + 4][nowX + 3] == '<' || board[nowY + 4][nowX + 3] == '.')
        { board[nowY + 4][nowX + 4] = '*'; } // ↓ ↓ -→

        if (board[nowY + 4][nowX - 5] == '<' || board[nowY + 4][nowX - 5] == '.')
        { board[nowY + 4][nowX - 4] = '*'; } // ←- ↓ ↓

        if (board[nowY + 2][nowX + 7] == '<' || board[nowY + 2][nowX + 7] == '.')
        { board[nowY + 2][nowX + 8] = '*'; } // ↓ -→ -→

        if (board[nowY + 2][nowX - 9] == '<' || board[nowY + 2][nowX - 9] == '.')
        { board[nowY + 2][nowX - 8] = '*'; } // ←- ←- ↓

        if (board[nowY - 4][nowX + 3] == '<' || board[nowY - 4][nowX + 3] == '.')
        { board[nowY - 4][nowX + 4] = '*'; } // ↑ ↑ -→

        if (board[nowY - 4][nowX - 5] == '<' || board[nowY - 4][nowX - 5] == '.')
        { board[nowY - 4][nowX - 4] = '*'; } // ←- ↑ ↑

        if (board[nowY - 2][nowX + 7] == '<' || board[nowY - 2][nowX + 7] == '.')
        { board[nowY - 2][nowX + 8] = '*'; } // ↑ -→ -→

        if (board[nowY - 2][nowX - 9] == '<' || board[nowY - 2][nowX - 9] == '.')
        { board[nowY - 2][nowX - 8] = '*'; } // ←- ←- ↑
    }

}


/**
    함수 이름 : Queen
    함수 설명 : 이동시킬 말이 Queen인 경우 이동 가능 위치에 '*'로 표시한다.
               Queen은 이동하려는 경로에 말이 있지 않는 한 사방, 대각선으로 이동이 가능하다.
    파라미터 이름 : nowY, nowX
    파라미터 설명
        nowY : 이동시킬 Queen의 현재 y좌표
        nowX : 이동시킬 Queen의 현재 x좌표
    참조 함수들 : Rook(), Bishop()
    @ exception 예외처리
    //
**/
void Queen(int nowY, int nowX)
{ // Queen 선택 시 이동 가능 위치 *로 표시
    Rook(nowY, nowX); // 사방으로 이동 가능 = Rook
    Bishop(nowY, nowX); // 대각선으로 이동 가능 = Bishop
}


/**
    함수 이름 : King
    함수 설명 : 이동시킬 말이 King인 경우 이동 가능 위치에 '*'로 표시한다.
               King은 이동하려는 경로에 말이 있지 않는 한 사방, 대각선으로 한 칸 이동이 가능하다.
    파라미터 이름 : nowY, nowX
    파라미터 설명
        nowY : 이동시킬 King의 현재 y좌표
        nowX : 이동시킬 King의 현재 x좌표
    참조 함수들 :
    @ exception 예외처리
    //
**/
void King(int nowY, int nowX)
{ // King 선택 시 이동 가능 위치 *로 표시
    if (board[nowY][nowX - 1] == '<')
    { //player1의 경우
        if (board[nowY + 2][nowX - 1] != '<') {board[nowY + 2][nowX] = '*';} // ↓

        if (board[nowY - 2][nowX - 1] != '<') {board[nowY - 2][nowX] = '*';} // ↑

        if (board[nowY][nowX + 3] != '<') {board[nowY][nowX + 4] = '*';} // -→

        if (board[nowY][nowX - 5] != '<') {board[nowY][nowX - 4] = '*';} // ←-

        if (board[nowY + 2][nowX + 3] != '<') {board[nowY + 2][nowX + 4] = '*';} // ↓ -→

        if (board[nowY + 2][nowX - 5] != '<') {board[nowY + 2][nowX - 4] = '*';} // ←- ↓

        if (board[nowY - 2][nowX + 3] != '<') {board[nowY - 2][nowX + 4] = '*';} // ↑ -→

        if (board[nowY - 2][nowX - 5] != '<') {board[nowY - 2][nowX - 4] = '*';} // ←- ↑
    }

    else
    { //player2의 경우

        if (board[nowY + 2][nowX - 1] != '[') {board[nowY + 2][nowX] = '*';} // ↓

        if (board[nowY - 2][nowX - 1] != '[') {board[nowY - 2][nowX] = '*';} // ↑

        if (board[nowY][nowX + 3] != '[') {board[nowY][nowX + 4] = '*';} // -→

        if (board[nowY][nowX - 5] != '[') {board[nowY][nowX - 4] = '*';} // ←-

        if (board[nowY + 2][nowX + 3] != '[') {board[nowY + 2][nowX + 4] = '*';} // ↓ -→

        if (board[nowY + 2][nowX - 5] != '[') {board[nowY + 2][nowX - 4] = '*';} // ←- ↓

        if (board[nowY - 2][nowX + 3] != '[') {board[nowY - 2][nowX + 4] = '*';} // ↑ -→

        if (board[nowY - 2][nowX - 5] != '[') {board[nowY - 2][nowX - 4] = '*';} // ←- ↑
    }
}


/**
    함수 이름 : SaveGame
    함수 설명 : 현재 게임 상황(게임보드, 순서, 죽은 말)을 지정한 파일에 저장한다. (파일이 존재하지 않는 경우 생성하여 저장한다.)
    참조 함수들 : fopen(), fprintf(), fclose()
    @ exception 예외처리
    //
**/
void SaveGame(void)
{ // 현재 실행중인 체스판 저장
    FILE *fp = NULL; // 저장용 텍스트 파일 저장할 공간
    fp = fopen(boardSaveFile, "w+"); // chess_save.txt파일 열기 (없으면 만들어서 열기)

    for (int i = -1; i < 19; i++)
    { // 한줄씩 파일에 입력하기

        if (i == -1) {
            fprintf(fp, "%s \n", deadPiece2);
        }
        else if (i == 17) {
            fprintf(fp, "%s \n", deadPiece1);
        }
        else if (i == 18) {
            fprintf(fp, "%d,%d,%d,%d,%d", startPlayer, longCastlingCheck[0], longCastlingCheck[1],
                    shortCastlingCheck[0], shortCastlingCheck[1]);
        }
        else {
            fprintf(fp, "%s \n", board[i]);
        }
    }
    fclose(fp); // 파일 닫기
}


/**
    함수 이름 : Check
    함수 설명 : 현재 왕의 좌표를 기준으로 체크 상태 여부를 확인한다.
               1. 사방 기준으로 상대방의 말 Q 또는 R이 있는 경우 체크이다.
               2. 대각선 기준으로 상대방의 말 B 또는 Q가 있는 경우 체크이다.
                  (한 칸 대각선 위치에 상대방의 말 P가 있는 경우 체크이다.)
               3. 상대방 말의 Knight에 의한 체크여부를 확인한다.
    파라미터 이름 : player, king
    파라미터 설명
        player : 체크 여부를 확인하는 player ( player는 1 또는 2)
        king : 이동 시킬 말이 King인 경우 이동 후 좌표를 이용하여 체크 여부를 확인하기 위해 이동 여부를 알린다. (king 이동 시 1)
    참조 함수들 :
    @ exception 예외처리
    //
**/
int Check(int player, int king)
{ // 모든 말이 움직일 수 있는 경우의 수를 계산하고 그 중 check상태가 될 수 있는 경우가 있는지 확인하기
    int c = 0; // check가 될 수 있는 경우의 수
    int x, y;

    if (player == 1)
    { // player1의 경우

        if (king == 1) { x = kingX3, y = kingY3; } // 현재 king의 좌표 저장
        else { x = kingX1, y = kingY1; } // 현재 king의 좌표 저장

        // 오른쪽
        for (int i = (x + 4); i < 34; i += 4)
        { // 체스판 끝까지 확인
            if (board[y][i - 1] == '<') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[y][i - 1] == '[')
            { // 상대방 R이나 Q가 있으면 check
                if (board[y][i] == 'R' || board[y][i] == 'Q')
                {
                    c++;
                    break;
                }
            }
        }

        //왼쪽
        for (int i = (x - 4); i >= 0; i -= 4)
        { // 체스판 끝까지 확인
            if (board[y][i - 1] == '<') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[y][i - 1] == '[')
            { // 상대방 R이나 Q가 있으면 check
                if (board[y][i] == 'R' || board[y][i] == 'Q')
                {
                    c++;
                    break;
                }
            }
        }

        //위
        for (int i = (y + 2); i < 17; i += 2)
        { // 체스판 끝까지 확인
            if (board[i][x - 1] == '<') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[i][x - 1] == '[')
            { // 상대방 R이나 Q가 있으면 check
                if (board[i][x] == 'R' || board[i][x] == 'Q')
                {
                    c++;
                    break;
                }
            }
        }

        //아래
        for (int i = (y - 2); i >= 0; i -= 2)
        { // 체스판 끝까지 확인
            if (board[i][x - 1] == '<') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[i][x - 1] == '[')
            { // 상대방 R이나 Q가 있으면 check
                if (board[i][x] == 'R' || board[i][x] == 'Q')
                {
                    c++;
                    break;
                }
            }
        }

        //대각선1
        int pp = 0; // pawn 확인용 (대각선 기준 몇 칸 차이인지)
        while (1)
        {
            x += 4;
            y += 2;
            pp++;

            if (board[y][x - 1] == '<') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[y][x - 1] == '[')
            {
                if (board[y][x] == 'P' && pp == 1)
                { // 한 칸 대각선에 상대방 P가 있으면 check
                    c++;
                    break;
                }
                else if (board[y][x] == 'Q' || board[y][x] == 'B')
                { // 상대방 B나 Q가 있으면 check
                    c++;
                    break;
                }
            }
            if (x > 33 || y > 16) { break; } // 체스판 끝까지 확인
        }

        if (king == 1) { x = kingX3, y = kingY3; }
        else { x = kingX1, y = kingY1; } // 좌표 원래대로 돌려놓기

        //대각선2
        pp = 0; // pawn 확인용 (대각선 기준 몇 칸 차이인지)
        while (1)
        {
            x -= 4;
            y += 2;
            pp++;

            if (board[y][x - 1] == '<') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[y][x - 1] == '[')
            {
                if (board[y][x] == 'P' && pp == 1)
                { // 한 칸 대각선에 상대방 P가 있으면 check
                    c++;
                    break;
                }
                else if (board[y][x] == 'Q' || board[y][x] == 'B')
                { // 상대방 B나 Q가 있으면 check
                    c++;
                    break;
                }
            }

            if (x < 0 || y > 16) {break;} // 체스판 끝까지 확인
        }

        if (king == 1) { x = kingX3, y = kingY3; }
        else { x = kingX1, y = kingY1; } // 좌표 원래대로 돌려놓기

        // 대각선3
        while (1)
        {
            x -= 4;
            y -= 2;

            if (board[y][x - 1] == '<') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[y][x - 1] == '[')
            { // 상대방 B나 Q가 있으면 check
                if (board[y][x] == 'Q' || board[y][x] == 'B')
                {
                    c++;
                    break;
                }
            }

            if (x < 0 || y < 0) {break;} // 체스판 끝까지 확인
        }

        if (king == 1) { x = kingX3, y = kingY3; }
        else { x = kingX1, y = kingY1; } // 좌표 원래대로 돌려놓기

        //대각선4
        while (1)
        {
            x += 4;
            y -= 2;

            if (board[y][x - 1] == '<') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[y][x - 1] == '[')
            { // 상대방 B나 Q가 있으면 check
                if (board[y][x] == 'Q' || board[y][x] == 'B')
                {
                    c++;
                    break;
                }
            }
            if (x > 33 || y < 0) break; // 체스판 끝까지 확인
        }

        if (king == 1) { x = kingX3, y = kingY3; }
        else { x = kingX1, y = kingY1; } // 좌표 원래대로 돌려놓기

        //knight
        if (board[y + 4][x + 3] == '[' && board[y + 4][x + 4] == 'N') {
            if ((x + 3) < 33 && (y + 4) < 17) {c++;} // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if (board[y + 4][x - 5] == '[' && board[y + 4][x - 4] == 'N') {
            if ((x - 5) >= 0 && (y + 4) < 17) {c++;} // ←- ↓ ↓ 위치에 상대방 N있으면 check
        }
        if (board[y - 2][x - 9] == '[' && board[y - 2][x - 8] == 'N') {
            if ((x - 9) >= 0 && (y - 2) >= 0) {c++;} // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if (board[y + 2][x + 7] == '[' && board[y + 2][x + 8] == 'N') {
            if ((x + 7) < 33 && (y + 2) < 17) {c++;} // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if (board[y + 2][x - 9] == '[' && board[y + 2][x - 8] == 'N') {
            if ((x - 9) >= 0 && (y + 2) < 17) {c++;} // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if (board[y - 4][x + 3] == '[' && board[y - 4][x + 4] == 'N') {
            if ((x + 3) < 33 && (y - 4) >= 0) {c++;} // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if (board[y - 4][x - 5] == '[' && board[y - 4][x - 4] == 'N') {
            if ((x - 5) >= 0 && (y - 4) >= 0) {c++;} // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if (board[y - 2][x + 7] == '[' && board[y - 2][x + 8] == 'N') {
            if ((x + 7) < 33 && (y - 2) >= 0) {c++;} // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
    }
    else
    { // player2의 경우
        if (king == 1) { x = kingX3, y = kingY3; }
        else { x = kingX2, y = kingY2; } // 현재 king의 좌표 저장

        //오른쪽
        for (int i = (x + 4); i < 34; i += 4)
        { // 체스판 끝까지 확인
            if (board[y][i - 1] == '[') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[y][i - 1] == '<') { // 상대방 R이나 Q가 있으면 check
                if (board[y][i] == 'R' || board[y][i] == 'Q')
                {
                    c++;
                    break;
                }
            }
        }

        //왼쪽
        for (int i = (x - 4); i >= 0; i -= 4)
        { // 체스판 끝까지 확인
            if (board[y][i - 1] == '[') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[y][i - 1] == '<')
            { // 상대방 R이나 Q가 있으면 check
                if (board[y][i] == 'R' || board[y][i] == 'Q')
                {
                    c++;
                    break;
                }
            }
        }

        //위
        for (int i = (y + 2); i < 17; i += 2)
        { // 체스판 끝까지 확인
            if (board[i][x - 1] == '[') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[i][x - 1] == '<')
            { // 상대방 R이나 Q가 있으면 check
                if (board[i][x] == 'R' || board[i][x] == 'Q')
                {
                    c++;
                    break;
                }
            }
        }

        //아래
        for (int i = (y - 2); i >= 0; i -= 2)
        { // 체스판 끝까지 확인
            if (board[i][x - 1] == '[') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[i][x - 1] == '<')
            { // 상대방 R이나 Q가 있으면 check
                if (board[i][x] == 'R' || board[i][x] == 'Q')
                {
                    c++;
                    break;
                }
            }
        }

        //대각선1
        while (1)
        {
            x += 4;
            y += 2;

            if (board[y][x - 1] == '[') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[y][x - 1] == '<')
            { // 상대방 B나 Q가 있으면 check
                if (board[y][x] == 'Q' || board[y][x] == 'B')
                {
                    c++;
                    break;
                }
            }

            if (x > 33 || y > 16) {break;} // 체스판 끝까지 확인
        }

        if (king == 1) { x = kingX3, y = kingY3; }
        else { x = kingX2, y = kingY2; } // 좌표 원래대로 돌려놓기

        //대각선2
        while (1)
        {
            x -= 4;
            y += 2;

            if (board[y][x - 1] == '[') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[y][x - 1] == '<')
            { // 상대방 B나 Q가 있으면 check
                if (board[y][x] == 'Q' || board[y][x] == 'B')
                {
                    c++;
                    break;
                }
            }

            if (x < 0 || y > 16) {break;} // 체스판 끝까지 확인
        }

        if (king == 1) { x = kingX3, y = kingY3; }
        else { x = kingX2, y = kingY2; } // 좌표 원래대로 돌려놓기

        //대각선3
        int pp = 0; // pawn 확인용 (대각선 기준 몇 칸 차이인지)

        while (1)
        {
            x -= 4;
            y -= 2;
            pp++;

            if (board[y][x - 1] == '[') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[y][x - 1] == '<')
            {
                if (board[y][x] == 'P' && pp == 1)
                { // 한 칸 대각선에 상대방 P가 있으면 check
                    c++;
                    break;
                }
                else if (board[y][x] == 'Q' || board[y][x] == 'B')
                { // 상대방 B나 Q가 있으면 check
                    c++;
                    break; // 체스판 끝까지 확인
                }
            }

            if (x < 0 || y < 0) {break;} // 체스판 끝까지 확인
        }

        if (king == 1) { x = kingX3, y = kingY3; }
        else { x = kingX2, y = kingY2; } // 좌표 원래대로 돌려놓기

        //대각선4
        pp = 0; // pawn 확인용 (대각선 기준 몇 칸 차이인지)

        while (1)
        {
            x += 4;
            y -= 2;
            pp++;

            if (board[y][x - 1] == '[') {break;} // 본인 말이 가장 가까이에 있으면 check 아님
            else if (board[y][x - 1] == '<')
            {
                if (board[y][x] == 'P' && pp == 1)
                { // 한 칸 대각선에 상대방 P가 있으면 check
                    c++;
                    break;
                }
                else if (board[y][x] == 'Q' || board[y][x] == 'B')
                { // 상대방 B나 Q가 있으면 check
                    c++;
                    break;
                }
            }

            if (x > 33 || y < 0) {break;} // 체스판 끝까지 확인
        }

        if (king == 1) { x = kingX3, y = kingY3; }
        else { x = kingX2, y = kingY2; } // 좌표 원래대로 돌려놓기

        //knight
        if (board[y + 4][x + 3] == '<' && board[y + 4][x + 4] == 'N') {
            if ((x + 3) < 33 && (y + 4) < 17) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if (board[y + 4][x - 5] == '<' && board[y + 4][x - 4] == 'N') {
            if ((x - 5) >= 0 && (y + 4) < 17) c++; // ←- ↓ ↓ 위치에 상대방 N있으면 check
        }
        if (board[y - 2][x - 9] == '<' && board[y - 2][x - 8] == 'N') {
            if ((x - 9) >= 0 && (y - 2) >= 0) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if (board[y + 2][x + 7] == '<' && board[y + 2][x + 8] == 'N') {
            if ((x + 7) < 33 && (y + 2) < 17) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if (board[y + 2][x - 9] == '<' && board[y + 2][x - 8] == 'N') {
            if ((x - 9) >= 0 && (y + 2) < 17) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if (board[y - 4][x + 3] == '<' && board[y - 4][x + 4] == 'N') {
            if ((x + 3) < 33 && (y - 4) >= 0) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if (board[y - 4][x - 5] == '<' && board[y - 4][x - 4] == 'N') {
            if ((x - 5) >= 0 && (y - 4) >= 0) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
        if (board[y - 2][x + 7] == '<' && board[y - 2][x + 8] == 'N') {
            if ((x + 7) < 33 && (y - 2) >= 0) c++; // ↓ ↓ -→ 위치에 상대방 N있으면 check
        }
    }

    if (c != 0) {return 1;} // check인 경우가 있다
    else {return 0;} // check인 경우가 없다
}


/**
    함수 이름 : Checkmate
    함수 설명 : 체크메이트 여부를 확인하여 체크메이트인 경우 해당 player를 패배처리한다.

    파라미터 이름 : nowY, nowX
    파라미터 설명
        nowY : 이동시킬 King의 현재 y좌표
        nowX : 이동시킬 King의 현재 x좌표
    참조 함수들 :
**/
int Checkmate(int player)
{
    if (player == 1)
    {
        BackupBoard(); // 확인 후 체스판을 처음 상태로 돌려놓기 위해 저장 (백업)

        for (int I = 1; I < 17; I += 2)
        {
            for (int J = 2; J < 34; J += 4)
            {
                if (board[I][J - 1] == '<')
                { // 판에 있는 모든 player1의 말을 대상으로 확인
                    switch (board[I][J])
                    { // 말의 종류에 따라 이동 가능한 위치 표시
                        case 'P': { Pawn(I, J); break; }
                        case 'R': { Rook(I, J); break; }
                        case 'N': { Knight(I, J); break; }
                        case 'B': { Bishop(I, J); break; }
                        case 'Q': { Queen(I, J); break;}
                        case 'K': { King(I, J); break; }
                    }

                    // 이동 후 check 상태가 되는 곳은 .으로 변경
                    for (int i = 0; i < 8; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            BackupCheckBoard(); //확인이 끝난 후 돌려놓을 판을 위해 저장해놓음 (백업용)
                            int checkx = -1, checky = -1; // 이동을 금지시킬 행과 열 인덱스 저장용

                            if (board[2 * i + 1][4 * j + 2] == '*')
                            {  // 이동 가능하다고 표시된 곳으로 이동시키기 (임시)
                                ChangePiece(2*i+1, 4*j+2, '<', board[I][J], '>');
                                ChangePiece(I, J, '.', '.', '.');
                                DeleteStar();

                                if (board[2 * i + 1][4 * j + 2] != 'K')
                                {
                                    if (Check(1, 0) == 1)
                                    {
                                        checkx = 4 * j + 2;
                                        checky = 2 * i + 1;
                                    } // check인 경우 좌표 저장
                                }
                                else
                                {
                                    kingX3 = 4 * j + 2;
                                    kingY3 = 2 * i + 1; // 움직일 말이 King이면 Check함수에서 이용할 변수값도 바꿔야 함

                                    if (Check(1, 1) == 1)
                                    {
                                        checkx = 4 * j + 2;
                                        checky = 2 * i + 1;
                                    } // check인 경우 좌표 저장
                                }

                                ReturnCheckBoard(); //다시 현재 판으로 돌려놓음

                                if (checkx >= 0) { board[checky][checkx] = '.'; }
                                // 만약 check인 경우가 있어서 checkx, checky에 인덱스 값이 저장되어있으면 * -> . (이동 못함)
                            }
                        }
                    }

                    // 선택된 말이 움질일 수 있는 경로가 있는지 확인
                    for (int i = 0; i < 8; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            if (board[2 * i + 1][4 * j + 2] == '*') { countStar += 1; } // countStar >= 1이면 말이 움질일 수 있다는 것을 의미
                        }
                    }

                    if (countStar != 0)
                    { // 이동할 수 있는 곳이 적어도 하나 있으면 checkmate아님
                        countStar = 0;
                        return 1;
                    }
                    else
                    {
                        for (int k = 0; k < 17; k++) { strcpy(board[k], backupBoard[k]); }
                    }
                }
            }
        }

        if (countStar == 0) {return countStar;}
    }
    else
    {
        BackupBoard(); // 확인 후 체스판을 처음 상태로 돌려놓기 위해 저장 (백업)

        for (int I = 1; I < 17; I += 2)
        {
            for (int J = 2; J < 34; J += 4)
            {
                if (board[I][J - 1] == '[')
                { // 판에 있는 모든 player2의 말을 대상으로 확인
                    switch (board[I][J])
                    { // 말의 종류에 따라 이동 가능한 위치 표시
                        case 'P': { Pawn(I, J); break; }
                        case 'R': { Rook(I, J); break; }
                        case 'N': { Knight(I, J); break; }
                        case 'B': { Bishop(I, J); break; }
                        case 'Q': { Queen(I, J); break; }
                        case 'K': { King(I, J); break; }
                    }

                    // 이동 후 check 상태가 되는 곳은 .으로 변경
                    for (int i = 0; i < 8; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            BackupCheckBoard(); // 확인이 끝난 후 돌려놓을 판을 위해 저장해놓음 (백업용)
                            int checkx = -1, checky = -1; // 이동을 금지시킬 행과 열 인덱스 저장용

                            if (board[2 * i + 1][4 * j + 2] == '*')
                            { // 이동 가능하다고 표시된 곳으로 이동시키기 (임시)
                                ChangePiece(2*i+1, 4*j+2, '[', board[I][J], ']');
                                ChangePiece(I, J, '.', '.', '.');
                                DeleteStar();

                                if (board[2 * i + 1][4 * j + 2] != 'K')
                                {
                                    if (Check(2, 0) == 1)
                                    {
                                        checkx = 4 * j + 2;
                                        checky = 2 * i + 1;
                                    } // check인 경우 좌표 저장
                                }
                                else
                                {
                                    kingX3 = 4 * j + 2;
                                    kingY3 = 2 * i + 2; // 움직일 말이 King이면 Check함수에서 이용할 변수값도 바꿔야 함

                                    if (Check(2, 1) == 1)
                                    {
                                        checkx = 4 * j + 2;
                                        checky = 2 * i + 1;
                                    } // check인 경우 좌표 저장
                                }

                                ReturnCheckBoard(); //다시 현재 판으로 돌려놓음

                                if (checkx >= 0) { board[checky][checkx] = '.'; }
                                // checkx, checky에 인덱스 값이 저장되어있으면 * -> . (이동 못함)
                            }
                        }
                    }

                    // 선택된 말이 움질일 수 있는 경로가 있는지 확인
                    for (int i = 0; i < 8; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            if (board[2 * i + 1][4 * j + 2] == '*') { countStar += 1; } // countStar >= 1이면 말이 움질일 수 있다는 것을 의미
                        }
                    }

                    if (countStar != 0)
                    {
                        countStar = 0;
                        return 1;
                    } // 이동할 수 있는 곳이 적어도 하나 있으면 checkmate아님
                    else
                    {
                        for (int k = 0; k < 17; k++) { strcpy(board[k], backupBoard[k]); }
                    }
                }
            }
        }
        if (countStar == 0) {return countStar;} // checkmate인 경우 return 0;
    }
}


/**
    함수 이름 : Promotion
    함수 설명 : 특수룰(프로모션), 상대방 진영 끝에 Pawn이 도달했을 경우 Pawn을  Q, B, N, R 중 하나로 바꿀 수 있다.
    파라미터 이름 : beforex, beforey, aftery, player
    파라미터 설명
        beforex : player가 이동시킬 말의 x좌표
        beforey : player가 이동시킬 말의 y좌표
        aftery : 말을 이동시킬 위치의 y좌표
        player : 실행시킬 player (1 or 2)
    @ exception 예외처리
    //
**/
void Promotion(int beforex, int beforey, int aftery, int player) {

    if (player == 1) {
        if (board[beforey][beforex] == 'P' && aftery == 15) { // 프로모션 - Pawn이 상대측 체스판 끝에 도달 시 Q, B, N, R 중 하나로 변경
            char Change[2];
            Clear();
            printf("change P to ? (Q or B or N or R) : ");
            scanf("%s", Change);

            if (Change[0] == 'Q' || Change[0] == 'B' || Change[0] == 'N' || Change[0] == 'R') {
                board[beforey][beforex] = Change[0]; // 입력받은 문자열로 변경
            }
            else { // 다른 문자 입력하면 PASS!
                printf("You can not change P to %c\n", Change[0]);
            }
        }
    }

    else {
        if (board[beforey][beforex] == 'P' && aftery == 1) { // 프로모션 - Pawn이 상대측 체스판 끝에 도달 시 Q, B, N, R 중 하나로 변경
            char Change[2];
            Clear();
            printf("change P to ? (Q or B or N or R) : ");
            scanf("%s", Change);

            if (Change[0] == 'Q' || Change[0] == 'B' || Change[0] == 'N' || Change[0] == 'R')
            {
                board[beforey][beforex] = Change[0]; // 입력받은 문자열로 변경
            }
            else
            { // 다른 문자 입력하면 PASS!
                printf("You can not change P to %c\n", Change[0]);
            }
        }
    }
}

/**
    함수 이름 : Castling
    함수 설명 : 특수룰(캐슬링), 다음과 같은 조건이 성립할 때 K와 R을 정해진 위치로 동시에 이동시킬 수 있다.
    파라미터 이름 : player
    파라미터 설명
        player : 실행시키는 player 번호 (1 or 2)
    @ exception 예외처리
    //
**/
void Castling(int player) {
    int piece = 0;

    if (player == 1) {
        /**longcastling**/
        if (longCastlingCheck[0] == 0) { // K과 R이 모두 움직이지 않았다.
            for (int i = (kingX1 + 4) ; i < 29 ; i += 4)
            {
                if (board[kingY1][i] != '.' && board[kingY1][i] != '*')
                {
                    piece += 1;
                    break;
                }
            }

            if (piece == 0) {
                ChangePiece( kingY1, kingX1 + 8, 'C', 'a', 's');
            }
        }
        piece = 0;
        /**shortcasting**/
        if (shortCastlingCheck[0] == 0) {
            for (int i = (kingX1 - 4) ; i > 2 ; i -= 4)
            {
                if (board[kingY1][i] != '.' && board[kingY1][i] != '*')
                { piece += 1; break; }
            }

            if (piece == 0) {
                ChangePiece(kingY1, kingX1 - 8, 'C', 'a', 's');
            }
        }
    }
    else if (player == 2) {
        /**longcastling**/
        if (longCastlingCheck[1] == 0) { // K과 R이 모두 움직이지 않았다.
            for (int i = (kingX2 + 4) ; i < 29 ; i += 4)
            {
                if (board[kingY2][i] != '.' && board[kingY2][i] != '*')
                {
                    piece += 1;
                    break;
                }
            }

            if (piece == 0)
            { ChangePiece(kingY2, kingX2 + 8, 'C', 'a', 's'); }
        }
        piece = 0;
        /**shortcasting**/
        if (shortCastlingCheck[1] == 0) {
            for (int i = (kingX2 - 4) ; i > 2 ; i -= 4)
            {
                if (board[kingY2][i] != '.' && board[kingY2][i] != '*')
                { piece += 1; break; }
            }

            if (piece == 0) {
                { ChangePiece(kingY2, kingX2 - 8, 'C', 'a', 's'); }
            }
        }
    }
}