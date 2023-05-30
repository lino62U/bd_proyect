#include "iostream"
#include "vector"
#include "fstream"
#include "string.h"
#include <stdexcept>
using namespace std;

// typedef typename Sector*:: ptrSector;

class Sector
{
    private:
        int memory;
        int *position;
        string origin;
        int space;                  //  -> how many space is there respect to the memory
        bool opc;                   //  -> posibility to insert data
        int size_per_register;
        int size_per_register_file;
    public:
        Sector(int _memory, int _register_size, int _register_file_size):memory(_memory)
        {
            position = new int[2];
            space =  memory;
            opc = true;
            size_per_register = _register_size;
            size_per_register_file = _register_file_size;
        }
        ~Sector()
        {
            delete [] position;
        }
        void getData()
        {
            
            fstream archivo("files/"+ origin +".txt");

            int start =  (position[0] -1) * size_per_register_file + (position[0] - 1);
            int end =  position[1] * size_per_register_file + (position[1]-1);
            

            archivo.seekg(start,ios::beg);
            char *A = new char[end - start +1 ] ;            
            archivo.read(A, end - start);
            A[end - start]='\0';
            
            cout<<A<<endl;

            delete []A;
            archivo.close();
        }
      
    
        void setSector(int *positions, string _origin)
        {
            origin = _origin;
            *(position)=*(positions);
            *(position + 1)=*(positions + 1);
            
            //check how many register is imported
            int aux = position[1] - position[0] + 1;
            //space = space - aux*size_per_register_file;
            space = space - aux*size_per_register;
            
            // check if there is not enough space
            if(space == 0)
                opc = false;
            else
                opc=true;         
        }
        bool isEmpty(){ return space==0?true:false; }
        int getSpace(){return space;}
        int getCapacity() {return memory;}
        int getsize_per_register() {return size_per_register;}
        friend class Disco;
        friend class Plato;
        friend class Superficie;
        friend class Pista;
        
        friend class Bloque;
        friend class Directorio;

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

    friend class Bloque;
    friend class Directorio;

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

        friend class Bloque;
        friend class Directorio;
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

        friend class Bloque;
        friend class Directorio;

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
        int *scheme_conditions;
        int size_per_register_file;
        int size_register;
        string name_Table;
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
            scheme_conditions = NULL;
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
                                            nPlatos[i]->nSuperficies[j]->nPistas[k]->nSectores[l]->getData();
                                    }
                                }
                                
                            }
                        }        
                    }
                } 
            }
        }
        void cabezal_escritura(int pPlato, int pSuperficie, int pPista, int gap_Sector, int register_size, int file_register_size,int *positions, string origin)
        {
            //  try
            // {
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

                nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista]->nSectores[gap_Sector] = new Sector(memoria_por_sector, register_size,file_register_size);
                nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista]->nSectores[gap_Sector]->setSector( positions, origin);
            // }

            // // catch (invalid_argument& e)
            // // {
            // //     cerr << e.what() << endl;
            // //     return;
            // // }
            
        }
        
        void getDisk_info()
        {
            int num_discos = 1;
            cout<<"\n************************************************************************************"<<endl;
            cout<<"\tInformacion... "<<endl;
            cout<<"\t\t\t\t\tPor \tTotal"<<endl;
            cout<<"\t\tDiscos: \t\t"<<num_discos<<"\t"<<num_discos<<endl;
            cout<<"\t\tPlatos: \t\t"<<numero_plato<<"\t"<<num_discos*numero_plato<<endl;
            cout<<"\t\tSuperficies: \t\t"<<2<< "\t"<<num_discos* numero_plato*2 <<endl;
            cout<<"\t\tPistas: \t\t"<<numero_pistas_por_superficie<< "\t"<<num_discos* numero_plato*2 *numero_pistas_por_superficie<<endl;
            cout<<"\t\tSectores: \t\t"<<numero_sectores_por_pista<< "\t" <<num_discos* numero_plato*2 *numero_pistas_por_superficie * numero_sectores_por_pista <<endl;
            cout<<"\t\tMemoria (bytes): \t"<<memoria_por_sector <<"\t"<< memoria_por_sector*numero_sectores_por_pista <<endl;
            cout<<"\t\tCapacidad total: \t"<<num_discos* numero_plato*2 *numero_pistas_por_superficie * numero_sectores_por_pista  * memoria_por_sector<< " bytes"<<endl;
            
            
            // cout<<"\t\tTotal de Discos: \t1"<<endl;
            // cout<<"\t\tTotal de Platos: \t"<<numero_plato<<endl;
            // cout<<"\t\tTotal de Superficies: \t"<<numero_plato*2 <<endl;
            // cout<<"\t\tTotal de Pistas: \t"<< numero_plato*2 *numero_pistas_por_superficie<<endl;
            // cout<<"\t\tTotal de Sectores: \t"<<numero_plato*2 *numero_pistas_por_superficie * numero_sectores_por_pista <<endl;
            // cout<<"\t\tMemoria por sector: \t"<<memoria_por_sector <<" bytes"<<endl;
            // cout<<"\t\tCapacidad total: \t"<<numero_plato*2 *numero_pistas_por_superficie * numero_sectores_por_pista  * memoria_por_sector<< " bytes"<<endl;
            cout<<"\n************************************************************************************"<<endl;
        }
        void getSector(int pPlato, int pSuperficie, int pPista, int gap_Sector)
        {
            
            int auxPl=pPlato;
            int auxSpr=pSuperficie;
            int auxPst=pPista;
            int pSector=gap_Sector;
            cout<<"\n************************************************************************************"<<endl;
            cout<<"\n\tAll data in sector "<<endl;

            if (!nPlatos[pPlato] || !nPlatos[pPlato]->nSuperficies[pSuperficie] || !nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista]) 
            {
                cout<<"No exist data in this sector"<<endl;
                cout<<"\n************************************************************************************\n"<<endl;
                return;
            };
            
            if (nPlatos[auxPl]->nSuperficies[auxSpr]->nPistas[auxPst]->nSectores[pSector])
            {
                nPlatos[auxPl]->nSuperficies[auxSpr]->nPistas[auxPst]->nSectores[pSector]->getData(); 
                
                int capacidad = nPlatos[auxPl]->nSuperficies[auxSpr]->nPistas[auxPst]->nSectores[pSector]->getCapacity();
                int size_per_Registre = nPlatos[auxPl]->nSuperficies[auxSpr]->nPistas[auxPst]->nSectores[pSector]->getsize_per_register();
                int espacio = nPlatos[auxPl]->nSuperficies[auxSpr]->nPistas[auxPst]->nSectores[pSector]->getSpace();
                int cant_regis = (capacidad-espacio)/size_per_Registre;
                cout<<"\n\n\t*************************************"<<endl;
                cout<<"\t\tInformation about sector"<<endl;
                cout<<"\t1. Disk: \t\t\t1 "<<endl;
                cout<<"\t2. Plate: \t\t\t"<<pPlato+1<<endl;
                cout<<"\t3. Surface: \t\t\t"<<pPlato+1<<endl;
                cout<<"\t4. Track: \t\t\t"<<pPista+1<<endl;
                cout<<"\t5. Sector: \t\t\t"<<pSector+1<<endl;
                cout<<"\t6. Capacity: \t\t\t"<<capacidad<<" bytes"<<endl;
                cout<<"\t5. Bytes per register: \t\t"<<size_per_Registre<<" bytes"<<endl;;
                cout<<"\t7. Number of register: \t\t"<<cant_regis<<endl;
                cout<<"\t8. State: \t\t\t"<<espacio<<" bytes available"<<endl;
                cout<<"\t***************************************"<<endl;
            }  
            else{
                cout<<"Sector no contiene datos"<<endl;
            }            
            cout<<"\n************************************************************************************\n"<<endl;
        }
        
        void getOneRegister(int n, string nameTable)
        {
            
            fstream archivo("files/"+ nameTable +".txt");
            
            // archivo.seekg(201,ios::beg);
            // char *B = new char[200 +1 ] ;            
            // archivo.read(B, 200);
            // B[200]='\0';
            
            int start =  (n -1) * size_per_register_file + (n - 1);
            int end =  n * size_per_register_file + (n-1);
            //cout<<"ST: "<<start<<"  "<<"end: "<<end<<endl;
            archivo.seekg(start,ios::beg);
            
            char *A = new char[end - start +1 ] ;            
            archivo.read(A, end - start);
            A[end - start]='\0';


            archivo.seekg(0,ios::beg);
            //cout<<"len: "<<archivo.tellg()<<endl;
            archivo.seekg(0,ios::end);
            int length = archivo.tellg();
            
            
            cout<<"\n************************************************************************************"<<endl;
            cout<<"\tShow Register "<<n<<":\n"<<endl;
            cout<<A<<endl;
            cout<<"\n\n\t*************************************"<<endl;
            cout<<"\n\tInformation: "<<endl;
            cout<<"\t1. Size register file \t"<<size_per_register_file<<" bytes"<<endl;
            cout<<"\t2. Size register \t"<<size_register<<" bytes"<<endl;
            cout<<"\t2. Size file \t\t"<<length<<" bytes"<<endl;

            cout<<"\n\n\t*************************************"<<endl;
            cout<<"\n************************************************************************************"<<endl;

            delete []A;
            archivo.close();
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
        

        void values_KeyWords(int &register_bytes, string type, string amount)
        {
            if(type=="int")
                register_bytes+=4;
            
            if(type=="str")
                register_bytes+= (stoi(amount) * 1);
            
            if(type=="float")
                register_bytes+=4;
            
            if(type=="bool")
                register_bytes+=1;
            
        }
        void loadDataScheme(string nameScheme, string nameTable)
        {
            ifstream scheme("files/"+nameScheme+".txt" ,fstream::in);
            
            string token;
            string aux;
            int register_size=0;
            bool opc= false;
            int n=1;
            int p = 0;
            int count =0;
            int *arreglo = (int*) malloc(n*sizeof(int));
            while (getline(scheme, token))
            {
                int position = token.find(nameTable);
                if(position != -1)
                {
                    token = token.substr(nameTable.length()+1, token.length()- nameTable.length());
                    int m=1;

                    for (size_t i = 0; i <= token.length(); i++)
                    {
                        if(token[i]=='#' || token[i]=='\0')
                        {
                            string aux2="";
                            string aux3="";
                            if(m%2==0)
                            {
                                for (size_t q = 0; q < aux.length(); q++)
                                {
                                    if(isdigit(aux[q]))
                                        aux2+=aux[q];
                                    else
                                        aux3+=aux[q];    
                                }
                                
                                values_KeyWords(register_size,aux3,aux2);
                                count++;
                                arreglo[p]=stoi(aux2);
                                p++;
			                    n++;
			                    arreglo = (int *)realloc(arreglo, n * sizeof(int)); // redimensionamos el arreglo
                                aux2.clear();
                                aux3.clear();

                            }
                            aux.clear();
                            m++;
                        }
                        else
                            aux+=token[i];
                    }
                }
                aux.clear();     
            }
            
            // save conditions of scheme
            int file_register_size=0;
            scheme_conditions = new int[count];
            for (size_t i = 0; i < count; i++)
            {
                scheme_conditions[i] = arreglo[i];
                file_register_size += arreglo[i];
            }
            
            this->size_register =register_size;
            this->size_per_register_file = file_register_size;
            this->name_Table = nameTable+"2";

            //Create a new register
            
            generate_txt(arreglo,nameTable,name_Table);
            
            
            delete[]arreglo;
            scheme.close();

            // load data to disk
            
            assignData_Disco(name_Table, register_size, file_register_size);

        }

        void assignData_Disco(string nameArchivo, int register_size, int file_register_size)
        {
            // variables to navigate into disk
            // p -> plato
            // s -> superficie
            // pst -> pistas
            // sc -> sectores
            int pl,spr,pst,sc;
            int *positions_data = new int[2];
            pl = spr = pst = sc=0;
            int *position_ofcontinuation= new int[4];

            //check how many register could save in each sector
            int n = memoria_por_sector/register_size;

            cout<<":: "<<n<<endl;
            //fixes positions to save
            int start = 1;
            int number_lines=0;
            // count number of register in the file
            fstream archivo("files/"+nameArchivo+".txt");
            string line;
            while(archivo.peek()!=EOF)
            {
                getline(archivo, line);
                number_lines++;
            }
            archivo.close();
            for (int i = n; i <= number_lines; i+=n)
            {                
                *(positions_data) = start;
                *(positions_data +1) = i;

                cabezal_escritura(pl, spr, pst, sc, register_size, file_register_size,positions_data, nameArchivo);
                sc++;
                contadores(pl,spr,pst,sc);
                start = i+1;

                if(i+n>number_lines)
                {
                    *(positions_data) = start;
                    *(positions_data +1) = number_lines;
                    cabezal_escritura(pl, spr, pst, sc, register_size, file_register_size,positions_data, nameArchivo);
                    sc++;
                    contadores(pl,spr,pst,sc);
                }
            } 
            setFreePosition(pl,spr,pst,sc);
        
            delete [] position_ofcontinuation;
            delete [] positions_data;
        }


        void generate_txt(int *pesos,string file_original, string file_end)
        {
            
            ifstream archivo("files/"+file_original+".csv");
            
            fstream archivo2("files/"+file_end + ".txt",fstream::app);
            
            char c;
            int capacidad = 2;
            int j = 0;
            char *arreglo = new char[capacidad];
            while (archivo.get(c))
            {
                
                if (c == '\n')
                {
                    const char * word = "";
                    int h=0;
                    int aux = 0;
                    while(arreglo[h] != '\0'){
                        if(arreglo[h] == '#' || arreglo[h] == '\n'){
                            char *palabra = new char[pesos[aux]+1];
                            
                            strcpy(palabra,word);
                            for(int k=strlen(word);k < pesos[aux];k++){
                                strcat(palabra," ");
                            }
                            archivo2<<std::string(palabra);
                            aux++;
                            word="";
                            delete [] palabra;
                        }
                        else{
                            char *aux_copy = new char [strlen(word)+2];
                            char ss [2]= {arreglo[h],'\0'};
                            const char *ptr = ss;
                            strcpy(aux_copy,word);
                            strcat(aux_copy,ptr);
                            word = aux_copy;
                        }
                        h++;
                    }
                    archivo2<<endl;
                    capacidad = 2;
                    j = 0;
                    delete[] arreglo;
                    arreglo = new char[capacidad];
                }
                else
                {
                    arreglo[j] = c;
                    arreglo[j+1]='\0';
                    j++;
                    capacidad++;
                    arreglo = (char *)realloc(arreglo, capacidad * sizeof(char)); // redimensionamos el arreglo
                }
            }
            archivo.close();
            archivo2.close();
            cout<<"\n\t!Archivo "<<file_end<< ".txt creado con exito..."<<endl;
            
        }
        string getNameTable(){return name_Table;}
        friend class Bloque;
        friend class Directorio;
};


class Bloque
{
    private:
        vector <Sector *> bloques;
        Disco *ptrDisk;
        int i;
        // int *ptrPosition;
    public:
        Bloque(int sectors_per_block, Disco *preDis)
        {
            bloques.resize(sectors_per_block);
            ptrDisk = preDis;
            i=0;
            // ptrPosition = new int[4];
        }
  
        void print_block_content()
        {
            int j=0;
            int capacity=0;
            //cout<<"\n************************************************************************************"<<endl;
            cout<<"\n\tShow data saved in block\n"<<endl;
            
            for(auto i:bloques){
                i->getData();
                cout<<endl;
                cout<<endl;
                int aux = i->getCapacity() - i->getSpace();
                j++;
                capacity+=aux;
            }

            cout<<"\n\n\t*************************************"<<endl;
            cout<<"\t\tInformation about block"<<endl;
            cout<<"\t1. Total of sector: \t"<<j<<endl;
            cout<<"\t2. Capacity: \t\t"<<capacity<<" bytes"<<endl;
           
            
            cout<<"\t***************************************"<<endl;
            
            cout<<"\n************************************************************************************"<<endl;
        }
        void making_Block (int pPlato, int pSuperficie, int pPista, int pSector)
        {      
            // ptrPosition[0] = pPlato;
            // ptrPosition[1] = pSuperficie;
            // ptrPosition[2] = pPista;
            // ptrPosition[3] = pSector;
            bloques[i]=(ptrDisk->nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista]->nSectores[pSector]); 
            
            i++;
        }
    
        void contadores (int &pl,int &spr,int &pst,int &sc)
        {
           
            if(sc== ptrDisk->numero_sectores_por_pista)
            {
                sc=0;
                pst++;
               
            }
            if(pst==ptrDisk->numero_pistas_por_superficie)
            {
                spr++;
                pst=0;
            }
            if(spr==ptrDisk->numero_superficies_por_plato)
            {
                pl++;
                spr=0;
            }
            
            if(pl==ptrDisk->numero_plato)
            {
                cout<<"Insuficente size";
                return;
            }
        }
        //int *¨getPosition(){return ptrPosition;}
        vector <Sector *> * getBlocks(){return &bloques;}
        friend class Directorio;
};

class Directorio
{
    private:
        vector<Bloque *> directory;
        int cantidad_bloques;
        int sectores_por_bloques;
        Disco *ptrDisk;
    public:
        Directorio( int num_block, int num_secs, Disco *ptrDis)
        {
            cantidad_bloques = num_block;
            sectores_por_bloques = num_secs;
            ptrDisk = ptrDis;
            directory.resize(cantidad_bloques);
        }
        void createBlock (int pos,int pPlato, int pSuperficie, int pPista, int pSector)
        {
            
            if (!directory[pos])
            {
                directory[pos] = new Bloque(sectores_por_bloques, ptrDisk);
            }
            
            if (!ptrDisk->nPlatos[pPlato] || !ptrDisk->nPlatos[pPlato]->nSuperficies[pSuperficie] || !ptrDisk->nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista])
             {
                cout<<"No existe datos en ese sector"<<endl;
                return;
             }
            
            if (ptrDisk->nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista]->nSectores[pSector])
                directory[pos]->making_Block(pPlato,pSuperficie,pPista,pSector);
            else
                cout<<"No existe datos en ese sector"<<endl;
           
        }
        void passSECTOR(int pos, int sector)
        {
            int auxPl=0;
            int auxSpr=0;
            int auxPst=0;
            int pSector=0;
            for (int i = 0; i < sector-1; i++)
            {
                pSector++;
                contadores(auxPl, auxSpr, auxPst, pSector);
            }

            createBlock(pos, auxPl, auxSpr, auxPst, pSector);
        }
        void contadores (int &pl,int &spr,int &pst,int &sc)
        {
           
            if(sc== ptrDisk->numero_sectores_por_pista)
            {
                sc=0;
                pst++;
               
            }
            if(pst==ptrDisk->numero_pistas_por_superficie)
            {
                spr++;
                pst=0;
            }
            if(spr==ptrDisk->numero_superficies_por_plato)
            {
                pl++;
                spr=0;
            }
            
            if(pl==ptrDisk->numero_plato)
            {
                cout<<"Insuficente size";
                return;
            }
        }
        void showBlock(int n)
        {
            if(n>cantidad_bloques)
            {
                cout<<"No existe ese bloque"<<endl;
                cout<<"************************************************************************************\n"<<endl;
                return;
            }
            if(directory[n-1])
            {
                cout<<"\n\t\t\tBloque "<<n<<endl;
                cout<<"\t***************************************"<<endl;
                directory[n-1]->print_block_content();
            }
            
        }
        void showALL_Block()
        {
            cout<<"************************************************************************************"<<endl;
            for (int i = 0; i < cantidad_bloques; i++)
            {
                cout<<"\n\t\t\tBloque "<<i+1<<endl;
                cout<<"\t***************************************"<<endl;
                directory[i]->print_block_content();
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
                
                //int *aux_ptr = ptrDisk->getFreePositions();
                    //ptrDisk->cabezal_escritura(aux_ptr[0],aux_ptr[1],ptr[2],aux_ptr[3], )
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
    
// void show_datas(string atributos, string relation, string headers)
// {
    // ifstream MyReadFile(relation);
    // string tuples;
    // // getting header
    // size_t pos_in,pos_en;
    // int it = 1;
    // while ((pos_en = headers.find('#')) != std::string::npos) {
    //     headers.replace(pos_en, 1, ",");
    //     if(it%2==0)
    //         headers.replace(pos_in, pos_en-pos_in+1, ",");    
    //     pos_in=pos_en;
    //     it++;
    // }
    // headers.erase(pos_in, string::npos);
    
    // //showing all
    // if(atributos=="*")
    // {
    //     //headers
    //     while ((pos_en = headers.find(',')) != std::string::npos) {
    //         headers.replace(pos_en, 1, "\t");
    //     }
    //     cout<<headers<<endl;
    //     //getting data of file
    //     while (getline (MyReadFile, tuples)) {
    //         size_t pos;
    //         while ((pos = tuples.find('#')) != std::string::npos) {
    //             tuples.replace(pos, 1, "\t");
    //         }
    //         cout<<tuples<<endl;
    //     }
    //     return;
    // }

    // // show some columns
    // vector<string> atributos_headers;
    // vector<string> tuples_data;
    // vector<string> atributos_user;    

    // int st,en;
    // while ((en = headers.find(',')) != std::string::npos) {
    //     atributos_headers.push_back(headers.substr(0,en));
    //     headers.erase(0, en+1);
    // }
    // atributos_headers.push_back(headers);

    // while ((en = atributos.find(',')) != std::string::npos) {
    //     atributos_user.push_back(atributos.substr(0,en));
    //     atributos.erase(0, en+1);
    // }
    // atributos_user.push_back(atributos);
    // for(int i=0; i<atributos_user.size();i++)
    //     cout<<atributos_user[i]<<"\t";

    // cout<<endl;
    // while (getline (MyReadFile, tuples)) {
    //     size_t pos;
    //     while ((pos = tuples.find('#')) != std::string::npos) {
    //         tuples_data.push_back(tuples.substr(0,pos));
    //         tuples.erase(0, pos+1);
    //     }
    //     tuples_data.push_back(tuples);

    //     for (int i = 0; i < atributos_user.size(); i++) {
    //         for(int j=0; j<atributos_headers.size();j++)
    //         {
    //             if(atributos_user[i]==atributos_headers[j])
    //                cout<<tuples_data[j]<<"\t";
    //         }
    //     }
    //     cout<<endl;
    //     tuples_data.clear();

    // }
    // atributos_headers.clear();
    // tuples_data.clear();
    // atributos_user.clear();
    // MyReadFile.close();

    
    
    // cout<<"IS SELECT"<<endl;
    
    // cout<<query<<endl;
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



void convert(int *pesos,int nro_columnas,string file_original, string file_end){

	// ifstream archivo(file_original);
	// fstream archivo2(file_end,fstream::app);

    ifstream archivo("files/"+file_original+".csv");
    fstream archivo2("files/"+file_end + ".txt",fstream::app);
	char c;
	int capacidad = 2;
	int j = 0;
	char *arreglo = new char[capacidad];
	while (archivo.get(c))
	{
		if (c == '\n')
		{
			const char * word = "";
			int h=0;
			int aux = 0;
			while(arreglo[h] != '\0'){
				if(arreglo[h] == '#' || arreglo[h] == '\n'){
					char *palabra = new char[pesos[aux]+1];
					
					strcpy(palabra,word);
					for(int k=strlen(word);k < pesos[aux];k++){
						strcat(palabra," ");
					}
					archivo2<<std::string(palabra);
					aux++;
					word="";
					delete [] palabra;
				}
				else{
					char *aux_copy = new char [strlen(word)+2];
					char ss [2]= {arreglo[h],'\0'};
					const char *ptr = ss;
					strcpy(aux_copy,word);
					strcat(aux_copy,ptr);
					word = aux_copy;
				}
				h++;
			}
			archivo2<<endl;
			capacidad = 2;
			j = 0;
			delete[] arreglo;
			arreglo = new char[capacidad];
		}
		else
		{
			arreglo[j] = c;
			arreglo[j+1]='\0';
			j++;
			capacidad++;
			arreglo = (char *)realloc(arreglo, capacidad * sizeof(char)); // redimensionamos el arreglo
		}
	}
	
}

void menu()
{
    cout<<"\n**************************************************************************************\n"<<endl;
}


//Get info disk
void opcion_1(Disco *ptrDisk)
{
    ptrDisk->getDisk_info();
}

// Register
void opcion_2(Disco *ptrDisk)
{
    menu();
    string nameTable = ptrDisk->getNameTable();
    int n;
    cout<<"\tNumero de registro: ";cin>>n; 
    cout<<"dd: "<<n<<" "<<nameTable<<endl;
    ptrDisk->getOneRegister(n,nameTable);
}

// get Sector
void opcion_3(Disco *ptrDisk)
{
    menu();
    int plato=0;
    int superficie=0;
    int pista=0;
    int sector=0;
    int n;
    cout<<"Sector a imprimir: ";cin>>n;
    for (int i = 0; i < n-1; i++)
    {
        sector++;
        ptrDisk->contadores(plato,superficie,pista,sector);
    }
    
    ptrDisk->getSector(plato,superficie,pista,sector);
}

//making Directotio

void opcion_4(Disco *ptrDisk)
{
    int num_blocks;
    int num_sec_per_blocks;
    int sector;

    menu();
    cout<<"Making a directory..."<<endl;
    cout<<"\tNumero de bloques: ";cin>>num_blocks;
    cout<<"\tNumero de sectores por bloque: ";cin>>num_sec_per_blocks;
    
    Directorio directorios(num_blocks, num_sec_per_blocks, ptrDisk);
    
    // First loop to number of blocks
    for (int i = 0; i < num_blocks; i++)
    {
        cout<<"\tBloque "<<i+1<<endl;
        for (int j = 0; j < num_sec_per_blocks; j++)
        {
            cout<<"\t\tSector a guardar: ";cin>>sector;
            directorios.passSECTOR(i,sector);
        }
    }

    cout<<"\n*************************************"<<endl;   
    cout<<"\tCreation of directory complete!"<<endl;
    int ops;
    while (true)
    {
        cout<<"\t\t1. Mostrar DATA de Bloque"<<endl;
        cout<<"\t\t2. Mostrar DATA del Directorio"<<endl;
        cout<<"\t\t3. Salir"<<endl;
        cout<<"\t\t Opc: ";cin>>ops;
        
        if(ops==1) 
        {
            cout<<"\t\t\tNumero de Bloque: ";cin>>ops;
            directorios.showBlock(ops);
        }
        else if(ops==2)
        {
            directorios.showALL_Block();
        }
        else break;
    }
    

}

void menu_opciones(Disco *ptrDisco)
{
    int opc;
    
    while (true)
    {
        cout<<"WELCOME TO MEGAST\n"<<endl;
        cout<<"Opciones que desea realizar"<<endl;
        cout<<"\t1. Obtener informacion del disco"<<endl;
        cout<<"\t2. Imprimir un registro"<<endl;
        cout<<"\t3. Imprimir datos de un SECTOR"<<endl;
        cout<<"\t4. Crear un Directorio"<<endl;
        cout<<"\t5. Salir"<<endl;
        cout<<"\topc (1-5)?: ";cin>>opc;
        
        if(opc==1) opcion_1(ptrDisco);
        else if(opc==2) opcion_2(ptrDisco);
        else if(opc==3) opcion_3(ptrDisco);
        else if(opc==4) opcion_4(ptrDisco);
        else return;
        menu();
    }
    
}


int main()
{
    int plato;
    int pista;
    int sector;
    int memoria;
    //string nameTable;
    //692 -> 4r
    menu();
    
    string nameTable = "titanic";
    Disco disco(4, 10, 200, 692);
    disco.loadDataScheme("scheme",nameTable.c_str());
    disco.getOneRegister(891,"titanic2");
    // disco.getDisk_info();
    // disco.getSector(0,0,0,1);
    // opcion_4(&disco);
    
    // cout<<"\tDisk setting"<<endl;
    // cout<<"\tPlato: ";cin>>plato;
    // cout<<"\tPista: ";cin>>pista;
    // cout<<"\tSector: ";cin>>sector;
    // cout<<"\tMemoria: ";cin>>memoria;
    // cout<<"\nInserta nombre del archivo: ";
    // cin.ignore();
    // getline(cin,nameTable);
    // cout<<"Making new file with format"<<endl;
    // cout<<"Wait... "<<endl;

    // Disco disco(plato, pista, sector, memoria);  //5 register per sector
    // //cout<<nameTable<<endl;
    // disco.loadDataScheme("scheme",nameTable.c_str());
    // cout<<"\tDatos subidos con exito al disco"<<endl;
    // menu();
    // menu_opciones(&disco);

    
    return 0;
}
