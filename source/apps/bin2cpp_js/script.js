document.getElementById('converter-form').addEventListener('submit', function (e) {
    e.preventDefault();

    const inputDataElement = document.getElementById('input-data');
    const fileUploadElement = document.getElementById('file-upload');
    const variableName = document.getElementById('variable-name').value.trim();
    const conversionType = document.getElementById('conversion-type').value;
    const isUnicode = document.getElementById('unicode').checked;
    const isSorted = document.getElementById('sorted').checked;
    const outputCodeElement = document.getElementById('output-code');

    if (!variableName.match(/^[a-zA-Z_][a-zA-Z0-9_]*$/)) {
        alert('Invalid variable name. It must start with a letter or underscore and contain only letters, numbers, and underscores.');
        return;
    }

    let inputData = inputDataElement.value;

    if (fileUploadElement.files.length > 0) {
        const file = fileUploadElement.files[0];
        const reader = new FileReader();
        reader.onload = function () {
            inputData = reader.result;
            processConversion(inputData);
        };
        reader.readAsBinaryString(file);
    } else {
        processConversion(inputData);
    }

    function processConversion(data) {
        let outputCode = '';
        switch (conversionType) {
            case 'array':
                outputCode = convertToArray(variableName, data);
                break;
            case 'vector':
                outputCode = convertToVector(variableName, data);
                break;
            case 'string':
                outputCode = convertToString(variableName, data, isUnicode, isSorted);
                break;
            default:
                outputCode = '// Invalid conversion type selected.';
        }
        outputCodeElement.textContent = outputCode;
    }
});

function convertToArray(name, data) {
    const byteArray = Array.from(data, char => char.charCodeAt(0));
    const hexArray = byteArray.map(byte => `0x${byte.toString(16).toUpperCase()}`);
    const arraySize = hexArray.length;
    return `inline const std::array<unsigned char, ${arraySize}> ${name} = {\n    ${hexArray.join(', ')}\n};`;
}

function convertToVector(name, data) {
    const byteArray = Array.from(data, char => char.charCodeAt(0));
    const hexArray = byteArray.map(byte => `0x${byte.toString(16).toUpperCase()}`);
    return `inline const std::vector<unsigned char> ${name} = {\n    ${hexArray.join(', ')}\n};`;
}

function convertToString(name, data, unicode, sorted) {
    let lines = data.split(/\r?\n/).filter(line => line.trim() !== '');
    if (sorted) {
        lines.sort();
    }
    lines = lines.map(line => escapeString(line));
    const prefix = unicode ? 'L' : '';
    const quotedLines = lines.map(line => `${prefix}"${line}"`);
    const arrayType = unicode ? 'std::wstring' : 'std::string';
    return `inline const ${arrayType} ${name}[] = {\n    ${quotedLines.join(',\n    ')}\n};\ninline unsigned long ${name}_length = ${lines.length};`;
}

function escapeString(str) {
    return str.replace(/\\/g, '\\\\').replace(/"/g, '\\"').replace(/\r/g, '').replace(/\n/g, '');
}

// Copy to Clipboard Functionality
document.getElementById('copy-button').addEventListener('click', function () {
    const outputCodeElement = document.getElementById('output-code');
    const code = outputCodeElement.textContent;

    navigator.clipboard.writeText(code).then(function () {
        alert('Code copied to clipboard!');
    }, function (err) {
        alert('Failed to copy code: ' + err);
    });
});
