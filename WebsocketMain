/* Questo programma utilizza i metodi visti in HTTP-GET per aprire una websocket e inviare al server desiderato
 * la stringa "Hello World" periodicamente.
 * Poichè la libreria Websocket è stata scritta solo dopo la realizzazione di mbed-os 5 è stato necessario modificare il file 
 * "Websocket.cpp" (scaricabile dal sito mbed) per renderlo compatibile con mbed-os 2 (necessario per il funzionamento di tutte
 * le altre librerie utilizzate ma non ancora portate su mbed-os 5).
 * Il file "Websocket.cpp" modificato è presente nella cartella di questo file.
 * Ancora non è stata effettuata una prova vera e propria con un server online ma è stato verificato tramite Wireshark che
 * la richiesta per l'apertura della websocket è eseguita come descritto nella documentazione sull'upgrade da http a websocket.
 */

#include "mbed.h"
#include "SpwfInterface.h"
#include "Websocket.h"
#include "TCPSocket.h"

Serial pc(USBTX, USBRX);
DigitalOut led(LED1);
SpwfSAInterface spwf(D8, D2, false);

int main() {   

    char * ssid = "*****"; //inserire il prorpio ssid
    char * seckey = "*****"; //inserire la propria password  
    // announce
    pc.printf("Websocket Example v1.0.0\r\n");
    
    // Create a websocket instance
    
    if(spwf.connect(ssid, seckey, NSAPI_SECURITY_WPA2)) {      
        pc.printf("\r\nnow connected\r\n");
    } else {
        pc.printf("\r\nerror connecting to AP.\r\n");
        return -1;
    }   
    
    
    const char *ip = spwf.get_ip_address();
    const char *mac = spwf.get_mac_address();
    
    
    pc.printf("\r\nIP Address is: %s\r\n", (ip) ? ip : "No IP");
    pc.printf("\r\nMAC Address is: %s\r\n", (mac) ? mac : "No MAC");    
    
   
    Websocket ws("ws://192.168.0.2:8000/", &spwf); //ip e path di un server di prova locale  
    int connect_error = ws.connect();
    if(connect_error<0){ 
        pc.printf("error");
    }else{
        pc.printf("OK");
    }        
    
    // begin main loop
    while (true) {

        // blink... 
        led = !led; 
        wait(0.5);
        
        int error_c = ws.send("Hello World\r\n");
    }
}
