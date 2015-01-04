This folder contains a small application which functions as a simple server for the R3X VM.
Compile for Linux using 'build.sh'. Then move it to your bin directory, from there do './server'.
You will see a prompt like this:
[/directory/]$:
To run an application, just type: 
run <app path>
After the application starts you will see that the prompt disappeared. To recover, switch
to your terminal and just press ENTER.
You can run as many applications as you want by repeating the steps above.
The client VMs process ID would be on it's window header, and would also be printed
by the server app when you execute it.
There are a few things you can mess with:
kill <client pid> - Kill a client
pause <client pid> - Pause a client
cont <client pid> - continue a client after pause
status <client pid> - make the client print VM status.
quit - Quits the server and all clients