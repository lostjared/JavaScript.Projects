class Block {
    constructor(x, y, color) {
        this.x = x;
        this.y = y;
        this.color = color;
    }
}

const WINDOW_WIDTH = 1280;
const WINDOW_HEIGHT = 720;
const TILE_W = Math.floor(WINDOW_WIDTH / 32);
const TILE_H = Math.floor(WINDOW_HEIGHT / 16);
const TILE_WIDTH = 32;
const TILE_HEIGHT = 16;

class Grid {
    constructor(widthx, heightx) {
        this.width = widthx;
        this.height = heightx;
        this.blocks = [];
        this.piece = [];
        this.score = 0;
        this.gameOver = false;
        this.pieceShape = 0;
        this.lines = 0;

        for (let i = 0; i < TILE_W; i++) {
            this.blocks[i] = [];
            for (let j = 0; j < TILE_H; j++) {
                this.blocks[i][j] = new Block(i, j, 0);
            }
        }
        this.newPiece();
    }

    newPiece() {
        const randColor = () => Math.floor(Math.random() * 9) + 1;
        this.piece = [
            new Block(Math.floor(TILE_W / 2), 0, randColor()),
            new Block(Math.floor(TILE_W / 2), 1, randColor()),
            new Block(Math.floor(TILE_W / 2), 2, randColor())
        ];

        while (
            this.piece[0].color === this.piece[1].color ||
            this.piece[0].color === this.piece[2].color
        ) {
            this.piece[0].color = randColor();
            this.piece[1].color = randColor();
            this.piece[2].color = randColor();
        }
        this.pieceShape = 0;
    }

    resetGame() {
        for (let x = 0; x < this.width; x++) {
            for (let y = 0; y < this.height; y++) {
                this.blocks[x][y].color = 0;
            }
        }
        this.newPiece();
        this.score = 0;
        this.lines = 0;
    }

    swapPieceColors(dir) {
        if (dir === 0) {
            const temp = this.piece[0].color;
            this.piece[0].color = this.piece[1].color;
            this.piece[1].color = this.piece[2].color;
            this.piece[2].color = temp;
        } else {
            const temp = this.piece[0].color;
            this.piece[0].color = this.piece[2].color;
            this.piece[2].color = this.piece[1].color;
            this.piece[1].color = temp;
        }
    }

    getGridPoint(x, y) {
        return this.blocks[x][y].color;
    }

    getBlock() {
        return this.piece.slice();
    }

    getWidth() {
        return this.width;
    }

    getHeight() {
        return this.height;
    }

    shiftLeft() {
        if (
            this.pieceShape === 0 &&
            this.checkBlock(0, this.piece[1].x - 1, this.piece[1].y - 1) &&
            this.checkBlock(0, this.piece[2].x - 2, this.piece[2].y - 2)
        ) {
            this.piece[1].x -= 1;
            this.piece[1].y -= 1;
            this.piece[2].x -= 2;
            this.piece[2].y -= 2;
            this.pieceShape = 1;
        } else if (
            this.pieceShape === 1 &&
            this.checkBlock(0, this.piece[1].x + 1, this.piece[1].y + 1) &&
            this.checkBlock(0, this.piece[2].x + 2, this.piece[2].y + 2)
        ) {
            this.piece[1].x += 1;
            this.piece[1].y += 1;
            this.piece[2].x += 2;
            this.piece[2].y += 2;
            this.pieceShape = 0;
        }
    }

    shiftRight() {
        if (
            this.pieceShape === 0 &&
            this.checkBlock(0, this.piece[1].x + 1, this.piece[1].y - 1) &&
            this.checkBlock(0, this.piece[2].x + 2, this.piece[2].y - 2)
        ) {
            this.piece[1].x += 1;
            this.piece[1].y -= 1;
            this.piece[2].x += 2;
            this.piece[2].y -= 2;
            this.pieceShape = 2;
        } else if (
            this.pieceShape === 2 &&
            this.checkBlock(0, this.piece[1].x + 1, this.piece[1].y + 1) &&
            this.checkBlock(0, this.piece[2].x + 2, this.piece[2].y + 2)
        ) {
            this.piece[1].x -= 1;
            this.piece[1].y += 1;
            this.piece[2].x -= 2;
            this.piece[2].y += 2;
            this.pieceShape = 0;
        }
    }

    moveLeft() {
        let go = true;
        for (let i = 0; i < 3; i++) {
            if (
                this.piece[i].x <= 0 ||
                this.blocks[this.piece[i].x - 1][this.piece[i].y].color !== 0
            ) {
                go = false;
            }
        }
        if (go) {
            for (let i = 0; i < 3; i++) {
                this.piece[i].x -= 1;
            }
        }
    }

    moveRight() {
        let go = true;
        for (let i = 0; i < 3; i++) {
            if (
                this.piece[i].x >= TILE_W - 1 ||
                this.blocks[this.piece[i].x + 1][this.piece[i].y].color !== 0
            ) {
                go = false;
            }
        }
        if (go) {
            for (let i = 0; i < 3; i++) {
                this.piece[i].x += 1;
            }
        }
    }

    moveDown() {
        if (this.piece[2].y + 1 > TILE_H - 1) {
            this.setBlock();
            return;
        }

        if (
            this.piece[2].y === 2 &&
            this.piece[2].y + 1 < TILE_H - 1 &&
            this.blocks[this.piece[2].x][this.piece[2].y + 1].color !== 0
        ) {
            this.resetGame();
            this.gameOver = true;
            return;
        }

        for (let i = 0; i < 3; i++) {
            if (
                this.piece[i].y + 1 < TILE_H &&
                this.blocks[this.piece[i].x][this.piece[i].y + 1].color !== 0
            ) {
                this.setBlock();
                return;
            }
        }

        for (let i = 0; i < 3; i++) {
            this.piece[i].y += 1;
        }
    }

    setBlock() {
        
        if (this.piece[2].y <= 2) {
            this.resetGame();
            return;
        }


        for (let i = 0; i < 3; i++) {
            this.blocks[this.piece[i].x][this.piece[i].y].color = this.piece[i].color;
        }
        this.newPiece();
        this.procBlocks();


    }

    checkBlock(color, x, y) {
        return (
            x >= 0 &&
            x < TILE_W &&
            y >= 0 &&
            y < TILE_H &&
            color === this.blocks[x][y].color
        );
    }

    procMoveDown() {
        for (let x = 0; x < this.getWidth(); x++) {
            for (let y = 0; y < this.getHeight() - 1; y++) {
                let color = this.blocks[x][y].color;
                let color2 = this.blocks[x][y + 1].color;
                if (color !== 0 && color2 === 0) {
                    this.blocks[x][y].color = 0;
                    this.blocks[x][y + 1].color = color;
                    return;
                }
            }
        }
    }

    procBlocks() {
        for (let x = 0; x < this.getWidth(); x++) {
            for (let y = 0; y < this.getHeight(); y++) {
                let xpos = x;
                let ypos = y;
                let color = this.blocks[xpos][ypos].color;
                if (color >= 1) {
                    if (
                        this.checkBlock(color, x + 1, y) &&
                        this.checkBlock(color, x + 2, y)
                    ) {
                        this.blocks[xpos][ypos].color = -1;
                        this.blocks[xpos + 1][ypos].color = -1;
                        this.blocks[xpos + 2][ypos].color = -1;

                        if (this.checkBlock(color, x + 3, y)) {
                            this.blocks[xpos + 3][ypos].color = -1;
                            this.score += 1;
                            this.score += 1;
                            if (this.checkBlock(color, x + 4, y)) {
                                this.blocks[xpos + 4][ypos].color = -1;
                                this.score += 1;
                            }
                        }
                        this.score += 1;
                        this.lines += 1;
                        return;
                    }
                    if (
                        this.checkBlock(color, x, y + 1) &&
                        this.checkBlock(color, x, y + 2)
                    ) {
                        this.blocks[xpos][ypos].color = -1;
                        this.blocks[xpos][ypos + 1].color = -1;
                        this.blocks[xpos][ypos + 2].color = -1;

                        if (this.checkBlock(color, x, y + 3)) {
                            this.blocks[xpos][ypos + 3].color = -1;
                            this.score += 1;
                            if (this.checkBlock(color, x, y + 4)) {
                                this.blocks[xpos][ypos + 4].color = -1;
                                this.score += 1;
                            }
                        }

                        this.score += 1;
                        this.lines += 1;
                        return;
                    }
                    if (
                        this.checkBlock(color, x + 1, y + 1) &&
                        this.checkBlock(color, x + 2, y + 2)
                    ) {
                        this.blocks[xpos][ypos].color = -1;
                        this.blocks[xpos + 1][ypos + 1].color = -1;
                        this.blocks[xpos + 2][ypos + 2].color = -1;

                        if (this.checkBlock(color, x + 3, y + 3)) {
                            this.blocks[xpos + 3][ypos + 3].color = -1;
                            this.score += 2;
                            if (this.checkBlock(color, x + 4, y + 4)) {
                                this.blocks[xpos + 4][ypos + 4].color = -1;
                                this.score += 2;
                            }
                        }

                        this.score += 2;
                        this.lines += 1;
                        return;
                    }
                    if (
                        this.checkBlock(color, x + 1, y - 1) &&
                        this.checkBlock(color, x + 2, y - 2)
                    ) {
                        this.blocks[xpos][ypos].color = -1;
                        this.blocks[xpos + 1][ypos - 1].color = -1;
                        this.blocks[xpos + 2][ypos - 2].color = -1;

                        if (this.checkBlock(color, x + 3, y - 3)) {
                            this.blocks[xpos + 3][ypos - 3].color = -1;
                            this.score += 2;
                            if (this.checkBlock(color, x + 4, y - 4)) {
                                this.blocks[xpos + 4][ypos - 4].color = -1;
                                this.score += 2;
                            }
                        }
                        this.score += 2;
                        this.lines += 1;
                        return;
                    }
                } else if (color < 0) {
                    color -= 1;
                    if (color < -90) {
                        this.blocks[xpos][ypos].color = 0;
                    } else {
                        this.blocks[xpos][ypos].color = color;
                    }
                }
            }
        }
        this.procMoveDown();
    }
}

function loadImages(files, images, callback) {
    let loadedImages = 0;
    const totalImages = files.length;
    let offset = 0;
    files.forEach((file) => {
        const img = new Image();
        img.src = `img_converted/${file}`;
        img.onload = () => {
            loadedImages++;
            if (loadedImages === totalImages) {
                callback();
            }
        };
        images[offset++] = img;
    });
}

document.addEventListener("DOMContentLoaded", () => {
    let cur_screen = 0;
    const canvas = document.getElementById("gameCanvas");
    const ctx = canvas.getContext("2d");
    const grid = new Grid(TILE_W, TILE_H);

    const blockFiles = [
        "block_black.png", "block_clear.png", "block_dblue.png", "block_gray.png",
        "block_green.png", "block_ltblue.png", "block_orange.png", "block_pink.png",
        "block_purple.png", "block_red.png", "block_yellow.png"
    ];
    const levelFiles = [
        "level8.png", "level1.png", "level2.png", "level3.png", "level4.png",
        "level5.png", "level6.png", "level7.png", "level8.png"
    ];
    const screenFiles = [
        "gameover.png", "gameover_logo.png", "icon.png",
        "intro.png", "lostlogo.png", "lostsidedead_logo.png"
    ];
    
    const blockImages = {};
    const levelImages = {};
    const screenImages = {};
    let level_index = 0;

    loadImages(blockFiles, blockImages, () => {
        console.log("Block images loaded.");
    });
    loadImages(levelFiles, levelImages, () => {
        console.log("Level images loaded.");
    });
    loadImages(screenFiles, screenImages, () => {
        console.log("Screen images loaded.");
    });

    let timer;
    let timer_start;

    function handle() {
        cur_screen = 1;
        clearTimeout(timer_start);
    }

    function gameLoop() {
        if(cur_screen == 0) {
            timer_start = setTimeout(handle,2000);
        }
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        drawGrid(ctx, blockImages, grid);
        if(cur_screen == 2) {
            drawPiece(ctx, blockImages, grid);
        }
        requestAnimationFrame(gameLoop);
    }

    document.addEventListener('keydown', (event) => {

        switch(cur_screen) {
            case 1:
                if(event.keyCode == 32)
                    cur_screen = 2; 
            break;
            case 2: {
                if (event.key === 'ArrowUp') {
                    grid.swapPieceColors(0);
                } else if (event.key === 'ArrowDown') {
                    grid.moveDown();
                 } else if (event.key === 'ArrowLeft') {
                    grid.moveLeft();
                } else if (event.key === 'ArrowRight') {
                    grid.moveRight();
                } else if (event.key === 'a' || event.key === 'A') {
                    grid.shiftLeft();
                } else if (event.key === 's' || event.key === 'S') {
                    grid.shiftRight();    
                }
        }
        break;
    }
    });

    let cur_level;
    let timer_delay;
    
    function setLevelAndTimer(grid) {
        if (grid.score >= 0 && grid.score <= 10) {
            cur_level = 1;
            timer_delay = 1500;
        } else if (grid.score >= 11 && grid.score <= 20) {
            cur_level = 2;
            timer_delay = 1200;
        } else if (grid.score >= 21 && grid.score <= 30) {
            cur_level = 3;
            timer_delay = 1000;
        } else if (grid.score >= 31 && grid.score <= 40) {
            cur_level = 4;
            timer_delay = 800;
        } else if (grid.score >= 41 && grid.score <= 50) {
            cur_level = 5;
            timer_delay = 700;
        } else if (grid.score >= 51 && grid.score <= 60) {
            cur_level = 6;
            timer_delay = 400;
        } else if (grid.score >= 61 && grid.score <= 65) {
            cur_level = 7;
            timer_delay = 300;
        } else if (grid.score >= 66) {
            cur_level = 8;
            timer_delay = 100;
        }
    }
    
    function startTimer(grid) {
            if (timer) {
                clearTimeout(timer);
            }   
            setLevelAndTimer(grid

            );    
            timer = setTimeout(() => {
                if(cur_screen == 2) {
                    grid.moveDown();
                }
            startTimer(grid);
        }, timer_delay);
    }
    
    function drawGrid(ctx, blockImages, grid) {
        if(cur_screen == 0) {
            ctx.drawImage(screenImages[4], 0,0,canvas.width,canvas.height);
        } else if(cur_screen == 1) {
            ctx.drawImage(screenImages[3], 0,0,canvas.width, canvas.height);
        }
        if(cur_screen == 2) {
            ctx.drawImage(levelImages[cur_level], 0, 0, canvas.width, canvas.height);  
            for (let x = 0; x < TILE_W; x++) {
                for (let y = 0; y < TILE_H; y++) {
                    let block = grid.blocks[x][y];
                    if (block.color >= 1) {
                        ctx.drawImage(blockImages[block.color], x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
                    } else if (block.color < 0) {
                        let randomColor = `rgb(${Math.floor(Math.random() * 255)}, ${Math.floor(Math.random() * 255)}, ${Math.floor(Math.random() * 255)})`;
                        ctx.fillStyle = randomColor;
                        ctx.fillRect(x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH-1, TILE_HEIGHT-1);
                    } else if (block.color === 0) {
                        ctx.fillStyle = "black";
                        ctx.fillRect(x * TILE_WIDTH + 1, y * TILE_HEIGHT + 1, TILE_WIDTH - 1, TILE_HEIGHT - 1);
                    }
                }
            }
            ctx.fillStyle="#FFFFFF";
            ctx.fillText("Score: " + grid.score + " Lines: " + grid.lines + " Level: " + cur_level,15, 15);
            grid.procBlocks();
        }
    }
    
    function drawPiece(ctx, blockImages, grid) {
        for (let block of grid.piece) {
            ctx.drawImage(blockImages[block.color], block.x * TILE_WIDTH, block.y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
        }
    }

    grid.score = 0;
    startTimer(grid);
    requestAnimationFrame(gameLoop);

});