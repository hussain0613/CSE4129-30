#include<bits/stdc++.h>
using namespace std;


bool parse(FILE *src, FILE *dest);
bool parse_whitespaces(FILE *src, FILE *dest);
bool parse_comments(FILE *src, FILE *dest);

int main()
{
    FILE *src_file, *cmpld_file;

    string fns, fnc;

    char c;

    bool in_str = false, escape = false, flag = true;
    
    fns = "assignment1.cpp";
    fnc = "filtered.cpp";

    src_file = fopen(fns.c_str(), "r");
    cmpld_file = fopen(fnc.c_str(), "w");

    // cmpld_file = stdout;

    // Testing singleline comments: "not a string"
    
    /*
    Testing multiline comments: "not a string again."
    */

    printf("Test string... \n");
    printf("\t not any true whitespaces   in the source code \t.\n");
    printf("\t //not an actual single line comment\n");
    printf("\t /* not an actual multi line comment */\n");
    printf("\t \" not an actual end of string\n");
    printf("\t not an actual char in the source code '");
    printf("\n");

    printf("Starting 'Compiling'...\n");

    c = fgetc(src_file);
    while(c != EOF){
        flag = true;
        if(c == '"'){
            fputc(c, cmpld_file);
            if(!fseek(src_file, -2L, SEEK_CUR)){
                c = fgetc(src_file); // the character before the double quotaion (") character
                // if already in_str and the previous character is a backslash (\) then it's still in the str i.e. in_str = in_str
                // or if not in_str and the previous char is a single quotation mark (') then it's still outside any str
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
                    printf("Got a single line comment\n");
                    while( (c = fgetc(src_file)) != EOF && c != '\n');
                }
                else if(c == '*'){
                    printf("Got a multiline comment\n");
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
                    printf("Not a comment %c\n", c);
                    fputc('/', cmpld_file);
                    // if(c!= EOF) fputc(c, cmpld_file);
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

    printf("'Compiled' succesfully!\n");

    return 0;
}