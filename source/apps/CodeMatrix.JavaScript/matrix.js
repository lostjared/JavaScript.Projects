 
 
       const LETTER_MAX = 21;
        const LETTER_NUM = 40;
        const LETTER_SIZE = 32;
        const TIMEOUT = 1;
        const COLOR = "rgb(0, 255, 0)";
        const FONT = "32px Arial";

        class Letter {
            constructor(ch, xpos, ypos) {
                this.ch = ch;
                this.xpos = xpos;
                this.ypos = ypos;
            }
        }

        class LetterGen {
            constructor() {
                this.letters = [];
                this.letterRow = [];
                let x = 0;
                for (let i = 0; i < LETTER_NUM; i++) {
                    let column = [];
                    let y = -LETTER_SIZE;
                    for (let j = 0; j < LETTER_MAX; j++) {
                        column.push(new Letter(this.getRandomChar(), x, y));
                        y += LETTER_SIZE + 4;
                    }
                    this.letters.push(column);
                    this.letterRow.push(Math.floor(Math.random() * (32 - 24 + 1) + 24));
                    x += LETTER_SIZE;
                }
            }

             getRandomChar() {
                const chars = 'あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわをん';
                return chars.charAt(Math.floor(Math.random() * chars.length));
            }
        }

        const canvas = document.getElementById('matrixCanvas');
        const ctx = canvas.getContext('2d');

        ctx.font = FONT;
        ctx.fillStyle = COLOR;

        const letterGen = new LetterGen();
        let tickCount = 0;
        let dir = false;

        document.addEventListener('keydown', (event) => {
            if (event.key === 'Escape') {
                cancelAnimationFrame(animationFrameId);
            } else if (event.key === 'ArrowUp') {
                dir = true;
            } else if (event.key === 'ArrowDown') {
                dir = false;
            }
        });

        function draw() {
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            for (let i = 0; i < letterGen.letters.length; i++) {
                for (let letter of letterGen.letters[i]) {
                    ctx.fillText(letter.ch, letter.xpos, letter.ypos);
                    const speed = letterGen.letterRow[i];
                    if (tickCount > TIMEOUT) {
                        if (dir) {
                            letter.ypos -= speed;
                            if (letter.ypos <= -LETTER_SIZE) {
                                letter.ypos = canvas.height;
                                letter.ch = letterGen.getRandomChar();
                            }
                        } else {
                            letter.ypos += speed;
                            if (letter.ypos >= canvas.height) {
                                letter.ypos = -LETTER_SIZE;
                                letter.ch = letterGen.getRandomChar();
                            }
                        }
                    }
                }
            }

            if (tickCount > TIMEOUT) {
                tickCount = 0;
            }
            tickCount++;

            animationFrameId = requestAnimationFrame(draw);
        }

        let animationFrameId = requestAnimationFrame(draw);