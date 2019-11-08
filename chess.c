#include <stdio.h> //printf(). scanf(), fopen(), fscanf(), fprintf() �� ���࿡ �ְ� �Ǵ� �Լ��� ����
#include <string.h> //ü���ǰ� �����͸� �����ϰ� �����ϴ� ������ �־ �ʿ��� strcpy(), strcat(), strcmp(), strlen()�� ����
#include <stdlib.h> //system("clear")�� ����
#include <termio.h> // getch() �Լ��� �����ϱ� ���� �ʿ��� tcgetattr(), ICANON, ECHO, VMIN, VTIME, tcsetattr(), TCSAFLUSH�� ����


char pan[17][34]; //�������� ü����
char backpan[17][34]; //�ǵ����� ���� ��׶��� ����� ü����
char checkbackpan[17][34]; //check���� Ȯ�� �� ����� ü����
char (*pStr)[34]; //���� �� ����� ��� (pan[0][0]�� ����Ʈ�Ѵ�)
char PAN[100] = "C:\\Users\\SoHyun Kim\\Desktop\\chess.txt"; // �� ���ӿ� ü����
char SPAN[100] = "C:\\Users\\SoHyun Kim\\Desktop\\chess_save.txt"; // ����� ü����
static int startplayer; //�����ϴ� player Ȯ�ο�
static int end; // �¸��� player Ȯ�ο� (1 : player 1 ��, 2 : player 2 ��, 3 : ��������)
static int F=0; // ������ ���� ������ �� �ִ� ��ġ�� ���� �� F=0 -> �ٽ� �� ����
static int kx1=18, ky1=1; //player 1�� King ��ǥ (check���� Ȯ�ν� �̿�)
static int kx2=18, ky2=15; //player 2�� King ��ǥ (check���� Ȯ�ν� �̿�)
static int kx3=-1, ky3=-1; //King�� �̵���ų �� check Ȯ�� ��ǥ
void clear() { system("clear"); } // clear�Լ� (ȭ���� �����ϰ� ��)
char die1[20]={""}, die2[20]={""}; // ���� �� ǥ��


void Load(int); // ������ �����ϱ� �� �ʿ��� ����
void Print(void); //���� �� ���
void Backpan(void); //�ǵ������ �� ����
void Checkbackpan(void); //�ǵ������ �� ����
void Returncheckpan(void); //�� �ǵ�����
void Rule(void); //���� ���
void Save(void); //����
void Delete_s(void); // *ǥ�� ����(. �Ǵ� ��)�� ������
void Move1(void); //Player1 �̵�
void Move2(void); //Player2 �̵�
void Pawn(int, int); //P ���ý� �̵� ���� ��ġ *�� ǥ��
void Rook(int, int); //R ���ý� �̵� ���� ��ġ *�� ǥ��
void Knight(int, int); //N ���ý� �̵� ���� ��ġ *�� ǥ��
void Bishop(int, int); //B ���ý� �̵� ���� ��ġ *�� ǥ��
void Queen(int, int); //Q ���ý� �̵� ���� ��ġ *�� ǥ��
void King(int, int); //K ���ý� �̵� ���� ��ġ *�� ǥ��
int Check(int, int); //check ���� Ȯ��
int Checkmate(int); //���� ����
int getch() {
    int ch;
    struct termios buf, save;

    tcgetattr(0, &save); // ���� �͹̳� ���� ����
    buf = save;
    buf.c_lflag &= ~(ICANON | ECHO); // CANONICAL�� ECHO ��
    buf.c_cc[VMIN] = 1; // �ּ� �Է� ���� ���� 1�� ����
    buf.c_cc[VTIME] = 0; // �ּ� �б� ��� �ð��� 0���� ����
    tcsetattr(0, TCSAFLUSH, &buf); // �͹̳ο� ���� �Է�
    ch = getchar(); // Ű���� �Է� ����
    tcsetattr(0, TCSAFLUSH, &save); // ������ �������� ����
    return ch;
}


int main() {
    int a=0; // �޴� ���ÿ� ����

    while (1) {
        clear(); //ȭ������
        if (end == 1 || end == 2) {printf("\nPlayer %d WIN!\n\n",end); end=0;} //���� �� �¸��� ǥ�� (ó�� ���� ���� �� ����Ʈ ���� ����)
        printf("+---- MENU ----+\n|  1.New game  |\n|  2.Load game |\n|    3.Help    |\n|    4.Exit    |\n+--------------+\n");
        printf("Meun Number : ");
        scanf("%d", &a); clear(); // �޴� ����
        switch (a) {
            case 1: { // New game
                Load(1); Print(); //�� ���� ���� �ε��Ͽ� ����Ʈ
                while (1) {
                    if (startplayer == 1) {
                        Move1(); //player 1 �̵�

                        if(end==1||end==2) { //���� king�� �����ų� checkmate���°� �Ǹ� ���� ����
                            break;
                        }
                        else if (end==3) {Save(); break;} //����ڰ� exit �Է½� �ڵ� ���� �� ���� ����

                        clear(); Print(); //�̵� �� ����� ü������ ������ ȭ�鿡 ����Ʈ
                        startplayer = 2;
                    }

                    if ( startplayer == 2 ){
                        Move2(); //player 2 �̵�
                        if (end == 1 || end == 2) { //���� king�� �����ų� checkmate���°� �Ǹ� ���� ����
                            break;
                        } else if (end == 3) {
                            Save();
                            break;
                        } //����ڰ� exit �Է½� �ڵ� ���� �� ���� ����

                        clear(); Print(); //�̵� �� ����� ü������ ������ ȭ�鿡 ����Ʈ
                        startplayer = 1;
                    }

                }break;
            }
            case 2: { //Load
                Load(2); Print(); //�� ���� ���� �ε��Ͽ� ����Ʈ
                while (1) {
                    if (startplayer == 1) {
                        Move1(); //player 1 �̵�

                        if(end==1||end==2) { //���� king�� �����ų� checkmate���°� �Ǹ� ���� ����
                            break;
                        }
                        else if (end==3) {Save(); break;} //����ڰ� exit �Է½� �ڵ� ���� �� ���� ����

                        clear(); Print(); //�̵� �� ����� ü������ ������ ȭ�鿡 ����Ʈ
                        startplayer = 2;
                    }

                    if ( startplayer == 2 ){
                        Move2(); //player 2 �̵�
                        if (end == 1 || end == 2) { //���� king�� �����ų� checkmate���°� �Ǹ� ���� ����
                            break;
                        } else if (end == 3) {
                            Save();
                            break;
                        } //����ڰ� exit �Է½� �ڵ� ���� �� ���� ����

                        clear(); Print(); //�̵� �� ����� ü������ ������ ȭ�鿡 ����Ʈ
                        startplayer = 1;
                    }

                }break;
            }
            case 3: { //Help (���� ���)
                Rule();
                break;
            }
            default:
                break;
        }

        if (a==4) { //���� ����
            printf("Thank you");
            break; //while�� ����
        }

    }
    return 0;
}

void Load(int k) { // ������ �����ϱ� �� �ʿ��� ���� (�� ���� ���� �� k=1, ����� ���� ���� �� k=2)
    FILE *fp = NULL; // ü������ ����� �ؽ�Ʈ ������ ������ ���� ����
    char input[50]; // ���� �� ���ڿ��� �о �迭�� �����ϱ� ���� �߰� ���ڿ�
    int I=-1; // while�� ���
    if (k==1) fp = fopen(PAN, "r"); // �� ���ӿ� �ؽ�Ʈ���� ����
    else { //����� ���ӿ� �ؽ�Ʈ���� ����
        fp = fopen(SPAN, "r");
        if (fp==NULL) { //���� ����� ����(ü����)�� ������ �� ���� ����
            printf("no savefile. new game start\n");
            fp = fopen(PAN, "r");
        }
    }
    while (fscanf(fp, "%s", input) != EOF) { //������ ���� ������ ���� �� �о input�� ����
        if (I == -1) { strcpy(die2, input); I++; }
        else if ( I>=0 && I < 17) {
            strcpy(pan[I], input); // input�� ����� ���ڿ� ü���� �迭 (pan)�� ����
            I++; //���� ���� �а� �����ϱ� ����
        }
        else if(I == 17) { strcpy(die1, input); I++; }
        else startplayer = input[0] - 48;
    } fclose(fp); //���� �ݱ�

    pStr = pan; // ������ pStr�� �迭 pan�� �����ּҸ� ����Ʈ ��

 }

void Print(void) { // ü���� ���
    int pnum = 0;
    printf("%s\n", die2); // player1�� ���� �� ���
    printf("    A   B   C   D   E   F   G   H\n"); // �� ǥ��
    for (int i=0;i<17;i++) { // ü������ ����� �迭pan�� ����Ʈ�ϴ� pStr�� �̿��Ͽ� ���� �� ���
        if (i%2==0) printf("  %s\n", *(pStr+i)); // �� ǥ��
        else { printf("%d %s\n", pnum,*(pStr+i)); pnum++;}
    }
    printf("%s\n", die1); // player2�� ���� �� ���
}

void Backpan(void) { // ��׶��� ü���ǿ� ���� �� ����
    for(int i=0;i<17;i++)
        strcpy(backpan[i], pan[i]); // ���� �� pan�� backpan�� �����Ѵ�
}

void Checkbackpan(void) { // üũ����Ȯ�� ü���ǿ� ���� �� ����
    for(int i=0;i<17;i++)
        strcpy(checkbackpan[i], pan[i]);
}

void Returncheckpan(void) { // ���� �ǿ� üũ����Ȯ�� ü���� ����
    for(int i=0;i<17;i++)
        strcpy(pan[i], checkbackpan[i]);
}

void Rule(void) { //���� ���
    printf("\n<  > :Player 1, [  ] : Player 2\n");
    printf("\nYou can move your pieces to '*'\n");
    printf("\nK can move one square horizontally, vertically, or diagonally.\n"
           "Q can move any number of vacant squares diagonally, horizontally, or vertically.\n"
           "R can move any number of vacant squares vertically or horizontally. It also is moved while castling.\n"
           "B can move any number of vacant squares in any diagonal direction.\n"
           "N can move one square along any rank or file and then at an angle. The knight��s movement can also be viewed as an ��L�� or ��7�� laid out at any horizontal or vertical angle.\n"
           "P can move forward one square, if that square is unoccupied. \n(If it has not yet moved, the pawn can move two squares forward provided both squares in front of the pawn are unoccupied.)\n\n");
    getch(); // �޴� ��� �� a�Է� ���� �� ���๮�� �Է¹���
    getch(); //����ڰ� Enter ������ ����
}

void Delete_s(void) { // �̵� �� �̵� ���� ���� üũ�ߴ� *ǥ�� ������� ��������
    char Back; // �������� ���� ����� ����

    for(int i=0;i<17;i++)
        for(int j=0;j<49;j++)
            if (*(*(pStr+i)+j)=='*') { //��� ���� üũ�Ͽ� *ǥ�ø� backpan�� ����� ���� ���ڷ� ��������
                Back = backpan[i][j];
                *(*(pStr+i)+j) = Back;
            }
}

void Move1(void) { //player 1 �̵��Լ�
    char before[5], after[3]; //�̵���ų ��, ��ġ �Է¹��� ���� (save�� exit �Է¹޾ƾ��ϴ� ��찡 �־ ũ��: 5)
    int b1=0, b2=0, a1=0, a2=0; //�̵���ų ���� ��, ��, �̵���ų ��ġ�� ��, ��
    Backpan();

    //�̵���ų �� �Է¹޾� �̵��� �� �ִ� ��� *�� ǥ���ϱ�
    while(1) {

        if(Checkmate(1) == 0) { end = 2; break;} //���� player1�� ������ �� �ִ� ���� ���� ��� player2 �¸�, ���� ����
        else { for(int k=0;k<17;k++) {strcpy(pan[k], backpan[k]);} } //�ƴϸ� �ٽ� �������� �� ����

        printf("\n< Player 1 >\n(save: save game board, exit: end game)\nWhat? : ");
        scanf("%s", before);
        if (strcmp(before, "save") == 0) { // ����ڰ� save�Է� �� ���� ü���� �ؽ�Ʈ ���Ͽ� ����
            Save();
            printf("S A V E . . . !\n");
            continue;
        }
        else if(strcmp(before, "exit") == 0) { // ����ڰ� exit�Է� �� ���� ���� �� �޴��� ���ư�
            end += 3;
            break;
        }
        else if (strlen(before) == 2) {
            b1 = 2 * (before[1] - 48) + 1; //�Է� ���� ����(���ڿ� ������ ����) �ƽ�Ű �ڵ带 �̿��ؼ� �� �ε����� ����
            b2 = 4 * (before[0] - 64) - 2; //�Է� ���� �빮�� ���ĺ� �ƽ�Ű �ڵ带 �̿��ؼ� �� �ε����� ����
            if (*(*(pStr + b1) + (b2-1)) == '[') { printf("It is not yours\n\n"); continue; } // ���� �� ���ý� �ٽ� ����
            if (*(*(pStr + b1) + b2) == '.') { printf("There are nothing\n\n"); continue; } // ����ִ� ��ġ ���ý� �ٽ� ����
            else {
                // ������ ���� ������ ���� �̵� ������ �� *���� ǥ��
                switch (*(*(pStr+b1)+b2)) {
                    case 'P': { Pawn(b1,b2); break; }
                    case 'R': { Rook(b1,b2); break; }
                    case 'N': { Knight(b1,b2); break; }
                    case 'B': { Bishop(b1,b2); break; }
                    case 'Q': { Queen(b1,b2); break; }
                    case 'K': { King(b1,b2); break; }
                }

                //�̵� �� check ���°� �Ǵ� ���� .���� ����
                for(int i=0;i<8;i++) {
                    for(int j=0;j<8;j++) {
                        Checkbackpan(); //Ȯ���� ���� �� �������� ���� ���� �����س��� (�����)
                        int checkx=-1, checky=-1; // �̵��� ������ų ��� �� �ε��� �����
                        if(*(*(pStr+(2*i+1))+(4*j+2)) == '*') { // �̵� �����ϴٰ� ǥ�õ� ������ �̵���Ű�� (�ӽ�)
                            *(*(pStr+(2*i+1))+(4*j+1)) = '<';
                            *(*(pStr+(2*i+1))+(4*j+2)) = *(*(pStr+b1)+b2);
                            *(*(pStr+(2*i+1))+(4*j+3)) = '>';
                            *(*(pStr+b1)+(b2-1)) = *(*(pStr+b1)+b2) = *(*(pStr+b1)+(b2+1)) = '.';
                            Delete_s();
                            if ( *(*(pStr+(2*i+1))+(4*j+2)) != 'K' ) {
                                if ( Check(1, 0) == 1 ) { checkx = 4*j+2; checky = 2*i+1; } //check�� ��� ��ǥ ����
                            }
                            else {
                                kx3 = 4*j+2; ky3 = 2*i+1; //������ ���� King�̸� Check�Լ����� �̿��� �������� �ٲ�� ��
                                if ( Check(1, 1) == 1 ) { checkx = 4*j+2; checky = 2*i+1; } //check�� ��� ��ǥ ����
                            }
                            Returncheckpan(); //�ٽ� ���� ������ ��������
                            if (checkx >= 0) {*(*(pStr+(checky))+(checkx)) = '.'; }
                            // ���� check�� ��찡 �־ checkx, checky�� �ε��� ���� ����Ǿ������� * -> . (�̵� ����)
                        }
                    }
                }

                // ���õ� ���� ������ �� �ִ� ��ΰ� �ִ��� Ȯ��
                for(int i=0;i<8;i++) {
                    for(int j=0;j<8;j++) {
                        if(*(*(pStr+(2*i+1))+(4*j+2)) == '*')
                            F += 1; // F >= 1 �̸� �̵� ��ų �� �ִ�
                    }
                }
                if(F == 0) { //�̵���ų �� �ִ� ��ġ�� ������ ��ġ �ٽ� �Է� �ޱ�
                    printf("You can not move that!\n\n");
                    continue;
                }

                else { //�̵� ��ų �� �ִ� ��ġ�� �ִ� ���
                    F=0; //���� �̿��� ���� �ʱ�ȭ
                    clear();
                    Print(); // �̵� ���� ��ġ ǥ�õ� ü���� ���
                    break;}
            }
        }
        else {printf("again\n"); clear(); Print(); continue;}
    }

    // ���õ� �� �̵���ų ��� �Է¹޾� �̵���Ű��
    while(1) {
        if(end == 3||end == 2) break; // �տ��� exit�� �Է¹޾Ұų� ���� ������ ������ ��� ����
        printf("\n< Player 1 >\nWhere? : ");
        scanf("%s", after);
        if (strlen(after) == 2) {
            a1 = 2 * (after[1] - 48) + 1; //�Է� ���� ����(���ڿ� ������ ����) �ƽ�Ű �ڵ带 �̿��ؼ� �� �ε����� ����
            a2 = 4 * (after[0] - 64) - 2; //�Է� ���� �빮�� ���ĺ� �ƽ�Ű �ڵ带 �̿��ؼ� �� �ε����� ����

            if( *(*(pStr+b1)+b2) =='P' && a1==15) { //Ư���� - Pawn�� ����� ü���� ���� ���� �� Q, B, N, R �� �ϳ��� ����
                char Change[2];
                clear();
                printf("change P to ? (Q or B or N or R or P) : ");
                scanf("%s", Change);
                if(Change[0] == 'Q' || Change[0] == 'B' || Change[0] == 'N' || Change[0] == 'R' || Change[0] == 'P' )
                    *(*(pStr+b1)+b2) = Change[0]; // �Է¹��� ���ڿ��� ����
                else { printf("You can not change P to %c\n", Change[0]); } // �ٸ� ���� �Է��ϸ� PASS!
            }

            if(*(*(pStr+a1)+a2) != '*') { printf("again\n"); continue; } // ������ ��ġ�� �̵� �Ұ��� �� ��ġ�� �ٽ� �Է�
            if (*(*(pStr + a1) + (a2 - 1)) == '<') { printf("again\n"); continue; } // ���� ���� �ִ� ��ġ ���� �� �ٽ� �Է�
            else { Delete_s(); break; } // �̵��� ��ġ ���������� *ǥ�� ��� ����
        }
        else {printf("again\n"); clear(); Print(); continue;}
    }

    if( end!=3 && end!=2 ) { //������ ����Ǵ� ��찡 �ƴϸ� ����

        //�̵���ų ���� King�� ��� ����� ��ǥ ���� (for checkȮ��)
        if (*(*(pStr+b1)+b2)=='K') {
            kx1 = a1; ky1 = a2;
        }

        //�� ������ end
        if (*(*(pStr+a1)+ a2)=='K') end=1;

        //�ڸ� �ű��
        if (*(*(pStr+a1)+(a2-1)) == '[') { //��븻 ���� ���� ��� ü���� ����Ʈ�� ǥ��
            switch (backpan[a1][a2]) { // "DIE-" �ڿ� �̾ ���� (strcat() �̿�)
                case 'P': { strcat(die2, "[P]"); break; }
                case 'R': { strcat(die2, "[R]"); break; }
                case 'B': { strcat(die2, "[B]"); break; }
                case 'N': { strcat(die2, "[N]"); break; }
                case 'Q': { strcat(die2, "[Q]"); break; }
            }
        }
        //��ġ ���� �� ���� �ڸ��� ...���� ǥ��
        *(*(pStr+a1)+ a2) = *(*(pStr+b1)+b2);
        *(*(pStr+a1)+ ++a2) = '>'; a2--;
        *(*(pStr+a1)+ --a2) = '<';
        *(*(pStr+b1)+b2) = '.';
        *(*(pStr+b1)+ ++b2) = '.'; b2--;
        *(*(pStr+b1)+ --b2) = '.';
    }
}

void Move2(void) { //player 2 �̵��Լ�
    char before[5], after[3];  //�̵���ų ��, ��ġ �Է¹��� ���� (save�� exit �Է¹޾ƾ��ϴ� ��찡 �־ ũ��: 5)
    int a1=0, a2=0, b1=0, b2=0; //�̵���ų ���� ��, ��, �̵���ų ��ġ�� ��, ��
    Backpan();

    //�̵���ų �� �Է¹޾� �̵��� �� �ִ� ��� *�� ǥ���ϱ�
    while(1) {

        if(Checkmate(2) == 0) { end = 1; break;} //���� player2�� ������ �� �ִ� ���� ���� ��� player1 �¸�, ���� ����
        for(int k=0;k<17;k++) {strcpy(pan[k], backpan[k]);} //�ƴϸ� �ٽ� �������� �� ����

        printf("\n[ Player 2 ]\n(save: save game board, exit: end game)\nWhat? : ");
        scanf("%s", before);
        if (strcmp(before, "save") == 0) { // ����ڰ� save�Է� �� ���� ü���� �ؽ�Ʈ ���Ͽ� ����
            Save();
            printf("S A V E . . . !\n");
            continue;
        }
        else if(strcmp(before, "exit") == 0) { // ����ڰ� exit�Է� �� ���� ���� �� �޴��� ���ư�
            end += 3;
            break;
        }
        else if (strlen(before) == 2) {
            b1 = 2 * (before[1] - 48) + 1; //�Է� ���� ����(���ڿ� ������ ����) �ƽ�Ű �ڵ带 �̿��ؼ� �� �ε����� ����
            b2 = 4 * (before[0] - 64) - 2; //�Է� ���� �빮�� ���ĺ� �ƽ�Ű �ڵ带 �̿��ؼ� �� �ε����� ����
            if (*(*(pStr + b1) + (b2-1)) == '<') { printf("It is not yours\n\n"); continue; } // ���� �� ���ý� �ٽ� ����
            if (*(*(pStr + b1) + b2) == '.') { printf("There are nothing\n\n"); continue; } // ����ִ� ��ġ ���ý� �ٽ� ����
            else {
                // ������ ���� ������ ���� �̵� ������ �� *���� ǥ��
                switch (*(*(pStr+b1)+b2)) {
                    case 'P': { Pawn(b1,b2); break; }
                    case 'R': { Rook(b1,b2); break; }
                    case 'N': { Knight(b1,b2); break; }
                    case 'B': { Bishop(b1,b2); break; }
                    case 'Q': { Queen(b1,b2); break; }
                    case 'K': { King(b1,b2); break; }
                }

                //�̵� �� check ���°� �Ǵ� ���� .���� ����
                for(int i=0;i<8;i++) {
                    for(int j=0;j<8;j++) {
                        Checkbackpan(); // Ȯ���� ���� �� �������� ���� ���� �����س��� (�����)
                        int checkx=-1, checky=-1; // �̵��� ������ų ��� �� �ε��� �����
                        if( *(*(pStr+(2*i+1))+(4*j+2)) == '*' ) { // �̵� �����ϴٰ� ǥ�õ� ������ �̵���Ű�� (�ӽ�)
                            *(*(pStr+(2*i+1))+(4*j+1)) = '[';
                            *(*(pStr+(2*i+1))+(4*j+2)) = *(*(pStr+b1)+b2);
                            *(*(pStr+(2*i+1))+(4*j+3)) = ']';
                            *(*(pStr+b1)+(b2-1)) = *(*(pStr+b1)+b2) = *(*(pStr+b1)+(b2+1)) = '.';
                            Delete_s();

                            if ( *(*(pStr+(2*i+1))+(4*j+2)) != 'K' ) {
                                if ( Check(2,0) == 1 ) { checkx = 4*j+2; checky = 2*i+1; } //check�� ��� ��ǥ ����
                            }
                            else {
                                kx3 = 4*j+2; ky3 = 2*i+1; //������ ���� King�̸� Check�Լ����� �̿��� �������� �ٲ�� ��
                                if ( Check(2,1) == 1 ) { checkx = kx3; checky = ky3; } //check�� ��� ��ǥ ����
                            }

                            Returncheckpan(); //�ٽ� ���� ������ ��������
                            if (checkx >= 0) {*(*(pStr+checky)+checkx) = '.'; }
                            // ���� check�� ��찡 �־ checkx, checky�� �ε��� ���� ����Ǿ������� * -> . (�̵� ���� ��Ű��)
                        }
                    }
                }

                // ���õ� ���� ������ �� �ִ� ��ΰ� �ִ��� Ȯ��
                for(int i=0;i<8;i++) {
                    for(int j=0;j<8;j++) {
                        if(*(*(pStr+(2*i+1))+(4*j+2)) == '*')
                            F += 1; // F >= 1 �̸� �̵� ��ų �� �ִ�
                    }
                }
                if(F == 0) { //�̵���ų �� �ִ� ��ġ�� ������ ��ġ �ٽ� �Է� �ޱ�
                    printf("You can not move that!\n");
                    continue;
                }

                else { //�̵� ��ų �� �ִ� ��ġ�� �ִ� ���
                    F=0; //���� �̿��� ���� �ʱ�ȭ
                    clear();
                    Print(); // �̵� ���� ��ġ ǥ�õ� ü���� ���
                    break;}
            }
        }
        else {printf("again\n"); clear(); Print(); continue;}
    }

    // ���õ� �� �̵���ų ��� �Է¹޾� �̵���Ű��
    while(1) {
        if (end==3 || end==1) break; // �տ��� exit�� �Է¹޾Ұų� ���� ������ ������ ��� ����
        printf("\n[ Player 2 ]\nWhere? : ");
        scanf("%s", after);
        if (strlen(after) == 2) {
            a1 = 2 * (after[1] - 48) + 1; //�Է� ���� ����(���ڿ� ������ ����) �ƽ�Ű �ڵ带 �̿��ؼ� �� �ε����� ����
            a2 = 4 * (after[0] - 64) - 2; //�Է� ���� �빮�� ���ĺ� �ƽ�Ű �ڵ带 �̿��ؼ� �� �ε����� ����

            if( *(*(pStr+b1)+b2) == 'P' && a1==1) { //Ư���� - Pawn�� ����� ü���� ���� ���� �� Q, B, N, R �� �ϳ��� ����
                char Change[2];
                clear();
                printf("change P to ? (Q or B or N or R) : ");
                scanf("%s", Change);
                if(Change[0] == 'Q' || Change[0] == 'B' || Change[0] == 'N' || Change[0] == 'R' || Change[0] == 'P' )
                    *(*(pStr+b1)+b2) = Change[0]; // �Է¹��� ���ڿ��� ����
                else { printf("You can not change P to %c\n", Change[0]); } // �ٸ� ���� �Է��ϸ� PASS!
            }

            if(*(*(pStr+a1)+a2) != '*') { printf("again\n"); continue; } // ������ ��ġ�� �̵� �Ұ��� �� ��ġ�� �ٽ� �Է�
            if (*(*(pStr + a1) + (a2 - 1)) == '[') { printf("again\n"); continue; } // ���� ���� �ִ� ��ġ ���� �� �ٽ� �Է�
            else { Delete_s(); break; } // �̵��� ��ġ ���������� *ǥ�� ��� ����
        }
        else {printf("again\n"); clear(); Print(); continue;}
    }

    if (end != 3 && end != 1) { //������ ����Ǵ� ��찡 �ƴϸ� ����

        //�̵���ų ���� King�� ��� ����� ��ǥ ���� (for checkȮ��)
        if (*(*(pStr+b1)+b2)=='K') {
            kx2 = a1; ky2 = a2;
        }

        // �� ������ end
        if (*(*(pStr+a1)+ a2)=='K') end=2;


        //�ڸ� �ű��
        if (*(*(pStr+a1)+(a2-1)) == '<') { //��븻 ���� ���� ��� ü���� ����Ʈ �� ǥ��
            switch (backpan[a1][a2]) { // "DIE-" �ڿ� �̾ ���� (strcat() �̿�)
                case 'P': { strcat(die1, "<P>"); break; }
                case 'R': { strcat(die1, "<R>"); break; }
                case 'B': { strcat(die1, "<B>"); break; }
                case 'N': { strcat(die1, "<N>"); break; }
                case 'Q': { strcat(die1, "<Q>"); break; }
            }
        }

        //��ġ ���� �� ���� �ڸ��� ...���� ǥ��
        *(*(pStr+a1)+ a2) = *(*(pStr+b1)+b2);
        *(*(pStr+a1)+ ++a2) = ']'; a2--;
        *(*(pStr+a1)+ --a2) = '[';
        *(*(pStr+b1)+b2) = '.';
        *(*(pStr+b1)+ ++b2) = '.'; b2--;
        *(*(pStr+b1)+ --b2) = '.';
    }
}

void Pawn(int b1, int b2) { // Pawn ���� �� �̵� ���� ��ġ *�� ǥ��

    if(*(*(pStr+b1)+(b2-1))=='<') { //player1�� ���
        if(*(*(pStr+(b1+2))+b2) == '.' ) { // Pawn�� �� ĭ�� �̾��ִ� ��� ������ �� ĭ ����(�� �ε��� 2 ����) ���� �ϴ�
            *(*(pStr+(b1+2))+b2) = '*';
            if( b1==3 ) {*(*(pStr+(b1+4))+b2) = '*';} // ó�� �̵� ��Ű�� ��� �� ĭ ������ �����ϴ�
        }
        if( *(*(pStr+(b1+2))+(b2-4))!='.' && *(*(pStr+(b1+2))+(b2-5))=='[' ) {
            // �� ĭ �밢���� ��븻 ���� �ִ� ��� �̵��Ͽ� ��� ���� �����ϴ�
            *(*(pStr+(b1+2))+(b2-4)) = '*';
        }
        if( *(*(pStr+(b1+2))+(b2+4)) != '.' && *(*(pStr+(b1+2))+(b2+5))==']' ) {
            // �� ĭ �밢���� ��븻 ���� �ִ� ��� �̵��Ͽ� ��� ���� �����ϴ�

            *(*(pStr+(b1+2))+(b2+4)) = '*'; }
    }
    else { //player2�� ���
        if(*(*(pStr+(b1-2))+b2) == '.') { // Pawn�� �� ĭ�� �̾��ִ� ��� ������ �� ĭ ����(�� �ε��� 2 ����) ���� �ϴ�
            *(*(pStr+(b1-2))+b2) = '*';
            if( b1==13 ) {*(*(pStr+(b1-4))+b2) = '*';} // ó�� �̵� ��Ű�� ��� �� ĭ ������ �����ϴ�
        }
        if( *(*(pStr+(b1-2))+(b2-4))!='.' && *(*(pStr+(b1-2))+(b2-5))=='<') {
            // �� ĭ �밢���� ��븻 ���� �ִ� ��� �̵��Ͽ� ��� ���� �����ϴ�
            *(*(pStr+(b1-2))+(b2-4)) = '*';
        }
        if( *(*(pStr+(b1-2))+(b2+4)) != '.' && *(*(pStr+(b1-2))+(b2+3))=='<') {
            // �� ĭ �밢���� ��븻 ���� �ִ� ��� �̵��Ͽ� ��� ���� �����ϴ�
            *(*(pStr+(b1-2))+(b2+4)) = '*';
        }
    }
}

void Rook(int b1, int b2) { // Rook ���� �� �̵� ���� ��ġ *�� ǥ��
    int B1 = b1, B2 = b2; // ������ ���� �� �ʱ�ȭ�� ����

    if (*(*(pStr + B1) + (B2 - 1)) == '<') { // Player 1�� ���
        while (1) { // ������ �̵� ����
            B1 += 2;
            if(B1 > 16) break; //ü���� ������ Ȯ��
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //����ִ� ĭ�̸� ��� �̵� ����
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
        B1 = b1; // �ٽ� ���� �ڸ���
        while (1) { // �ڷ� �̵� ����
            B1 -= 2;
            if (B1 < 0) break; //ü���� ������ Ȯ��
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //����ִ� ĭ�̸� ��� �̵� ����
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
        B1 = b1; // �ٽ� ���� �ڸ���
        while (1) { // ���������� �̵� ����
            B2 += 4;
            if (B2 > 33) break; //ü���� ������ Ȯ��
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //����ִ� ĭ�̸� ��� �̵� ����
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
        B2 = b2; // �ٽ� ���� �ڸ���
        while (1) { // �������� �̵� ����
            B2 -= 4;
            if (B2<0) break; //ü���� ������ Ȯ��
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //����ִ� ĭ�̸� ��� �̵� ����
            else if (*(*(pStr + B1) + (B2 - 1)) == '[' || *(*(pStr + B1) + B2) == '.') {
                //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
    }

    else { // Player 2�� ���
        while (1) { //�ڷ� �̵� ����
            B1 += 2;
            if(B1 > 16) break; //ü���� ������ Ȯ��
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //����ִ� ĭ�̸� ��� �̵� ����
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
        B1 = b1; // �ٽ� ���� �ڸ���
        while (1) { //������ �̵� ����
            B1 -= 2;
            if(B1<0) break; //ü���� ������ Ȯ��
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //����ִ� ĭ�̸� ��� �̵� ����
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
        B1 = b1; // �ٽ� ���� �ڸ���
        while (1) { //���������� �̵� ����
            B2 += 4;
            if (B2 > 33) break; //ü���� ������ Ȯ��
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //����ִ� ĭ�̸� ��� �̵� ����
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
        B2 = b2; // �ٽ� ���� �ڸ���
        while (1) { //�������� �̵� ����
            B2 -= 4;
            if (B2 < 0) break; //ü���� ������ Ȯ��
            else if (*(*(pStr + B1) + B2) == '.') *(*(pStr + B1) + B2) = '*'; //����ִ� ĭ�̸� ��� �̵� ����
            else if (*(*(pStr + B1) + (B2 - 1)) == '<' || *(*(pStr + B1) + B2) == '.') {
                //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr + B1) + B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
    }
}

void Bishop(int b1, int b2) { // Bishop ���� �� �̵� ���� ��ġ *�� ǥ��
    int B1 = b1, B2 = b2; // ������ ���� �� �ʱ�ȭ�� ����

    if(*(*(pStr+b1)+(b2-1))=='<') { //player1�� ���
        while (1) { // ��
            B1 += 2;
            B2 += 4;
            if (B1 > 16 || B2 > 33) break; //ü���� ������ Ȯ��
            else if(*(*(pStr+B1)+B2) == '.') { //����ִ� ĭ�̸� ��� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '[') { //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
        B1=b1; B2=b2; // �ٽ� ���� �ڸ���

        while (1) { // ��
            B1 += 2;
            B2 -= 4;
            if (B1 > 16 || B2 < 0) break; //ü���� ������ Ȯ��
            else if(*(*(pStr+B1)+B2) == '.') { //����ִ� ĭ�̸� ��� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '[') { //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
        B1=b1; B2=b2; // �ٽ� ���� �ڸ���

        while (1) { // ��
            B1 -= 2;
            B2 += 4;
            if (B1 < 0 || B2 > 33) break; //ü���� ������ Ȯ��
            else if(*(*(pStr+B1)+B2) == '.') { //����ִ� ĭ�̸� ��� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '[') { //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
        B1=b1; B2=b2; // �ٽ� ���� �ڸ���

        while (1) { // ��
            B1 -= 2;
            B2 -= 4;
            if (B1 < 0 || B2 < 0) break; //ü���� ������ Ȯ��
            else if(*(*(pStr+B1)+B2) == '.') { //����ִ� ĭ�̸� ��� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '[') { //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
    }

    else { //player2�� ���
        while (1) { // ��
            B1 += 2;
            B2 += 4;
            if (B1 > 16 || B2 > 33) break;
            else if(*(*(pStr+B1)+B2) == '.') { //����ִ� ĭ�̸� ��� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '<') { //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
        B1=b1; B2=b2; // �ٽ� ���� �ڸ���

        while (1) { // ��
            B1 += 2;
            B2 -= 4;
            if (B1 > 16 || B2 < 0) break;
            else if(*(*(pStr+B1)+B2) == '.') { //����ִ� ĭ�̸� ��� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '<') { //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
        B1=b1; B2=b2; // �ٽ� ���� �ڸ���

        while (1) { // ��
            B1 -= 2;
            B2 += 4;
            if (B1 < 0 || B2 > 33) break;
            else if(*(*(pStr+B1)+B2) == '.') { //����ִ� ĭ�̸� ��� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '<') { //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
        B1=b1; B2=b2; // �ٽ� ���� �ڸ���

        while (1) { // ��
            B1 -= 2;
            B2 -= 4;
            if (B1 < 0 || B2 < 0) break;
            else if(*(*(pStr+B1)+B2) == '.') { //����ִ� ĭ�̸� ��� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                continue;
            }
            else if(*(*(pStr+B1)+(B2-1)) == '<') { //���� ���� ������ �� ĭ���� �̵� ����
                *(*(pStr+B1)+B2) = '*';
                break;
            }
            else break; //���� ���� ������ �� ĭ���� �̵� �Ұ���
        }
    }
}

void Knight(int b1, int b2) { // Knight ���� �� �̵� ���� ��ġ *�� ǥ��
    //����ִ� ĭ�̰ų� ���� ���� �ִ� ��� �̵� ����

    if(*(*(pStr+b1)+(b2-1)) == '<') { //player1�� ��� (������ ���� �ְų� ��������� �̵�����)
        if( *(*(pStr+(b1+4))+(b2+3)) == '[' || *(*(pStr+(b1+4))+(b2+3)) == '.' ) *(*(pStr+(b1+4))+(b2+4)) = '*'; // �� �� -��
        if( *(*(pStr+(b1+4))+(b2-5)) == '[' || *(*(pStr+(b1+4))+(b2-5)) == '.' ) *(*(pStr+(b1+4))+(b2-4)) = '*'; // ��- �� ��
        if( *(*(pStr+(b1+2))+(b2+7)) == '[' || *(*(pStr+(b1+2))+(b2+7)) == '.' ) *(*(pStr+(b1+2))+(b2+8)) = '*'; // �� -�� -��
        if( *(*(pStr+(b1+2))+(b2-9)) == '[' || *(*(pStr+(b1+2))+(b2-9)) == '.' ) *(*(pStr+(b1+2))+(b2-8)) = '*'; // ��- ��- ��
        if( *(*(pStr+(b1-4))+(b2+3)) == '[' || *(*(pStr+(b1-4))+(b2+3)) == '.' ) *(*(pStr+(b1-4))+(b2+4)) = '*'; // �� �� -��
        if( *(*(pStr+(b1-4))+(b2-5)) == '[' || *(*(pStr+(b1-4))+(b2-5)) == '.' ) *(*(pStr+(b1-4))+(b2-4)) = '*'; // ��- �� ��
        if( *(*(pStr+(b1-2))+(b2+7)) == '[' || *(*(pStr+(b1-2))+(b2+7)) == '.' ) *(*(pStr+(b1-2))+(b2+8)) = '*'; // �� -�� -��
        if( *(*(pStr+(b1-2))+(b2-9)) == '[' || *(*(pStr+(b1-2))+(b2-9)) == '.' ) *(*(pStr+(b1-2))+(b2-8)) = '*'; // ��- ��- ��
    }
    else { //player2�� ��� (������ ���� �ְų� ��������� �̵�����)
        if( *(*(pStr+(b1+4))+(b2+3)) == '<' || *(*(pStr+(b1+4))+(b2+3)) == '.' ) *(*(pStr+(b1+4))+(b2+4)) = '*'; // �� �� -��
        if( *(*(pStr+(b1+4))+(b2-5)) == '<' || *(*(pStr+(b1+4))+(b2-5)) == '.' ) *(*(pStr+(b1+4))+(b2-4)) = '*'; // ��- �� ��
        if( *(*(pStr+(b1+2))+(b2+7)) == '<' || *(*(pStr+(b1+2))+(b2+7)) == '.' ) *(*(pStr+(b1+2))+(b2+8)) = '*'; // �� -�� -��
        if( *(*(pStr+(b1+2))+(b2-9)) == '<' || *(*(pStr+(b1+2))+(b2-9)) == '.' ) *(*(pStr+(b1+2))+(b2-8)) = '*'; // ��- ��- ��
        if( *(*(pStr+(b1-4))+(b2+3)) == '<' || *(*(pStr+(b1-4))+(b2+3)) == '.' ) *(*(pStr+(b1-4))+(b2+4)) = '*'; // �� �� -��
        if( *(*(pStr+(b1-4))+(b2-5)) == '<' || *(*(pStr+(b1-4))+(b2-5)) == '.' ) *(*(pStr+(b1-4))+(b2-4)) = '*'; // ��- �� ��
        if( *(*(pStr+(b1-2))+(b2+7)) == '<' || *(*(pStr+(b1-2))+(b2+7)) == '.' ) *(*(pStr+(b1-2))+(b2+8)) = '*'; // �� -�� -��
        if( *(*(pStr+(b1-2))+(b2-9)) == '<' || *(*(pStr+(b1-2))+(b2-9)) == '.' ) *(*(pStr+(b1-2))+(b2-8)) = '*'; // ��- ��- ��
    }

}

void Queen(int b1, int b2) { // Queen ���� �� �̵� ���� ��ġ *�� ǥ��
    Rook(b1,b2); // ������� �̵� ���� = Rook
    Bishop(b1,b2); // �밢������ �̵� ���� = Bishop
}

void King(int b1, int b2) { // King ���� �� �̵� ���� ��ġ *�� ǥ��
    if(*(*(pStr+b1)+(b2-1)) == '<') { //player1�� ���
        if( *(*(pStr+(b1+2))+(b2-1)) != '<' ) *(*(pStr+(b1+2))+b2) = '*'; // ��
        if( *(*(pStr+(b1-2))+(b2-1)) != '<' ) *(*(pStr+(b1-2))+b2) = '*'; // ��
        if( *(*(pStr+b1)+(b2+3)) != '<' ) *(*(pStr+b1)+(b2+4)) = '*'; // -��
        if( *(*(pStr+b1)+(b2-5)) != '<' ) *(*(pStr+b1)+(b2-4)) = '*'; // ��-
        if( *(*(pStr+(b1+2))+(b2+3)) != '<' ) *(*(pStr+(b1+2))+(b2+4)) = '*'; // �� -��
        if( *(*(pStr+(b1+2))+(b2-5)) != '<' ) *(*(pStr+(b1+2))+(b2-4)) = '*'; // ��- ��
        if( *(*(pStr+(b1-2))+(b2+3)) != '<' ) *(*(pStr+(b1-2))+(b2+4)) = '*'; // �� -��
        if( *(*(pStr+(b1-2))+(b2-5)) != '<' ) *(*(pStr+(b1-2))+(b2-4)) = '*'; // ��- ��
    }
    else { //player2�� ���
        if( *(*(pStr+(b1+2))+(b2-1)) != '[' ) *(*(pStr+(b1+2))+b2) = '*'; // ��
        if( *(*(pStr+(b1-2))+(b2-1)) != '[' ) *(*(pStr+(b1-2))+b2) = '*'; // ��
        if( *(*(pStr+b1)+(b2+3)) != '[' ) *(*(pStr+b1)+(b2+4)) = '*'; // -��
        if( *(*(pStr+b1)+(b2-5)) != '[' ) *(*(pStr+b1)+(b2-4)) = '*'; // ��-
        if( *(*(pStr+(b1+2))+(b2+3)) != '[' ) *(*(pStr+(b1+2))+(b2+4)) = '*'; // �� -��
        if( *(*(pStr+(b1+2))+(b2-5)) != '[' ) *(*(pStr+(b1+2))+(b2-4)) = '*'; // ��- ��
        if( *(*(pStr+(b1-2))+(b2+3)) != '[' ) *(*(pStr+(b1-2))+(b2+4)) = '*'; // �� -��
        if( *(*(pStr+(b1-2))+(b2-5)) != '[' ) *(*(pStr+(b1-2))+(b2-4)) = '*'; // ��- ��
    }
}

void Save(void) { //���� �������� ü���� ����
    FILE *fp = NULL; // ����� �ؽ�Ʈ ���� ������ ����
    fp = fopen(SPAN, "w+"); //chess_save.txt���� ���� (������ ���� ����)

    for(int i=-1;i<19;i++) { // ���پ� ���Ͽ� �Է��ϱ�
        if (i == -1) fprintf(fp, "%s \n", die2);
        else if (i == 17) fprintf(fp, "%s \n", die1);
        else if (i == 18) fprintf(fp, "%d", startplayer);
        else fprintf(fp, "%s \n", *(pStr+i));
    }
    fclose(fp); // ���� �ݱ�
}

int Check(int player, int king) { // ��� ���� ������ �� �ִ� ����� ���� ����ϰ� �� �� check���°� �� �� �ִ� ��찡 �ִ��� Ȯ���ϱ�
    int c=0; // check�� �� �� �ִ� ����� ��
    int x,y;

    if(player==1) { //player1�� ���
        if ( king == 1 ) { x=kx3, y=ky3; } //���� king�� ��ǥ ����
        else { x=kx1, y=ky1; } //���� king�� ��ǥ ����

        //������
        for(int i=(x+4);i<34;i+=4) { // ü���� ������ Ȯ��
            if ( *(*(pStr+i)+(x-1)) == '<') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if( *(*(pStr+y)+(i-1))  == '[') { // ���� R�̳� Q�� ������ check
                if( *(*(pStr+y)+i) == 'R' || *(*(pStr+y)+i) == 'Q' ) {
                    c++; break;
                }
            }
        }

        //����
        for(int i=(x-4);i>=0;i-=4) { // ü���� ������ Ȯ��
            if ( *(*(pStr+i)+(x-1)) == '<') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if( *(*(pStr+y)+(i-1))  == '[') { // ���� R�̳� Q�� ������ check
                if( *(*(pStr+y)+i) == 'R' || *(*(pStr+y)+i) == 'Q' ) {
                    c++; break;
                }
            }
        }

        //��
        for(int i=(y+2);i<17;i+=2) { // ü���� ������ Ȯ��
            if ( *(*(pStr+i)+(x-1)) == '<') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if( *(*(pStr+i)+(x-1))  == '[') { // ���� R�̳� Q�� ������ check
                if( *(*(pStr+i)+x) == 'R' || *(*(pStr+i)+x) == 'Q' ) {
                    c++; break;
                }
            }
        }

        //�Ʒ�
        for(int i=(y-2);i>=0;i-=2) { // ü���� ������ Ȯ��
            if ( *(*(pStr+i)+(x-1)) == '<') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if( *(*(pStr+i)+(x-1))  == '[') { // ���� R�̳� Q�� ������ check
                if( *(*(pStr+i)+x) == 'R' || *(*(pStr+i)+x) == 'Q' ) {
                    c++; break;
                }
            }
        }

        //�밢��1
        int pp=0; //pawn Ȯ�ο� (�밢�� ���� �� ĭ ��������)
        while(1) {
            x += 4; y += 2; pp++;
            if( *(*(pStr+y)+(x-1)) == '<') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if( *(*(pStr+y)+(x-1)) == '[') {
                if ( *(*(pStr+y)+x) == 'P' && pp == 1) { // �� ĭ �밢���� ���� P�� ������ check
                    c++; break;
                }
                else if( *(*(pStr+y)+(x-1)) == 'Q' || *(*(pStr+y)+(x-1)) == 'B' ) { // ���� B�� Q�� ������ check
                    c++; break;
                }
            }
            if(x>33||y>16) {break;} // ü���� ������ Ȯ��
        }

        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx1, y=ky1; } // ��ǥ ������� ��������

        //�밢��2
        pp=0; //pawn Ȯ�ο� (�밢�� ���� �� ĭ ��������)
        while(1) {
            x -= 4; y += 2; pp++;
            if( *(*(pStr+y)+(x-1)) == '<') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if( *(*(pStr+y)+(x-1)) == '[') {
                if ( *(*(pStr+y)+x) == 'P' && pp == 1) { // �� ĭ �밢���� ���� P�� ������ check
                    c++; break;
                }
                else if( *(*(pStr+y)+(x-1)) == 'Q' || *(*(pStr+y)+(x-1)) == 'B' ) { // ���� B�� Q�� ������ check
                    c++; break;
                }
            }
            if(x<0||y>16) break; // ü���� ������ Ȯ��
        }
        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx1, y=ky1; } // ��ǥ ������� ��������

        //�밢��3
        while(1) {
            x -= 4; y -= 2;
            if( *(*(pStr+y)+(x-1)) == '<') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if( *(*(pStr+y)+(x-1)) == '[') { // ���� B�� Q�� ������ check
                if( *(*(pStr+y)+(x-1)) == 'Q' || *(*(pStr+y)+(x-1)) == 'B' ) {
                    c++; break;
                }
            }
            if(x<0||y<0) break; // ü���� ������ Ȯ��
        }
        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx1, y=ky1; } // ��ǥ ������� ��������

        //�밢��4
        while(1) {
            x += 4; y -= 2;
            if( *(*(pStr+y)+(x-1)) == '<') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if( *(*(pStr+y)+(x-1)) == '[') { // ���� B�� Q�� ������ check
                if( *(*(pStr+y)+(x-1)) == 'Q' || *(*(pStr+y)+(x-1)) == 'B' ) {
                    c++; break;
                }
            }
            if(x>33||y<0) break; // ü���� ������ Ȯ��
        }

        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx1, y=ky1; } // ��ǥ ������� ��������

        //knight
        if( *(*(pStr+(y+4))+(x+3)) == '[' && *(*(pStr+(y+4))+(x+4)) == 'N' ) {
            if( (x+3)<33 && (y+4)<17) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y+4))+(x-5)) == '[' && *(*(pStr+(y+4))+(x-4)) == 'N' ) {
            if ((x-5) >= 0 && (y+4) < 17) c++; // ��- �� �� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y-2))+(x-9)) == '[' && *(*(pStr+(y-2))+(x-8)) == 'N' ) {
            if((x-9)>=0 && (y-2)>=0) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y+2))+(x+7)) == '[' && *(*(pStr+(y+2))+(x+8)) == 'N' ) {
            if((x+7)<33 && (y+2)<17) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y+2))+(x-9)) == '[' && *(*(pStr+(y+2))+(x-8)) == 'N' ) {
            if((x-9)>=0 && (y+2)<17) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y-4))+(x+3)) == '[' && *(*(pStr+(y-4))+(x+4)) == 'N' ) {
            if((x+3)<33 && (y-4)>=0) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y-4))+(x-5)) == '[' && *(*(pStr+(y-4))+(x-4)) == 'N' ) {
            if((x-5)>=0 && (y-4)>=0) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y-2))+(x+7)) == '[' && *(*(pStr+(y-2))+(x+8)) == 'N' ) {
            if((x+7)<33 && (y-2)>=0) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
    }


    else { //player2�� ���
        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx2, y=ky2; } //���� king�� ��ǥ ����

        //������
        for (int i = (x+4); i < 34; i += 4) { // ü���� ������ Ȯ��
            if ( *(*(pStr+i)+(x-1)) == '[') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if (*(*(pStr + y) + (i - 1)) == '<') { // ���� R�̳� Q�� ������ check
                if (*(*(pStr + y) + i) == 'R' || *(*(pStr + y) + i) == 'Q') {
                    c++; break;
                }
            }
        }

        //����
        for (int i = (x-4); i >= 0; i -= 4) { // ü���� ������ Ȯ��
            if ( *(*(pStr+i)+(x-1)) == '[') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if (*(*(pStr + y) + (i - 1)) == '<') { // ���� R�̳� Q�� ������ check
                if (*(*(pStr + y) + i) == 'R' || *(*(pStr + y) + i) == 'Q') {
                    c++;  break;
                }
            }
        }

        //��
        for (int i = (y+2); i < 17; i += 2) { // ü���� ������ Ȯ��
            if ( *(*(pStr+i)+(x-1)) == '[') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if (*(*(pStr + i) + (x - 1)) == '<') { // ���� R�̳� Q�� ������ check
                if (*(*(pStr + i) + x) == 'R' || *(*(pStr + i) + x) == 'Q') {
                    c++;  break;
                }
            }
        }

        //�Ʒ�
        for (int i = (y-2); i >= 0; i -= 2) { // ü���� ������ Ȯ��
            if ( *(*(pStr+i)+(x-1)) == '[') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if (*(*(pStr + i) + (x - 1)) == '<') { // ���� R�̳� Q�� ������ check
                if (*(*(pStr + i) + x) == 'R' || *(*(pStr + i) + x) == 'Q') {
                    c++;  break;
                }
            }
        }

        //�밢��1
        while (1) {
            x += 4; y += 2;
            if( *(*(pStr+y)+(x-1)) == '[') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if (*(*(pStr + y) + (x - 1)) == '<') { // ���� B�� Q�� ������ check
                if (*(*(pStr + y) + (x - 1)) == 'Q' || *(*(pStr + y) + (x - 1)) == 'B') {
                    c++;  break;
                }
            }
            if (x > 33 || y > 16) break; // ü���� ������ Ȯ��
        }
        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx2, y=ky2; } // ��ǥ ������� ��������

        //�밢��2
        while (1) {
            x -= 4; y += 2;
            if( *(*(pStr+y)+(x-1)) == '[') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if (*(*(pStr + y) + (x - 1)) == '<') { // ���� B�� Q�� ������ check
                if (*(*(pStr + y) + (x - 1)) == 'Q' || *(*(pStr + y) + (x - 1)) == 'B') {
                    c++;  break;
                }
            }
            if (x < 0 || y > 16) break; // ü���� ������ Ȯ��
        }
        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx2, y=ky2; } // ��ǥ ������� ��������

        //�밢��3
        int pp = 0; //pawn Ȯ�ο� (�밢�� ���� �� ĭ ��������)
        while (1) {
            x -= 4; y -= 2; pp++;
            if( *(*(pStr+y)+(x-1)) == '[') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if (*(*(pStr + y) + (x - 1)) == '<') {
                if (*(*(pStr + y) + x) == 'P' && pp == 1) { // �� ĭ �밢���� ���� P�� ������ check
                    c++;  break;
                }
                else if (*(*(pStr + y) + (x - 1)) == 'Q' || *(*(pStr + y) + (x - 1)) == 'B') { // ���� B�� Q�� ������ check
                    c++; break; // ü���� ������ Ȯ��
                }
            }
            if (x < 0 || y < 0) break; // ü���� ������ Ȯ��
        }
        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx2, y=ky2; } // ��ǥ ������� ��������

        //�밢��4
        pp = 0; //pawn Ȯ�ο� (�밢�� ���� �� ĭ ��������)
        while (1) {
            x += 4; y -= 2; pp++;
            if( *(*(pStr+y)+(x-1)) == '[') break; // ���� ���� ���� �����̿� ������ check �ƴ�
            else if (*(*(pStr + y) + (x - 1)) == '<') {
                if (*(*(pStr + y) + x) == 'P' && pp == 1) { // �� ĭ �밢���� ���� P�� ������ check
                    c++; break;
                }
                else if (*(*(pStr + y) + (x - 1)) == 'Q' || *(*(pStr + y) + (x - 1)) == 'B') { // ���� B�� Q�� ������ check
                    c++;  break;
                }
            }
            if (x > 33 || y < 0) break; // ü���� ������ Ȯ��
        }

        if ( king == 1 ) { x=kx3, y=ky3; }
        else { x=kx2, y=ky2; } // ��ǥ ������� ��������

        //knight
        if( *(*(pStr+(y+4))+(x+3)) == '<' && *(*(pStr+(y+4))+(x+4)) == 'N' ) {
            if( (x+3)<33 && (y+4)<17) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y+4))+(x-5)) == '<' && *(*(pStr+(y+4))+(x-4)) == 'N' ) {
            if ((x-5) >= 0 && (y+4) < 17) c++; // ��- �� �� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y-2))+(x-9)) == '<' && *(*(pStr+(y-2))+(x-8)) == 'N' ) {
            if((x-9)>=0 && (y-2)>=0) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y+2))+(x+7)) == '<' && *(*(pStr+(y+2))+(x+8)) == 'N' ) {
            if((x+7)<33 && (y+2)<17) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y+2))+(x-9)) == '<' && *(*(pStr+(y+2))+(x-8)) == 'N' ) {
            if((x-9)>=0 && (y+2)<17) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y-4))+(x+3)) == '<' && *(*(pStr+(y-4))+(x+4)) == 'N' ) {
            if((x+3)<33 && (y-4)>=0) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y-4))+(x-5)) == '<' && *(*(pStr+(y-4))+(x-4)) == 'N' ) {
            if((x-5)>=0 && (y-4)>=0) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
        if( *(*(pStr+(y-2))+(x+7)) == '<' && *(*(pStr+(y-2))+(x+8)) == 'N' ) {
            if((x+7)<33 && (y-2)>=0) c++; // �� �� -�� ��ġ�� ���� N������ check
        }
    }

    if(c != 0) return 1; // check�� ��찡 �ִ�
    else return 0; // check�� ��찡 ����
}

int Checkmate(int player) {
    if(player == 1) {
        Backpan(); // Ȯ�� �� ü������ ó�� ���·� �������� ���� ���� (���)

        for(int I=1;I<17;I+=2) {
            for(int J=2;J<34;J+=4) {
                if ( *(*(pStr+I)+(J-1)) == '<' ) { // �ǿ� �ִ� ��� player1�� ���� ������� Ȯ��
                    switch (*(*(pStr+I)+J)) { // ���� ������ ���� �̵� ������ ��ġ ǥ��
                        case 'P': { Pawn(I,J); break;}
                        case 'R': { Rook(I,J); break;}
                        case 'N': { Knight(I,J); break;}
                        case 'B': { Bishop(I,J); break;}
                        case 'Q': { Queen(I,J); break;}
                        case 'K': { King(I,J); break;}
                    }

                    //�̵� �� check ���°� �Ǵ� ���� .���� ����
                    for(int i=0;i<8;i++) {
                        for(int j=0;j<8;j++) {
                            Checkbackpan(); //Ȯ���� ���� �� �������� ���� ���� �����س��� (�����)
                            int checkx=-1, checky=-1; // �̵��� ������ų ��� �� �ε��� �����
                            if(*(*(pStr+(2*i+1))+(4*j+2)) == '*') {  // �̵� �����ϴٰ� ǥ�õ� ������ �̵���Ű�� (�ӽ�)
                                *(*(pStr+(2*i+1))+(4*j+1)) = '<';
                                *(*(pStr+(2*i+1))+(4*j+2)) = *(*(pStr+I)+J);
                                *(*(pStr+(2*i+1))+(4*j+3)) = '>';
                                *(*(pStr+I)+(J-1)) = *(*(pStr+I)+J) = *(*(pStr+I)+(J+1)) = '.';
                                Delete_s();
                                if ( *(*(pStr+(2*i+1))+(4*j+2)) != 'K' ) {
                                    if ( Check(1, 0) == 1 ) { checkx = j; checky = i; } //check�� ��� ��ǥ ����
                                }
                                else {
                                    kx3 = 4*j+2; ky3 = 2*i+1; //������ ���� King�̸� Check�Լ����� �̿��� �������� �ٲ�� ��
                                    if ( Check(1, 1) == 1 ) { checkx = j; checky = i; } //check�� ��� ��ǥ ����
                                }
                                Returncheckpan(); //�ٽ� ���� ������ ��������
                                if (checkx >= 0) {*(*(pStr+(2*checky+1))+(4*checkx+2)) = '.';}
                                // ���� check�� ��찡 �־ checkx, checky�� �ε��� ���� ����Ǿ������� * -> . (�̵� ����)
                            }
                        }
                    }

                    // ���õ� ���� ������ �� �ִ� ��ΰ� �ִ��� Ȯ��
                    for(int i=0;i<8;i++) {
                        for(int j=0;j<8;j++) {
                            if(*(*(pStr+(2*i+1))+(4*j+2)) == '*')
                                F += 1; // F >= 1�̸� ���� ������ �� �ִٴ� ���� �ǹ�
                        }
                    }

                    if(F!=0) {F=0; return 1;} // �̵��� �� �ִ� ���� ��� �ϳ� ������ checkmate�ƴ�
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
        Backpan(); // Ȯ�� �� ü������ ó�� ���·� �������� ���� ���� (���)

        for(int I=1;I<17;I+=2) {
            for(int J=2;J<34;J+=4) {
                if ( *(*(pStr+I)+(J-1)) == '[' ) { // �ǿ� �ִ� ��� player2�� ���� ������� Ȯ��
                    switch (*(*(pStr+I)+J)) { // ���� ������ ���� �̵� ������ ��ġ ǥ��
                        case 'P': { Pawn(I,J); break;}
                        case 'R': { Rook(I,J); break;}
                        case 'N': { Knight(I,J); break;}
                        case 'B': { Bishop(I,J); break;}
                        case 'Q': { Queen(I,J); break;}
                        case 'K': { King(I,J); break;}
                    }

                    //�̵� �� check ���°� �Ǵ� ���� .���� ����
                    for(int i=0;i<8;i++) {
                        for(int j=0;j<8;j++) {
                            Checkbackpan(); //Ȯ���� ���� �� �������� ���� ���� �����س��� (�����)
                            int checkx=-1, checky=-1; // �̵��� ������ų ��� �� �ε��� �����
                            if(*(*(pStr+(2*i+1))+(4*j+2)) == '*') { // �̵� �����ϴٰ� ǥ�õ� ������ �̵���Ű�� (�ӽ�)
                                *(*(pStr+(2*i+1))+(4*j+1)) = '[';
                                *(*(pStr+(2*i+1))+(4*j+2)) = *(*(pStr+I)+J);
                                *(*(pStr+(2*i+1))+(4*j+3)) = ']';
                                *(*(pStr+I)+(J-1)) = *(*(pStr+I)+J) = *(*(pStr+I)+(J+1)) = '.';
                                Delete_s();
                                if ( *(*(pStr+(2*i+1))+(4*j+2)) != 'K' ) {
                                    if ( Check(2, 0) == 1 ) { checkx = j; checky = i;} //check�� ��� ��ǥ ����
                                }
                                else {
                                    kx3 = 4*j+2; ky3 = 2*i+2; //������ ���� King�̸� Check�Լ����� �̿��� �������� �ٲ�� ��
                                    if ( Check(2, 1) == 1 ) { checkx = j; checky = i; } //check�� ��� ��ǥ ����
                                }
                                Returncheckpan(); //�ٽ� ���� ������ ��������
                                if (checkx >= 0) {*(*(pStr+(2*checky+1))+(4*checkx+2)) = '.'; }
                                //  checkx, checky�� �ε��� ���� ����Ǿ������� * -> . (�̵� ����)
                            }
                        }
                    }

                    // ���õ� ���� ������ �� �ִ� ��ΰ� �ִ��� Ȯ��
                    for(int i=0;i<8;i++) {
                        for(int j=0;j<8;j++) {
                            if(*(*(pStr+(2*i+1))+(4*j+2)) == '*')
                                F += 1; // F >= 1�̸� ���� ������ �� �ִٴ� ���� �ǹ�
                        }
                    }

                    if(F!=0) {F=0; return 1;} // �̵��� �� �ִ� ���� ��� �ϳ� ������ checkmate�ƴ�
                    else {
                        for(int k=0;k<17;k++)
                            strcpy(pan[k], backpan[k]);
                    }
                }
            }
        }
        if (F==0) return F; // checkmate�� ��� return 0;
    }
}
