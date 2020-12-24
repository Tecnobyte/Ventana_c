#include <windows.h>

const char g_szClassName[] = "myWindowClass"; // nombre que se le asignara al proceso de la ventana, servira para el handle

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

// datos adicionales

/*
    Microsoft Windows utiliza identificadores de ventana internamente para acceder a las ventanas. 
    Cada ventana tiene un número arbitrario específico. Haciendo aritmética con ese número obtendrás resultados impredecibles. 
    Utilice el nodo HWND para obtener el identificador de una ventana con un título determinado (que se muestra en el área de título de la ventana). 
    HWND recuperará todas las ventanas con un título determinado.
    esto podría ser más poderoso si lo desea, por ejemplo,
    hay muchas ventanas con un título vacío. Puede que le guste absolutamente utilizar GetSlice aquí.
*/

// se hace la llamada a la api
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    WNDCLASSEX wc; // es una estructura de datos. almacena la informacion de la ventana // http://winapi.freetechsecrets.com/win32/WIN32WNDCLASSEX.htm
    HWND hwnd; // obtiene el apuntador de la ventana de un grupo de ventanas
    MSG Msg; // Contiene información de mensajes de la cola de mensajes de un hilo.

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX); // tamaño de la ventana
    wc.style         = 0; // el estilo de la clase ->investigar cs y ws
    wc.lpfnWndProc   = WndProc; // puntero al window procesure de la clase
    wc.cbClsExtra    = 0; // cantidad extra de asignacion de memoria
    wc.cbWndExtra    = 0; // cantidad extra de asignacion de memoria por ventana 
    wc.hInstance     = hInstance; // Handle a la instancia de la aplicación (la que obtenemos en el primer parámetro de WinMain( ) )
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION); // Handle al ícono grande (32x32), mostrado cuando el usuario presiona Alt+Tab.
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW); // Handle al cursor que será mostrado sobre la ventana.
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); //Pincel para fijar el color de fondo de nuestra ventana.
    wc.lpszMenuName  = NULL; // Nombre del recurso Menú para usar con las ventanas de esta clase.
    wc.lpszClassName = g_szClassName; // Nombre para identificar la clase.
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION); // Handle al ícono pequeño (16x16), usado en la barra de tareas y en la esquina superior izquierda de la ventana.

    if(!RegisterClassEx(&wc)){
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx( // hwnd apunta a la ventana que se esta creando.
        WS_EX_CLIENTEDGE, // es el estilo extendido de la ventana, en este caso he configurado éste para darle un borde interno y hundido al marco de la ventana. Pon este valor en cero para observar la diferencia, o prueba con otros valores para ver que hacen.
        g_szClassName, // éste le dice al sistema que tipo de ventana crear. Debido a que queremos crear una ventana a partir de la clase que hemos registrado, usamos el nombre de dicha clase
        "The title of my window", // titulo de la ventana
        WS_OVERLAPPEDWINDOW, // es estilo de la ventana. Hay algunos de éstos y deberías mirarlos y experimentar para observar que hacen
        /* Los siguientes cuatro parámetros ( CW_USEDEFAULT, CW_USEDEFAULT,320,240)
            son las coordenadas X e Y de la esquina superior izquierda de la ventana y el alto y ancho de de la ventana,
            respectivamente. He puesto los campos X e Y con el valor CW_USEDEFALT para dejar que windows elija en qué lugar de la pantalla ubicar la ventana. 
            Recuerda que la parte izquierda de la pantalla tiene un valor de X igual a cero y se incrementa hacia la derecha y la parte superior de la pantalla
            tiene un valor cero de Y y se incrementa cuando avanzamos hacia abajo en la pantalla. Las unidades son los pixeles, el cual es la miníma unidad que una pantalla puede 
            mostrar en una resolución determinada.
        */

        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        /* (NULL,NULL,g_hInstance,NULL)
            corresponden al handle de la ventana padre, al handle del menú, al handle de la instancia de la aplicación y un puntero a los datoscon los cuales se creó la ventana. 
            En windows las ventanas son ordenadas en la pantalla de acuerdo a una jerarquía de padre e hijo. 
            Cuando ves un botón en una ventana, el botón es el Hijo de la ventana en la cual está contenido y dicha ventana es su Padre. En este ejemplo, 
            el handle al padre es nulo debido a que no tenemos padre, estamos en el nivel principal de las ventanas. 
            El handle al menú es nulo debido a que, por ahora, no tenemos menú. 
            El handle a la instancia de la aplicación es puesto con el valor que es pasado en el primer parámetro del WinMain( ).
            Los datos de creación (los culaes nunca uso) pueden ser usados para enviar información adicional a la ventana que está siendo creada. 
            También es nulo. Si estás preguntándote que es la palabra mágica NULL, es simplemente definida como cero (0). En realidad, en C está definida como ((void *) 0), 
            debido a que está destinada a ser usada con punteros. Por lo tanto probablemente obtengas algunos "warnings" si la utilizas para valores enteros, 
            de todas maneras esto depende del compilador y del nivel de warnings en la configuaración del mismo. Puedes elegir ignorar los warnings o usar el valor
            cero (0)en lugar de NULL.
        */
        NULL, NULL, hInstance, NULL
    );

    // verifica si se ha creado el apuntador de la ventana
    if(hwnd == NULL){
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow); // muestra la ventana en pantalla
    UpdateWindow(hwnd); // refresca la pantalla

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
