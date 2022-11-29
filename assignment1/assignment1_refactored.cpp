#include<bits/stdc++.h>
using namespace std;

/*
Parses a src file and filter out the extra whitespaces and comments and write it to a dest file
*/
void filter(FILE *src, FILE *dest);

/*
Detects and filters out comments in src file by moving the pointer to the end of the comment if found
i.e. after the terminating character of the comment

char c - last read char from the file, to start detection with
FILE *src - src file pointer
returns - true if comment detected, false otherwise 
*/
bool skip_comments(char c, FILE *src);

int main()
{
    FILE *src_fp, *output_fp;

    string fns, fnc;

    fns = "assignment1_refactored.cpp";
    fnc = "filtered_refactored.cpp";

    src_fp = fopen(fns.c_str(), "r");
    output_fp = fopen(fnc.c_str(), "w");

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

    printf("Starting Filtering...\n");

    filter(src_fp, output_fp);

    fclose(src_fp);
    fclose(output_fp);

    printf("Filtered succesfully!\n");

    return 0;
}


void filter(FILE *src, FILE *dest)
{
    char c;
    bool in_str = false, flag = true;
    c = fgetc(src);
    while(c != EOF){
        flag = true;
        if(c == '"'){
            fputc(c, dest);
            if(!fseek(src, -2L, SEEK_CUR)){
                c = fgetc(src); // reading the character before the double quotaion (") character
                // if already in_str and the previous character is a backslash (\) then it's still in the str i.e. in_str = in_str
                // or if not in_str and the previous char is a single quotation mark (') then it's still outside any str
                if( !((in_str && c == '\\') || (!in_str && c == '\'')) ) in_str ^= 1;
                fgetc(src);
            }
            else{
               in_str ^= 1; 
            }
        }
        else if(!in_str && skip_comments(c, src));
        else if(!in_str && (c == ' ' || c == '\n' || c == '\r' || c == '\t')){
            fputc(' ', dest);
            while( (c = fgetc(src)) != EOF && (c == ' ' || c == '\n' || c == '\r' || c == '\t' || skip_comments(c, src)));
            flag = false; // at the end of parsing these we got a character that has not been checked
                        // so we need to check this one before fetching a new character from the src
        }
        else{
            fputc(c, dest);
        }

        if(flag) c = fgetc(src);
    }
}

bool skip_comments(char c, FILE *src)
{
    if(c == '/'){ // now there's a chance that it might be the start of a comment
        
        c = fgetc(src);
        
        if(c != EOF){
            if(c == '/'){ // surely a single-line comment
                printf("Got a single line comment\n");
                while( (c = fgetc(src)) != EOF && c != '\n');
                return true;
            }
            else if(c == '*'){ // surely start of a multiline comment
                printf("Got a multiline comment\n");
                c = fgetc(src);
                while(c != EOF){
                    if(c == '*'){
                        c = fgetc(src);
                        if(c == '/') { // end of the multiline comment
                            return true;
                        }
                    }else{
                        c = fgetc(src);
                    }
                }
                return true; // multiline comment without ending
            }
            else{ // not a comment
                printf("Not a comment %c\n", c);
                fseek(src, -1, SEEK_CUR); // undoing the last read operation with the src file pointer postion
                // it's not necessary when we find a comment as we want them to be skipped
                // and it's not ncessary when there's no chance of a comment as we do not do any read operation with the pointer.
            }
        }
    }
    return false;
}

