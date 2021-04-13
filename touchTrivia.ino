/* Adam Welker   BYU  ECEN 301    03/2021
 *  
 *  Lab 6 -- touch-trivia
 *  
 *  Purpose: Demonstrate the Capacitive Sensor library by making a game
 *  of trivia operable via homemade touch screen grid
 *  
 *  INPUTS: None
 *  
 *  OUTPUT: Serial Terminal will display the questions, answers, and final score
 *  DEPENDENCIES: the CapacitiveSensor Library and Vector Library, available on the library manager in the arduino IDE
 *  
 */
#include <CapacitiveSensor.h>

 
// TO DEBUG, CHANGE THESE VARIABLES
#define X1_CAP 50
#define X2_CAP 70
#define Y1_CAP 30
#define Y2_CAP 30


#define MODE  1 // if mode is below 0, program is in debug mode

// import libraries and set up grid vars



using namespace std;

const int y1_pins[] = {11,8};
const int y2_pins[] = {11,13};

const int x1_pins[] = {5,7};
const int x2_pins[] = {5,2};


// make Capacitive Sensors

CapacitiveSensor x1 = CapacitiveSensor(x1_pins[0],x1_pins[1]);
CapacitiveSensor x2 = CapacitiveSensor(x2_pins[0],x2_pins[1]);

CapacitiveSensor y1 = CapacitiveSensor(y1_pins[0],y1_pins[1]);
CapacitiveSensor y2 = CapacitiveSensor(y2_pins[0],y2_pins[1]);



// make trivia variables

#define num_questions 4

char *questions[] = {"What is the Captial of Brazil?\n a.)Manaus\t b.) Sao Paulo\n c.)Rio de Janeiro\t d.)Brasilia",
                     "What is the Captial of Australia?\n a.)Canberra\t b.) Sidney\n c.)Perth\t d.)Adelaide",
                      "What is the Captial of Slovakia?\n a.)Zilina\t b.)Presov\n c.)Bratislava\t d.)Kosice",
                      "What is the Captial of Belgium?\n a.)Antwerp\t b.)Ghent\n c.)Brussels\t d.)Bruges"};

char answers[] = {'d', 'a', 'c', 'c'};

int turn;
int score;
char ans = 'x';
 


void setup()
{
  // Open a serial connection
  Serial.begin(9600);


  // set the turn and score to 0
  turn = 0;
  score = 0;

  Serial.println("Welcome to World Geography Trivia! Let's Begin!");
  delay(2000);
}

void loop()
{
  // check for debug mode   
  if(MODE < 0)
  {
    //Read the capacitance at each grid value
    int x1_read = x1.capacitiveSensor(30);
    int x2_read = x2.capacitiveSensor(30);
    
    int y1_read = y1.capacitiveSensor(30);
    int y2_read = y2.capacitiveSensor(30);

    //print all the values
    Serial.print("x1: ");   
    Serial.print(x1_read);
    Serial.print("\tx2: ");
    Serial.print(x2_read);
    Serial.print("\ty1: ");


    Serial.print(y1_read);
    Serial.print("\ty2:");
    Serial.println(y2_read);
    return;
  } 

  
  // ------------------------------------------------------  The trivia implementation begins here
  
  // The trivia works in three phases until turns % 3 == questions.size()
  // Phase 1 -- the arudiuno prints the questions
  // Phase 2 -- the arduino waits for the user to hold a touch screen node
  // Phase 3 -- the arduino will evaluate the user's choice and add a point if correct. the process then resets



  // if the game is over
  if(turn > num_questions * 3)
  {
    return;
  }
  else if(turn == num_questions * 3) // or is ending 
  {
    Serial.print("Thanks for playing! Your score was: ");
    Serial.print(score);
    Serial.print('/');
    Serial.println(num_questions);
    turn++;
  }
  else if(turn % 3 == 0) // phase 1
  {
    Serial.println(questions[turn/3]); // print the question, move on!
    turn++;
  }
  else if(turn % 3 == 1) // phase 2
  {
    int answerCount = 0;


    ans == 'x';

    // repeat until we get the same answer 7 times
    while(answerCount < 15)
    {      
       int x_index = 0;
       int y_index = 0;
      
      //Read the capacitance at each grid value
      int x1_read = x1.capacitiveSensor(30);
      int x2_read = x2.capacitiveSensor(30);
      
      int y1_read = y1.capacitiveSensor(30);
      int y2_read = y2.capacitiveSensor(30);
  
      if(x1_read > X1_CAP)
      {
        x_index = 1;
      }
      else if(x2_read > X2_CAP)
      {
        x_index = 2;
      }
  
      if(y1_read > Y1_CAP)
      {
        y_index = 1;
      }
      else if(y2_read > Y2_CAP)
      {
        y_index = 2;
      }

      // now determine the answer

      char newAns = ' ';

      if(x_index == 1 && y_index == 2)
      {
        newAns = 'a';
      }
      else if(x_index == 2 && y_index == 2)
      {
        newAns = 'b';
      }
      else if(x_index == 1 && y_index == 1)
      {
        newAns = 'c';
      }
      else if(x_index == 2 && y_index == 1)
      {
        newAns = 'd';
      }

      // check for repeating answers

      if(newAns == ans && newAns != ' ')
      {
        answerCount++;
      }
      else
      {
        ans = newAns;
        answerCount = 0;
      }

      
    }

    Serial.print("Your answer was: ");
    Serial.println(ans);
    delay(500);

    turn++;
  }
  else if(turn % 3 == 2) // phase 3
  {
    // compare against the real answer
    bool correct = (ans == answers[turn/3]);
    
    if(correct)
    {
      Serial.println("Correct!");
      score++;
    }
    else
    {
      Serial.print("Sorry, the answer was: ");
      Serial.println(answers[turn/3]);
    }

    turn++;
    ans = 'x';
    delay(3000);
  }
    
}
