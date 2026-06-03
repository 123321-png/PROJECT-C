#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int achat(int prix);

int main()
{
    int choix;

    int cafe = 0, the = 0, choco = 0, capuccino = 0;

    int ca = 0;

    FILE *fvente;

    do
    {
        printf("\n==============================\n");
        printf("   DISTRIBUTEUR AUTOMATIQUE\n");
        printf("==============================\n");

        printf("1. Cafe court .......... 0.40 €\n");

        printf("2. The a la menthe ..... 0.50 €\n");

        printf("3. Chocolat chaud ...... 0.60 €\n");

        printf("4. Cappuccino ......... 0.80 €\n");

        printf("------------------------------\n");

        printf("999. ADMIN\n");

        printf("0. QUITTER\n");

        printf("\nVotre choix : ");

        scanf("%d", &choix);

        if (choix == 1)
        {
            printf("\nCafe court choisi\n");

            cafe++;

            ca += 40;

            achat(40);

            fvente = fopen("vente.dat", "a");

            fprintf(fvente, "Cafe court - 0.40 euro\n");

            fclose(fvente);
        }
        else if (choix == 2)
        {
            printf("\nThe a la menthe choisi\n");

            the++;

            ca += 50;

            achat(50);

            fvente = fopen("vente.dat", "a");

            fprintf(fvente, "The - 0.50 euro\n");

            fclose(fvente);
        }
        else if (choix == 3)
        {
            printf("\nChocolat chaud choisi\n");

            choco++;

            ca += 60;

            achat(60);

            fvente = fopen("vente.dat", "a");

            fprintf(fvente, "Chocolat chaud - 0.60 euro\n");

            fclose(fvente);
        }
        else if (choix == 4)
        {
            printf("\nCappuccino choisi\n");

            capuccino++;

            ca += 80;

            achat(80);

            fvente = fopen("vente.dat", "a");

            fprintf(fvente, "Cappuccino - 0.80 euro\n");

            fclose(fvente);
        }

        else if (choix == 999)
        {
            FILE *fa = fopen("auth.txt", "r");

            char user[50], pass[50];

            char fileUser[50], filePass[50];

            if (fa == NULL)
            {
                printf("\n=== CREATION COMPTE ADMIN ===\n");

                fa = fopen("auth.txt", "w");

                printf("Nom utilisateur : ");
                scanf("%s", fileUser);

                printf("Mot de passe : ");
                scanf("%s", filePass);

                fprintf(fa, "%s\n%s\n", fileUser, filePass);
                fclose(fa);

                printf("Compte cree !\n");
            }
            else
            {
                fscanf(fa, "%s\n%s", fileUser, filePass);
                fclose(fa);

                printf("\nLOGIN : ");
                scanf("%s", user);

                printf("PASSWORD : ");
                scanf("%s", pass);

                if (strcmp(user, fileUser) == 0 && strcmp(pass, filePass) == 0)
                {
                    printf("\n=== ADMIN CONNECTE ===\n");

                    printf("Cafe : %d\n", cafe);

                    printf("The : %d\n", the);

                    printf("Chocolat : %d\n", choco);

                    printf("Cappuccino : %d\n", capuccino);

                    printf("Chiffre d'affaires : %.2f €\n", ca / 100.0);
                }
                else
                {
                    printf("Identifiants incorrects !\n");
                }
            }
        }

    } while (choix != 0);

    printf("Au revoir !\n");
    return 0;
}

/* ========================= */

int achat(int prix)
{
    float piece;
    int total = 0;

    do
    {
        printf("Inserez piece (0.10 / 0.20 / 0.50 / 1 / 2 €) : ");

        scanf("%f", &piece);

        int valeur = 0;

        if (piece == 0.10f) valeur = 10;

        else if (piece == 0.20f) valeur = 20;

        else if (piece == 0.50f) valeur = 50;

        else if (piece == 1.0f) valeur = 100;

        else if (piece == 2.0f) valeur = 200;
        else
        {
            printf("Piece invalide\n");
            continue;
        }

        total += valeur;

        if (total < prix)
        {
            printf("Reste a payer : %.2f €\n", (prix - total) / 100.0);
        }

    } while (total < prix);

    printf("Preparation en cours...\n");

    printf("Monnaie rendue : %.2f €\n", (total - prix) / 100.0);
    
    printf("Merci !\n");

    system("pause");

    return total - prix;
}