#ifndef POULE_H_INCLUDED
#define POULE_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <sstream>
#include <windows.h>

#define HIT 0
#define MISS 1
#define CRIT 2
#define BLOCKED 3

class Poule {
public :
    /* Le d�tail des m�thodes est donn� dans le fichier Poule.cpp */
    Poule(int numeroJoueur = 0);

    int lancerOeuf(Poule &poule);
    int gratterTerre(Poule &poule);

    void finTour();

    int getVie();
    int getDegatsDerniereAttaque();
    sf::Sprite getBarreVie();
    sf::Sprite getSprite();

    void reinitialiser();


protected :
    /* Ces trois m�thodes sont prot�g�es car les diff�rentes instance de la classe Poule vont s'en servir,
        mais elles n'ont pas besoin d'�tre accessible "� l'ext�rieur" */
    void setFaitFace(bool face);
    void setPrecision(int modificateur);
    void recevoirDegats(int degatsRecus);

    /* Les noms sont ici explicites */
    int vie;

    int degatsAttaqueOeuf;
    int precisionAttaqueOeuf;
    int blocageOeuf;

    int degatsAttaqueTerre;
    int precisionAttaqueTerre;
    int malusPrecisionAttaqueTerre;

    int critique;
    int multiplicateurCritique;

    /* A ne pas confondre avec malusPrecisionAttaqueTerre, ici malusPrecision est le d�buff actif sur le joueur */
    int malusPrecision;

    /* Utilis� pour interagir avec l'affichage dynamique des d�g�ts (qui sont d�clar�s dans le main) */
    int degatsDerniereAttaque;

    /* Utilis� pour d�terminer quelle position du joueur on affiche (dos ou face au joueur d'en face) */
    bool faitFace;

    sf::Texture image;
    sf::Sprite sprite;

    sf::Texture imageBarreDeVie;
    sf::Sprite barreDeVie;
};

#endif // POULE_H_INCLUDED
