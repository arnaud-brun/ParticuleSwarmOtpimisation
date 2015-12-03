#include "Entete.h"

//%%%%%%%%%%%%%%%%%%%%%%%%% IMPORTANT: %%%%%%%%%%%%%%%%%%%%%%%%% 
//Le fichier de probleme (.txt) doit se trouver dans le r�pertoire courant du projet 
//pour une ex�cution � l'aide du compilateur. Indiquer les arguments du programme dans les 
//propri�t�s du projet - d�bogage - arguements.
//Sinon, utiliser le r�pertoire execution.

//*****************************************************************************************
// Prototype des fonctions
//*****************************************************************************************
void InitialisationIntervalleVariable(tProblem &unProb);
tPosition InitialisationEssaim(std::vector<tParticule> &unEssaim, tProblem unProb, tPSO &unPSO);
void InitialisationInformatrices(std::vector<tParticule> &unEssaim, tPSO &unPSO);
void InitialisationPositionEtVitesseAleatoire(tParticule &Particule, tProblem unProb);
void EvaluationPosition(tPosition &Pos, tProblem unProb, tPSO &unPSO);
void DeplacerEssaim(std::vector<tParticule> &unEssaim, tProblem unProb, tPSO &unPSO, tPosition &Meilleure);
void DeplacerUneParticule(tParticule &Particule, tProblem unProb, tPSO &unPSO);
tParticule* TrouverMeilleureInformatrice(tParticule &Particule, tPSO &unPSO);
double AleaDouble(double a, double b);
void AfficherSolutions(std::vector<tParticule> unEssaim, int Debut, int Fin, int Iter, tProblem unProb);
void AfficherUneSolution(tPosition P, int Iter, tProblem unProb);
void AfficherResultats (tPosition uneBest, tProblem unProb, tPSO unPSO);
void LibererMemoireFinPgm (std::vector<tParticule> &unEssaim, tProblem unProb, tPSO unPSO);
/* ### POUR MAXCUT */
void LectureProblemeMAXCUT(std::string FileName, tProblem & unProb, tPSO &unPSO);
void AfficherProblemeMAXCUT (tProblem unProb);



// % modif % //
int run(int NbParam, char *Param[]);				//Prorotype de la fonction run : ancien main
int analyse(int function_choice, int nb_run);		//Fonction d'analyse du fichier mis � jour par les appels de la fonction run


/* 
Fonction main annexe pour automatiser le processus 
Appelle 100 fois la fonction run avec les memes parametres
*/
int main(){

	int status = 0;

	/* Nombre d'appel souhait� de la fonction run */
	int nb_iter = 1;

	/*
	D�finiton des param�tres pour l'ex�cution de la fonction run :
	arg1 = fonction : 0 <=> ALPINE / 1 <=> BANANE / 2 <=> EGGHOLDER / 3  <=> MAXCUT
	arg2 = taille
	arg3 = C1
	arg4 = C2
	arg5 = C3
	arg6 = nb_eval
	arg7 = nom du fichier � lancer pour maxcut
	*/
	/* Ex�cution classique */
	//char* args[] = {"2", "30", "0.7", "2.0", "2.0", "10000"};
	/* Ex�cution MAXCUT */
	char* args[] = {"3", "30", "0.9", "1.7", "1.7", "10000", "G1.txt"};


	for(int i=0; i<nb_iter; i++){
		/* Ex�cution normale */
		//status = run(5, args);
		/* Ex�cution de MAXCUT */
		status = run(6 , args);

		if (status != 0){
			cout << "ERROR in run termination";
		}
	}
	
	//Analyse des r�sultats issus des ex�cutions	
	//analyse(EGGHOLDER, nb_iter);

	system("PAUSE");
	return 0;
}


//Fonction d'analyse des r�sultats retourn�s par les diff�rents appels � la fonction run
/*
Cr�e un fichier texte sp�cifique � l'analyse des r�sultat d'une fonction 
Sauvegarde en fichier des diff�rents r�sultats finaux de chaque ex�cution
Sauvegarde en fichier de la moyenne obtenu sur les ex�cution
*/
int analyse(int function_choice, int nb_run){

	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << "ANALYSE " << endl;

	ifstream myfile;	//Cr�er un flux pour acc�der au fichier
	string name_file;	//G�rer le nom du fichier
	
	switch (function_choice)
	{
		//Chaque type de fonction ecrit dans un fichier sp�cifique : r�cup�rer ce fichier
		case ALPINE: cout << "ALPINE"; name_file = "alpine_res.txt"; break;
		case BANANE: cout << "BANANE"; name_file = "banane_res.txt"; break;
		case EGGHOLDER: cout << "EGGHOLDER"; name_file = "eggholder_res.txt"; break;
		default: cout << " a definir..."; return -1;
	}

	//Ouverture du fichier associ� a la fonction
	myfile.open(name_file);

	cout << endl;

	//parcours dans le fichier pour r�cup�rer les diff�rents r�sultats des diff�rents appels
	int index = 1;
	vector <double> best_solutions;
	if(myfile){			
		while(myfile){
			string line;
			getline(myfile, line);
			//Stockage des r�sultats des ex�cutions dans un vecteur
			if (strcmp(line.c_str(), "\n") != 0) {
				best_solutions.resize(index, strtod(line.c_str(), NULL));
				index++;
			}
		}
	}

	//fermeture et suppression du fichier g�n�r� par les ex�cutions
	myfile.close();
	if( remove(name_file.c_str()) != 0 )
		perror( "Error deleting file" );
	else
		puts( "File successfully deleted" );

	/*
	Cr�ation du fichier sp�cifique � la sauvegarde des ex�cutions
	Sauvegarde des diff�rentes ex�cution
	Sauvegarde de la moyenne 
	*/
	cout << "CHECK VECTOR : " <<endl;
	double nb_iter = 0.0;
	double mean = 0.0;
	vector<double>::iterator it;

	int opened = -1;
	ofstream result_file;
	if (strcmp(name_file.c_str(), "alpine_res.txt") == 0){
		result_file.open("ALPINE_EXECUTION.txt");
		opened = 1;
	}

	if (strcmp(name_file.c_str(), "banane_res.txt") == 0){
		result_file.open("BANANE_EXECUTION.txt");
		opened = 1;
	}

	if (strcmp(name_file.c_str(), "eggholder_res.txt") == 0){
		result_file.open("EGGHOLDER_EXECUTION.txt");
		opened = 1;
	}

	if (opened != 1){
		cout << "fail to open result file for execution";
		return -1;
	}
	
	//R�glage de la precision d'affichage
	cout << fixed << showpoint;
	cout << setprecision(12);

	//R�glage de la precision d'affichage dans le fichier
	result_file << fixed << showpoint;
	result_file << setprecision(12);
	
	//Parcours des solutions pr�c�demment trouv�es + sauvegarde en fichier
	for(it=best_solutions.begin(); it<best_solutions.end(); it++){
		if (nb_iter < nb_run){
			cout << nb_iter << " : " << *it << endl;
			// result_file << *it << endl; // A activer si on veut sauvegarder une solution
			mean += *it;
		}
		nb_iter += 1.0;
	}

	// Calcul de la moyenne
	nb_iter -= 1.0;
	mean = mean/(nb_iter);

	cout << "MEAN on " << nb_run << " runs : ";
	cout << mean << endl;
	//result_file << "MOYENNE : " << mean << endl; // A activer si on veut sauvegarder une solution

	result_file.close();
	return 0;

}
// % modif end % //


//******************************************************************************************
// Fonction main initiale transform�e en fonction normale afin d'automatiser le processus d'anlayse
//******************************************************************************************
int run(int NbParam, char *Param[])
{
	
	tProblem LeProb;					//**D�finition de l'instance de probl�me
	tPSO LePSO;							//**D�finition des param�tres du PSO
	std::vector<tParticule> Essaim;		//**Ensemble de solutions 
	tPosition Best;						//**Meilleure solution depuis le d�but de l'algorithme
	
	string NomFichier;					//### Pour MAXCUT
	
	//**Lecture des param�tres

	//**Sp�cifications du probl�me � traiter, pass� en param�tre
	LeProb.Fonction = static_cast<eProb>(atoi(Param[0]));	

	LePSO.Taille		= atoi(Param[1]);
	LePSO.C1			= atof(Param[2]);
	LePSO.C2			= atof(Param[3]);
	LePSO.C3			= atof(Param[4]);
	LePSO.NB_EVAL_MAX	= atoi(Param[5]);

	//### Pour MAXCUT
	if (LeProb.Fonction == MAXCUT)
		NomFichier.assign(Param[6]);

	LePSO.Iter			= 0;
	LePSO.CptEval		= 0;
		
	srand((unsigned) time(NULL));			//**Precise un germe pour le generateur aleatoire
	cout.setf(ios::fixed|ios::showpoint);
	
	InitialisationIntervalleVariable(LeProb);

	//### Pour MAXCUT
	//**Lecture du fichier de MAXCUT
	if (LeProb.Fonction == MAXCUT){
		LectureProblemeMAXCUT(NomFichier, LeProb, LePSO);			
		AfficherProblemeMAXCUT(LeProb);
	}

	
	//**Dimension du tableaux de l'essaim selon le nombre de particules
	Best = InitialisationEssaim(Essaim, LeProb, LePSO);
	InitialisationInformatrices(Essaim, LePSO);

	// Obtenir la bonne fonction d'affichage selon la fonction � analyser
	if (LeProb.Fonction == MAXCUT)
		AfficherSolutions(Essaim, 0, LePSO.Taille, LePSO.Iter, LeProb);
	else
		AfficherUneSolution(Best, LePSO.Iter, LeProb);



	//**Boucle principale du PSO
	while (LePSO.CptEval < LePSO.NB_EVAL_MAX) 	//**NE PAS ENLEVER LA CONDITION SUR LE NOMBRE D'�VALUATION
	{																									
		LePSO.Iter++;
		DeplacerEssaim(Essaim, LeProb, LePSO, Best);  
		
		// Obtenir la bonne fonction d'affichage selon la fonction � analyser
		if (LeProb.Fonction == MAXCUT)
			AfficherSolutions(Essaim, 0, LePSO.Taille, LePSO.Iter, LeProb);
		else
			AfficherUneSolution(Best, LePSO.Iter, LeProb);

		cout << LePSO.CptEval << endl;
	};

	AfficherResultats (Best, LeProb, LePSO);		//**NE PAS ENLEVER
	LibererMemoireFinPgm(Essaim, LeProb, LePSO);

	//system("PAUSE"); // Pause mise au niveau du main
	return 0;
}

//**-----------------------------------------------------------------------
//**D�termine l'intervalle de recherche selon la fonction choisie
void InitialisationIntervalleVariable(tProblem &unProb)
{
	switch(unProb.Fonction)
	{
		case ALPINE:	unProb.Xmin = -10.0;	unProb.Xmax = 10.0;	unProb.D = 2; break;
		case BANANE:	unProb.Xmin = -10.0;	unProb.Xmax = 10.0;	unProb.D = 2; break;
			
		// % modif % //
		case EGGHOLDER: 
			unProb.Xmin = -512.0;
			unProb.Xmax =512.0; 
			unProb.D = 2; 
			break;
		case MAXCUT:	
			unProb.Xmin = 0;
			unProb.Xmax = 1; 
			//unProb.D d�j� fix� par la lecture du fichier MAXCUT
			break;
		// % modif end % //

		default:		unProb.Xmin = 0.0;		unProb.Xmax = 0.0;	unProb.D = 0; break; 
	}
}

//**-----------------------------------------------------------------------
//**Dimension des vecteurs de l'essaim, initialisation des particules et retourne la meilleure solution rencontr�e
tPosition InitialisationEssaim(std::vector<tParticule> &unEssaim, tProblem unProb, tPSO &unPSO)
{
	int i;
	tPosition Meilleure;

	unEssaim.resize(unPSO.Taille);		//**Le tableau utilise les indices de 0 � Taille-1.
	
	for (i=0; i<unPSO.Taille; i++)
	{
		unEssaim[i].Pos.X.resize(unProb.D);
		unEssaim[i].V.resize(unProb.D);
		InitialisationPositionEtVitesseAleatoire(unEssaim[i], unProb);
		EvaluationPosition(unEssaim[i].Pos, unProb, unPSO);
		//Initialisation de la meilleure position de la particule � sa position initiale
		unEssaim[i].BestPos.X.resize(unProb.D);
		unEssaim[i].BestPos = unEssaim[i].Pos;
		//Conservation de la meilleure solution initiale
		if (i == 0)	
			Meilleure = unEssaim[i].Pos;
		else
			// % modif % //
			if (unProb.Fonction == MAXCUT)
				/* MAXCUT, on doit maximiser la fonction objectif */
				if (unEssaim[i].Pos.FctObj > Meilleure.FctObj)
					Meilleure = unEssaim[i].Pos;
			
			else
				/* Cas normal : on doit minimser la fonction objectif */
				if (unEssaim[i].Pos.FctObj < Meilleure.FctObj)
					Meilleure = unEssaim[i].Pos;
			// % end modif % //
	}
	//Retourne la meilleure solution renontr�e jusqu'� maintenant
	return(Meilleure);
}

//**-----------------------------------------------------------------------
//**D�termine le groupement d'informatrices des particules
void InitialisationInformatrices(std::vector<tParticule> &unEssaim, tPSO &unPSO)
{
	int i;
	
	unPSO.NbInfo = 4;	// Chaque particule aura 4 informatrices
	for(i=0; i<unPSO.Taille; i++)
	{
		//Dimension du vecteur d'informatrices
		unEssaim[i].Info.resize(unPSO.NbInfo);
		

		// % modif % //
		/*
		Alocation al�atoire des informatrices � chaque particule parmis les unPSO.NbInfo informatrices disponibles
		--> Cr�ation d'un vecteur pour enregistrer les informatrices de la particule
		--> Toutes les informatrices d'une particules sont diff�rentes
		--> La particule ne peut pas �tre sa propre informatrice
		*/

		//D�finition du vecteur de sauvegarde des informatrices connues par une particule
		vector <int> neighbour_save;
		neighbour_save.resize(unPSO.NbInfo);

		for (int j=0; j<unPSO.NbInfo; j++)
		{
			bool ok = false;		
			while(!ok){				
				//Recuperer un index de particule aleatoire
				int target = AleaDouble(0, unPSO.Taille-1);
				
				//s'assurer que cet index n'est pas celui de la particule en question, ni qu'il existe deja dans la liste
				if( (target != i ) &&
					! (std::find(neighbour_save.begin(), neighbour_save.end(), target) != neighbour_save.end()) ) 
				{
					//Ajouter cet index de particule � la liste des informatrices de la particule
					neighbour_save.insert(neighbour_save.end(), target);
					ok = true;

				}

				//Ajouter cet index a la liste des informatrices de la particule
				unEssaim[i].Info[j] = & unEssaim[j];
			}			
		}

		//Reset de la liste des particules pour le traitement de la particule suivante
		neighbour_save.clear();
		// % modif end % //
	}
}

//**-----------------------------------------------------------------------
//**Initialise al�atoirement le vecteur de positions et de vitesses
void InitialisationPositionEtVitesseAleatoire(tParticule &Particule, tProblem unProb)
{
	int d;

	for(d=0; d<unProb.D; d++)
	{
		

		// % modif % //
		if (unProb.Fonction == MAXCUT ) {
			/* Initialisation de la position soit � 0 soit � 1 */
			Particule.Pos.X[d] = AleaDouble(unProb.Xmin, unProb.Xmax);

			// Arondir � la valeur enti�re la plus proche
			if (Particule.Pos.X[d] <= (unProb.Xmin + unProb.Xmax) )
				Particule.Pos.X[d] = unProb.Xmin;
			else
				Particule.Pos.X[d] = unProb.Xmax;

			//initialiser la vitesse aleatoirement sur l'intervalle tol�r� par la fonction sigmoidale : [-4;4]
			Particule.V[d] = AleaDouble(-4, 4);

		}
		else{
			/* Cas normal : on initialise avec des variables al�atoires */
			Particule.Pos.X[d] = AleaDouble(unProb.Xmin, unProb.Xmax);
			Particule.V[d] = AleaDouble(unProb.Xmin, unProb.Xmax);
		}
		// % end modif % //
	}
}

//**-----------------------------------------------------------------------
//**�value la position de la particule avec la fonction choisie
void EvaluationPosition(tPosition &Pos, tProblem unProb, tPSO &unPSO)
{
	double xd, som1=0.0, som2=0.0, valeur=0.0, p=1;
	int d;

	switch(unProb.Fonction)
	{
		case ALPINE: //Alpine: Min 0 en (0,0 ... 0)
				for(d=0; d<unProb.D; d++) valeur = valeur + fabs(Pos.X[d]*(sin(Pos.X[d])+0.1));
				break;
		case BANANE: //Rosenbrock ou fonction Banane. Min 0 en (1, ... 1)
				for(d=0; d<unProb.D-1; d++)
				{
					xd = 1-Pos.X[d];
					valeur += xd*xd;
					xd = Pos.X[d]*Pos.X[d]-Pos.X[d+1];
					valeur += 100*xd*xd;
				}
				break;

		// % modif % 
		case EGGHOLDER: // Fonction coquetier/boite d'oeufs. 
				//Optimun global -959.6407 en (512, 404.2319)			
				xd = -(Pos.X[1] + 47);
				som1 += xd*sin(sqrt(fabs(Pos.X[1] + Pos.X[0]/2 + 47)));
				som2 += Pos.X[0]*sin(sqrt(fabs(Pos.X[0] + xd)));
				valeur += som1 - som2;
				break;

		case MAXCUT: // Fonction maxcut : Calcul de la coupe maximale dans le graphe
				/*
				Pseudo code la fonction objectif
					Pour chaque noeud (dimension) <d> de ma solution
						Pour chaque Arc <a> du probl�me
							si <d> est la source de l'arc <a> alors :
								si le noeud <d'> destination de cet arc � une position diff�rente de <d> alors :
									Ajout du poids de l'arc � la solution
								fsi
							fsi
						fpour
					fpour
				*/
				for(d=0; d<unProb.D-1; d++){				
					vector<tArc>::iterator current_arc;
					for(current_arc = unProb.Arc.begin(); current_arc < unProb.Arc.end(); current_arc++){				

						if (current_arc->Ni == d){
							if (Pos.X[d] != Pos.X[current_arc->Nj]){							
								valeur += current_arc->Poids;
							}
						}
					}			
				}
				break;
		
		// % modif end %
		default: valeur = FLT_MAX;
	}
	Pos.FctObj = valeur;
	unPSO.CptEval ++;
}

//-----------------------------------------------------------------------
void DeplacerEssaim(std::vector<tParticule> &unEssaim, tProblem unProb, tPSO &unPSO, tPosition &Meilleure)
{
	int i;
	//D�placement de l'essaim pour chaque particule----------------------
	for(i=0; i<unPSO.Taille; i++)
	{
		DeplacerUneParticule(unEssaim[i], unProb, unPSO);
		
		//�valuation de la nouvelle position-----------------------------
		EvaluationPosition(unEssaim[i].Pos, unProb, unPSO);
		
		//Mise � jour de la meilleure position de la particule-----------		

		// % modif % //
		// Prise en compte du cas MAXCUT : maximiser la meilleure solution
		if (unProb.Fonction == MAXCUT){			
			if(unEssaim[i].Pos.FctObj >= unEssaim[i].BestPos.FctObj){
				unEssaim[i].BestPos = unEssaim[i].Pos;			

				//M�morisation du meilleur r�sultat atteint jusqu'ici pour MAXCUT ----
				if(unEssaim[i].BestPos.FctObj > Meilleure.FctObj)
					Meilleure = unEssaim[i].BestPos;
			}
		}
		else{
			// Cas normal : minimser
			if(unEssaim[i].Pos.FctObj <= unEssaim[i].BestPos.FctObj)
			{
				unEssaim[i].BestPos = unEssaim[i].Pos;			
				
				//M�morisation du meilleur r�sultat atteint jusqu'ici------------
				if(unEssaim[i].BestPos.FctObj < Meilleure.FctObj)
					Meilleure = unEssaim[i].BestPos;
			}
		}
		// % end modif % //
	}
}

//-----------------------------------------------------------------------
//D�placement d'une seule particule
void DeplacerUneParticule(tParticule &Particule, tProblem unProb, tPSO &unPSO)
{
	tParticule* MeilleureInfo;
	int d;

	//Meilleure informatrice de la particule-----------------------------
	MeilleureInfo = TrouverMeilleureInformatrice(Particule, unPSO);

	// % modif % //	
	// Cas pour la mise a jour de particule sur Maxcut
	if (unProb.Fonction == MAXCUT){
		
		vector<double> sigmoide;
		sigmoide.resize(unProb.D);

		// Calcul de la nouvelle vitesse + stockage de la sigmoide associ�e
		for(d=0; d<unProb.D; d++){
				Particule.V[d] =	unPSO.C1 * Particule.V[d] + 
								AleaDouble(0,unPSO.C2) * (Particule.BestPos.X[d] - Particule.Pos.X[d]) + 
								AleaDouble(0,unPSO.C3) * (MeilleureInfo->BestPos.X[d] - Particule.Pos.X[d]);

				// Confinement de la vitesse sur l'intervalle [-4; 4]
				if (Particule.V[d] < -4.0){
					Particule.V[d] = -4.0;
				}
				if (Particule.V[d] > 4.0){
					Particule.V[d] = 4.0;
				}

				// Calcul de la fonction sigmoidale associ�e � la vitesse
				double sigmo_tmp = 1.0 / (1.0 + exp(-Particule.V[d]));
				sigmoide[d] = sigmo_tmp;
		}

		// MAJ de la position de la particule :
		for(int d=0; d<unProb.D; d++)
		{
			double proba = AleaDouble(0, 1); // R�cup�ration d'une probabilit� entre 0 et 1

			// Comparaison de cette probabilit� avec la valeur de la fonction sigmoidale associ�e sur la dimension d
			if ( proba < sigmoide[d]){
				Particule.Pos.X[d] = 1;	// Probabilit� valid�e
			}else{
				Particule.Pos.X[d] = 0; // Probabilit� non suffisante
			}
		}
	}
	else{
		// Cas normal : fonction aux variables continues
		//Calcul de la nouvelle vitesse--------------------------------------
		for(d=0; d<unProb.D; d++)
				Particule.V[d] =	unPSO.C1 * Particule.V[d] + 
								AleaDouble(0,unPSO.C2) * (Particule.BestPos.X[d] - Particule.Pos.X[d]) + 
								AleaDouble(0,unPSO.C3) * (MeilleureInfo->BestPos.X[d] - Particule.Pos.X[d]);

		//Mise � jour de la nouvelle position--------------------------------
		for(d=0; d<unProb.D; d++)
			Particule.Pos.X[d] += Particule.V[d];

		//Confinement d'intervalle pour la valeur des positions--------------
		for(int d=0; d<unProb.D; d++)
		{
			if(Particule.Pos.X[d] < unProb.Xmin)
			{
				Particule.Pos.X[d] = unProb.Xmin;
				Particule.V[d] = 0; //Remise � z�ro de la vitesse
			}
			if(Particule.Pos.X[d] > unProb.Xmax)
			{
				Particule.Pos.X[d] = unProb.Xmax;
				Particule.V[d] = 0; //Remise � z�ro de la vitesse
			}
		}
	}	
	// % end modif % //
}

//-----------------------------------------------------------------------
//Trouve la meilleure informatrice d'une particule
tParticule* TrouverMeilleureInformatrice(tParticule &Particule, tPSO &unPSO)
{
	int Rang;
	int k;
	double Valeur;

	Rang = 0;
	Valeur = Particule.Info[0]->BestPos.FctObj;
	for(k=1; k<unPSO.NbInfo; k++)
	{
		if(Particule.Info[k]->BestPos.FctObj < Valeur)
		{
			Valeur = Particule.Info[k]->BestPos.FctObj;
			Rang = k;
		}
	}
	return Particule.Info[Rang];
}

//**-----------------------------------------------------------------------
//**Renvoie un double al�atoire entre a et b
double AleaDouble(double a, double b)
{
	double resultat = (double)rand()/RAND_MAX;
	return (resultat*(b-a) + a);
}

//**-----------------------------------------------------------------------
//DESCRIPTION: Fonction qui affiche le d�tail des solutions (de Debut jusqu'a Fin-1) de l'essaim
void AfficherSolutions(std::vector<tParticule> unEssaim, int Debut, int Fin, int Iter, tProblem unProb)
{
	int i,j;
	cout << "AFFICHAGE ESSAIM (Iteration #" << Iter << ")" << endl;
	for (i=Debut; i<Fin; i++)
	{
		cout << "SOL: " << i << "\t";
		for(j=0; j<unProb.D; j++)
		{
			cout << setprecision(6) << setw(10) << unEssaim[i].Pos.X[j];
		}
		cout <<  endl << "\tFctObj: " << setw(10) << unEssaim[i].Pos.FctObj;
		cout << endl;
		cout << "BEST POS: " << i << "\t";
		for(j=0; j<unProb.D; j++)
		{
			cout << setprecision(6) << setw(10) << unEssaim[i].BestPos.X[j];
		}
		cout <<  endl << "\tFctObj: " << setw(10) << unEssaim[i].BestPos.FctObj;
		cout << endl;
	}
}

//**-----------------------------------------------------------------------
//Fonction qui affiche le d�tail d'une solution
void AfficherUneSolution(tPosition Pos, int Iter, tProblem unProb)
{
	int j;
	cout << "ITER #" << Iter << " \t";

	cout << "SOL: ";
	for(j=0; j<unProb.D; j++)
	{
		cout << setprecision(6) << setw(10) << Pos.X[j] << ' ';
	}
	cout <<  "\tFctObj: " << setw(10) << Pos.FctObj;
	cout << endl;
}

//**-----------------------------------------------------------------------
//DESCRIPTION: Fonction affichant les r�sultats du PSO
void AfficherResultats (tPosition uneBest, tProblem unProb, tPSO unPSO)
{
	cout.setf(ios::fixed|ios::showpoint);
	cout << endl << endl;
	cout << "===============================================================================" <<endl;
	cout << "                                    RESULTATS FINAUX" << endl;
	cout << "===============================================================================" <<endl;
	cout << "FONCTION: " ;

	// % modif % //
	/* 
	Permet le stockage en fichier des r�sultats d'un appel � la fonction run
	Analyse de ce fichier de r�sultats par la suite, dans la fonction analyse(...)
	*/
	ofstream myfile;
	
	switch (unProb.Fonction)
	{
		//Chaque type de fonction �crit dans un fichier sp�cifique
		case ALPINE: cout << "ALPINE"; myfile.open("alpine_res.txt", ios_base::app); break;
		case BANANE: cout << "BANANE"; myfile.open("banane_res.txt", ios_base::app); break;
		case EGGHOLDER: cout << "EGGHOLDER"; myfile.open("eggholder_res.txt", ios_base::app); break;
		case MAXCUT: cout << "MAXCUT"; myfile.open("maxcut_res.txt", ios_base::app);break;
		default: cout << " a definir...";
	}

	//console display
	cout << endl; 
	cout << "PARAMETRES" << endl;
	cout << "Taille : " << unPSO.Taille << "\tC1: " << setprecision(6) << unPSO.C1 
		 << "\tC2: " << setprecision(6) << unPSO.C2 << "\tC3: " << setprecision(6) << unPSO.C3 
		 << "\tNbInfo: " << setprecision(2) << unPSO.NbInfo << endl; 
	cout << "===============================================================================" << endl; 
	cout << "Nombre d iterations effectuees : " << unPSO.Iter << endl;
	cout << "Nombre Total d'evaluations : " << unPSO.CptEval << "/" << unPSO.NB_EVAL_MAX << endl;
	cout << "Meilleure solution trouvee: "  << setprecision(6) << uneBest.FctObj << endl; 
	cout << endl << "===============================================================================" << endl;

	//Ecriture dans le fichier de r�sultat de l'ex�cution
	myfile << setprecision(6) << uneBest.FctObj << endl; 
	myfile.close();
	// % modif end % //
}

//**-----------------------------------------------------------------------
//**Liberation de la m�moire allou�e dynamiquement
void LibererMemoireFinPgm (std::vector<tParticule> &unEssaim, tProblem unProb, tPSO unPSO)
{
	int i;

	for (i=0; i<unPSO.Taille; i++)
	{
		unEssaim[i].Pos.X.clear();
		unEssaim[i].BestPos.X.clear();
		unEssaim[i].V.clear();
		unEssaim[i].Info.clear();
	}
	unEssaim.clear();
}


//**MAXCUT
//**-----------------------------------------------------------------------
//**Lecture du Fichier probleme MAXSAT et initialiation de la structure Problem
//**NB: Lors de la lecture, le num�ro des noeuds est transform� (0 � NbNoeud-1 AU LIEU de 1 � NbNoeud)
void LectureProblemeMAXCUT(std::string FileName, tProblem & unProb, tPSO &unPSO)
{
	ifstream	Fichier;
	int x, i, j;

	unProb.Nom = FileName;
	Fichier.open(unProb.Nom.c_str(), ios::in) ;
	if (Fichier.fail())
		cout << "Erreur � l'ouverture" << endl ;
	else
	{
		Fichier >> unProb.NbNoeud >> unProb.NbArc; // Lecture du nombre de noeuds (dimension) et du nombre d'arcs
		unProb.D = unProb.NbNoeud;

		//Allocation dynamique en fonction de la taille du probleme	
		unProb.Arc.resize(unProb.NbArc);	
		
		for (x=0; x<unProb.NbArc; x++)
		{
			Fichier >> i >> j >> unProb.Arc[x].Poids;
			unProb.Arc[x].Ni = i-1; //Transformation du num�ro des arcs : 0 � NbNoeud-1  (au lieu de 1 � NbNoeud)
			unProb.Arc[x].Nj = j-1;
		}
		
		Fichier.close() ;
		if (Fichier.fail())
			cout << "Erreur � la fermeture" << endl ;
	}
}
//**-----------------------------------------------------------------------
//Fonction d'affichage � l'�cran permettant de voir si les donn�es du fichier probl�me ont �t� lues correctement
void AfficherProblemeMAXCUT (tProblem unProb)
{
	int i;
	cout << "*******************************************************" << endl;
	cout << "NOM FICHIER:\t"		<<  unProb.Nom	<< endl;
	cout << "NOMBRE NOEUDS:\t"		<<  unProb.NbNoeud	<< endl;
	cout << "NOMBRE ARCS:\t"		<<  unProb.NbArc << endl;
	cout << endl << "ARC: ";
	for (i=0; i<unProb.NbArc; i++)
	{
		cout << endl << "#" << i << ": ";
		cout << "\t" << unProb.Arc[i].Ni << "\t" << unProb.Arc[i].Nj;
		cout << "\tPoids= " << setw(2) << unProb.Arc[i].Poids;
	}
	cout << endl << "*******************************************************";
	cout << endl << endl;
}
