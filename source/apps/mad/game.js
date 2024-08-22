const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

canvas.width = 800;
canvas.height = 600;

const bucketWidth = 80;
const bucketHeight = 20;
let bucketX = (canvas.width - bucketWidth) / 2;
const bucketY = canvas.height - bucketHeight - 10;

const bomberWidth = 60;
const bomberHeight = 40;
let bomberX = 0;
const bomberY = 10;
let bomberSpeed = 3;

let bombs = [];
const bombRadius = 10;
const bombSpeed = 4;

let score = 0;
let lives = 5;
let gameOver = false;
let clickCount = 0;

// Event listeners for bucket movement
document.addEventListener('mousemove', moveBucket);
document.addEventListener('touchmove', moveBucketTouch);
document.addEventListener('click', handleRestart);
document.addEventListener('touchstart', handleRestart);

function moveBucket(e) {
    const rect = canvas.getBoundingClientRect();
    bucketX = e.clientX - rect.left - bucketWidth / 2;
}

function moveBucketTouch(e) {
    const rect = canvas.getBoundingClientRect();
    bucketX = e.touches[0].clientX - rect.left - bucketWidth / 2;
}

function handleRestart() {
    if (gameOver) {
        clickCount++;
        if (clickCount === 2) {
            resetGame();
            gameOver = false;
            clickCount = 0;
            update();
        }
    }
}

function drawBucket() {
    ctx.fillStyle = 'white';
    ctx.fillRect(bucketX, bucketY, bucketWidth, bucketHeight);
}

function drawBomber() {
    ctx.fillStyle = 'red';
    ctx.fillRect(bomberX, bomberY, bomberWidth, bomberHeight);
}

function dropBomb() {
    bombs.push({ x: bomberX + bomberWidth / 2, y: bomberY + bomberHeight });
}

function drawBombs() {
    ctx.fillStyle = 'yellow';
    bombs.forEach(bomb => {
        ctx.beginPath();
        ctx.arc(bomb.x, bomb.y, bombRadius, 0, Math.PI * 2);
        ctx.fill();
    });
}

function moveBombs() {
    bombs.forEach(bomb => {
        bomb.y += bombSpeed;
    });
}

function checkCatch() {
    bombs.forEach((bomb, index) => {
        if (bomb.y + bombRadius >= bucketY &&
            bomb.x >= bucketX && bomb.x <= bucketX + bucketWidth) {
            bombs.splice(index, 1);
            score++;
        } else if (bomb.y > canvas.height) {
            bombs = []; // Clear all bombs
            lives--;
            if (lives > 0) {
                resetRound(); // Reset for another round
            } else {
                gameOver = true;
            }
        }
    });
}

function drawScore() {
    ctx.font = '24px Arial';
    ctx.fillStyle = 'white';
    ctx.textAlign = 'left'; // Ensure the text alignment is consistent
    ctx.fillText(`Score: ${score}`, 10, 30);
    ctx.fillText(`Lives: ${lives}`, canvas.width - 100, 30);
}

function drawGameOver() {
    ctx.font = '48px Arial';
    ctx.fillStyle = 'red';
    ctx.textAlign = 'center';
    ctx.fillText('Game Over!', canvas.width / 2, canvas.height / 2);
    ctx.font = '24px Arial';
    ctx.fillText('Double Click or Double Tap to Restart', canvas.width / 2, canvas.height / 2 + 50);
}

function resetRound() {
    bomberX = 0;
    bomberSpeed = 3;
    bombs = [];
    bucketX = (canvas.width - bucketWidth) / 2;
}

function resetGame() {
    resetRound();
    score = 0;
    lives = 5;
}

function update() {
    if (gameOver) {
        drawGameOver();
        return;
    }

    ctx.clearRect(0, 0, canvas.width, canvas.height); // Clear the entire canvas

    bomberX += bomberSpeed;
    if (bomberX + bomberWidth > canvas.width || bomberX < 0) {
        bomberSpeed *= -1;
    }

    if (Math.random() < 0.02) {
        dropBomb();
    }

    moveBombs();
    checkCatch();

    drawBomber();
    drawBombs();
    drawBucket();
    drawScore();

    requestAnimationFrame(update);
}

resetGame();
update();