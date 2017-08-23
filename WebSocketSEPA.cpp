/* In questo esempio è stato utilizzato il modulo WiFi IDW01M1 con la scheda Nucle F411RE per aprire una weboskcet 
 * per comunicare con il server locale del SEPA-engine e poter ricevere sia i messaggi di ping che di update.
 * A tal fine è stata modificata ulteriormente la Websocket.cpp che inizialmente non permetteva di ricevere i messaggi di ping.
 * I file Websocket.h e Websocket.cpp modificati sono presenti nella cartella HTTP-WiFi.

#include "mbed.h"
#include "SpwfInterface.h"
#include "Websocket.h"
#include "TCPSocket.h"

Serial pc(USBTX, USBRX);
DigitalOut led(LED1);
SpwfSAInterface spwf(D8, D2, false);

int main() {   

    char * ssid = "*****"; // inserire il prorpio
    char * seckey = "*****"; // inserire il proprio 
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
    
   
    Websocket ws("ws://192.168.0.7:9000/subscribe", &spwf); //inserire il proprio
    int connect_error = ws.connect();
    if(connect_error<0){ 
        pc.printf("error");
    }else{
        pc.printf("OK\r\n");
    }        
    int res = 0;
    res = ws.send("{'subscribe' : 'select * where {?s ?p ?o}'}\r\n");
    if (res<0) pc.printf("wc.send error");
    else pc.printf("messaggio inviato");
    
    // begin main loop
    char message [100] ;
    
    while(1) {        
        if(res = ws.read(message)){
            if(res<0) { 
                pc.printf("error");
           } else {
               pc.printf("MESSAGE: %s\r\n",message);
          }
        }
    }
}
