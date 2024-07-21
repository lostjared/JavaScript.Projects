const cppOperators = [
    "=", "==", "!=", "<", ">", "<=", ">=", "+", "-", "*", "/", "%",
    "++", "--", "&&", "||", "!", "&", "|", "^", "~", "<<", ">>",
    "?", ":", "::", ".", "->", "[]", "()", "{}", "[]=", "->*", ".*", "+=", "-=", "*=", "/=", "%=",
    "&=", "|=", "^=", "<<=", ">>=", ",", ";", "."
];

function escapeRegExp(string) {
    return string.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
}

function removeSpaces(input) {
    const cppOperators = ["+", "-", "*", "/", "=", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "!", "&", "|", "^", "%", "<<", ">>", "++", "--"];
    const symbols = ["{", "}", "(", ")", "[", "]", ",", ":", ";", "\"", "\\"];
    for (let op of cppOperators) {
        let parts = input.split(op);
        input = parts.map(part => part.trim()).join(op);
    }
    for (let symbol of symbols) {
        let parts = input.split(symbol);
        input = parts.map(part => part.trim()).join(symbol);
    }
    let result = '';
    let previousChar = '';
    for (let i = 0; i < input.length; i++) {
        let currentChar = input[i];

        if (symbols.includes(currentChar) || symbols.includes(previousChar)) {
            if (currentChar !== ' ') {
                result += currentChar;
            }
        } else {
            result += currentChar;
        }
        previousChar = currentChar;
    }
    return result;
}


function filterString(input) {
    let left = '';
    let quoted = '';
    let right = '';
    let i = 0;

    for (i = 0; i < input.length; ++i) {

        if(input[i] == '/'  && i + 1 < input.length && input[i+1] == '/') {
            // eat comment
            while(i < input.length && input[i] != '\n')
            i++;
            continue;
        }

        if (input[i] == '\"')
            break;
        else
            left += input[i];
    }

    let rightPart = input.substring(i);
    let quotedResult = filterQuote(rightPart);

    if (quotedResult) {
        quoted = quotedResult[0];
        let quoteEndIndex = quotedResult[1];
        right = rightPart.substring(quoteEndIndex);

        return [left, quoted, right];
    }
}

function filterQuote(input) {
    let quoted = '\"';
    for (let i = 1; i < input.length; ++i) {
        if (input[i] == '\\' && i < input.length - 1) {
            quoted += input[i];
            quoted += input[i + 1];
            i += 1;
            continue;
        }
        if (input[i] == '\"') {
            quoted += '\"';
            return [quoted, i + 1];
        } else {
            quoted += input[i];
        }
    }
    return null;
}

function parseInput(input) {
    let arr = [];
    let pos = input.indexOf("\"");

    while (input.length > 0) {

        if(pos > 0 && input[pos-1] == '\\') {
            pos = input.indexOf("\"", pos + 1);
        }

         let pos2 = input.indexOf("#");
         if(pos != -1) {
            let dbl_chk = input.indexOf("\"", pos+1);
            if(dbl_chk == -1) {
                return arr;
            }
         }
        if (pos === -1) {
            let lines = input.split('\n');
            for (let line of lines) {
                if (line.trim() === '') continue;
                if (line.startsWith('#')) {
                    arr.push(line);
                } else {
                    let words = line.split('\n').filter(word => word.trim() !== '');
                    arr.push(...words);
                }
            }
            break;
        } else if(pos2 != -1 && pos2 < pos) {;
             if(input.trim().startsWith("#")) {
                    let posx = input.indexOf("\n");
                    if(posx != -1) {
                        let left = input.substring(0, posx); 
                        arr.push(left);
                        let right = input.substring(posx + 1);
                        input = right;                   
                        pos = input.indexOf("\"");
                        continue;
                    } else {
                        arr.push(input);
                        return arr;
                    }
                }
                else {

                    let nl = input.indexOf("\n");
                    if(nl != -1 && nl < pos2) {
                        let left = input.substring(0, nl);
                        let right = input.substring(nl+1);
                        let words = left.split('\n').filter(word => word.trim() !== '');
                        arr.push(...words);
                        input = right;
                        pos = input.indexOf("\"");
                        continue;
                    } else {
                        console.log(input);
                    }
                }
        }
        else {
            let filter_str = filterString(input);
            let left = filter_str[0];
            let quotedValue = filter_str[1];
            let remaining = filter_str[2];

            if (left.length > 0) {
                let lines = left.split('\n');
                for (let line of lines) {
                    if (line.trim() === '') continue;
                    if (line.trim().startsWith('#')) {
                        arr.push(line);
                    } else {
                        let words = line.split('\n').filter(word => word.trim() !== '');
                        arr.push(...words);
                    }
                }
            }

            arr.push(quotedValue.trim());

            input = remaining;
        }

        pos = input.indexOf("\"");
    }

    return arr;
}

let regular = false;

function procLine(line) {
    let output = '';
    if (line.trim() === '') {
        regular = false;
        return '';
    }
    if (line.trim().startsWith("\"")) {
        output += line;
        regular = false;
    } else if (line.trim().startsWith('#')) {
        if(regular == true) {
            output += "\n";
        }
        regular = false;
        output += line + "\n";
    } else {
        let linez = crunchLine(line) + ' ';
        linez = linez.replace(/\s\s+/g, ' ');
        linez = removeSpaces(linez);
        output += linez;
        if(linez.length > 0)
            regular = true;
    }
    return output;
}

function crunch(input) {
    let data = input;
    if(data.indexOf("\n") == -1) {
        return procLine(data);
    }
    data = removeMlComment(data);
    data = removeComment(data);
    const lines = parseInput(data);
    let output = '';
    for (let i = 0; i < lines.length; ++i) {
        let line = lines[i];
        if(line.trim() == '') continue;
        output += procLine(line);
    }
    return output;
}

function removeMlComment(text) {
    let temp = '';
    for (let i = 0; i < text.length; i++) {
        if (chkChr(text, i, '/') && chkChr(text, i + 1, '*')) {
            i++;
            do {
                i++;
            } while (!(chkChr(text, i, '*') && chkChr(text, i + 1, '/')));
            i++;
        } else {
            temp += text[i];
        }
    }
    return temp;
}

function removeComment(input) {
    let output = '';
    let i = 0;
    let inDoubleQuote = false;
    let inSingleQuote = false;

    while (i < input.length) {
        if (input[i] === '\"' && (i === 0 || input[i - 1] !== '\\') && !inSingleQuote) {
            inDoubleQuote = !inDoubleQuote;
            output += input[i];
            i++;
        } else if (input[i] === '\'' && (i === 0 || input[i - 1] !== '\\') && !inDoubleQuote) {
            inSingleQuote = !inSingleQuote;
            output += input[i];
            i++;
        } else if (!inDoubleQuote && !inSingleQuote && input[i] === '/' && i + 1 < input.length && input[i + 1] === '/') {
            while (i < input.length && input[i] !== '\n') {
                i++;
            }
        } else {
            output += input[i];
            i++;
        }
    }
    console.log(output);
    return output;
}

function chkChr(text, i, c) {
    return i < text.length && text[i] === c;
}

function crunchLine(s) {
    let output = '';
    let grabString = false;
    let grabChar = false;

    for (let i = 0; i < s.length; i++) {
        if (!grabString && !grabChar) {
            if (!testchr(s, i)) {
                continue;
            }
            if (s[i] === '/' && i + 1 < s.length && s[i + 1] === '/') {
                if (output.endsWith(' ')) {
                    output = output.slice(0, -1);
                }
                return output;
            }
        }

        if (s[i] === '\\' && (grabString || grabChar)) {
            output += s[i];
            if (i + 1 < s.length) {
                output += s[i + 1];
                i++;
            }
            continue;
        } else if (s[i] === '\"') {
            grabString = !grabString;
        } else if (s[i] === '\'' && !grabString) {
            grabChar = !grabChar;
        }

        output += s[i];
    }
    return output;
}

function testchr(s, i) {
    if (s[i] === '\t' || s[i] === '\r' || s[i] === '\n') {
        return false;
    }
    if (s[i] === ' ' && i + 1 < s.length && s[i + 1] === ' ') {
        i++;
        return false;
    }
    if (s[i] === '\\' && i + 1 < s.length && s[i + 1] !== '\\') {
        return false;
    }
    return true;
}

function crunchText() {
    const inputText = document.getElementById('inputText').value;
    const outputText = crunch(inputText);
    document.getElementById('output').innerText = outputText;
}
