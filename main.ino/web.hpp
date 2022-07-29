#pragma once

namespace Web
{
  char web[] = R"=====(<!doctype html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <meta name="apple-mobile-web-app-capable" content="yes" />
    <meta name="mobile-web-app-capable" content="yes" />
    <style>*, body {
    border: 0;
    padding: 0;
    margin: 0;
    box-sizing: border-box;
    font-family: sans-serif;
    font-weight: 300
}

main{
    position: relative;
    overflow: hidden;
    height: calc(100vh - 80px);
    width: 100vw;
}

header{
    width: 100%;
    height: 80px;
    background: #D9D9D9;
    border-bottom-left-radius: 10px;
    border-bottom-right-radius: 10px;
    padding-top: 13px;
    display: flex;
    justify-content: center;
}

.header-box{
    width: 364px;
    height: 54px;
    background: #FFFFFF;
    border-radius: 10px;
    text-align: center;
    font-size: 32px;
    line-height: 54px;
    color: #545353;
    overflow: scroll;
    display: inline-block;
}

.cancel-wrapper{
    width: 100%;
    height: 114px;
    position: relative;
}

.error{
    position: absolute;
    top: 34px;
    left: 28px;
    width: 180px;
    height: 47px;
    background: #EA6A6A;
    color: #FFFFFF;
    font-size: 24px;
    line-height: 47px;
    text-align: center;
    border-radius: 10px;
    overflow: hidden;
    transition: all 0.2s;
    display: none
}

.error-active{
    top: 114px;
    left: 0;
    width: 100vw;
    height: calc(100vh - 194px);
    z-index: 10;
}

.cancel{
    position: absolute;
    top: 34px;
    right: 28px;
    width: 120px;
    height: 47px;
    background: #EA6A6A;
    color: #FFFFFF;
    font-size: 24px;
    line-height: 47px;
    text-align: center;
    border-radius: 10px;
}

.panel{
    width: 100%;
    height: 324px;
    background: #F4F1F4;
    border-radius: 10px;
    display: flex;
    justify-content: space-between;
}

.state-box{
    height: 266px;
    width: 298px;
    margin-top: 30px;
    margin-left: 28px;
    margin-right: 23px
}

.time{
    width: 294px;
    height: 174px;
    background: #FFFFFF;
    border-radius: 10px;
    font-size: 96px;
    text-align: center;
    line-height: 174px;
    color: #717171;
    margin-bottom: 28px
}

.status{
    width: 294px;
    height: 63px;
    border-radius: 10px;
    font-size: 36px;
    line-height: 63px;
    text-align: center;
    background: #89B4F5;
    color: #FFFFFF
}

.voltage-wrapper{
    width: 50px;
    height: 324px;
    background: #B1B0B0;
    border-radius: 10px;
    position: relative;
    display: flex;
    align-items: flex-end;
}

.voltage{
    width: 26px;
    height: 0%;
    background: #A2E5A9;
    margin: 0 auto 12px auto;
    transition: all 1s;
}

.button{
    width: 256px;
    height: 120px;
    background: #81AFF5;
    border-radius: 20px;
    margin: 76px auto 0 auto;
    font-size: 36px;
    color: #FFFFFF;
    text-align: center;
    line-height: 120px;
}

.timer-wrapper{
    position: absolute;
    width: 1445px;
    height: 1445px;
    border-radius: 50%;
    background: #F4F1F4;
    top: calc(100vh - 170px);
    left: calc((1445px - 100vw) / -2);
    display: flex;
    flex-direction: column;
    align-items: center;
    transition: all 0.2s;
}

.timer-button-wrapper{
    width: 52px;
    height: 52px;
    margin-top: 20px;
    border-radius: 50%;
    background: #B0ABAB;
    display: flex;
    justify-content: center;
    align-items: center;
    margin-bottom: 24px;
    cursor: pointer
}

.timer{
    width: calc(100vw - 20px);
    height: 38px;
    margin: 0 auto 5px auto;
    background: #D9D9D9;
    border-radius: 10px;
    padding: 5px;
    display: flex;
    justify-content: space-between;
}

.checkbox, .timer-min, .timer-sec{
    width: 29px;
    height: 29px;
    border-radius: 5px;
    background: #F4F1F4;
    text-align: center;
    font-size: 20px;
}

.checkbox{
    cursor: pointer
}

.checkbox-active{
    background: #89B4F5;
}

.timer-name{
    width: calc(100% - 100px);
    border-radius: 5px;
    background: #F4F1F4;
    padding-left: 10px;
    font-size: 20px;
}

.timer-actions{
    margin-top: 80px;
    width: 100%;
    display: flex;
    justify-content: center;
}

.no-timer{
    width: 162px;
    height: 59px;
    background: #81AFF5;
    border-radius: 10px;
    color: #F4F1F4;
    font-size: 24px;
    line-height: 59px;
    text-align: center;
    margin-right: calc((100vw - 324px) / 3);
}

.erase{
    width: 162px;
    height: 59px;
    background: #EA6A6A;
    border-radius: 10px;
    color: #F4F1F4;
    font-size: 24px;
    line-height: 59px;
    text-align: center;
}</style></head>
<body>
    <header>
        <div class="header-box">
            Без таймера
        </div>
    </header>
    <main>
        <div class="cancel-wrapper">
            <div class="error">
                Ошибка!
                <p class="error-text"></p>
            </div>
            <div class="cancel">Отмена</div>
        </div>
        <div class="panel">
            <div class="state-box">
                <div class="time">
                    --:--
                </div>
                <div class="status">
                    Нет Связи
                </div>
            </div>
            <div class="voltage-wrapper">
                <div class="voltage"></div>
            </div>
        </div>
        <div class="button">

        </div>
        <div class="timer-wrapper">
            <div class="timer-button-wrapper">
                <svg display="inherit" class="open-timers" width="30" height="29" viewBox="0 0 30 29" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M16.4142 0.585788C15.6332 -0.19526 14.3668 -0.19526 13.5858 0.585788L0.857864 13.3137C0.0768158 14.0948 0.0768158 15.3611 0.857864 16.1421C1.63891 16.9232 2.90524 16.9232 3.68629 16.1421L15 4.82843L26.3137 16.1421C27.0948 16.9232 28.3611 16.9232 29.1421 16.1421C29.9232 15.3611 29.9232 14.0948 29.1421 13.3137L16.4142 0.585788ZM17 29L17 2H13L13 29H17Z" fill="#F4F1F4"/>
                </svg>
                <svg display="none" class="close-timers" width="30" height="29" viewBox="0 0 30 29" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M13.5858 28.4142C14.3668 29.1953 15.6332 29.1953 16.4142 28.4142L29.1421 15.6863C29.9232 14.9052 29.9232 13.6389 29.1421 12.8579C28.3611 12.0768 27.0948 12.0768 26.3137 12.8579L15 24.1716L3.68629 12.8579C2.90524 12.0768 1.63891 12.0768 0.857864 12.8579C0.0768158 13.6389 0.0768158 14.9052 0.857864 15.6863L13.5858 28.4142ZM13 0L13 27H17L17 0L13 0Z" fill="#F4F1F4"/>
                </svg>

            </div>
            <div class="timer-box">
                <form id="0" class="timer">
                    <div class="checkbox"></div>
                    <input name="name" class="timer-name"/>
                    <input name="min" type="number" min="0" max="9" class="timer-min"/>
                    <input name="sec" type="number" min="0" max="59" class="timer-sec"/>
                </form>
                <form id="1" class="timer">
                    <div class="checkbox"></div>
                    <input name="name" class="timer-name"/>
                    <input name="min" type="number" min="0" max="9" class="timer-min"/>
                    <input name="sec" type="number" min="0" max="59" class="timer-sec"/>
                </form>
                <form id="2" class="timer">
                    <div class="checkbox"></div>
                    <input name="name" class="timer-name"/>
                    <input name="min" type="number" min="0" max="9" class="timer-min"/>
                    <input name="sec" type="number" min="0" max="59" class="timer-sec"/>
                </form>
                <form id="3" class="timer">
                    <div class="checkbox"></div>
                    <input name="name" class="timer-name"/>
                    <input name="min" type="number" min="0" max="9" class="timer-min"/>
                    <input name="sec" type="number" min="0" max="59" class="timer-sec"/>
                </form>
                <form id="4" class="timer">
                    <div class="checkbox"></div>
                    <input name="name" class="timer-name"/>
                    <input name="min" type="number" min="0" max="9" class="timer-min"/>
                    <input name="sec" type="number" min="0" max="59" class="timer-sec"/>
                </form>
                <form id="5" class="timer">
                    <div class="checkbox"></div>
                    <input name="name" class="timer-name"/>
                    <input name="min" type="number" min="0" max="9" class="timer-min"/>
                    <input name="sec" type="number" min="0" max="59" class="timer-sec"/>
                </form>
                <form id="6" class="timer">
                    <div class="checkbox"></div>
                    <input name="name" class="timer-name"/>
                    <input name="min" type="number" min="0" max="9" class="timer-min"/>
                    <input name="sec" type="number" min="0" max="59" class="timer-sec"/>
                </form>
                <form id="7" class="timer">
                    <div class="checkbox"></div>
                    <input name="name" class="timer-name"/>
                    <input name="min" type="number" min="0" max="9" class="timer-min"/>
                    <input name="sec" type="number" min="0" max="59" class="timer-sec"/>
                </form>
                <form id="8" class="timer">
                    <div class="checkbox"></div>
                    <input name="name" class="timer-name"/>
                    <input name="min" type="number" min="0" max="9" class="timer-min"/>
                    <input name="sec" type="number" min="0" max="59" class="timer-sec"/>
                </form>
                <form id="9" class="timer">
                    <div class="checkbox"></div>
                    <input name="name" class="timer-name"/>
                    <input name="min" type="number" min="0" max="9" class="timer-min"/>
                    <input name="sec" type="number" min="0" max="59" class="timer-sec"/>
                </form>
            </div>
            <div class="timer-actions">
                <div class="no-timer">Без Таймера</div>
                <div class="erase">Сбросить</div>
            </div>
        </div>
    </main>
    <script> const timerMenuButton = document.querySelector('.timer-button-wrapper');
const timerMenuButtonOpen = document.querySelector('.open-timers');
const timerMenuButtonCLose = document.querySelector('.close-timers');
const timerWrapper = document.querySelector('.timer-wrapper');
const timerCheckboxes = document.querySelectorAll('.checkbox');
const headerInfo = document.querySelector('.header-box');
const time = document.querySelector('.time');
const timerForms = document.querySelectorAll('.timer');
const noTimerButton = document.querySelector('.no-timer');
const eraseTimersButton = document.querySelector('.erase');
const error = document.querySelector('.error');
const errorText = document.querySelector('.error-text');
const button =  document.querySelector('.button');
const statusText = document.querySelector('.status');
const cancelButton = document.querySelector('.cancel');
const voltageBar = document.querySelector('.voltage');

const licenceText = `Соглашение по использованию
Приложение предназначено исключительно для совместного использования с оборудованием серии МАРТ, которое имеет такую возможность. 
Совместное использование не изменяет основных характеристик оборудования и не предоставляет материальной выгоды.
Никакие действия или бездействия приложения не могут быть истолкованы как основания по причинению, какого либо, вреда.
Приложение предоставляется по принципу «как есть».  Для ознакомления возможностей приложения отводится десять часов совместного использования с оборудованием или месяц после установки.
Дальнейшее использование приложения подтверждает, что оно полностью отвечает вашим ожиданиям и удовлетворяет вашим потребностям.
Не допускается вносить любые изменения в приложение, разбирать его код. Нарушение этого пункта приведет к немедленному расторжению соглашения.`

const errorTexts = {
    "cancel-error-default": "Текущий процесс не возможно отменить.",
    "cancel-error-initial": "Прибор находится в изначальном состоянии,\n Отмена не возможна.\n Чтобы продолжить Возьмите внешнее управление\n или включите прибор вручную.",
    "cancel-error-pause": "Прибор находится в состоянии ПАУЗА,\n Отмена не возможна \n Дождитесь конца паузы чтобы продолжить использовать прибор.",
    "cancel-error-working": "При сбросе работы прибора возникла ошибка! \n Чтобы выключить прибор отключитесь от точки доступа.",
    "cancel-error-manage-off": "При сбросе внешнего управления возникла ошибка! \n Чтобы выключить прибор отключитесь от точки доступа.",
    "manage-error-key": "Не возможно взять внешнее управление, пока ключ находится в состоянии ВКЛ.\n Верните ключ в состояние ВЫКЛ чтобы продолжить!"
}

const statuses = ['Нет Связи', 'Готов', 'Поверните ключ', 'Включите', 'Работает', 'Пауза'];
const buttons = [null, 'Взять Управление', null, 'Включить', 'Выключить', null];
const voltageColors = ['#A2E5A9', '#E5DAA2', '#DD6969'];

let timers = {
    activeIndex: null,
    info: [
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: 'Без Таймера', min: null, sec: null}
    ]
}

let state = 0;
let voltage = 0;
let pauseTime = 0;

/**
 * UX/UI Code
 */

let touchStartY = 0;
let errorTimeout = setInterval(()=>{}, 0);

timerWrapper.addEventListener('touchstart', (e)=>{
    touchStartY = e.changedTouches[0].screenY
})

timerWrapper.addEventListener('touchend', (e)=>{
    if(e.changedTouches[0].screenY > touchStartY){
        timerMenuButtonOpen.style.display = 'inherit';
        timerMenuButtonCLose.style.display = 'none';
        timerWrapper.style.top = 'calc(100vh - 170px)';
    }
    else{
        timerMenuButtonOpen.style.display = 'none';
        timerMenuButtonCLose.style.display = 'inherit';
        timerWrapper.style.top = '14px';
    }
})

/**
 * Timers logic
 */

let timerID;
//clearInterval(timer);

timerCheckboxes.forEach((checkbox, index) => {
    checkbox.addEventListener('click', ()=>{
        timerCheckboxes[timers.activeIndex]?.classList.remove('checkbox-active');
        checkbox.classList.add('checkbox-active');
        timers.activeIndex = index;
        setTimerInfo(index);
    })
});

timerForms.forEach((form, index) => {
    form.addEventListener('change', ()=>{
        timers.info[index] = {
            name: form.name.value,
            min: form.min.value < 10 ? form.min.value : 9,
            sec: form.sec.value < 60 ? form.sec.value : 59
        }
        setTimerForms();
        if(index === timers.activeIndex)
            setTimerInfo(index);
        localStorage.setItem('MART-timers', JSON.stringify(timers));
    })
})

noTimerButton.addEventListener('click', ()=>{
    timerCheckboxes[timers.activeIndex]?.classList.remove('checkbox-active');
    timers.activeIndex = null;
    setTimerInfo(10);
})

eraseTimersButton.addEventListener('click', ()=>{
    timerCheckboxes[timers.activeIndex]?.classList.remove('checkbox-active');
    timers.activeIndex = null;
    setTimerInfo(10);
    timers.info.forEach((timer, index) => {
        if(index < 10) timers.info[index] = {name: null, min: null, sec: null};
    })
    localStorage.setItem('MART-timers', timers);
    setTimerForms();
})

error.addEventListener('click', ()=>{
    if(error.classList[1] === 'error-active'){
        error.classList.remove('error-active');
        error.style.display = 'none';
    }
    else
        error.classList.add('error-active');
})

button.addEventListener('click', ()=>{
    let address = 'http://192.168.4.1';
    switch(state){
        case 1:
            address += '/manage-on';
            break;
        case 3:
            if(timers.activeIndex !== null)
                address += `/manage-off?time=${msUtil(timers.info[timers.activeIndex])}`;
            else
                address += '/manage-off';
            break;
        case 4:
            address += '/cancel';
            break;
        default:
            address = null;
            break;
    }
    if(address){
        const request = new XMLHttpRequest();
        request.open("GET", address, false);
        request.onload = () => {
            if(request.readyState === 4 && request.status === 200){
                console.log(request.response);
                if(state === 3 && timers.activeIndex >= 0){
                    setTimer(timers.info[timers.activeIndex].min, timers.info[timers.activeIndex].sec, 4, ()=>{
                            clearInterval(timerID);
                        setTimer(minSecUtil(pauseTime, 'min'), minSecUtil(pauseTime, 'sec'), 5, ()=>{
                            clearInterval(timerID);
                        });
                    });
                }
                else if(state === 3 && timers.activeIndex === -1){
                    setTimer(10, 0, 4, ()=>{
                        clearInterval(timerID)
                        setTimer(minSecUtil(pauseTime, 'min'), minSecUtil(pauseTime, 'sec'), 5, ()=>{
                            clearInterval(timerID);
                        });
                    });
                }
            }
            else if(request.readyState === 4 && request.status === 500){
                setError(errorTexts[request.response]);
            }
        }
        request.send();
    }
    getState();
})

cancelButton.addEventListener('click', ()=>{
    const request = new XMLHttpRequest();
    request.open("GET", 'http://192.168.4.1/cancel', false);
    request.onreadystatechange = () => {
        if(request.readyState === 4 && request.status === 200){
            console.log(request.response);
        }
        else if(request.readyState === 4 && request.status === 500){
            setError(errorTexts[request.response]);
        }
    }
    request.send();
    getState();
});

window.onload = async ()=>{
    if(localStorage.getItem('MART-timers')){
        timers = JSON.parse(localStorage.getItem('MART-timers'));
    }
    setTimerForms();
    setInterval(await getState, 2000);
}

function timeUtil(min, sec){
    if (!min && !sec)
        return '--:--';
    min = min ? `0${min}` : '00';
    sec = sec < 10 ? sec > 0 ? `0${sec}` : '00' : `${sec}`;
    return min + ':' + sec;
}

function msUtil({min, sec}){
    return min * 60000 + sec * 1000;
}

function minSecUtil(time, param){
    if(param === 'min')
        return Math.floor(time / 60000);
    else
        return Math.floor((time / 1000) % 60);
}

function setTimerInfo(index){
    headerInfo.innerHTML = timers.info[index].name || "Безымянный таймер";
    time.innerHTML = timeUtil(timers.info[index].min, timers.info[index].sec);
}

function setTimerForms(){
    timerForms.forEach((form, index)=>{
        form.name.value = timers.info[index].name;
        form.min.value = timers.info[index].min;
        form.sec.value = timers.info[index].sec;
    })
}

function setStatus(){
    statusText.innerText = statuses[state];
    button.innerHTML = buttons[state];
    voltageBar.style.height = `${(voltage / 1500) * 100}%`;
    const colorIndex = voltage < 900 ? voltage > 500 ? 0 : 1 : 2;
    voltageBar.style.background = voltageColors[colorIndex];
    if([0, 2, 5, -1].includes(state)) button.style.display = 'none';
    else button.style.display = 'inherit';

}

function setError(errorMessage){
    error.style.display = 'inherit';
    errorText.innerHTML = errorMessage;
    clearTimeout(errorTimeout);
    errorTimeout = setTimeout(()=>{
        error.style.display = 'none';
        errorText.innerHTML = null;

    }, 20000);
}

function setTimer(min, sec, normalState, action = ()=>{}){
    timerID = setInterval(()=>{
        if(sec > 0){
            sec --;
        }
        else if(min > 0){
            sec = 59;
            min --;
        }
        else{
            setTimerInfo(timers.activeIndex);
            getState().then(action);
            //setTimeout(action, 1000);
        }

        if(state !== normalState){
            setTimerInfo(timers.activeIndex);
            getState().then(action);
            //setTimeout(action, 1000);
        }

        time.innerHTML = timeUtil(min, sec);
    }, 1000);
}

async function getState(){
    return new Promise(function (resolve, reject) {
        makeRequest('GET', 'http://192.168.4.1/state')
            .then(response => JSON.parse(response))
            .then((response) => {
                console.log(response);
                state = +response.state;
                voltage = +response.voltage;
                pauseTime = +response.pausetime;
                setStatus();
                resolve(true);
            })
            .catch((err)=>{
                setError("Потеряна связь с прибором!");
                state = 0;
                voltage = 0;
                pauseTime = 0;
                setStatus();
                resolve(false);
            })
    });
    /*const request = new XMLHttpRequest();
    request.open('GET', 'http://192.168.4.1/state', false);

    request.addEventListener('loadend',  () => {
        if (request.readyState === 4 && request.status === 200) {
            const response = JSON.parse(request.response);
            state = +response.state;
            voltage = +response.voltage;
            pauseTime = +response.pausetime;
        } else {
            setError("Потеряна связь с прибором!");
            state = 0;
            voltage = 0;
            pauseTime = 0;
        }
        setStatus();
    })

    request.addEventListener('error', (error) => {
        setError(error + "\nОтвет не получен. Потеряна связь с прибором!");
        state = 0;
        voltage = 0;
        setStatus();
    })
    request.send();

     */
}

function makeRequest(method, url) {
    return new Promise(function (resolve, reject) {
        let xhr = new XMLHttpRequest();
        xhr.open(method, url);
        xhr.onload = function () {
            if (this.status >= 200 && this.status < 300) {
                resolve(xhr.response);
            } else {
                reject({
                    status: this.status,
                    statusText: xhr.statusText
                });
            }
        };
        xhr.onerror = function () {
            reject({
                status: this.status,
                statusText: xhr.statusText
            });
        };
        xhr.send();
    });
}</script>
</body>
</html>)=====";
}
