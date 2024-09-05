#ifndef __CURRENT_H_GAME_
#define __CURRENT_H_GAME_



namespace current {

	template<int SIZE_W, int SIZE_H>
	class Current {
	public:

		enum direction { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };
		enum direction current_direction;

		int speed;
		unsigned int score, num;

		Current();
		void initgraph();
		void update();
		void timer_update();
		int operator()(int x, int y);
		void setGraphItem(int x, int y, unsigned char col);

		inline unsigned int getScore() const { return score; }
		inline unsigned int getClears() const { return num; }
		bool gameOver() const { return game_over; }

		void addLine();
		void moveLeft();
		void moveRight();
		void switchColors();

		unsigned char graph[SIZE_W][SIZE_H];
	protected:

		bool game_over;

		enum { HALF_SIZE = SIZE_H/2 };
		enum { MAX_COLORS = 8 };

		void mergeBlock();
		void moveBlocksDown();
		void procBlocks();

		class Block {
		public:

			int x, y, blocks[3];

			Block();
			Block(const Block &b);
			inline Block &operator=(const Block &b) { x=b.x, y=b.y; blocks[0]=b.blocks[0]; blocks[1]=b.blocks[1]; blocks[2]=b.blocks[2]; return *this; }
			void reset();
			void setRand(const int max);
			void move(direction d);
			void shift();
		};

	public:
		Block nextblock;
		Block gameblock;
	};
template<int SIZE_W, int SIZE_H>
	Current<SIZE_W, SIZE_H>::Current() {
		initgraph();
	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W,SIZE_H>::initgraph() {
		gameblock.reset();
		nextblock.reset();
		current_direction = MOVE_UP;
		score = num = 0;
		game_over = false;
		memset( graph , 0, sizeof(graph) );
		speed = 800;
	}

	template<int SIZE_W, int SIZE_H>
	int Current<SIZE_W, SIZE_H>::operator()(int x, int y) {
		if(x >= 0 && x < SIZE_W && y >= 0 && y < SIZE_H)
			return graph[x][y];

		return -1;
	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::setGraphItem(int x, int y, unsigned char col) {
		graph[x][y] = col;
	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::timer_update() {

		if(current_direction == MOVE_DOWN) {
			if(gameblock.y <= HALF_SIZE && operator()(gameblock.x, gameblock.y+3) > 0) {
				game_over = true;
				return;
			} else if(operator()(gameblock.x, gameblock.y+3) == 0)
				gameblock.move(MOVE_DOWN);
			else if((operator()(gameblock.x, gameblock.y+3) != 0) || (gameblock.y+3 >= SIZE_H)) mergeBlock();

		} else if(current_direction == MOVE_UP) {
			if(gameblock.y >= HALF_SIZE && operator()(gameblock.x, gameblock.y-3) > 0) {
				game_over = true;
				return;
			} else if(operator()(gameblock.x, gameblock.y-1) == 0)
				gameblock.move(MOVE_UP);
			else if((operator()(gameblock.x, gameblock.y-1) != 0) || ( gameblock.y-1 <= 0)) mergeBlock();
		}

		if(operator()(gameblock.x, gameblock.y) > 0)
			game_over = true;

	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::update() {

		moveBlocksDown();
		procBlocks();
	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::mergeBlock() {
		graph[gameblock.x][gameblock.y] = gameblock.blocks[0];
		graph[gameblock.x][gameblock.y+1] = gameblock.blocks[1];
		graph[gameblock.x][gameblock.y+2] = gameblock.blocks[2];
		current_direction = (current_direction == MOVE_UP) ? MOVE_DOWN : MOVE_UP;
		gameblock = nextblock;
		nextblock.reset();
	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::moveLeft() {

		if(operator()(gameblock.x-1, gameblock.y+3) == 0 && operator()(gameblock.x-1, gameblock.y-1) == 0)
			gameblock.move(MOVE_LEFT);


	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::moveRight() {

		if(operator()(gameblock.x+1, gameblock.y+3) == 0 && operator()(gameblock.x+1, gameblock.y-1) == 0)
			gameblock.move(MOVE_RIGHT);

	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::switchColors() {
		gameblock.shift();
	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::moveBlocksDown() {
		unsigned int i,z ;
		for(i = 0; i < SIZE_W; i++) {
			for(z = HALF_SIZE; z < SIZE_H; z++) {

				if(operator()(i,z) != 0 && operator()(i, z+1) == 0) {
					graph[i][z+1] = graph[i][z];
					graph[i][z] = 0;
				}

			}
		}
		for(i = 0; i < SIZE_W; i++) {
			for(z = HALF_SIZE; z > 0; z--) {
				if(operator()(i,z) != 0 && operator()(i, z-1) == 0) {
					graph[i][z-1] = graph[i][z];
					graph[i][z] = 0;
				}
			}
		}
	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::addLine() {
		score += 7;
		++num;

		if((num % 12) == 0) {
			// set speed loewr
			speed -= 40;
		}
	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::procBlocks() {

		int  i, z;
		unsigned char cur;

		for(i = 0; i < SIZE_W; i++) {
			for(z = 0; z < SIZE_H; z++) {
				cur = operator()(i, z);
				if(cur > 0) {
					if(operator()(i, z+1) == cur && operator()(i, z+2) == cur) {
						graph[i][z] = 0;
						graph[i][z+1] = 0;
						graph[i][z+2] = 0;

						if(operator()(i, z+3) == cur)
							graph[i][z+3] = 0;

						addLine();
					}
					if(operator()(i+1,z) == cur && operator()(i+2, z) == cur) {
						graph[i][z] = 0;
						graph[i+1][z] = 0;
						graph[i+2][z] = 0;
						if(operator()(i+3, z) == cur)
							graph[i+3][z] = 0;

						addLine();
					}

					if(operator()(i+1,z+1) == cur && operator()(i+2,z+2) == cur) {
						graph[i][z] = 0;
						graph[i+1][z+1] = 0;
						graph[i+2][z+2] = 0;

						addLine();
					}

					if(operator()(i-1, z-1) == cur && operator()(i-2,z-2) == cur) {
						graph[i][z] = 0;
						graph[i-1][z-1] = 0;
						graph[i-2][z-2] = 0;

						addLine();
					}

					if(operator()(i-1, z+1) == cur && operator()(i-2, z+2) == cur) {
						graph[i][z] = 0;
						graph[i-1][z+1] = 0;
						graph[i-2][z+2] = 0;

						addLine();
					}

					if(operator()(i+1, z-1) == cur && operator()(i+2, z-2) == cur) {
						graph[i][z] = 0;
						graph[i+1][z-1] = 0;
						graph[i+2][z-2] = 0;

						addLine();
					}
				}
			}
		}

	}

	template<int SIZE_W, int SIZE_H>
	Current<SIZE_W, SIZE_H>::Block::Block() {
		reset();
	}

	template<int SIZE_W, int SIZE_H>
	Current<SIZE_W, SIZE_H>::Block::Block(const Block &b) {
		operator=(b);
	}


	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::Block::reset() {
		x = SIZE_W/2;
		y = HALF_SIZE; // check if anything is in the way
		memset( blocks, 0, sizeof(blocks) );
		setRand(MAX_COLORS);
	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::Block::setRand(const int max) {
		do {
			blocks[0] = 1+(rand()%(max-1));
			blocks[1] = 1+(rand()%(max-1));
			blocks[2] = 1+(rand()%(max-1));
		} while( blocks[0] == blocks[1] && blocks[0] == blocks[2] );
	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::Block::move(direction d) {
		switch(d) {
		case MOVE_LEFT: if(x > 0) x--; return;
		case MOVE_RIGHT: if(x < SIZE_W) x++; return;
		case MOVE_UP: if(y > 0) y--; return;
		case MOVE_DOWN: if(y < SIZE_H) y++; return;
		}
	}

	template<int SIZE_W, int SIZE_H>
	void Current<SIZE_W, SIZE_H>::Block::shift() {

		int temp[3];
		temp[0] = blocks[0];
		temp[1] = blocks[1];
		temp[2] = blocks[2];

		blocks[0] = temp[2];
		blocks[1] = temp[0];
		blocks[2] = temp[1];

	}
}

#endif

