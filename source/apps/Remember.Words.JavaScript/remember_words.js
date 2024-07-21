document.addEventListener("DOMContentLoaded", function () {
    const canvas = document.getElementById("gameCanvas");
    const ctx = canvas.getContext("2d");
    //const messageDiv = document.getElementById("message");

    class Words {
        constructor(words) {
            this.words = words.split("\n").filter(word => word.trim() !== "");
            this.index = 0;
            if (this.words.length <= 5) {
                alert("Not enough words in the list.");
                return;
            }
        }

        shuffle() {
            this.words = this.words.sort(() => Math.random() - 0.5);
        }

        next() {
            if (this.index < this.words.length) {
                return this.words[this.index++];
            } else {
                this.shuffle();
                this.index = 0;
                return this.words[0];
            }
        }
    }

    class Game {
        constructor() {
            this.mode = 0;
            this.curTime = 3;
            this.timeoutT = 1000 * this.curTime;
            this.newGame();
            this.init();
        }

        newGame() {
            const wordList = "apple\nbanana\ncherry\ndate\nelderberry\nfig\ngrape\nhoneydew";
            this.words = new Words(wordList);
            this.words.shuffle();
            this.text = "Press Space to Start Game";
            this.wordText = "";
            this.mode = 0;
            this.timeoutT = 1000 * 15;
            this.inputText = "";
            this.curTime = 3;
        }

        init() {
            document.addEventListener('keydown', (e) => this.handleEvent(e));
            this.loop();
        }

        handleEvent(e) {
            if ((this.mode === 0 || this.mode === 4) && e.code === 'Space') {
                this.addWord();
                this.mode = 1;
            } else if (this.mode === 1 && e.code === 'Enter') {
                this.startTicks = Date.now();
                this.mode = 2;
            } else if (this.mode === 3 && e.code === 'Backspace' && this.inputText.length >= 1) {
                this.inputText = this.inputText.slice(0, -1);
            } else if (this.mode === 3 && e.code === 'Enter' && this.inputText.length >= 1) {
                this.checkInput();
            } else if (this.mode === 3 && e.key.length === 1) {
                this.inputText += e.key;
            } else if (this.mode === 5 && e.code === 'Enter') {
                this.newGame();
            }
        }

        addWord() {
            if (this.wordText.length === 0) {
                this.wordText = this.words.next();
            } else {
                this.wordText += " " + this.words.next();
            }
        }

        checkInput() {
            if (this.inputText.trim() === this.wordText.trim()) {
                this.mode = 4;
            } else {
                this.mode = 5;
            }
        }

        wrapText(ctx, text, x, y, maxWidth, lineHeight) {
            const words = text.split(' ');
            let line = '';
            let lines = [];

            for (let n = 0; n < words.length; n++) {
                let testLine = line + words[n] + ' ';
                let metrics = ctx.measureText(testLine);
                let testWidth = metrics.width;
                if (testWidth > maxWidth && n > 0) {
                    lines.push(line);
                    line = words[n] + ' ';
                } else {
                    line = testLine;
                }
            }
            lines.push(line);

            for (let i = 0; i < lines.length; i++) {
                ctx.fillText(lines[i], x, y);
                y += lineHeight;
            }
        }

        draw() {
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            ctx.fillStyle = "white";
            ctx.font = "24px Arial";

            const lineHeight = 30;
            const maxWidth = canvas.width - 30;

            if (this.mode === 0) {
                this.wrapText(ctx, this.text, 15, 50, maxWidth, lineHeight);
            } else if (this.mode === 1) {
                ctx.fillStyle = "red";
                this.wrapText(ctx, this.wordText, 15, 50, maxWidth, lineHeight);
                ctx.fillStyle = "white";
                this.wrapText(ctx, "Press Enter when ready to start countdown", 15, 100, maxWidth, lineHeight);
            } else if (this.mode === 2) {
                this.timeoutT = Math.floor((Date.now() - this.startTicks) / 1000);
                if (this.timeoutT > this.curTime) {
                    this.mode = 3;
                    this.curTime += 1;
                    this.inputText = "";
                }
                this.wrapText(ctx, `${this.timeoutT}`, 15, 100, maxWidth, lineHeight);
            } else if (this.mode === 3) {
                this.wrapText(ctx, this.inputText + "_", 15, 50, maxWidth, lineHeight);
            } else if (this.mode === 4) {
                this.wrapText(ctx, "Correct! Press Space to Continue", 15, 50, maxWidth, lineHeight);
            } else if (this.mode === 5) {
                ctx.fillStyle = "blue";
                this.wrapText(ctx, "Incorrect! Game Over", 15, 50, maxWidth, lineHeight);
            }
        }

        loop() {
            this.draw();
            requestAnimationFrame(() => this.loop());
        }
    }

    const game = new Game();
});
