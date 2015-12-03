#ifndef __ENTETE_H_
#define __ENTETE_H_

#include <cstdio>
#include <cstdlib> 
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>  
#include <cmath>
#include <vector>
#include <limits>
using namespace std;

enum eProb	{ALPINE, BANANE, EGGHOLDER, MAXCUT};

/* POUR TRAITER MAXCUT: ENLEVER LES COMMENTAIRES ET RETIRER L'AUTRE ENREGISTREMENT DE tProblem */
struct tArc
{
	int Ni;										//Noeud départ de l'arc NB: 0 à NbNoeud-1 
	int Nj;										//Noeud arrivée de l'arc NB: 0 à NbNoeud-1
	int Poids;									//Poids de l'arc (+1, 0, -1)
};

struct tProblem									//**Définition du problème de MAXCUT:
{
	eProb	Fonction;							//**Nom de la fonction ou du problème à traiter
	int		D;									//**Nbre de Variables (Nombre de noeuds pour le MAX CUT)
	double	Xmin;								//**Domaine des variables: valeur minimale 
	double	Xmax;								//**Domaine des variables: valeur maximale
	std::string Nom;							//**Nom du fichier de données
	int NbNoeud;								//**Nbre de noeuds dans l'instance NB: numérotés de 0 à NbNoeud-1
	int NbArc;									//**Nbre d'arcs dans l'instance  NB: numérotés de 0 à NbArc-1
	std::vector <tArc> Arc;						//**Définition de chaque arc. NB: Tableaux de 0 à NbArc-1. 
};

/*
struct tProblem									//**Définition pour fonction continue:
{
	eProb	Fonction;							//**Nom de la fonction ou du problème à traiter
	int		D;									//**Nbre de Variables (dimensions)
	double	Xmin;								//**Domaine des variables: valeur minimale 
	double	Xmax;								//**Domaine des variables: valeur maximale
};
*/

struct tPosition								//**Définition de la position d'une particule: 
{
	std::vector<double>		X;					//**Position actuelle de la particule pour chacune des dimensions
	double					FctObj;				//**Valeur de la fonction objectif
};

struct tParticule								//**Définition d'une solution: 
{
	tPosition				Pos;				//**Position actuelle de la particule
	std::vector<double>		V;					//**Vitesse actuelle de la particule pour chacune des dimensions
	std::vector<tParticule*> Info;				//**Liste des informatrices de la particule
	tPosition				BestPos;			//**Meilleure position de la particule (expérience)
};

struct tPSO
{
	int		Taille;						//**Taille de l'essaim (nombre de particules)
	double	C1;							//**Coefficient de confiance en soi
	double	C2;							//**Coefficient de confiance en son expérience
	double	C3;							//**Coefficient de confiance en sa meilleure informatrice
	int		NbInfo;						//**Nombre d'informatrices pour chaque particule
	double	Vmax;						//**Vitesse maximale								//### Pour MAXSAT
	int		Iter;						//**Compteur du nombre de générations				
	double	Precision;					//**Niveau de précision souhaité pour les problèmes à variables continues	//### Pour fonctions continues
	int		CptEval;					//**COMPTEUR DU NOMBRE DE SOLUTIONS EVALUEES. SERT POUR CRITERE D'ARRET.
	int		NB_EVAL_MAX;				//**CRITERE D'ARRET: MAXIMUM "NB_EVAL_MAX" EVALUATIONS.
};

#endif