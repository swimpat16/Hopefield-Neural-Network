#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
using namespace std;

int P_SIZE = 50;
int V_SIZE = 100;
int NUM_RUNS = 20;

vector< vector<int> > data;
vector< vector< vector<double> > > weights;
vector<int> stable_count;
vector<double> stable_prob;

void initialize_count()
{
    stable_count.resize(P_SIZE);
    for (int p = 0; p < P_SIZE; p++)
    {
        stable_count[p] = 0;
    }
}

void calculate_probability()
{
    stable_prob.resize(P_SIZE);
    for (int p = 0; p < P_SIZE; p++)
    {
        stable_prob[p] = stable_count[p] * 1.0 / NUM_RUNS;
    }
}

void initialize_data()
{
    data.resize(P_SIZE);
    
    for (int p = 0; p < P_SIZE; p++)
    {
        data[p].resize(V_SIZE);
        
        for (int i = 0; i < V_SIZE; i++)
        {
            data[p][i] = rand() % 2;
            if (data[p][i] == 0)
                data[p][i] = -1;
        }
    }
}

void calculate_weights()
{
    weights.resize(P_SIZE);
    
    for (int p = 0; p < P_SIZE; p++)
    {
        weights[p].resize(V_SIZE);
        
        for (int i = 0; i < V_SIZE; i++)
        {
            weights[p][i].resize(V_SIZE);
            
            for (int j = 0; j < V_SIZE; j++)
            {
                weights[p][i][j] = 0.0;
                
                if (i != j)
                {
                    for (int k = 0; k <= p; k++)
                    {
                        weights[p][i][j] += data[k][i] * data[k][j];
                    }
                    
                    weights[p][i][j] /= 100.0;
                }
            }
        }
    }
}

void print_data()
{
    for (int p = 0; p < P_SIZE; p++)
    {
        for (int i = 0; i < V_SIZE; i++)
        {
            printf("%2d: %2d: %+d\n", p, i, data[p][i]);
        }
        printf("\n");
    }
}

void calculate_stability()
{
    vector<int> neural_net;
    double sum;
    bool stable;
    int count;
    
    stable_count.resize(P_SIZE);
    stable_prob.resize(P_SIZE);
    
    count = 0;
    
    for (int p = 0; p < P_SIZE; p++)
    {
        neural_net = data[p]; // step a
        stable = true;
        
        for (int i = 0; i < V_SIZE; i++)
        {
            sum = 0.0;
            for (int j = 0; j < V_SIZE; j++)
            {
               sum += (weights[p][i][j] * data[p][j]);
            }
            neural_net[i] = (sum >= 0 ? 1 : -1);
            
            if (neural_net[i] != data[p][i])
                stable = false;
                
        }
        
        if (stable)
            stable_count[p]++;
    }
}

void print_weights()
{
    for (int p = 0; p < P_SIZE; p++)
    {
        for (int i = 0; i < V_SIZE; i++)
        {
            printf("%2d: %2d: ", p, i);
            for (int j = 0; j < V_SIZE; j++)
            {
                printf("%+4.2f ", weights[p][i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void print_to_csv()
{
    ofstream outfile("out20.csv", ofstream::out);
    
    if (!outfile.is_open())
    {
        cerr << "Opening out.csv failed" << endl;
        exit(1);
    }
    
    outfile << "P, STABLE COUNT, STABLE PROBABILITY" << endl;
    
    for (int p = 0; p < P_SIZE; p++)
    {
        outfile << p << ", " << stable_count[p] << ", " << stable_prob[p] << endl;
    }
    
    outfile.close();
}

int main(int argc, char **argv)
{
    if (argc > 1)
        P_SIZE = atoi(argv[1]);
    if (argc > 2)
        V_SIZE = atoi(argv[2]);
    if (argc > 3)
        NUM_RUNS = atoi(argv[3]);
    
    srand(time(NULL));
 
    initialize_count();
    
    for(int i = 0; i < NUM_RUNS; i++)
    {
        initialize_data();
        calculate_weights();
        calculate_stability();
    	cout << i + 1 << endl;
	}
    
    calculate_probability();
    
    print_to_csv();
    
    return 0;
}
