
window.onload = function() {
    const canvas = document.getElementById('matrixCanvas');
    const context = canvas.getContext('2d');

    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;

    const screenWidth = canvas.width;
    const screenHeight = canvas.height;

    const fontSize = 24;
    context.font = `${fontSize}px 'Noto Sans JP'`;
    context.textAlign = 'center';
    context.textBaseline = 'top';

    const metrics = context.measureText('あ');
    const charWidth = metrics.width;
    const charHeight = fontSize; // Approximate height

    const numColumns = Math.floor(screenWidth / charWidth) + 1;
    const numRows = Math.floor(screenHeight / charHeight) + 1;

    const fallPositions = new Array(numColumns);
    const fallSpeeds = new Array(numColumns);
    const trailLengths = new Array(numColumns);

    for (let col = 0; col < numColumns; col++) {
        fallPositions[col] = Math.random() * numRows;
        fallSpeeds[col] = (Math.random() * 7 + 3) * 2.0; // Speed multiplier is 2.0
        trailLengths[col] = Math.floor(Math.random() * 15 + 5);
    }

    const codepointRanges = [
        [0x3041, 0x3096], // Hiragana
        [0x30A0, 0x30FF]  // Katakana
    ];
    function getRandomCharacter() {
        const rangeIndex = Math.floor(Math.random() * codepointRanges.length);
        const range = codepointRanges[rangeIndex];
        const start = range[0];
        const end = range[1];
        const codepoint = start + Math.floor(Math.random() * (end - start + 1));
        return String.fromCodePoint(codepoint);
    }

    function computeTrailColor(trailOffset, trailLength) {
        let intensity = 1.0 - trailOffset / trailLength;
        if (intensity < 0.0) intensity = 0.0;

        let alpha = 255 * intensity;
        if (alpha < 50) alpha = 50;

        let green = 255 * intensity;
        if (green < 100) green = 100;

        return `rgba(0, ${green}, 0, ${alpha / 255})`;
    }

    let lastTime = performance.now();

    function mainLoop(time) {
        const deltaTime = (time - lastTime) / 1000.0; // In seconds
        lastTime = time;
        context.fillStyle = 'rgba(0, 0, 0, 0.05)';
        context.fillRect(0, 0, canvas.width, canvas.height);

        context.font = `${fontSize}px 'Noto Sans JP'`;

        for (let col = 0; col < numColumns; col++) {
            fallPositions[col] += fallSpeeds[col] * deltaTime;
            if (fallPositions[col] >= numRows) {
                fallPositions[col] -= numRows;
                trailLengths[col] = Math.floor(Math.random() * 15 + 5);
                fallSpeeds[col] = (Math.random() * 7 + 3) * 2.0;
            }

            for (let trailOffset = 0; trailOffset < trailLengths[col]; trailOffset++) {
                let row = Math.floor(fallPositions[col] - trailOffset + numRows) % numRows;
                let randomChar = getRandomCharacter();
                let color = computeTrailColor(trailOffset, trailLengths[col]);
                context.fillStyle = color;

                let x = col * charWidth;
                let y = row * charHeight;

                context.fillText(randomChar, x + charWidth / 2, y);
            }
        }

        requestAnimationFrame(mainLoop);
    }

    requestAnimationFrame(mainLoop);
};
