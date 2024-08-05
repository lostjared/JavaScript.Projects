function getRandomNumber() {
    return Math.floor(Math.random() * 3) + 1;
}

class Deck {
    constructor(size) {
        this.size = size;
        this.deck = [];
        this.index = 0;
        for(let i = 0; i < size; ++i) {
            this.deck.push(getRandomNumber());
        }
    }

    drawCard() {
        let card = this.deck[this.index++];
        if(this.index > this.size) {
            return 0;
        }
        return card;
    }
    curCard() {
        if(this.index < this.size) {
            return this.deck[this.index];
        }
        return 0;
    }
    nextCard() {
        this.index++;
    }
}


let max_cards = 10;
let deck = new Deck(max_cards);
let game_over = false;

const canvas = document.getElementById('gameCanvas');
const context = canvas.getContext('2d');
let game_string = "Use your Mental ability and guess between 1-3 by pressing 1 -3 keys " + (deck.index+1) + "/" + deck.size;
let lastTime = 0;
let draw_card = 0;
let next_input = true;
let correct = 0;
let psychic_string = "";

function genRating() {
    switch(correct) {
        case 0:
            return "You either have bad luck or just thing aren't going your way to score zero.";
        case 1:
        case 2:
        case 3:
            return "You are a average player of this game and have little psychic abilites.";
        case 4:
        case 5:
        case 6:
            return "You are above average with some mild psychic abilities.";
        case 7:
        case 8:
            return "You are an experienced user and know what you are doing.";
        case 9:
        case 10:
            return "You are Psychic.";    
    }
    return "Should never get here - You are smarter than the average bear.";
}

function init() {
    loadGfx();
    window.addEventListener('keydown', keyEventDown);
    console.log('Game Initialized');
    requestAnimationFrame(gameLoop);
}

function displayCard() {
    draw_card = 0;
    next_input = true;
    deck.nextCard();
    if(deck.index > deck.size-1) {
        game_string = "Game Over Your Score: " + correct + "/" + deck.size + " - [Press Enter for New Game]";
        game_over = true;
        psychic_string = genRating();
    } else {
         game_string = "Use your Mental ability and guess between 1-3 by pressing 1 -3 keys " + (deck.index+1) + "/" + deck.size;
    }
}

function keyEventDown(event) {
    keyDown(event.key);
}

function keyDown(key) {

    if(next_input == false)
        return;

    let card = deck.curCard();
    if(game_over == true) {
        switch(key) {
            case 'Enter': {
                    game_over = false;
                    deck = new Deck(max_cards);
                    game_string = "Use your Mental ability and guess between 1-3 by pressing 1 -3 keys " + (deck.index+1) + "/" + deck.size;
                    correct = 0;
            }
            break;
        }
    } else {
        switch(key) {
            case '1': {
                if(card == 1) {
                    correct++;
                    game_string = "Correct your Mental Intuition was correct. ";
                } else {
                    game_string = "Incorrect your Mental Intution was incorrect. ";
                }
                draw_card = card;
                next_input = false;
                setTimeout(displayCard, 3000);
            }
                break;
            case '2': {
                if(card == 2) {
                    correct++;
                    game_string = "Correct your Mental Intuition was correct. ";
                } else {
                    game_string = "Incorrect your Mental Intution was incorrect. ";
                }
                draw_card = card;
                next_input = false;
                setTimeout(displayCard, 3000);
            }
                break;
            case '3': {
                if(card == 3) {
                    correct++;
                    game_string = "Correct your Mental Intuition was correct. ";
                } else {
                    game_string = "Incorrect your Mental Intution was incorrect. ";
                }
                draw_card = card;
                next_input = false;
                setTimeout(displayCard, 3000);
            }
            break;
        }
    }
}

const images = [];

function loadGfx() {
    images[0] = new Image();
    images[0].src = "cardback.jpg"; 
    images[0].onload = function() {}
    images[1] = new Image();
    images[1].src = "card1.jpg"; 
    images[1].onload = function() {}
    images[2] = new Image();
    images[2].src = "card2.jpg"; 
    images[2].onload = function() {}
    images[3] = new Image();
    images[3].src = "card3.jpg"; 
    images[3].onload = function() {}
    console.log("Images loaded.\n");
}

function gameLoop(timestamp) {
    const deltaTime = timestamp - lastTime;
    lastTime = timestamp;

    update(deltaTime);
    draw();

    requestAnimationFrame(gameLoop);
}

function update(deltaTime) {

}

function draw() {
    context.clearRect(0, 0, canvas.width, canvas.height);
    context.fillStyle = '#fff';
    context.font = '20px Arial';
    context.fillText('Intuition Builder - ', 50, 50);
    context.fillStyle = "#FFFFFF";
    context.fillText(game_string, 225, 50);
    let x_pos = 50;
    for(let i = 0; i < deck.size-deck.index; ++i) {
        context.drawImage(images[0], x_pos, 100);
        x_pos += 10;
    }

    if(draw_card != 0) {
        context.drawImage(images[draw_card], 600, 100);
    }
    if(game_over == true) {
        context.fillStyle="#BD0000";
        context.fillText(psychic_string,150, 300);
    }
}   
init();
