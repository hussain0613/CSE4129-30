#include<bits/stdc++.h>
#include<ctype.h>
using namespace std;

int main()
{
    FILE *f = fopen("s1le1_input.txt", "r");

    string f_cont = "", temp = "";
    char c;// = fgetc(f);
    int state = 1, i, j;
    while((c = fgetc(f)) != EOF){
        f_cont += c;
    }
    fclose(f);

    for(i = 0; i < f_cont.size(); ++i){
        c = f_cont[i];
        if(c == '#' && (i == 0 || iswspace(f_cont[i-1]))){
            temp += c;
            for(j = i+1; !iswspace(f_cont[j]) && j < f_cont.size(); ++j){
                
            }
        }
    }

    return 0;
}