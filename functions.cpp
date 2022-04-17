//------------------PROGRAM FUNCTIONS--------------------

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "pacman.h"

//Data una costante char del labirinto la converte nel rispettivo Labirinth_Elem
Labirinth_Elems char2lab_elem(char c)
{
    switch (c)
    {
      case WALL_C:
        return  wall;
      case EMPTY_C:
        return empty;
      case DOT_C:
        return dot;
      default:
        return empty;
    }
}

//Dato un labirinth elem lo converte nel rispettivo carattere char
char lab_elem2char(Labirinth_Elems e)
{
    switch (e)
    {
      case wall:
          return  WALL_C;
      case empty:
          return EMPTY_C;
      case dot:
          return DOT_C;
      default: throw "not a valid labirinth element";
    }
}

//Converte l'input da tastiera in un comando
Command char2command(const char c)
{
    switch (c) {
       case 'w':
	       case'W':
	         return Go_N;
           break;
       case 'a':
	       case'A':
	         return Go_W;
           break;
       case 'd':
	       case'D':
	         return Go_E;
           break;
       case 's':
	       case'S':
	         return Go_S;
           break;
       case 'q':
	       case'Q':
	         return Quit;
           break;
       default:
	         return Unknown;
    }
}

//Converte una direzione nel corrispondente carattere pacman da stampare a schermo
char PacmanDir2char(PacmanDir d)
{
    switch (d)
    {
       case South: return '^';
       case East:return '<';
       case West:return '>';
       case North:return 'v';
       default: throw "not a valid direction";
    }
}

//Verifica se un frutto è presente e visibile in una certa posizione
bool displayFruit(Fruit F[], int i, int j, int z)
{
      if (z==FRUITS)
        return false;
      if (i==F[z].X && j==F[z].Y && F[z].visible)
        return true;
      else
        return displayFruit(F, i, j, z+1);
}

//Funzione che include file di configurazione del labirinto
void init_maze(string config_file_name, Labirinth_Elems mm[SIZE][SIZE])
{
    ifstream cfgfile;
    cfgfile.open(config_file_name);
    if (!cfgfile)
        throw "file not found: missing " + config_file_name;

    for (int y = 0; y<SIZE; ++y)
    {
        std::string line;
        if (!getline(cfgfile, line))
            line = "";
        std::istringstream line_stream(line);

        for (int x = 0; x < SIZE; ++x)
        {
            char c;
            if (line_stream >> c)
                mm[x][y] = char2lab_elem(c);
            else
                mm[x][y] = Labirinth_Elems::empty;//missing input is integrated by empty spaces
        }
    }
    cfgfile.close();
}

//Funzione set_pacman - assegna i valori ai campi della struct Pacman
void set_pacman(PacMan& pac_man, int x, int y, PacmanDir d)
{
	pac_man.X = x;
	pac_man.Y = y;
	pac_man.direction = d;
}

void init_fruit(Labirinth_Elems M[SIZE][SIZE], Fruit F[], int x, int y, int i)
{
	rand_coord(x, y, M);
		F[i].X = x;
		F[i].Y = y;
		F[i].visible = (rand()%2);
		F[i].moves = FRUIT_MOVES;
}

//Funzione init - Inizializza labirinto e pacman
void init(std::string config_file_name, Labirinth_Elems M[SIZE][SIZE], PacMan& pac_man, int &score, Fruit F[], Ghost G[])
{
	init_maze(config_file_name, M); //configurazione struttura labirinto

	//configurazione posizione casuale pacman
	srand(time(0));
	int x, y;
	rand_coord(x, y, M);
	set_pacman(pac_man, x, y, South);

	//configurazione frutti
	for (int i=0; i<FRUITS; i++)
	{
		init_fruit(M, F, x, y, i);
	}

	//Configurazione fantasmi
	for (int i=0; i<GHOSTS; i++)
	{
	  init_ghost(M, G, x, y, i);
	}

	//inizializzazione puneggio
	score= 0;
}

//Funzione set_fruits- Aggiorna lo stato dei frutti
void set_fruits(Fruit F[], int x, int y, Labirinth_Elems M[SIZE][SIZE], int& score, kill& k)
{
  for (int i=0; i< FRUITS; i++)
  {
    //Decremento numero mosse del frutto
    F[i].moves-=1;

    //Se pacman raggiunge un frutto
    if (x==F[i].X && y==F[i].Y && F[i].visible)
    {
      score+= FRUIT_POINT;
      F[i].visible= false;
      M[x][y]= empty;
      k.eat=true;
      k.turni=0;
    }

    //Se il numero di mosse è terminato e il frutto è ancora visibile, scompare
    if (F[i].moves==0 && F[i].visible)
    {
      F[i].visible= false;
      M[x][y]= empty;
      F[i].moves= FRUIT_MOVES;
    }

    //Controllo frutti non visibili
    if (F[i].moves==0 && !F[i].visible)
    {
      init_fruit(M, F, x, y, i);
    }

    //Aggiorno elementi labirinto
    lab_elem2char(M[x][y]);
  }
}

//Funzione display- stampa lo stato corrente del labirinto
void display(Labirinth_Elems m[SIZE][SIZE], Fruit F[], Ghost G[], PacMan pac_man, int score, kill k)
{
  //Stampa il punteggio e un messaggio che annuncia la possibilità di mangiare un fantasma
	cout<< "Score: " << score << endl;
	if(k.eat)
		cout<< RED << "EAT THE GHOSTS!"<< RESET << endl;

  //Stampa muri perimetro superiore labirinto
	for (int i=0; i< SIZE+2; i++)
		cout << BLUE << WALL_C << RESET;
	cout<< endl;

  //Stampa elementi labirinto ed elementi gioco
	for (int i=0; i< SIZE; i++)
	{
		cout << BLUE << WALL_C << RESET;
		for (int j=0, z=0; j< SIZE; j++)
		{
      //Stampa Pacman
		  if (i==pac_man.X && j==pac_man.Y)
			   cout<< YELLOW << PacmanDir2char(pac_man.direction)<< RESET;

      //Stampa fantasmi
		  else if (displayGhost(G, i, j, z)&& !k.eat)
			   cout<< RED << GHOST_C << RESET;
		  else if(displayGhost(G, i, j, z)&& k.eat)
			   cout<< MAGENTA << GHOST_C << RESET;

      //Stampa frutta
		  else if (displayFruit(F, i, j, z))
			   cout<< GREEN << FRUIT_C << RESET;

      //Stampa elementi labirinto
		  else if(lab_elem2char(m[i][j])==WALL_C)
			   cout<< BLUE << lab_elem2char(m[i][j]) << RESET;
		  else
			   cout<< lab_elem2char(m[i][j]);
		}
		cout << BLUE << WALL_C << RESET <<endl;
	}

  //Stampa perimetro inferiore labirinto
	for (int i=0; i< SIZE+2; i++)
		cout << BLUE << WALL_C << RESET;
	cout<< endl;
}

//Funzione get_command - chiede all'utente di inserire un comando finche' l'utente non ne inserisce uno noto, poi lo restituisce
Command get_command()
{
	char my_char;
	Command my_cmd;

  //Richiedo un comando finchè non ne ottengo uno conosciuto
	do
	{
	 cout<< "Inserisci un comando (W/w/S/s/A/a/D/d/Q/q): ";
	 cin>> my_char;
	 my_cmd= char2command(my_char);
	}
	while (my_cmd== Unknown);

	return my_cmd;
}

//Funzione cmd2dir - converte il comando nella direzione che pac_man deve assumere
PacmanDir cmd2dir(Command c)
{
	switch(c)
	{
		case Go_N:
		   return North;
		case Go_S:
		   return South;
		case Go_E:
		   return East;
		case Go_W:
		   return West;
		case Quit:
		   throw "there is no direction for quitting";
		case Unknown:
		   throw "I don't know where to go";
	}
}

//Dato lo stato del gioco verifica se la mossa e' ammissibile
//In caso positivo aggiorna la posizione di pac_man
bool make_move(PacMan& pac_man, Command c, Labirinth_Elems M[SIZE][SIZE], int &score, Fruit F[],Ghost G[], kill& k)
{
  int x= pac_man.X;
	int y= pac_man.Y;
	int z, w;

	switch (c)
	{
	   case Quit:
	      throw "no move";
	   case Unknown:
	      throw "no move";
	   case Go_S:
		    x++;
        break;
	   case Go_N:
		    x--;
        break;
	   case Go_E:
		    y++;
        break;
	   case Go_W:
		    y--;
        break;
	}

  //Controlla che la nuova posizione di pacman non coincida con i muri
	if (x<0 || x>=SIZE || y<0 || y>=SIZE)
		return false;
	if(M[x][y]== wall)
		return false;

  //Imposto nuova posizione pacman
  set_pacman(pac_man, x, y, cmd2dir(c));

  //Imposto nuove posizioni fantasmi e controllo se ci sono dei morti
  for (int i=0; i<GHOSTS; i++)
	{
		z= G[i].X;
		w= G[i].Y;
    set_ghosts(G, i, z, w, M);

    //se la posizione di pacman e quella di un fantasma in vita coincidono
	  if (((G[i].X == pac_man.X && G[i].Y == pac_man.Y) || (z == pac_man.X && w == pac_man.Y)) && G[i].visible)
	  {
        //Se pacman può uccidere il fantasma, quest'ultimo muore e aumenta il puinteggio
		    if(k.eat)
        {
          score+=GHOST_POINT;
          G[i].visible= false;
        }

        //Altrimenti muore pacman e termina il gioco
		    else
			     throw -1;
	  }
	}

  //Imposto nuovo stato frutti
	set_fruits(F, x, y, M, score, k);

  //controllo se pacman è sovrapposto ad un punto
	if (M[x][y]== dot)
	{
		score+= POINT;
		M[x][y]= empty;
		lab_elem2char(M[x][y]);
	}

  //conteggio turni per la possibilità per pac man di mangiare i fantasmini
	if(k.eat)
	{
		if(k.turni==4)
		  k.eat=!k.eat;
		else
			k.turni++;
	}
	return true;
}

//Verifica se una posizione nel labirinto è accettabile (non coincide con muri e non esce dal labirinto)
bool acceptable(int x, int y, Labirinth_Elems M[SIZE][SIZE])
{
	if (M[x][y]==wall)
		return false;
	if (x<0 || x>=SIZE || y<0 || y>=SIZE)
		return false;
	if (x>0 && M[x-1][y]!= wall)
		return true;
	if (y>0 && M[x][y-1]!= wall)
		return true;
	if (x< SIZE-1 && M[x+1][y]!= wall)
		return true;
	if (y< SIZE-1 && M[x][y+1]!=wall)
		return true;
	return false;
}

//Genera coordinate casuali accettabili
void rand_coord(int &x, int&y, Labirinth_Elems M[SIZE][SIZE])
{
	do
	{
		x=(rand()%SIZE);
		y=(rand()%SIZE);
	}
	while (!acceptable(x, y, M));
}

//Inizializza i fantasmi ad inizio partita in posizioni casuali
void init_ghost(Labirinth_Elems M[SIZE][SIZE], Ghost G[], int x, int y, int i)
{
	rand_coord(x, y, M);
	G[i].X = x;
	G[i].Y = y;
	G[i].direction= static_cast<GhostDir>(rand()%4);
  G[i].visible= true;
}

//Verifica se un fantasma è visibile a schermo
bool displayGhost(Ghost G[], int i, int j, int z)
{
	if (z==GHOSTS)
        return false;
	if (i==G[z].X && j==G[z].Y && G[z].visible)
			return true;
	else
        return displayGhost(G, i, j, z+1);
}

//Genera una direzione diversa dalla precedente per un fantasma
void newDirection(Ghost G[], int i)
{
  GhostDir newdir;
  do
  {
    newdir=static_cast<GhostDir>(rand()%4);
  }
  while (newdir==G[i].direction);

  G[i].direction= newdir;
}

//Reimposta lo stato del fantasma ad ogni mossa
void set_ghosts(Ghost G[], int i, int x, int y, Labirinth_Elems M[SIZE][SIZE])
{
	//Con probabilità del 25% genero una nuova direzione per il fantasma
	int prob=rand()%100;
  if (prob<GHOST_P)
    newDirection(G, i);

  //In base alla direzione sposto il fantasma in una posizione accettabile
	ghost_move(x, y, i, M, G);

  //Assegno al fantsama le nuove coordinate
  G[i].X = x;
  G[i].Y = y;
}

//In base alla direzione sposta il fantasma in una posizione accettabile
void ghost_move(int& x, int& y, int i, Labirinth_Elems M[SIZE][SIZE], Ghost G[])
{
    if (G[i].direction== north && acceptable(x-1, y, M))
      x--;
    else if (G[i].direction== south && acceptable(x+1, y, M))
      x++;
    else if (G[i].direction== east && acceptable(x, y+1, M))
      y++;
    else if (G[i].direction== west && acceptable(x, y-1, M))
      y--;
    else
    {
      newDirection(G, i);
      ghost_move(x, y, i, M, G);
    }
}
