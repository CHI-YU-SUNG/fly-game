# FLY-GAME Progect
Author : CHI-YU-SUNG\
Practice how to use graphic mode of C language to wirte a funny shootiong game.
## Introduction
At the beginning, you will enter the main screen selection page menu. After start playing the game, a small plane and an enemy machine will be generated. The enemies can automatically move left and right and randomly produced and continuously fire bullets. Our army can press the SPACE button for firing bullets. You can go forward ,backward ,left and right to move the aircraft. Futhermore,you can eat some treasure gor addtional suprise.\

## System flow block
![alt tag](https://i.imgur.com/NFwxxvg.png)
![alt tag](https://i.imgur.com/W6hmB8p.png?1)

## Fucnction
There are different objects has been built. There will be some bombs and stars on the way. The stars or bombs will have different functions. The program size is about one thousand lines.
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
