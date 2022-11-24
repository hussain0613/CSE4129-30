#include<bits/stdc++.h>
using namespace std;

int main(){

    char cmd;
    bool flag = true;

    FILE *src_file;
    string fns;
    char c;
    
    fns = "to.txt";
    src_file = fopen(fns.c_str(), "r");
    
    while((cmd = getchar()) != 'n'){
        if(cmd == 't') {
            flag ^= 1;
            printf("flag: %d\n", flag);
        }
        else if(cmd == 'c'){
            c = fgetc(src_file);
            printf("current char: %c\n", c);
        }
        else if(cmd == 'p'){
            getchar();
            cmd = getchar();
            int delta = '0' - cmd;
            printf("cmd: %c, delta: %d\n", cmd, delta);

            // src_file -= delta;
            fseek(src_file, delta, SEEK_CUR);
            
            c = fgetc(src_file);
            printf("%d th prev char: %c\n", delta, c);
        }

    }


    return 0;
}