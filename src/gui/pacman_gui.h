#ifndef pacman_gui_h
#define pacman_gui_h

#include "renderer.h"
#include "shader.h"
#include "vertex_array.h"
#include "texture.h"
#include "Windows.h"
#include "time.h"

const int BoardWindow_Width  = 872;
const int BoardWindow_Height = 872;
const int padding     = 20;
const int BoardWidth  = BoardWindow_Width  - 2 * padding;
const int BoardHeight = BoardWindow_Height - 2 * padding;
const int BoxHeight   = BoardHeight / 26;
const int BoxWidth    = BoardWidth  / 32;
const double trasparencyFactor = 0.6f;

int position = 0;
int gBlinkyPos = 398;
int gInkyPos = 399;
int gPinkyPos = 400;
int gClydePos = 401;
int gameOverPos = 0; 

const int sleepTime = 100;

// float OriginalColors[26][26][3];
float BoxColors[26][32][3];

GLint u_Color;
GLint u_RowShift;
GLint u_ColShift;
GLint u_isTexture;
GLint isGameOver;

//Textures---------------------------------------------------------------------
//Pacman
Texture pacmanOpened;
Texture pacmanClosed;

//Ghosts
Texture gBlinkyUp0;
Texture gBlinkyUp1;

Texture gInkyUp0;
Texture gInkyUp1;

Texture gPinkyUp0;
Texture gPinkyUp1;

Texture gClydeUp0;
Texture gClydeUp1;

//Pellets, Powerups etc.
Texture pellet;
Texture powerPellet;

Texture gameOver;

/* Map Key
0 - Pellet
1 - Blue (wall)
2 - Black (space to move)
3 - Gate
4 - Power Pellets
*/

char map[26][32] = {
    "00000000000000011000000000000000",
    "01111101111111011011111110111110",
    "41111101111111011011111110111114",
    "00000000000000000000000000000000",
    "01111101110111111111101110111110",
    "01111101110111111111101110111110",
    "00000001110000011000001110000000",
    "11111101111111211211111110111111",
    "11111101111111211211111110111111",
    "22221101112222222222221110112222",
    "11111101112111133111121110111111",
    "11111101112112222221121110111111",
    "22222202222112222221122220222222",
    "11111101112112222221121110111111",
    "11111101112111111111121110111111",
    "22221101112222222222221110112222",
    "11111101111111211211111110111111",
    "11111101111111211211111110111111",
    "00000001110000011000001110000000",
    "01111101110111111111101110111110",
    "41111101110111111111101110111114",
    "00000000000000000000000000000000",
    "01111101111111011011111110111110",
    "01111101111111011011111110111110",
    "00000000000000011000000000000000",
    "11111111111111111111111111111111"
};

/* Function declarations ------------------------------------------------------------------ */

void PacmanGUI ();
void WindowSetup     (Renderer*);
void GameLoop        (Renderer*);

void Window0_Update  (Window*, VertexArray* vao, IndexBuffer* ibo, ShaderProgram* program);
void Window0_Process (Window*);

/* Function definitions ------------------------------------------------------------------- */

void PacmanGUI () {
    Renderer renderer;
    Renderer_Constructor(&renderer);
    WindowSetup(&renderer);
    GameLoop(&renderer);
    Renderer_Destructor(&renderer);
}

void WindowSetup (Renderer* renderer) {
    Window_AddHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    Window_AddHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    Window_AddHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    Window_AddHint(GLFW_RESIZABLE, GLFW_FALSE);

    const int xPos0 = (renderer->ScreenWidth  - BoardWindow_Width)  / 2;
    const int yPos0 = (renderer->ScreenHeight - BoardWindow_Height) / 2;

    Renderer_CreateWindow(renderer, BoardWindow_Width, BoardWindow_Height, "Pacman");
    Window_SetClearColor(&renderer->m_Windows.data[0], (int[]) { 0, 0, 255*0.7, 255 });
    Window_SetPosition(&renderer->m_Windows.data[0], xPos0, yPos0);
}

void GameLoop (Renderer* renderer) {

    //top left, top right, bottom right, bottom left
    GLfloat vertices[] = {
      // Box coordinates                                                                           Texture coordinates
        -BoardWindow_Width / 2 + padding           , BoardWindow_Height / 2 - padding            , 0.0f, 1.0f,
        -BoardWindow_Width / 2 + padding + BoxWidth, BoardWindow_Height / 2 - padding            , 1.0f, 1.0f,
        -BoardWindow_Width / 2 + padding + BoxWidth, BoardWindow_Height / 2 - padding - BoxHeight, 1.0f, 0.0f,
        -BoardWindow_Width / 2 + padding           , BoardWindow_Height / 2 - padding - BoxHeight, 0.0f, 0.0f
    };

    for (int i = 0; i < 4 * 4; i += 4) {
        vertices[i]     /= BoardWindow_Width  / 2.0f;
        vertices[i + 1] /= BoardWindow_Height / 2.0f;
    }

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // const  float max_RGBAValue = 255.0f;
    //Colors for board
    for (int i = 0; i < 26; ++i) for (int j = 0; j < 32; ++j) {
        for (int k = 0; k < 3; ++k) {
            if (map[i][j] == '1')
                BoxColors[i][j][2] = 0.7f;//blue
            else if (map[i][j] == '2')
                BoxColors[i][j][k] = 0.0f;//black
            //gate
            else if (map[i][j] == '3'){
                BoxColors[i][j][0] = 0.88f;
                BoxColors[i][j][1] = 0.08f;
                BoxColors[i][j][2] = 0.57f;
            }

        }
    }

    Window_Activate(&renderer->m_Windows.data[0]);

    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_CALL(glEnable(GL_BLEND));

    VertexArray vao;
    VertexArray_Constructor(&vao);

    VertexBuffer vbo;
    VertexBuffer_Constructor(&vbo, vertices, 4 * 4 * GetSize(GL_FLOAT));

    BufferLayout layout;
    BufferLayout_Constructor(&layout);
    BufferLayout_Add_GLfloat(&layout, 2);
    BufferLayout_Add_GLfloat(&layout, 2);

    VertexArray_AddBuffer(&vao, &vbo, &layout);

    IndexBuffer ibo;
    IndexBuffer_Constructor(&ibo, indices, 6);

    Shader vertexShader;
    Shader fragmentShader;
    Shader_Constructor(&vertexShader, GL_VERTEX_SHADER, "../src/gui/shader/vertex.glsl");
    Shader_Constructor(&fragmentShader, GL_FRAGMENT_SHADER, "../src/gui/shader/fragment.glsl");

    ShaderProgram program;
    ShaderProgram_Constructor(&program);
    ShaderProgram_AddShader(&program, &vertexShader);
    ShaderProgram_AddShader(&program, &fragmentShader);
    ShaderProgram_Link(&program);
    ShaderProgram_Bind(&program);
    
    Texture_Constructor(&pacmanOpened, "../resources/pacman_opened.png");
    Texture_Constructor(&pacmanClosed, "../resources/pacman_closed.png");

    Texture_Constructor(&pellet, "../resources/pellet.png");
    Texture_Constructor(&powerPellet, "../resources/pellet_big.png");
    
    Texture_Constructor(&gBlinkyUp0, "../resources/ghosts/Blinky/up0.png");
    Texture_Constructor(&gBlinkyUp1, "../resources/ghosts/Blinky/up1.png");
    
    Texture_Constructor(&gInkyUp0, "../resources/ghosts/Inky/up0.png");
    Texture_Constructor(&gInkyUp1, "../resources/ghosts/Inky/up1.png");

    Texture_Constructor(&gPinkyUp0, "../resources/ghosts/Pinky/up0.png");
    Texture_Constructor(&gPinkyUp1, "../resources/ghosts/Pinky/up1.png");

    Texture_Constructor(&gClydeUp0, "../resources/ghosts/Clyde/up0.png");
    Texture_Constructor(&gClydeUp1, "../resources/ghosts/Clyde/up1.png");
    
    Texture_Constructor(&gameOver, "../resources/gameOver.png");

    GL_CALL(u_Color     = glGetUniformLocation(program.m_ID, "u_Color"));
    GL_CALL(u_RowShift  = glGetUniformLocation(program.m_ID, "u_RowShift"));
    GL_CALL(u_ColShift  = glGetUniformLocation(program.m_ID, "u_ColShift"));
    GL_CALL(u_isTexture = glGetUniformLocation(program.m_ID, "u_isTexture"));
    
    srand(time(NULL));
    while (true) {
        bool isWindowClosed = false;
        for (int i = 0; i < renderer->m_Windows.size; ++i) {
            Window* window = &renderer->m_Windows.data[i];

            if (!is_WindowOpen(window)) {
                isWindowClosed = true;
                break;
            }

            Window_ClearScreen(window);
        }

        if (isWindowClosed)
            break;
        
        Window* window0 = &renderer->m_Windows.data[0];
        Window_Activate(window0);
        Window0_Update(window0, &vao, &ibo, &program);
        Window0_Process(window0);
        Sleep(sleepTime);

        for (int i = 0; i < renderer->m_Windows.size; ++i) {
            Window* window = &renderer->m_Windows.data[i];
            Window_SwapBuffers(window);
        }
        
        glfwPollEvents();
    }

    ShaderProgram_Destructor(&program);
    IndexBuffer_Destructor(&ibo);
    BufferLayout_Destructor(&layout);
    VertexBuffer_Destructor(&vbo);
    VertexArray_Destructor(&vao);
}

//To draw objects - switches between two specified textures
void drawObject(int position, Texture texture1, Texture texture2){
    float x_shift = position % 32;
    float y_shift = position / 32;
    
    int parity = (int)(x_shift + y_shift) % 2;

    x_shift *= (float)BoxWidth  / (BoardWindow_Width  / 2.0f);
    y_shift *= (float)BoxHeight / (BoardWindow_Height / 2.0f);

    Texture_Bind((parity ? &texture1 : &texture2), 0);
    GL_CALL(glUniform1f(u_RowShift, x_shift));
    GL_CALL(glUniform1f(u_ColShift, y_shift));

    GL_CALL(glUniform1i(u_isTexture, true));
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
    GL_CALL(glUniform1i(u_isTexture, false));
    Texture_Unbind((parity ? &texture1 : &texture2)); 
}

void drawBoard () {
    for (int i = 0; i < 26; ++i) for (int j = 0; j < 32; ++j) {
        float shift1 = (float)j * BoxWidth  / (BoardWindow_Width  / 2.0f);
        float shift2 = (float)i * BoxHeight / (BoardWindow_Height / 2.0f);

        int pos = 32*i+j;
        if (map[i][j] == '0'){
            drawObject(pos, pellet, pellet);
        }
        else if (map[i][j] == '4'){
            drawObject(pos, powerPellet, powerPellet);
        }
        else{
            GL_CALL(glUniform4f(u_Color, BoxColors[i][j][0], BoxColors[i][j][1], BoxColors[i][j][2], 1.0f));
            GL_CALL(glUniform1f(u_RowShift, shift1));
            GL_CALL(glUniform1f(u_ColShift, shift2));
            GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
        }   
    }

    //make border
    for (int i = 12; i < 13; ++i) for (int j = -1; j < 33; j += 33) {
        float shift1 = (float)j * BoxWidth  / (BoardWindow_Width  / 2.0f);
        float shift2 = (float)i * BoxHeight / (BoardWindow_Height / 2.0f);
   
        GL_CALL(glUniform4f(u_Color, 0.0f, 0.0f, 0.0f, 1.0f));
        // GL_CALL(glUniform4f(u_Color, 0.5f, 0.0f, 0.0f, 1.0f));
        GL_CALL(glUniform1f(u_RowShift, shift1));
        GL_CALL(glUniform1f(u_ColShift, shift2));

        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
    }
}

//Checks collision between pacman and Ghosts
bool death() {
    if (position == gBlinkyPos ||
        position == gPinkyPos || 
        position == gInkyPos ||
        position == gClydePos){
        return true;
        }
    return false;
}

//Function to calculate the score
//Pellets eaten = Total Pellets - Pellets left
int score(){ 
    int score = 25000;
    for (int i = 0; i < 26; ++i) for (int j = 0; j < 32; ++j) {
        if (map[i][j] == '0')
            score -= 100;
        else if (map[i][j] == '4')
            score -= 400;
    }
    return score;
}

void Window0_Update (Window* window, VertexArray* vao, IndexBuffer* ibo, ShaderProgram* program) {
    ShaderProgram_Bind(program);
    VertexArray_Bind(vao);
    IndexBuffer_Bind(ibo);

    drawBoard();
    drawObject(position, pacmanOpened, pacmanClosed);
    drawObject(gBlinkyPos, gBlinkyUp0, gBlinkyUp1);
    drawObject(gInkyPos, gInkyUp0, gInkyUp1);
    drawObject(gPinkyPos, gPinkyUp0, gPinkyUp1);
    drawObject(gClydePos, gClydeUp0, gClydeUp0);

    if (death()) {
        drawObject(gameOverPos, gameOver, gameOver);
        printf("\n\n\n\n-----------------------------------------------------\n\n\n\n");
        printf("                      GAME OVER!\n\n");
        printf("                     SCORE: %d", score());
        printf("\n\n\n\n-----------------------------------------------------\n\n\n\n");
        Sleep(200);
        glfwSetWindowShouldClose(window->m_Window, true);
    }
}

//Checks if a move is valid
/* 
-1 = Left
+1 = Right
-32 = Up
+32 = Down
*/
int validMove(int position, int prev, int move, int isPacman) {
    int x;
    int y;
    int newPosition;

    // Ghosts shouldn't go back to the direction they were coming in
    // to avoid oscillation and ineffective movement
    if (!isPacman && (prev+move) == 0)
        return 0; 

    //384 and 415
    //Teleportation in the right and left edge centers
    if (move == -1 && position == 384){
        position = 415;
        return 31;
    }
    else if (move == 1 && position == 415){
        position = 384;
        return -31;
    }

    //checks if the move is within the bounds
    if ((move == -1) && ((position - 1) % 32 != 31 && position >= 1) ||
        (move == +1) && ((position + 1) % 32 != 0 && position + 1 < 832) ||
        (move == -32) && (position - 32 >= 0) ||
        (move == +32) && (position + 32 < 832)) {
            newPosition = position + move;
            x = newPosition % 32;
            y = newPosition / 32;
    }
    else
        return 0;

    //Checks whether the move is towards wall/gate or not
    if (map[y][x] != '1' && map[y][x] != '3'){
        //Eats pellet if Pacman
        if (isPacman)
            map[y][x] = '2';
        return move; 
    }
    else
        return 0;
}


//Moving the ghosts out of the box at the starting
void whoLetTheGhostsOut(int step){
    int moves[4][4] = {{-32, -32, 1, -1}, {-32, -32, -32, -32}, {-32, -32, -32, -32}, {-1, 1, -32, -32}};
    gInkyPos    +=  moves[step][0];
    gPinkyPos   +=  moves[step][1];
    gBlinkyPos  +=  moves[step][2];
    gClydePos   +=  moves[step][3];
}

//Starting movement directions of the ghosts after they are out of the box
int gBlinkyPrev = -32;
int gPinkyPrev = +1;
int gInkyPrev = -1;
int gClydePrev = -32;

//Function that randomizes the movement of the ghosts
void moveGhosts() {
    int moves[] = {1, -1, 32, -32};

    void moveGhost(int *ghostPos, int *ghostPrev){
        int ghostMove = validMove(*ghostPos, *ghostPrev, moves[rand()%4], 0);
        if (ghostMove + *ghostPrev != 0){
            *ghostPos += ghostMove;
            if (ghostMove != 0)
                *ghostPrev = ghostMove;
        }   
    }

    moveGhost(&gBlinkyPos, &gBlinkyPrev);
    moveGhost(&gInkyPos, &gInkyPrev);
    moveGhost(&gPinkyPos, &gPinkyPrev);
    moveGhost(&gClydePos, &gClydePrev);
}

int step = 0;

void Window0_Process (Window* window) {

    if (step < 4){
        whoLetTheGhostsOut(step);
        step += 1;
    }
    else    
        moveGhosts();

    static int key = GLFW_KEY_LEFT;
    int pacManPrev = -1;

    if (glfwGetKey(window->m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window->m_Window, true);
    
    if (glfwGetKey(window->m_Window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        key = GLFW_KEY_LEFT;
    }

    if (glfwGetKey(window->m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        key = GLFW_KEY_RIGHT; 
    }

    if (glfwGetKey(window->m_Window, GLFW_KEY_UP) == GLFW_PRESS) {
        key = GLFW_KEY_UP;
    }

    if (glfwGetKey(window->m_Window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        key = GLFW_KEY_DOWN;  
    }

    switch (key){
        case GLFW_KEY_LEFT : {
            pacManPrev = validMove(position, pacManPrev, -1, 1);
            position += pacManPrev;
        }break;

        case GLFW_KEY_RIGHT : {
            pacManPrev = validMove(position, pacManPrev, 1, 1);
            position += pacManPrev;
        }break;
        
        case GLFW_KEY_UP : {
            pacManPrev = validMove(position, pacManPrev, -32, 1);   
            position += pacManPrev;
        }break;

        case GLFW_KEY_DOWN : {
            pacManPrev = validMove(position, pacManPrev,  32, 1);   
            position += pacManPrev;
        }break;
    }
}

#endif