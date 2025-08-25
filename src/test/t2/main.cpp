

#include <stdio.h>
#include <windows.h>

int
main(int, char*[])
{
    printf("Hello, t2\n");

    UINT out_put_cp = GetConsoleOutputCP();
    UINT in_put_cp  = GetConsoleCP();

    char 中文[] = "测试用中文。";

    // 输出当前控制台代码页
    printf("Current Console Output Code Page: %u\n", out_put_cp);
    printf("Current Console Input Code Page: %u\n", in_put_cp);

    printf("Test output: %s\n", 中文);


    // 改变控制台代码页
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    printf("Current Console Output Code Page: %u\n", GetConsoleOutputCP());
    printf("Current Console Input Code Page: %u\n", GetConsoleCP());

    printf("Test output: %s\n", 中文);

    printf("\n");
    int 针 = 1;
    while(中文[针] != '\0')
    {
        printf("%c", 中文[针]);
        针++;
    }

    return 0;
}
