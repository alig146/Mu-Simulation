#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <stdlib.h> // srand, rand

using namespace std;

int main(int argc, char *argv[]) {

	ifstream eData;
	eData.open("muon5body_data.csv");
			
	//Declaring Vars
	string e1x, e1y, e1z, e2x, e2y, e2z, e3x, e3y, e3z;
	vector <float> p1x;
	vector <float> p1y;
	vector <float> p1z;
	vector <float> p2x;
	vector <float> p2y;
	vector <float> p2z;
	vector <float> p3x;
	vector <float> p3y;
	vector <float> p3z;
	
	//number of lines
	int i = 0;

	if (eData.is_open()) //if the file is open
	{
		//ignore first line
		string line;
		getline(eData, line);

		while (eData.peek() != EOF) //while the end of file is NOT reached
		{
			//I have 9 sets {e1x, e1y, e1z, e2x, e2y, e2z, e3x, e3y, e3z} so use 9 getlines
			getline(eData, e1x, ',');
			p1x.push_back(stof(e1x));
			getline(eData, e1y, ',');
			p1y.push_back(stof(e1y));
			getline(eData, e1z, ',');
			p1z.push_back(stof(e1z)); //electron 1 momenta
			getline(eData, e2x, ',');
			p2x.push_back(stof(e2x));
			getline(eData, e2y, ',');
			p2y.push_back(stof(e2y));
			getline(eData, e2z, ',');
			p2z.push_back(stof(e2z)); //electron 2 momenta
			getline(eData, e3x, ',');
			p3x.push_back(stof(e3x));
			getline(eData, e3y, ',');
			p3y.push_back(stof(e3y));
			getline(eData, e3z, '\n'); //new line after z-momentum of the third electron
			p3z.push_back(stof(e3z)); //electron 3 momenta

			i += 1; //increment number of lines
		}
		eData.close(); // closing the file
		if (argc == 1) // non-randomized output
		{
			std::cout << '\n' << "Non-random electron sample:" << '\n';
			std::cout << "e1 px" << "\t" << "\t" << "e1 py" << "\t" << "\t" << "e1 pz" << "\t" << "\t" << "e2 px" << "\t" << "\t" << "e2 py" << "\t" << "\t" << "e2 pz" << "\t" << "\t" << "e3 px" << "\t" << "\t" << "e3 py" << "\t" << "\t" << "e3 pz" << endl;
			std::cout << p1x[0] << "\t" << p1y[0] << "\t" << p1z[0] << "\t" << p2x[0] << "\t" << p2y[0] << "\t" << p2z[0] << "\t" << p3x[0] << "\t" << p3y[0] << "\t" << p3z[0] << '\n' << endl;
		}
		else if (argc > 1)
		{
			string randomFlag = "rn"; // flags
			string theBank = "bank";
			string howTo = "usage";
			
			// randomized output
			if (argv[1] == randomFlag) {
				std::cout << '\n' << "Random electron sample:" << endl;
				srand(time(NULL));
				int electronSample = rand() % 10000 + 0; //generates random number from 0 to 10,000 (total of 10,001)
				
				std::cout << "e1 px" << "\t" << "\t" << "e1 py" << "\t" << "\t" << "e1 pz" << "\t" << "\t" << "e2 px" << "\t" << "\t" << "e2 py" << "\t" << "\t" << "e2 pz" << "\t" << "\t" << "e3 px" << "\t" << "\t" << "e3 py" << "\t" << "\t" << "e3 pz" << endl;
				std::cout << p1x[electronSample] << "\t" << p1y[electronSample] << "\t" << p1z[electronSample] << "\t" << p2x[electronSample] << "\t" << p2y[electronSample] << "\t" << p2z[electronSample] << "\t" << p3x[electronSample] << "\t" << p3y[electronSample] << "\t" << p3z[electronSample] << '\n' << endl;
			}

			// Printing the electron bank
			else if (argv[1] == theBank) {
				std::cout << '\n' << "e1 px" << "\t" << "\t" << "e1 py" << "\t" << "\t" << "e1 pz" << "\t" << "\t" << "e2 px" << "\t" << "\t" << "e2 py" << "\t" << "\t" << "e2 pz" << "\t" << "\t" << "e3 px" << "\t" << "\t" << "e3 py" << "\t" << "\t" << "e3 pz" << endl;
				for (int j = 0; j < i; j++)
				{
					std::cout << p1x[j] << "\t" << p1y[j] << "\t" << p1z[j] << "\t" << p2x[j] << "\t" << p2y[j] << "\t" << p2z[j] << "\t" << p3x[j] << "\t" << p3y[j] << "\t" << p3z[j] << endl;
				}
				std::cout << endl;
			}
			
			// help menu
			else if (argv[1] == howTo) {
				std::cout << "-----------------------------------------------------------------------------------------------------" << endl;
				std::cout << '\n' <<"e1 is positivly charged, e2 and e3 are the negatively charged electrons. x,y,z are momenta directions." << '\n' << endl;
				std::cout << "The bank of decay electron momenta currently has " << p1x.size() << " events in it." << '\n' << endl;
				std::cout << "Flags/Options: " << '\n' << "rn" << '\t' << "For a random set of electron momenta." << '\n' << "bank" << '\t' << "To print the entire 10,000 bank of electron momenta." << '\n' << "usage" << '\t' << "Brings up this help section." << '\n' << endl;
				std::cout << "With no flag, program will return a non-random set of electron momenta." << '\n' << endl;
				std::cout << "-----------------------------------------------------------------------------------------------------" << endl;
			}

			//user mistake
			else
			{
				std::cout << "Unknown flag used" << endl;
			}

		}
	}
		else std::cout << "Error: Unable to open file.  Make sure it is in the same directory or explicitly specify directory of file"; //if the file is not open output

}