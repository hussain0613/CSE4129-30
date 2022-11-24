#include<bits/stdc++.h>
using namespace std;

int main()
{
    FILE *src_file, *cmpld_file;

    string fns, fnc;

    char c;

    bool in_str = false, escape = false, flag = true;
    
    cout << "Path to source file: ";
    cin >> fns;
    
    fnc = "filtered_output.txt";

    src_file = fopen(fns.c_str(), "r");
    cmpld_file = fopen(fnc.c_str(), "w");

    printf("Starting filtering...\n");

    c = fgetc(src_file);
    while(c != EOF){
        flag = true;
        if(c == '"'){
            fputc(c, cmpld_file);
            if(!fseek(src_file, -2L, SEEK_CUR)){
                c = fgetc(src_file); 
                if( ((in_str && c == '\\') || (!in_str && c == '\'')) ) in_str ^= 0;
                else in_str ^= 1;
                fgetc(src_file);
            }
            else{
               in_str ^= 1; 
            }
        }
        else if(!in_str && c == '/'){
            c = fgetc(src_file);
            flag = false;

            if(c != EOF){
                if(c == '/'){
                    while( (c = fgetc(src_file)) != EOF && c != '\n');
                }
                else if(c == '*'){
                    while( (c=fgetc(src_file)) != EOF){
                        if(c == '*'){
                            c = fgetc(src_file);
                            if(c != EOF && c == '/'){
                                flag = true;
                                break;
                            };
                        }
                    }
                }
                else{
                    fputc('/', cmpld_file);
                }
            }
            else{
                break;
            }
        }
        else if(!in_str && (c == ' ' || c == '\n' || c == '\r' || c == '\t')){
            fputc(' ', cmpld_file);
            while( (c = fgetc(src_file)) != EOF && (c == ' ' || c == '\n' || c == '\r' || c == '\t'));
            flag = false;
        }
        else{
            fputc(c, cmpld_file);
        }

        if(flag) c = fgetc(src_file);
    }

    printf("Done!\n");

    return 0;
}
