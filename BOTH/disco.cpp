#include "iostream"
#include "vector"
#include "fstream"
#include "string.h"
using namespace std;

// typedef typename Sector*:: ptrSector;

class Sector
{
    private:
        int memory;
        char *data;
        bool state;         //  ->  if the tuple need more size
        int *ptrArray;      //  ->  save the position of the end of the tuple number of plate sector...
        string origin;
        int space;                  //  -> how many space is there respect to the memory
        int i;                      //  -> save the position of iterator in array data to continue insert data
        bool opc;                   //  -> posibility to insert data
    public:
        Sector(int _memory):memory(_memory)
        {
            
            data = new char[memory];
            state = false;
            ptrArray = nullptr;
            space =  memory;
            this->i = 0;
            opc = true;
             
        }


        char* getData() {return data;}
        void setSector(const char *indata, bool _state, int *ptr, string _origin)
        {
     
            for (; this->i < memory; this->i++)
            {

                *(data + this->i) = *(indata + this->i);  
            }

            space -= this->i;
            this->state = _state;
            if(state)
            {
                ptrArray = new int[4];
                for (size_t m = 0; m < 4; m++)
                    *(ptrArray + m) = *(ptr +m);
                
            }
            this->origin = _origin;   
            opc=false;         
        }
        bool isEmpty(){ return space>0?true:false; }
        
        friend class Disco;
        friend class Plato;
        friend class Superficie;
        friend class Pista;      
};
typedef vector<Sector *> vSector;
class Pista
{
    private:
        int numero_sectores;
        int gap;
        vSector nSectores;
        bool pistaOpc;
    public:
        Pista(int nPista): numero_sectores(nPista) {}
    friend class Disco;
    friend class Plato;
    friend class Superficie;

};
class Superficie
{
    private:
        int numero_pistas;
        vector<Pista *> nPistas;
        bool superficieOpc;
    public:
        Superficie(int npistas):numero_pistas(npistas){}
        friend class Disco;
        friend class Plato;
};
class Plato
{
    private:
        int numero_superficie;
        vector<Superficie *> nSuperficies;
        bool platoOpc;
    public:
        Plato():numero_superficie(2){nSuperficies.resize(numero_superficie);}
        friend class Disco;
};
class Disco
{
    private:
        int numero_plato;
        int numero_superficies_por_plato;
        int numero_pistas_por_superficie;
        int numero_sectores_por_pista;
        int memoria_por_sector;
        vector<Plato *> nPlatos;
        int *freeposition ;
    public:
        Disco(int nPlato, int nPistas, int nSectores, int nBytes) 
        {
            numero_plato = nPlato;
            numero_superficies_por_plato=2;
            numero_pistas_por_superficie = nPistas;
            numero_sectores_por_pista = nSectores;
            memoria_por_sector = nBytes;
            nPlatos.resize(numero_plato);
            freeposition = new int[4];
        }
        void giro()
        {
            int numero_superfice = 2;
            for (size_t i = 0; i < numero_plato; i++)
            {
                if(nPlatos[i])
                {
                    for (size_t j = 0; j < numero_superfice; j++)
                    {
                        if(nPlatos[i]->nSuperficies[j])
                        {
                            for (size_t k = 0; k < numero_pistas_por_superficie; k++)
                            {
                                if(nPlatos[i]->nSuperficies[j]->nPistas[k])
                                {
                                    for (size_t l = 0; l < numero_sectores_por_pista; l++)
                                    {
                                        if(nPlatos[i]->nSuperficies[j]->nPistas[k]->nSectores[l])
                                            cout<<nPlatos[i]->nSuperficies[j]->nPistas[k]->nSectores[l]->getData()<<endl;
                                    }
                                }
                                
                            }
                        }        
                    }
                } 
            }
        }
        void cabezal_escritura(int pPlato, int pSuperficie, int pPista, int gap_Sector, const char *dato, bool state, int *ptr, string origin)
        {
            if(!nPlatos[pPlato]){ nPlatos[pPlato] = new Plato ();}
            
            if(!nPlatos[pPlato]->nSuperficies[pSuperficie])
            {
                nPlatos[pPlato]->nSuperficies[pSuperficie]  = new Superficie(numero_pistas_por_superficie);
                nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas.resize(numero_pistas_por_superficie);
            }
            
            if (!nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista])
            {
                nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista] = new Pista(numero_sectores_por_pista);
                nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista]->nSectores.resize(numero_sectores_por_pista);
            }

            nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista]->nSectores[gap_Sector] = new Sector(memoria_por_sector);
            nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista]->nSectores[gap_Sector]->setSector( dato, state, ptr, origin);
        }
        //bloque
        vector<Sector *> cabezal_lectura(int pPlato, int pSuperficie, int pPista, int gap_Sector, int nSectores)
        {
            vector<Sector *> bloque;
            int auxPl=pPlato;
            int auxSpr=pSuperficie;
            int auxPst=pPista;
            int pSector=gap_Sector;
            if (!nPlatos[pPlato] || !nPlatos[pPlato]->nSuperficies[pSuperficie] || !nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista]) return vector<Sector *> ();
            for (int i = 0; i < nSectores; i++)
            {
                if (nPlatos[auxPl]->nSuperficies[auxSpr]->nPistas[auxPst]->nSectores[pSector])
                    bloque.push_back(nPlatos[auxPl]->nSuperficies[auxSpr]->nPistas[auxPst]->nSectores[pSector]); 
                pSector+=1;
                contadores(auxPl,auxSpr, auxPst, pSector);
            }
            
            return bloque;
        }

        //
        void setFreePosition(int &pl,int &spr,int &pst,int &sc)
        {
            freeposition[0] = pl;
            freeposition[1] = spr;
            freeposition[2] = pst;    
            freeposition[3] = sc;
        }
        
        int* getFreePositions(){return freeposition;}
        void contadores (int &pl,int &spr,int &pst,int &sc)
        {
           
            if(sc==numero_sectores_por_pista)
            {
                sc=0;
                pst++;
               
            }
            if(pst==numero_pistas_por_superficie)
            {
                spr++;
                pst=0;
            }
            if(spr==numero_superficies_por_plato)
            {
                pl++;
                spr=0;
            }
            
            // if(pl==numero_plato)
            // {
            //     pst++;
            //     sc==0;
            // }
        }
        void importarData_Disco(string nameArchivo)
        {
            // variables to navigate into disk
            // p -> plato
            // s -> superficie
            // pst -> pistas
            // sc -> sectores
            int pl,spr,pst,sc;
            pl = spr = pst = sc=0;
            
            fstream archivo(nameArchivo+".txt", fstream::in);
            char c;
            
//      SAVE DATA INTO SECTOR: ONE LINE IN ONE SECTOR
//      i => represent memory in bytes
            int lengh_char = 2;
            int i = 0;
            char *token = new char[lengh_char];
            while (archivo>>noskipws>>c)
            {
                if(int(c)==10 || int(c)==13)
                {
                    
                    // check if data size is more than memory per sector
                    
                    if( i > memoria_por_sector)
                    {                      
                        int *position_ofcontinuation= new int[4];
                        int aux = i;
                        char *aucPtr = new char[memoria_por_sector];
                        int aux2 =0;
                        while (aux >0 )
                        {
                            for (int j = 0; j < memoria_por_sector; j++)
                            {
                                *(aucPtr + j) = *(token+aux2);
                                aux2++;
                            }
                           
                            *(position_ofcontinuation +0) =pl;
                            *(position_ofcontinuation +1) =spr;
                            *(position_ofcontinuation +2) =pst;
                            *(position_ofcontinuation +3) =sc;
                            
                            cabezal_escritura(pl, spr, pst, sc, aucPtr, true, position_ofcontinuation, nameArchivo);
                        
                            sc++;
                            contadores(pl,spr,pst,sc);
                            //cout<<"next: "<<pl<<" : "<<spr<<" : "<<pst<<" : "<<sc<<endl;
                           // setFreePosition(pl,spr,pst,sc);
                            aux = aux -memoria_por_sector;
                        }
                        delete [] aucPtr;
                        delete [] position_ofcontinuation;

                    }else{
                        
                        //cout<<token<<endl;
                        cabezal_escritura(pl, spr, pst, sc, token, false, nullptr, nameArchivo);
                        
                        sc++;
                        contadores(pl,spr,pst,sc);
                        //setFreePosition(pl,spr,pst,sc);
                    }
                    
                    lengh_char = 2;
                    i = 0;
                    delete[]token;
                    token = new char[lengh_char];
                }
                else
                {
                    token[i] = c;
                   // token[i+1] = '\0';
                    i++;
                    lengh_char++;
                    token = (char *) realloc (token, lengh_char * sizeof (char));	// redimensionamos el token
                }
                
            }
        }        

        void DataInsertUser(string nameArchivo,char* token, int i,  int &pl,int &spr,int &pst,int &sc)
        {
            if( i > memoria_por_sector)
            {                      
                int *position_ofcontinuation= new int[4];
                int aux = i;
                char *aucPtr = new char[memoria_por_sector];
                int aux2 =0;
                while (aux >0 )
                {
                    for (int j = 0; j < memoria_por_sector; j++)
                    {
                        *(aucPtr + j) = *(token+aux2);
                        aux2++;
                    }
                    
                    *(position_ofcontinuation +0) =pl;
                    *(position_ofcontinuation +1) =spr;
                    *(position_ofcontinuation +2) =pst;
                    *(position_ofcontinuation +3) =sc;
                    
                    cabezal_escritura(pl, spr, pst, sc, aucPtr, true, position_ofcontinuation, nameArchivo);
                
                    sc++;
                    contadores(pl,spr,pst,sc);
                    //cout<<"next: "<<pl<<" : "<<spr<<" : "<<pst<<" : "<<sc<<endl;
                    //setFreePosition(pl,spr,pst,sc);
                    aux = aux -memoria_por_sector;
                }
                delete [] aucPtr;
                delete [] position_ofcontinuation;

            }else{
                
                //cout<<token<<endl;
                cabezal_escritura(pl, spr, pst, sc, token, false, nullptr, nameArchivo);
                
                sc++;
                contadores(pl,spr,pst,sc);
                //setFreePosition(pl,spr,pst,sc);
            }
                    
     
        }

};


/*

_________________________________________________________________________________________________

*/
struct palabras_Reservadas
{
    char select[6] = { 'S', 'E', 'L', 'E', 'C', 'T' };
    char create[6] = { 'C', 'R', 'E', 'A', 'T', 'E' };
    char insert[6] = { 'I', 'N', 'S', 'E', 'R', 'T' };
    char from[4] = { 'F', 'R', 'O', 'M' };
    char where[5] = { 'W', 'H', 'E', 'R', 'E' };
    char operadores[8] = { '*', '=', '+', '-', '<', '>', '/' };
    char simbolos_separacion[6] = { '&', '|', '(', ')', ',' };

    char find_operacion (char *w, int length)
    {
        //cout<<": "<<w<< " "<< length<<endl;
        bool c, s, i = false;
        for (int j = 0; j < length; j++)
        {
            if (select[j] == w[j]) s = true;
            else s = false; break;
        }
        for (int j = 0; j < length; j++)
        {
            if (create[j] == w[j]) c = true;
            else c = false;break;
        }
        for (int j = 0; j < length; j++)
        {
            if (insert[j] == w[j]) i = true;
            else i = false;break;
        }
        if (c) return 'c';
        if (s) return 's';
        if (i) return 'i';
        return '0';
    }
};

void create (string query)
{
  ofstream archivo;
  archivo.open("scheme.txt", fstream::app);
  archivo<<"\n";

  int lengh_char = 2;
  int i = 0;
  char *token = new char[lengh_char];
  string aux = "TABLE";
  int start = query.find (aux);

  if (start > -1)
    {
        for (int k = start + aux.length () + 1; k < query.length () + 1; k++)
	    {
            if (query[k] == ' ')
            {
                if(k==query.length()-2)
                    archivo<<token;
                else
                    archivo<<token<<'#';
                    
                lengh_char = 2;
                i = 0;
                delete[]token;
                token = new char[lengh_char];

            }
            else if (query[k] != '(' && query[k] != ',' && query[k] != ')')
            {
                token[i] = query[k];
                token[i + 1] = '\0';
                i++;
                lengh_char++;
                token =  (char *) realloc (token, lengh_char * sizeof (char));
            }
	    
	    }
    }
    cout<<"***\tSuccessful relation created\t***"<<endl;
}

void insert (string query, Disco &disk)
{
    ofstream archivo;
 
    int lengh_char = 2;
    int i = 0;
    char *token = new char[lengh_char];
    string aux = "INTO";
    string vaux = "VALUES";
    int start = query.find (aux);
    int vStart = query.find (vaux);
    string nameR = query.substr (start + aux.length()+1, vStart - (start + aux.length()+2));
    
    archivo.open(nameR+".txt", fstream::app);
    archivo<<"\n";
    if (vStart > -1)
    {
        cout<<"INICAIC: "<<query<<endl;    
        for (int k = vStart + vaux.length () + 1; k < query.length () + 1; k++)
        {

        if (query[k] == ' ')
            {
                
                //int *aux_ptr = disk.getFreePositions();
                    //disk.cabezal_escritura(aux_ptr[0],aux_ptr[1],ptr[2],aux_ptr[3], )
                if(k==query.length()-2)
                    archivo<<token;
                else
                    archivo<<token<<'#';
                
                
                
                lengh_char = 2;
                i = 0;
                delete[]token;
                token = new char[lengh_char];
            }
            else if (query[k] != '(' && query[k] != ',' && query[k] != ')')
            {
                token[i] = query[k];
                token[i + 1] = '\0';
                i++;
                lengh_char++;
                token =  (char *) realloc (token, lengh_char * sizeof (char));
            }
        }
    }
    cout<<"***\tSuccessful data inserted\t***"<<endl;
}

void select (string query)
{
    cout<<"IS SELECT"<<endl;
    
    cout<<query<<endl;
}
void analisis_lexico (string & query, Disco &disk)
{

    int lengh_char = 2;
    int i = 0;

    char *token = new char[lengh_char];
    palabras_Reservadas rv;
    for (int k = 0; k < query.length (); k++)
    {
        if (query[k] == ' ')
        {
            char aux = rv.find_operacion (token, i);
   
            if (aux == 'c')
                {
                create (query.substr (k + 1, query.length () - k));
                return;
                }
            if (aux == 'i')
                {
    
                insert (query.substr (k + 1, query.length () - k), disk);
                return;
                }
            if (aux == 's')
                {
                    select (query.substr (k + 1, query.length () - k));
                    return;
                }

            lengh_char = 2;
            i = 0;
            delete[]token;
            token = new char[lengh_char];
        }
        else
        {
            token[i] = query[k];
            //token[i+1] = '\0';
            i++;
            lengh_char++;
            token = (char *) realloc (token, lengh_char * sizeof (char));	// redimensionamos el token
        }

    }
}

int main()
{
    Disco disco(2, 2, 3, 4);

    
    disco.importarData_Disco("students");

    vector <Sector *> b = disco.cabezal_lectura(0,0, 0,1,5);
    cout<<"HOLA "<<endl;
    for(auto i:b){
        cout<<i->getData()<<endl;
    }
    //string consulta ="& SELECT * FROM depts#";
    string consulta = "& INSERT INTO students VALUES (pipi, 143, CS) #";
    //string consulta = "& CREATE TABLE students (psg int, names str, depst str) #";
    analisis_lexico (consulta, disco);







    // disco.cabezal_escritura(0, 0, 0, 0, "hola");
    // disco.cabezal_escritura(0, 0, 0, 2, "dos");

    // disco.cabezal_escritura(0, 0, 1, 1, "tres");
    
    // disco.cabezal_escritura(0, 1, 0, 0, "siete");
    // disco.cabezal_escritura(0, 1, 0, 1, "cinco");
    // disco.cabezal_escritura(0, 1, 0, 2, "seis");
    
    // disco.cabezal_escritura(1, 0, 1, 2, "nueve");

    // disco.cabezal_escritura(1, 1, 1, 0, "diez");
    
    // disco.giro();

    // vector <Sector *> b = disco.cabezal_lectura(0,1, 0,1,2);
    // for(auto i:b){
    //     cout<<i->getData()<<endl;
    // }

    return 0;
}
