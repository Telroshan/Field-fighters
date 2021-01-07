#include "Poule.h"

using namespace std;
using namespace sf;


/* L'entier numeroJoueur demand� en param�tre est simplement utilis� pour d�terminer quelle image du joueur on affiche */
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

    /* On r�initialise le stringstream (on s'en sert � nouveau, mais pour charger l'image de la barre de vie cette fois) */
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

    /* On tire les d�s pour savoir si le joueur touche / fait un coup critique / se fait bloquer */
    bool crit = (rand()%100) < critique;
    bool hit = (rand()%100) < (precisionAttaqueOeuf - malusPrecision);
    bool block = (rand()%100) < blocageOeuf;

    /* Si le joueur touche mais ne fait pas de coup critique ... */
    if(hit && !block && !crit) {
        poule.recevoirDegats(degatsAttaqueOeuf);
        degatsDerniereAttaque = degatsAttaqueOeuf;
        /* Ces constantes sont d�finies dans Poule.h, elles sont utilis�es dans le main pour choisir le texte dynamique � afficher selon le code de retour de la fonction */
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


/* L'attaque "gratter terre" qui inflige moins de d�g�ts que "lancer oeuf", mais profite d'une pr�cision plus �lev�e, ne peut pas �tre bloqu�e et applique un malus de pr�cision sur l'adversaire */
int Poule::gratterTerre(Poule &poule) {
    /* La poule attaquante se retourne (elle gratte la terre vers l'arri�re) */
    setFaitFace(false);

    /* On tire les d�s pour savoir si le joueur touche et fait un coup critique */
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


/* Une fois le tour du joueur fini, on r�initialise ses (d�)buffs et on le remet de face � son adversaire */
void Poule::finTour() {
    malusPrecision = 0;
    setFaitFace(true);
}


/* M�thode accesseur, renvoie la vie actuelle du joueur */
int Poule::getVie() {
    return vie;
}


/* M�thode accesseur, renvoie les d�g�ts de la derni�re attaque effectu�e par le joueur (utilis� pour l'affichage dynamique des d�g�ts) */
int Poule::getDegatsDerniereAttaque() {
    return degatsDerniereAttaque;
}


/* M�thode accesseur, renvoie le Sprite de la barre de vie, r�cup�r� dans le main pour l'affichage */
Sprite Poule::getBarreVie() {
    return barreDeVie;
}

/* M�thode accesseur, renvoie le Sprite du joueur, r�cup�r� dans le main pour l'affichage */
Sprite Poule::getSprite() {
    return sprite;
}

/* M�thode appel�e lors de l'appui sur la touche Entr�e, remet les valeurs par d�faut des attributs du joueur */
void Poule::reinitialiser() {
    vie = 100;
    malusPrecision = 0;
    degatsDerniereAttaque = 0;
    barreDeVie.setTextureRect(IntRect(0, 0, 14, 226));
    barreDeVie.setPosition(Vector2f(barreDeVie.getPosition().x, 440));
    setFaitFace(true);
}



/* M�thode priv�e, appel�e par les m�thodes d'attaques ou lorsque la poule bloque l'oeuf adverse */
void Poule::setFaitFace(bool face) {
    faitFace = face;
    IntRect newRect = IntRect(213*faitFace, 0, 213, 209);
    sprite.setTextureRect(newRect);
}


/* M�thode �galement priv�e, pour appliquer le malus de pr�cision de l'attaque "gratter terre" */
void Poule::setPrecision(int malus) {
    malusPrecision = malus;
}


/* M�thode priv�e, elle est appel�e par l'autre instance de Poule (attaquante) */
void Poule::recevoirDegats(int degatsRecus) {
    /* On v�rifie que la vie ne descend pas en dessous des 0 PV */
    vie -= degatsRecus;
    if(vie < 0)
        vie = 0;

    /* Et on met � jour l'affichage de la barre de vie ... */
    float pourcentageViePerdue = (100 - (float)vie)/100;
    /* ... On affiche un pourcentage de l'image, en fonction du pourcentage de PV actuels */
    barreDeVie.setTextureRect(IntRect(0, (int)(226*pourcentageViePerdue), 14, (int)(226 - 226*pourcentageViePerdue)));
    barreDeVie.setPosition(Vector2f(barreDeVie.getPosition().x, 440 + (int)(226*pourcentageViePerdue)));
}
