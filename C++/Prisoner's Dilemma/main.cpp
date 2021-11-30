//Prisoner's Dilemma
//Paweł Mika & Ryszard Kałuziński
//gr. GKiO1

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//Moves are stored as booleans to save the space
// "TRUE" is Cooperate (1)
// "FALSE" is Defect (0)

int winner_points = 5;
int both_cooperate_points = 3;
int both_betray_points = 2;
int loser_points = 0;

int enemy_strategy = 0; //0 - Random
                        //1 - Always Cooperate
                        //2 - Always Defect
                        //3 - Tit for Tat (First Cooperate)
                        //4 - Suspicious Tit for Tat (First Defect)
                        //5 - Imperfect TFT (Imitates opponent's last move with high, but less than one, probability)
                        //6 - Pavlov (Cooperates if it and its opponent moved alike in previous move and defects if they moved differently)

int population_size = 100; //Should be even
int turns_in_a_game = 1000;
int generations_to_do = 100;



//Initialize solution table with random tactic
void randomize_table(bool* table)
{
    for(int i=0;i<turns_in_a_game;i++) //For every move in tactic randomize it
    {
        if(rand()%2==0)
            table[i]= true; //Cooperate
        else
            table[i]=false; //Defect
    }
}

//Calculate the points of 2 players basing on their moves and the reward values
void calculate_points(bool* player1,bool* player2,int &player1score, int &player2score)
{
    player1score = 0;
    player2score = 0;
    for(int i=0; i<turns_in_a_game; i++)
    {
        if(player1[i]==true && player2[i]==true) //Both players cooperate
        {
            player1score+=both_cooperate_points;
            player2score+=both_cooperate_points;
        }

        else if(player1[i]==true && player2[i]==false) //First player cooperates, second defects
        {
            player1score+=loser_points;
            player2score+=winner_points;
        }
        else if(player1[i]==false && player2[i]==true) //First player defects, second cooperates
        {
            player1score+=winner_points;
            player2score+=loser_points;
        }
        else if(player1[i]==false && player2[i]==false) //Both players defect
        {
            player1score+=both_betray_points;
            player2score+=both_betray_points;
        }
    }
}

//Calculate what the enemy is doing
void calculate_enemy_strategy(bool **enemy, bool **player)
{
    if(enemy_strategy == 0) //Random
    {
        bool enemy_table[turns_in_a_game];
        randomize_table(enemy_table);

        for(int i=0;i<population_size;i++)
        {
            for(int j=0;j<turns_in_a_game;j++)
            {
                enemy[i][j]=enemy_table[j];
            }
        }
    }
    else if(enemy_strategy == 1) //Always Cooperate
    {
        for(int i=0;i<population_size;i++)
        {
            for(int j=0;j<turns_in_a_game;j++)
            {
                enemy[i][j]=true;
            }
        }

    }
    else if(enemy_strategy == 2) //Always Betray
    {
        for(int i=0;i<population_size;i++)
        {
            for(int j=0;j<turns_in_a_game;j++)
            {
                enemy[i][j]=false;
            }
        }
    }
    else if(enemy_strategy == 3) //Tit for Tat
    {
        for(int i=0;i<population_size;i++)
        {
            enemy[i][0]=true;

            for(int j=1;j<turns_in_a_game;j++)
            {
                enemy[i][j]=player[i][j-1];
            }
        }
    }
    else if(enemy_strategy == 4) //Suspicious Tit for Tat
    {
       for(int i=0;i<population_size;i++)
        {
            enemy[i][0]=false;

            for(int j=1;j<turns_in_a_game;j++)
            {
                enemy[i][j]=player[i][j-1];
            }
        }
    }
    else if(enemy_strategy == 5) //Imperfect TFT
    {
        for(int i=0;i<population_size;i++)
        {
            enemy[i][0]=true;

            for(int j=1;j<turns_in_a_game;j++)
            {
                if(rand()%10>1)
                    enemy[i][j]=player[i][j-1];
                else
                {
                    enemy[i][j]=!player[i][j-1];
                }
            }
        }
    }
    else if(enemy_strategy == 6) //Pavlov
    {
        for(int i=0;i<population_size;i++)
        {
            enemy[i][0]=true;

            for(int j=1;j<turns_in_a_game;j++)
            {
                if(enemy[i][j-1]==player[i][j-1])
                    enemy[i][j]=true;
                else
                    enemy[i][j]=false;
            }
        }
    }
}

//Calculate enemy strategy for the final battle
void enemy_for_final_battle(bool *enemy, bool* player)
{
    if(enemy_strategy == 0) //Random
    {
        bool enemy_table[turns_in_a_game];
        randomize_table(enemy_table);

        for(int i=0;i<turns_in_a_game;i++)
        {
            enemy[i]=enemy_table[i];
        }
    }
    else if(enemy_strategy == 1) //Always Cooperate
    {
        for(int i=0;i<turns_in_a_game;i++)
        {
            enemy[i]=true;
        }

    }
    else if(enemy_strategy == 2) //Always Betray
    {
        for(int i=0;i<turns_in_a_game;i++)
        {
            enemy[i]=false;
        }
    }
    else if(enemy_strategy == 3) //Tit for tat
    {
        enemy[0]=true;
        for(int i=1;i<turns_in_a_game;i++)
        {
            enemy[i]=player[i-1];
        }
    }
    else if(enemy_strategy == 4) //Suspicious Tit for Tat
    {
       enemy[0]=false;
        for(int i=1;i<turns_in_a_game;i++)
        {
            enemy[i]=player[i-1];
        }
    }
    else if(enemy_strategy == 5) //Imperfect TFT
    {
        enemy[0]=true;
        for(int i=1;i<turns_in_a_game;i++)
        {
            if(rand()%10>1)
                enemy[i]=player[i-1];
            else
            {
                enemy[i]=!player[i-1];
            }
        }
    }
    else if(enemy_strategy == 6) //Pavlov
    {
        enemy[0]=true;

        for(int i=1;i<turns_in_a_game;i++)
        {
            if(enemy[i-1]==player[i-1])
                enemy[i]=true;
            else
                enemy[i]=false;
        }
    }
}

//Find_best_strategy
void find_best_strategy(bool *best_strategy, int& generations_to_learn)
{
    int generations_done=0;//Count how many generations there were

    srand(time(NULL));

    bool **player; //Table for finding the best strategy
    bool **playertemp; //Table to keep better half temporarily
    bool **enemy; //Table for enemies moves in response to player's moves
    player = new bool *[population_size]; //Table is the size of the population
    playertemp = new bool *[population_size/2]; //Table is the size of the half of the population (needs space only for "better" half)
    enemy = new bool *[population_size]; //Table is the size of the population

    int points[2][population_size]; //Table for difference of player's and enemy's points and the amount of player's points

    for(int i=0;i<population_size;i++)
    {
        player[i] = new bool [turns_in_a_game]; //Every member of the population has space for every move
        enemy[i] = new bool [turns_in_a_game]; //Every member of the population has space for every move
    }
    for(int i=0;i<population_size/2;i++)
    {
        playertemp[i] = new bool [turns_in_a_game]; //Every member of the population has space for every move
    }

    for(int i=0;i<population_size;i++)
    {
        randomize_table(player[i]); //Randomize the table of the tactics to have something to begin with
    }

    //Calculate, what the enemy is going to do
    calculate_enemy_strategy(enemy, player);

    //Population cycle
    while(generations_to_learn>0) //While there is still time to learn
    {
        //Calculate the points of every player's and enemy's member
        int p,e,d; //player's, enemy's and difference of the points
        for(int i=0;i<population_size;i++)
        {
            calculate_points(player[i],enemy[i],p,e); //Calculate points of every member of the population
            d=p-e; //Calculate the difference of the points
            points[0][i]=d; //Save the difference
            points[1][i]=p; //and the player's points to be used in tournament comparisons
        }

        //Pick half of the population by binary tournament
        for(int i=0;i<population_size/2;i++)
        {
            if(points[0][i]>points[0][i+population_size/2]) //First wins by greater difference
            {
                for(int j=0;j<turns_in_a_game;j++)
                {
                    playertemp[i][j] = player[i][j]; //Pick and save i-th member strategy
                }
            }
            else if(points[0][i]==points[0][i+population_size/2] && points[1][i]>=points[1][i+population_size/2])
                                                                                    //First wins by having greater score
            {                                                                       //(or is the same and can choose either)
                for(int j=0;j<turns_in_a_game;j++)
                {
                    playertemp[i][j] = player[i][j]; //Pick and save i-th member strategy
                }
            }
            else //Second one wins by having greater difference or more points
            {
                for(int j=0;j<turns_in_a_game;j++)
                {
                    playertemp[i][j] = player[i+population_size/2][j]; //Pick and save (i+population_size/2)-th member strategy
                }
            }
        }

        //Now put them back into the original population
        for(int i=0;i<population_size/2;i++)
        {
            for(int j=0;j<turns_in_a_game;j++)
            {
                player[i][j]=playertemp[i][j];
            }
        }

        //Now create another half of the population randomly mixing the members of the winning half
        for(int i=population_size/2;i<population_size;i++)
        {
            int parent1 = rand()%50;
            int parent2 = rand()%50;
            for(int j=0;j<turns_in_a_game;j++)//For every move decide from which parent it is taken
            {
                if(rand()%2==0) //This move is taken from first parent
                {
                    player[i][j]=player[parent1][j];
                }
                else //This move is taken from the second parent
                {
                    player[i][j]=player[parent2][j];
                }
            }
            //Randomize one cell
            int cell_to_change = rand()%turns_in_a_game;
            if(player[i][cell_to_change]==true)
                player[i][cell_to_change]=false;
            else
                player[i][cell_to_change]=true;
        }


        //Calculate, what the enemy is going to do
        calculate_enemy_strategy(enemy, player);

        //Now a new population is ready for the next cycle
        generations_to_learn--;
        generations_done++;

        if(generations_done%100==0)
        {
            std::cout<<generations_done<<". generations done. "<<std::endl; //Inform the user about how many generations are done
        }
    }

    //Pick the best strategy after all the learning
    for(int i=0;i<turns_in_a_game;i++)
        best_strategy[i]=player[0][i];
    int best_difference,best_points,initialb,initiale; //Start with the first strategy in the table
    calculate_points(best_strategy,enemy[0],initialb,initiale);
    best_difference=initialb-initiale;
    best_points=initialb;

    //Compare strategy to every member of population and choose the best
    //(biggest difference & highest score)
    for(int i=1;i<population_size;i++)
    {
        int p,e,d; //player's, enemy's and difference of the points
        calculate_points(player[i],enemy[i],p,e);
        d=p-e;

        if(d>best_difference) //Change the strategy if the difference is better with the new one
        {
            for(int j=0;j<turns_in_a_game;j++)
            {
                best_strategy[j]=player[i][j];
                best_difference=d;
                best_points=p;
            }
        }
        else if(d==best_difference && p>best_points) //Change the strategy if the differences are the same, but the new one is higher score
        {
            for(int j=0;j<turns_in_a_game;j++)
            {
                best_strategy[j]=player[i][j];
                best_difference=d;
                best_points=p;
            }
        }
        //Else don't change, cause the strategy is the same or worse
    }

    //Free the space
    for(int i=0;i<population_size;i++)
    {
        delete[] player[i];
        delete[] enemy[i];
    }
    for(int i=0;i<population_size/2;i++)
    {
        delete[] playertemp[i];
    }
    delete[]player;
    delete[]enemy;
    delete[]playertemp;
}

int main()
{
    std::cout<<"Welcome! Insert the size of the learning population (should be even): ";
    std::cin>>population_size;
    std::cout<<std::endl<<std::endl;

    std::cout<<"Now tell me how many turns there are in one game: ";
    std::cin>>turns_in_a_game;
    std::cout<<std::endl<<std::endl;

    std::cout<<"Now tell me how many generations there are to learn the strategy: ";
    std::cin>>generations_to_do;
    std::cout<<std::endl<<std::endl;

    std::cout<<"0 - Random"<<std::endl;
    std::cout<<"1 - Always Cooperate"<<std::endl;
    std::cout<<"2 - Always Defect"<<std::endl;
    std::cout<<"3 - Tit for Tat"<<std::endl;
    std::cout<<"4 - Suspicious Tit for Tat"<<std::endl;
    std::cout<<"5 - Imperfect Tit for Tat"<<std::endl;
    std::cout<<"6 - Pavlov"<<std::endl<<std::endl;
    std::cout<<"Final question. Tell me which strategy does the enemy choose by inserting it's number: ";
    std::cin>>enemy_strategy;
    system("CLS");

    bool best_strategy[turns_in_a_game];
    bool enemy_final[turns_in_a_game];

    find_best_strategy(best_strategy,generations_to_do);

    for(int i=0;i<turns_in_a_game;i++)
    {
        std::cout<<"The best strategy move "<<i+1<<". is ";
        if(best_strategy[i]==true)
            std::cout<<"to cooperate."<<std::endl;
        else
            std::cout<<"to defect."<<std::endl;
    }

    enemy_for_final_battle(enemy_final,best_strategy);
    int p,e,d;
    calculate_points(best_strategy,enemy_final,p,e);
    d=p-e;

    std::cout<<"Player score: "<<p<<". Enemy score: "<<e;
    if(d>0)
    {
        std::cout<<". Player wins by "<<d<<" points."<<std::endl;
    }
    else if(d==0)
    {
        std::cout<<". It is a draw!"<<std::endl;
    }
    else
    {
        std::cout<<". Enemy wins by "<<-d<<" points."<<std::endl;
    }


    return 0;
}
