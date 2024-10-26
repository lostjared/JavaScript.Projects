async function startEcho() {
    const text = document.getElementById("inputText").value;
    const delay = parseInt(document.getElementById("delayTime").value) || 100;
    const output = document.getElementById("output");
    output.textContent = ''; // Clear previous output

    // Loop through each character in the text and append it with a delay
    for (const char of text) {
        output.textContent += char;
        output.scrollTop = output.scrollHeight; // Scroll to the bottom
        await new Promise(resolve => setTimeout(resolve, delay));
    }
}
