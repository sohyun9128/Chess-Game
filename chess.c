#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

char pan[17][34];
char backpan[17][34];
char (*pStr)[34]; //현재 맵 변경시 사용
char PAN[100] = "C:\\Users\\SoHyun Kim\\Desktop\\chess.txt";
char SPAN[100] = "C:\\Users\\SoHyun Kim\\Desktop\\chess_save.txt";
static int end;
void clear() { system("clear"); }


void Load(int); //파일 읽어서 배열, 포인터에 저장
void Print(void); //현재 판 출력
void Backpan(void); //되돌리기용 판
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
                    Move1(); clear(); Print();
                    if(end!=0) {
                        printf("\nPlayer %d WIN!*\n\n",end); break;
                    }

                    Move2(); clear(); Print();
                    if(end!=0) {
                        printf("\nPlayer %d WIN!\n\n",end); break;
                    }
                }break;
            }
            case 2: {
                Load(2); Print();
                while (1) {
                    Move1(); clear(); Print();
                    if(end!=0) {
                        printf("\nPlayer %d WIN!*\n\n",end); break;
                    }
                    Move2(); clear(); Print();
                    if(end!=0) break;
                }break;
            }
            case 3: {
                Rule();
                printf("5\n");
                Sleep(1000);
                printf("4\n");
                Sleep(1000);
                printf("3\n");
                Sleep(1000);
                printf("2\n");
                Sleep(1000);
                printf("1\n");
                Sleep(1000);
                break;
            }
            default:
                break;
        }

        if (a==4) {
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
}

void Print(void) {
    int pnum=1;
    printf("    A   B   C   D   E   F   G   H\n");
    for (int i=0;i<17;i++) {
        if (i%2==0) printf("  %s\n", *(pStr+i));
        else {printf("%d %s\n",pnum,*(pStr+i)); pnum++;}
    }
}

void Backpan(void) {
    for(int i=0;i<17;i++)
        strcpy(backpan[i], pan[i]);
}

void Rule(void) {
    printf("\n<  > :Player 1, [  ] : Player 2\n");
    printf("\nK can move one square horizontally, vertically, or diagonally.\n"
           "Q can move any number of vacant squares diagonally, horizontally, or vertically.\n"
           "R can move any number of vacant squares vertically or horizontally. It also is moved while castling.\n"
           "B can move any number of vacant squares in any diagonal direction.\n"
           "N can move one square along any rank or file and then at an angle. The knight´s movement can also be viewed as an “L” or “7″ laid out at any horizontal or vertical angle.\n"
           "P can move forward one square, if that square is unoccupied. \n(If it has not yet moved, the pawn can move two squares forward provided both squares in front of the pawn are unoccupied.)\n\n");
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

void Move1(void) {
    char before[5], after[5];
    int b1,b2,a1,a2;
    Backpan();

    //말 없는 자리 선택시 다시
    while(1) {
        printf("< Player 1 >\n(save: save game board)\nWhat? : ");
        scanf("%s", before);
        if (strcmp(before, "save") == 0) {
            Save();
            printf("S A V E . . . !\n");
            continue;
        }
        else {
            b1 = 2 * (before[1] - 48) - 1;
            b2 = 4 * (before[0] - 64) - 2;
            if (*(*(pStr + b1) + (b2-1)) == '[') { printf("again\n"); continue; }
            if (*(*(pStr + b1) + b2) == '.') { printf("again\n"); continue; }
            else {
                switch (*(*(pStr+b1)+b2)) {
                    case 'P': { Pawn(b1,b2); break;}
                    case 'R': { Rook(b1,b2); break;}
                    case 'N': { Knight(b1,b2); break;}
                    case 'B': { Bishop(b1,b2); break;}
                    case 'Q': { Queen(b1,b2); break;}
                    case 'K': { King(b1,b2); break;}
                }
                clear();
                Print();
                break;
            }
        }
    }

    //자기 말 있는 곳 선택시 다시
    while(1) {
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

    //왕 잡으면 end
    if (*(*(pStr+a1)+ a2)=='K') end=1;
    *(*(pStr+a1)+ a2) = *(*(pStr+b1)+b2);

    //자리 옮기기
    *(*(pStr+a1)+ ++a2) = '>'; a2--;
    *(*(pStr+a1)+ --a2) = '<';
    *(*(pStr+b1)+b2) = '.';
    *(*(pStr+b1)+ ++b2) = '.'; b2--;
    *(*(pStr+b1)+ --b2) = '.';
}

void Move2(void) { // 2 이동
    char before[3], after[3];
    int a1,a2,b1,b2;
    Backpan();

    // 말 없는 장소 선택시 다시
    while(1) {
        printf("[ Player 2 ]\n(save game board)\nWhat? : ");
        scanf("%s", before);
        if (strcmp(before, "save") == 0) {
            Save();
            printf("S A V E . . . !\n");
            continue;
        }
        b1 = 2 * (before[1] - 48) - 1;
        b2 = 4 * (before[0] - 64) - 2;
        if (*(*(pStr + b1) + (b2-1)) == '<') { printf("again\n"); continue; }
        if (*(*(pStr + b1) + b2) == '.') { printf("again\n"); continue; }
        else {
            switch (*(*(pStr+b1)+b2)) {
                case 'P': { Pawn(b1,b2); break;}
                case 'R': { Rook(b1,b2); break;}
                case 'N': { Knight(b1,b2); break;}
                case 'B': { Bishop(b1,b2); break;}
                case 'Q': { Queen(b1,b2); break;}
                case 'K': { King(b1,b2); break;}
            }
            clear();
            Print();
            break;
        }
    }

    //본인 말 있는 곳 선택시 다시
    while(1) {
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

    // 왕 잡으면 end
    if (*(*(pStr+a1)+ a2)=='K') end=2;

    //자리 옮기기
    *(*(pStr+a1)+ a2) = *(*(pStr+b1)+b2);
    *(*(pStr+a1)+ ++a2) = ']'; a2--;
    *(*(pStr+a1)+ --a2) = '[';
    *(*(pStr+b1)+b2) = '.';
    *(*(pStr+b1)+ ++b2) = '.'; b2--;
    *(*(pStr+b1)+ --b2) = '.';
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
            if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
        B1 = b1;
        while (1) {
            B1 -= 2;
            if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
        B1 = b1;
        while (1) {
            B2 += 4;
            if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
        B2 = b2;
        while (1) {
            B2 -= 4;
            if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
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
            if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
        B1 = b1;
        while (1) {
            B1 -= 2;
            if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
        B1 = b1;
        while (1) {
            B2 += 4;
            if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break;
        }
        B2 = b2;
        while (1) {
            B2 -= 4;
            if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*';
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
            if(*(*(pStr+B1)+B2) == '.') {
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
            if(*(*(pStr+B1)+B2) == '.') {
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
            if(*(*(pStr+B1)+B2) == '.') {
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
            if(*(*(pStr+B1)+B2) == '.') {
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
            if(*(*(pStr+B1)+B2) == '.') {
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
            if(*(*(pStr+B1)+B2) == '.') {
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
            if(*(*(pStr+B1)+B2) == '.') {
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
            if(*(*(pStr+B1)+B2) == '.') {
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
        if( *(*(pStr+(b1+4))+(b2+3)) != '<' ) *(*(pStr+(b1+4))+(b2+4)) = '*';
        if( *(*(pStr+(b1+4))+(b2-5)) != '<' ) *(*(pStr+(b1+4))+(b2-4)) = '*';
        if( *(*(pStr+(b1+2))+(b2+7)) != '<' ) *(*(pStr+(b1+2))+(b2+8)) = '*';
        if( *(*(pStr+(b1+2))+(b2-9)) != '<' ) *(*(pStr+(b1+2))+(b2-8)) = '*';
        if( *(*(pStr+(b1-4))+(b2+3)) != '<' ) *(*(pStr+(b1-4))+(b2+4)) = '*';
        if( *(*(pStr+(b1-4))+(b2-5)) != '<' ) *(*(pStr+(b1-4))+(b2-4)) = '*';
        if( *(*(pStr+(b1-2))+(b2+7)) != '<' ) *(*(pStr+(b1-2))+(b2+8)) = '*';
        if( *(*(pStr+(b1-2))+(b2-9)) != '<' ) *(*(pStr+(b1-2))+(b2-8)) = '*';
    }
    else {
        if( *(*(pStr+(b1+4))+(b2+3)) != '[' ) *(*(pStr+(b1+4))+(b2+4)) = '*';
        if( *(*(pStr+(b1+4))+(b2-5)) != '[' ) *(*(pStr+(b1+4))+(b2-4)) = '*';
        if( *(*(pStr+(b1+2))+(b2+7)) != '[' ) *(*(pStr+(b1+2))+(b2+8)) = '*';
        if( *(*(pStr+(b1+2))+(b2-9)) != '[' ) *(*(pStr+(b1+2))+(b2-8)) = '*';
        if( *(*(pStr+(b1-4))+(b2+3)) != '[' ) *(*(pStr+(b1-4))+(b2+4)) = '*';
        if( *(*(pStr+(b1-4))+(b2-5)) != '[' ) *(*(pStr+(b1-4))+(b2-4)) = '*';
        if( *(*(pStr+(b1-2))+(b2+7)) != '[' ) *(*(pStr+(b1-2))+(b2+8)) = '*';
        if( *(*(pStr+(b1-2))+(b2-9)) != '[' ) *(*(pStr+(b1-2))+(b2-8)) = '*';
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
    char input[50];
    int I=0;
    fp = fopen(SPAN, "w+");

    for(int i=0;i<17;i++) {
        fprintf(fp, "%s \n", *(pStr+i));
    }
    fclose(fp);
}