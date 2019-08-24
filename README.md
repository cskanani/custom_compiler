# custom_compiler
Developed a compiler for coverting code of a custom language into c language. See **Question.pdf** file for understanding the requirements of compiler.

## Requirements
python3  
ply 3.11

## Testing the compiler
1. Navigate to assignment directory
2. Test cases are stored in folder 'test_case'
3. Uou can compile test cases to c code using two methods
    1. You can compile test cases one by one using 'cc' excutable provided with assignment and check the output
        1. 'cc' can be used as **./cc -o output_filename(default is a.c)    input_file_to_be_parsed**
    2. You can use 'compile_c' executable to compile all files in a folder and store output to a folder
        1. 'compile_c' can be used as **./compile_c    dir_where_test_cases_stored    dir_where_c_code_will_be_stored**
4. Generated c code can also be converted to object files using  two methods
    1. compile each file one by one using gcc or
    2. use 'compile_co' executable to compile all files in a folder using gcc and store output to a folder
        1. 'compile_co' can be used as **./compile_co    dir_where_c_code_is_stored    dir_where_.out_files_will_be_stored**

**NOTE : some test cases (2,3,8,9) will not generate c output and will give error in terminal, those test cases were kept intentionaly like that to check error handling capability of compiler.**

## Tese Cases
0. Code without signal and process declaration to check if empty program can be compiled with compiler using only system declaration.
1. Code with only system and signal declaration
2. To verify that a declaration cannot be empty, THIS CODE WILL GIVE A ERROR - Syntax error at ';'
3. To verify that only declared process ids can be defined, THIS CODE WILL GIVE A ERROR - Error : p is defined but is not declared as PROCESS
4. Program with a block
5. Program with a decision
6. Program with two blocks and GOTO calling
7. Program with many nested if-else
8. To verify that only defined block/decision ids can be called through GOTO statment, THIS CODE WILL GIVE A ERROR - KeyError: 'b2' referenced in ['b1'] ,but is not defined
9. To verify that first encounter of undefined block/decision id is reported and compilation stos, THIS CODE WILL GIVE A ERROR - KeyError: 'b2' referenced in ['b1'] ,but is not defined
10. Verify that code after GOTO statment is neglected
11. A simple loop using a single DECISION statment
12. Removal of unwanted (which is not being called by any other block/decision) decision
13. Verify that strings can have keywords
14. Removal of unwanted (which is not being called by any other block/decision) block
15. Program contaiing single line comment inside block
16. Program contaiing a multiline comment
17. Program contaiing comment between declaration
18. Program contaiing " inside a printf statment
19. Program contaiing keywords in comment
20. Code provided with assignment
