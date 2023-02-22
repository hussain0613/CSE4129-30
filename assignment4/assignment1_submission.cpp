#include<bits/stdc++.h>
#include<ctype.h>
using namespace std;



/*
Parses a src file and filter out the extra whitespaces and comments and write it to a dest file

Parameters:
    FILE *src - the source file to be filtered
    FILE *dest - the output file where the filtered code will be written

Returns: a vector of pairs containing line number and the corresponding line
*/
vector<pair<int, string> > filter(FILE *src, FILE *dest);



/*
Detects and filters out comments in src file by moving the pointer to the end of the comment if found
i.e. after the terminating character of the comment

Parameters:
    char c - last read char from the file, to start detection with
    FILE *src - src file pointer
    int *curr_line - a integer pointer pointing to the current_line_number tracker, increments the value each time encounters a new line in the comments 

Returns - true if comment detected, false otherwise 
*/
bool skip_comments(char c, FILE *src, int *curr_line);




vector<pair<int, string> > filter(FILE *src, FILE *dest)
{
    int curr_line = 0;
    vector<pair<int, string> > lines;
    string line;

    char c;
    bool in_str = false, flag = true;
    c = fgetc(src);
    if(c != EOF) ++curr_line;
    while(c != EOF){

        flag = true;
        if(c == '"'){
            fputc(c, dest);
            line += c;

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
        else if(!in_str && skip_comments(c, src, &curr_line));
        else if(!in_str && iswspace(c)){
            fputc(' ', dest);
            if (line.size()> 0) line += ' '; // replacing many whitespaces with one space
                                            // however ignoring it if this is the first char for a 'line'
                                            // a likely scenario is when a file starts with some comment or empty line(s)
            
            if(c == '\n'){
                if(line.size() > 0) lines.push_back({curr_line, line});
                ++curr_line;
                line = "";
            }
            // printf("skipping wspaces\n");
            while( (c = fgetc(src)) != EOF && (iswspace(c) || skip_comments(c, src, &curr_line))){
                if(c == '\n'){
                    if(line.size() > 0) lines.push_back({curr_line, line}); // for cases where the multiline comments
                                                                            // start in the 'middle' of a line
                    ++curr_line;
                    line = "";
                }
            };
            flag = false; // at the end of parsing these we got a character that has not been checked
                        // so we need to check this one before fetching a new character from the src
        }
        else{
            fputc(c, dest);
            line += c;
        }

        if(flag) c = fgetc(src);
    }
    if(line.size()>0){
        lines.push_back({curr_line, line});
    }
    return lines;
}



bool skip_comments(char c, FILE *src, int *curr_line)
{
    // printf("skipping comment 1 !! curr_line: %d, char: %c\n", *(curr_line), c);
    if(c == '/'){ // now there's a chance that it might be the start of a comment
        
        c = fgetc(src);
        
        if(c != EOF){
            if(c == '/'){ // surely a single-line comment
                // printf("found single line comment\n");
                while( (c = fgetc(src)) != EOF && c != '\n');
                if(c == '\n') ++(*curr_line);
                // printf("skipped comment 1 !!\n");
                return true;
            }
            else if(c == '*'){ // surely start of a multiline comment
                // printf("found multiline comment\n");
                c = fgetc(src);
                
                // printf("in multiline comment 1, c: %c\n", c);
                while(c != EOF){
                    // printf("in multiline comment 2, c: %c\n", c);
                    if(c == '*'){
                        c = fgetc(src);
                        if(c == '/') { // end of the multiline comment
                            // printf("skipped comment 2 !!\n");
                            return true;
                        }
                        else if(c == '\n') ++(*curr_line);
                    }
                    else{
                        if(c == '\n') ++(*curr_line);
                        c = fgetc(src);
                    }
                }
                // printf("skipped comment 3 !!\n");
                return true; // multiline comment without ending
            }
            else{ // not a comment
                fseek(src, -1, SEEK_CUR); // undoing the last read operation with the src file pointer postion
                // it's not necessary when we find a comment as we want them to be skipped
                // and it's not ncessary when there's no chance of a comment as we do not do any read operation with the pointer.
            }
        }
    }
    // printf("skipped no comment 1 !!\n");
    return false;
}

