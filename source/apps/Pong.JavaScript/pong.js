    const canvas = document.getElementById('gameCanvas');
    const ctx = canvas.getContext('2d');

    const WIDTH = canvas.width;
    const HEIGHT = canvas.height;

    class Paddle {
        constructor(x) {
            this.x = x;
            this.y = HEIGHT / 2 - 50;
            this.width = 20;
            this.height = 100;
            this.speed = 10;
        }

        draw() {
            ctx.fillStyle = 'white';
            ctx.fillRect(this.x, this.y, this.width, this.height);
        }

        moveUp() {
            if (this.y > 0) {
                this.y -= this.speed;
            }
        }

        moveDown() {
            if (this.y + this.height < HEIGHT) {
                this.y += this.speed;
            }
        }
    }

    class Ball {
        constructor() {
            this.reset();
            this.size = 10;
            this.speed = 5;
        }

        reset() {
            this.x = WIDTH / 2;
            this.y = HEIGHT / 2;
            const directions = ['UpLeft', 'DownLeft', 'UpRight', 'DownRight'];
            this.direction = directions[Math.floor(Math.random() * directions.length)];
        }

        draw() {
            ctx.fillStyle = 'white';
            ctx.fillRect(this.x, this.y, this.size, this.size);
        }

        move() {
            switch (this.direction) {
                case 'UpLeft':
                    this.x -= this.speed;
                    this.y -= this.speed;
                    break;
                case 'DownLeft':
                    this.x -= this.speed;
                    this.y += this.speed;
                    break;
                case 'UpRight':
                    this.x += this.speed;
                    this.y -= this.speed;
                    break;
                case 'DownRight':
                    this.x += this.speed;
                    this.y += this.speed;
                    break;
            }

            if (this.y <= 0) {
                this.direction = this.direction === 'UpLeft' ? 'DownLeft' : 'DownRight';
            } else if (this.y + this.size >= HEIGHT) {
                this.direction = this.direction === 'DownLeft' ? 'UpLeft' : 'UpRight';
            }

            if (this.x <= player1.x + player1.width && this.y + this.size >= player1.y && this.y <= player1.y + player1.height) {
                this.direction = this.direction === 'UpLeft' ? 'UpRight' : 'DownRight';
            } else if (this.x + this.size >= player2.x && this.y + this.size >= player2.y && this.y <= player2.y + player2.height) {
                this.direction = this.direction === 'UpRight' ? 'UpLeft' : 'DownLeft';
            }

            if (this.x < 0 || this.x > WIDTH) {
                this.reset();
            }
        }
    }

    const player1 = new Paddle(10);
    const player2 = new Paddle(WIDTH - 30);
    const ball = new Ball();

    const keys = {};

    window.addEventListener('keydown', (e) => {
        keys[e.key] = true;
    });

    window.addEventListener('keyup', (e) => {
        keys[e.key] = false;
    });

    function gameLoop() {
        ctx.clearRect(0, 0, WIDTH, HEIGHT);

        player1.draw();
        player2.draw();
        ball.draw();
        ball.move();

        if (keys['ArrowUp']) {
            player1.moveUp();
        }

        if (keys['ArrowDown']) {
            player1.moveDown();
        }

        if (ball.y < player2.y) {
            player2.moveUp();
        } else if (ball.y > player2.y + player2.height) {
            player2.moveDown();
        }

        requestAnimationFrame(gameLoop);
    }

    gameLoop();