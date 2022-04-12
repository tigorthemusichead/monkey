/* 
Входы:
    Adc0(pin2) -  значение работы (от 0 – 1,5 в)  от 500мв до 900мв – хорошо,  остальное плохо.
    Показываем в «статусе»
Io01(pin 11) – контроль работы (0 – работает).
Если мы включили, то можем выключить или обязательно выключить при потере связи. Если не мы включили только смотрим и показываем «статус» 
Io04(pin13) –состояние ошибки. ( 0 - ошибка)
Io16(pin4) – значение поворота ключа ( 0 – ключ в положении вкл. 1 – выкл.)
Выходы:
#   Io14(pin5) – индикатор состояния Wi-Fi ( 1- включение индикатора) 
    Индикатор мигает – поиск устройства, индикатор горит – связь установлена,     индикатор не горит  - поиск устройства завершен и неудачен.
Io05(pin14) – включение «внешнего управления»  (1- включить) и индикатора «внешнего управления»
Включаем внешнее управление при отсутствии «ошибки», отсутствие работы, установленной связи, и положении ключа – «выкл». 
Включение происходит при «выключении внешнего управления» 
При потере связи – установка «ошибки» на 500мс. Приведет к автоматическому выключению устройства.
Io12(pin6) – индикатор LOCK ( 1- включение индикатора)
Включаем индикатор при «включенном внешнем управлении» и значении поворота ключа «выкл» , выключаем после поворота ключа на «вкл». 
#   Io13(pin7) - установка «ошибки» (1- установка ошибки)
    При включенном «внешнем управлении» и повернутом ключе ( 0 – положение вкл) 
    Просто выключить нельзя. Надо установить «ошибку» и при выставленной ошибке снять «внешнее управление» и снять ошибку.
*/


#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>

#define WIFI_SEARCHING_TIME 60000
#define WIFI_BLINK_TIME 500
#define ERROR_TIME 500
#define WORKING_RATE 100

//INPUTS

const byte analog_state = 0; // Analog input for checking working status 
const byte working_state = 2;
const byte error_state = 4; // Input for error checking: 0 - error
const byte key_state = 16; // Input for key state checking: 0 - on, 1 - off 

//OUTPUTS

const byte wifi_ind = 14; // Output wifi status led: blink - in search, even - connected
const byte dist_manage = 5; // Output distant mangement led and status: 1 - on, 0 - off
const byte lock_ind = 12; 
const byte error_set = 13;

//WIFI settings

const char *ssid = "MART-250"; // wifi name
const char *pass = "12345678"; // wifi password


ESP8266WebServer HTTP(80); // run server

char web[] = R"=====(
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <title>WiFi Button</title>
    <link rel="stylesheet" href="css/style.css">
    <style>
        body{
        font-family: sans-serif;
        padding: 0;
        margin: 0;
        box-sizing: border-box;
    }
    .main{
        display: flex;
    }
    h1{
        display: inline-block;
        font-size: 80px;
        text-align: center;
    }
    .menu{
        height: 200px;
        width: 200px;
        position: absolute;
        left: 0;
        background: #ccc;
    }
    .logo{
        display: flex;
        justify-content: center;
        color: #cdcdcd;
        border-bottom: solid #cdcdcd 10px;
    }

    .box{
        width: 100vw;
        height: calc(100vh - 210px);
        display: flex;
        flex-direction: column;
        align-items: center;
        overflow: hidden;
    }
    .monkey{
        width: 100vw;
        height: calc(50vh - 210px);
        background: url("img/monkey.png") no-repeat center;
        display: flex;
        position: relative;
        justify-content: end;
    }
    .error{
        position: absolute;
        top: 20px;
        left: 20px;
        color: #be3636;
        font-size: 60px;
        border: solid 5px #be3636;
        border-radius: 20px;
        padding: 20px;
        display: none;
    }
    .voltage_bar{
        width: 100px;
        height: 100%;
        background-color: #cdcdcd;
        display: flex;
        flex-direction: column;
        justify-content: end;
    }
    .voltage{
        width: 50px;
        /*height: 70%;*/
        background-color: #572c2c;
        margin: 0 auto;
        transition: all 1s;
    }
    .timer_box{
        width: 100vw;
        height: 200px;
        display: flex;
        border-bottom: #cdcdcd solid 10px;
    }
    .chosen_timer{
        width:50%;
        height: 100%;
    }
    .time_left{
        width:50%;
        height: 100%;
    }
    .info_text{
        font-size: 80px;
        display: flex;
        align-items: center;
        justify-content: center
    }
    .state{
        height: 200px;
        width: 100vw;
        border-top: #cdcdcd solid 10px;
        border-bottom: #cdcdcd solid 10px;
    }
    .button_wrapper{
        height: calc(50vh - 310px);
        width: 100vw;
        display: flex;
        justify-content: center;
        align-items: center;
    }
    .button{
        height: 200px;
        width: 200px;
    }
    .button_off{
        background: #757575;
        border-radius: 50%;
    }
    .button_on{
        background: #572c2c;
        border-radius: 50%;
    }
    /* menu */

    .menu_box{
        width: 0vw;
        overflow: hidden;
    }
    .timer_list{
        list-style: none;
        width: 100%;
        height: calc(100% - 260px);
    }
    .no_timer_wrapper{
        display: flex;
        align-items: center;
        color: #cdcdcd;
    }
    #choose0{
        margin-right: 36px;
    }
    .set_box{
        width: 100%;
        display: flex;
        justify-content: space-between;
        align-items: center;
    }
    .item{
        height: calc((100vh - 410px)/10);
    }
    .name{
        width: 30%;
        font-size: 60px;
    }
    .timer_min{
        width: 22%;
        font-size: 60px;
    }
    .timer_sec{
        width: 22%;
        font-size: 60px;
        margin-right: 40px;
    }
    input[type=radio] {
        display: block;
        -ms-transform: scale(4);
        -webkit-transform: scale(4);
        transform: scale(4);
    }

    </style>
</head>
<body>
    <div class="logo">
            <div class="menu"></div>
            <h1>МАРТЫШКА</h1>
    </div>
    <div class="main">
        <div class="menu_box info_text">
            <ul class="timer_list">
                <li class="item">
                    <div class="set_box">
                        <div class="no_timer_wrapper">
                            <input type="radio" class="not_chosen" id="choose0" name="choose" checked>
                            <label for="choose0">- без таймера</label>
                        </div>
                    </div>
                </li>
                <li class="item">
                    <div class="set_box">
                        <input type="radio" class="chosen" id="choose1" name="choose" >
                        <label for="choose1"> </label>
                        <input type="text" class="name" id="name1" placeholder="имя">
                        <label for="name1">-</label>
                        <input type="number" class="timer_min" id="min1" min="0" max="3" step="1" placeholder="минут">
                        <label for="sec1">:</label>
                        <input type="number" class="timer_sec" id="sec1" min="0" max="59" step="1" placeholder="секунд">
                    </div>
                </li>
                <li class="item">
                    <div class="set_box">
                        <input type="radio" class="chosen" id="choose2" name="choose" >
                        <label for="choose2"> </label>
                        <input type="text" class="name" id="name2" placeholder="имя">
                        <label for="name3">-</label>
                        <input type="number" class="timer_min" id="min2" min="0" max="3" step="1" placeholder="минут">
                        <label for="sec2">:</label>
                        <input type="number" class="timer_sec" id="sec2" min="0" max="59" step="1" placeholder="секунд">
                    </div>
                </li>
                <li class="item">
                    <div class="set_box">
                        <input type="radio" class="chosen" id="choose3" name="choose" >
                        <label for="choose3"> </label>
                        <input type="text" class="name" id="name3" placeholder="имя">
                        <label for="name3">-</label>
                        <input type="number" class="timer_min" id="min3" min="0" max="3" step="1" placeholder="минут">
                        <label for="sec2">:</label>
                        <input type="number" class="timer_sec" id="sec3" min="0" max="59" step="1" placeholder="секунд">
                    </div>
                </li>
            </ul>
        </div>
        <div class="box">
            <div class="timer_box">
                <div class="chosen_timer info_text">нет таймера</div>
                <div class="time_left info_text">--:--</div>
            </div>
            <div class="monkey">
                <div class="error">
                    ОШИБКА!
                </div>
                <div class="voltage_bar">
                    <div class="voltage"></div>
                </div>
            </div>
            <div class="state info_text">СТАТУС: <span id="state">ПОДКЛЮЧИТЕ</span></div>
            <div class="button_wrapper">
                <div class="button button_off">
                </div>
            </div>
        </div>
    </div>
    <script>
        let btn = document.querySelector('.button');
        let isOn = 0;

        function turn_off(){
            let request = new XMLHttpRequest();
            request.open('GET', 'http://192.168.4.1/off', false);
            request.onload = () => {
                if(request.readyState === 4 && request.status === 200) {
                    let response = request.responseText;
                    return Number.parseInt(response);
                }
                else return -1;
            };
            request.send();
        }

        function manage_on(){
            let request = new XMLHttpRequest();
            request.open('GET', 'http://192.168.4.1/manage-on', false);
            request.onload = () => {
                if(request.readyState === 4 && request.status === 200) {
                    let response = request.responseText;
                    return Number.parseInt(response);
                }
                else return -1;
            };
            request.send();
        }

        function manage_off(){
            let request = new XMLHttpRequest();
            request.open('GET', 'http://192.168.4.1/manage-off?length:5000', false);
            request.onload = () => {
                if(request.readyState === 4 && request.status === 200){
                    let response = request.responseText;
                    return Number.parseInt(response);
                }
                else return -1;
            };
            request.send();
        }
        const timerCount = 3;
        //Current data
        const notFound = 'ПОДКЛЮЧИТЕ';
        const notReady = 'ПОДОЖДИТЕ';
        const ready = 'ГОТОВО';
        const status = ['ПОДКЛЮЧИТЕ', 'ВОЗЬМИТЕ УПРАВЛЕНИЕ', 'ПОВЕРНИТЕ КЛЮЧ', 'ВКЛЮЧИТЕ', 'РАБОТАЕТ'];
        let staticData = {
            current: {
                timerActive: 0,
                time: 0,
                name: null,
                state: 0,
                button: 0,
                licenseShown: 0
            },
            timers: [{time:0, name:null},{time:0, name:null},{time:0, name:null}]
        };
        let moduleStatus = {};
        if(localStorage.getItem('staticData') != null)
            console.log('not null')
        else
            localStorage.setItem('staticData',  JSON.stringify(staticData));

        //Status checking
        function getStatus(){

            const faceState = document.querySelector('#state');
            const voltage = document.querySelector('.voltage');
            const error = document.querySelector('.error');
            //timer loop setting up
            setInterval(()=> {
                faceState.innerText = status[staticData.current.state];
                faceState.style.color = '#6F7E6E';
                faceState.style.color = '#AF9B59';
                faceState.style.color = '#DD8C70';
                // Try to send a request
                let err = 1;
                try {
                    let request = new XMLHttpRequest();
                    request.open('GET', 'http://192.168.4.1/state', true);
                    request.onload = () => {
                        if (request.readyState === 4 && request.status === 200) {
                            err = 0;
                            let response = JSON.parse(request.responseText);
                            console.log(response);
                            moduleStatus = response;
                            staticData.current.state = +response.stage + 1;

                            voltage.style.height = (moduleStatus.voltage / 1500 * 100) + "%";
                            if(+moduleStatus.voltage < 500) voltage.style.background ='#AF9B59';
                            else if(+moduleStatus.voltage < 900) voltage.style.background = '#6F7E6E';
                            else voltage.style.background = '#DD8C70';

                            if(+moduleStatus.on_error) error.style.display = "block";
                            else error.style.display = "none"
                        }
                    }
                    request.send();
                    // if something went wrong during sending
                } finally {
                    if (err) staticData.current.state = 0;
                }
            }, 1000);

        }

        //Menu open and close
        function toggleMenu(){
            const menu_box = document.querySelector('.menu_box');
            const box = document.querySelector('.box');
            if(!staticData.current.button) {
                if (box.style.width === "100vw") {
                    box.style.width = "0vw";
                    menu_box.style.width = "100vw";
                } else {
                    menu_box.style.width = "0vw";
                    box.style.width = "100vw";
                }
            }
        }

        const menu = document.querySelector(".menu");
        menu.addEventListener('click', toggleMenu);


        //Page onload
        window.onload = ()=>{
            const tempStatic = JSON.parse(localStorage.getItem('staticData'));
            for(let i = 0; i < timerCount; i++){
                const name = document.querySelector(`#name${i+1}`);
                const minInput = document.querySelector(`#min${i+1}`);
                const secInput = document.querySelector(`#sec${i+1}`);

                name.value = tempStatic.timers[i].name;
                minInput.value = Math.floor(parseInt(tempStatic.timers[i].time) / 60);
                secInput.value = parseInt(tempStatic.timers[i].time) % 60;
            }
            getStatus();
        }


        //Choosing timer

        //Choosing timer Callback
        function chooseTimer(num){
            return () => {
                staticData.current.timerActive = 1;
                const name = document.querySelector(`#name${num}`).value;
                let min = document.querySelector(`#min${num}`).value;
                let sec = document.querySelector(`#sec${num}`).value;
                if(sec.length === 1)
                    sec = '0' + sec;
                if(min.length === 1)
                    min = '0' + min;
                //Changing the timer name
                const faceName = document.querySelector('.chosen_timer');
                faceName.innerText = name;
                //Changing the time
                const faceTime = document.querySelector('.time_left');
                faceTime.innerText = min + ':' + sec;
                staticData.current.name = name;
                staticData.current.time = parseInt(min) * 60 + parseInt(sec);
                if (isNaN(staticData.current.time))
                    staticData.current.time = 0;
            }
        }
        //Correcting data
        function checkTimeData(num){
            return () => {
                const name = document.querySelector(`#name${num}`).value;
                const minInput = document.querySelector(`#min${num}`);
                const secInput = document.querySelector(`#sec${num}`);
                if(parseInt(minInput.value) > 3)
                    minInput.value = 3;
                if(parseInt(secInput.value) > 59)
                    secInput.value = 59;
                //Local Storage
                let tempStatic = JSON.parse(localStorage.getItem('staticData'));
                tempStatic.timers[num-1].time = parseInt(minInput.value)*60 + parseInt(secInput.value);
                tempStatic.timers[num-1].name = name;
                localStorage.setItem('staticData', JSON.stringify(tempStatic));
            };
        }
        //Setting event listeners
        for(let i = 1; i < timerCount+1; i++){
            document.querySelector(`#choose${i}`).addEventListener('click', chooseTimer(i));
            document.querySelector(`#min${i}`).addEventListener('blur', checkTimeData(i));
            document.querySelector(`#sec${i}`).addEventListener('blur', checkTimeData(i));
        }
        //No timer
        document.querySelector('#choose0').addEventListener('click', ()=>{
            const faceName = document.querySelector('.chosen_timer');
            faceName.innerText = 'нет таймера';
            const faceTime = document.querySelector('.time_left');
            faceTime.innerText = '--:--';
            staticData.current.timerActive = 0;
        });

        //Timer Function
        function setTimer(){
            let len = staticData.current.time;
            const faceTime = document.querySelector('.time_left');
            console.log('\n\n\n' + staticData.current.button + '\n\n\n');
            if(staticData.current.button === 0) {
                if (len && /*staticData.current.state && */staticData.current.timerActive) {
                    manage_off();
                    staticData.current.button = 1;
                    button.classList.toggle('button_on');
                    let interval = setInterval(function timer() {
                        len--;
                        let min = Math.floor(len / 60).toString();
                        if (min.length === 1)
                            min = '0' + min;
                        let sec = (len % 60).toString();
                        if (sec.length === 1)
                            sec = '0' + sec;
                        faceTime.innerText = min + ':' + sec;
                        if ((len < 1)||(staticData.current.button === 0)) {
                            clearInterval(interval);
                            min = Math.floor(staticData.current.time / 60).toString();
                            sec = (staticData.current.time % 60).toString();
                            if (sec.length === 1)
                                sec = '0' + sec;
                            if (min.length === 1)
                                min = '0' + min;
                            faceTime.innerText = min + ':' + sec;
                            staticData.current.button = 0;
                            button.classList.toggle('button_on');
                            turn_off();
                        }
                    }, 1000);
                }
                else if(!staticData.current.timerActive){
                    manage_off();
                    staticData.current.button = 1;
                    button.classList.toggle('button_on');
                }
            }
            else{
                staticData.current.button = 0;
                if(!staticData.current.timerActive)
                    turn_off();
                button.classList.toggle('button_on');
            }
        }

        function manageButton(){
            if(+staticData.current.state > 1){
                setTimer();
            }
            else{
                manage_on();
            }
        }
        const button = document.querySelector('.button');
        button.addEventListener('click', manageButton);


    </script>

</body>
</html>
)=====";

//variables

unsigned long timer = 0;
bool connected = 0;
bool distant = 0;
short decsec = 0;

class State
{
  private:
  
  unsigned long start_time = millis();
  unsigned long timer = millis();
  unsigned long error_timer = 0;
  unsigned long blink_timer = 0;
  unsigned long working_timer = 0;

  
  void searchWIFI(){
    if((timer - start_time < WIFI_SEARCHING_TIME) && !WiFi.softAPgetStationNum() && !this->is_connected){
      if((millis() - start_time) % 1000 < 500){
        digitalWrite(wifi_ind, 1);
        //Serial.println("led on");
      }
      else{
        digitalWrite(wifi_ind, 0);
        //Serial.println("led off");
      }
    }
    //succesfuly connected
    else if(WiFi.softAPgetStationNum() && !this->is_connected){
      digitalWrite(wifi_ind, 1);
      //Serial.println("led on");
      this->is_connected = 1;
    }
    //lost connection
    else if(!WiFi.softAPgetStationNum() && this->is_connected){
      if(!on_error){
        digitalWrite(error_set, 1);
        this->on_manage = 0;
        digitalWrite(dist_manage, 0);
        //Serial.println("error set");
        this->error_timer = timer;
      }
      if(timer - error_timer > ERROR_TIME){
        digitalWrite(error_set, 0);
        //Serial.println("error removed");
        this->is_connected = 0;
        start_time = timer;
        error_timer = 0;
      }
    }
    //not found
    else if((timer - start_time > WIFI_SEARCHING_TIME) && !WiFi.softAPgetStationNum()){
      digitalWrite(wifi_ind, 0);
      //Serial.println("led off");
    }
  }

  void checkLOCK(){
    if(!this->key_on && this->on_manage && !this->on_lock){
      this->on_lock = 1;
      digitalWrite(lock_ind, 1);
      this->stage = 1;
    }
    else if(this->key_on && this->on_manage && this->on_lock){
      this->on_lock = 0;
      digitalWrite(lock_ind, 0);
      this->stage = 2;
      Serial.print(this->stage);
      Serial.println(" - stage");
    }  
  }

  void turn_off_seed(){
    if(this->needs_stop && !error_timer){
      if(this->is_working)
        this->stage = 0;
      digitalWrite(error_set, 1);
      error_timer = timer;
    }
    else if(timer - error_timer > 500){
      this->needs_stop = 0;
      digitalWrite(error_set, 0);
      error_timer = 0;
    }
  }
  
  public:

  unsigned long working_time_limit = 0;
  float voltage;
  bool is_working = 0;
  bool is_connected = 0;
  bool on_lock = 0;
  bool on_error = 0;
  bool on_manage = 0;
  bool key_on = 0;
  bool needs_stop = 0;
  short stage = 0;
  
  void check(){
    if(millis() - timer > WORKING_RATE){
      timer = millis();
      
      this->voltage = analogRead(analog_state);
      this->is_working = !digitalRead(working_state);
      this->on_error = !digitalRead(error_state);
      this->key_on = !digitalRead(key_state);
      
      searchWIFI();
      checkLOCK();
      turn_off_seed();
    }
  }
  
  String manage_turn_on(){
    String success = "0";
    if(!this->on_error && !this->key_on && !this->is_working && this->is_connected){
      on_manage = 1;
      digitalWrite(dist_manage, 1);
      success = "1";
    }
    return success;
  }

  String manage_turn_off(){
    if(!this->on_lock)
      this->stage = 3;
    else
      this->stage = 0;
    on_manage = 0;
    digitalWrite(dist_manage, 0);
    return("<h1>Manage is turned off</h1>");
  }
};

State state;

void setup() {
  pinMode(analog_state, INPUT);
  pinMode(working_state, INPUT);
  pinMode(error_state, INPUT);
  pinMode(key_state, INPUT);  
  pinMode(wifi_ind, OUTPUT);
  pinMode(dist_manage, OUTPUT);
  pinMode(lock_ind, OUTPUT);
  pinMode(error_set, OUTPUT);
  Serial.begin(9600); // Инициализируем вывод данных на серийный порт со скоростью 9600 бод
  WiFi.softAP(ssid, pass, 1, 0, 1); // Создаём точку доступа
  HTTP.begin(); // Инициализируем Web-сервер

  HTTP.on("/", [](){
    
    HTTP.send(200, "text/html", web);
  });
  
  HTTP.on("/manage-on", [](){
    HTTP.enableCORS(true);
    HTTP.send(200, "text/plain", state.manage_turn_on());
  });
  
  HTTP.on("/manage-off", [](){
    HTTP.enableCORS(true);
    //state.working_time_limit = HTTP.arg("length").toInt();    //  http://192.168.4.1/manage-off?length=5000
    HTTP.send(200, "text/plain", state.manage_turn_off());
  });

  HTTP.on("/off", [](){
    HTTP.enableCORS(true);HTTP.enableCORS(true);
    state.needs_stop = 1;
    HTTP.send(200, "text/plain", "stop");
  });

  HTTP.on("/state", [](){
    String stateJson = "{\"stage\":\"" + String(state.stage) + "\",\"voltage\":\"" + String(state.voltage) + "\",\"is_working\":\"" + String(state.is_working) + "\",\"on_lock\":\"" + String(state.on_lock) + "\",\"on_error\":\"" + String(state.on_error) + "\",\"on_manage\":\"" + String(state.on_manage)+"\"}";
    HTTP.enableCORS(true);
    HTTP.send(200, "application/json", stateJson);
  });
  
}

void loop() 
{  
  HTTP.handleClient(); // HTTP handler

  state.check();
  
}
