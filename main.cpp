#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <irrklang/irrKlang.h>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

//using namespace irrklang;
//#pragma comment(lib, "irrKlang.lib")

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

//ISoundEngine* SoundEngine = createIrrKlangDevice();

Texture* numbersTextures = new Texture[9];
const char* paths[] = {
        "resources/textures/number_one.png" ,
        "resources/textures/number_two.png",
        "resources/textures/number_three.png",
        "resources/textures/number_four.png",
        "resources/textures/number_five.png",
        "resources/textures/number_six.png ",
        "resources/textures/number_seven.png",
        "resources/textures/number_eight.png",
        "resources/textures/square_2.png",
};

int startBoard[3][3];
int GameBoard[3][3] = {
    {0,0,0},
    {0,0,0},
    {0,0,0},
};
int WinBoard[3][3] = {
    {1,2,3},
    {4,5,6},
    {7,8,9},
};
int emptyY = 2, emptyX = 2;

void WinGame();
void restartGame();
void renderGame(int array[3][3]);
void processInput(GLFWwindow* window, int key, int scancode, int action, int mods);
bool checkWin();


int main()
{
    std::cout << "ENTER 8 NUMBERS TO START : \n";
    for (int i=0; i < 3; i++) {
        for (int j=0; j < 3; j++) {
            std::cin >> GameBoard[i][j];
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
             startBoard[i][j] = GameBoard[i][j];

        }
    }

    for (int i = 0; i < 3; i++) {
        std::cout << "\n";
        for (int j = 0; j < 3; j++) {
           std:: cout <<  startBoard[i][j] << "  ";

        }
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Sliding_Number_Puzzle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gladLoadGL();
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    Shader shaderProgram("default.vert", "default.frag");
    
    float Back_vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,// bottom left  
        1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,// bottom right
        -1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,// top left

        -1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,// top left 
        1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,// top right
       1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f// bottom right

    };

    float Board_vertices[] = {
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,// bottom left
        -0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,// top left
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,// bottom right

        0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,// top right
        -0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,// top left
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f// bottom right

    };

    float restart_vertices[] = {

        // restart button
       -0.25f, -0.9f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,// bottom left
       -0.25f, -0.7f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,// top left
       0.25f, -0.9f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,// bottom right

       0.25f, -0.7f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,// top right
       -0.25f, -0.7f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,// top left
       0.25f, -0.9f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f// bottom right
    };

    float x = -0.45, y = 0.45, z = 0.0f, side = 0.29, inc = 0.305;

    float Squares_vertices[] = {

        // first
        x, y, z,                1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // left top 
        x + side, y, z,         1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // right top
        x, y - side, z,         0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // left down
        x + side, y - side, z,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // right down

        // second
        x + inc, y, z,              1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // left top  
        x + side + inc, y, z,       1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right top
        x + inc, y - side, z,       0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // left down
        x + side + inc, y - side, z,0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // right down

        // third
        x + 2 * inc, y, z,               1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // left top  
        x + side + 2 * inc, y, z,        1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right top
        x + 2 * inc, y - side, z,        0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // left down
        x + side + 2 * inc, y - side, z, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // right down

        // fourth
        x, y - inc, z,              1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // left top  
        x + side, y - inc, z,       1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right top
        x, y - side - inc, z,       0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // left down
        x + side, y - side - inc, z,0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // right down

        // fifth
        x + inc, y - inc, z,               1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // left top  
        x + side + inc, y - inc, z,        1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right top
        x + inc, y - side - inc, z,        0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // left down
        x + side + inc, y - side - inc, z, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // right down

        // sixth
        x + 2 * inc, y - inc, z,               1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // left top  
        x + side + 2 * inc, y - inc, z,        1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right top
        x + 2 * inc, y - side - inc, z,        0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // left down
        x + side + 2 * inc, y - side - inc, z, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // right down

        // seventh
        x, y - 2 * inc, z,                1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // left top  
        x + side, y - 2 * inc, z,       1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right top
        x, y - side - 2 * inc, z,       0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // left down
        x + side, y - side - 2 * inc, z,0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // right down

        // eighth
        x + inc, y - 2 * inc, z,                1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // left top  
        x + side + inc, y - 2 * inc, z,       1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right top
        x + inc, y - side - 2 * inc, z,       0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // left down
        x + side + inc, y - side - 2 * inc, z,0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // right down

        // ninth
        x + 2 * inc, y - 2 * inc, z,               1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // left top  
        x + side + 2 * inc, y - 2 * inc, z,        1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right top
        x + 2 * inc, y - side - 2 * inc, z,        0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // left down
        x + side + 2 * inc, y - side - 2 * inc, z, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // right down

    };

    unsigned int indices[] = {
        //first
        0, 2, 3,
        0, 1, 3,

        //second
        4, 6, 7,
        4, 5, 7,

        //third
        8, 10, 11,
        8, 9, 11,

        //fourth
        12, 14, 15,
        12, 13, 15,

        //fifth
        16, 18, 19,
        16, 17, 19,

        //sixth
        20, 22, 23,
        20, 21, 23,

        //seventh
        24, 26, 27,
        24, 25, 27,

        //eighth
        28, 30, 31,
        28, 29, 31,

        //eighth
        32, 34, 35,
        32, 33, 35,
    };

    // Background
    VAO Background_VAO;
    Background_VAO.Bind();

    VBO Background_VBO(Back_vertices, sizeof(Back_vertices));

    Background_VAO.LinkAttrib(Background_VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    Background_VAO.LinkAttrib(Background_VBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    Background_VAO.LinkAttrib(Background_VBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    
    // Board
    VAO Board_VAO;
    Board_VAO.Bind();

    VBO Board_VBO(Board_vertices, sizeof(Board_vertices));
    
    Board_VAO.LinkAttrib(Board_VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    Board_VAO.LinkAttrib(Board_VBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    Board_VAO.LinkAttrib(Board_VBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Squares
    VAO Squares_VAO;
    Squares_VAO.Bind();

    VBO Squares_VBO(Squares_vertices, sizeof(Squares_vertices));
    EBO Squares_EBO(indices, sizeof(indices));

    Squares_VAO.LinkAttrib(Squares_VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    Squares_VAO.LinkAttrib(Squares_VBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    Squares_VAO.LinkAttrib(Squares_VBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    
    // Restart
    VAO Restart_VAO;
    Restart_VAO.Bind();

    VBO Restart_VBO(restart_vertices, sizeof(restart_vertices));

    Restart_VAO.LinkAttrib(Restart_VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    Restart_VAO.LinkAttrib(Restart_VBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    Restart_VAO.LinkAttrib(Restart_VBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Textures
    Texture Background ("resources/textures/background_final.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Background.texUnit(shaderProgram, "texture1", 0);

    Texture Board("resources/textures/border_2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Board.texUnit(shaderProgram, "texture1", 0);

    Texture Restart("resources/textures/restart_2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Restart.texUnit(shaderProgram, "texture1", 0);

    Texture Square("resources/textures/square_3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Board.texUnit(shaderProgram, "texture1", 0);

    Texture Win("resources/textures/Win.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Win.texUnit(shaderProgram, "texture1", 0);

    for (int i = 0; i < 9; i++) {

        numbersTextures[i] = Texture(paths[i], GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        numbersTextures[i].texUnit(shaderProgram, "texture1", 0);
    }
    glBindVertexArray(0);

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, processInput);

       //SoundEngine->play2D("resorces/sounds/game.mp3", true);
            
            while (!checkWin()) {

                shaderProgram.Activate();
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                Background.Bind();
                Background_VAO.Bind();
                glDrawArrays(GL_TRIANGLES, 0, 6);

                Restart.Bind();
                Restart_VAO.Bind();
                glDrawArrays(GL_TRIANGLES, 0, 6);

                Board.Bind();
                Board_VAO.Bind();
                glDrawArrays(GL_TRIANGLES, 0, 6);

                Squares_VAO.Bind();
                renderGame(GameBoard);

                glfwSwapBuffers(window);
                glfwPollEvents();
            }

            while (!glfwWindowShouldClose(window))
            {

                Win.Bind();
                Background_VAO.Bind();
                glDrawArrays(GL_TRIANGLES, 0, 6);

                glfwSwapBuffers(window);
                glfwPollEvents();
                //glfwSetWindowShouldClose(window, true);
                //std::cout << "You Win !!";
            }

        Background_VAO.Delete();
        Background_VBO.Delete();

        Board_VAO.Delete();
        Board_VBO.Delete();

        Restart_VAO.Delete();
        Restart_VBO.Delete();

        Squares_VAO.Delete();
        Squares_VBO.Delete();
        Squares_EBO.Delete();

        Background.Delete();
        Board.Delete();
        Square.Delete();
        Restart.Delete();
        Win.Delete();

        glfwTerminate();
        return 0;
    }

void processInput(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS) {

        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);

        if (key == GLFW_KEY_R)
            restartGame();

        if (key == GLFW_KEY_SPACE)
            WinGame();

        if (key == GLFW_KEY_W) {

            if (emptyY < 2) {
                // Swap the empty space with the square above
                std::swap(GameBoard[emptyY][emptyX], GameBoard[emptyY + 1][emptyX]);
                emptyY++;
            }
            else restartGame();
        }
        else if (key == GLFW_KEY_S) {
            if (emptyY > 0) {
                // Swap the empty space with the square below
                std::swap(GameBoard[emptyY][emptyX], GameBoard[emptyY - 1][emptyX]);
                emptyY--;
            }
             else restartGame();
        }
        else if (key == GLFW_KEY_A) {
            if (emptyX < 2) {
                // Swap the empty space with the square to the left
                std::swap(GameBoard[emptyY][emptyX], GameBoard[emptyY][emptyX + 1]);
                emptyX++;

            }
             else restartGame();
        }
        else if (key == GLFW_KEY_D) {
            if (emptyX > 0) {
                // Swap the empty space with the square to the right
                std::swap(GameBoard[emptyY][emptyX], GameBoard[emptyY][emptyX - 1]);
                emptyX--;
            }
            else restartGame();
        }
        else if (key == GLFW_MOUSE_BUTTON_LEFT){
            restartGame();
        }
    }
}

void renderGame(int array[3][3]){
    
    unsigned int count = 0;
    //int s = startBoard[0][0];

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int index = array[i][j] - 1;
            numbersTextures[index].Bind();
            glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
            count += 4;
        }
    }
}

void restartGame() {

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            GameBoard[i][j] = startBoard[i][j];
        }
    }

    emptyX = 2;
    emptyY = 2;

    for (int i = 0; i < 3; i++) {
        std::cout << "\n";
        for (int j = 0; j < 3; j++) {
            std::cout << GameBoard[i][j] << "  ";
        }
    }
}

bool checkWin() {

    int count = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (GameBoard[i][j] != WinBoard[i][j])
                count++;
        }
    }
    std::cout << count;
    if (count == 0) {
        return true;
    }
}

void WinGame() {

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            GameBoard[i][j] = WinBoard[i][j];
        }
    }

    emptyX = 2;
    emptyY = 2;

}