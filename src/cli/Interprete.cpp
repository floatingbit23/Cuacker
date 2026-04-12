#include <iostream>
#include <string>
#include <list>
#include "Fecha.h"
#include "Cuac.h"
#include "Persistencia.h"
#include "Interprete.h"

// Usamos declaraciones individuales para mantener el código limpio sin contaminar el namespace global
using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::cerr;
using std::list;
using std::getline;
using std::unordered_set;

/**
 * @brief Constructor por defecto de nuestro Intérprete.
 */
Interprete::Interprete(){}

/**
 * @brief Ejecutamos el motor de nuestra aplicación.
 * Implementamos un bucle que lee comandos de la entrada estándar y los distribuye
 * a las funciones de nuestro Diccionario.
 */
void Interprete::ejecutarCuacker(){

    // 1. Abrimos la conexión SQLite (garantiza persistencia durante toda la sesión)
    Persistencia db("cuacs.db"); // BBDD en el directorio raíz del proyecto

    // 2. Cargamos los cuacs desde la BBDD (también sincroniza el contador de IDs)
    list<Cuac> cargados = db.cargar(); // Lista de objetos Cuac reconstruidos desde SQLite

    // 3. Recargamos las estructuras en RAM (TablaHash + ArbolAVL + Índices)
    //  En este punto tenemos que "_persistencia == nullptr", por lo que NO se re-insertan en SQLite
    for (const Cuac& c : cargados) {
        _servicio_datos.insertar(c); // Insertamos todos los cuacs en memoria
    }

    // 4. Activamos el auto-guardado; a partir de aquí cada insertar/eliminar escribe a SQLite
    _servicio_datos.setPersistencia(&db); // Establecemos el puntero a Persistencia

    // 5. Informamos al usuario de los cuacs cargados (si los hay)
    if (!cargados.empty()) {
        cout << "[i] Base de datos restaurada. " << _servicio_datos.numElem() << " cuacs disponibles." << endl;
    }

    string comando;

    // Entramos en nuestro bucle de control infinito hasta que se solicite el cierre con 'exit'
    while(cin >> comando){

        // Gestionamos el cierre de nuestra sesión
        if (comando == "exit"){

            // La conexión SQLite se cierra automáticamente al salir del scope
            cout << "[i] Hasta la proxima!" << endl;
            break; // Salimos del bucle y finaliza el programa

        } else if (comando == "check") { 

            // Con auto-guardado, 'check' ejecuta una verificación de integridad de la BBDD
            db.verificarIntegridad();

        } else if (comando == "mcuac" || comando == "pcuac"){ // ej. "mcuac john hello world" o "pcuac john 123"

            // Gestionamos la creación de nuevas publicaciones
            Cuac c;

            // Intentamos leer el cuac; si la entrada es correcta, lo incorporamos a nuestro sistema
            if( c.read_cuac(comando) ){

                _servicio_datos.insertar(c); // Inserta en TablaHash+ArbolAVL+Índices Y en SQLite (guardado automático)

                // Informamos al usuario de cuántas publicaciones totales gestionamos ahora
                cout << _servicio_datos.numElem() << " cuac" << endl;
            }

        } else if (comando == "last"){ // ej. "last 5"

            // Procesamos la solicitud de ver los mensajes más recientes

            int n; // Número de mensajes a mostrar
            cin >> n;

            // Si hay un usuario logueado, mostramos su timeline personalizado
            if (!_usuario_activo.empty()) {
                _servicio_datos.lastPersonalizado(n, _usuario_activo);
            } else {
                // Sin login: timeline global (comportamiento original)
                _servicio_datos.last(n);
            }

        } else if (comando == "follow"){ // ej. "follow john"
 
            // Seguimos a un usuario y mostramos sus cuacs (Opción A)
            string usuario;
            cin >> usuario;
 
            if (!_servicio_datos.existeUsuario(usuario)) {
                cout << "[!] El usuario '" << usuario << "' no esta registrado en Cuacker." << endl;
            } else {

                // Si hay login activo, registramos la relación de seguimiento
                if (!_usuario_activo.empty()) {

                    // No permitimos seguirse a uno mismo
                    if (usuario == _usuario_activo) {
                        cout << "[!] No puedes seguirte a ti mismo." << endl;
                    } else {
                        _servicio_datos.seguir(_usuario_activo, usuario);
                        cout << "[i] Ahora sigues a '" << usuario << "'." << endl;
                    }
                }

                // Siempre mostramos los cuacs del usuario (comportamiento original)
                _servicio_datos.follow(usuario);
            }

        } else if (comando == "delete") { // ej. "delete 123"

            // Procesamos la solicitud de borrado
            int id_a_borrar;
            cin >> id_a_borrar;

            _servicio_datos.eliminar(id_a_borrar); // Borra de RAM Y de SQLite (guardado automático)

        } else if (comando == "date"){ // ej. "date 01/01/2022 31/12/2022"

            // Filtramos publicaciones en un periodo de tiempo determinado
            Fecha fecha_inicio, fecha_fin;

            // Capturamos los límites temporales
            fecha_inicio.leer_fecha();
            fecha_fin.leer_fecha();

            _servicio_datos.date(fecha_inicio, fecha_fin);

        } else if (comando == "tag"){ // ej. "tag #programacion"

            // Buscamos publicaciones que contengan un hashtag específico
            string hashtag;
            cin >> hashtag;

            _servicio_datos.tag(hashtag);

        } else if (comando == "search"){ // ej. "search hola"

            // Buscamos publicaciones que contengan una subcadena en su texto
            string texto;

            // Leemos toda la línea con getline() para permitir búsquedas con espacios
            cin.ignore();
            getline(cin, texto);

            _servicio_datos.search(texto);

        } else if (comando == "stats"){ // ej. "stats"

            // Mostramos las estadísticas generales del sistema
            _servicio_datos.stats();

        } else if (comando == "help"){ // ej. "help"

            // Mostramos la lista de comandos disponibles
            cout << "\n=== [i] Comandos disponibles en Cuacker ===" << endl;
            cout << "  mcuac <usuario> <fecha> <mensaje>  - Publicar un cuac manual" << endl;
            cout << "  pcuac <usuario> <fecha> <numero>   - Publicar un cuac predefinido" << endl;
            cout << "  last <N>                           - Ver los ultimos N cuacs" << endl;
            cout << "  follow <usuario>                   - Seguir a un usuario (y ver sus cuacs)" << endl;
            cout << "  unfollow <usuario>                 - Dejar de seguir a un usuario" << endl;
            cout << "  following                          - Ver a quien sigues" << endl;
            cout << "  followers                          - Ver quien te sigue" << endl;
            cout << "  delete <id>                        - Borrar un cuac permanentemente" << endl;
            cout << "  date <fecha_ini> <fecha_fin>       - Cuacs en un rango de fechas" << endl;
            cout << "  tag <#hashtag>                     - Buscar por hashtag" << endl;
            cout << "  search <texto>                     - Buscar texto en los cuacs" << endl;
            cout << "  stats                              - Estadisticas del sistema" << endl;
            cout << "  check                              - Verificar integridad de la BBDD" << endl;
            cout << "  login <usuario>                    - Iniciar sesion" << endl;
            cout << "  logout                             - Cerrar sesion" << endl;
            cout << "  whoami                             - Ver usuario activo" << endl;
            cout << "  help                               - Mostrar esta ayuda" << endl;
            cout << "  exit                               - Cerrar Cuacker" << endl;
            cout << "=============================\n" << endl;

        } else if (comando == "login"){ // ej. "login alice"

            // Establecemos la identidad del usuario activo para la sesión
            string usuario;
            cin >> usuario;

            _usuario_activo = usuario;

            // Cargamos los seguidos de este usuario desde SQLite a la caché en RAM
            list<string> seguidos = db.cargarSeguidos(usuario);
            _servicio_datos.cargarGrafo(usuario, seguidos);

            cout << "[i] Sesion iniciada como '" << _usuario_activo << "'.";

            if (!seguidos.empty()) {
                cout << " Siguiendo a " << seguidos.size() << " usuario(s).";
            }

            cout << endl;

        } else if (comando == "logout"){ // ej. "logout"

            // Cerramos la sesión del usuario activo
            if (_usuario_activo.empty()) {
                cout << "[!] No hay sesion activa." << endl;
            } else {
                cout << "[i] Sesion de '" << _usuario_activo << "' cerrada. Timeline global restaurado." << endl;
                _usuario_activo.clear();
            }

        } else if (comando == "whoami"){ // ej. "whoami"

            // Mostramos el usuario activo actual
            if (_usuario_activo.empty()) {
                cout << "[i] No hay sesion activa. Usa 'login <usuario>' para iniciar sesion." << endl;
            } else {
                cout << "[i] Sesion activa: " << _usuario_activo << endl;
            }

        } else if (comando == "unfollow"){ // ej. "unfollow bob"

            // Dejamos de seguir a un usuario
            string usuario;
            cin >> usuario;

            if (_usuario_activo.empty()) {
                cout << "[!] Necesitas iniciar sesion para usar 'unfollow'. Usa 'login <usuario>'." << endl;
            } else {
                bool eliminado = _servicio_datos.dejarDeSeguir(_usuario_activo, usuario);

                if (eliminado) {
                    cout << "[i] Has dejado de seguir a '" << usuario << "'." << endl;
                } else {
                    cout << "[!] No sigues a '" << usuario << "'." << endl;
                }
            }

        } else if (comando == "following"){ // ej. "following"

            // Mostramos la lista de usuarios que sigue el usuario activo
            if (_usuario_activo.empty()) {
                cout << "[!] Necesitas iniciar sesion para usar 'following'. Usa 'login <usuario>'." << endl;
            } else {
                const unordered_set<string>& seguidos = _servicio_datos.getSeguidos(_usuario_activo);

                if (seguidos.empty()) {
                    cout << "[i] No sigues a nadie." << endl;
                } else {
                    cout << "\n=== Usuarios que sigue " << _usuario_activo << " ===" << endl;
                    int contador = 1;
                    for (const string& s : seguidos) {
                        cout << "  " << contador << ". " << s << endl;
                        contador++;
                    }
                    cout << "Total: " << seguidos.size() << " usuario(s)" << endl;
                    cout << "============================\n" << endl;
                }
            }

        } else if (comando == "followers"){ // ej. "followers"

            // Mostramos la lista de seguidores del usuario activo (consulta directa a SQLite)
            if (_usuario_activo.empty()) {
                cout << "[!] Necesitas iniciar sesion para usar 'followers'. Usa 'login <usuario>'." << endl;
            } else {
                list<string> seguidores = db.cargarSeguidores(_usuario_activo);

                if (seguidores.empty()) {
                    cout << "[i] Nadie te sigue (todavia)." << endl;
                } else {
                    cout << "\n=== Seguidores de " << _usuario_activo << " ===" << endl;
                    int contador = 1;
                    for (const string& s : seguidores) {
                        cout << "  " << contador << ". " << s << endl;
                        contador++;
                    }
                    cout << "Total: " << seguidores.size() << " seguidor(es)" << endl;
                    cout << "============================\n" << endl;
                }
            }

        } else {
            // Informamos al usuario de que no reconocemos el comando introducido
            cerr << "Error: comando '" << comando << "' no reconocido. Escribe 'help' para ver la lista de comandos disponibles. " << endl;
        }

        // Refuerzo: Si algun comando dejo 'cin' en estado de error, lo limpiamos aqui para no morir
        if (std::cin.fail()) {
            std::cin.clear(); 
            std::string recuperacion;
            std::getline(std::cin, recuperacion); 
        }
    }
}
