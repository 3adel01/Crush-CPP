#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>
#include<cmath>
#include<ctime>
#include <unistd.h>

using namespace std;

const int l = 6;
const int c = 6;

class Personage{
	private:
		int tableau[l][c];
		int score;
		int Mouvements;
	public:
		bool movDisp(){
			// Routurn s'il y a des mouvements restantes
			if (Mouvements != 0)
				return true;
			else
				return false;
		}
		bool jouer_tour(int i, int j, string direction) {
			while (i >= l || j >= c || i < 0 || j < 0){
				// Verification de la validité des coordonnées
				// Si la/les valeurs entrées sont invalide redemander a nouveau
				cout << "Valeur invalide, Ligne ( entre 0 ~ " << l-1 << " ), Colone (entre 0 ~ " << c-1 <<" )"<<endl;
				cout << "Entrez les coordonnées du bonbon à déplacer (ligne colonne) : ";
				cin >> i >> j;
			}
			
			cout << "Entrez la direction (Haut - (H/h), Bas - (B/b), Gauche - (G/g), Droite - (D/d) : ";
			cin >> direction;
			while (!(direction == "H" || direction == "B" || direction == "G" || direction == "D" || direction == "h" || direction == "b" || direction == "g" || direction == "d")){
				// Verification de la validité de la direction
				// Si la direction est invalide redemander a nouveau
				cout << "Direction invalide, direction (H : haut, B: bas, G : gauche, D : droite) : ";
				cin >> direction;
			}
			if (deplacer(i, j, direction)) {
				Mouvements --;
				system("clear"); // Efface la sortie du console
				return true;
			}else{
				system("clear"); // Efface la sortie du console
				return false;
			}
		}
		
		bool deplacer(int i, int j, string direction) {
			// Vérifie si le déplacement est valide (c'est-à-dire s'il y a un autre bonbon à côté du bonbon à déplacer dans la direction choisie)
			int temp; // Variable temporaire pour ne pas ecraser des bonbons
			if ((direction == "H" || direction == "h") && i > 0 && tableau[i-1][j] != 0) {
				// Guarder la valeur de la casse prochaine dans la Variable temporaire
				temp = tableau[i-1][j];
				// Echangement de position
				tableau[i-1][j] = tableau[i][j];
				tableau[i][j] = temp;
				return true;
			}
			else if ((direction == "B" || direction == "b") && i < l-1 && tableau[i+1][j] != 0) {
				// Guarder la valeur de la casse prochaine dans la Variable temporaire
				temp = tableau[i+1][j]; 
				// Echangement de position
				tableau[i+1][j] = tableau[i][j]; 
				tableau[i][j] = temp;
				return true;
			}
			else if ((direction == "G" || direction == "g") && j > 0 && tableau[i][j-1] != 0) {
				// Guarder la valeur de la casse prochaine dans la Variable temporaire
				temp = tableau[i][j-1];
				// Echangement de position
				tableau[i][j-1] = tableau[i][j];
				tableau[i][j] = temp;
				return true;
			}
			else if ((direction == "D" || direction == "d") && j < c-1 && tableau[i][j+1] != 0) {
				// Guarder la valeur de la casse prochaine dans la Variable temporaire
				temp = tableau[i][j+1];
				// Echangement de position
				tableau[i][j+1] = tableau[i][j];
				tableau[i][j] = temp;
				return true;
			}
			return false;
		}
		bool supprimer_bonbons() {
			bool done = true;
			for (int i = 0; i < l; i++) {
				// Vérifie s'il y a trois bonbons ou plus de la même couleur consécutifs dans la ligne i
				for (int j = 0; j < c-2; j++) {
					if (abs(tableau[i][j]) == abs(tableau[i][j+1]) && abs(tableau[i][j+1]) == abs(tableau[i][j+2]) && abs(tableau[i][j]) != 0) {
						/*
						On invese la valeur des bonbons de la même couleur pour pouvoire encore comparer les 
						en valeur absolu dans la deuxieme verification (colonne par colonne)
						*/ 
						tableau[i][j] = -tableau[i][j];
						tableau[i][j+1] = -tableau[i][j+1];
						tableau[i][j+2] = -tableau[i][j+2];
						done = false;
					}
				}

				// Vérifie s'il y a trois bonbons ou plus de la même couleur consécutifs dans la colonne i
				for (int j = 0; j < l-2; j++) {
					if (abs(tableau[j][i]) == abs(tableau[j+1][i]) && abs(tableau[j+1][i]) == abs(tableau[j+2][i]) && abs(tableau[j][i]) != 0) {
						// On invese la valeur des bonbons adjacentes de la même couleur (3 ou plus)
						tableau[j][i] = -abs(tableau[j][i]);
						tableau[j+1][i] = -abs(tableau[j+1][i]);
						tableau[j+2][i] = -abs(tableau[j+2][i]);
						done = false;
					}
				}
			}
			// Supprime les bonbons en remplaçant par des espaces vides "0"
			for (int i = 0; i < l; i++)
				for (int j = 0; j < c; j++)
					if (tableau[j][i] < 0){
						tableau[j][i] = 0;
						score +=1; // Incrementation du score
					}
			return done;	
		}
		
		void gravite(){
			// Pour que emiminer les espaces vides au-dessus des bonbons
			for (int j = 0; j != c; j++){
				int idx = l-1; // initiation de l'index pour suivre la position du derniere valeurs non nulle (de bas en haut)
				for (int i = l-1 ; i != -1; i--){
					if (tableau[i][j] > 0){
						tableau[idx][j] = tableau[i][j];
						idx -=1;
					}
				}
				// Ecraser les position presedentes des bonbons (ombre)
				for (int i = idx; i != -1; i--)
					tableau[i][j] = 0;
			}
		}
		
		void remplacer_bonbons() {
			bool stable;
			do{
				// Pour avoir des resultats plus aleatroires a chaque fois qu'on relance le programme (en utilisant le variable de temps)
				srand(time(NULL)); 
				for (int i = 0; i != l; i++)
					for (int j = 0; j != c; j++)
						if (tableau[i][j] == 0)
							tableau[i][j] = rand() % 5 + 1; // Remplacer le vide par une valeur aleatoire de 1 a 5
				stable = supprimer_bonbons(); // Vérifie s'il y a des lignes ou des colonnes de trois bonbons ou plus de la même couleur et les supprime si nécessaire
				gravite(); // Ajoute des bonbons aléatoires en haut de la grille pour remplacer ceux qui ont été supprimés
			}while(!stable);
		}
		

	friend class Partie; // pour pouvoir y acceder au attributs prive depuis la classe Partie
	friend class Ecran; // pour pouvoir y acceder au attributs prive depuis la classe Ecran
};

class Partie : public Personage{
	// La classe Partie herite les methodes de la classe Personage
	public:
		void remplir(){
			srand(time(NULL)); // Pour avoir des resultats plus aleatroires a chaque fois qu'on relance le programme
			for (int i = 0; i!=l; i++){
				for (int j = 0; j!=c; j++){
					tableau[i][j] = rand() % 5 + 1; // Remplacer le vide par une valeur aleatoire de 1 a 5
				}
			}
			bool stable;
			do{
				stable = supprimer_bonbons(); // Vérifie s'il y a des lignes ou des colonnes de trois bonbons ou plus de la même couleur et les supprime si nécessaire
				gravite(); // Ajoute des bonbons aléatoires en haut de la grille pour remplacer ceux qui ont été supprimés
			}while(!stable);
			remplacer_bonbons();
			// Initialise le compteur du score
			score = 0;
			Mouvements = 15;
		}
		
		void LoadGame(){
			// Chargement des donnes du partie precedente depuis le fichier Text 
			fstream fin;
			fin.open("SaveData.txt", ios::in);
			for (int i = 0; i < l; i++)
				for (int j = 0; j < c; j++)
					fin >> tableau[i][j];
			fin >> score;
			fin >> Mouvements;
			fin.close();
		}
		
		void SaveGame(){
			// Sauvgarde des donnes de la partie courante dans le fichier Text 
			// Si le fichier n'existe pas en le cree
			fstream fout;
			fout.open("SaveData.txt", ios::out);
			for (int i = 0; i < l; i++)
				for (int j = 0; j < c; j++)
					fout << tableau[i][j]<<" ";
			fout <<" "<< score;
			fout <<"  "<< Mouvements;
			fout.close();
		}
		bool fileExiste(){
			// Tester si le fichier de sauvgarde existe
			ifstream file("SaveData.txt");
			if (!file.is_open()){
				cout<<"Fichier de sauvgarde non disponible ."<<endl;
				return false;
			}
			return true;
		}
};

class Ecran : public Partie{
	// La classe Ecran herite les methodes de la classe Partie
	public:
		Ecran(){
			system("clear");
			cout<<"Lancemet de la partie"<<endl;
		}
		
		void affiche(){
			//afficher la bordure de la premiere line en haut
			cout<<" ";
			for (int i = 0; i!=c ; i++){
				cout<<"   "<<i<<".";
			}
			cout<<endl;
			cout<<"  ╔"; // Corner gauche
			for (int i = 0; i!=c-1 ; i++){
				cout<<"════╦";
			}
			cout<<"════╗"<<endl; // Corner droite
			
			// Affichage des emojis
			for (int i = 0; i != l; i++){
				cout<<i<<".║"; // Bordure de la premiere colone
				for (int j = 0; j != c; j++){
					switch (tableau[i][j]){
						case 1:
							cout << " 🍬 "; // Affichage emoji pour tableau[i][j] = 1
							break;
						case 2:
							cout << " 🍪 "; // Affichage emoji pour tableau[i][j] = 2
							break;
						case 3:
							cout << " 🍭 "; // Affichage emoji pour tableau[i][j] = 3
							break;
						case 4:
							cout << " 🍩 "; // Affichage emoji pour tableau[i][j] = 4
							break;
						case 5:
							cout << " 🍫 "; // Affichage emoji pour tableau[i][j] = 5
							break;
						default:
							cout << "    "; // Affichage espace vide pour le reste "0"
					}
					cout<<"║"; // Separation entre emojis
				}
				cout << endl; // Revenir a la ligne 
				if (i!=l-1){
					// afficher la bordure des lignes au milieux
					cout<<"  ╠";
					for (int i = 0; i!=c-1; i++){
						cout<<"════╬";
					}
					cout<<"════╣"<<endl;
				}else{
					// afficher la bordure de la derniere line en base
					cout<<"  ╚"; // Corner gauche
					for (int i = 0; i!=c-1 ; i++){
						cout<<"════╩";
					}
					cout<<"════╝"<<endl; // Corner droite
				}
			}
		}
		
		void afficher_score(){
			// Affichage du score du joueur
			cout << "Score : " << score <<" ."<< endl;
			cout << "Mouvements  restants : " << Mouvements <<" ."<< endl;
		}
		
		void estGagnant(){
			// Pour afficher le resultats de la partie : Gagnat Score > 30
			if (score >= 30)
				cout<<"Partie gagnée - Score : " <<score<<" > 30 ."<<endl;
			else
				cout<<"Partie perdue - Score : " <<score<<" < 30 ."<<endl;
		}
		bool newgame(){
			score = 0; // Mise a zerp du compteur de score
			Mouvements = 15; // Reinitialisation du compteur de mouvements
			cout<<"Lancer une nouvelle partie : (Oui : 1, Non : 0) : "<<endl;
			int y;
			cin >> y;
			if (y == 1)
				return true;
			else
				return false;
		}
};
