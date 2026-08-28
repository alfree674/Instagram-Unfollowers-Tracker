#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>

#include "json.hpp" // https://github.com/nlohmann/json single_include/nlohmann/json.hpp

using json = nlohmann::json;

//Ficheros de entrada/salida
static const std::string ARCHIVO_SEGUIDORES = "followers_1.json";
static const std::string ARCHIVO_SEGUIDOS = "following.json";
static const std::string ARCHIVO_SALIDA = "SeguidosNoSeguidores.txt";

// Carga un JSON. Excepcion si hay error.
json cargarJSON(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + ruta);
    }

    json j;
    archivo >> j;
    return j;
}


// Extrae el nombre de usuario de una entrada individual según provenga de followers o following
std::string extraerUsuario(const json& entrada) {
    //para followers: string_list_data[0].value
    if (entrada.contains("string_list_data") && entrada["string_list_data"].is_array() && !entrada["string_list_data"].empty()) {

        const json& dato = entrada["string_list_data"][0];

        if (dato.contains("value") && dato["value"].is_string()) {
            return dato["value"].get<std::string>();
        }

    }
    //para following: title
    if (entrada.contains("title") && entrada["title"].is_string()) {
        std::string titulo = entrada["title"].get<std::string>();
        if (!titulo.empty()) return titulo;
    }

    return ""; //no se pudo encontrar el usuario para esta entrada
}


// Obtiene la lista de seguidores o seguidos
const json& localizarLista(const json& raiz) {
    //followers: el JSON es la propia lista
    if (raiz.is_array()) return raiz;
    //following: el JSON es un objeto que contiene la lista
    if (raiz.is_object()) {
        for (auto it = raiz.begin(); it != raiz.end(); ++it) {
            if (it.value().is_array()) return it.value();
        }
    }
    throw std::runtime_error("No se ha encontrado una lista de usuarios en el JSON");
}


// Convierte el JSON completo en una lista de nombres de usuario.
std::vector<std::string> extraerUsuarios(const json& raiz) {
    std::vector<std::string> usuarios;
    const json& lista = localizarLista(raiz);

    usuarios.reserve(lista.size());
    for (const auto& entrada : lista) {
        std::string usuario = extraerUsuario(entrada);
        if (!usuario.empty()) usuarios.push_back(usuario);
    }
    return usuarios;
}


// Devuelve las personas que sigues (seguidos) y que no te siguen (no en seguidores)
std::vector<std::string> resolver(const std::vector<std::string>& seguidores, const std::vector<std::string>& seguidos) {

    std::unordered_set<std::string> setSeguidores(seguidores.begin(), seguidores.end());  //para optimizar la busqueda

    std::vector<std::string> resultado;
    for (const auto& usuario : seguidos) {
        if (setSeguidores.find(usuario) == setSeguidores.end()) {
            resultado.push_back(usuario);
        }
    }
    return resultado;
}


int main() {
    try {
        std::vector<std::string> seguidores = extraerUsuarios(cargarJSON(ARCHIVO_SEGUIDORES));
        std::vector<std::string> seguidos = extraerUsuarios(cargarJSON(ARCHIVO_SEGUIDOS));

        std::cout << "Seguidores leidos: " << seguidores.size() << "\n";
        std::cout << "Seguidos leidos:   " << seguidos.size() << "\n\n";

        std::vector<std::string> noTeSiguen = resolver(seguidores, seguidos);

        std::ofstream salida(ARCHIVO_SALIDA);
        if (!salida.is_open()) {
            throw std::runtime_error("No se pudo crear el archivo de salida: " + ARCHIVO_SALIDA);
        }
        for (const auto& usuario : noTeSiguen) salida << usuario << "\n";
        salida.close();

        std::cout << "Personas que sigues pero no te siguen (" << noTeSiguen.size() << "):\n";
        for (const auto& usuario : noTeSiguen) std::cout << usuario << "\n";

        std::cout << "\nResultado guardado en " << ARCHIVO_SALIDA << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
