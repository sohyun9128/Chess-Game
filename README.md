# Chess-Game  
  
## 메뉴  
+---- MENU ----+  
&nbsp;&nbsp;&nbsp; 1. 새 게임  
&nbsp;&nbsp;&nbsp; 2. 이어하기  
&nbsp;&nbsp;&nbsp; 3. 도움말  
&nbsp;&nbsp;&nbsp; 4. 종료  
+--------------+<br><br>

## 진행 방법  
    0. <  > :Player 1, [  ] : Player 2  
    1. 이동 시키길 원하는 말의 좌표를 입력한다.  
    2. * 표시(이동 가능 공간) 중 원하는 곳의 좌표를 입력한다.<br><br>

## 기본 맵  
DIE -  
+---+---+---+---+---+---+---+---+  
|&#60;R&#62;|&#60;N&#62;|&#60;B&#62;|&#60;Q&#62;|&#60;K&#62;|&#60;B&#62;|&#60;N&#62;|&#60;R&#62;|  
+---+---+---+---+---+---+---+---+  
|&#60;P&#62;|&#60;P&#62;|&#60;P&#62;|&#60;P&#62;|&#60;P&#62;|&#60;P&#62;|&#60;P&#62;|&#60;P&#62;|  
+---+---+---+---+---+---+---+---+  
|...|...|...|...|...|...|...|...|  
+---+---+---+---+---+---+---+---+  
|...|...|...|...|...|...|...|...|  
+---+---+---+---+---+---+---+---+  
|...|...|...|...|...|...|...|...|  
+---+---+---+---+---+---+---+---+  
|...|...|...|...|...|...|...|...|  
+---+---+---+---+---+---+---+---+  
|[P]|[P]|[P]|[P]|[P]|[P]|[P]|[P]|  
+---+---+---+---+---+---+---+---+  
|[R]|[N]|[B]|[Q]|[K]|[B]|[N]|[R]|  
+---+---+---+---+---+---+---+---+  
DIE -  
// 위 아래 DIE - 뒷 부분에 죽은 말을 표시한다.<br><br>

## 종료 조건  
 1. KING이 잡히는 경우  
 2. 체크메이트 (체크 상태를 피할 수 없는 경우)<br><br>
  

## 주요 함수 설명
&nbsp;&nbsp;    함수 이름 : LoadChessGame  
&nbsp;&nbsp;    함수 설명 : 게임을 시작하기 위해 필요한 과정을 수행한다. (텍스트 파일에서 체스판을 읽어 지정된 배열에 정보를 저장한다)  
&nbsp;&nbsp;    파라미터 이름 : mode  
&nbsp;&nbsp;    파라미터 설명  
&nbsp;&nbsp;&nbsp;        mode : 1이면 새 게임, 2면 저장된 게임  
  
&nbsp;&nbsp;    함수 이름 : ChangePiece  
&nbsp;&nbsp;    함수 설명 : 말을 이동시킨다.  
&nbsp;&nbsp;    파라미터 이름 : y, x, one, two, three  
&nbsp;&nbsp;    파라미터 설명  
&nbsp;&nbsp;&nbsp;        y, x : 이동시킬 말의 y, x좌표  
&nbsp;&nbsp;&nbsp;        one, two, three : x좌표를 중심으로 바꿀 3개의 문자  
   
&nbsp;&nbsp;    함수 이름 : MovePiece1, 2  
&nbsp;&nbsp;    함수 설명 :  
&nbsp;&nbsp;&nbsp;               A - player1,2가 말을 움직이기 전 체크메이트 여부를 확인하여 체크메이트인 경우 패배로 인지하여 게임을 종료한다.  
&nbsp;&nbsp;&nbsp;               B - player1, 2가 이동을 원하는 말의 좌표를 입력받는다.  
&nbsp;&nbsp;&nbsp;                  (SAVE 입력시 현재 게임 상황을 저장, GG 입력시 기권패 처리하여 게임을 종료한다.)  
&nbsp;&nbsp;&nbsp;               C - 입력 받은 문자열을 각각 열과 행의 인덱스 번호로 변환한다.  
&nbsp;&nbsp;&nbsp;                  (만약 입력받은 위치에 상대방의 말이 있거나 빈 공간인 경우 이동시킬 말의 좌표를 다시 입력받는다.)  
&nbsp;&nbsp;&nbsp;               D - 선택한 말의 종류에 따라 정의된 함수를 이용하여 이동 가능한 위치를 표시한다.  
&nbsp;&nbsp;&nbsp;               E - 이동이 가능한 위치 중 이동 후 체크 상태가 되는 곳은 이동을 제한한다.  
&nbsp;&nbsp;&nbsp;               F - 이동 가능한 위치가 없다면 이동시킬 말의 좌표를 다시 입력받는다.  
&nbsp;&nbsp;&nbsp;               G - 이동 시킬 위치의 좌표를 입력받아 이동가능 여부를 확인한 후 이동시키거나 좌표를 다시 입력받는다.  
&nbsp;&nbsp;&nbsp;                  (상대방의 말을 잡은 경우 해당 배열에 정보를 저장하여 이후 출력시킬 수 있도록 한다.)  
&nbsp;&nbsp;    참조 함수들 : Pawn, Rook, King, Knight, Bishop, Queen - 각 말을 선택했을 때 이동 가능한 위치 *표시   
&nbsp;&nbsp;&nbsp;                 Castling, Check - 캐슬링 가능여부나 체크여부 확인  
&nbsp;&nbsp;&nbsp;                 ChangePiece - 말 이동시키기  
  
&nbsp;&nbsp;    함수 이름 : Pawn, Rook, King, Knight, Bishop, Queen  
&nbsp;&nbsp;    함수 설명 : 이동시킬 말이 Pawn인 경우 이동 가능 위치에 '*'로 표시한다.  
&nbsp;&nbsp;    파라미터 이름 : nowY, nowX  
&nbsp;&nbsp;    파라미터 설명  
&nbsp;&nbsp;&nbsp;        nowY : 이동시킬 말의 현재 y좌표  
&nbsp;&nbsp;&nbsp;        nowX : 이동시킬 말의 현재 x좌표  
  
&nbsp;&nbsp;    함수 이름 : SaveGame  
&nbsp;&nbsp;    함수 설명 : 현재 게임 상황(게임보드, 순서, 죽은 말)을 지정한 파일에 저장한다. (파일이 존재하지 않는 경우 생성하여 저장한다.)  
    
&nbsp;&nbsp;    함수 이름 : Check  
&nbsp;&nbsp;    함수 설명 : 현재 왕의 좌표를 기준으로 체크 상태 여부를 확인한다.  
&nbsp;&nbsp;&nbsp;               1. 사방 기준으로 상대방의 말 Q 또는 R이 있는 경우 체크이다.  
&nbsp;&nbsp;&nbsp;               2. 대각선 기준으로 상대방의 말 B 또는 Q가 있는 경우 체크이다.  
&nbsp;&nbsp;&nbsp;                  (한 칸 대각선 위치에 상대방의 말 P가 있는 경우 체크이다.)  
&nbsp;&nbsp;&nbsp;               3. 상대방 말의 Knight에 의한 체크여부를 확인한다.  
&nbsp;&nbsp;    파라미터 이름 : player, king  
&nbsp;&nbsp;    파라미터 설명  
&nbsp;&nbsp;&nbsp;        player : 체크 여부를 확인하는 player ( player는 1 또는 2)   
&nbsp;&nbsp;&nbsp;        king : 이동 시킬 말이 King인 경우 이동 후 좌표를 이용하여 체크 여부를 확인하기 위해 이동 여부를 알린다. (king 이동 시 1)  
  
&nbsp;&nbsp;    함수 이름 : Checkmate  
&nbsp;&nbsp;    함수 설명 : 체크메이트 여부를 확인하여 체크메이트인 경우 해당 player를 패배처리한다.  
&nbsp;&nbsp;    파라미터 이름 : nowY, nowX  
&nbsp;&nbsp;    파라미터 설명  
&nbsp;&nbsp;&nbsp;        nowY : 이동시킬 King의 현재 y좌표  
&nbsp;&nbsp;&nbsp;        nowX : 이동시킬 King의 현재 x좌표  
&nbsp;&nbsp;    참조 함수들 : Check - 이동 가는 한 곳으로 임시 이동 시킨 후 체크여부를 확인  
 
&nbsp;&nbsp;    함수 이름 : Promotion  
&nbsp;&nbsp;    함수 설명 : 특수룰(프로모션), 상대방 진영 끝에 Pawn이 도달했을 경우 Pawn을  Q, B, N, R 중 하나로 바꿀 수 있다.  
&nbsp;&nbsp;    파라미터 이름 : beforex, beforey, aftery, player  
&nbsp;&nbsp;    파라미터 설명  
&nbsp;&nbsp;&nbsp;        beforex : player가 이동시킬 말의 x좌표  
&nbsp;&nbsp;&nbsp;        beforey : player가 이동시킬 말의 y좌표  
&nbsp;&nbsp;&nbsp;        aftery : 말을 이동시킬 위치의 y좌표  
&nbsp;&nbsp;&nbsp;        player : 실행시킬 player (1 or 2)  

&nbsp;&nbsp;    함수 이름 : Castling  
&nbsp;&nbsp;    함수 설명 : 특수룰(캐슬링), 다음과 같은 조건이 성립할 때 K와 R을 정해진 위치로 동시에 이동시킬 수 있다.  
&nbsp;&nbsp;    파라미터 이름 : player  
&nbsp;&nbsp;    파라미터 설명  
&nbsp;&nbsp;&nbsp;        player : 실행시키는 player 번호 (1 or 2)  
