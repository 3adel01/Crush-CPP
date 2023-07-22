#include<iostream>
#include<cstdlib>
#include "classes.h"

using namespace std;

int main(){
	Ecran e; // Creation de l'objet e du classe Ecran
	cout << "Pour charger la derniere partie enregistrer, taper 1 : ";
	int y;
	cin >> y; //  1 : Charger la partie enregistrer
	bool replay = false;
	// on reboucle cette partie
	do{
		if (y != 1 || replay){
			e.remplir();  // Nouvelle partie
		}else{
			if (e.fileExiste())
				e.LoadGame(); // Charger la partie enregistrer
			else
				return 0;
		}
		system("clear"); // clear console
		e.affiche(); // Affichage de la grille
		e.afficher_score(); // Affichage du score et le nombre des mouvements restentes
		while (e.movDisp()) {
			cout << "Entrez les coordonnées du bonbon à déplacer (ligne colonne) (-1 -1) pour enregistrer et quiter: ";
			int i, j;
			string direction;
			cin >> i >> j ;
			if (i == -1 && j == -1){
				// Option de sauvgarde
				e.SaveGame();
				cout<<"Partie enregistrer, merci d'avoir joué ."<<endl;
				return 0;
			}
			
			if(e.jouer_tour(i,j,direction)){
				bool stable = true;
				do{
					// boucle de suppresion et animation
					stable = e.supprimer_bonbons(); 
					// Vérifie s'il y a des lignes ou des colonnes de trois bonbons ou plus de la même couleur et les supprime si nécessaire
					system("clear"); // clear console
					e.affiche(); // Affichage
					usleep(100000); // delai d'une seconde 
					e.gravite();
				}while(!stable);
				e.remplacer_bonbons();
				// Ajoute des bonbons aléatoires en haut de la grille pour remplacer ceux qui ont été supprimés
				system("clear"); // clear console
				e.affiche();
				e.afficher_score();
				cout<<"deplacement reussi."<<endl;
				}else{
					system("clear"); // clear console
					e.affiche();
					e.afficher_score();
					cout<<"deplacement impossible."<<endl;
				}
			
		}
		e.estGagnant(); // Affichage du resultat de la partie
		replay = e.newgame(); // affichage et demende d'initiation d'une nouvelle partie
		// Si la reponse est vrai on reboucle la boucle do-while
	}while(replay);
	return 0;
}
