#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>

#define WIFI_SEARCHING_TIME 60000
#define CONNECTION_LOSS_AFTER 10000
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

const char *ssid = "MART-250-001"; // wifi name
const char *pass = "12345678"; // wifi password


ESP8266WebServer HTTP(80); // run server

char web[] = R"=====(<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <title>MART</title>
    <style>body{
    font-family: sans-serif;
    padding: 0;
    margin: 0;
    box-sizing: border-box;
}
.cover{
    position: absolute;
    height: 100%;
    width: 100%;
    z-index: 100;
    display: flex;
    justify-content: center;
    align-items: center;
    animation: all 1000;
}
.working-cover{
    background: #99e29ed7;
    visibility: hidden;
}
.not_found-cover{
    background: #e9d2d2d7;
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
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: space-evenly;
}
.menu > div{
    height: 20px;
    width: 150px;
    background: #ffffff;
    transition: all 0.1s;
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
    justify-content: flex-end;
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
    justify-content: flex-end;
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
    width:60%;
    height: 100%;
}
.time_left{
    width:50%;
    height: 100%;
}
.info_text{
    font-size: 70px;
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
.off_button_text{
    line-height: 150px;
    text-align: center;
    font-size: 35px;
    font-weight: bold;
    color: #ffffff;
}
.button_text{
    line-height: 200px;
    text-align: center;
    font-size: 45px;
    font-weight: bold;
    color: #ffffff;
}
.button{
    height: 200px;
    width: 200px;
    background: #757575;
    border: solid 5px #cdcdcd;
    border-radius: 50%;
}
.button_turn_off{
    height: 150px;
    width: 150px;
    position: absolute;
    bottom: calc(25vh - 245px);
    left: 20vw;
    background: #572c2c;
    border: solid 5px #572c2c;
    border-radius: 50%;
}
.button_inactive{
    background: none!important;
    pointer-events: none;
    cursor: not-allowed;
}
.button_off{

}
.button_on{
    /*background: #572c2c;
    border-radius: 50%;*/
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
    min-height: 1400px;
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
    width: 58%;
    font-size: 60px;
    
}
.timer_min{
    width: 8%;
    font-size: 60px;
}
.timer_sec{
    width: 8%;
    font-size: 60px;
    margin-right: 40px;
}
input[type=radio] {
    display: block;
    -ms-transform: scale(4);
    -webkit-transform: scale(4);
    transform: scale(4);
}</style></head>
<body>
    <div class="cover not_found-cover">
        <h1>
            НЕТ ПОДКЛЮЧЕНИЯ
        </h1>
    </div>
    <div class="cover working-cover">
        <h1>
            ПОДОЖДИТЕ, РАБОТАЕТ
        </h1>
    </div>
    <div class="logo">
            <div class="menu">
                <div class="menu-line"></div>
                <div class="menu-line"></div>
                <div class="menu-line"></div>
            </div>
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
                        <label for="sec3">:</label>
                        <input type="number" class="timer_sec" id="sec3" min="0" max="59" step="1" placeholder="секунд">
                    </div>
                </li>
                <li class="item">
                    <div class="set_box">
                        <input type="radio" class="chosen" id="choose4" name="choose" >
                        <label for="choose4"> </label>
                        <input type="text" class="name" id="name4" placeholder="имя">
                        <label for="name4">-</label>
                        <input type="number" class="timer_min" id="min4" min="0" max="3" step="1" placeholder="минут">
                        <label for="sec4">:</label>
                        <input type="number" class="timer_sec" id="sec4" min="0" max="59" step="1" placeholder="секунд">
                    </div>
                </li>
                <li class="item">
                    <div class="set_box">
                        <input type="radio" class="chosen" id="choose5" name="choose" >
                        <label for="choose5"> </label>
                        <input type="text" class="name" id="name5" placeholder="имя">
                        <label for="name5">-</label>
                        <input type="number" class="timer_min" id="min5" min="0" max="3" step="1" placeholder="минут">
                        <label for="sec5">:</label>
                        <input type="number" class="timer_sec" id="sec5" min="0" max="59" step="1" placeholder="секунд">
                    </div>
                </li>
                <li class="item">
                    <div class="set_box">
                        <input type="radio" class="chosen" id="choose6" name="choose" >
                        <label for="choose6"> </label>
                        <input type="text" class="name" id="name6" placeholder="имя">
                        <label for="name6">-</label>
                        <input type="number" class="timer_min" id="min6" min="0" max="3" step="1" placeholder="минут">
                        <label for="sec6">:</label>
                        <input type="number" class="timer_sec" id="sec6" min="0" max="59" step="1" placeholder="секунд">
                    </div>
                </li>
                <li class="item">
                    <div class="set_box">
                        <input type="radio" class="chosen" id="choose7" name="choose" >
                        <label for="choose7"> </label>
                        <input type="text" class="name" id="name7" placeholder="имя">
                        <label for="name7">-</label>
                        <input type="number" class="timer_min" id="min7" min="0" max="3" step="1" placeholder="минут">
                        <label for="sec7">:</label>
                        <input type="number" class="timer_sec" id="sec7" min="0" max="59" step="1" placeholder="секунд">
                    </div>
                </li>
                <li class="item">
                    <div class="set_box">
                        <input type="radio" class="chosen" id="choose8" name="choose" >
                        <label for="choose8"> </label>
                        <input type="text" class="name" id="name8" placeholder="имя">
                        <label for="name8">-</label>
                        <input type="number" class="timer_min" id="min8" min="0" max="3" step="1" placeholder="минут">
                        <label for="sec8">:</label>
                        <input type="number" class="timer_sec" id="sec8" min="0" max="59" step="1" placeholder="секунд">
                    </div>
                </li>
                <li class="item">
                    <div class="set_box">
                        <input type="radio" class="chosen" id="choose9" name="choose" >
                        <label for="choose9"> </label>
                        <input type="text" class="name" id="name9" placeholder="имя">
                        <label for="name9">-</label>
                        <input type="number" class="timer_min" id="min9" min="0" max="3" step="1" placeholder="минут">
                        <label for="sec9">:</label>
                        <input type="number" class="timer_sec" id="sec9" min="0" max="59" step="1" placeholder="секунд">
                    </div>
                </li>
                <li class="item">
                    <div class="set_box">
                        <input type="radio" class="chosen" id="choose10" name="choose" >
                        <label for="choose10"> </label>
                        <input type="text" class="name" id="name10" placeholder="имя">
                        <label for="name10">-</label>
                        <input type="number" class="timer_min" id="min10" min="0" max="3" step="1" placeholder="минут">
                        <label for="sec10">:</label>
                        <input type="number" class="timer_sec" id="sec10" min="0" max="59" step="1" placeholder="секунд">
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
                <button class="button button_off">
                    <div class="button_text"></div>
                </button>
                <button class="button_turn_off button_inactive">
                    <div class="off_button_text">СБРОС</div>
                </button>
            </div>
        </div>
    </div>

    <script> let btn = document.querySelector('.button');
let isOn = 0;

const licenceText = `Соглашение по использованию
Приложение предназначено исключительно для совместного использования с оборудованием серии МАРТ, которое имеет такую возможность. 
Совместное использование не изменяет основных характеристик оборудования и не предоставляет материальной выгоды.
Никакие действия или бездействия приложения не могут быть истолкованы как основания по причинению, какого либо, вреда.
Приложение предоставляется по принципу «как есть».  Для ознакомления возможностей приложения отводится десять часов совместного использования с оборудованием или месяц после установки.
Дальнейшее использование приложения подтверждает, что оно полностью отвечает вашим ожиданиям и удовлетворяет вашим потребностям.
Не допускается вносить любые изменения в приложение, разбирать его код. Нарушение этого пункта приведет к немедленному расторжению соглашения.`

function turn_off(){
    let request = new XMLHttpRequest();
    request.open('GET', 'http://192.168.4.1/off', false);
    request.onload = () => {
        if(request.readyState === 4 && request.status === 200) {
            let response = request.responseText;
            setSilentTimer(parseInt(response));
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

function getReady(){
    let request = new XMLHttpRequest();
    request.open('GET', 'http://192.168.4.1/ready', false);
    request.onload = () => {
        if(request.readyState === 4 && request.status === 200){
            let response = request.responseText;
            return response;
        }
        else return -1;
    };
    request.send();
}

const timerCount = 10;
//Current data
const notFound = 'ПОДКЛЮЧИТЕ';
const notReady = 'ПОДОЖДИТЕ';
const ready = 'ГОТОВО';
const status = ['ПОДКЛЮЧИТЕ', 'ВОЗЬМИТЕ УПРАВЛЕНИЕ', 'ПОВЕРНИТЕ КЛЮЧ', 'ВКЛЮЧИТЕ', 'РАБОТАЕТ', 'ПАУЗА'];
let staticData = {
    current: {
        timerActive: 0,
        time: 0,
        name: null,
        state: 0,
        button: 0
    },
    timers: [{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null}]
};

let moduleStatus = {};

//Status checking
function getStatus(){

    const faceState = document.querySelector('#state');
    const voltage = document.querySelector('.voltage');
    const error = document.querySelector('.error');
    const button = document.querySelector('.button');
    const offButton = document.querySelector('.button_turn_off');
    const buttonText = document.querySelector('.button_text');
    const notFoundCover = document.querySelector('.not_found-cover');
    const workingCover = document.querySelector('.working-cover');
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

                    notFoundCover.style.visibility = 'hidden';

                    if((staticData.current.state === 1)&&(moduleStatus.is_working === '1'))
                        workingCover.style.visibility = 'visible';
                    else{
                        workingCover.style.visibility = 'hidden';
                    }
                        

                    voltage.style.height = (moduleStatus.voltage / 1500 * 100) + "%";
                    if(+moduleStatus.voltage < 500) voltage.style.background ='#AF9B59';
                    else if(+moduleStatus.voltage < 900) voltage.style.background = '#6F7E6E';
                    else voltage.style.background = '#DD8C70';

                    if(+moduleStatus.on_error) error.style.display = "block";
                    else error.style.display = "none"

                    if((staticData.current.state === 2)||(staticData.current.state === 5)){
                        button.classList.add('button_inactive');
                    }
                    else
                        button.classList.remove('button_inactive');
                    if(staticData.current.state === 4) button.style.background = "#572c2c";
                    else button.style.background = "#757575";

                    if((staticData.current.state > 1)&&(staticData.current.state !== 4)){
                        offButton.classList.remove('button_inactive');
                    }      
                    else 
                        offButton.classList.add('button_inactive');

                    if(staticData.current.state === 1) buttonText.innerText = "ВЗЯТЬ";
                    else if(staticData.current.state === 3) buttonText.innerText = "ВКЛ";
                    else if(staticData.current.state === 4) buttonText.innerText = "ВЫКЛ";
                    else buttonText.innerText = "";
                }
                
            }
            request.send();
            // if something went wrong during sending
        } finally {
            if (err) {
                //condition to except blinking
                if(staticData.current.state === 0) {
                    notFoundCover.style.visibility = 'visible';
                    workingCover.style.visibility = 'hidden';
                }
                staticData.current.state = 0;
            }
        }
    }, 1000);

}

//Menu open and close
function toggleMenu(){
    const menu_box = document.querySelector('.menu_box');
    const box = document.querySelector('.box');
    const reset = document.querySelector('.button_turn_off');
    const lines = document.querySelectorAll('.menu-line');
    const button = document.querySelector('.menu');
    console.log(button.style);

    if(!staticData.current.button) {
        if (box.style.width === "100vw") {
            box.style.width = "0vw";
            menu_box.style.width = "100vw";
            reset.style.visibility = "hidden";
            lines[0].style.transform = "rotate(45deg)";
            lines[1].style.display = "none";
            lines[2].style.transform = "rotate(-45deg)";
            button.style.justifyContent = "center";
        } else {
            menu_box.style.width = "0vw";
            box.style.width = "100vw";
            reset.style.visibility = "visible";
            lines[0].style.transform = "rotate(0deg)";
            lines[1].style.width = "150px";
            lines[1].style.display = "block";
            lines[2].style.transform = "rotate(0deg)";
            button.style.justifyContent = "space-evenly";
        }
    }

}

const menu = document.querySelector(".menu");
menu.addEventListener('click', toggleMenu);


//Page onload
window.onload = ()=>{
    if(localStorage.getItem('licenceShown') === null)
        localStorage.setItem('licenceShown', '0');
    if(localStorage.getItem('staticData') === null){
        localStorage.setItem('staticData',  JSON.stringify(staticData));
    }
    
    const tempStatic = JSON.parse(localStorage.getItem('staticData'));
    staticData = JSON.parse(JSON.stringify(tempStatic));

    if(localStorage.getItem('licenceShown') === '0'){
        if(confirm(licenceText))
            localStorage.setItem('licenceShown', '1');
        else
            localStorage.setItem('licenceShown', '0');
    }
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
        if(parseInt(minInput.value) > 10)
            minInput.value = 10;
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

function setSilentTimer(len){ 
    const faceTime = document.querySelector('.time_left');
    len = Math.floor(len / 1000);
    let interval = setInterval(()=>{
        len -= 1;
        let min = Math.floor(len / 60).toString();
        if (min.length === 1)
            min = '0' + min;
        let sec = (len % 60).toString();
        if (sec.length === 1)
            sec = '0' + sec;
        faceTime.innerText = min + ':' + sec;
        //console.log("stage - " + staticData.current.stage);
        if(((len <= 0)&&(staticData.current.state === 5)) || staticData.current.state === 0){
            clearInterval(interval);
            getReady();
            if(staticData.current.time){
                min = Math.floor(staticData.current.time / 60).toString();
                sec = (staticData.current.time % 60).toString();
                if (sec.length === 1)
                    sec = '0' + sec;
                if (min.length === 1)
                    min = '0' + min;
                faceTime.innerText = min + ':' + sec;
            }
            else{
                sec = '--';
                min = '--';
                faceTime.innerText = min + ':' + sec;
            }
        }
    }, 1000);
}

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
                console.log(staticData.current.state);  
                if ((len < 1)||(staticData.current.button === 0)||(staticData.current.state === 0)) {
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
    if(staticData.current.state > 1){
        setTimer();
    }
    else{
        manage_on();
    }
}

function manageOffButton(){
    if(staticData.current.state === 2) manage_off();
    else turn_off();
}
const button = document.querySelector('.button');
const offButton = document.querySelector('.button_turn_off');
button.addEventListener('click', manageButton);
offButton.addEventListener('click', manageOffButton);</script>

</body>
</html>)=====";

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
  bool reboot = 0;
  
  void searchWIFI(){
    if((timer - start_time < WIFI_SEARCHING_TIME) && !WiFi.softAPgetStationNum() && !this->is_connected){
      if((millis() - start_time) % 1000 < 500){
        digitalWrite(wifi_ind, 1);
      }
      else{
        digitalWrite(wifi_ind, 0);
      }
    }
    //succesfuly connected
    else if(WiFi.softAPgetStationNum() && !this->is_connected){
      digitalWrite(wifi_ind, 1);
      this->is_connected = 1;
      timer += WIFI_SEARCHING_TIME;
    }
    //lost connection
    else if(!WiFi.softAPgetStationNum() && this->is_connected){
      if(!((this->stage == 0) && this->is_working)){
        this->needs_stop = 1;
        this->reboot = 1;
      }
      digitalWrite(wifi_ind, 0);
      this->is_connected = 0;
    }
    //not found
    else if((timer - start_time > WIFI_SEARCHING_TIME) && !WiFi.softAPgetStationNum()){
      digitalWrite(wifi_ind, 0);
      //Serial.println("led off");
    }
  }

  void checkLOCK(){
    //enter LOCK
    if(!this->key_on && this->on_manage && !this->on_lock){
      this->on_lock = 1;
      digitalWrite(lock_ind, 1);
      this->stage = 1;
    }
    //exit LOCK
    else if((this->key_on && this->on_manage && this->on_lock)||(!this->key_on && !this->on_manage && this->on_lock)){
      this->on_lock = 0;
      digitalWrite(lock_ind, 0);
      this->stage = 2;
      Serial.print(this->stage);
      Serial.println(" - stage");
    }
  }

  void turn_off_seed(){
    if(this->needs_stop && !error_timer){
      if(reboot) this->stage = 0;
      else this->stage = 4;
      
      digitalWrite(error_set, 1);
      digitalWrite(dist_manage, 0);
      
      if(millis() - this->working_timer > 60000)
        this->working_timer = millis() - this->working_timer;
      else
        this->working_timer = 60000;
        
      error_timer = timer;
    }
    else if(timer - error_timer > ERROR_TIME){
      this->needs_stop = 0;
      reboot = 0;
      digitalWrite(error_set, 0);
      error_timer = 0;
    }
  }

  void connection_loss_seed(){
    if((this->state_timer > 0) && (timer - this->state_timer > CONNECTION_LOSS_AFTER)){
      if(!((this->stage == 0) && this->is_working)){
        reboot = 1;
        this->needs_stop = 1;
      }
      state_timer = 0;
      
    }
  }
  
  public:

  unsigned long working_time_limit = 0;
  unsigned long state_timer = 0;
  unsigned long working_timer = 0;
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
      this->on_manage = digitalRead(dist_manage);
      
      searchWIFI();
      checkLOCK();
      turn_off_seed();
      connection_loss_seed();
    }
  }
  
  String manage_turn_on(){
    String success = "0";
    if(!this->on_error && !this->key_on && !this->is_working && this->is_connected){
      on_manage = 1;
      digitalWrite(dist_manage, 1);
      success = "1";
      this->stage = 1;
    }
    return success;
  }

  String manage_turn_off(){
    if(!this->on_lock){
      this->stage = 3;
      this->working_timer = millis();
    }
    else{
      this->stage = 0;
      this->on_lock = 0;
      digitalWrite(lock_ind, 0);  
    }
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
  Serial.begin(9600);
  WiFi.softAP(ssid, pass, 1, 0, 1);
  HTTP.begin();

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
    HTTP.enableCORS(true);
    state.needs_stop = 1;
    state.stage = 4;
    if(millis() - state.working_timer > 60000)
      state.working_timer = millis() - state.working_timer;
    else
      state.working_timer = 60000;
    HTTP.send(200, "text/plain", String(state.working_timer));
  });

  HTTP.on("/ready", [](){
    HTTP.enableCORS(true);
    state.stage = 0;
    state.working_timer = 0;
    HTTP.send(200, "text/plain", "ready");
  });

  HTTP.on("/state", [](){
    String stateJson = "{\"stage\":\"" + String(state.stage) + "\",\"voltage\":\"" + String(state.voltage) + "\",\"is_working\":\"" + String(state.is_working) + "\",\"on_lock\":\"" + String(state.on_lock) + "\",\"on_error\":\"" + String(state.on_error) + "\",\"on_manage\":\"" + String(state.on_manage)+"\"}";
    HTTP.enableCORS(true);
    state.state_timer = millis();
    HTTP.send(200, "application/json", stateJson);
  });
  
}

void loop() 
{  
  HTTP.handleClient(); // HTTP handler

  state.check();
  
}
