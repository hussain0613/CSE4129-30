#include<bits/stdc++.h>
#include<ctype.h>
using namespace std;



/*
Parses a src file and filter out the extra whitespaces and comments and write it to a dest file

Parameters:
    FILE *src - the source file to be filtered

Returns: a vector of pairs containing line number and the corresponding line
*/
vector<pair<int, string> > filter(FILE *src, FILE *dest);



/*
Detects and filters out comments in src file by moving the pointer to the end of the comment if found
i.e. after the terminating character of the comment

Parameters:
    char c - last read char from the file, to start detection with
    int *curr_line - a integer pointer pointing to the current_line_number tracker, increments the value each time encounters a new line in the comments 

Returns - true if comment detected, false otherwise 
*/
bool skip_comments(char c, FILE *src, int *curr_line);




vector<pair<int, string> > filter(FILE *src)
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
            if (line.size()> 0) line += ' '; // replacing many whitespaces with one space
                                            // however ignoring it if this is the first char for a 'line'
                                            // a likely scenario is when a file starts with some comment or empty line(s)
            
            if(c == '\n'){
                if(line.size() > 0) lines.push_back({curr_line, line});
                ++curr_line;
                line = "";
            }
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
    if(c == '/'){ // now there's a chance that it might be the start of a comment
        
        c = fgetc(src);
        
        if(c != EOF){
            if(c == '/'){ // surely a single-line comment
                while( (c = fgetc(src)) != EOF && c != '\n');
                if(c == '\n') ++(*curr_line);
                return true;
            }
            else if(c == '*'){ // surely start of a multiline comment
                c = fgetc(src);
                
                while(c != EOF){
                    if(c == '*'){
                        c = fgetc(src);
                        if(c == '/') { // end of the multiline comment
                            return true;
                        }
                        else if(c == '\n') ++(*curr_line);
                    }
                    else{
                        if(c == '\n') ++(*curr_line);
                        c = fgetc(src);
                    }
                }
                return true; // multiline comment without ending
            }
            else{ // not a comment
                fseek(src, -1, SEEK_CUR); // undoing the last read operation with the src file pointer postion
                // it's not necessary when we find a comment as we want them to be skipped
                // and it's not ncessary when there's no chance of a comment as we do not do any read operation with the pointer.
            }
        }
    }
    return false;
}

