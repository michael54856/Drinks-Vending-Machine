#include <Servo.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
int out = 2;
int s0 = 3;
int s1 = 4;
int s2 = 5;
int s3 = 6;

int RedColor = 0;
int GreenColor = 0;
int BlueColor = 0;

int colorArr[8][3] = {{7,4,10},{15,10,6},{14,11,11},{8,9,6}, {5,6,8},{6,12,10}, {16,17,13}, {12,10,11}};

int RedDrinks = 100; //紅色飲料剩餘量
int GreenDrinks = 100; //綠色飲料剩餘量
int BlueDrinks = 100; //藍色飲料剩餘量


Servo myservo;  // 建立SERVO物件

void ScanColor() 
{
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  RedColor = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  BlueColor = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  GreenColor = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

}

void giveDrinks(int n)
{
    //500ms 是用來充滿水管
    //9 -> r
    //10 -> g
    //13 -> b
    if(n == 0)//紅
    {
        digitalWrite(9,LOW); 
        delay(2000);
        digitalWrite(9,HIGH); 
        RedDrinks -= 15;
    }
    else if(n == 1)//藍
    {
        digitalWrite(13,LOW); 
        delay(2000);
        digitalWrite(13,HIGH); 
        BlueDrinks -= 15;
    }
    else if(n == 2)//綠
    {
        digitalWrite(10,LOW); 
        delay(2000);
        digitalWrite(10,HIGH); 
        GreenDrinks -= 15;
    }
    else if(n == 3)//紫
    {
        digitalWrite(9,LOW); 
        digitalWrite(13,LOW); 
        delay(1300);
        digitalWrite(9,HIGH); 
        digitalWrite(13,HIGH); 
        RedDrinks -= 8;
        BlueDrinks -= 8;
    }
    else if(n == 4)//天藍
    {
        digitalWrite(10,LOW); 
        digitalWrite(13,LOW); 
        delay(1300);
        digitalWrite(10,HIGH); 
        digitalWrite(13,HIGH); 
        GreenDrinks -= 8;
        BlueDrinks -= 8;
    }
    else if(n == 5)//橘
    {
        digitalWrite(9,LOW); 
        digitalWrite(10,LOW); 
        delay(1300);
        digitalWrite(9,HIGH); 
        digitalWrite(10,HIGH); 
        RedDrinks -= 8;
        GreenDrinks -= 8;
    }
    else if(n == 6)//黑
    {
        digitalWrite(9,LOW); 
        digitalWrite(10,LOW); 
        digitalWrite(13,LOW); 
        delay(1000);
        digitalWrite(9,HIGH); 
        digitalWrite(10,HIGH); 
        digitalWrite(13,HIGH); 
        RedDrinks -= 5;
        GreenDrinks -= 5;
         BlueDrinks -= 5;
    }
    else//預設顏色,沒有投入代幣
    {
        
    }

    
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(10,HIGH); //低電平觸發，LOW時繼電器觸發
  digitalWrite(9,HIGH); //低電平觸發，LOW時繼電器觸發
  digitalWrite(13,HIGH); //低電平觸發，LOW時繼電器觸發

   myservo.attach(11);  // 設定要將伺服馬達接到哪一個PIN腳
   myservo.write(25);  //旋轉到0度，就是一般所說的歸零
}

// the loop function runs over and over again forever
void loop() {
 
  if (Serial.available()) 
  {
     char comingValue =Serial.read(); // 從串列埠緩衝
     Serial.print(comingValue);
     
     if(comingValue == '1')//藍芽傳東西過來
     {
        ScanColor();
        /*
        Serial.print("Red: "); Serial.print(RedColor); Serial.print("\n");
        Serial.print("Green: "); Serial.print(GreenColor); Serial.print("\n");
        Serial.print("Blue : "); Serial.print(BlueColor); Serial.print("\n");
        */

        //旋轉伺服馬達讓代幣掉下去
        myservo.write(180);  
        delay(1500);  
        myservo.write(25);  

        int minDiff = 10000;
        int chooseColor;
        
        for(int i = 0; i < 8; i++) //找出數值最相近的顏色
        {
            int sum = 0;
            sum += abs(RedColor-colorArr[i][0]);
            sum += abs(GreenColor-colorArr[i][1]);
            sum += abs(BlueColor-colorArr[i][2]);
            if(sum < minDiff)
            {
                minDiff = sum;
                chooseColor = i;
            }
        }
        giveDrinks(chooseColor);

        String s1 = String(RedDrinks) + "," + String(GreenDrinks) + "," + String(BlueDrinks);
        char Buffer[100];
        s1.toCharArray(Buffer, 100);
        Serial.write(Buffer);
     }
  }
}
