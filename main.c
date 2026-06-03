#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ---- Couleurs terminal ---- */
#define VERT    "\033[32m"
#define RESET   "\033[0m"
#define GRAS    "\033[1m"

/* ---- Produits ---- */
#define NB_PRODUITS 4

char  noms[NB_PRODUITS][30]  = {"Cafe Court", "The a la Menthe", "Chocolat Chaud", "Cappuccino"};
float prix[NB_PRODUITS]      = {0.40f, 0.50f, 0.60f, 0.80f};
int   compteur[NB_PRODUITS]  = {0, 0, 0, 0};

/* ---- Caisse ---- */
float chiffre_affaires = 0.0f;

/* ---- Fichiers ---- */
#define FICHIER_VENTE "vente.dat"
#define FICHIER_AUTH  "auth.txt"

/* ---- Pieces acceptees ---- */
float pieces_valides[5] = {0.10f, 0.20f, 0.50f, 1.00f, 2.00f};

/* ================================================
   FONCTIONS UTILITAIRES
   ================================================ */

/* Verifie si la piece inseree est valide */
int piece_valide(float piece) {
    int i;
    for (i = 0; i < 5; i++) {
        /* Comparaison avec une petite marge pour les flottants */
        if (piece >= pieces_valides[i] - 0.001f &&
            piece <= pieces_valides[i] + 0.001f) {
            return 1;
        }
    }
    return 0;
}

/* Sauvegarde les ventes dans le fichier */
void sauvegarder_ventes() {
    FILE *f = fopen(FICHIER_VENTE, "w");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir vente.dat\n");
        return;
    }
    int i;
    for (i = 0; i < NB_PRODUITS; i++) {
        fprintf(f, "%s;%d\n", noms[i], compteur[i]);
    }
    fprintf(f, "CA;%.2f\n", chiffre_affaires);
    fclose(f);
}

/* Charge les ventes depuis le fichier */
void charger_ventes() {
    FILE *f = fopen(FICHIER_VENTE, "r");
    if (f == NULL) return; /* Premiere execution, pas encore de fichier */

    char ligne[100];
    int i = 0;
    while (fgets(ligne, sizeof(ligne), f) && i < NB_PRODUITS) {
        char nom_lu[30];
        int  qte_lue;
        if (sscanf(ligne, "%29[^;];%d", nom_lu, &qte_lue) == 2) {
            compteur[i] = qte_lue;
            i++;
        } else {
            float ca_lu;
            if (sscanf(ligne, "CA;%f", &ca_lu) == 1) {
                chiffre_affaires = ca_lu;
            }
        }
    }
    fclose(f);
}

/* ================================================
   FONCTIONS ADMINISTRATION
   ================================================ */

/* Verifie si le compte admin existe deja */
int compte_existe() {
    FILE *f = fopen(FICHIER_AUTH, "r");
    if (f == NULL) return 0;
    fclose(f);
    return 1;
}

/* Cree le compte admin (premier acces) */
void creer_compte() {
    char login[50], mdp[50];
    printf(VERT "\n=== Creation du compte administrateur ===\n" RESET);
    printf("Choisissez un nom d'utilisateur : ");
    scanf("%49s", login);
    printf("Choisissez un mot de passe : ");
    scanf("%49s", mdp);

    FILE *f = fopen(FICHIER_AUTH, "w");
    if (f == NULL) {
        printf("Erreur : impossible de creer auth.txt\n");
        return;
    }
    fprintf(f, "%s\n%s\n", login, mdp);
    fclose(f);
    printf(VERT "Compte cree avec succes !\n" RESET);
}

/* Authentifie l'admin, retourne 1 si OK */
int authentifier() {
    char login_fichier[50], mdp_fichier[50];
    char login_saisi[50],   mdp_saisi[50];

    FILE *f = fopen(FICHIER_AUTH, "r");
    if (f == NULL) return 0;
    fscanf(f, "%49s\n%49s", login_fichier, mdp_fichier);
    fclose(f);

    printf("Login : ");
    scanf("%49s", login_saisi);
    printf("Mot de passe : ");
    scanf("%49s", mdp_saisi);

    if (strcmp(login_saisi, login_fichier) == 0 &&
        strcmp(mdp_saisi,   mdp_fichier)   == 0) {
        return 1;
    }
    return 0;
}

/* Changer le mot de passe */
void changer_mdp() {
    char login_fichier[50], mdp_fichier[50];
    char ancien[50], nouveau[50];

    FILE *f = fopen(FICHIER_AUTH, "r");
    if (f == NULL) return;
    fscanf(f, "%49s\n%49s", login_fichier, mdp_fichier);
    fclose(f);

    printf("Ancien mot de passe : ");
    scanf("%49s", ancien);

    if (strcmp(ancien, mdp_fichier) != 0) {
        printf(VERT "Mot de passe incorrect !\n" RESET);
        return;
    }

    printf("Nouveau mot de passe : ");
    scanf("%49s", nouveau);

    f = fopen(FICHIER_AUTH, "w");
    if (f == NULL) return;
    fprintf(f, "%s\n%s\n", login_fichier, nouveau);
    fclose(f);
    printf(VERT "Mot de passe modifie avec succes !\n" RESET);
}

/* Reinitialie les ventes et la caisse */
void reinitialiser() {
    int i;
    for (i = 0; i < NB_PRODUITS; i++) compteur[i] = 0;
    chiffre_affaires = 0.0f;
    sauvegarder_ventes();
    printf(VERT "Machine reinitialisee !\n" RESET);
}

/* ================================================
   MENU ADMINISTRATION
   ================================================ */
void menu_admin() {

    /* Premier acces : creation du compte */
    if (!compte_existe()) {
        creer_compte();
        return;
    }

    /* Authentification */
    printf(VERT "\n--- Espace Administration ---\n" RESET);
    int essais = 0;
    while (essais < 3) {
        if (authentifier()) break;
        essais++;
        printf(VERT "Identifiants incorrects. Essai %d/3\n" RESET, essais);
        if (essais == 3) {
            printf("Trop d'essais. Retour au menu principal.\n");
            return;
        }
    }

    int choix_admin;
    do {
        printf(VERT "\n---------- MENU ADMINISTRATION ----------\n" RESET);
        printf("1. Etat de la caisse (Chiffre d'affaires)\n");
        printf("2. Rapport des ventes (Quantites vendues)\n");
        printf("3. Gestion du compte (Changer le mot de passe)\n");
        printf("4. Reinitialiser la machine (Caisse et ventes)\n");
        printf("0. Retour au menu client\n");
        printf(VERT "-----------------------------------------\n" RESET);
        printf("Saisissez votre choix : ");
        scanf("%d", &choix_admin);

        switch (choix_admin) {
            case 1:
                printf(VERT "\nChiffre d'affaires total : %.2f EUR\n" RESET, chiffre_affaires);
                break;
            case 2:
                printf(VERT "\n--- Rapport des ventes ---\n" RESET);
                int i;
                for (i = 0; i < NB_PRODUITS; i++) {
                    printf("%-20s : %d vendu(s)\n", noms[i], compteur[i]);
                }
                break;
            case 3:
                changer_mdp();
                break;
            case 4:
                reinitialiser();
                break;
            case 0:
                printf("Retour au menu client.\n");
                break;
            default:
                printf(VERT "Choix invalide !\n" RESET);
        }
    } while (choix_admin != 0);
}

/* ================================================
   CYCLE DE VENTE
   ================================================ */
void cycle_vente(int index_produit) {
    float prix_produit = prix[index_produit];
    float total_insere = 0.0f;
    float piece;

    printf(VERT "\nVous avez choisi : %s (%.2f EUR)\n" RESET,
           noms[index_produit], prix_produit);
    printf("Inserez vos pieces (0.10 / 0.20 / 0.50 / 1.00 / 2.00) :\n");

    do {
        printf("Inserez une piece : ");
        scanf("%f", &piece);

        if (!piece_valide(piece)) {
            printf(VERT "Piece non acceptee ! Valeurs valides : 0.10, 0.20, 0.50, 1.00, 2.00\n" RESET);
            continue;
        }

        total_insere += piece;

        if (total_insere < prix_produit) {
            printf("Montant restant a payer : %.2f EUR\n", prix_produit - total_insere);
        }

    } while (total_insere < prix_produit);

    /* Finalisation */
    printf(VERT "\nPreparation en cours...\n" RESET);

    float rendu = total_insere - prix_produit;
    if (rendu > 0.001f) {
        printf("Monnaie rendue : %.2f EUR\n", rendu);
    } else {
        printf("Pas de monnaie a rendre.\n");
    }

    /* Mise a jour des statistiques */
    compteur[index_produit]++;
    chiffre_affaires += prix_produit;
    sauvegarder_ventes();

    printf(VERT "Bonne degustation !\n" RESET);
}

/* ================================================
   MENU PRINCIPAL
   ================================================ */
void afficher_menu() {
    printf(VERT "\n****************************************************\n" RESET);
    printf(VERT "*       BIENVENUE AU DISTRIBUTEUR AUTOMATIQUE      *\n" RESET);
    printf(VERT "****************************************************\n" RESET);
    printf("*                                                  *\n");
    printf("*  1. %-20s       %.2f EUR  *\n", noms[0], prix[0]);
    printf("*  2. %-20s       %.2f EUR  *\n", noms[1], prix[1]);
    printf("*  3. %-20s       %.2f EUR  *\n", noms[2], prix[2]);
    printf("*  4. %-20s       %.2f EUR  *\n", noms[3], prix[3]);
    printf("*                                                  *\n");
    printf("*  0. Quitter le programme                         *\n");
    printf(VERT "*  999. ESPACE ADMINISTRATION                       *\n" RESET);
    printf("*                                                  *\n");
    printf(VERT "****************************************************\n" RESET);
    printf("Saisissez votre choix : ");
}

/* ================================================
   MAIN
   ================================================ */
int main() {
    int choix;

    /* Charger les donnees existantes */
    charger_ventes();

    do {
        afficher_menu();
        scanf("%d", &choix);

        switch (choix) {
            case 1:
            case 2:
            case 3:
            case 4:
                cycle_vente(choix - 1);
                break;
            case 999:
                menu_admin();
                break;
            case 0:
                printf(VERT "\nMerci et a bientot !\n" RESET);
                break;
            default:
                printf(VERT "Choix invalide ! Veuillez recommencer.\n" RESET);
        }

    } while (choix != 0);

    return 0;
}