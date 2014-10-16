#include <Console.h>
#include <Bridge.h>
#include <HttpClient.h>


// the setup function runs once when you press reset or power the board
void setup() {

  Bridge.begin();
  Console.begin();
  // initialize digital pin 13 as an output.
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
}

char preamble[] = "html>";

// the loop function runs over and over again forever
void loop() {
  digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  HttpClient client;
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  client.get("http://192.168.0.3/Hello.html");
  
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  int index=0;
  bool matches;
  char c = '';
  
  while (client.available()
  && (index==0
  || index<sizeof(preamble)
  || (matches = preamble[index] == (c = client.read()))))
      {
        Console.print("discard ");
        Console.println(c);
      }

  while (matches
      && client.available()
      && (c = client.read()) != '<')
      {
        Console.print(c);
      }
 
  Console.println();
  digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
