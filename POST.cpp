/* Segue un esempio di POST tramite il modulo WiFi IDW01M1 e scheda Nucle F411RE.
 * Per eseguire la richiesta sono state provate diverse librerie per client http ma non è stato possibile utilizzarle in quanto 
 * risultavano incompatibili con mbed-os2 o semplicemente con la libreria per le comunicazioni TCP.
 * Per questo motivo, questo codice, è stato adattato in modo da eseguire direttamente le POST tramite le funzioni messe a 
 * disposizione dalla libreria TCPSocket.h.
 * Il body della post che è stato utilizzato ha reso possibile l'aggiornamento del db blazegraph tramite SEPA-engine. 
 */ 

#include "mbed.h"
#include "SpwfInterface.h"
#include "TCPSocket.h"

Serial pc(USBTX, USBRX);
DigitalOut myled(LED1);
SpwfSAInterface spwf(D8, D2, false);


int main() {
    
    int err;    
    char * ssid = "*****"; //inserire il proprio
    char * seckey = "*****"; //inserire il proprio  
          
    pc.printf("\r\nX-NUCLEO-IDW01M1 mbed Application\r\n");     
    pc.printf("\r\nconnecting to AP\r\n");
            
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
    
   
    TCPSocket socket(&spwf);
    err = socket.connect("192.168.0.4", 8000);
    if(err!=0) 
    {
      pc.printf("\r\nCould not connect to Socket, err = %d!!\r\n", err); 
      return -1;
    } else pc.printf("\r\nconnected to host server\r\n"); 
    
    char buf[1500];
    char host[]="*****"; //inserire il proprio 
    char uri[]="/update";
    char body[]="PREFIX test:<http://sepa/test#> insert data {test:n test:p \"H\"}";

    //send request
    socket.send("POST ", 5);
    socket.send(uri, strlen(uri));
    socket.send(" HTTP/1.1\r\nHost: ", 17);
    socket.send(host, strlen(host));
    socket.send("\r\n", 2);
    socket.send("Accept: application/json\r\n", 26);
    socket.send("Content-Type: application/sparql-update\r\n", 41);
    sprintf(buf, "Content-Length: %d\r\n", strlen(body));
    socket.send(buf, strlen(buf));
    socket.send("\r\n", 2);
    socket.send(body, strlen(body));

    // Recieve a simple http response and print out the response line
    char rbuffer[64];
    int rcount = socket.recv(rbuffer, sizeof rbuffer);
    pc.printf("recv %d [%.*s]\r\n", rcount, strstr(rbuffer, "\r\n")-rbuffer, rbuffer);

    pc.printf("\r\nClosing Socket\r\n");
    socket.close();
    
    pc.printf("\r\nUnsecure Socket Test complete.\r\n");
    pc.printf ("Socket closed\n\r");
    spwf.disconnect();
    pc.printf ("WIFI disconnected, exiting ...\n\r");
    
    while(1){ 
      wait(1);
      myled = !myled;
    }
}
