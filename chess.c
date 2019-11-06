#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termio.h>

char pan[17][34];
char backpan[17][34];
char checkbackpan[17][34];
char (*pStr)[34]; //현재 맵 변경시 사용
char PAN[100] = "C:\\Users\\SoHyun Kim\\Desktop\\chess.txt";
char SPAN[100] = "C:\\Users\\SoHyun Kim\\Desktop\\chess_save.txt";
static int end; // King 잡은 player 확인용
static int F=0; // 움직일 수 있는 위치가 없을 때
static int kx1=18, ky1=1; //player 1의 King 좌표
static int kx2=18, ky2=15; //player 2의 King 좌표
void clear() { system("clear"); } // clear함수
char die1[20]={""}, die2[20]={""}; // 죽은 말 표시


void Load(int); //파일 읽어서 배열, 포인터에 저장
void Print(void); //현재 판 출력
void Backpan(void); //되돌리기용 판 저장
void Checkbackpan(void); //되돌리기용 판 저장
void Returncheckpan(void); //판 되돌리기
void Rule(void); //도움말 출력
void Save(void); //저장
void Delete_s(void); // *표시 원래로 돌리기
void Move1(void); //Player1 이동
void Move2(void); //Player2 이동
void Pawn(int, int); //P 선택시
void Rook(int, int); //R 선택시
void Knight(int, int); //N 선택시
void Bishop(int, int); //B 선택시
void Queen(int, int); //Q 선택시
void King(int, int); //K 선택시
int Check(int); //check 여부 확인
int Checkmate(int); //종료 조건
int getch() {
    int ch;
    struct termios buf;
    struct termios save;

    tcgetattr(0, &save);
    buf = save;
    buf.c_lflag &= ~(ICANON | ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}


int main() {
    int a=0;

    while (1) {
        clear();
        if (end != 0) {printf("\nPlayer %d WIN!\n\n",end); end=0;}
        printf("+---- MANU ----+\n|  1.New game  |\n|  2.Lord game |\n|    3.Help    |\n|    4.Exit    |\n+--------------+\n");
        printf("Maun Number : ");
        scanf("%d", &a); clear();
        switch (a) {
            case 1: {
                Load(1); Print();
                while (1) {
                    Move1();
                    if(end==1||end==2) {
                        break;
                    }
                    else if (end==3) {Save(); break;}
                    clear(); Print();

                    Move2();
                    if(end==1||end==2) {
                        break;
                    }
                    else if (end==3) {Save(); break;}
                    clear(); Print();

                }break;
            }
            case 2: {
                Load(2); Print();
                while (1) {
                    Move1();
                    if(end==1||end==2) {
                        break;
                    }
                    else if (end==3) {Save(); break;}
                    clear(); Print();

                    Move2();
                    if(end==1||end==2) {
                        break;
                    }
                    else if (end==3) {Save(); break;}
                    clear(); Print();

                }break;
            }
            case 3: {
                Rule();
                break;
            }
            default:
                break;
        }

        if (a==4||end==3) {
            printf("Thank you");
            break;
        }

    }
    return 0;
}

void Load(int k) {
    FILE *fp = NULL;
    char input[50];
    int I=0;
    if (k==1) fp = fopen(PAN, "r+");
    else {
        fp = fopen(SPAN, "r");
        if (fp==NULL) {
            printf("no savefile. new game start\n");
            fp = fopen(PAN, "r+");
        }
    }
    while (fscanf(fp, "%s", input) != EOF) {
        strcpy(pan[I],input);
        I++;
    } fclose(fp);

    pStr = pan;

    strcpy(die1, "DIE-");
    strcpy(die2, "DIE-");
 }

void Print(void) {
    int pnum=1;
    //printf("  %s\n", die2);
    if(strlen(die2) > 4) printf("%s\n", die2);
    printf("    A   B   C   D   E   F   G   H\n");
    for (int i=0;i<17;i++) {
        if (i%2==0) printf("  %s\n", *(pStr+i));
        else {printf("%d %s\n",pnum,*(pStr+i)); pnum++;}
    }
    if(strlen(die1) > 4) printf("%s\n", die1);
}

void Backpan(void) {
    for(int i=0;i<17;i++)
        strcpy(backpan[i], pan[i]);
}

void Checkbackpan(void) {
    for(int i=0;i<17;i++)
        strcpy(checkbackpan[i], pan[i]);
}

void Returncheckpan(void) {
    for(int i=0;i<17;i++)
        strcpy(*(pStr+i), checkbackpan[i]);
}

void Rule(void) {
    printf("\n<  > :Player 1, [  ] : Player 2\n");
    printf("\nYou can move your pieces to '*'\n");
    printf("\nK can move one square horizontally, vertically, or diagonally.\n"
           "Q can move any number of vacant squares diagonally, horizontally, or vertically.\n"
           "R can move any number of vacant squares vertically or horizontally. It also is moved while castling.\n"
           "B can move any number of vacant squares in any diagonal direction.\n"
           "N can move one square along any rank or file and then at an angle. The knight´s movement can also be viewed as an “L” or “7″ laid out at any horizontal or vertical angle.\n"
           "P can move forward one square, if that square is unoccupied. \n(If it has not yet moved, the pawn can move two squares forward provided both squares in front of the pawn are unoccupied.)\n\n");
    getch();
    getch();
}

void Delete_s(void) {
    char Back;

    for(int i=0;i<17;i++)
        for(int j=0;j<49;j++)
            if (*(*(pStr+i)+j)=='*') {
                Back = backpan[i][j];
                *(*(pStr+i)+j) = Back;
            }

}

//player 1 이동함수
void Move1(void) {
    char before[5], after[5];
    int b1=0, b2=0, a1=0, a2=0;
    Backpan();

    //말 없는 자리 선택시 다시
    while(1) {

        if(Checkmate(1) == 0) { end = 2; break;}
        for(int k=0;k<17;k++) {strcpy(pan[k], backpan[k]);}

        printf("< Player 1 >\n(save: save game board, exit: end game)\nWhat? : ");
        scanf("%s", before);
        if (strcmp(before, "save") == 0) {
            Save();
            printf("S A V E . . . !\n");
            continue;
        }
        else if(strcmp(before, "exit") == 0) {
            end += 3;
            break;
        }
        else {
            b1 = 2 * (before[1] - 48) - 1;
            b2 = 4 * (before[0] - 64) - 2;
            if (*(*(pStr + b1) + (b2-1)) == '[') { printf("It is not yours\n\n"); continue; }
            if (*(*(pStr + b1) + b2) == '.') { printf("There are nothing\n\n"); continue; }
            else {
                switch (*(*(pStr+b1)+b2)) {  //이동 가능한 곳 *표시
                    case 'P': { Pawn(b1,b2); break;}
                    case 'R': { Rook(b1,b2); break;}
                    case 'N': { Knight(b1,b2); break;}
                    case 'B': { Bishop(b1,b2); break;}
                    case 'Q': { Queen(b1,b2); break;}
                    case 'K': { King(b1,b2); break;}
                }

                //이동 후 check 상태가 되는 곳은 .으로 변경
                for(int i=0;i<8;i++) {
                    for(int j=0;j<8;j++) {
                        Checkbackpan(); //마지막에 다시 이걸로 돌려놔!
                        int checkx=-1, checky=-1; //checkx = j, checky = i
                        if(*(*(pStr+(2*i+1))+(4*j+2)) == '*') {
                            *(*(pStr+(2*i+1))+(4*j+1)) = '<';
                            *(*(pStr+(2*i+1))+(4*j+2)) = *(*(pStr+b1)+b2);
                            *(*(pStr+(2*i+1))+(4*j+3)) = '>';
                            *(*(pStr+b1)+(b2-1)) = *(*(pStr+b1)+b2) = *(*(pStr+b1)+(b2+1)) = '.';
                            Delete_s();
                            if ( Check(1) == 1 ) { checkx = j; checky = i; } //check인 경우 좌표 저장
                            Returncheckpan();
                            if (checkx >= 0) {*(*(pStr+(2*checky+1))+(4*checkx+2)) = '.';}
                        }
                    }
                }

                //움질일 수 있는 경로가 없는 경우 다시
                for(int i=0;i<8;i++) {
                    for(int j=0;j<8;j++) {
                        if(*(*(pStr+(2*i+1))+(4*j+2)) == '*')
                            F += 1;
                    }
                }
                if(F == 0) {
                    printf("You can not move that!\n\n");
                    continue;
                }

                else {
                    F=0;
                    clear();
                    Print();
                    break;}
            }
        }
    }

    //자기 말 있는 곳 선택시 다시
    while(1) {
        if(end == 3||end == 2) break;
        printf("< Player 1 >\nWhere? : ");
        scanf("%s", after);
        a1 = 2 * (after[1] - 48) - 1;
        a2 = 4 * (after[0] - 64) - 2;
        if( *(*(pStr+b1)+b2) =='P' && a1==15) { //특수룰
            char Change[2];
            clear();
            printf("change P to ? (Q or B or N or R) : ");
            scanf("%s", Change);
            *(*(pStr+b1)+b2) = Change[0];
        }
        if(*(*(pStr+a1)+a2) != '*') { printf("again\n"); continue; }
        if (*(*(pStr + a1) + (a2 - 1)) == '<') { printf("again\n"); continue; }
        else { Delete_s(); break;}
    }

    if( end!=3 && end!=2 ) {

        //King 움직인 경우 좌표 저장 (for check확인)
        if (*(*(pStr+b1)+b2)=='K') {
            kx1 = a1; ky1 = a2;
        }

        //왕 잡으면 end
        if (*(*(pStr+a1)+ a2)=='K') end=1;

        //자리 옮기기
        if (*(*(pStr+a1)+(a2-1)) == '[') {
            switch (backpan[a1][a2]) {
                case 'P': { strcat(die2, "[P]"); break; }
                case 'R': { strcat(die2, "[R]"); break; }
                case 'B': { strcat(die2, "[B]"); break; }
                case 'N': { strcat(die2, "[N]"); break; }
                case 'Q': { strcat(die2, "[Q]"); break; }
            }
        }
        *(*(pStr+a1)+ a2) = *(*(pStr+b1)+b2);
        *(*(pStr+a1)+ ++a2) = '>'; a2--;
        *(*(pStr+a1)+ --a2) = '<';
        *(*(pStr+b1)+b2) = '.';
        *(*(pStr+b1)+ ++b2) = '.'; b2--;
        *(*(pStr+b1)+ --b2) = '.';
    }
}

//player 2 이동함수
void Move2(void) { // 2 이동
    char before[3], after[3];
    int a1=0, a2=0, b1=0, b2=0;
    Backpan();

    // 말 없는 장소 선택시 다시
    while(1) {

        if(Checkmate(2) == 0) { end = 1; break;}
        for(int k=0;k<17;k++) {strcpy(pan[k], backpan[k]);}

        printf("[ Player 2 ]\n(save: save game board, exit: end game)\nWhat? : ");
        scanf("%s", before);
        if (strcmp(before, "save") == 0) {
            Save();
            printf("S A V E . . . !\n");
            continue;
        }
        else if(strcmp(before, "exit") == 0) {
            end += 3;
            break;
        }
        else {
            b1 = 2 * (before[1] - 48) - 1;
            b2 = 4 * (before[0] - 64) - 2;
            if (*(*(pStr + b1) + (b2-1)) == '<') { printf("It is not yours\n\n"); continue; }
            if (*(*(pStr + b1) + b2) == '.') { printf("There are nothing\n\n"); continue; }
            else {
                switch (*(*(pStr+b1)+b2)) {
                    case 'P': { Pawn(b1,b2); break;}
                    case 'R': { Rook(b1,b2); break;}
                    case 'N': { Knight(b1,b2); break;}
                    case 'B': { Bishop(b1,b2); break;}
                    case 'Q': { Queen(b1,b2); break;}
                    case 'K': { King(b1,b2); break;}
                }

                //이동 후 check 상태가 되는 곳은 .으로 변경
                for(int i=0;i<8;i++) {
                    for(int j=0;j<8;j++) {
                        Checkbackpan(); //마지막에 다시 이걸로 돌려놔!
                        int checkx=-1, checky=-1; //checkx = j, checky = i
                        if(*(*(pStr+(2*i+1))+(4*j+2)) == '*') {
                            *(*(pStr+(2*i+1))+(4*j+1)) = '[';
                            *(*(pStr+(2*i+1))+(4*j+2)) = *(*(pStr+b1)+b2);
                            *(*(pStr+(2*i+1))+(4*j+3)) = ']';
                            *(*(pStr+b1)+(b2-1)) = *(*(pStr+b1)+b2) = *(*(pStr+b1)+(b2+1)) = '.';
                            Delete_s();
                            if ( Check(2) == 1 ) { checkx = j; checky = i; } //check인 경우 좌표 저장
                            Returncheckpan();
                            if (checkx >= 0) {*(*(pStr+(2*checky+1))+(4*checkx+2)) = '.';}
                        }
                    }
                }

                //움질일 수 있는 경로가 없는 경우
                for(int i=0;i<8;i++) {
                    for(int j=0;j<8;j++) {
                        if(*(*(pStr+(2*i+1))+(4*j+2)) == '*')
                            F += 1;
                    }
                }
                if(F == 0) {
                    printf("You can not move that!\n");
                    continue;
                }

                else {
                    F=0;
                    clear();
                    Print();
                    break;}
            }
        }
    }

    //본인 말 있는 곳 선택시 다시
    while(1) {
        if (end==3||end==1) break;
        printf("[ Player 2 ]\nWhere? : ");
        scanf("%s", after);
        a1=2*(after[1]-48)-1;
        a2=4*(after[0]-64)-2;
        if( *(*(pStr+b1)+b2) == 'P' && a1==1) { //특수룰
            char Change[2];
            clear();
            printf("change P to ? (Q or B or N or R) : ");
            scanf("%s", Change);
            *(*(pStr+b1)+b2) = Change[0];
        }
        if(*(*(pStr+a1)+a2) != '*') { printf("again\n"); continue; }
        if(*(*(pStr+a1)+(a2-1)) == '[') {printf("again\n"); continue;}
        else {Delete_s(); break;}
    }

    if (end != 3 && end != 1) {

        //King 움직인 경우 좌표 저장 (for check확인)
        if (*(*(pStr+b1)+b2)=='K') {
            kx2 = a1; ky2 = a2;
        }

        // 왕 잡으면 end
        if (*(*(pStr+a1)+ a2)=='K') end=2;


        //자리 옮기기
        if (*(*(pStr+a1)+(a2-1)) == '<') {
            switch (backpan[a1][a2]) {
                case 'P': { strcat(die1, "<P>"); break; }
                case 'R': { strcat(die1, "<R>"); break; }
                case 'B': { strcat(die1, "<B>"); break; }
                case 'N': { strcat(die1, "<N>"); break; }
                case 'Q': { strcat(die1, "<Q>"); break; }
            }
        }

        *(*(pStr+a1)+ a2) = *(*(pStr+b1)+b2);
        *(*(pStr+a1)+ ++a2) = ']'; a2--;
        *(*(pStr+a1)+ --a2) = '[';
        *(*(pStr+b1)+b2) = '.';
        *(*(pStr+b1)+ ++b2) = '.'; b2--;
        *(*(pStr+b1)+ --b2) = '.';
    }
}

void Pawn(int b1, int b2) {

    if(*(*(pStr+b1)+(b2-1))=='<'){
        if(*(*(pStr+(b1+2))+b2) == '.' ) {
            *(*(pStr+(b1+2))+b2) = '*';
            if( b1==3 ) {*(*(pStr+(b1+4))+b2) = '*';}
        }
        if( *(*(pStr+(b1+2))+(b2-4))!='.' && *(*(pStr+(b1+2))+(b2-5))=='[' ) {
            *(*(pStr+(b1+2))+(b2-4)) = '*';
        }
        if( *(*(pStr+(b1+2))+(b2+4)) != '.' && *(*(pStr+(b1+2))+(b2+5))==']' ) {

            *(*(pStr+(b1+2))+(b2+4)) = '*'; }
    }
    else {
        if(*(*(pStr+(b1-2))+b2) == '.')
        {*(*(pStr+(b1-2))+b2) = '*';
            if( b1==13 ) {*(*(pStr+(b1-4))+b2) = '*';}}
        if( *(*(pStr+(b1-2))+(b2-4))!='.' && *(*(pStr+(b1-2))+(b2-5))=='<') {
            *(*(pStr+(b1-2))+(b2-4)) = '*';
        }
        if( *(*(pStr+(b1-2))+(b2+4)) != '.' && *(*(pStr+(b1-2))+(b2+3))=='<') {
            *(*(pStr+(b1-2))+(b2+4)) = '*';
        }
    }
}

void Rook(int b1, int b2) {
    int B1 = b1, B2 = b2;
    if (*(*(pStr + B1) + (B2 - 1)) == '<') { // Player 1
        while (1) {
            B1 += 2;
            if(B1 > 16) break;
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
        B1 = b1;
        while (1) {
            B1 -= 2;
            if (B1 < 0) break;
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
        B1 = b1;
        while (1) {
            B2 += 4;
            if (B2 > 33) break;
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
        B2 = b2;
        while (1) {
            B2 -= 4;
            if (B2<0) break;
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') { //여기 손봐야해!!!!!!!!!! ?
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
    }
    else { // Player 2
        while (1) {
            B1 += 2;
            if(B1 > 16) break;
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
        B1 = b1;
        while (1) {
            B1 -= 2;
            if(B1<0) break;
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
        B1 = b1;
        while (1) {
            B2 += 4;
            if (B2 > 33) break;
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
        B2 = b2;
        while (1) {
            B2 -= 4;
            if (B2 < 0) break;
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
    }
}

void Bishop(int b1, int b2) {
    int B1 = b1, B2 = b2;

    if(*(*(pStr+b1)+(b2-1))=='<') {
        while (1) {
            B1 += 2;
            B2 += 4;
            if (B1 > 16 || B2 > 33) break;
            else if(*(*(pStr+B1)+B2) == '.') {
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '[') {
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break;
        }
        B1=b1; B2=b2;

        while (1) {
            B1 += 2;
            B2 -= 4;
            if (B1 > 16 || B2 < 0) break;
            else if(*(*(pStr+B1)+B2) == '.') {
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '[') {
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break;
        }
        B1=b1; B2=b2;

        while (1) {
            B1 -= 2;
            B2 += 4;
            if (B1 < 0 || B2 > 33) break;
            else if(*(*(pStr+B1)+B2) == '.') {
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '[') {
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break;
        }
        B1=b1; B2=b2;

        while (1) {
            B1 -= 2;
            B2 -= 4;
            if (B1 < 0 || B2 < 0) break;
            else if(*(*(pStr+B1)+B2) == '.') {
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '[') {
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break;
        }
    }

    else {
        while (1) {
            B1 += 2;
            B2 += 4;
            if (B1 > 16 || B2 > 33) break;
            else if(*(*(pStr+B1)+B2) == '.') {
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '<') {
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break;
        }
        B1=b1; B2=b2;

        while (1) {
            B1 += 2;
            B2 -= 4;
            if (B1 > 16 || B2 < 0) break;
            else if(*(*(pStr+B1)+B2) == '.') {
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '<') {
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break;
        }
        B1=b1; B2=b2;

        while (1) {
            B1 -= 2;
            B2 += 4;
            if (B1 < 0 || B2 > 33) break;
            else if(*(*(pStr+B1)+B2) == '.') {
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '<') {
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break;
        }
        B1=b1; B2=b2;

        while (1) {
            B1 -= 2;
            B2 -= 4;
            if (B1 < 0 || B2 < 0) break;
            else if(*(*(pStr+B1)+B2) == '.') {
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '<') {
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break;
        }
    }
}

void Knight(int b1, int b2) {

    if(*(*(pStr+b1)+(b2-1)) == '<') {

        if( *(*(pStr+(b1+4))+(b2+3)) == '[' || *(*(pStr+(b1+4))+(b2+3)) == '.' ) *(*(pStr+(b1+4))+(b2+4)) = '*';
        if( *(*(pStr+(b1+4))+(b2-5)) == '[' || *(*(pStr+(b1+4))+(b2-5)) == '.' ) *(*(pStr+(b1+4))+(b2-4)) = '*';
        if( *(*(pStr+(b1+2))+(b2+7)) == '[' || *(*(pStr+(b1+2))+(b2+7)) == '.' ) *(*(pStr+(b1+2))+(b2+8)) = '*';
        if( *(*(pStr+(b1+2))+(b2-9)) == '[' || *(*(pStr+(b1+2))+(b2-9)) == '.' ) *(*(pStr+(b1+2))+(b2-8)) = '*';
        if( *(*(pStr+(b1-4))+(b2+3)) == '[' || *(*(pStr+(b1-4))+(b2+3)) == '.' ) *(*(pStr+(b1-4))+(b2+4)) = '*';
        if( *(*(pStr+(b1-4))+(b2-5)) == '[' || *(*(pStr+(b1-4))+(b2-5)) == '.' ) *(*(pStr+(b1-4))+(b2-4)) = '*';
        if( *(*(pStr+(b1-2))+(b2+7)) == '[' || *(*(pStr+(b1-2))+(b2+7)) == '.' ) *(*(pStr+(b1-2))+(b2+8)) = '*';
        if( *(*(pStr+(b1-2))+(b2-9)) == '[' || *(*(pStr+(b1-2))+(b2-9)) == '.' ) *(*(pStr+(b1-2))+(b2-8)) = '*';
    }
    else {
        if( *(*(pStr+(b1+4))+(b2+3)) == '<' || *(*(pStr+(b1+4))+(b2+3)) == '.' ) *(*(pStr+(b1+4))+(b2+4)) = '*';
        if( *(*(pStr+(b1+4))+(b2-5)) == '<' || *(*(pStr+(b1+4))+(b2-5)) == '.' ) *(*(pStr+(b1+4))+(b2-4)) = '*';
        if( *(*(pStr+(b1+2))+(b2+7)) == '<' || *(*(pStr+(b1+2))+(b2+7)) == '.' ) *(*(pStr+(b1+2))+(b2+8)) = '*';
        if( *(*(pStr+(b1+2))+(b2-9)) == '<' || *(*(pStr+(b1+2))+(b2-9)) == '.' ) *(*(pStr+(b1+2))+(b2-8)) = '*';
        if( *(*(pStr+(b1-4))+(b2+3)) == '<' || *(*(pStr+(b1-4))+(b2+3)) == '.' ) *(*(pStr+(b1-4))+(b2+4)) = '*';
        if( *(*(pStr+(b1-4))+(b2-5)) == '<' || *(*(pStr+(b1-4))+(b2-5)) == '.' ) *(*(pStr+(b1-4))+(b2-4)) = '*';
        if( *(*(pStr+(b1-2))+(b2+7)) == '<' || *(*(pStr+(b1-2))+(b2+7)) == '.' ) *(*(pStr+(b1-2))+(b2+8)) = '*';
        if( *(*(pStr+(b1-2))+(b2-9)) == '<' || *(*(pStr+(b1-2))+(b2-9)) == '.' ) *(*(pStr+(b1-2))+(b2-8)) = '*';
    }

}

void Queen(int b1, int b2) {
    Rook(b1,b2);
    Bishop(b1,b2);
}

void King(int b1, int b2) {
    if(*(*(pStr+b1)+(b2-1)) == '<') {
        if( *(*(pStr+(b1+2))+(b2-1)) != '<' ) *(*(pStr+(b1+2))+b2) = '*';
        if( *(*(pStr+(b1-2))+(b2-1)) != '<' ) *(*(pStr+(b1-2))+b2) = '*';
        if( *(*(pStr+b1)+(b2+3)) != '<' ) *(*(pStr+b1)+(b2+4)) = '*';
        if( *(*(pStr+b1)+(b2-5)) != '<' ) *(*(pStr+b1)+(b2-4)) = '*';
        if( *(*(pStr+(b1+2))+(b2+3)) != '<' ) *(*(pStr+(b1+2))+(b2+4)) = '*';
        if( *(*(pStr+(b1+2))+(b2-5)) != '<' ) *(*(pStr+(b1+2))+(b2-4)) = '*';
        if( *(*(pStr+(b1-2))+(b2+3)) != '<' ) *(*(pStr+(b1-2))+(b2+4)) = '*';
        if( *(*(pStr+(b1-2))+(b2-5)) != '<' ) *(*(pStr+(b1-2))+(b2-4)) = '*';
    }
    else {
        if( *(*(pStr+(b1+2))+(b2-1)) != '[' ) *(*(pStr+(b1+2))+b2) = '*';
        if( *(*(pStr+(b1-2))+(b2-1)) != '[' ) *(*(pStr+(b1-2))+b2) = '*';
        if( *(*(pStr+b1)+(b2+3)) != '[' ) *(*(pStr+b1)+(b2+4)) = '*';
        if( *(*(pStr+b1)+(b2-5)) != '[' ) *(*(pStr+b1)+(b2-4)) = '*';
        if( *(*(pStr+(b1+2))+(b2+3)) != '[' ) *(*(pStr+(b1+2))+(b2+4)) = '*';
        if( *(*(pStr+(b1+2))+(b2-5)) != '[' ) *(*(pStr+(b1+2))+(b2-4)) = '*';
        if( *(*(pStr+(b1-2))+(b2+3)) != '[' ) *(*(pStr+(b1-2))+(b2+4)) = '*';
        if( *(*(pStr+(b1-2))+(b2-5)) != '[' ) *(*(pStr+(b1-2))+(b2-4)) = '*';
    }
}

void Save(void) {
    FILE *fp = NULL;
    fp = fopen(SPAN, "w+");

    for(int i=0;i<17;i++) {
        fprintf(fp, "%s \n", *(pStr+i));
    }
    fclose(fp);
}

int Check(int player) { // king의 좌표로 check 여부 확인
    int c=0;
    if(player==1) {
        int x=kx1, y=ky1;

        //오른쪽
        for(int i=x;i<34;i+=4) {
            if ( *(*(pStr+i)+(x-1)) == '<') break;
            else if( *(*(pStr+y)+(i-1))  == '[') {
                if( *(*(pStr+y)+i) == 'R' || *(*(pStr+y)+i) == 'Q' ) {
                    c++; break;
                }
            }
        }

        //왼쪽
        for(int i=x;i>=0;i-=4) {
            if ( *(*(pStr+i)+(x-1)) == '<') break;
            else if( *(*(pStr+y)+(i-1))  == '[') {
                if( *(*(pStr+y)+i) == 'R' || *(*(pStr+y)+i) == 'Q' ) {
                    c++; break;
                }
            }
        }

        //위
        for(int i=y;i<17;i+=2) {
            if ( *(*(pStr+i)+(x-1)) == '<') break;
            else if( *(*(pStr+i)+(x-1))  == '[') {
                if( *(*(pStr+i)+x) == 'R' || *(*(pStr+i)+x) == 'Q' ) {
                    c++; break;
                }
            }
        }

        //아래
        for(int i=y;i>=0;i-=2) {
            if ( *(*(pStr+i)+(x-1)) == '<') break;
            else if( *(*(pStr+i)+(x-1))  == '[') {
                if( *(*(pStr+i)+x) == 'R' || *(*(pStr+i)+x) == 'Q' ) {
                    c++; break;
                }
            }
        }

        //대각선1
        int pp=0; //pawn 확인용
        while(1) {
            x += 4; y += 2; pp++;
            if( *(*(pStr+y)+(x-1)) == '<') break;
            else if( *(*(pStr+y)+(x-1)) == '[') {
                if ( *(*(pStr+y)+x) == 'P' && pp == 1) {
                    printf("%d : %c\n", pp, *(*(pStr+y)+x));
                    c++; break;
                }
                else if( *(*(pStr+y)+(x-1)) == 'Q' || *(*(pStr+y)+(x-1)) == 'B' ) {
                    c++; break;
                }
            }
            if(x>33||y>16) {break;}
        }
        x=kx1, y=ky1;

        //대각선2
        pp=0; //pawn 확인용
        while(1) {
            x -= 4; y += 2; pp++;
            if( *(*(pStr+y)+(x-1)) == '<') break;
            else if( *(*(pStr+y)+(x-1)) == '[') {
                if ( *(*(pStr+y)+x) == 'P' && pp == 1) {
                    c++; break;
                }
                else if( *(*(pStr+y)+(x-1)) == 'Q' || *(*(pStr+y)+(x-1)) == 'B' ) {
                    c++; break;
                }
            }
            if(x<0||y>16) break;
        }
        x=kx1, y=ky1;

        //대각선3
        while(1) {
            x -= 4; y -= 2;
            if( *(*(pStr+y)+(x-1)) == '<') break;
            else if( *(*(pStr+y)+(x-1)) == '[') {
                if( *(*(pStr+y)+(x-1)) == 'Q' || *(*(pStr+y)+(x-1)) == 'B' ) {
                    c++; break;
                }
            }
            if(x<0||y<0) break;
        }
        x=kx1, y=ky1;

        //대각선4
        while(1) {
            x += 4; y -= 2;
            if( *(*(pStr+y)+(x-1)) == '<') break;
            else if( *(*(pStr+y)+(x-1)) == '[') {
                if( *(*(pStr+y)+(x-1)) == 'Q' || *(*(pStr+y)+(x-1)) == 'B' ) {
                    c++; break;
                }
            }
            if(x>33||y<0) break;
        }

        x=kx1, y=ky1;
        //knight
        if( *(*(pStr+(y+4))+(x+3)) == '[' && *(*(pStr+(y+4))+(x+4)) == 'N' ) c++;
        if( *(*(pStr+(y+4))+(x-5)) == '[' && *(*(pStr+(y+4))+(x-4)) == 'N' ) c++;
        if( *(*(pStr+(y-2))+(x-9)) == '[' && *(*(pStr+(y-2))+(x-8)) == 'N' ) c++;
        if( *(*(pStr+(y+2))+(x+7)) == '[' && *(*(pStr+(y+2))+(x+8)) == 'N' ) c++;
        if( *(*(pStr+(y+2))+(x-9)) == '[' && *(*(pStr+(y+2))+(x-8)) == 'N' ) c++;
        if( *(*(pStr+(y-4))+(x+3)) == '[' && *(*(pStr+(y-4))+(x+4)) == 'N' ) c++;
        if( *(*(pStr+(y-4))+(x-5)) == '[' && *(*(pStr+(y-4))+(x-4)) == 'N' ) c++;
        if( *(*(pStr+(y-2))+(x+7)) == '[' && *(*(pStr+(y-2))+(x+8)) == 'N' ) c++;
    }

    // PLAYER2 //
    else {
        int x = kx2, y = ky2;

        //오른쪽
        for (int i = x; i < 34; i += 4) {
            if ( *(*(pStr+i)+(x-1)) == '[') break;
            else if (*(*(pStr + y) + (i - 1)) == '<') {
                if (*(*(pStr + y) + i) == 'R' || *(*(pStr + y) + i) == 'Q') {
                    c++;
                    break;
                }
            }
        }

        //왼쪽
        for (int i = x; i >= 0; i -= 4) {
            if ( *(*(pStr+i)+(x-1)) == '[') break;
            else if (*(*(pStr + y) + (i - 1)) == '<') {
                if (*(*(pStr + y) + i) == 'R' || *(*(pStr + y) + i) == 'Q') {
                    c++;
                    break;
                }
            }
        }

        //위
        for (int i = y; i < 17; i += 2) {
            if ( *(*(pStr+i)+(x-1)) == '[') break;
            else if (*(*(pStr + i) + (x - 1)) == '<') {
                if (*(*(pStr + i) + x) == 'R' || *(*(pStr + i) + x) == 'Q') {
                    c++;
                    break;
                }
            }
        }

        //아래
        for (int i = y; i >= 0; i -= 2) {
            if ( *(*(pStr+i)+(x-1)) == '[') break;
            else if (*(*(pStr + i) + (x - 1)) == '<') {
                if (*(*(pStr + i) + x) == 'R' || *(*(pStr + i) + x) == 'Q') {
                    c++;
                    break;
                }
            }
        }

        //대각선1
        while (1) {
            x += 4; y += 2;
            if( *(*(pStr+y)+(x-1)) == '[') break;
            else if (*(*(pStr + y) + (x - 1)) == '<') {
                if (*(*(pStr + y) + (x - 1)) == 'Q' || *(*(pStr + y) + (x - 1)) == 'B') {
                    c++;
                    break;
                }
            }
            if (x > 33 || y > 16) break;
        }
        x=kx1, y=ky1;

        //대각선2
        while (1) {
            x -= 4; y += 2;
            if( *(*(pStr+y)+(x-1)) == '[') break;
            else if (*(*(pStr + y) + (x - 1)) == '<') {
                if (*(*(pStr + y) + (x - 1)) == 'Q' || *(*(pStr + y) + (x - 1)) == 'B') {
                    c++;
                    break;
                }
            }
            if (x < 0 || y > 16) break;
        }
        x=kx1, y=ky1;

        //대각선3
        int pp = 0; //pawn 확인용
        while (1) {
            x -= 4; y -= 2; pp++;
            if( *(*(pStr+y)+(x-1)) == '[') break;
            else if (*(*(pStr + y) + (x - 1)) == '<') {
                if (*(*(pStr + y) + x) == 'P' && pp == 1) {
                    c++;
                    break;
                } else if (*(*(pStr + y) + (x - 1)) == 'Q' || *(*(pStr + y) + (x - 1)) == 'B') {
                    c++;
                    break;
                }
            }
            if (x < 0 || y < 0) break;
        }
        x=kx1, y=ky1;

        //대각선4
        pp = 0; //pawn 확인용
        while (1) {
            x += 4; y -= 2; pp++;
            if( *(*(pStr+y)+(x-1)) == '[') break;
            else if (*(*(pStr + y) + (x - 1)) == '<') {
                if (*(*(pStr + y) + x) == 'P' && pp == 1) {
                    c++;
                    break;
                } else if (*(*(pStr + y) + (x - 1)) == 'Q' || *(*(pStr + y) + (x - 1)) == 'B') {
                    c++;
                    break;
                }
            }
            if (x > 33 || y < 0) break;
        }

        x=kx1, y=ky1;

        //knight
        if( *(*(pStr+(y+4))+(x+3)) == '<' && *(*(pStr+(y+4))+(x+4)) == 'N' ) c++;
        if( *(*(pStr+(y+4))+(x-5)) == '<' && *(*(pStr+(y+4))+(x-4)) == 'N' ) c++;
        if( *(*(pStr+(y-2))+(x-9)) == '<' && *(*(pStr+(y-2))+(x-8)) == 'N' ) c++;
        if( *(*(pStr+(y+2))+(x+7)) == '<' && *(*(pStr+(y+2))+(x+8)) == 'N' ) c++;
        if( *(*(pStr+(y+2))+(x-9)) == '<' && *(*(pStr+(y+2))+(x-8)) == 'N' ) c++;
        if( *(*(pStr+(y-4))+(x+3)) == '<' && *(*(pStr+(y-4))+(x+4)) == 'N' ) c++;
        if( *(*(pStr+(y-4))+(x-5)) == '<' && *(*(pStr+(y-4))+(x-4)) == 'N' ) c++;
        if( *(*(pStr+(y-2))+(x+7)) == '<' && *(*(pStr+(y-2))+(x+8)) == 'N' ) c++;
    }

    if(c != 0) return 1;
    else return 0;
}

int Checkmate(int player) {
    if(player == 1) {
        Backpan();

        for(int I=1;I<17;I+=2) {
            for(int J=2;J<34;J+=4) {
                if ( *(*(pStr+I)+(J-1)) == '<' ) {
                    switch (*(*(pStr+I)+J)) {
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
                            Checkbackpan(); //마지막에 다시 이걸로 돌려놔!
                            int checkx=-1, checky=-1; //checkx = j, checky = i
                            if(*(*(pStr+(2*i+1))+(4*j+2)) == '*') {
                                *(*(pStr+(2*i+1))+(4*j+1)) = '<';
                                *(*(pStr+(2*i+1))+(4*j+2)) = *(*(pStr+I)+J);
                                *(*(pStr+(2*i+1))+(4*j+3)) = '>';
                                *(*(pStr+I)+(J-1)) = *(*(pStr+I)+J) = *(*(pStr+I)+(J+1)) = '.';
                                Delete_s();
                                if ( Check(1) == 1 ) { checkx = j; checky = i; } //check인 경우 좌표 저장
                                Returncheckpan();
                                if (checkx >= 0) {*(*(pStr+(2*checky+1))+(4*checkx+2)) = '.';}
                            }
                        }
                    }

                    //움질일 수 있는 경로가 없는 경우 다시
                    for(int i=0;i<8;i++) {
                        for(int j=0;j<8;j++) {
                            if(*(*(pStr+(2*i+1))+(4*j+2)) == '*')
                                F += 1;
                        }
                    }

                    if(F!=0) {F=0; return 1;}
                    else {
                        for(int k=0;k<17;k++) strcpy(pan[k], backpan[k]);
                        continue;
                    }
                }
            }
        }
        if (F==0) return F;
    }

    else {
        Backpan();

        for(int I=1;I<17;I+=2) {
            for(int J=2;J<34;J+=4) {
                if ( *(*(pStr+I)+(J-1)) == '[' ) {
                    switch (*(*(pStr+I)+J)) {
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
                            Checkbackpan(); //마지막에 다시 이걸로 돌려놔!
                            int checkx=-1, checky=-1; //checkx = j, checky = i
                            if(*(*(pStr+(2*i+1))+(4*j+2)) == '*') {
                                *(*(pStr+(2*i+1))+(4*j+1)) = '[';
                                *(*(pStr+(2*i+1))+(4*j+2)) = *(*(pStr+I)+J);
                                *(*(pStr+(2*i+1))+(4*j+3)) = ']';
                                *(*(pStr+I)+(J-1)) = *(*(pStr+I)+J) = *(*(pStr+I)+(J+1)) = '.';
                                Delete_s();
                                if ( Check(2) == 1 ) { checkx = j; checky = i; } //check인 경우 좌표 저장
                                Returncheckpan();
                                if (checkx >= 0) {*(*(pStr+(2*checky+1))+(4*checkx+2)) = '.';}
                            }
                        }
                    }

                    //움질일 수 있는 경로가 없는 경우 다시
                    for(int i=0;i<8;i++) {
                        for(int j=0;j<8;j++) {
                            if(*(*(pStr+(2*i+1))+(4*j+2)) == '*')
                                F += 1;
                        }
                    }

                    if(F!=0) {F=0; return 1;}
                    else {

                        continue;
                    }
                }
            }
        }
        if (F==0) return F;
    }
}

