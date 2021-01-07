#include "Poule.h"

using namespace std;
using namespace sf;


/* L'entier numeroJoueur demandé en paramètre est simplement utilisé pour déterminer quelle image du joueur on affiche */
Poule::Poule(int numeroJoueur) {

    /* Initialisation des attributs de la Poule */
    vie = 100;

    degatsAttaqueOeuf = 15;
    precisionAttaqueOeuf = 70;
    blocageOeuf = 10;

    degatsAttaqueTerre = 10;
    precisionAttaqueTerre = 90;
    malusPrecisionAttaqueTerre = 35;

    critique = 25;
    multiplicateurCritique = 2;

    malusPrecision = 0;

    degatsDerniereAttaque = 0;

    faitFace = true;

    /* On automatise la recherche du fichier d'image selon numeroJoueur (numerojoueur = 0 <=> joueur 1 -> joueur1.png, numeroJoueur = 1 <=> joueur 2 -> joueur2.png) */
    stringstream path;
    path << "images/poule" << numeroJoueur+1 << ".png";
    image.loadFromFile(path.str());

    sprite.setTexture(image);
    IntRect rectSprite = IntRect(213, 0, 213, 209);
    sprite.setTextureRect(rectSprite);
    sprite.setPosition(Vector2f(17 + numeroJoueur*753, 433));

    /* On réinitialise le stringstream (on s'en sert à nouveau, mais pour charger l'image de la barre de vie cette fois) */
    path.str("");
    path << "images/barreDeVie" << numeroJoueur+1 << ".png";
    imageBarreDeVie.loadFromFile(path.str());

    barreDeVie.setTexture(imageBarreDeVie);
    barreDeVie.setPosition(Vector2f(1 + 984*numeroJoueur, 440));
}


/* L'attaque "lancer oeuf" */
int Poule::lancerOeuf(Poule &poule) {
    /* La poule attaquante se retourne pour "lancer" son oeuf */
    setFaitFace(false);

    /* On tire les dés pour savoir si le joueur touche / fait un coup critique / se fait bloquer */
    bool crit = (rand()%100) < critique;
    bool hit = (rand()%100) < (precisionAttaqueOeuf - malusPrecision);
    bool block = (rand()%100) < blocageOeuf;

    /* Si le joueur touche mais ne fait pas de coup critique ... */
    if(hit && !block && !crit) {
        poule.recevoirDegats(degatsAttaqueOeuf);
        degatsDerniereAttaque = degatsAttaqueOeuf;
        /* Ces constantes sont définies dans Poule.h, elles sont utilisées dans le main pour choisir le texte dynamique à afficher selon le code de retour de la fonction */
        return HIT;
    }

    /* ... Si le joueur touche et fait un coup critique ... */
    else if(hit && !block && crit) {
        poule.recevoirDegats(degatsAttaqueOeuf * multiplicateurCritique);
        degatsDerniereAttaque = degatsAttaqueOeuf * multiplicateurCritique;
        return CRIT;
    }

    /* ... Si le joueur se fait bloquer ... */
    else if(block){
        degatsDerniereAttaque = 0;
        poule.setFaitFace(false);
        return BLOCKED;
    }

    /* ... Ou s'il rate son attaque */
    else {
        degatsDerniereAttaque = 0;
        return MISS;
    }
}


/* L'attaque "gratter terre" qui inflige moins de dégâts que "lancer oeuf", mais profite d'une précision plus élevée, ne peut pas être bloquée et applique un malus de précision sur l'adversaire */
int Poule::gratterTerre(Poule &poule) {
    /* La poule attaquante se retourne (elle gratte la terre vers l'arrière) */
    setFaitFace(false);

    /* On tire les dés pour savoir si le joueur touche et fait un coup critique */
    bool crit = (rand()%100) < critique;
    bool hit = (rand()%100) < (precisionAttaqueTerre - malusPrecision);

    /* S'il touche mais ne fait pas de coup critique ... */
    if(hit && !crit) {
        poule.recevoirDegats(degatsAttaqueTerre);
        poule.setPrecision(malusPrecisionAttaqueTerre);
        degatsDerniereAttaque = degatsAttaqueTerre;
        return HIT;
    }

    /* ... S'il touche et fait un coup critique ... */
    else if(hit && crit) {
        poule.recevoirDegats(degatsAttaqueTerre * multiplicateurCritique);
        poule.setPrecision(malusPrecisionAttaqueTerre);
        degatsDerniereAttaque = degatsAttaqueTerre * multiplicateurCritique;
        return CRIT;
    }

    /* ... Ou s'il rate */
    else {
        degatsDerniereAttaque = 0;
        return MISS;
    }
}


/* Une fois le tour du joueur fini, on réinitialise ses (dé)buffs et on le remet de face à son adversaire */
void Poule::finTour() {
    malusPrecision = 0;
    setFaitFace(true);
}


/* Méthode accesseur, renvoie la vie actuelle du joueur */
int Poule::getVie() {
    return vie;
}


/* Méthode accesseur, renvoie les dégâts de la dernière attaque effectuée par le joueur (utilisé pour l'affichage dynamique des dégâts) */
int Poule::getDegatsDerniereAttaque() {
    return degatsDerniereAttaque;
}


/* Méthode accesseur, renvoie le Sprite de la barre de vie, récupéré dans le main pour l'affichage */
Sprite Poule::getBarreVie() {
    return barreDeVie;
}

/* Méthode accesseur, renvoie le Sprite du joueur, récupéré dans le main pour l'affichage */
Sprite Poule::getSprite() {
    return sprite;
}

/* Méthode appelée lors de l'appui sur la touche Entrée, remet les valeurs par défaut des attributs du joueur */
void Poule::reinitialiser() {
    vie = 100;
    malusPrecision = 0;
    degatsDerniereAttaque = 0;
    barreDeVie.setTextureRect(IntRect(0, 0, 14, 226));
    barreDeVie.setPosition(Vector2f(barreDeVie.getPosition().x, 440));
    setFaitFace(true);
}



/* Méthode privée, appelée par les méthodes d'attaques ou lorsque la poule bloque l'oeuf adverse */
void Poule::setFaitFace(bool face) {
    faitFace = face;
    IntRect newRect = IntRect(213*faitFace, 0, 213, 209);
    sprite.setTextureRect(newRect);
}


/* Méthode également privée, pour appliquer le malus de précision de l'attaque "gratter terre" */
void Poule::setPrecision(int malus) {
    malusPrecision = malus;
}


/* Méthode privée, elle est appelée par l'autre instance de Poule (attaquante) */
void Poule::recevoirDegats(int degatsRecus) {
    /* On vérifie que la vie ne descend pas en dessous des 0 PV */
    vie -= degatsRecus;
    if(vie < 0)
        vie = 0;

    /* Et on met à jour l'affichage de la barre de vie ... */
    float pourcentageViePerdue = (100 - (float)vie)/100;
    /* ... On affiche un pourcentage de l'image, en fonction du pourcentage de PV actuels */
    barreDeVie.setTextureRect(IntRect(0, (int)(226*pourcentageViePerdue), 14, (int)(226 - 226*pourcentageViePerdue)));
    barreDeVie.setPosition(Vector2f(barreDeVie.getPosition().x, 440 + (int)(226*pourcentageViePerdue)));
}
