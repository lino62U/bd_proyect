#include "iostream"
#include "vector"
using namespace std;

// typedef typename Sector*:: ptrSector;

class Sector
{
    private:
        int memory;
        char *data;
    public:
        Sector(int _memory, const char *indata):memory(_memory)
        {
            data = new char[memory];
            for (size_t i = 0; i < memory; i++)
                *(data + i) = *(indata +i);      
        }
        char* getData() {return data;}
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
    public:
        Plato():numero_superficie(2){nSuperficies.resize(numero_superficie);}
        friend class Disco;
};
class Disco
{
    private:
        int numero_plato;
        int numero_pistas_por_superficie;
        int numero_sectores_por_pista;
        int memoria_por_sector;
        vector<Plato *> nPlatos;
    public:
        Disco(int nPlato, int nPistas, int nSectores, int nBytes) 
        {
            numero_plato = nPlato;
            numero_pistas_por_superficie = nPistas;
            numero_sectores_por_pista = nSectores;
            memoria_por_sector = nBytes;
            nPlatos.resize(numero_plato);
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
        void cabezal_escritura(int pPlato, int pSuperficie, int pPista, int gap_Sector, const char *dato)
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
            nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista]->nSectores[gap_Sector] = new Sector(memoria_por_sector, dato);
        }
        //bloque
        vector<Sector *> cabezal_lectura(int pPlato, int pSuperficie, int pPista, int gap_Sector, int nSectores)
        {
            vector<Sector *> bloque;
            
            if (!nPlatos[pPlato] || !nPlatos[pPlato]->nSuperficies[pSuperficie] || !nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista]) return vector<Sector *> ();
            for (int i = 0; i < nSectores; i++)
            {
                if (nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista]->nSectores[gap_Sector + i])
                    bloque.push_back(nPlatos[pPlato]->nSuperficies[pSuperficie]->nPistas[pPista]->nSectores[gap_Sector + i]); 
            }
            
            return bloque;
        }
        
};

int main()
{
    Disco disco(2, 2, 3, 4);
    disco.cabezal_escritura(0, 0, 0, 0, "hola");
    disco.cabezal_escritura(0, 0, 0, 2, "dos");

    disco.cabezal_escritura(0, 0, 1, 1, "tres");
    
    disco.cabezal_escritura(0, 1, 0, 0, "siete");
    disco.cabezal_escritura(0, 1, 0, 1, "cinco");
    disco.cabezal_escritura(0, 1, 0, 2, "seis");
    
    disco.cabezal_escritura(1, 0, 1, 2, "nueve");

    disco.cabezal_escritura(1, 1, 1, 0, "diez");
    
    disco.giro();

    // vector <Sector *> b = disco.cabezal_lectura(0,1, 0,1,2);
    // for(auto i:b){
    //     cout<<i->getData()<<endl;
    // }

    return 0;
}
