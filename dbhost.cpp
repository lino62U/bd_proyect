#include "iostream"
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <list>
#include <bits/stdc++.h>
using namespace std;

void show_datas(string atributos, string relation, string headers)
{
    ifstream MyReadFile(relation);
    string tuples;
    // getting header
    size_t pos_in,pos_en;
    int it = 1;
    while ((pos_en = headers.find('#')) != std::string::npos) {
        headers.replace(pos_en, 1, ",");
        if(it%2==0)
            headers.replace(pos_in, pos_en-pos_in+1, ",");    
        pos_in=pos_en;
        it++;
    }
    headers.erase(pos_in, string::npos);
    
    //showing all
    if(atributos=="*")
    {
        //headers
        while ((pos_en = headers.find(',')) != std::string::npos) {
            headers.replace(pos_en, 1, "\t");
        }
        cout<<headers<<endl;
        //getting data of file
        while (getline (MyReadFile, tuples)) {
            size_t pos;
            while ((pos = tuples.find('#')) != std::string::npos) {
                tuples.replace(pos, 1, "\t");
            }
            cout<<tuples<<endl;
        }
        return;
    }

    // show some columns
    vector<string> atributos_headers;
    vector<string> tuples_data;
    vector<string> atributos_user;    

    int st,en;
    while ((en = headers.find(',')) != std::string::npos) {
        atributos_headers.push_back(headers.substr(0,en));
        headers.erase(0, en+1);
    }
    atributos_headers.push_back(headers);

    while ((en = atributos.find(',')) != std::string::npos) {
        atributos_user.push_back(atributos.substr(0,en));
        atributos.erase(0, en+1);
    }
    atributos_user.push_back(atributos);
    for(int i=0; i<atributos_user.size();i++)
        cout<<atributos_user[i]<<"\t";

    cout<<endl;
    while (getline (MyReadFile, tuples)) {
        size_t pos;
        while ((pos = tuples.find('#')) != std::string::npos) {
            tuples_data.push_back(tuples.substr(0,pos));
            tuples.erase(0, pos+1);
        }
        tuples_data.push_back(tuples);

        for (int i = 0; i < atributos_user.size(); i++) {
            for(int j=0; j<atributos_headers.size();j++)
            {
                if(atributos_user[i]==atributos_headers[j])
                   cout<<tuples_data[j]<<"\t";
            }
        }
        cout<<endl;
        tuples_data.clear();

    }
    atributos_headers.clear();
    tuples_data.clear();
    atributos_user.clear();
    MyReadFile.close();
}
void checking (string &input)
{
    ifstream MyReadFile("scheme.txt");
    string tuples;
    string headers="";

    if (input[0]!='&' && input[input.size()-1]!='#')
        return;
    
    //key words start positions
    int slc = input.find("SELECT") + 6;
    int frm = input.find("FROM");
    int where = input.find("WHERE");

    // users inputs
    string atrb;
    string nm_rlt;
    string conditions;
    atrb = input.substr(slc, frm - slc);
    atrb.erase(remove_if(atrb.begin(), atrb.end(), ::isspace), atrb.end());
    nm_rlt = input.substr(frm + 4, input.size() - frm -5);
    nm_rlt.erase(std::remove_if(nm_rlt.begin(), nm_rlt.end(), ::isspace), nm_rlt.end());
    
    int found_rlt;
    while (getline (MyReadFile, tuples)) {
        headers = tuples.substr(tuples.find('#')+1 , tuples.size());
        //check relation
        found_rlt  = tuples.find(nm_rlt);
        if (found_rlt != std::string::npos)
        {
            //check atributes
            if(atrb=="*")
            {
                show_datas(atrb, nm_rlt.append(".txt"), headers);
                return;
            }
            
            stringstream atb_in(atrb);
            string atb_input;
        
            while (getline(atb_in, atb_input, ','))
            {
                int found_atrb = tuples.find(atb_input);
                if (found_atrb == std::string::npos)
                    return;
            }
            show_datas(atrb, nm_rlt.append(".txt"), headers);
        }  
    }
    if (found_rlt == std::string::npos)
        cout<<"Relation not found"<<endl;
    MyReadFile.close();
   // cout<<"Hellos world"<<endl;    
}

int main()
{
    string consulta ="&SELECT * FROM depts#";
    checking(consulta);
    
}