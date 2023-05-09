#include "iostream"
#include "fstream"
using namespace std;

struct palabras_Reservadas
{
    char select[6] = {'S','E','L','E','C','T'};
    char create[6] = {'C','R','E','A','T','E'};
    char insert[6] = {'I','N','S','E','R','T'};
    char from[4] = {'F','R','O','M'};
    char where[5]= {'W','H', 'E','R','E'};
    char operadores[8] = {'*','=','+','-','<','>','/'};
    char simbolos_separacion[6] = {'&','|','(',')',','};

    char find_operacion (char *w, int length)
    {
        bool c,s,i=false;
        for (int j = 0; j < length; j++)
        {
            if(select[j] == w[j])  
            {
                s=true;
                
            }
            else if(create[j] == w[j])  c=true;
            else if(insert[j] == w[j])  i=true;
        }

        if(c) return 'c';
        if(s) return 's';
        if(i) return 'i';
        return '0';
    }
};

void create(string query)
{
    ofstream archivo;
    archivo.open("scheme.txt", fstream::app);
    archivo<<"\n";

    int lengh_char = 2;
    int i=0;
    char *token = new char[lengh_char];
    string aux ="TABLE";
    int start = query.find(aux);

    if(start>-1)
    {
        for(int k=start + aux.length()+1; k<query.length()+1;k++ ){
            if(query[k] == ' '){
                string aux2="";
                for (size_t m = 0;  m< i; m++)
                {
                    if( token[m]=='(' || token[m]==',' || token[m]==')')
                        continue;
                    else                    
                        aux2+=token[m]; 
                }
                
                if(k==query.length()-2)
                    archivo<<aux2;
                else
                    archivo<<aux2<<'#';

                aux2.clear();
                lengh_char=2;
                i=0;
                delete[] token;
                token = new char[lengh_char];

            }else{ 
                token[i] = query[k];
                token[i+1] = '\0';
                i++;
                lengh_char++;
                token = (char*)realloc(token, lengh_char * sizeof(char)); // redimensionamos el token
            }
        }
    }
    
        
}
void insert(string query)
{
    ofstream archivo;
    archivo.open("scheme.txt", fstream::app);
    archivo<<"\n";

    int lengh_char = 2;
    int i=0;
    char *token = new char[lengh_char];
    string aux ="TABLE";
    int start = query.find(aux);

    if(start>-1)
    {
        for(int k=start + aux.length()+1; k<query.length()+1;k++ ){
            if(query[k] == ' '){
                string aux2="";
                for (size_t m = 0;  m< i; m++)
                {
                    if( token[m]=='(' || token[m]==',' || token[m]==')')
                        continue;
                    else                    
                        aux2+=token[m]; 
                }
                
                if(k==query.length()-2)
                    archivo<<aux2;
                else
                    archivo<<aux2<<'#';

                aux2.clear();
                lengh_char=2;
                i=0;
                delete[] token;
                token = new char[lengh_char];

            }else{ 
                token[i] = query[k];
                token[i+1] = '\0';
                i++;
                lengh_char++;
                token = (char*)realloc(token, lengh_char * sizeof(char)); // redimensionamos el token
            }
        }
    }
}
void select(string query){}



void analisis_lexico(string &query)

{
    
    int lengh_char = 2;
    int i=0;
    
    //char *token = new char[lengh_char];
    char *token = new char[lengh_char];
    char aux;
    palabras_Reservadas rv;
    for(int k=0; k<query.length();k++ ){
        if(query[k] == ' '){
      
            if (rv.find_operacion(token,i)=='c')
            {
                create(query.substr(k+1, query.length()-k));
                return;
            }
            if (rv.find_operacion(token,i)=='i')
            {

            }
            if (rv.find_operacion(token,i)=='s')
            {}
            
            lengh_char=2;
            i=0;
            delete[] token;
            token = new char[lengh_char];
        }else{ 
            token[i] = query[k];
            token[i+1] = '\0';
            i++;
            lengh_char++;
            token = (char*)realloc(token, lengh_char * sizeof(char)); // redimensionamos el token
        }

    }
    
    
    
    
}
void query(string consulta)
{
    ifstream MyReadFile("scheme.tx|t");
    
    
}

int main()
{
    //string consulta ="& SELECT * FROM depts#";
    string consulta ="& CREATE TABLE depts (id int, name str) |";
    analisis_lexico(consulta);
    
}
