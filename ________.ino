#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define WIFI_SSID "HH72VM_DEBA_2.4G" //WIFI
#define WIFI_PASSWORD "37QAhMtK" //Password
#define BOT_TOKEN "5678120868:AAHC_FAEzTx_Q1QkRLNLjFCpahIMC8fvZZs" //Telegram BOT Token

#define HEATER 12 // GPIO12
#define TEMP_SENSOR 15 // GPIO15

OneWire oneWire(TEMP_SENSOR);
DallasTemperature DS18B20(&oneWire);

const unsigned long BOT_MTBS = 1000; // mean time between scan messages

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done
bool cooking;
unsigned int cookingTime;
int cookingT;
unsigned int TimeStartCooking = 0;
bool ReadyToCooking = false;
float tempC; // temperature in Celsius

unsigned int ConvertToMS(unsigned int minutes){
  return minutes * 60 * 1000;
}

void Select(unsigned int inputTime, int inputTemp){
  cookingTime = ConvertToMS(inputTime);
  cookingT = inputTemp;
  cooking = true;
}

void handleNewMessages(int numNewMessages){
  DS18B20.requestTemperatures();       // send the command to get temperatures
  tempC = DS18B20.getTempCByIndex(0);  // read temperature in °C
  Serial.print(tempC);    // print the temperature in °C
  Serial.println("°C");

  
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);
  String global_chat_id = " ";
  for (int i = 0; i < numNewMessages; i++){
    String chat_id = bot.messages[i].chat_id;
    global_chat_id = chat_id;
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";
    if (text == "/start"){
      String welcome = "Привіт, " + from_name + ". Мене звати Єва, я бот-помічник розумної йогуртниці.\n";
      bot.sendMessage(chat_id, welcome, "");
      welcome = "/help - для детальної інформації команд";
      bot.sendMessage(chat_id, welcome, "");
    }
    else if (text == "/status"){
        bot.sendMessage(chat_id, String(tempC)+"°C", "Markdown");
        if(cooking && !TimeStartCooking){
          bot.sendMessage(chat_id, "Залишилось "+String((cookingTime - (millis()-TimeStartCooking)) / 60000)+ " хвилин до повного приготування",  "Markdown");
        }
    } 
    else if (text == "/help"){
      String helping = "/status - Перевірка часу готовності та температури\n/recipes - Каталог рецептів\n/select Назва напою (на латині) - для приготування напою\n/about Назва напою (на латині) - інформація, рецепт напою та час приготування\n/device - Про йогуртницю";
      bot.sendMessage(chat_id, helping, "Markdown");
    }
    else if (text == "/recipes"){
      String recipes = "Список назв рецептів та їхня назва в боті:\n";
      recipes += "- Кефір та напій “Біфідок” - Bifidok\n";
      recipes += "- Для напою “Ацидофілін” - Acidophilin\n";
      recipes += "- Для напою “Ряжанка” - Ryazhenko\n";
      recipes += "- Для напою “Біойогурт” - BioYogurt\n";
      recipes += "- Для напою “Ацидолакт” - Acidolact\n";
      recipes += "- Для напою “Біфілін” - Bifilin\n";
      recipes += "- Для напою “Геролакт” - Herolact\n";
      bot.sendMessage(chat_id, recipes, "Markdown");
      recipes = "/select Назва напою (на латині) - для приготування напою";
      bot.sendMessage(chat_id, recipes, "Markdown");
      recipes = "/about Назва напою (на латині) - щоб дізнатися інформацію про напій";
      bot.sendMessage(chat_id, recipes, "Markdown");
    }
    else if(text.indexOf("/select") != -1){
      if(text.indexOf("Bifidok") != -1){
        if(!cooking){
            Select(600,22);//Час у хвилинах, температура в С
            bot.sendMessage(chat_id, "Ви обрали: кефір “Біфідок”", "Markdown");
        }
        else
          bot.sendMessage(chat_id, "Зачекайте, поки приготується попереднє замовлення", "Markdown");
        }
      }
      else if(text.indexOf("Acidophilin") != -1){
        if(!cooking){
            Select(300,32);
            bot.sendMessage(chat_id, "Ви обрали: напій “Ацидофілін”", "Markdown");
        }
        else{
          bot.sendMessage(chat_id, "Зачекайте, поки приготується попереднє замовлення", "Markdown");
        }
      }
      else if(text.indexOf("Ryazhenko") != -1){
        if(!cooking){
            Select(300,42);
            bot.sendMessage(chat_id, "Ви обрали: напій “Ряжанка”", "Markdown");
        }
        else{
            bot.sendMessage(chat_id, "Зачекайте, поки приготується попереднє замовлення", "Markdown");
        }
      }
      else if(text.indexOf("BioYogurt") != -1){
        if(!cooking){
            Select(180,42);
            bot.sendMessage(chat_id, "Ви обрали: напій “Біойогурт”", "Markdown");
        }
        else{
            bot.sendMessage(chat_id, "Зачекайте, поки приготується попереднє замовлення", "Markdown");
        }
      }
      else if(text.indexOf("Acidolact") != -1){
         if(!cooking){
            Select(300,42);
            bot.sendMessage(chat_id, "Ви обрали: напій “Ацидолакт”", "Markdown");
         }
        else{
            bot.sendMessage(chat_id, "Зачекайте, поки приготується попереднє замовлення", "Markdown");
        }
      }
      else if(text.indexOf("Bifilin") != -1){
        if(!cooking){
            Select(360,44);
            bot.sendMessage(chat_id, "Ви обрали: напій “Біфілін”", "Markdown");
        }
        else{
            bot.sendMessage(chat_id, "Зачекайте, поки приготується попереднє замовлення", "Markdown");
        }
      }
      else if(text.indexOf("Herolact") != -1){
         if(!cooking){
            Select(540,37);
            bot.sendMessage(chat_id, "Ви обрали: напій “Геролакт”", "Markdown");
         }
        else{
            bot.sendMessage(chat_id, "Зачекайте, поки приготується попереднє замовлення", "Markdown");
        }
      }
      else{
        bot.sendMessage(chat_id, "Поки що такого режиму немає :(", "Markdown");
      }
    
    
    if(text.indexOf("/about") != -1){
      String info = "";
      if(text.indexOf("Bifidok") != -1){
        bot.sendMessage(chat_id, "“Біфідок”", "Markdown");
        info = "Кисломолочный продукт, що його отримують із незбираного або зі знежиреного коров’ячого молока шляхом кисломолочного бродіння із застосуванням кефірних «грибків».\n";
        info += "Для приготувавання цього напою Вам потрібно:\n";
        info += "покупне коров’яче молоко та рідкі біфідобактерії, які продаються в будь-якій аптеці в сухому вигляді.\n";
        info += "Ампулу такого засобу розводять кип’яченою водою і додають в склянку з прокип’яченим молоком.";
        bot.sendMessage(chat_id, "" + info, "Markdown");
        info = "Вся технологія зав’язана на дотриманні температурного режиму, що не перевищує +22 градуса за Цельсієм.";
        bot.sendMessage(chat_id, "" + info, "Markdown");
        bot.sendMessage(chat_id, "Час приготування: 10 годин.", "Markdown");             
      }
      else if(text.indexOf("Acidophilin") != -1){
        bot.sendMessage(chat_id, "“Ацидофілін”", "Markdown");
        info = "Дієтичний кисломолочний продукт, що отримують шляхом збродження молока ацидофільною паличкою.\n";
        info += "Для приготувавання цього напою Вам потрібно:\n";
        info += "Коров’яче, козяче або овече молоко, суміш для заквашування, цукр та ягоди.";
        bot.sendMessage(chat_id, "" + info, "Markdown");
        info = "Молоко квасять при нагріванні до 32 градусів.";
        bot.sendMessage(chat_id, "" + info, "Markdown");
        bot.sendMessage(chat_id, "Триває цей процес близько 5 годин.", "Markdown");                                                    
      }
      else if(text.indexOf("Ryazhenko") != -1){
        bot.sendMessage(chat_id, "“Ряжанка”", "Markdown");
        info = "Традиційний молочний продукт української кухні, що виготовляється з пряженого молока.\n";
        info += "Для приготувавання цього напою Вам потрібно:\n";
        info += "Пряжене молоко, закваска.";
        bot.sendMessage(chat_id, "" + info, "Markdown");
        info = "Режим працює до 42 градусів.";
        bot.sendMessage(chat_id, "" + info, "Markdown");
        bot.sendMessage(chat_id, "Ряжанка потребує 5 годин приготування.", "Markdown");        
      }
      else if(text.indexOf("BioYogurt") != -1){
        bot.sendMessage(chat_id, "“Біойогурт”", "Markdown");
        info = "Цілющі пробіотики які мають лікувальний ефект, як і усі кисломолочні та біопродукти.\n";
        info += "Для приготувавання цього напою Вам потрібно:\n";
        info += "Молоко, закваска, фрукти.";
        bot.sendMessage(chat_id, "" + info, "Markdown");
        info = "Для приготування біойогурта температура досягає 42 градусів.";
        bot.sendMessage(chat_id, "" + info, "Markdown");
        bot.sendMessage(chat_id, "Один із найшвидких для приготування напій - усього 3 години.", "Markdown");
      }
      else if(text.indexOf("Acidolact") != -1){
        bot.sendMessage(chat_id, "“Ацидолакт”", "Markdown");
        info = "продукт для підтримки мікрофлори людям, які приймають антибіотики та інші ліки.\n";
        info += "Для приготувавання цього напою Вам потрібно:\n";
        info += "Молоко, закваска, фрукти.";
        bot.sendMessage(chat_id, "" + info, "Markdown");
        info = "Напій готується на високій температурі, а саме до 42 градусів.";
        bot.sendMessage(chat_id, "Вже через 5 годин Ацидолакт буде готовий до споживання.", "Markdown");
      }
      else if(text.indexOf("Bifilin") != -1){
        bot.sendMessage(chat_id, "“Біфілін”", "Markdown");
        info = "Кисломолочний продукт, який ідеально підійде для підтримки сил та зміцнення імунітету.\n";
        info += "Для приготувавання цього напою Вам потрібно:\n";
        info += "Суха молочна суміш, вода, закваска.";
        bot.sendMessage(chat_id, "" + info, "Markdown");
        info = "Біфілін готується при 44 градусів Цельсія.";
        bot.sendMessage(chat_id, "" + info, "Markdown");
        bot.sendMessage(chat_id, "Зачекайте 6 годин, і Ви отримаєте це чудовий напій.", "Markdown");        
      }
      else if(text.indexOf("Herolact") != -1){
        bot.sendMessage(chat_id, "“Геролакт”", "Markdown");
        info = "Геролакт відрізняється від інших кисломолочних напоїв особливістю складу його мікрофлори та компонентного складу рецептури.\n";
        info += "Для приготувавання цього напою Вам потрібно:\n";
        info += "Молоко, закваска стрептосан.";
        bot.sendMessage(chat_id, "" + info, "Markdown");
        info = "";
        bot.sendMessage(chat_id, "Температура цього режиму досягає 37 градусів." + info, "Markdown");
        bot.sendMessage(chat_id, "І готується Геролакт 9 годин.", "Markdown");
      }
      else{
        bot.sendMessage(chat_id, "Поки що такого рецепту не існує :(", "Markdown");
      }
    }
    //else{
        //bot.sendMessage(chat_id, "Поки що такого команди немає :(", "Markdown");
      //}
  }
}

void setup() {

  Serial.begin(115200);
  pinMode(HEATER, OUTPUT);
  DS18B20.begin();// initialize the DS18B20 sensor 
  //digitalWrite(HEATER, HIGH);

  cooking = false;
  cookingTime = 0;
  cookingT = 0;

  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop() {
  DS18B20.requestTemperatures();       // send the command to get temperatures
  tempC = DS18B20.getTempCByIndex(0);  // read temperature in °C
    if(cooking){//Переходим в процесс готовки
      if(tempC > cookingT && !ReadyToCooking){
        digitalWrite(HEATER, LOW);
      }
      else if(tempC < cookingT && !ReadyToCooking){
        digitalWrite(HEATER, HIGH);
      }
      else if(tempC == cookingT && !ReadyToCooking){
        ReadyToCooking = true;
      }
      if(ReadyToCooking){
        if(!TimeStartCooking){
          TimeStartCooking = millis();
        }
        if(millis() > cookingTime + TimeStartCooking){//Запуск таймера
          cooking = false;
          TimeStartCooking = 0;
          //Тут создать сообщение о том что процесс готовки закончен 
        }
        else{
        //Тут проверки на температуру
        if(tempC >= cookingT){
        digitalWrite(HEATER, LOW);
      }
      else{
        digitalWrite(HEATER, HIGH);
      }
        }
      }
    }
    if(millis() - bot_lasttime > BOT_MTBS)
    {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_lasttime = millis();
  }
}