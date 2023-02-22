#pragma once

/*
Identifies various conditions in a character based on current char and state
Parameters:
    char ch - the character to examine
    int curr_state - current_state
Returns -
    -2: found closing mark (') for the invalid character token
    -1: invalid character token
    0: not part of an character
    1: part of a character [if curr_state was 0 then the opennig mark (') has just been found]
    2: part of a character, the 'actual character' has been found
    3: part of a character and back slash i.e. the escape character found [so the immidiate mark (') (if any) will not be the closing mark,
        also the lenght of the char has increase now
    4: end of a character, i.e. c is the closing mark
*/
int is_in_char(char c, int curr_state);



/*
Identifies various conditions in a string based on current char and state
Parameters:
    char ch - the character to examine
    int curr_state - current_state
Returns -
    0: not part of an string
    1: part of a string [if curr_state was 0 then the opennig mark (") has just been found]
    2: part of a string and back slash i.e. the escape character found [so the immidiate mark (") (if any) will not be the closing mark
    3: end of a string i.e. ch is the closing mark
*/
int is_in_str(char ch, int curr_state);



int is_in_char(char c, int curr_state)
{
    if(curr_state == 0 && c == '\''){
        curr_state = 1;
    }
    else if(curr_state == 1){
        if(c == '\'') curr_state = -2;
        else if (c == '\\')curr_state = 3;
        else curr_state = 2;
    }
    else if(curr_state == 2){
        if(c == '\'') curr_state = 4; // final state
        else curr_state = -1;
    }
    else if(curr_state == 3){
        curr_state = 2;
    }
    else if(curr_state == -1 && c == '\''){
        curr_state = -2;
    }

    return curr_state;
}



int is_in_str(char ch, int curr_state)
{
    if(curr_state == 0 && ch == '"'){ // curr_state will change from 0 only if the current character is '"'
        curr_state = 1;
    }
    else if(curr_state == 1 && ch == '\\'){
        curr_state = 2;
    }
    else if(curr_state == 2){ // escape
        curr_state = 1;
    }
    else if(curr_state == 1 && ch == '"'){
        curr_state = 3; // final_state
    }
    return curr_state;
}


template<class T1, class T2/*, class T3*/>
struct _Triple{
    T1 first;
    T2 second;
    // T3 third;
};

template<class T1, class T2>
using Triple = struct _Triple<T1, T2>;
