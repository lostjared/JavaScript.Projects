const TILE_SIZE = 16;
const WIDTH = 1280 / TILE_SIZE;
const HEIGHT = 720 / TILE_SIZE;

class Point {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }
}

class Grid {
    constructor() {
        this.blocks = Array.from({ length: WIDTH }, () => Array(HEIGHT).fill(0));
        this.score = 0;
        this.lives = 4;
        this.appleCount = 1;
        this.appleNum = 1;
    }

    resetLives() {
        this.score = 0;
        this.lives = 4;
        this.appleCount = 1;
        this.appleNum = 1;
    }

    clear() {
        for (let i = 0; i < WIDTH; i++) {
            for (let j = 0; j < HEIGHT; j++) {
                this.blocks[i][j] = 0;
            }
        }
        this.resetLives();
        this.setApple(this.randApple());
    }

    randApple() {
        let ix, iy;
        do {
            ix = Math.floor(Math.random() * (WIDTH - 4)) + 2;
            iy = Math.floor(Math.random() * (HEIGHT - 4)) + 2;
        } while (this.blocks[ix][iy] === 2);
        return [ix, iy];
    }

    setApple([x, y]) {
        this.blocks[x][y] = 2;
    }

    checkApples(x, y, snake) {
        for (const segment of snake.sn) {
            if (segment.x === x && segment.y === y) {
                snake.grow();
                this.blocks[x][y] = 0;
                this.appleNum -= 1;
                if (this.appleNum === 0) {
                    this.appleCount += 1;
                    this.appleNum = this.appleCount;
                    for (let i = 0; i < this.appleNum; i++) {
                        this.setApple(this.randApple());
                    }
                }
                this.score += 1;
                break;
            }
        }
    }

    drawGrid(ctx, snake) {
        for (let i = 0; i < WIDTH; i++) {
            for (let j = 0; j < HEIGHT; j++) {
                ctx.fillStyle = this.blocks[i][j] === 2 ? 'red' : 'black';
                if (this.blocks[i][j] === 2) this.checkApples(i, j, snake);
                ctx.fillRect(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            }
        }
        snake.drawSnake(ctx);
    }

    minusLife() {
        this.lives -= 1;
        if (this.lives <= 0) {
            this.clear();
        }
    }

    update(snake) {
        const tail = snake.sn[snake.sn.length - 1];
        if (snake.checkOut()) {
            snake.resetSnake();
            this.minusLife();
            return;
        }
        snake.moveSnake(tail);
    }
}

class Snake {
    constructor() {
        this.direction = 'Right';
        this.sn = [new Point(10, 10)];
    }

    duplicates() {
        const top = this.sn[this.sn.length - 1];
        for (let i = 0; i < this.sn.length - 1; i++) {
            if (top.x === this.sn[i].x && top.y === this.sn[i].y) return true;
        }
        return false;
    }

    checkOut() {
        for (const segment of this.sn) {
            if (segment.x <= 0 || segment.x >= WIDTH - 1 || segment.y <= 0 || segment.y >= HEIGHT - 1) {
                return true;
            }
        }
        return this.duplicates();
    }

    grow() {
        const tail = this.sn[this.sn.length - 1];
        this.growTail(tail);
    }

    growTail(tail) {
        switch (this.direction) {
            case 'Left': this.sn.push(new Point(tail.x - 1, tail.y)); break;
            case 'Right': this.sn.push(new Point(tail.x + 1, tail.y)); break;
            case 'Up': this.sn.push(new Point(tail.x, tail.y - 1)); break;
            case 'Down': this.sn.push(new Point(tail.x, tail.y + 1)); break;
        }
    }

    moveSnake(tail) {
        this.sn.shift();
        this.growTail(tail);
    }

    resetSnake() {
        this.sn = [new Point(10, 10)];
        this.direction = 'Right';
    }

    drawSnake(ctx) {
        ctx.fillStyle = 'green';
        for (const segment of this.sn) {
            ctx.fillRect(segment.x * TILE_SIZE, segment.y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
    }

    moveLeft() { if (this.direction !== 'Right') this.direction = 'Left'; }
    moveRight() { if (this.direction !== 'Left') this.direction = 'Right'; }
    moveDown() { if (this.direction !== 'Up') this.direction = 'Down'; }
    moveUp() { if (this.direction !== 'Down') this.direction = 'Up'; }
}

const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

const grid = new Grid();
const snake = new Snake();

let prevTick = 0;
let tickCount = 0;
grid.setApple(grid.randApple());

function gameLoop(timestamp) {
    const delta = timestamp - prevTick;
    prevTick = timestamp;
    tickCount += delta;

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    grid.drawGrid(ctx, snake);
    ctx.fillStyle = 'white';
    ctx.fillText(`Score: ${grid.score} Lives: ${grid.lives}`, 25, 25);

    if (tickCount > 75) {
        tickCount = 0;
        grid.update(snake);
    }

    requestAnimationFrame(gameLoop);
}

document.addEventListener('keydown', (e) => {
    switch (e.key) {
        case 'ArrowLeft': snake.moveLeft(); break;
        case 'ArrowRight': snake.moveRight(); break;
        case 'ArrowDown': snake.moveDown(); break;
        case 'ArrowUp': snake.moveUp(); break;
    }
});

requestAnimationFrame(gameLoop);