#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>


bool eliminarFecha(std::vector <std::string>& personas, bool link) {
    //Cojo la lista y me quedo solo con los nombres de usuario
    std::string usuario; std::cin >> usuario;
    
    if (!std::cin) return false;
    
    personas.push_back(usuario);

    std::string linea;
    if (link) {
        std::getline(std::cin, linea); // resto del usuario
        std::getline(std::cin, linea); // Línea del link
        std::getline(std::cin, linea); // Línea de la fecha
    }
    else {
        std::getline(std::cin, linea); // resto del usuario
        std::getline(std::cin, linea); // Línea de la fecha
    }
    return true;
}


void quitarFechas() {
    // Abre un archivo para escribir
    std::ofstream archivoSalida("Combinacion.txt");

    if (archivoSalida.is_open()) {
        // Redirige la salida estándar (stdout) al archivo
        std::streambuf* respaldoCout = std::cout.rdbuf(archivoSalida.rdbuf());

        std::vector <std::string> seguidores;
        std::vector <std::string> seguidos;


        std::ifstream entrada1("Followers.txt");
        if (!entrada1.is_open()) std::cout << "El archivo de entrada 1 no se ha abierto\n";
        else {
            // Redirige la entrada estándar (stdcin) al archivo
            std::streambuf* respaldoCin = std::cin.rdbuf(entrada1.rdbuf());


            while (eliminarFecha(seguidores, false));

            // Restaura la salida estándar original
            std::cin.rdbuf(respaldoCin);
        }
        entrada1.close();

        std::ifstream entrada2("Following.txt");
        if (!entrada2.is_open()) std::cout << "El archivo de entrada 2 no se ha abierto\n";
        else {
            // Redirige la entrada estándar (stdcin) al archivo
            std::streambuf* respaldoCin = std::cin.rdbuf(entrada2.rdbuf());


            while (eliminarFecha(seguidos, true));

            // Restaura la salida estándar original
            std::cin.rdbuf(respaldoCin);
        }
        entrada2.close();

        for (std::string& x : seguidores) std::cout << x << "\n";
        std::cout << "ALTO\n";
        for (std::string& x : seguidos) std::cout << x << "\n";


        // Restaura la salida estándar original
        std::cout.rdbuf(respaldoCout);
        // Cierra el archivo
        archivoSalida.close();
    }
    else {
        std::cerr << "No se pudo abrir el archivo de salida." << std::endl;
    }
}


void resolver(std::vector <std::string>& seguidores, std::vector <std::string>& seguidos, std::vector <std::string>& v) {
    for (int i = 0; i < seguidos.size(); i++) {
        int j = 0;
        while (j < seguidores.size() && seguidos[i] != seguidores[j]) {
            j++;
        }
        if (j == seguidores.size()) { //la persona a la que sigo no está entre mis seguidores
            v.push_back(seguidos[i]);
        }
    }

}


// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool meterEnVector(bool& cambio, std::vector <std::string>& seguidores, std::vector <std::string>& seguidos) {
    // leer los datos de la entrada
    std::string user; std::cin >> user;
    if (!std::cin)
        return false;

    if (user == "ALTO") {
        cambio = true;
        std::cin >> user;
    }

    if (!cambio) seguidores.push_back(user);
    else seguidos.push_back(user);

    return true;
}


void comparacion() {
    // Para la entrada por fichero.
    // Comentar para acepta el reto
#ifndef DOMJUDGE
    std::ifstream in("Combinacion.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf()); //save old buf and redirect std::cin to casos.txt
#endif 

    // Abre un archivo para escribir
    std::ofstream archivoSalida("SeguidosNoSeguidores.txt");

    if (archivoSalida.is_open()) {
        // Redirige la salida estándar (stdout) al archivo
        std::streambuf* respaldoCout = std::cout.rdbuf(archivoSalida.rdbuf());


        //CODIGO

        bool cambio = false;
        std::vector <std::string> seguidores;
        std::vector <std::string> seguidos;
        while (meterEnVector(cambio, seguidores, seguidos));

        std::vector <std::string> seguidosNoSeguidores;
        resolver(seguidores, seguidos, seguidosNoSeguidores);

        for (std::string& x : seguidosNoSeguidores) std::cout << x << "\n";


        // Restaura la salida estándar original
        std::cout.rdbuf(respaldoCout);
        // Cierra el archivo
        archivoSalida.close();
    }
    else {
        std::cerr << "No se pudo abrir el archivo de salida." << std::endl;
    }


    // Para restablecer entrada. Comentar para acepta el reto
#ifndef DOMJUDGE // para dejar todo como estaba al principio
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif
}





int main() {
    quitarFechas();
    comparacion();
    return 0;
}
