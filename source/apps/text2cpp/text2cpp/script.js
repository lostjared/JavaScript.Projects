function escapeString(input) {
    return input.replace(/\\/g, '\\\\').replace(/"/g, '\\"');
}

function isValidVariableName(name) {
    return /^[A-Za-z_][A-Za-z0-9_]*$/.test(name);
}

function convertText() {
    const inputText = document.getElementById("inputText").value;
    const varName = document.getElementById("varName").value.trim();
    const outputType = document.querySelector('input[name="outputType"]:checked').value;
    const skipBlanks = document.getElementById("skipBlanks").checked;
    const sortType = document.querySelector('input[name="sortType"]:checked').value;

    if (!isValidVariableName(varName)) {
        document.getElementById("output").textContent = "Invalid variable name. Must start with a letter or underscore and contain only letters, digits, or underscores.";
        return;
    }

    // Split lines, handling different line endings
    let lines = inputText.split(/\r?\n/);

    if (skipBlanks) {
        lines = lines.filter(line => line.trim().length > 0);
    }

    // Sort lines if required by sortType
    if (sortType === 'asc') {
        lines.sort((a, b) => a.localeCompare(b));
    } else if (sortType === 'desc') {
        lines.sort((a, b) => b.localeCompare(a));
    }

    let output = "";
    if (outputType === 'string') {
        output += `std::vector<std::string> ${varName}_arr = {\n`;
        output += lines.map(line => `    "${escapeString(line)}"`).join(',\n');
        output += `\n};\nconst size_t ${varName}_size = ${lines.length};\n`;
    } else if (outputType === 'char') {
        output += `const char* ${varName}_arr[] = {\n`;
        output += lines.map(line => `    "${escapeString(line)}"`).join(',\n');
        output += `\n};\nconst size_t ${varName}_size = ${lines.length};\n`;
    } else if (outputType === 'binary') {
        output += `std::vector<unsigned char> ${varName}_arr = {\n`;
        let hexValues = lines.flatMap(line => 
            Array.from(line).map(c => `0x${c.charCodeAt(0).toString(16).padStart(2, '0')}`)
        );
        output += '    ' + hexValues.join(', ');
        output += `\n};\n`;
    }

    document.getElementById("output").textContent = output;
}
