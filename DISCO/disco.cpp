#include "iostream"
#include "vector"
using namespace std;

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
class Pista
{
    private:
        int numero_sectores;
        int gap;
        vector<Sector *> nSectores;
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
        void giro(){}
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
    Disco disco(2, 100, 500, 5);
    disco.cabezal_escritura(0, 0, 50, 200, "dos");
    disco.cabezal_escritura(0, 0, 50, 201, "tres");
    disco.cabezal_escritura(0, 0, 50, 202, "cuatro");
    disco.cabezal_escritura(0, 0, 50, 203, "cinco");

    vector <Sector *> b = disco.cabezal_lectura(0,0, 50,200,3);
    for(auto i:b){
        cout<<i->getData()<<endl;
    }
}
