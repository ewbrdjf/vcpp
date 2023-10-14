#include <stdio.h>
#include <iostream>
#include <conio.h>
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80 // 매크로로 좌우상하를 설정하기

int main()
{
    char input;
    char exit;

    while (1) {
        printf("기술을 입력하세요:\n");
        input = _getch();

        if (input == 27) {
            return (0);
        }
        else if (input == -32) {
            input = _getch();
        }
        printf("\n");

        if (input == 13) {
            continue;
        }
        while (1) {
            char enter = _getch();
            if (enter == '\r')
                break;
        }

        switch (input) {
        case UP:
            printf("아도겐=0\n");
            break;
        case DOWN:
            printf("장풍=(\n");
            break;
        case LEFT:
            printf("발차기0==(\n");
            break;
        case RIGHT:
            printf("인사하기^^7(\n");
            break;
        default:
            printf("잘못된 입력입니다. 다시 시도하세요.\n");
        }
    }
    return 0;
}