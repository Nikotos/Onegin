#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <algorithm>

struct String {
    char* body;
    int len;
};

const int Dict = 1, Alphabet = 2;

void mode_operator(const int mode);
int str_begin_cmp(const void* str1, const void* str2);
int str_end_cmp(const void* str1, const void* str2);
void str_sort(const int mode);
int sizeof_FILE (FILE *filek);
int str_counter (const char* heap);
void split_text (String* text, char* heap);
void write_to_file (const String* text, const int num_of_str, FILE *outputs);


int main() {
    printf("Line Sorter;\n"
                   "Lets start\n");
    printf("Some Tips:\n"
                   "press '1' to choose the rhyming dictionary\n"
                   "press '2' to choose Alphabetical sort\n");
    int mode = 0;
    scanf("%d", &mode);
    assert(mode != 0);
    mode_operator(mode);
    return 0;
}

//-----------------------------------------------------------------------
//! choosing mode of program
//!
//! @param [in] mode   mode of sorting
//-----------------------------------------------------------------------
void mode_operator(const int mode){
    switch(mode){
        case Dict:
            str_sort(Dict);
            printf("The rhyming dictionary\n"
                           "Done\n");
         break;
        case Alphabet:
            str_sort(Alphabet);
            printf("The Alphabet\n"
                           "Done\n");
            break;
        default:
            printf("OOOOOPS!\n"
                           "You've pressed something wrong\n");
    }
}

//----------------------------------------------------------------------------
//! string's beginings comparator
//! @param [in] str1
//! @param [in] str2
//!
//! @return number > 0 if str1 > str2
//! @return number < 0 if str1 < str2
//! @return 0 if str1 == str2
//!
//! @note ignore spaces and punctuation marks
//----------------------------------------------------------------------------
int str_begin_cmp(const void* str1, const void* str2){
    assert(str1 != NULL);
    assert(str2 != NULL);
    String s1 = *(String*)str1;
    String s2 = *(String*)str2;
    while (ispunct(*s1.body) || isspace(*s1.body)) (s1.body)++;
    while (ispunct(*s2.body) || isspace(*s2.body)) (s2.body)++;
    return strcmp(s1.body, s2.body);
}

//----------------------------------------------------------------------------
//! String's endings comparator
//! @param [in] str1
//! @param [in] str2
//!
//! @return number > 0 if str1 > str2
//! @return number < 0 if str1 < str2
//! @return 0 if str1 == str2
//!
//! @note ignore spaces and punctuation marks
//----------------------------------------------------------------------------
int str_end_cmp(const void* str1, const void* str2){
    assert(str1 != NULL);
    assert(str2 != NULL);
    String s1 = *(String*)str1;
    String s2 = *(String*)str2;
    int i = 0, j = 0;
    if (s1.body == s2.body) return 0;
    const int max_len = std::max(s1.len, s2.len);
    while ((i < max_len) && (j < max_len)){
        if (ispunct(s1.body[s1.len - i - 1]) ||
            isspace(s1.body[s1.len - i - 1])) i++;
        else if (ispunct(s2.body[s2.len - j - 1]) ||
                 isspace(s2.body[s2.len - j - 1])) j++;
        else {
            const int difference = s1.body[s1.len - i - 1] -
                                  s2.body[s2.len - j - 1];
            if (difference != 0) return difference;
            else{
                i++;
                j++;
            }
        }
    }
    if (i == s1.len) return -1;
    else return 1;
}

//----------------------------------------------------------------------
//! Strings sorting
//!
//! @param [in] mode     mode of sorting
//!
//! @note take rows from input file, sort them and put into output file
//! @note "input.txt" - input file
//! @note "output.txt" - output file
//----------------------------------------------------------------------
void str_sort(const int mode) {
    FILE* inputs = fopen("input.txt","r");
    assert(inputs);
    assert(inputs != NULL);

    FILE* outputs = fopen("output.txt", "w");
    assert(outputs);
    assert(outputs != NULL);

    const int file_len = sizeof_FILE (inputs);
    assert (file_len > -1);

    char* heap = (char*) calloc (file_len + 1, sizeof (*heap));
    assert(heap != NULL);
    fread (heap, file_len, sizeof (*heap), inputs);

    const int num_of_str = str_counter(heap);
    assert (num_of_str > 0);

    String* text = (String*) calloc(num_of_str, sizeof (*text));
    assert(text != NULL);
    split_text (text, heap);

    switch (mode){
        case Dict:
            qsort (text, num_of_str, sizeof (*text), str_end_cmp);
            break;
        case Alphabet:
            qsort (text, num_of_str, sizeof (*text), str_begin_cmp);
            break;
    }
    write_to_file (text, num_of_str, outputs);

    fclose (inputs);
    fclose (outputs);

}
//---------------------------------------------------
//! size of input file
//!
//! @param [in] filek     input file
//!
//! @return number of symbols in input file
//----------------------------------------------------
int sizeof_FILE (FILE *filek){
    fseek (filek, 0, SEEK_END);
    const int file_length = ftell (filek);
    rewind (filek);
    return file_length;
}

//-----------------------------------------------------------------------
//! Number of lines in the input file
//!
//! @param [in] heap     pointer on first symbol of text from input file
//!
//! @return number of strings in input file
//-----------------------------------------------------------------------
int str_counter (const char* heap){
    int num_of_str = 1, i = 0;
    while (heap[i] != '\0'){
        if (heap[i] == '\n') num_of_str++;
        i++;
    }
    return num_of_str;
}
//------------------------------------------------------------------------
//! split big text to array of strings
//!
//! @param [in] heap     pointer on a first symbol of a big text from input file
//! @param [out] text      array of Strings
//------------------------------------------------------------------------
void split_text (String* text, char* heap) {
    assert(text != NULL);
    assert(heap != NULL);
    int j = 0; //index for array of strings
    text[j].body = heap;
    j++;
    int i = 0; //index for heap
    while (heap[i] != '\0') {
        if (heap[i] == '\n') {
            heap[i] = '\0';
            assert (heap[i] != '\n');
            assert (heap[i] == '\0');
            text[j - 1].len = heap + i - text[j - 1].body + 1;
            text[j].body = heap + i + 1;
            j++;
        }
        i++;
    }
    text[j - 1].len = heap + i - text[j - 1].body + 1;
    assert (j != 0);
}

//------------------------------------------------------------------
//! Write sorted strings into output file
//!
//! @param [in]  text             array of Strings
//! @param [in]  num_of_str       number of strings
//! @param [in] outputs          output file
//!
//! @note ignore empty strings
//-------------------------------------------------------------------
void write_to_file (const String* text, const int num_of_str, FILE *outputs){
    assert(text != NULL);
    assert(num_of_str > 0);
    const int min_str_size = 2;
    for (int i = 0; i < num_of_str; i++){
        if (text[i].len > min_str_size){
            fprintf (outputs, "%s\n", text[i].body);
        }
    }
}


