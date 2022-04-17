#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "pacman.h"

int main()
{
	Labirinth_Elems maze[SIZE][SIZE];
	PacMan p;
	int score;

	kill k;
	k.eat=false;
	k.turni=0;

	Fruit F[FRUITS];
	Ghost G[GHOSTS];

	//Stampo messaggio iniziale
	cout << YELLOW << ">"<< RED << " PAC MAN BASIC "<< YELLOW <<"<" << RESET << endl;

	//Provo inizializzazione del labirinto, se fallisce raccolgo messaggio d'errore
	try
	{
		init(MAZE_CONFIG_NAME, maze, p, score, F, G);
	}

	catch (string& e)
	{
		cout << e;
		return -1;
	}

	Command cmd = Unknown;

	//Ripeto per ogni mossa
	do
	{
		//Stampo
		display(maze, F, G, p, score, k);

		//Registro comando dell'utente
		cmd = get_command();

		//Provo ad analizzare comando ed effettuare la mossa
		try
		{
			switch (cmd)
			{
				 case Quit:
						cout << "Grazie di aver giocato con me, alla prossima!" << endl;
						return 0;
				 case Unknown:
						throw "unexpected command";
				 case Go_E:
				 case Go_W:
				 case Go_S:
				 case Go_N:
						if (!make_move(p, cmd, maze, score, F, G, k))
							 cout << "Mossa impossibile: non puoi passare attraverso i muri!" << endl;
						break;
				 default: throw "not a command!";
			 }
		}

		//Registo eventuali errori
		catch(int errore)
		{
			if(errore==-1)
			{
				cout<<"Sei stato mangiato da un fantasma. Hai perso :("<<endl;
				cout << "Grazie di aver giocato con me, alla prossima!" << endl;
				return 0;
			}
		}

		catch (string& m)
		{
			cout << m;
			return 0;
		}

	}
	while (true);
}
