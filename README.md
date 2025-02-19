# Web-Server
Developed a webserver to handle get requests using C
,Inorder to run this file
1. In terminal type "gcc main.c handleclient.c mime_utils.c log_utils.c -o main -lws2_32"
2. Then ".\main.exe"
3. Then you can type "http://localhost:8089"
4. If you want to check multithreading process you can open two browsers and give "http://localhost:8080/index.html" in one browser and "http://localhost:8089/about.html" in another browser.

<ul>
   <li></li>
</ul>Using .\test.bat command you can stimulate how 10000 requests are handled using multi threading
