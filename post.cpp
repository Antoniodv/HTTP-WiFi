/* SpwfInterface NetworkSocketAPI Example Program
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
 
 /*siccome le librerie http utilizzano mbedos5 oppure delle librerie tcp diverse e a loro volta compatibili eventualmente con mbed os5
  * ho aperto due di queste librerie e ho visto come venivano inviate le richieste http che non sono altro che piu tcp send in serie
  * è possibile scrivere una semplice libreria http partendo da questo programma
 */

#include "mbed.h"
#include "SpwfInterface.h"
#include "TCPSocket.h"

Serial pc(USBTX, USBRX);
DigitalOut myled(LED1);
SpwfSAInterface spwf(D8, D2, false);


int main() {
    
    int err;    
    char * ssid = "NETGEAR";
    char * seckey = "supermario1985";  
          
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
    
    /*
    // Send a simple http request
    char sbuffer[] = "POST /update HTTP/1.1\r\nHost: 192.168.0.3:80\r\nAccept: application/json\r\nContent-Type: application/sparql-update\r\n\r\nPREFIX test:<http://sepa/test#> insert data {test:n test:p \"H\"}\r\n\r\n";
    int scount = socket.send(sbuffer, sizeof sbuffer);
    pc.printf("sent %d [%.*s]\r\n", scount, strstr(sbuffer, "\r\n")-sbuffer, sbuffer);
    */ 
    
    char buf[1500];
    char host[]="192.168.0.4:8000"; 
    char uri[]="/update";
    char body[]="PREFIX test:<http://sepa/test#> insert data {test:n test:p \"H\"}";

    // send request
    socket.send("POST ", 5);
    socket.send(uri, strlen(uri));
    socket.send(" HTTP/1.1\r\nHost: ", 17);
    socket.send(host, strlen(host));
    socket.send("\r\n", 2);
    //socket.send("Connection: close\r\n", 19);
    socket.send("Accept: application/json\r\n", 26);
    socket.send("Content-Type: application/sparql-update\r\n", 41);
    sprintf(buf, "Content-Length: %d\r\n", strlen(body));
    socket.send(buf, strlen(buf));
    socket.send("\r\n", 2);
    // post method
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
