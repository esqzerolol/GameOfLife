#include <bits/stdc++.h>
using namespace std;
typedef vector<vector<bool>> GridType;

struct Grid {
    GridType Alive;
    int GridSize;

    Grid (int n){
        GridSize = n;
        Alive.resize(n);
        for (int i = 0; i < n; i++) Alive.at(i).resize(n);
    }

    // Executa um passo
    void Exec(){
        GridType buffer = Alive;
        for (int posX = 0; posX < GridSize; posX++){
            for (int posY = 0; posY < GridSize; posY++){
                // buffer = Rule1(buffer, posX, posY);
                // buffer = Rule2(buffer, posX, posY);
                buffer = Rule3(buffer, posX, posY);
                buffer = Rule4(buffer, posX, posY);
            }
        }
        Alive = buffer;
    }

    // Celulas com menos de 2 celulas vizinhas morrem.
    // GridType Rule1(GridType buffer, int posX, int posY){
    //     if (Alive.at(posX).at(posY) == false) return buffer;
    //     if (CheckSurrounds(posX, posY) < 2) buffer.at(posX).at(posY) = false;
    //     return buffer;
    // }

    // // Celulas com mais de 3 celulas vizinhas morrem.
    // GridType Rule2(GridType buffer, int posX, int posY){
    //     if (Alive.at(posX).at(posY) == false) return buffer;
    //     if (CheckSurrounds(posX, posY) > 3) buffer.at(posX).at(posY) = false;
    //     return buffer;
    // }

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
        for (int i = 0; i < GridSize; i++){
            for (int j = 0; j < GridSize; j++){
                if (Alive.at(i).at(j) == false) cout << ". ";
                else cout << "x ";
            } cout << "\n";
        }
        cout << "\n";
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
        Alive.at(coords.first).at(coords.second) = true;
    }
};

int main (){

    Grid gridpadrao(10);
    int choice;
    int x,y;
    while (1) {

        cout << "[0] - Exit" << "\n";
        cout << "[1] - Add Point" << "\n";
        cout << "[2] - Show Grid" << "\n";
        cout << "[3] - Check Surrounds" << "\n";
        cout << "[4] - Run One Step" << "\n";
        cin >> choice;
        switch (choice){
            case 0:
                return 0;
                break;

            case 1:
                cout << "type (X, Y)" << "\n";
                cin >> x >> y;
                gridpadrao.Ressurection({x,y});
                break;
            
            case 2:
                gridpadrao.GridPrint();
                break;

            case 3:
                cout << "type (X, Y)" << "\n";
                cin >> x >> y;
                gridpadrao.CheckSurrounds(x,y);
                break;

            case 4:
                gridpadrao.Exec();
                break;

            default:
                return 0;
        }        
    }
    return 0;
}