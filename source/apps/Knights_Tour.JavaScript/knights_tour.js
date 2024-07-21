const BOARD_SIZE = 8;
const CELL_SIZE = 100;
const CANVAS_SIZE = BOARD_SIZE * CELL_SIZE;

class Position {
    constructor(row, col) {
        this.row = row;
        this.col = col;
    }
}

const canvas = document.getElementById("knightsTourCanvas");
const ctx = canvas.getContext("2d");

const knightImg = new Image();
knightImg.crossOrigin = "anonymous"; // Allow cross-origin requests
knightImg.src = 'knight.png';

knightImg.onload = function() {
    const tempCanvas = document.createElement('canvas');
    const tempCtx = tempCanvas.getContext('2d');
    tempCanvas.width = knightImg.width;
    tempCanvas.height = knightImg.height;
    tempCtx.drawImage(knightImg, 0, 0);
    const imageData = tempCtx.getImageData(0, 0, knightImg.width, knightImg.height);
    const data = imageData.data;
    for (let i = 0; i < data.length; i += 4) {
        if (data[i] === 255 && data[i + 1] === 255 && data[i + 2] === 255) {
            data[i + 3] = 0;
        }
    }
    tempCtx.putImageData(imageData, 0, 0);

    const modifiedKnightImg = new Image();
    modifiedKnightImg.src = tempCanvas.toDataURL();

    modifiedKnightImg.onload = function() {
        function drawKnight() {
            ctx.drawImage(modifiedKnightImg, knightPos.col * CELL_SIZE, knightPos.row * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        }

        function draw() {
            ctx.clearRect(0, 0, CANVAS_SIZE, CANVAS_SIZE);
            drawBoard();
            drawKnight();
            requestAnimationFrame(draw);
        }

        window.addEventListener("keydown", (event) => {
            if (event.code === "Space" && !tourOver) {
                nextMove();
            } else if (event.code === "Enter") {
                tourOver = false;
                moves = 1;
                resetTour();
            }
        });

        canvas.addEventListener("touchstart", (event) => {
              if (event.touches.length === 1 && !tourOver) {
                  nextMove();
              } else if (event.touches.length === 2) {
                  tourOver = false;
                  moves = 1;
                  resetTour();
              }
              event.preventDefault();
          });

        resetTour();
        draw();
    };
};

let board = Array.from({ length: BOARD_SIZE }, () => Array(BOARD_SIZE).fill(0));
let horizontal = [2, 1, -1, -2, -2, -1, 1, 2];
let vertical = [-1, -2, -2, -1, 1, 2, 2, 1];
let knightPos = new Position(
    Math.floor(Math.random() * BOARD_SIZE),
    Math.floor(Math.random() * BOARD_SIZE)
);
let moveSequence = [];
let tourOver = false;
let moves = 1;

function drawBoard() {
    for (let row = 0; row < BOARD_SIZE; row++) {
        for (let col = 0; col < BOARD_SIZE; col++) {
            ctx.fillStyle = (row + col) % 2 === 0 ? "#ffffff" : "#ff0000";
            ctx.fillRect(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE);
            if (board[row][col] < 0) {
                ctx.fillStyle = "#000000";
                ctx.fillRect(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE);
            }
        }
    }
}

function clearBoard() {
    board = Array.from({ length: BOARD_SIZE }, () => Array(BOARD_SIZE).fill(0));
}

function isValidMove(pos) {
    return (
        pos.row >= 0 &&
        pos.row < BOARD_SIZE &&
        pos.col >= 0 &&
        pos.col < BOARD_SIZE &&
        board[pos.row][pos.col] === 0
    );
}

function getDegree(pos) {
    let count = 0;
    for (let i = 0; i < 8; i++) {
        let newRow = pos.row + horizontal[i];
        let newCol = pos.col + vertical[i];
        if (
            newRow >= 0 &&
            newRow < BOARD_SIZE &&
            newCol >= 0 &&
            newCol < BOARD_SIZE &&
            board[newRow][newCol] === 0
        ) {
            count++;
        }
    }
    return count;
}

function solveKnightsTour(pos, moveCount) {
    if (moveCount === BOARD_SIZE * BOARD_SIZE + 1) {
        return true;
    }

    let minDegreeIndex = null;
    let minDegree = 9;
    let nextPos = new Position(-1, -1);

    for (let i = 0; i < 8; i++) {
        let newPos = new Position(pos.row + horizontal[i], pos.col + vertical[i]);
        if (isValidMove(newPos)) {
            let degree = getDegree(newPos);
            if (degree < minDegree) {
                minDegreeIndex = i;
                minDegree = degree;
                nextPos = newPos;
            }
        }
    }

    if (minDegreeIndex !== null) {
        board[nextPos.row][nextPos.col] = moveCount;
        moveSequence.push(nextPos);
        if (solveKnightsTour(nextPos, moveCount + 1)) {
            return true;
        } else {
            board[nextPos.row][nextPos.col] = 0;
            moveSequence.pop();
        }
    }

    return false;
}

function nextMove() {
    if (moveSequence.length > 0) {
        let nextPos = moveSequence.shift();
        board[knightPos.row][knightPos.col] = -1;
        knightPos = nextPos;
        board[knightPos.row][knightPos.col] = moves;
        moves++;
        if (moves === BOARD_SIZE * BOARD_SIZE + 1) {
            tourOver = true;
        }
    }
}

function resetTour() {
    clearBoard();
    knightPos = new Position(
        Math.floor(Math.random() * BOARD_SIZE),
        Math.floor(Math.random() * BOARD_SIZE)
    );
    moveSequence = [];
    board[knightPos.row][knightPos.col] = 1;
    moveSequence.push(knightPos);
    solveKnightsTour(knightPos, 2);
}
