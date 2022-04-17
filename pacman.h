//--------------GENERAL CONFIGURATION ELEMENTS---------------
using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define MAGENTA "\033[35m"      /* Magenta */

const int SIZE = 15;//Dimensione del labirinto (maze); non comprende i muri perimetrali
const std::string MAZE_CONFIG_NAME = "maze.cfg";//nome del file con i dati iniziali del labirinto
const int POINT = 10; //Punteggio puntino mangiato

//Definizione costanti - Caratteri usati per stampare il labirinto a video
const char EMPTY_C = ' ';//spazio vuoto
const char WALL_C = '#';//muro
const char DOT_C = '.';//puntino

//----LABIRINTH CONFIGURATION ELEMENTS-----
enum Labirinth_Elems { wall, empty, dot };

//----PACMAN ELEMENTS-----
enum PacmanDir { North, South, West, East };
struct PacMan
{
    PacmanDir direction;
    int X;
    int Y;
};

//----FRUITS ELEMENTS-----
const int FRUIT_POINT = 300;
const int FRUIT_MOVES = 7;
const int FRUITS = 5;
const char FRUIT_C = 'o';

struct Fruit
{
    int X;
    int Y;
    bool visible;
    int moves;
};

//----GHOSTS ELEMENTS-----
const int GHOSTS = 4;
const char GHOST_C = 'M';
const int GHOST_P = 25;
const int GHOST_POINT = 1000;
enum GhostDir { north, south, west, east };

struct Ghost
{
    GhostDir direction;
    bool visible;
    int X;
    int Y;
};

//------KILL GHOSTS STRUCT-----
struct kill
{
	bool eat;
	int turni;
};

//----COMMAND ELEMENTS-----
enum Command { Go_N, Go_S, Go_W, Go_E, Quit, Unknown };

Command get_command();
Command char2command(const char c);
PacmanDir cmd2dir(Command c);

//-----PACMAN FUNCTIONS-------
char PacmanDir2char(PacmanDir d);
void set_pacman(PacMan& pac_man, int x, int y, PacmanDir d);

//-----LABIRINTH FUNCTIONS-------
Labirinth_Elems char2lab_elem(char c);
char lab_elem2char(Labirinth_Elems e);
void init_maze(std::string config_file_name, Labirinth_Elems mm[SIZE][SIZE]);

//-----FRUIS FUNCTIONS------
void init_fruit(Labirinth_Elems M[SIZE][SIZE], Fruit F[], int x, int y);
bool displayFruit(Fruit F[], int i, int j, int z);
void set_fruits (Fruit F[], int x, int y, Labirinth_Elems M[SIZE][SIZE], int& score, kill& k);

//-----GHOSTS FUNCTIONS----
void init_ghost(Labirinth_Elems M[SIZE][SIZE], Ghost G[], int x, int y, int i);
bool displayGhost(Ghost G[], int i, int j, int z);
void set_ghosts(Ghost G[], int i, int x, int y, Labirinth_Elems M[SIZE][SIZE]);
void ghost_move(int& x, int& y, int i, Labirinth_Elems M[SIZE][SIZE], Ghost G[]);
void newDirection(Ghost G[], int i);

//-----GAME FUNCTIONS-------
void init(std::string config_file_name, Labirinth_Elems M[SIZE][SIZE], PacMan& pac_man, int &score, Fruit F[], Ghost G[]);
void display(Labirinth_Elems m[SIZE][SIZE], Fruit F[], Ghost G[], PacMan pac_man, int score, kill k);
bool make_move(PacMan& pac_man, Command c, Labirinth_Elems M[SIZE][SIZE], int &score, Fruit F[], Ghost G[], kill&);
bool acceptable(int x, int y, Labirinth_Elems M[SIZE][SIZE]);
void rand_coord(int &x, int&y, Labirinth_Elems M[SIZE][SIZE]);
