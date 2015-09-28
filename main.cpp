//
//  main.cpp
//  ThreeDOFLander
//
//  Created by Sierra on 9/22/15.
//  Copyright (c) 2015 Sierra Adibi. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

class DOF{
public:
    double s, sdot, sdotdot;
    double target, direction;
    
    void initialize();
};

void DOF::initialize(){
    s = (rand()%1000)+1;
    //direction = pow(-1,rand());
    sdot = (rand()%10);
    //direction = pow(-1,rand());
    sdotdot = (rand()%10);
    
    target = 0;
}

class craft{
public:
    vector<DOF> frame;
    double mass;
    
    void initialize(int degrees);
    void printinitial();
};

void craft::initialize(int d){
    mass = 10;
    
    for(int i=0; i<d;i++){
        DOF d;
        d.initialize();
        frame.push_back(d);
    }
}

void craft::printinitial(){

    for(int i=0; i<frame.size();i++){
        cout<<setiosflags(ios::fixed) << setprecision(1)<<"Initial "<<i<<" Position: "<<frame.at(i).s;
        cout<<i<<" Target: "<<frame.at(i).target<<endl;
    }
}

double jeremy(double pos, double vel, double time, double target){
    return (pos - target)*pow(time,2);
}

double directioncheck(int x){
    double gravity;
    
    if(x==2){
        gravity = -9.81;
    }else{
        gravity =0;
    }
    
    return gravity;
}

void dynamicscalc(DOF & direction, double delt, double gravity, double m){
    double thrust = jeremy(direction.s, direction.sdot, delt, direction.target);
    double accelprev = direction.sdotdot;
    double velocityprev = direction.sdot;
    
    direction.sdotdot = gravity - thrust/m;
    direction.sdot = direction.sdot + 0.5*delt*(accelprev+direction.sdotdot);
    direction.s = direction.s + 0.5*delt*(velocityprev+direction.sdot);
    
}

void printheader(){
    cout << "Time \t X-Pos \t\t Y-Pos \t\t Z-Pos \t\t Energy" << endl;

}

void printround(ofstream & file, double time, vector<DOF> data, int degree, double E){
    cout << setiosflags(ios::fixed) << setprecision(1) << time << "\t\t";
    file << setiosflags(ios::fixed) << setprecision(1) << time << "\t\t";
    
    for(int i=0;i<degree;i++){
        cout << data.at(i).s << "\t\t";
        file << data.at(i).s << "\t\t";
    }
    
    cout << E << endl;
    file << E << endl;
}

double energycalcs(vector<DOF> directions, double m){
    double E = 0;
    for(int i=0;i<directions.size();i++){
        double g=0;
        if(i==2){
            g=9.81;
        }
        E = E + .5*m*pow(directions.at(i).sdot,2) + m*g*directions.at(i).s;
    }
    return E;
}

int main() {
    
    srand(time(NULL));
    int degreesfreedom = 3;
    double t = 0;
    double tstep = 0.1;
    double const tmax = 90;
    double g, energy;
    
    ofstream myfile;
    myfile.open("threeDOFdata.txt");

    craft lander;
    lander.initialize(degreesfreedom); //0 will be x, 1 will be y, 2 will be z
    lander.printinitial();
    printheader();
    
    
    while(t<tmax && lander.frame.at(2).s > lander.frame.at(2).target){
        for(int i=0;i<degreesfreedom;i++){
            g = directioncheck(i);
            dynamicscalc(lander.frame.at(i), tstep, g, lander.mass);
        }
        energy = energycalcs(lander.frame, lander.mass);
        t = t+tstep;
        printround(myfile, t, lander.frame, degreesfreedom, energy);
    }
    
    return 0;
}
