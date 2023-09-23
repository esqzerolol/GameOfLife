#include "raylib.h"
#include <bits/stdc++.h>
using namespace std;
typedef vector<vector<bool>> GridType;
typedef pair<int,int> PositionType;

static int FPS = 24;
static int ScreenWidth = 480;
static int ScreenHeight = 480;
static int CellSize = 24;
static int GridSize = ScreenHeight/CellSize;

static void InitGame(void);         // Inicializa o jogo
static void UpdateGame(void);       // Atualiza um frame (um passo)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

static void DrawGrid();

PositionType RetornarPosMouse();

struct Grid {
    GridType Alive;

    Grid (){
        Alive.resize(GridSize);
        for (int i = 0; i < GridSize; i++) Alive.at(i).resize(GridSize);
    }

    // Executa um passo
    void Exec(){
        GridType buffer = Alive;
        for (int posX = 0; posX < GridSize; posX++){
            for (int posY = 0; posY < GridSize; posY++){
                buffer = Rule3(buffer, posX, posY);
                buffer = Rule4(buffer, posX, posY);
            }
        }
        Alive = buffer;
    }

    // Celulas com 2 ou 3 sobrevivem (perpetuam).
    GridType Rule3(GridType buffer, int posX, int posY){
        if (Alive.at(posX).at(posY) == false) return buffer;
        if ((CheckSurrounds(posX, posY) == 2) || CheckSurrounds(posX, posY) == 3){
            return buffer;
        }
        else buffer.at(posX).at(posY) = false;
        return buffer;
    }

    // Celulas Vazias com EXATAMENTE 3 vizinhos, se tornam populadas.
    GridType Rule4(GridType buffer, int posX, int posY){
        if (Alive.at(posX).at(posY) == true) return buffer;
        if (CheckSurrounds(posX, posY) == 3) buffer.at(posX).at(posY) = true;
        return buffer;
    }    

    // Verifica as 8 grids ao redor da principal;
    int CheckSurrounds(int posX, int posY){
        
        GridType buffer = Alive;
        int AliveNeighbours = 0;

        // o = Main Point
        // . = Non-seeable Point
        // X = checked point

        // Grid layout example:
        // X | . | X
        // - - - - -
        // . | o | .
        // - - - - - 
        // X | . | X

        // SW = Southwest
        // . | . | .
        // - - - - -
        // . | o | .
        // - - - - - 
        // X | . | .
        if (((posX-1) < 0) || ((posY-1) < 0));
        else if (buffer.at(posX-1).at(posY-1) == true) AliveNeighbours++;
        
        // S = South
        // . | . | .
        // - - - - -
        // . | o | .
        // - - - - - 
        // . | X | .
        if ((posY-1) < 0);
        else if (buffer.at(posX).at(posY-1) == true) AliveNeighbours++;
        
        // SE = Southeast
        // . | . | .
        // - - - - -
        // . | o | .
        // - - - - - 
        // . | . | X
        if (((posX+1) >= GridSize) || ((posY-1) < 0));
        else if (buffer.at(posX+1).at(posY-1) == true) AliveNeighbours++;
        
        // W = West
        // . | . | .
        // - - - - -
        // X | o | .
        // - - - - - 
        // . | . | .
        if ((posX-1) < 0);
        else if (buffer.at(posX-1).at(posY) == true) AliveNeighbours++;
        
        // E = East
        // . | . | .
        // - - - - -
        // . | o | X
        // - - - - - 
        // . | . | .
        if ((posX+1) >= GridSize);
        else if (buffer.at(posX+1).at(posY) == true) AliveNeighbours++;
        
        // NW = Northwest
        // X | . | .
        // - - - - -
        // . | o | .
        // - - - - - 
        // . | . | .
        if (((posX-1) < 0) || ((posY+1) >= GridSize));
        else if (buffer.at(posX-1).at(posY+1) == true) AliveNeighbours++;
        
        // N = North
        // . | X | .
        // - - - - -
        // . | o | .
        // - - - - - 
        // . | . | .
        if ((posY+1) >= GridSize);
        else if (buffer.at(posX).at(posY+1) == true) AliveNeighbours++;
        
        // NE = Northeast
        // . | . | X
        // - - - - -
        // . | o | .
        // - - - - - 
        // . | . | .
        if (((posX+1) >= GridSize) || ((posY+1) >= GridSize));
        else if (buffer.at(posX+1).at(posY+1) == true) AliveNeighbours++;

        cout << "Possui vizinhos: " << AliveNeighbours << "\n";
        return AliveNeighbours;
    }

    void GridPrint(){
        for (int posX = 0; posX < GridSize; posX++){
            for (int posY = 0; posY < GridSize; posY++){
                if (Alive.at(posX).at(posY)) {
                    DrawRectangle((posX * CellSize), (posY * CellSize), CellSize, CellSize, BLACK);
                }
            }
        }
    }

    void Ressurection(pair<int,int> coords){
        if (coords.first >= GridSize) {
            cout << "A coordenada X e maior que os limites da grid" << "\n";
            return;
        }
        if (coords.second >= GridSize) {
            cout << "A coordenada Y e maior que os limites da grid" << "\n";
            return;
        }
        Alive.at(coords.first).at(coords.second) = !Alive.at(coords.first).at(coords.second);
    }
};

int main (){

    InitWindow(ScreenWidth, ScreenHeight, "Jogo da Vida");
    InitGame();
    
    return 0;
}

static void InitGame(void){

    Grid GridPadrao;
    SetTargetFPS(FPS);
    bool GameIsPaused = false;
    while (!WindowShouldClose()){
        // PROCESS EVENTS
        
        if (IsKeyPressed(KEY_SPACE)){
            GameIsPaused = !GameIsPaused;
        }

        if (IsKeyPressed(KEY_KP_7)){
            GridPadrao.Exec();
        }

    
        PositionType MousePos = RetornarPosMouse();
        if ((GameIsPaused) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))){
            GridPadrao.Ressurection(MousePos);
        }
        // UPDATE GAME LOGIC

        // RENDER
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGrid();
        if (GameIsPaused) DrawText("Game Is Paused", 90, 10, 20, BLACK);

        GridPadrao.GridPrint();

        DrawFPS(10, 10);
        EndDrawing();
    }
    // int choice;
    // int x,y;
    // while (1) {

    //     cout << "[0] - Exit" << "\n";
    //     cout << "[1] - Add/Remove Point" << "\n";
    //     cout << "[2] - Show Grid" << "\n";
    //     cout << "[3] - Check Surrounds" << "\n";
    //     cout << "[4] - Run One Step" << "\n";
    //     cin >> choice;
    //     switch (choice){
    //         case 0:
    //             return;
    //             break;

    //         case 1:
    //             cout << "type (X, Y)" << "\n";
    //             cin >> x >> y;
    //             GridPadrao.Ressurection({x,y});
    //             break;
            
    //         case 2:
    //             GridPadrao.GridPrint();
    //             break;

    //         case 3:
    //             cout << "type (X, Y)" << "\n";
    //             cin >> x >> y;
    //             GridPadrao.CheckSurrounds(x,y);
    //             break;

    //         case 4:
    //             GridPadrao.Exec();
    //             break;

    //         default:
    //             return;
    //     }        
    // }
}

static void UpdateGame(void);       // Atualiza um frame (um passo)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

static void DrawGrid(){
    for (int i = 0; i < GridSize; i++){
        DrawRectangle((i*CellSize), 0, 1, ScreenHeight, BLACK);
        DrawRectangle(0, (i*CellSize), ScreenWidth, 1, BLACK);
    }
}

PositionType RetornarPosMouse(){
    
    Vector2 MousePosition = GetMousePosition();
    int mouseX = (MousePosition.x / CellSize);
    int mouseY = (MousePosition.y / CellSize); 

    return make_pair(mouseX, mouseY);
}