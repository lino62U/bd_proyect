#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

void reemplazar(string file_name, int num_reg, string nueva_linea)
{

    fstream archivo(file_name, ios::in | ios::out | ios::binary); // Abre el archivo en modo lectura y escritura

    size_t lineaModificar = num_reg; // Índice de la línea a modificar (empezando desde 0)
    size_t posicionInicio = 0;
    size_t longitudLinea = 0;
    string linea;

    // Busca la posición de inicio y la longitud de la línea a modificar
    for (size_t i = 0; i <= lineaModificar; ++i)
    {
        posicionInicio = archivo.tellg(); // Obtiene la posición actual en el archivo
        if (!getline(archivo, linea))
        {
            cout << "La línea especificada no existe." << endl;
        }
        longitudLinea = linea.length() + 1; // +1 para contar el carácter de nueva línea ('\n')
    }
    // Modifica la línea deseada
    string nuevaLinea = nueva_linea;
    for (int j = nuevaLinea.length(); j < longitudLinea - 2; j++)
    {
        nuevaLinea += ' ';
    }
    // nuevaLinea += '\n';
    archivo.seekp(posicionInicio);
    archivo.write(nuevaLinea.c_str(), nuevaLinea.length()); // Escribe la nueva línea en el archivo

    archivo.close(); // Cierra el archivo
}

void eliminar(string file_name, int num_reg)
{

    fstream archivo(file_name);
    int temp = 0;
    string linea;
    getline(archivo, linea);
    linea.erase(remove(linea.begin(), linea.end(), ' '), linea.end());
    int inicio = 0;
    if (linea.size() > 0)
    {
        temp = 1;
        inicio = stoi(linea);
    }
    size_t pos_inicio;
    while (temp)
    {
        string aux;
        archivo.seekg(0, ios::beg);
        for (int i = 0; i < inicio; i++)
        {
            pos_inicio = archivo.tellg();
            getline(archivo, aux);
        }
        getline(archivo, aux);
        aux.erase(remove(aux.begin(), aux.end(), ' '), aux.end());
        if (aux.size() > 0)
        {
            inicio = stoi(aux); //- inicio + 1;
        }
        else
        {
            temp = 0;
        }
    }

    reemplazar(file_name, inicio, to_string(num_reg));
    reemplazar(file_name, num_reg, "");
}

void insertar(string file_name, string data)
{
    ifstream archivo(file_name);
    int temp = 0;
    string linea;
    getline(archivo, linea);
    linea.erase(remove(linea.begin(), linea.end(), ' '), linea.end());
    int inicio = 0;
    if (linea.size() > 0)
    {
        temp = 1;
        inicio = stoi(linea);
    }
    else{
        ofstream archivo2(file_name,ios::app);
        archivo2<<data<<endl;   
    }

    string aux;
    archivo.seekg(0, ios::beg);
    for (int i = 0; i < inicio; i++)
    {
        getline(archivo, aux);
    }
    getline(archivo, aux);
    aux.erase(remove(aux.begin(), aux.end(), ' '), aux.end());

    reemplazar(file_name, inicio, data);
    reemplazar(file_name, 0, aux);

}

void swap(string file_name, int reg1, int reg2){
    ifstream archivo(file_name);
    string temp1,temp2;

    archivo.seekg(0, ios::beg);
    for (int i = 0; i < reg1; i++)
    {
        getline(archivo, temp1);
    }
    getline(archivo, temp1);

    archivo.seekg(0, ios::beg);
    for (int i = 0; i < reg2; i++)
    {
        getline(archivo, temp2);
    }
    getline(archivo, temp2);

    reemplazar(file_name,reg1,temp2);
    reemplazar(file_name,reg2,temp1);
}   

int main()
{

    // eliminar("titanic2.txt", 2);
    // eliminar("titanic2.txt", 15);
    // eliminar("titanic2.txt", 20);
    // eliminar("titanic2.txt", 24);
    // eliminar("titanic2.txt", 30);
    // eliminar("titanic2.txt", 31);
    // insertar("titanic2.txt", "AAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    // insertar("titanic2.txt", "AAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    // insertar("titanic2.txt", "AAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    swap("titanic2.txt",5,13);

    return 0;
}
