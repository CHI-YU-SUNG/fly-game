# FLY-GAME Progect
Author : CHI-YU-SUNG
## Introduction
一開始會進入主畫面選擇頁面選單，進入遊戲後，產生一架小飛機以及一架敵人機，敵人機可以自動左右移動以及隨機連續射出子彈，我軍則是按下空白鍵可以連續發射子彈，可以利用上下左右移動飛機 ，此外已經建好產生不同物件的class，途中會有一些炸彈以及星星的產生
吃掉星星或炸彈會有不同的功能，程式大小大概一千多行。

## System flow block
![alt tag](https://i.imgur.com/NFwxxvg.png)
![alt tag](https://i.imgur.com/W6hmB8p.png?1)

## Fucnction
* gamestate() : 利用變數值來判斷遊戲是否繼續進行，當敵機血量歸零時的判斷(如是寶物產生)或炸彈(不同於子彈)
* draw enemy() : 定義敵機移動範圍並偵測我方飛機的位置進行移動，並確認敵機的射擊方式(如三個方向的連發)
* draw background() : 背景用不同的圖形連接達到視覺上向下移動的效果，並包括遊戲視窗的邊界條件

## User Interface
![alt tag](https://i.imgur.com/fsF5zBY.png?3)
![alt tag](https://i.imgur.com/fRq5HS8.png?3)

## Reference
* Graphics mode
* C language
* http://msdn.microsoft.com/en-us/library/windows/desktop/ms646293(v=vs.85).aspx
* http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
