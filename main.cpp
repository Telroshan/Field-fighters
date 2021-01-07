#include "Poule.h"

/** Chicken fight est un jeu � deux joueurs de combat en tour par tour **/

using namespace std;
using namespace sf;


int main()
{
    /* Initialisation des nombres al�atoires (utilis�s pour les jets de d�s de critique/pr�cision/blocage) */
    srand(time(0));


    int nombreJoueurs = 2;
    /* Cr�ation des personnages */
    Poule joueurs[] = {Poule(0), Poule(1)};


    /* Utilis� pour compter et afficher le nombre de tours �coul�s durant le combat */
    int tourActuel = 1;


    /* Utilis� pour d�terminer quel joueur joue actuellement son tour */
    int tourJoueur = 0;


    /* Ne vaudra faux que lorsque le combat sera termin� (-> l'utilisateur peut appuyer sur entr�e pour lancer un nouveau combat, ce qui remet inGame � true) */
    bool inGame = true;


    /* Cr�ation de la fen�tre (-> Librairie SFML) */
    RenderWindow app;
    app.create(VideoMode(1000, 667), "Chicken fight", Style::Titlebar | Style::Close);
    app.setFramerateLimit(100);
    app.setKeyRepeatEnabled(false);


    /* Cr�ation de l'image de fond (-> Librairie SFML)
    Dans un premier temps on charge l'image dans un objet Texture (non affichable) */
    Texture textureFond;
    textureFond.loadFromFile("images/fond.png");
    /* Puis on applique la texture charg�e sur un Sprite (-> SFML) qui sera quant � lui affichable */
    Sprite fond;
    fond.setTexture(textureFond);
    fond.setPosition(Vector2f(0, 0));


    /* Chargement de la "police d'�criture" utilis�e (on charge en fait des images qu'on affichera selon le texte � afficher) */
    /* Les 10 chiffres ici (Utilis�s pour afficher le nombre de tours �coul�s, les d�g�ts inflig�s et le num�ro du vainqueur) */
    Texture imageChiffres;
    imageChiffres.loadFromFile("font/chiffres.png");
    /* On choisit quelle partie de la texture on applique sur le Sprite, plut�t que de cr�er un sprite par chiffre, on cr�e un
        sprite par emplacement de texte et on modifiera la partie de texture qui s'y appliquera */
    IntRect rectsChiffres[10];
    for(int chiffre=0; chiffre < 10; ++ chiffre) {
        rectsChiffres[chiffre] = IntRect(58*chiffre, 0, 58, 60);
    }
    /* Les deux sprites utilis�s pour compter le nombre de tours �coul�s (1 sprite / chiffre -> 2 sprites, �tant impossible de d�passer les 100 tours) */
    Sprite spritesTourActuel[2];
    for(int sprite=0; sprite < 2; ++ sprite) {
        spritesTourActuel[sprite].setTexture(imageChiffres);
        spritesTourActuel[sprite].setTextureRect(rectsChiffres[0 + 1*sprite]);
        spritesTourActuel[sprite].setPosition(Vector2f(559 + 42*sprite, 158));
    }


    /* Cr�ation de l'image du bouton d'attaque "lancer oeuf" */
    Texture imageAttaqueOeuf;
    imageAttaqueOeuf.loadFromFile("images/attaqueOeuf.png");
    /* Le bouton a deux �tats ; "normal" et au survol de la souris (-> On divise donc la texture en 2 rectangles) */
    IntRect rectBoutonAttaqueOeuf[2];
    for(int etatBouton=0; etatBouton < 2; ++etatBouton)
        rectBoutonAttaqueOeuf[etatBouton] = IntRect(147*etatBouton, 0, 147, 148);
    Sprite boutonAttaqueOeuf;
    boutonAttaqueOeuf.setTexture(imageAttaqueOeuf);
    boutonAttaqueOeuf.setTextureRect(rectBoutonAttaqueOeuf[0]);
    boutonAttaqueOeuf.setPosition(Vector2f(333, 247));
    /* Les coordonn�es du centre et le rayon du bouton seront utilis�s pour d�terminer si oui ou non le curseur se trouve sur le bouton */
    Vector2f centreBoutonAttaqueOeuf = Vector2f(404, 318);
    int rayonBoutonAttaqueOeuf = 69;


    /* M�me principe pour l'attaque "gratter terre" */
    Texture imageAttaqueTerre;
    imageAttaqueTerre.loadFromFile("images/attaqueTerre.png");
    IntRect rectBoutonAttaqueTerre[2];
    for(int etatBouton=0; etatBouton < 2; ++etatBouton)
        rectBoutonAttaqueTerre[etatBouton] = IntRect(147*etatBouton, 0, 147, 148);
    Sprite boutonAttaqueTerre;
    boutonAttaqueTerre.setTexture(imageAttaqueTerre);
    boutonAttaqueTerre.setTextureRect(rectBoutonAttaqueTerre[0]);
    boutonAttaqueTerre.setPosition(Vector2f(519, 247));
    Vector2f centreBoutonAttaqueTerre = Vector2f(589, 318);
    int rayonBoutonAttaqueTerre = 69;


    /* Le texte qui sera affich� en cas de rat�/critique/blocage (affichage 'dynamique' : appara�t/dispara�t sur deux positions (joueur 1 et 2)) */
    Texture imageTextesDynamiques;
    imageTextesDynamiques.loadFromFile("font/textes.png");
    IntRect rectTextesDynamiques[4];
    for(int rect=0; rect < 4; ++rect)
        rectTextesDynamiques[rect] = IntRect(185*rect, 0, 185, 45);
    Sprite texteDynamique;
    texteDynamique.setTexture(imageTextesDynamiques);
    texteDynamique.setTextureRect(rectTextesDynamiques[0]);
    /* Les chiffres qui seront affich�s indiquant les d�g�ts de l'attaque effectu�e */
    Sprite degatsDynamiques[2];
    for(int sprite=0; sprite < 2; ++ sprite) {
        degatsDynamiques[sprite].setTexture(imageChiffres);
        degatsDynamiques[sprite].setTextureRect(rectsChiffres[0]);
        degatsDynamiques[sprite].setPosition(Vector2f(-50, -50));
    }


    /* La grosse fl�che rouge indiquant le joueur dont c'est le tour */
    Texture imageFleche;
    imageFleche.loadFromFile("images/fleche.png");
    Sprite fleche;
    fleche.setTexture(imageFleche);
    fleche.setPosition(Vector2f(58, 306));


    /* Le message de fin "Player  wins !\nPress Enter" ... */
    Texture imageFin;
    imageFin.loadFromFile("font/fin.png");
    Sprite ecranFin;
    ecranFin.setTexture(imageFin);
    ecranFin.setPosition(Vector2f(272, 398));
    /* ... Et on ins�re le num�ro du vainqueur au milieu */
    Sprite numeroVainqueur;
    numeroVainqueur.setTexture(imageChiffres);
    numeroVainqueur.setTextureRect(rectsChiffres[0]);
    numeroVainqueur.setPosition(Vector2f(477, 401));


    /* L'image de l'oeuf qui sera lanc� lors de l'attaque "lancer oeuf" */
    Texture imageOeuf;
    imageOeuf.loadFromFile("images/oeuf.png");
    Sprite oeuf;
    oeuf.setTexture(imageOeuf);
    IntRect rectOeuf[2];
    for(int rect=0; rect < 2; ++rect)
        rectOeuf[rect] = IntRect(46*rect, 0, 46, 49);
    oeuf.setTextureRect(rectOeuf[tourJoueur]);
    oeuf.setPosition(Vector2f(181, 518));


    /* L'image du tas de terre qui sera lanc� lors de l'attaque "gratter terre" */
    Texture imageTerre;
    imageTerre.loadFromFile("images/terre.png");
    Sprite terre;
    terre.setTexture(imageTerre);
    IntRect rectTerre[2];
    for(int rect=0; rect < 2; ++rect)
        rectTerre[rect] = IntRect(49*rect, 0, 49, 24);
    terre.setTextureRect(rectOeuf[tourJoueur]);
    terre.setPosition(Vector2f(102, 626));


    /* On ne sortira de cette boucle que lorsque l'utilisateur appuiera sur Echap ou cliquera sur la croix */
    while(app.isOpen()) {

        Event event;

        /* On r�ceptionne tout �v�nement qui se produit ... */
        while (app.pollEvent(event)) {
            /* ... Et on v�rifie sa nature */
            switch(event.type) {

            case Event::Closed :
                app.close();
                break;

            /* Clic de souris */
            case Event::MouseButtonPressed : {
                Vector2f positionCurseur = Vector2f(event.mouseButton.x, event.mouseButton.y);

                /* Clic gauche */
                if(event.mouseButton.button == Mouse::Left) {
                    if(inGame) {
                        /* On v�rifie si le clic a �t� effectu� sur les boutons d'attaque (v�rification de la distance curseur <-> centre du bouton,
                            c'est � dire si la norme du vecteur curseur -> centre du bouton est inf�rieure au rayon du bouton) */
                        bool distanceBoutonAttaqueOeuf = (sqrt(pow(positionCurseur.x - centreBoutonAttaqueOeuf.x, 2)
                                                               + pow(positionCurseur.y - centreBoutonAttaqueOeuf.y, 2) ) ) < rayonBoutonAttaqueOeuf;

                        bool distanceBoutonAttaqueTerre = (sqrt( pow(positionCurseur.x - centreBoutonAttaqueTerre.x, 2)
                                                                + pow(positionCurseur.y - centreBoutonAttaqueTerre.y, 2) ) ) < rayonBoutonAttaqueTerre;

                        /* On s'assure que le joueur a cliqu� sur un des deux boutons (ce serait b�te de passer un tour pour un missclick !) */
                        if(distanceBoutonAttaqueOeuf || distanceBoutonAttaqueTerre) {

                            /* La fonction finTour() enl�ve tout (d�)buff temporaire (ici il n'y a que le malus de pr�cision qui dure un tour) */
                            joueurs[nombreJoueurs - 1 - tourJoueur].finTour();

                            /* Attaque "lancer oeuf" */
                            if(distanceBoutonAttaqueOeuf) {
                                /* On met � jour le texte dynamique (�/Miss/Crit/Blocked) tout en appelant la fonction lancerOeuf de la classe Poule qui g�re tout seule les d�g�ts et lanc�s de d�s */
                                texteDynamique.setTextureRect(rectTextesDynamiques[joueurs[tourJoueur].lancerOeuf(joueurs[nombreJoueurs - 1 - tourJoueur])]);
                                /* On place l'image de l'oeuf � l'endroit d'o� celui-ci va �tre "lanc�" */
                                oeuf.setPosition(Vector2f(181 + 588*tourJoueur, 513));
                                oeuf.setTextureRect(rectOeuf[tourJoueur]);
                                /* Et on place l'image du tas de terre en dehors de l'�cran */
                                terre.setPosition(Vector2f(-50, -50));
                            }


                            /* Attaque "gratter terre" */
                            else if(distanceBoutonAttaqueTerre) {
                                /* Idem que pour "lancer oeuf", on met � jour le texte tout en laissant la classe s'occuper des d�g�ts � infliger */
                                texteDynamique.setTextureRect(rectTextesDynamiques[joueurs[tourJoueur].gratterTerre(joueurs[nombreJoueurs - 1 - tourJoueur])]);
                                /* Idem, placement des images */
                                oeuf.setPosition(Vector2f(-50, -50));
                                terre.setPosition(Vector2f(102 + 745*tourJoueur, 626));
                                terre.setTextureRect(rectTerre[tourJoueur]);
                            }


                            /* On place le texte/les d�g�ts (affichage dynamique) devant le joueur attaquant */
                            texteDynamique.setPosition(Vector2f(186 + 443*tourJoueur, 603));

                            /* (int)(n/10) donnera les dizaines tandis que n%10 donnera les chiffres entiers */
                            degatsDynamiques[0].setTextureRect(rectsChiffres[(int)(joueurs[tourJoueur].getDegatsDerniereAttaque() / 10)]);
                            degatsDynamiques[1].setTextureRect(rectsChiffres[joueurs[tourJoueur].getDegatsDerniereAttaque() % 10]);
                            for(int sprite=0; sprite < 2; ++sprite)
                                degatsDynamiques[sprite].setPosition(Vector2f(237 + 439*tourJoueur + 42*sprite, 541));


                            /* On v�rifie si l'adversaire est vaincu */
                            if(joueurs[nombreJoueurs - 1 - tourJoueur].getVie() == 0) {
                                inGame = false;
                                numeroVainqueur.setTextureRect(rectsChiffres[tourJoueur+1]);
                                break;
                            }


                            /* On incr�mente tourJoueur pour passer au joueur suivant */
                            ++tourJoueur;

                            /* On incr�mente le compteur de tours �coul�s seulement lorsque les deux joueurs ont jou� leur tour... */
                            if(tourJoueur == 2) {
                                tourJoueur = 0;
                                ++tourActuel;
                                /* ... Et on actualise les chiffres affich�s en cons�quence */
                                spritesTourActuel[0].setTextureRect(rectsChiffres[(int)(tourActuel/10)]);
                                spritesTourActuel[1].setTextureRect(rectsChiffres[tourActuel % 10]);
                            }


                            /* On place la fl�che rouge au dessus du joueur suivant */
                            fleche.setPosition(Vector2f(58 + 763*tourJoueur, 306));
                        }
                    }
                }
            }


            /* Curseur d�plac� */
            case Event::MouseMoved : {
                Vector2f positionCurseur = Vector2f(event.mouseMove.x, event.mouseMove.y);

                if(inGame) {
                    /* On va simplement actualiser l'affichage des boutons (si le curseur est sur un bouton, ce dernier passe en �tat "survol�") */
                    bool distanceBoutonAttaqueOeuf = (sqrt(pow(positionCurseur.x - centreBoutonAttaqueOeuf.x, 2)
                                                           + pow(positionCurseur.y - centreBoutonAttaqueOeuf.y, 2) ) ) < rayonBoutonAttaqueOeuf;
                    boutonAttaqueOeuf.setTextureRect(rectBoutonAttaqueOeuf[distanceBoutonAttaqueOeuf]);


                    bool distanceBoutonAttaqueTerre = (sqrt( pow(positionCurseur.x - centreBoutonAttaqueTerre.x, 2)
                                                            + pow(positionCurseur.y - centreBoutonAttaqueTerre.y, 2) ) ) < rayonBoutonAttaqueTerre;
                    boutonAttaqueTerre.setTextureRect(rectBoutonAttaqueTerre[distanceBoutonAttaqueTerre]);
                }
                break;
            }


            /* Touche enfonc�e */
            case Event::KeyPressed : {
                int touche = event.key.code;

                switch(touche) {

                /* La touche Echap termine le programme */
                case Keyboard::Escape :
                    app.close();
                    break;

                /* La touche entr�e relance/r�initialise une partie */
                case Keyboard::Return :
                    if(!inGame)
                        inGame = true;

                    /* On redonne leurs caract�ristiques de base aux joueurs ... */
                    for(int joueur=0; joueur < nombreJoueurs; ++joueur)
                        joueurs[joueur].reinitialiser();

                    /* ... Et on reset les autres variables : compteurs ... */
                    tourJoueur = 0;
                    tourActuel = 1;

                    /* ... Chiffres et texte */
                    spritesTourActuel[0].setTextureRect(rectsChiffres[0]);
                    spritesTourActuel[1].setTextureRect(rectsChiffres[1]);
                    texteDynamique.setTextureRect(rectTextesDynamiques[0]);
                    for(int chiffre=0; chiffre < 2; ++chiffre) {
                        degatsDynamiques[chiffre].setTextureRect(rectsChiffres[0]);
                        degatsDynamiques[chiffre].setPosition(Vector2f(-50, -50));
                    }

                    /* ... Ainsi que la fl�che que l'on replace sur le joueur 1 */
                    fleche.setPosition(Vector2f(58 + 763*tourJoueur, 306));

                    break;

                default :
                    break;
                }
            }


            default :
                break;

            }

        }


        /* On "nettoie" la fen�tre pour mettre � jour l'affichage */
        app.clear();


        /* On affiche dans l'ordre (pour ne pas avoir de probl�me de superposition d'image) : Le background... */
        app.draw(fond);


        /* ... Le nombre de tours �coul�s ... */
        for(int sprite=0; sprite < 2; ++ sprite)
            app.draw(spritesTourActuel[sprite]);


        /* ... Les boutons d'attaque ... */
        app.draw(boutonAttaqueOeuf);
        app.draw(boutonAttaqueTerre);


        /* ... La fl�che indiquant le joueur dont c'est le tour ... */
        app.draw(fleche);


        /* ... Les d�g�ts et textes concernant les attaques ... */
        app.draw(texteDynamique);
        for(int sprite=0; sprite < 2; ++ sprite)
            app.draw(degatsDynamiques[sprite]);


        /* ... Les joueurs et leur barre de vie actuelle ... */
        for(int numero=0; numero < nombreJoueurs; ++numero) {
            app.draw(joueurs[numero].getSprite());
            app.draw(joueurs[numero].getBarreVie());
        }


        /* ... L'�cran de fin et le num�ro du vainqueur si le combat est fini ... */
        if(!inGame) {
            app.draw(ecranFin);
            app.draw(numeroVainqueur);
        }


        /* ... L'oeuf / le tas de terre des animations d'attaque si elles sont en cours (Joueur 1) */
        if(tourJoueur == 1) {

            /* L'oeuf parcourt l'�cran jusqu'� atteindre le joueur d'en face */
            if(oeuf.getPosition().x < 780)
                oeuf.move(Vector2f(12,0));

            /* Une fois � destination, on le place hors de l'�cran (= il dispara�t) */
            else if(oeuf.getPosition().x > 780)
                oeuf.setPosition(Vector2f(-50, -50));

            /* M�me principe pour le tas de terre */
            if(terre.getPosition().x < 770) {
                float norme = sqrt(pow(770 - 102, 2) + pow(481 - 626, 2));
                terre.move(Vector2f( (12/norme)*(770 - 102), (12/norme)*(481 - 626) ));
            }

            else if(terre.getPosition().x > 770)
                terre.setPosition(Vector2f(-50, -50));
        }


        /* Idem pour le joueur 2 */
        else {
            if(oeuf.getPosition().x > 180)
                oeuf.move(Vector2f(-12, 0));

            else if(oeuf.getPosition().x < 180)
                oeuf.setPosition(Vector2f(-50, -50));

            if(terre.getPosition().x > 183) {
                float norme = sqrt(pow(183 - 847, 2) + pow(481 - 626, 2));
                terre.move(Vector2f( (12/norme)*(183 - 847), (12/norme)*(481 - 626) ));
            }

            else if(terre.getPosition().x < 183)
                terre.setPosition(Vector2f(-50, -50));
        }


        /* Puis on affiche l'oeuf et le tas de terre */
        app.draw(oeuf);
        app.draw(terre);


        /* Et enfin on met � jour l'affichage en demandant � la fen�tre d'afficher tout �a � l'�cran */
        app.display();
    }

    return 0;
}
