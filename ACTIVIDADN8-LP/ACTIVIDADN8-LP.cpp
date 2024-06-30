#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <algorithm>
#include <memory>

using namespace std;

// Clase base para libros
class Libro {
protected:
    string titulo;
    string autor;
    string ISBN;
    string genero;
    int ano;
    bool disponible;

public:
    Libro(string titulo, string autor, string ISBN, string genero, int ano, bool disponible)
        : titulo(titulo), autor(autor), ISBN(ISBN), genero(genero), ano(ano), disponible(disponible) {}

    string getTitulo() const { return titulo; }
    string getAutor() const { return autor; }
    string getISBN() const { return ISBN; }
    string getGenero() const { return genero; }
    int getAno() const { return ano; }
    bool isDisponible() const { return disponible; }

    void setDisponible(bool disponible) { this->disponible = disponible; }
};

// Clase derivada para libros con información adicional
class LibroAcademico : public Libro {
private:
    string editorial;
    int edicion;

public:
    LibroAcademico(string titulo, string autor, string ISBN, string genero, int ano, bool disponible, string editorial, int edicion)
        : Libro(titulo, autor, ISBN, genero, ano, disponible), editorial(editorial), edicion(edicion) {}

    string getEditorial() const { return editorial; }
    int getEdicion() const { return edicion; }
};

// Clase base para usuarios
class Usuario {
protected:
    string nombre;
    int ID;
    string tipo;

public:
    Usuario(string nombre, int ID, string tipo)
        : nombre(nombre), ID(ID), tipo(tipo) {}

    string getNombre() const { return nombre; }
    int getID() const { return ID; }
    string getTipo() const { return tipo; }
};

// Clase derivada para estudiantes
class Estudiante : public Usuario {
private:
    string carrera;

public:
    Estudiante(string nombre, int ID, string carrera)
        : Usuario(nombre, ID, "Estudiante"), carrera(carrera) {}

    string getCarrera() const { return carrera; }
};

// Clase derivada para profesores
class Profesor : public Usuario {
private:
    string departamento;

public:
    Profesor(string nombre, int ID, string departamento)
        : Usuario(nombre, ID, "Profesor"), departamento(departamento) {}

    string getDepartamento() const { return departamento; }
};

// Clase derivada para investigadores
class Investigador : public Usuario {
private:
    string institucion;

public:
    Investigador(string nombre, int ID, string institucion)
        : Usuario(nombre, ID, "Investigador"), institucion(institucion) {}

    string getInstitucion() const { return institucion; }
};

// Clase para préstamos
class Prestamo {
private:
    shared_ptr<Libro> libro;
    shared_ptr<Usuario> usuario;
    string fechaPrestamo;
    string fechaDevolucion;

public:
    Prestamo(shared_ptr<Libro> libro, shared_ptr<Usuario> usuario, string fechaPrestamo, string fechaDevolucion)
        : libro(libro), usuario(usuario), fechaPrestamo(fechaPrestamo), fechaDevolucion(fechaDevolucion) {}

    shared_ptr<Libro> getLibro() const { return libro; }
    shared_ptr<Usuario> getUsuario() const { return usuario; }
    string getFechaPrestamo() const { return fechaPrestamo; }
    string getFechaDevolucion() const { return fechaDevolucion; }
};

// Clase para la biblioteca
class Biblioteca {
private:
    vector<shared_ptr<Libro>> libros;
    vector<shared_ptr<Usuario>> usuarios;
    vector<shared_ptr<Prestamo>> prestamos;

public:
    Biblioteca() {}

    // Agregar libro
    void agregarLibro(shared_ptr<Libro> libro) { libros.push_back(libro); }

    // Modificar libro
    void modificarLibro(shared_ptr<Libro> libro) {
        auto it = find_if(libros.begin(), libros.end(), [libro](shared_ptr<Libro> l) { return l->getISBN() == libro->getISBN(); });
        if (it != libros.end()) {
            *it = libro;
        }
        else {
            throw runtime_error("Libro no encontrado");
        }
    }

    // Eliminar libro
    void eliminarLibro(const string& ISBN) {
        auto it = find_if(libros.begin(), libros.end(), [ISBN](shared_ptr<Libro> l) { return l->getISBN() == ISBN; });
        if (it != libros.end()) {
            libros.erase(it);
        }
        else {
            throw runtime_error("Libro no encontrado");
        }
    }

    // Registrar usuario
    void registrarUsuario(shared_ptr<Usuario> usuario) { usuarios.push_back(usuario); }

    // Realizar préstamo
    void realizarPrestamo(shared_ptr<Libro> libro, shared_ptr<Usuario> usuario, const string& fechaPrestamo, const string& fechaDevolucion) {
        if (!libro->isDisponible()) {
            throw runtime_error("Libro no disponible");
        }
        libro->setDisponible(false);
        prestamos.push_back(make_shared<Prestamo>(libro, usuario, fechaPrestamo, fechaDevolucion));
    }

    // Devolver préstamo
    void devolverPrestamo(shared_ptr<Prestamo> prestamo) {
        prestamo->getLibro()->setDisponible(true);
        auto it = find(prestamos.begin(), prestamos.end(), prestamo);
        if (it != prestamos.end()) {
            prestamos.erase(it);
        }
    }

    // Buscar libros
    vector<shared_ptr<Libro>> buscarLibros(const string& criterio) {
        vector<shared_ptr<Libro>> resultados;
        for (const auto& libro : libros) {
            if (libro->getTitulo().find(criterio) != string::npos ||
                libro->getAutor().find(criterio) != string::npos ||
                libro->getISBN().find(criterio) != string::npos ||
                libro->getGenero().find(criterio) != string::npos) {
                resultados.push_back(libro);
            }
        }
        return resultados;
    }

    // Ver detalles de un libro
    shared_ptr<Libro> verDetallesLibro(const string& ISBN) {
        auto it = find_if(libros.begin(), libros.end(), [ISBN](shared_ptr<Libro> l) { return l->getISBN() == ISBN; });
        if (it != libros.end()) {
            return *it;
        }
        else {
            throw runtime_error("Libro no encontrado");
        }
    }

    // Ver historial de préstamos
    vector<shared_ptr<Prestamo>> verHistorialPrestamos(shared_ptr<Usuario> usuario) {
        vector<shared_ptr<Prestamo>> resultados;
        for (const auto& prestamo : prestamos) {
            if (prestamo->getUsuario() == usuario) {
                resultados.push_back(prestamo);
            }
        }
        return resultados;
    }
};

int main() {
    Biblioteca biblioteca;

    // Agregar libros
    shared_ptr<Libro> libro1 = make_shared<Libro>("Paco Yunque", "Autor 1", "ISBN1", "Ficción", 2020, true);
    shared_ptr<Libro> libro2 = make_shared<Libro>("Huasipungo", "Autor 2", "ISBN2", "Ciencia Ficción", 2019, true);
    shared_ptr<LibroAcademico> libro3 = make_shared<LibroAcademico>("Memorias de Alex", "Alexander Velasquez", "ISBN3", "Matemáticas", 2018, true, "Editorial 1", 2);
    biblioteca.agregarLibro(libro1);
    biblioteca.agregarLibro(libro2);
    biblioteca.agregarLibro(libro3);

    // Registrar usuarios
    shared_ptr<Estudiante> estudiante1 = make_shared<Estudiante>("Estudiante 1", 1, "Ingeniería");
    shared_ptr<Profesor> profesor1 = make_shared<Profesor>("Profesor 1", 2, "Departamento de Física");
    shared_ptr<Investigador> investigador1 = make_shared<Investigador>("Investigador 1", 3, "Instituto de Investigación");
    biblioteca.registrarUsuario(estudiante1);
    biblioteca.registrarUsuario(profesor1);
    biblioteca.registrarUsuario(investigador1);

    // Realizar préstamos
    biblioteca.realizarPrestamo(libro1, estudiante1, "2024-01-01", "2024-04-15");
    biblioteca.realizarPrestamo(libro2, profesor1, "2024-01-05", "2024-05-20");
    biblioteca.realizarPrestamo(libro3, investigador1, "2024-01-10", "2024-06-25");

    // Mostrar informes
    cout << "Libros disponibles:" << endl;
    vector<shared_ptr<Libro>> librosDisponibles = biblioteca.buscarLibros("");
    for (const auto& libro : librosDisponibles) {
        cout << libro->getTitulo() << " - " << libro->getAutor() << endl;
    }

    cout << "Historial de prestamos de Estudiante 1:" << endl;
    vector<shared_ptr<Prestamo>> prestamosEstudiante1 = biblioteca.verHistorialPrestamos(estudiante1);
    for (const auto& prestamo : prestamosEstudiante1) {
        cout << prestamo->getLibro()->getTitulo() << " - " << prestamo->getFechaPrestamo() << " - " << prestamo->getFechaDevolucion() << endl;
    }

    return 0;
}
