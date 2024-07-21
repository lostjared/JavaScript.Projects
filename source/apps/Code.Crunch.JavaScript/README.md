# C++ Code Cruncher (compactor)

![screenshot2](https://github.com/lostjared/Code.Crunch.JavaScript/blob/main/crunch.jpg)

This project is a simple web-based tool to "crunch" C++ code by removing 
unnecessary whitespace and comments while preserving the code's 
functionality. It processes input C++ code and outputs a more compact 
version. I got this working for most C++ code. I am not going to work on this anymore
as I used this to create some videos and now am moving on.

## Motivation
I wanted to make a video with scrolling compacted C++ code so I wrote this to accomplish that.
I thought others might find it useful so I posted it here.

## Features
- Preserves preprocesscor directives.
- Removes multiline and single line comments.
- Preserves single character constants and string literals.
- Eliminates unnecessary whitespace, including double spaces and unrequired space between operaators.
- Skips empty lines and space characters before comments.

## Known Issues
- when you have // within a string under certain conditions it will cause the text to be unaligned
- does not work with most other languages
- raw strings as of now are unsupported.

if you want to fix these issues fork and submit a pull request!

## Usage

1. Open the `index.html` file in your web browser.
2. Enter your C++ code in the provided textarea.
3. Click the "Crunch" button.
4. View the processed code in the output section.

## Code Explanation

The main functionality is provided by the JavaScript code within the 
`index.html` file:

- **crunch(input)**: Main function that processes the input text, removing 
comments and crunching lines.
- **removeMlComment(text)**: Helper function to remove multiline comments.
- **chkChr(text, i, c)**: Helper function to check if a character matches 
a specified character.
- **crunchLine(s)**: Function to process each line, handling string 
literals, character constants, and removing unnecessary characters.
- **testchr(s, i)**: Helper function to test for characters that should be 
removed.
- **crunchText()**: Function to get input from the textarea, process it, 
and display the output.

## Example

Input:
```cpp
#include <iostream>

int main() {
    // This is a single line comment
    std::cout << "Hello, World!" << std::endl;  /* This is a
    multiline comment */
    char c = '\\';
    return 0;
}
```

Output:
```cpp
#include <iostream>
int main(){std::cout <<"Hello, World!"<<std::endl;char c ='\\';return 0;}
```

