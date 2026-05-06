#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <cctype>

using namespace std;


struct Usuario {
    int idUsuario;
    string nombre;
    string correo;
    string contrasena;
    string direccion;
    string metodoPago;
};

struct Producto {
    int idProducto;
    string nombre;
    string descripcion;
    double precio;
    int stock;
};

struct Comentario {
    int idComentario;
    string producto;
    string usuario;
    string comentario;
    string fecha;
};

struct Carrito {
    int idCarrito;
    int idUsuario;
    vector<Producto> productos;
};


vector<Usuario> usuarios;
vector<Producto> productos;
vector<Comentario> comentarios;
vector<Carrito> carritos;


string trim(string s) {
    while (!s.empty() && isspace(s[0])) s.erase(0,1);
    while (!s.empty() && isspace(s[s.size()-1])) s.erase(s.size()-1,1);
    return s;
}




void cargarUsuarios() {
    ifstream file("Usuarios.txt");
    if (!file.is_open()) {
        cout << "Error al abrir Usuarios.txt\n";
        return;
    }

    string linea;
    getline(file, linea);

    while (getline(file, linea)) {
        stringstream ss(linea);
        Usuario u;
        string temp;

        getline(ss, temp, ','); u.idUsuario = atoi(temp.c_str());
        getline(ss, u.nombre, ',');
        getline(ss, u.correo, ',');
        getline(ss, u.contrasena, ',');
        getline(ss, u.direccion, ',');
        getline(ss, u.metodoPago);

        usuarios.push_back(u);
    }
}


void cargarProductos() {
    ifstream file("Productos.txt");
    if (!file.is_open()) {
        cout << "Error al abrir Productos.txt\n";
        return;
    }

    string linea;
    getline(file, linea);

    while (getline(file, linea)) {
        stringstream ss(linea);
        Producto p;
        string temp;

        getline(ss, temp, ','); p.idProducto = atoi(temp.c_str());
        getline(ss, p.nombre, ',');
        getline(ss, p.descripcion, ',');
        getline(ss, temp, ','); p.precio = atof(temp.c_str());
        getline(ss, temp); p.stock = atoi(temp.c_str());

        productos.push_back(p);
    }
}


void cargarComentarios() {
    ifstream file("Comentarios.txt");
    if (!file.is_open()) {
        cout << "Error al abrir Comentarios.txt\n";
        return;
    }

    string linea;
    getline(file, linea);

    while (getline(file, linea)) {
        stringstream ss(linea);
        Comentario c;
        string temp;

        getline(ss, temp, ','); c.idComentario = atoi(temp.c_str());
        getline(ss, c.producto, ',');
        getline(ss, c.usuario, ',');
        getline(ss, c.comentario, ',');
        getline(ss, c.fecha);

        c.fecha = trim(c.fecha); 

        comentarios.push_back(c);
    }
}


int login() {
    string correo, pass;

    cout << "Correo: ";
    cin >> correo;
    cout << "Contrasena: ";
    cin >> pass;

    for (int i = 0; i < usuarios.size(); i++) {
        if (usuarios[i].correo == correo && usuarios[i].contrasena == pass) {
            return usuarios[i].idUsuario;
        }
    }

    return -1;
}


void listarProductosBajoStock() {
    cout << "\nProductos con stock menor a 15:\n";
    for (int i = 0; i < productos.size(); i++) {
        if (productos[i].stock < 15) {
            cout << productos[i].idProducto << " - "
                 << productos[i].nombre << " - Stock: "
                 << productos[i].stock << endl;
        }
    }
}


void comentariosDesdeFecha() {
    string fecha;
    cout << "Ingrese fecha (YYYY-MM-DD): ";
    cin >> fecha;

    bool existe = false;

    for (int i = 0; i < comentarios.size(); i++) {
        if (comentarios[i].fecha == fecha) {
            existe = true;
            break;
        }
    }

    if (!existe) {
        cout << "\nNo hay comentarios en esa fecha.\n";
        return;
    }

    cout << "\nComentarios:\n";
    for (int i = 0; i < comentarios.size(); i++) {
        if (comentarios[i].fecha == fecha) {
            cout << comentarios[i].producto << " - "
                 << comentarios[i].usuario << ": "
                 << comentarios[i].comentario << endl;
        }
    }
}


void listarUsuarios() {
    cout << "\nUsuarios:\n";

    for (int i = 0; i < usuarios.size(); i++) {
        string nombre = usuarios[i].nombre;

        for (int j = 0; j < nombre.length(); j++) {
            nombre[j] = toupper((unsigned char)nombre[j]);
        }

        cout << usuarios[i].idUsuario << " - "
             << nombre << " - "
             << usuarios[i].correo << endl;
    }
}


int buscarCarrito(int idUsuario) {
    for (int i = 0; i < carritos.size(); i++) {
        if (carritos[i].idUsuario == idUsuario) {
            return i;
        }
    }
    return -1;
}


int generarIdCarrito() {
    if (carritos.empty()) return 1;
    return carritos.back().idCarrito + 1;
}


void agregarProducto(int idUsuario) {
    int idProd, cantidad;

    cout << "ID Producto: ";
    cin >> idProd;
    cout << "Cantidad: ";
    cin >> cantidad;

    for (int i = 0; i < productos.size(); i++) {

        if (productos[i].idProducto == idProd) {

            if (productos[i].stock < cantidad) {
                cout << "Stock insuficiente\n";
                return;
            }

            int pos = buscarCarrito(idUsuario);

            if (pos == -1) {
                Carrito c;
                c.idCarrito = generarIdCarrito();
                c.idUsuario = idUsuario;
                carritos.push_back(c);
                pos = carritos.size() - 1;
            }

            for (int j = 0; j < cantidad; j++) {
                carritos[pos].productos.push_back(productos[i]);
            }

            productos[i].stock -= cantidad;

            cout << "Producto agregado al carrito\n";
            return;
        }
    }

    cout << "Producto no encontrado\n";
}


void listarCarrito(int idUsuario) {
    int pos = buscarCarrito(idUsuario);

    if (pos == -1 || carritos[pos].productos.empty()) {
        cout << "No tiene productos en el carrito.\n";
        return;
    }

    cout << "\nCarrito #" << carritos[pos].idCarrito << endl;

    double total = 0;

    for (int i = 0; i < carritos[pos].productos.size(); i++) {
        cout << carritos[pos].productos[i].nombre
             << " - $" << carritos[pos].productos[i].precio << endl;

        total += carritos[pos].productos[i].precio;
    }

    cout << "Total: $" << total << endl;
}


int main() {

    cargarUsuarios();
    cargarProductos();
    cargarComentarios();

    int idUsuario = login();

    if (idUsuario == -1) {
        cout << "Usuario invalido\n";
        return 0;
    }

    int opcion;

    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Productos bajo stock\n";
        cout << "2. Comentarios por fecha exacta\n";
        cout << "3. Listar usuarios\n";
        cout << "4. Agregar producto al carrito\n";
        cout << "5. Ver carrito\n";
        cout << "0. Salir\n";

        cin >> opcion;

        switch (opcion) {
            case 1: listarProductosBajoStock(); break;
            case 2: comentariosDesdeFecha(); break;
            case 3: listarUsuarios(); break;
            case 4: agregarProducto(idUsuario); break;
            case 5: listarCarrito(idUsuario); break;
        }

    } while (opcion != 0);

    return 0;
}
