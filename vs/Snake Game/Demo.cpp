#include "Demo.h"
int target = 30;
int tailLength = 0;

Demo::Demo()
{
}

Demo::~Demo()
{

}

void Demo::renderPlayer(SDL_Renderer* renderer, SDL_Rect player, int x, int y, int scale, vector<int> tailX, vector<int> tailY, int tailLength) {
	SDL_SetRenderDrawColor(renderer, 0, 128, 248, 255);
	player.w = scale;
	player.h = scale;

	
	for (int i = 0; i < tailLength; i++) {
		player.x = tailX[i];
		player.y = tailY[i];
		SDL_RenderFillRect(renderer, &player);
	}
	if (tailLength < (target / 3))
	{
		SDL_SetRenderDrawColor(renderer, 255, 25, 25, 255);
	}
	else if (tailLength < (target - 1))
	{
		SDL_SetRenderDrawColor(renderer, 255, 105, 0, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 46, 255, 55, 255);
	}
	player.x = x;
	player.y = y;
	SDL_RenderFillRect(renderer, &player);
}

void Demo::renderFood(SDL_Renderer* renderer, SDL_Rect food) {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &food);
}

void Demo::renderScore(SDL_Renderer* renderer, int tailLength, int scale, int wScale) {
	SDL_Color Orange = { 225, 120, 0 };

	
	TTF_Font* font = TTF_OpenFont((char*)"snakebold.ttf", 10);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), Orange);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect scoreRect;
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale * wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);

	TTF_CloseFont(font);
}

bool Demo::checkCollision(int foodx, int foody, int playerx, int playery) {
	if (playerx == foodx && playery == foody) {
		return true;
	}
	return false;
}

pair<int, int> Demo::getFoodSpawn(vector<int> tailX, vector<int> tailY, int playerX, int playerY, int scale, int wScale, int tailLength) {
	bool valid = false;
	int x = 0;
	int y = 0;
	srand(time(0));
	x = scale * (rand() % wScale);
	y = scale * (rand() % wScale);
	valid = true;

	for (int i = 0; i < tailLength; i++) {

		if ((x == tailX[i] && y == tailY[i]) || (x == playerX && y == playerY)) {
			valid = false;
		}

	}

	if (!valid) {
		pair<int, int> foodLoc;
		foodLoc = make_pair(-100, -100);
		return foodLoc;
	}

	pair<int, int> foodLoc;
	foodLoc = make_pair(x, y);

	return foodLoc;
}

void Demo::gameOver(SDL_Renderer* renderer, SDL_Event event, int scale, int wScale, int tailLength) {
	SDL_Color Red = { 255, 0, 0 };
	SDL_Color White = { 255, 255, 255 };
	SDL_Color Black = { 0, 0, 0 };
	SDL_Color Orange = { 255,120,0 };

	TTF_Font* font = TTF_OpenFont((char*)"snakebold.ttf", 10);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* gameover = TTF_RenderText_Solid(font, "Game Over", Red);
	SDL_Surface* retry = TTF_RenderText_Solid(font, "Press Enter to retry", White);
	SDL_Surface* esc = TTF_RenderText_Solid(font, "Press Escape to quit", White);
	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), Orange);
	SDL_Texture* gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture* retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Texture* escMessage = SDL_CreateTextureFromSurface(renderer, esc);
	SDL_Rect gameoverRect;
	SDL_Rect retryRect;
	SDL_Rect scoreRect;
	SDL_Rect escRect;
	gameoverRect.w = 200;
	gameoverRect.h = 100;
	gameoverRect.x = ((scale * wScale) / 2) - (gameoverRect.w / 2);
	gameoverRect.y = ((scale * wScale) / 2) - (gameoverRect.h / 2) - 50;
	retryRect.w = 300;
	retryRect.h = 50;
	retryRect.x = ((scale * wScale) / 2) - ((retryRect.w / 2));
	retryRect.y = (((scale * wScale) / 2) - ((retryRect.h / 2)) + 150);
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale * wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	escRect.w = 300;
	escRect.h = 50;
	escRect.x = ((scale * wScale) / 2) - ((escRect.w / 2));
	escRect.y = (((scale * wScale) / 2) - ((escRect.h / 2)) + 190);
	SDL_RenderCopy(renderer, gameoverMessage, NULL, &gameoverRect);
	SDL_RenderCopy(renderer, retryMessage, NULL, &retryRect);
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);
	SDL_RenderCopy(renderer, escMessage, NULL, &escRect);

	TTF_CloseFont(font);

	while (true) {
		SDL_RenderPresent(renderer);

		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				exit(0);
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				return;
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				SDL_DestroyWindow(window);
				Engine::Game& game = Demo();
				game.Start("Snake Menu", 576, 576, true, WindowFlag::WINDOWED, 60, 1);
				status = false;
			}

		}

	}

}

void Demo::youWin(SDL_Renderer* renderer, SDL_Event event, int scale, int wScale, int tailLength) {
	SDL_Color Red = { 255, 0, 0 };
	SDL_Color White = { 255, 255, 255 };
	SDL_Color Black = { 0, 0, 0 };
	SDL_Color Yellow = { 255, 255, 0 };
	SDL_Color Orange = { 255,120,0 };

	TTF_Font* font = TTF_OpenFont((char*)"snakebold.ttf", 10);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* gameover = TTF_RenderText_Solid(font, "You won!", Yellow);
	SDL_Surface* retry = TTF_RenderText_Solid(font, "Press Enter to play again", White);
	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), Orange);
	SDL_Surface* esc = TTF_RenderText_Solid(font, "Press Escape to quit", White);
	SDL_Texture* gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture* retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Texture* escMessage = SDL_CreateTextureFromSurface(renderer, esc);
	SDL_Rect gameoverRect;
	SDL_Rect retryRect;
	SDL_Rect scoreRect;
	SDL_Rect escRect;
	gameoverRect.w = 200;
	gameoverRect.h = 100;
	gameoverRect.x = ((scale * wScale) / 2) - (gameoverRect.w / 2);
	gameoverRect.y = ((scale * wScale) / 2) - (gameoverRect.h / 2) - 50;
	retryRect.w = 300;
	retryRect.h = 50;
	retryRect.x = ((scale * wScale) / 2) - ((retryRect.w / 2));
	retryRect.y = (((scale * wScale) / 2) - ((retryRect.h / 2)) + 150);
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale * wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	escRect.w = 300;
	escRect.h = 50;
	escRect.x = ((scale * wScale) / 2) - ((escRect.w / 2));
	escRect.y = (((scale * wScale) / 2) - ((escRect.h / 2)) + 190);
	SDL_RenderCopy(renderer, gameoverMessage, NULL, &gameoverRect);
	SDL_RenderCopy(renderer, retryMessage, NULL, &retryRect);
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);
	SDL_RenderCopy(renderer, escMessage, NULL, &escRect);

	TTF_CloseFont(font);

	while (true) {
		SDL_RenderPresent(renderer);

		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				exit(0);
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				return;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				SDL_DestroyWindow(window);
				Engine::Game& game = Demo();
				game.Start("Snake Menu", 576, 576, true, WindowFlag::WINDOWED, 60, 1);
				status = false;
			}

		}

	}

}

void Demo::Init()
{
	InitText();
	InitButton();
	this->program = BuildShader("shader.vert", "shader.frag");
	InputMapping("SelectButton", SDLK_RETURN);
	InputMapping("NextButton", SDLK_DOWN);
	InputMapping("PrevButton", SDLK_UP);
}

void Demo::DeInit() {

}

void Demo::renderA() {
	//Setting Viewport
	glViewport(0, 0, GetScreenWidth(), GetScreenHeight());

	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set the background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Get the font used for displaying text
	// Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program, "projection"), 1, GL_FALSE, value_ptr(projection));


	RenderText("Otong", (GetScreenWidth() / 2) - 105, 120, 1.0f, vec3(255.0f / 255.0f, 120.0f / 255.0f, 10.0f / 255.0f));
}

void Demo::renderGame() {

	tailLength = 0;

	if (TTF_Init() < 0) {
		cout << "Error: " << TTF_GetError() << endl;
	}

	SDL_Renderer* renderer;
	SDL_Event event;

	SDL_Rect player;
	player.x = 0;
	player.y = 0;
	player.h = 0;
	player.w = 0;

	vector<int> tailX;
	vector<int> tailY;

	int scale = 24;
	int wScale = 24;

	int x = 0;
	int y = 0;
	int prevX = 0;
	int prevY = 0;

	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;
	bool inputThisFrame = false;
	bool redo = false;

	SDL_Rect food;
	food.w = scale;
	food.h = scale;
	food.x = 0;
	food.y = 0;

	pair<int, int> foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
	food.x = foodLoc.first;
	food.y = foodLoc.second;

	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scale * wScale + 1, scale * wScale + 1, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	float time = SDL_GetTicks() / 100;

	while (true) {

		float newTime = SDL_GetTicks() / this->div;
		float delta = newTime - time;
		time = newTime;

		inputThisFrame = false;

		if (tailLength >= target) {
			youWin(renderer, event, scale, wScale, tailLength);
			x = 0;
			y = 0;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
			tailLength = 0;
			redo = false;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);

			if (food.x == -100 && food.y == -100) {
				redo = true;
			}

			food.x = foodLoc.first;
			food.y = foodLoc.second;
		}

		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				exit(0);
			}
			if (event.type == SDL_KEYDOWN && inputThisFrame == false) {

				if (down == false && event.key.keysym.scancode == SDL_SCANCODE_UP) {
					up = true;
					left = false;
					right = false;
					down = false;
					inputThisFrame = true;
				}
				else if (right == false && event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					up = false;
					left = true;
					right = false;
					down = false;
					inputThisFrame = true;
				}
				else if (up == false && event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					up = false;
					left = false;
					right = false;
					down = true;
					inputThisFrame = true;
				}
				else if (left == false && event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					up = false;
					left = false;
					right = true;
					down = false;
					inputThisFrame = true;
				}

			}

		}

		prevX = x;
		prevY = y;

		if (up) {
			y -= delta * scale;
		}
		else if (left) {
			x -= delta * scale;
		}
		else if (right) {
			x += delta * scale;
		}
		else if (down) {
			y += delta * scale;
		}

		if (redo == true) {

			redo = false;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;

			if (food.x == -100 && food.y == -100) {
				redo = true;
			}

		}

		if (checkCollision(food.x, food.y, x, y)) {

			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;

			if (food.x == -100 && food.y == -100) {
				redo = true;
			}

			tailLength++;
		}

		if (delta * scale == 24) {

			if (tailX.size() != tailLength) {
				tailX.push_back(prevX);
				tailY.push_back(prevY);
			}

			for (int i = 0; i < tailLength; i++) {

				if (i > 0) {
					tailX[i - 1] = tailX[i];
					tailY[i - 1] = tailY[i];
				}

			}

			if (tailLength > 0) {
				tailX[tailLength - 1] = prevX;
				tailY[tailLength - 1] = prevY;
			}

		}

		for (int i = 0; i < tailLength; i++) {

			if (x == tailX[i] && y == tailY[i]) {
				gameOver(renderer, event, scale, wScale, tailLength);
				x = 0;
				y = 0;
				up = false;
				left = false;
				right = false;
				down = false;
				tailX.clear();
				tailY.clear();
				tailLength = 0;
				redo = false;

				foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
				if (food.x == -100 && food.y == -100) {
					redo = true;
				}

				food.x = foodLoc.first;
				food.y = foodLoc.second;
			}

		}

		if (x < 0 || y < 0 || x > scale * wScale - scale || y > scale * wScale - scale) {
			gameOver(renderer, event, scale, wScale, tailLength);
			x = 0;
			y = 0;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
			tailLength = 0;
			redo = false;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;

			if (food.x == -100 && food.y == -100) {
				redo = true;
			}

		}

		renderFood(renderer, food);
		renderPlayer(renderer, player, x, y, scale, tailX, tailY, tailLength);

		SDL_RenderDrawLine(renderer, 0, 0, 0, 24 * 24);
		SDL_RenderDrawLine(renderer, 0, 24 * 24, 24 * 24, 24 * 24);
		SDL_RenderDrawLine(renderer, 24 * 24, 24 * 24, 24 * 24, 0);
		SDL_RenderDrawLine(renderer, 24 * 24, 0, 0, 0);

		SDL_RenderPresent(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
	}

	SDL_DestroyWindow(window);

	TTF_Quit();

	SDL_Quit();

}

void Demo::Update(float deltaTime)
{
	if (IsKeyDown("SelectButton")) {
		if (activeButtonIndex == 2) {
			SDL_Quit();
			exit(0);
		}
		if (activeButtonIndex == 1) {
			this->div = 100;
			status = true;
		}
		if (activeButtonIndex == 0)
		{
			this->div = 60;
			status = true;
		}
	}

	if (IsKeyUp("NextButton")) {
		if (activeButtonIndex < NUM_BUTTON - 1) {
			activeButtonIndex++;
			SDL_Delay(150);
		}
	}

	if (IsKeyUp("PrevButton")) {
		if (activeButtonIndex > 0) {
			activeButtonIndex--;
			SDL_Delay(150);
		}
	}

}

void Demo::Render()
{
	//Setting Viewport
	glViewport(0, 0, GetScreenWidth(), GetScreenHeight());

	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set the background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program, "projection"), 1, GL_FALSE, value_ptr(projection));


	RenderText("SNAKE", (GetScreenWidth() / 2) - 95, 120, 1.0f, vec3(255.0f / 255.0f, 120.0f / 255.0f, 10.0f / 255.0f));

	RenderButton();

}

void Demo::InitText() {
	// Init Freetype
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		Err("ERROR::FREETYPE: Could not init FreeType Library");
	}
	FT_Face face;
	if (FT_New_Face(ft, FONTNAME, 0, &face)) {
		Err("ERROR::FREETYPE: Failed to load font");
	}

	FT_Set_Pixel_Sizes(face, 0, FONTSIZE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(pair<GLchar, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, NULL,
		GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Demo::RenderText(string text, GLfloat x, GLfloat y, GLfloat scale, vec3 color)
{
	// Activate corresponding render state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	UseShader(this->program);

	glUniform3f(glGetUniformLocation(this->program, "ourColor"), color.x, color.y, color.z);
	glUniform1i(glGetUniformLocation(this->program, "text"), 1);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(this->program, "ourTexture"), 0);
	mat4 model;
	glUniformMatrix4fv(glGetUniformLocation(this->program, "model"), 1, GL_FALSE, value_ptr(model));
	glBindVertexArray(VAO);

	// Iterate through all characters
	string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];
		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * scale;
		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character

		GLfloat vertices[] = {
			// Positions   // Texture Coords
			xpos + w,	ypos + h,	1.0f, 1.0f, // Bottom Right
			xpos + w,	ypos,		1.0f, 0.0f, // Top Right
			xpos,		ypos,		0.0f, 0.0f, // Top Left
			xpos,		ypos + h,	0.0f, 1.0f  // Bottom Left 
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_QUADS, 0, 4);
		// Now advance cursors for next glyph (note that advance is number of 1 / 64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels(2 ^ 6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void Demo::InitButton() {
	string buttons[NUM_BUTTON] = { "button_hard.png", "button_normal.png", "button_halt.png" };
	string hover_buttons[NUM_BUTTON] = { "button_hard_hover.png", "button_normal_hover.png","button_halt_hover.png" };

	glGenTextures(3, &texture[0]);
	glGenTextures(3, &hover_texture[0]);

	for (int i = 0; i < NUM_BUTTON; i++) {
		// Load and create a texture 
		glBindTexture(GL_TEXTURE_2D, texture[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image(buttons[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

		// Set up vertex data (and buffer(s)) and attribute pointers
		button_width[i] = (float)width;
		button_height[i] = (float)height;

		// Load and create a texture 
		glBindTexture(GL_TEXTURE_2D, hover_texture[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		image = SOIL_load_image(hover_buttons[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

		 // Set up vertex data (and buffer(s)) and attribute pointers
		hover_button_width[i] = (float)width;
		hover_button_height[i] = (float)height;
	}

	GLfloat vertices[] = {
		// Positions	// Texture Coords
		1,  1,			1.0f, 1.0f, // Bottom Right
		1,  0,			1.0f, 0.0f, // Top Right
		0,  0,			0.0f, 0.0f, // Top Left
		0,  1,			0.0f, 1.0f  // Bottom Left 
	};


	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Demo::RenderButton() {
	// Enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	int texture_id[NUM_BUTTON] = { GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3 };
	// Activate shader
	UseShader(this->program);
	glUniform1i(glGetUniformLocation(this->program, "text"), 0);

	glBindVertexArray(VAO2);
	for (int i = 0; i < NUM_BUTTON; i++) {

		glActiveTexture(texture_id[i]);
		glBindTexture(GL_TEXTURE_2D, (activeButtonIndex == i) ? hover_texture[i] : texture[i]);
		glUniform1i(glGetUniformLocation(this->program, "ourTexture"), i + 1);

		mat4 model;
		model = translate(model, vec3((GetScreenWidth() - button_width[i]) / 2, 100 + (i + 1) * 100, 0.0f));
		model = scale(model, vec3(button_width[i], button_height[i], 1));
		glUniformMatrix4fv(glGetUniformLocation(this->program, "model"), 1, GL_FALSE, value_ptr(model));

		glDrawArrays(GL_QUADS, 0, 4);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}


int main(int argc, char** argv) {

	Engine::Game& game = Demo();
	game.Start("Snake Menu", 576, 576, true, WindowFlag::WINDOWED, 60, 1);
	return 0;
}
