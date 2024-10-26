function removeChars(input) {
    return input.replace(/['".]/g, ' ');
}

function generateWords() {
    const inputText = document.getElementById('inputText').value;
    const numWords = parseInt(document.getElementById('numWords').value);
    const minLength = parseInt(document.getElementById('minLength').value);
    const maxLength = parseInt(document.getElementById('maxLength').value);
    const containsString = document.getElementById('containsString').value || '';
    const underscoreWords = document.getElementById('underscoreWords').checked;
    const sortWords = document.getElementById('sortWords').checked;
    const listAllWords = document.getElementById('listAllWords').checked;

    const cleanedText = removeChars(inputText);
    let tokens = cleanedText.match(/\b[A-Za-z_][A-Za-z0-9_]*\b/g) || [];

    let wordSet = new Set();
    tokens.forEach(token => {
        let word = token;
        if (underscoreWords) {
            const index = token.indexOf('_');
            if (index !== -1) {
                word = token.substring(0, index);
            } else {
                return;
            }
        }

        const lengthValid = maxLength === -1
            ? word.length >= minLength
            : word.length >= minLength && word.length <= maxLength;

        const containsValid = containsString === '' || word.includes(containsString);

        if (lengthValid && containsValid) {
            wordSet.add(word);
        }
    });

    const wordArray = Array.from(wordSet);

    const outputDiv = document.getElementById('output');
    outputDiv.innerHTML = '';

    if (listAllWords) {
        outputDiv.innerHTML += `<h3>List of Collected Words:</h3><p>${wordArray.join(' ')}</p>`;
    }

    if (wordArray.length < numWords) {
        alert('Not enough words collected to generate the requested number of words.');
        return;
    }

    let selectedWords = [];
    const wordsCopy = [...wordArray];

    for (let i = 0; i < numWords; i++) {
        const randomIndex = Math.floor(Math.random() * wordsCopy.length);
        selectedWords.push(wordsCopy[randomIndex]);
        wordsCopy.splice(randomIndex, 1);
    }

    if (sortWords) {
        selectedWords.sort();
    }

    outputDiv.innerHTML += `<h3>Generated Words:</h3><p>${selectedWords.join(' ')}</p>`;
}
